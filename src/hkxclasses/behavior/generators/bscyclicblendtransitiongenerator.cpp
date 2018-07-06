#include "bscyclicblendtransitiongenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"

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
    name = "CyclicBlendTransitionGenerator"+QString::number(refCount);
}

QString BSCyclicBlendTransitionGenerator::getClassname(){
    return classname;
}

QString BSCyclicBlendTransitionGenerator::getName() const{
    return name;
}

bool BSCyclicBlendTransitionGenerator::insertObjectAt(int , DataIconManager *obj){
    if (((HkxObject *)obj)->getSignature() == HKB_BLENDER_GENERATOR){
        pBlenderGenerator = HkxSharedPtr((HkxObject *)obj);
        return true;
    }else{
        return false;
    }
}

bool BSCyclicBlendTransitionGenerator::removeObjectAt(int index){
    if (index == 0 || index == -1){
        pBlenderGenerator = HkxSharedPtr();
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

bool BSCyclicBlendTransitionGenerator::isEventReferenced(int eventindex) const{
    if (eventToFreezeBlendValue.id == eventindex || eventToCrossBlend.id == eventindex){
        return true;
    }
    return false;
}

void BSCyclicBlendTransitionGenerator::updateEventIndices(int eventindex){
    if (eventToFreezeBlendValue.id > eventindex){
        eventToFreezeBlendValue.id--;
    }
    if (eventToCrossBlend.id > eventindex){
        eventToCrossBlend.id--;
    }
}

void BSCyclicBlendTransitionGenerator::mergeEventIndex(int oldindex, int newindex){
    if (eventToFreezeBlendValue.id == oldindex){
        eventToFreezeBlendValue.id = newindex;
    }
    if (eventToCrossBlend.id == oldindex){
        eventToCrossBlend.id = newindex;
    }
}

void BSCyclicBlendTransitionGenerator::fixMergedEventIndices(BehaviorFile *dominantfile){
    hkbBehaviorGraphData *recdata;
    hkbBehaviorGraphData *domdata;
    QString thiseventname;
    int eventindex;
    if (!getIsMerged() && dominantfile){
        //TO DO: Support character properties...
        recdata = static_cast<hkbBehaviorGraphData *>(static_cast<BehaviorFile *>(getParentFile())->getBehaviorGraphData());
        domdata = static_cast<hkbBehaviorGraphData *>(dominantfile->getBehaviorGraphData());
        if (recdata && domdata){
            auto fixIndex = [&](int & id){ if (id < 0){return;}
                thiseventname = recdata->getEventNameAt(id);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                id = eventindex;
            };
            fixIndex(eventToFreezeBlendValue.id);
            fixIndex(eventToCrossBlend.id);
            setIsMerged(true);
        }
    }
}

void BSCyclicBlendTransitionGenerator::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (eventToFreezeBlendValue.payload.data()){
        ref++;
        eventToFreezeBlendValue.payload.data()->setReference(ref);
    }
    if (eventToCrossBlend.payload.data()){
        ref++;
        eventToCrossBlend.payload.data()->setReference(ref);
    }
}

bool BSCyclicBlendTransitionGenerator::merge(HkxObject *recessiveObject){
    BSCyclicBlendTransitionGenerator *recobj;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == BS_CYCLIC_BLEND_TRANSITION_GENERATOR){
        recobj = static_cast<BSCyclicBlendTransitionGenerator *>(recessiveObject);
        injectWhileMerging(recobj);
        if (!eventToFreezeBlendValue.payload.data() && recobj->eventToFreezeBlendValue.payload.data()){
            getParentFile()->addObjectToFile(recobj->eventToFreezeBlendValue.payload.data(), -1);
        }
        if (!eventToCrossBlend.payload.data() && recobj->eventToCrossBlend.payload.data()){
            getParentFile()->addObjectToFile(recobj->eventToCrossBlend.payload.data(), -1);
        }
        recobj->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
        return true;
    }else{
        return false;
    }
}

QVector<HkxObject *> BSCyclicBlendTransitionGenerator::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (eventToFreezeBlendValue.payload.data()){
        list.append(eventToFreezeBlendValue.payload.data());
    }
    if (eventToCrossBlend.payload.data()){
        list.append(eventToCrossBlend.payload.data());
    }
    return list;
}

QList<DataIconManager *> BSCyclicBlendTransitionGenerator::getChildren() const{
    QList<DataIconManager *> list;
    if (pBlenderGenerator.data()){
        list.append(static_cast<DataIconManager*>(pBlenderGenerator.data()));
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
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pBlenderGenerator"){
            if (!pBlenderGenerator.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'pBlenderGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "EventToFreezeBlendValue"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToFreezeBlendValue.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'eventToFreezeBlendValueId' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToFreezeBlendValue.payload.readShdPtrReference(index, reader)){
                        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'eventToFreezeBlendValuePayload' reference!\nObject Reference: "+ref);
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
                    eventToCrossBlend.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'eventToCrossBlendId' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToCrossBlend.payload.readShdPtrReference(index, reader)){
                        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'eventToCrossBlendPayload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "fBlendParameter"){
            fBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'fBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fTransitionDuration"){
            fTransitionDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'fTransitionDuration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eBlendCurve"){
            eBlendCurve = reader.getElementValueAt(index);
            if (eBlendCurve == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'eBlendCurve' data field!\nObject Reference: "+ref);
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("EventToFreezeBlendValue"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(eventToFreezeBlendValue.id));
        if (eventToFreezeBlendValue.payload.data()){
            refString = eventToFreezeBlendValue.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("EventToCrossBlend"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(eventToCrossBlend.id));
        if (eventToCrossBlend.payload.data()){
            refString = eventToCrossBlend.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fBlendParameter"), QString::number(fBlendParameter, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fTransitionDuration"), QString::number(fTransitionDuration, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eBlendCurve"), eBlendCurve);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (pBlenderGenerator.data() && !pBlenderGenerator.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'pBlenderGenerator'!!!");
        }
        if (eventToFreezeBlendValue.payload.data() && !eventToFreezeBlendValue.payload.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'eventToFreezeBlendValuePayload'!!!");
        }
        if (eventToCrossBlend.payload.data() && !eventToCrossBlend.payload.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'eventToCrossBlendPayload'!!!");
        }
    }
    return true;
}

bool BSCyclicBlendTransitionGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(pBlenderGenerator.getShdPtrReference());
    if (!ptr){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'pBlenderGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\n'pBlenderGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pBlenderGenerator = *ptr;
    }else{
        pBlenderGenerator = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToFreezeBlendValue.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        eventToFreezeBlendValue.payload = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToCrossBlend.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        eventToCrossBlend.payload = *ptr;
    }
    return true;
}

void BSCyclicBlendTransitionGenerator::unlink(){
    HkDynamicObject::unlink();
    pBlenderGenerator = HkxSharedPtr();
    eventToFreezeBlendValue.payload = HkxSharedPtr();
    eventToCrossBlend.payload = HkxSharedPtr();
}

QString BSCyclicBlendTransitionGenerator::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (eventToFreezeBlendValue.id >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": eventToFreezeBlendValue event id out of range! Setting to max index in range!\n");
        eventToFreezeBlendValue.id = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (eventToCrossBlend.id >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": eventToCrossBlend event id out of range! Setting to max index in range!\n");
        eventToCrossBlend.id = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (eventToFreezeBlendValue.payload.data() && eventToFreezeBlendValue.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid eventToFreezeBlendValue.payload type! Signature: "+QString::number(eventToFreezeBlendValue.payload.data()->getSignature(), 16)+" Setting null value!\n");
        eventToFreezeBlendValue.payload = HkxSharedPtr();
    }
    if (eventToCrossBlend.payload.data() && eventToCrossBlend.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid eventToCrossBlend.payload type! Signature: "+QString::number(eventToCrossBlend.payload.data()->getSignature(), 16)+" Setting null value!\n");
        eventToCrossBlend.payload = HkxSharedPtr();
    }
    if (!pBlenderGenerator.data()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null pBlenderGenerator!\n");
    }else if (pBlenderGenerator.data()->getSignature() != HKB_BLENDER_GENERATOR){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid pBlenderGenerator type! Signature: "+QString::number(pBlenderGenerator.data()->getSignature(), 16)+" Setting null value!\n");
        pBlenderGenerator = HkxSharedPtr();
    }
    setDataValidity(isvalid);
    return errors;
}

BSCyclicBlendTransitionGenerator::~BSCyclicBlendTransitionGenerator(){
    refCount--;
}
