#include "bsragdollcontactlistenermodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"

/*
 * CLASS: BSRagdollContactListenerModifier
*/

uint BSRagdollContactListenerModifier::refCount = 0;

QString BSRagdollContactListenerModifier::classname = "BSRagdollContactListenerModifier";

BSRagdollContactListenerModifier::BSRagdollContactListenerModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true)
{
    setType(BS_RAGDOLL_CONTACT_LISTENER_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "RagdollContactListenerModifier"+QString::number(refCount);
}

QString BSRagdollContactListenerModifier::getClassname(){
    return classname;
}

QString BSRagdollContactListenerModifier::getName() const{
    return name;
}

bool BSRagdollContactListenerModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "id"){
            contactEvent.id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
            }
        }else if (text == "payload"){
            if (!contactEvent.payload.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
            }
        }else if (text == "bones"){
            if (!bones.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'bones' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSRagdollContactListenerModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enable"), getBoolAsString(enable));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("event"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(contactEvent.id));
        if (contactEvent.payload.data()){
            refString = contactEvent.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        if (bones.data()){
            refString = bones.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bones"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (contactEvent.payload.data() && !contactEvent.payload.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'payload'!!!");
        }
        if (bones.data() && !bones.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'bones'!!!");
        }
    }
    return true;
}

bool BSRagdollContactListenerModifier::isEventReferenced(int eventindex) const{
    if (contactEvent.id == eventindex){
        return true;
    }
    return false;
}

void BSRagdollContactListenerModifier::updateEventIndices(int eventindex){
    if (contactEvent.id > eventindex){
        contactEvent.id--;
    }
}

void BSRagdollContactListenerModifier::mergeEventIndex(int oldindex, int newindex){
    if (contactEvent.id == oldindex){
        contactEvent.id = newindex;
    }
}

void BSRagdollContactListenerModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
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
            fixIndex(contactEvent.id);
            setIsMerged(true);
        }
    }
}

void BSRagdollContactListenerModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (contactEvent.payload.data()){
        ref++;
        contactEvent.payload.data()->updateReferences(ref);
    }
    if (bones.data()){
        ref++;
        bones.data()->updateReferences(ref);
    }
}

QVector<HkxObject *> BSRagdollContactListenerModifier::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (contactEvent.payload.data()){
        list.append(contactEvent.payload.data());
    }
    if (bones.data()){
        list.append(bones.data());
    }
    return list;
}

bool BSRagdollContactListenerModifier::merge(HkxObject *recessiveObject){
    BSRagdollContactListenerModifier *recobj;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == BS_RAGDOLL_CONTACT_LISTENER_MODIFIER){
        recobj = static_cast<BSRagdollContactListenerModifier *>(recessiveObject);
        injectWhileMerging(recobj);
        if (!contactEvent.payload.data() && recobj->contactEvent.payload.data()){
            getParentFile()->addObjectToFile(recobj->contactEvent.payload.data(), -1);
        }
        if (!bones.data() && recobj->bones.data()){
            recobj->bones.data()->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
            getParentFile()->addObjectToFile(recobj->bones.data(), -1);
        }
        return true;
    }else{
        return false;
    }
}

bool BSRagdollContactListenerModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(contactEvent.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        contactEvent.payload = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(bones.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_INDEX_ARRAY){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'bones' is not a HKB_BONE_INDEX_ARRAY!");
            setDataValidity(false);
        }
        bones = *ptr;
    }
    return true;
}

void BSRagdollContactListenerModifier::unlink(){
    HkDynamicObject::unlink();
    contactEvent.payload = HkxSharedPtr();
    bones = HkxSharedPtr();
}

QString BSRagdollContactListenerModifier::evaluateDataValidity(){
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
    if (contactEvent.id >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": contactEvent event id out of range! Setting to max index in range!\n");
        contactEvent.id = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (contactEvent.payload.data() && contactEvent.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid contactEvent.payload type! Signature: "+QString::number(contactEvent.payload.data()->getSignature(), 16)+" Setting null value!\n");
        contactEvent.payload = HkxSharedPtr();
    }
    if (!bones.data()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null bones!\n");
    }else if (bones.data()->getSignature() != HKB_BONE_INDEX_ARRAY){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid bones type! Signature: "+QString::number(bones.data()->getSignature(), 16)+" Setting null value!\n");
        bones = HkxSharedPtr();
    }
    setDataValidity(isvalid);
    return errors;
}

BSRagdollContactListenerModifier::~BSRagdollContactListenerModifier(){
    refCount--;
}
