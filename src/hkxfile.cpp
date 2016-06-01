#include "hkxfile.h"
#include "hkobject.h"
//#include "utility.h"
#include "hkxxmlreader.h"
#include "modifiers.h"

template <typename T>
bool BehaviorFile::appendAndReadData(int index, T * obj){
    bool ok;
    QByteArray temp = reader.getNthAttributeValueAt(index, 0);
    if (temp.at(0) == '#'){
        temp.remove(0, 1);
    }
    if (obj->getType() == HkObject::TYPE_GENERATOR){
        generators.append(HkObjectExpSharedPtr(reinterpret_cast<hkbGenerator*>(obj), temp.toLong(&ok)));
    }else if (obj->getType() == HkObject::TYPE_MODIFIER){
        modifiers.append(HkObjectExpSharedPtr(reinterpret_cast<hkbModifier*>(obj), temp.toLong(&ok)));
    }else if (obj->getType() == HkObject::TYPE_OTHER){
        otherTypes.append(HkObjectExpSharedPtr(obj, temp.toLong(&ok)));
    }else{
        return false;
    }
    if (!ok){
        return false;
    }
    index++;    //Skip the current line.
    if (!obj->readData(reader, index)){
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
    while (index < reader.getNumElements()){
        value = reader.getNthAttributeNameAt(index, 1);
        if (value == "class"){
            value = reader.getNthAttributeValueAt(index, 2);
            if (value != ""){
                signature = value.toULongLong(&ok, 16);
                if (!ok){
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
                }else if (signature == HKB_MODIFIER_GENERATOR){
                    if (!appendAndReadData(index, new hkbModifierGenerator(this))){
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
                }else if (signature == HKB_BLENDER_GENERATOR){
                    if (!appendAndReadData(index, new hkbBlenderGenerator(this))){
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
                }if (signature == HK_ROOT_LEVEL_CONTAINER){
                    if (!appendAndReadData(index, new hkRootLevelContainer(this))){
                        return false;
                    }
                    setRootObject(otherTypes.last());
                }else{
                    //return false;
                }
            }
        }
        index++;
    }
    closeFile();
    link();
    return true;
}

bool BehaviorFile::link(){
    if (!getRootObject().constData() || getRootObject()->getSignature() != HK_ROOT_LEVEL_CONTAINER){
        return false;
    }
    if (!static_cast<hkRootLevelContainer *>(getRootObject().data())->link()){
        return false;
    }
    return true;
}

HkObjectExpSharedPtr * BehaviorFile::findGenerator(long ref){
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

HkObjectExpSharedPtr * BehaviorFile::findModifier(long ref){
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

HkObjectExpSharedPtr * BehaviorFile::findHkObject(long ref){
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




























