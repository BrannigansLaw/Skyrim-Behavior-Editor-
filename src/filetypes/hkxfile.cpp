#include "hkxfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/xml/hkxxmlreader.h"
//#include "src/hkxclasses/modifiers.h"
#include "src/hkxclasses/generators/hkbgenerator.h"
#include "src/ui/mainwindow.h"
#include "src/hkxclasses/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/generators/hkbstatemachine.h"
#include "src/hkxclasses/generators/hkbmodifiergenerator.h"
#include "src/hkxclasses/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/generators/bscyclicblendtransitiongenerator.h"
#include "src/hkxclasses/generators/bsboneswitchgenerator.h"
#include "src/hkxclasses/generators/bsboneswitchgeneratorbonedata.h"
#include "src/hkxclasses/generators/bssynchronizedclipgenerator.h"
#include "src/hkxclasses/generators/hkbmanualselectorgenerator.h"
#include "src/hkxclasses/generators/hkbblendergeneratorchild.h"
#include "src/hkxclasses/hkbboneweightarray.h"
#include "src/hkxclasses/generators/hkbblendergenerator.h"
#include "src/hkxclasses/generators/bsoffsetanimationgenerator.h"
#include "src/hkxclasses/generators/hkbposematchinggenerator.h"
#include "src/hkxclasses/generators/hkbclipgenerator.h"
#include "src/hkxclasses/generators/hkbbehaviorreferencegenerator.h"
#include "src/hkxclasses/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/hkbstatemachineeventpropertyarray.h"
#include "src/hkxclasses/generators/hkbbehaviorgraph.h"
#include "src/hkxclasses/hkbbehaviorgraphdata.h"
#include "src/hkxclasses/hkbbehaviorgraphstringdata.h"
#include "src/hkxclasses/hkbvariablevalueset.h"
#include "src/hkxclasses/hkbvariablebindingset.h"
#include "src/hkxclasses/hkrootlevelcontainer.h"

/**
 * Class: HkxFile
 *
 */

HkxFile::HkxFile(MainWindow *window, const QString & name)
    : QFile(name),
      ui(window)
{
    parse();
}

void HkxFile::closeFile(){
    if (isOpen()){
        close();
    }
}

HkxObjectExpSharedPtr & HkxFile::getRootObject(){
    return rootObject;
}

bool HkxFile::parse(){
    return true;
}

bool HkxFile::link(){
    return true;
}

void HkxFile::setRootObject(HkxObjectExpSharedPtr & obj){
    rootObject = obj;
}

MainWindow * HkxFile::getUI(){
    return ui;
}

void HkxFile::writeToLog(const QString & message, bool isError){
    if (ui){
        ui->writeToLog(message, isError);
    }
}

void HkxFile::setProgressData(const QString & message, int value){
    ui->setProgressData(message, value);
}

HkxFile::~HkxFile(){
    //
}

/**
 * Class: BehaviorFile
 *
 */

BehaviorFile::BehaviorFile(MainWindow *window, const QString & name): HkxFile(window, name){
    reader.setBehaviorFile(this);
    if (!parse()){
        writeToLog("MainWindow: parse() failed!");
        getUI()->drawGraph = false;
    }
    reader.clear();
}

template <typename T>
bool BehaviorFile::appendAndReadData(int index, T * obj){
    QByteArray temp = reader.getNthAttributeValueAt(index, 0);
    if (temp.at(0) == '#'){
        temp.remove(0, 1);
    }
    if (!addObjectToFile(obj, temp)){
        writeToLog("BehaviorFile: appendAndReadData() failed!\nThe object reference string contained invalid characters and failed to convert to an integer!", true);
        return false;
    }
    //Skip the current line.
    index++;
    if (!obj->readData(reader, index)){
        return false;
    }
    return true;
}

template <typename T>
bool BehaviorFile::addObjectToFile(T *obj, const QByteArray & temp){
    bool ok;
    if (obj->getType() == HkxObject::TYPE_GENERATOR){
        generators.append(HkxObjectExpSharedPtr(obj, temp.toLong(&ok)));
    }else if (obj->getType() == HkxObject::TYPE_MODIFIER){
        modifiers.append(HkxObjectExpSharedPtr(obj, temp.toLong(&ok)));
    }else if (obj->getType() == HkxObject::TYPE_OTHER){
        otherTypes.append(HkxObjectExpSharedPtr(obj, temp.toLong(&ok)));
    }else{
        writeToLog("BehaviorFile: addObjectToFile() failed!\nInvalid type enum for this object!\nObject signature is: "+QString::number(obj->getSignature(), 16), true);
        return false;
    }
    if (!ok){
        return false;
    }
    return true;
}

bool BehaviorFile::parse(){
    if (!reader.parse()){
        return false;
    }
    int index = 2;
    bool ok = true;
    qulonglong signature;
    QByteArray value;
    setProgressData("Creating HKX objects...", 60);
    while (index < reader.getNumElements()){
        value = reader.getNthAttributeNameAt(index, 1);
        if (value == "class"){
            value = reader.getNthAttributeValueAt(index, 2);
            if (value != ""){
                signature = value.toULongLong(&ok, 16);
                if (!ok){
                    writeToLog("BehaviorFile: parse() failed!\nThe object signature string contained invalid characters and failed to convert to an integer!", true);
                    return false;
                }
                if (signature == HKB_STATE_MACHINE_STATE_INFO){
                    if (!appendAndReadData(index, new hkbStateMachineStateInfo(this))){
                        return false;
                    }
                }else if (signature == HKB_STATE_MACHINE){
                    if (!appendAndReadData(index, new hkbStateMachine(this))){
                        return false;
                    }
                }else if (signature == HKB_VARIABLE_BINDING_SET){
                    if (!appendAndReadData(index, new hkbVariableBindingSet(this))){
                        return false;
                    }
                }else if (signature == HKB_MODIFIER_GENERATOR){
                    if (!appendAndReadData(index, new hkbModifierGenerator(this))){
                        return false;
                    }
                }else if (signature == BS_I_STATE_TAGGING_GENERATOR){
                    if (!appendAndReadData(index, new BSiStateTaggingGenerator(this))){
                        return false;
                    }
                }else if (signature == BS_CYCLIC_BLEND_TRANSITION_GENERATOR){
                    if (!appendAndReadData(index, new BSCyclicBlendTransitionGenerator(this))){
                        return false;
                    }
                }else if (signature == BS_BONE_SWITCH_GENERATOR){
                    if (!appendAndReadData(index, new BSBoneSwitchGenerator(this))){
                        return false;
                    }
                }else if (signature == BS_BONE_SWITCH_GENERATOR_BONE_DATA){
                    if (!appendAndReadData(index, new BSBoneSwitchGeneratorBoneData(this))){
                        return false;
                    }
                }else if (signature == BS_SYNCHRONIZED_CLIP_GENERATOR){
                    if (!appendAndReadData(index, new BSSynchronizedClipGenerator(this))){
                        return false;
                    }
                }else if (signature == HKB_MANUAL_SELECTOR_GENERATOR){
                    if (!appendAndReadData(index, new hkbManualSelectorGenerator(this))){
                        return false;
                    }
                }else if (signature == HKB_BLENDER_GENERATOR_CHILD){
                    if (!appendAndReadData(index, new hkbBlenderGeneratorChild(this))){
                        return false;
                    }
                }else if (signature == HKB_BONE_WEIGHT_ARRAY){
                    if (!appendAndReadData(index, new hkbBoneWeightArray(this))){
                        return false;
                    }
                }else if (signature == HKB_BLENDER_GENERATOR){
                    if (!appendAndReadData(index, new hkbBlenderGenerator(this))){
                        return false;
                    }
                }else if (signature == BS_OFFSET_ANIMATION_GENERATOR){
                    if (!appendAndReadData(index, new BSOffsetAnimationGenerator(this))){
                        return false;
                    }
                }else if (signature == HKB_POSE_MATCHING_GENERATOR){
                    if (!appendAndReadData(index, new hkbPoseMatchingGenerator(this))){
                        return false;
                    }
                }else if (signature == HKB_CLIP_GENERATOR){
                    if (!appendAndReadData(index, new hkbClipGenerator(this))){
                        return false;
                    }
                }else if (signature == HKB_BEHAVIOR_REFERENCE_GENERATOR){
                    if (!appendAndReadData(index, new hkbBehaviorReferenceGenerator(this))){
                        return false;
                    }
                }else if (signature == HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
                    if (!appendAndReadData(index, new hkbStateMachineTransitionInfoArray(this))){
                        return false;
                    }
                }else if (signature == HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
                    if (!appendAndReadData(index, new hkbStateMachineEventPropertyArray(this))){
                        return false;
                    }
                }else if (signature == HKB_BEHAVIOR_GRAPH){
                    if (!appendAndReadData(index, new hkbBehaviorGraph(this))){
                        return false;
                    }
                    behaviorGraph = generators.last();
                    generators.removeLast();
                }else if (signature == HKB_BEHAVIOR_GRAPH_DATA){
                    if (!appendAndReadData(index, new hkbBehaviorGraphData(this))){
                        return false;
                    }
                    graphData = otherTypes.last();
                    otherTypes.removeLast();
                }else if (signature == HKB_BEHAVIOR_GRAPH_STRING_DATA){
                    if (!appendAndReadData(index, new hkbBehaviorGraphStringData(this))){
                        return false;
                    }
                    stringData = otherTypes.last();
                    otherTypes.removeLast();
                }else if (signature == HKB_VARIABLE_VALUE_SET){
                    if (!appendAndReadData(index, new hkbVariableValueSet(this))){
                        return false;
                    }
                    variableValues = otherTypes.last();
                    otherTypes.removeLast();
                }else if (signature == HK_ROOT_LEVEL_CONTAINER){
                    if (!appendAndReadData(index, new hkRootLevelContainer(this))){
                        return false;
                    }
                    setRootObject(otherTypes.last());
                    otherTypes.removeLast();
                }else{
                    writeToLog("BehaviorFile: parse()!\nUnknown signature detected!\nUnknown object class name is: "+reader.getNthAttributeValueAt(index, 1)+"\nUnknown object signature is: "+QString::number(signature, 16));
                }
            }
        }
        index++;
    }
    closeFile();
    setProgressData("Linking HKX objects...", 80);
    if (!link()){
        writeToLog("BehaviorFile: parse() failed because link() failed!", true);
        return false;
    }
    removeUnneededGenerators();
    return true;
}

bool BehaviorFile::link(){
    if (!getRootObject().constData()){
        writeToLog("BehaviorFile: link() failed!\nThe root object of this behavior file is NULL!", true);
        return false;
    }else if (getRootObject()->getSignature() != HK_ROOT_LEVEL_CONTAINER){
        writeToLog("BehaviorFile: link() failed!\nThe root object of this behavior file is NOT a hkRootLevelContainer!\nThe root object signature is: "+QString::number(getRootObject()->getSignature(), 16), true);
        return false;
    }
    if (!static_cast<hkRootLevelContainer * >(getRootObject().data())->link()){
        writeToLog("BehaviorFile: link() failed!\nThe root object of this behavior file failed to link to it's children!", true);
        return false;
    }
    for (int i = generators.size() - 1; i >= 0; i--){
        if (!static_cast<hkbGenerator * >(generators.at(i).data())->link()){
            writeToLog("BehaviorFile: link() failed!\nA generator failed to link to it's children!\nObject signature: "+QString::number(generators.at(i)->getSignature(), 16)+"\nObject reference: "+QString::number(generators.at(i).getReference()), true);
            return false;
        }
    }
    if (!static_cast<hkbBehaviorGraph * >(behaviorGraph.data())->link()){
        writeToLog("BehaviorFile: link() failed!\nhkbBehaviorGraph failed to link to it's children!\n", true);
        return false;
    }
    if (!static_cast<hkbVariableValueSet * >(variableValues.data())->link()){
        writeToLog("BehaviorFile: link() failed!\nhkbVariableValueSet failed to link to it's children!\n", true);
        return false;
    }
    if (!static_cast<hkbBehaviorGraphData * >(graphData.data())->link()){
        writeToLog("BehaviorFile: link() failed!\nhkbBehaviorGraphData failed to link to it's children!\n", true);
        return false;
    }
    return true;
}

HkxObjectExpSharedPtr * BehaviorFile::findGenerator(long ref){
    if (ref < 0){
        return NULL;
    }
    for (int i = 0; i < generators.size(); i++){
        if (ref == generators.at(i).getReference()){
            return &generators[i];
        }
    }
    return NULL;
}

HkxObjectExpSharedPtr * BehaviorFile::findModifier(long ref){
    if (ref < 0){
        return NULL;
    }
    for (int i = 0; i < modifiers.size(); i++){
        if (ref == modifiers.at(i).getReference()){
            return &modifiers[i];
        }
    }
    return NULL;
}

HkxObjectExpSharedPtr * BehaviorFile::findHkxObject(long ref){
    if (ref < 0){
        return NULL;
    }
    for (int i = 0; i < otherTypes.size(); i++){
        if (ref == otherTypes.at(i).getReference()){
            return &otherTypes[i];
        }
    }
    return NULL;
}

void BehaviorFile::removeBindings(int varIndex){
    for (int i = 0; i < otherTypes.size(); i++){
        if (otherTypes.at(i).data()->getSignature() == HKB_VARIABLE_BINDING_SET){
            static_cast<hkbVariableBindingSet *>(otherTypes.at(i).data())->removeBinding(varIndex);
        }
    }
}

HkxObjectExpSharedPtr * BehaviorFile::findBehaviorGraph(long ref){
    if (behaviorGraph.getReference() == ref){
        return &behaviorGraph;
    }
    return NULL;
}

QVector<int> BehaviorFile::removeGeneratorData(){
    QVector<int> removedIndices;
    for (int i = generators.size() - 1; i >= 0; i--){
        if (generators.at(i).constData() && generators.at(i).constData()->ref < 2){
            generators.removeAt(i);
            removedIndices.append(i);
        }
    }
    return removedIndices;
}

QVector<int> BehaviorFile::removeModifierData(){
    QVector<int> removedIndices;
    for (int i = modifiers.size() - 1; i >= 0; i--){
        if (modifiers.at(i).constData() && modifiers.at(i).constData()->ref < 2){
            modifiers.removeAt(i);
            removedIndices.append(i);
        }
    }
    return removedIndices;
}

QVector<int> BehaviorFile::removeOtherData(){
    QVector<int> removedIndices;
    for (int i = otherTypes.size() - 1; i >= 0; i--){
        if (otherTypes.at(i).constData() && otherTypes.at(i).constData()->ref < 2){
            otherTypes.removeAt(i);
            removedIndices.append(i);
        }
    }
    return removedIndices;
}

hkVariableType BehaviorFile::getVariableTypeAt(int index) const{
    return static_cast<hkbBehaviorGraphData *>(graphData.data())->getVariableTypeAt(index);
}

QStringList BehaviorFile::getGeneratorNames(){
    QStringList list;
    qulonglong sig;
    for (int i = 0; i < generators.size(); i++){
        if (generators.at(i).constData()->getType() == HkxObject::TYPE_GENERATOR){
            list.append(static_cast<hkbGenerator *>(generators.at(i).data())->getName());
            sig = generators.at(i).constData()->getSignature();
            switch (sig){
            case HKB_STATE_MACHINE:
                list.last().append(" Type: hkbStateMachine");
                break;
            case HKB_MANUAL_SELECTOR_GENERATOR:
                list.last().append(" Type: hkbManualSelectorGenerator");
                break;
            case HKB_BLENDER_GENERATOR:
                list.last().append(" Type: hkbBlenderGenerator");
                break;
            case BS_I_STATE_TAGGING_GENERATOR:
                list.last().append(" Type: BSiStateTaggingGenerator");
                break;
            case BS_BONE_SWITCH_GENERATOR:
                list.last().append(" Type: BSBoneSwitchGenerator");
                break;
            case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
                list.last().append(" Type: BSCyclicBlendTransitionGenerator");
                break;
            case BS_SYNCHRONIZED_CLIP_GENERATOR:
                list.last().append(" Type: BSSynchronizedClipGenerator");
                break;
            case HKB_MODIFIER_GENERATOR:
                list.last().append(" Type: hkbModifierGenerator");
                break;
            case BS_OFFSET_ANIMATION_GENERATOR:
                list.last().append(" Type: BSOffsetAnimationGenerator");
                break;
            case HKB_POSE_MATCHING_GENERATOR:
                list.last().append(" Type: hkbPoseMatchingGenerator");
                break;
            case HKB_CLIP_GENERATOR:
                list.last().append(" Type: hkbClipGenerator");
                break;
            case HKB_BEHAVIOR_REFERENCE_GENERATOR:
                list.last().append(" Type: hkbBehaviorReferenceGenerator");
                break;
            default:
                writeToLog("BehaviorFile: getGeneratorNames() failed!\n'generators' contains an invalid type!\n", true);
                break;
            }
        }
    }
    return list;
}

QStringList BehaviorFile::getModifierNames(){
    QStringList list;
    /*qulonglong sig;
    for (int i = 0; i < modifiers.size(); i++){
        if (modifiers.at(i).constData()->getType() == HkxObject::TYPE_MODIFIER){
            list.append("Name: "+static_cast<hkbModifier *>(modifiers.at(i).data())->getName());
            sig = modifiers.at(i).constData()->getSignature();
            switch (sig){
            case HKB_STATE_MACHINE:
                list.last().append(" Type: hkbStateMachine");
                break;
            case HKB_MANUAL_SELECTOR_GENERATOR:
                list.last().append(" Type: hkbManualSelectorGenerator");
                break;
            case HKB_BLENDER_GENERATOR:
                list.last().append(" Type: hkbBlenderGenerator");
                break;
            case BS_I_STATE_TAGGING_GENERATOR:
                list.last().append(" Type: BSiStateTaggingGenerator");
                break;
            case BS_BONE_SWITCH_GENERATOR:
                list.last().append(" Type: BSBoneSwitchGenerator");
                break;
            case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
                list.last().append(" Type: BSCyclicBlendTransitionGenerator");
                break;
            case BS_SYNCHRONIZED_CLIP_GENERATOR:
                list.last().append(" Type: BSSynchronizedClipGenerator");
                break;
            case HKB_MODIFIER_GENERATOR:
                list.last().append(" Type: hkbModifierGenerator");
                break;
            case BS_OFFSET_ANIMATION_GENERATOR:
                list.last().append(" Type: BSOffsetAnimationGenerator");
                break;
            case HKB_POSE_MATCHING_GENERATOR:
                list.last().append(" Type: hkbPoseMatchingGenerator");
                break;
            case HKB_CLIP_GENERATOR:
                list.last().append(" Type: hkbClipGenerator");
                break;
            default:
                writeToLog("BehaviorFile: getGeneratorNames() failed!\n'generators' contains an invalid type!\n", true);
                break;
            }
        }
    }*/
    return list;
}

HkxObject * BehaviorFile::getBehaviorGraphData() const{
    return graphData.data();
}

int BehaviorFile::getIndexOfGenerator(const HkxObjectExpSharedPtr & obj) const{
    return generators.indexOf(obj);
}

bool BehaviorFile::setGeneratorData(HkxObjectExpSharedPtr & ptrToSet, int index){
    if (index >= 0 && index < generators.size()){
        ptrToSet = generators.at(index);
        return true;
    }
    return false;
}

HkxObject * BehaviorFile::getGeneratorDataAt(int index){
    if (index >= 0 && index < generators.size()){
        return generators[index].data();
    }
    return NULL;
}

int BehaviorFile::getIndexOfModifier(const HkxObjectExpSharedPtr & obj) const{
    return modifiers.indexOf(obj);
}

bool BehaviorFile::setModifierData(HkxObjectExpSharedPtr & ptrToSet, int index){
    if (index >= 0 && index < modifiers.size()){
        ptrToSet = modifiers.at(index);
        return true;
    }
    return false;
}

HkxObject* BehaviorFile::getModifierDataAt(int index){
    if (index >= 0 && index < modifiers.size()){
        return modifiers[index].data();
    }
    return NULL;
}

QStringList BehaviorFile::getVariableNames() const{
    return static_cast<hkbBehaviorGraphData *>(graphData.data())->getVariableNames();
}

QStringList BehaviorFile::getEventNames() const{
    return static_cast<hkbBehaviorGraphData *>(graphData.data())->getEventNames();
}

void BehaviorFile::removeUnneededGenerators(){
    for (int i = generators.size() - 1; i >= 0; i--){
        if (generators.at(i).constData()->getSignature() == HKB_STATE_MACHINE_STATE_INFO ||
            generators.at(i).constData()->getSignature() == HKB_BLENDER_GENERATOR_CHILD ||
            generators.at(i).constData()->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA ||
            generators.at(i).constData()->getSignature() == HKB_BONE_WEIGHT_ARRAY
           )
        {
            generators.removeAt(i);
        }
    }
}

BehaviorFile::~BehaviorFile(){
    //
}
