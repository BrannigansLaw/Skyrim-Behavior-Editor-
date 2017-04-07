#include "bscyclicblendtransitiongenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: BSCyclicBlendTransitionGenerator
*/

uint BSCyclicBlendTransitionGenerator::refCount = 0;

QString BSCyclicBlendTransitionGenerator::classname = "BSCyclicBlendTransitionGenerator";

QStringList BSCyclicBlendTransitionGenerator::BlendCurve = {"BLEND_CURVE_SMOOTH", "BLEND_CURVE_LINEAR", "BLEND_CURVE_LINEAR_TO_SMOOTH", "BLEND_CURVE_SMOOTH_TO_LINEAR"};

BSCyclicBlendTransitionGenerator::BSCyclicBlendTransitionGenerator(HkxFile *parent, long ref)
    :hkbGenerator(parent, ref),
    userData(0),
    fBlendParameter(1),
    fTransitionDuration(0),
    eBlendCurve(BlendCurve.first())
{
    setType(BS_CYCLIC_BLEND_TRANSITION_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "BSCyclicBlendTransitionGenerator"+QString::number(refCount);
}

QString BSCyclicBlendTransitionGenerator::getClassname(){
    return classname;
}

QString BSCyclicBlendTransitionGenerator::getName() const{
    return name;
}

bool BSCyclicBlendTransitionGenerator::insertObjectAt(int index, DataIconManager *obj){
    if (((HkxObject *)obj)->getSignature() == HKB_BLENDER_GENERATOR){
        if (index == 0){
            pBlenderGenerator = HkxObjectExpSharedPtr((HkxObject *)obj);
        }else{
            return false;
        }
        return true;
    }else{
        return false;
    }
}

bool BSCyclicBlendTransitionGenerator::removeObjectAt(int index){
    if (index == 0){
        pBlenderGenerator = HkxObjectExpSharedPtr();
    }else{
        return false;
    }
    return true;
}

bool BSCyclicBlendTransitionGenerator::hasChildren() const{
    if (pBlenderGenerator.data()){
        return true;
    }
    return false;
}

QList<DataIconManager *> BSCyclicBlendTransitionGenerator::getChildren() const{
    QList<DataIconManager *> list;
    if (pBlenderGenerator.data()){
        list.append((DataIconManager *)pBlenderGenerator.data());
    }
    return list;
}

int BSCyclicBlendTransitionGenerator::getIndexOfObj(DataIconManager *obj) const{
    if (pBlenderGenerator.data() == (HkxObject *)obj){
        return 0;
    }
    return -1;
}

bool BSCyclicBlendTransitionGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pBlenderGenerator"){
            if (!pBlenderGenerator.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'pBlenderGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "EventToFreezeBlendValue"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToFreezeBlendValueId = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventToFreezeBlendValueId' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToFreezeBlendValuePayload.readReference(index, reader)){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventToFreezeBlendValuePayload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "EventToCrossBlend"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToCrossBlendId = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventToCrossBlendId' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToCrossBlendPayload.readReference(index, reader)){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventToCrossBlendPayload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "fBlendParameter"){
            fBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'fBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fTransitionDuration"){
            fTransitionDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'fTransitionDuration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eBlendCurve"){
            eBlendCurve = reader.getElementValueAt(index);
            if (eBlendCurve == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'eBlendCurve' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSCyclicBlendTransitionGenerator::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString = "null";
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        if (variableBindingSet.data()){
            refString = variableBindingSet.data()->getReferenceString();
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("variableBindingSet"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("userData"), QString::number(userData));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("name"), name);
        if (pBlenderGenerator.data()){
            refString = pBlenderGenerator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pBlenderGenerator"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eventToFreezeBlendValue"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(eventToFreezeBlendValueId));
        if (eventToFreezeBlendValuePayload.data()){
            refString = eventToFreezeBlendValuePayload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eventToCrossBlend"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(eventToCrossBlendId));
        if (eventToCrossBlendPayload.data()){
            refString = eventToCrossBlendPayload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fBlendParameter"), QString::number(fBlendParameter));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fTransitionDuration"), QString::number(fTransitionDuration));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fBlendParameter"), eBlendCurve);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (pBlenderGenerator.data() && !pBlenderGenerator.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'pBlenderGenerator'!!!", true);
        }
        if (eventToFreezeBlendValuePayload.data() && !eventToFreezeBlendValuePayload.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'eventToFreezeBlendValuePayload'!!!", true);
        }
        if (eventToCrossBlendPayload.data() && !eventToCrossBlendPayload.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'eventToCrossBlendPayload'!!!", true);
        }
    }
    return true;
}

bool BSCyclicBlendTransitionGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxObjectExpSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(pBlenderGenerator.getReference());
    if (!ptr){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'pBlenderGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR){
        writeToLog(getClassname()+": link()!\n'pBlenderGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pBlenderGenerator = *ptr;
    }else{
        pBlenderGenerator = *ptr;
    }
    return true;
}

void BSCyclicBlendTransitionGenerator::unlink(){
    HkDynamicObject::unlink();
    pBlenderGenerator = HkxObjectExpSharedPtr();
    eventToFreezeBlendValuePayload = HkxObjectExpSharedPtr();
    eventToCrossBlendPayload = HkxObjectExpSharedPtr();
}

bool BSCyclicBlendTransitionGenerator::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (!pBlenderGenerator.data() || pBlenderGenerator.data()->getSignature() != HKB_BLENDER_GENERATOR){
    }else if (eventToFreezeBlendValuePayload.data() && eventToFreezeBlendValuePayload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else if (eventToCrossBlendPayload.data() && eventToCrossBlendPayload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else if (!BlendCurve.contains(eBlendCurve)){
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSCyclicBlendTransitionGenerator::~BSCyclicBlendTransitionGenerator(){
    refCount--;
}
