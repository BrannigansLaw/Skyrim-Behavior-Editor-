#include "bseventeveryneventsmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"

/*
 * CLASS: BSEventEveryNEventsModifier
*/

uint BSEventEveryNEventsModifier::refCount = 0;

QString BSEventEveryNEventsModifier::classname = "BSEventEveryNEventsModifier";

BSEventEveryNEventsModifier::BSEventEveryNEventsModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      numberOfEventsBeforeSend(0),
      minimumNumberOfEventsBeforeSend(0),
      randomizeNumberOfEvents(false)
{
    setType(BS_EVENT_EVERY_N_EVENTS_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "EventEveryNEventsModifier"+QString::number(refCount);
}

QString BSEventEveryNEventsModifier::getClassname(){
    return classname;
}

QString BSEventEveryNEventsModifier::getName() const{
    return name;
}

bool BSEventEveryNEventsModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eventToCheckFor"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToCheckFor.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToCheckFor.payload.readShdPtrReference(index, reader)){
                        WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "eventToSend"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToSend.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToSend.payload.readShdPtrReference(index, reader)){
                        WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "numberOfEventsBeforeSend"){
            numberOfEventsBeforeSend = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'numberOfEventsBeforeSend' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minimumNumberOfEventsBeforeSend"){
            minimumNumberOfEventsBeforeSend = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'minimumNumberOfEventsBeforeSend' data field!\nObject Reference: "+ref);
            }
        }else if (text == "randomizeNumberOfEvents"){
            randomizeNumberOfEvents = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'randomizeNumberOfEvents' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSEventEveryNEventsModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eventToCheckFor"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(eventToCheckFor.id));
        if (eventToCheckFor.payload.data()){
            refString = eventToCheckFor.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eventToSend"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(eventToSend.id));
        if (eventToSend.payload.data()){
            refString = eventToSend.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("numberOfEventsBeforeSend"), QString::number(numberOfEventsBeforeSend));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minimumNumberOfEventsBeforeSend"), QString::number(minimumNumberOfEventsBeforeSend));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("randomizeNumberOfEvents"), getBoolAsString(randomizeNumberOfEvents));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSEventEveryNEventsModifier::isEventReferenced(int eventindex) const{
    if (eventToCheckFor.id == eventindex || eventToSend.id == eventindex){
        return true;
    }
    return false;
}

void BSEventEveryNEventsModifier::updateEventIndices(int eventindex){
    if (eventToCheckFor.id > eventindex){
        eventToCheckFor.id--;
    }
    if (eventToSend.id > eventindex){
        eventToSend.id--;
    }
}

void BSEventEveryNEventsModifier::mergeEventIndex(int oldindex, int newindex){
    if (eventToCheckFor.id == oldindex){
        eventToCheckFor.id = newindex;
    }
    if (eventToSend.id == oldindex){
        eventToSend.id = newindex;
    }
}

void BSEventEveryNEventsModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
    hkbBehaviorGraphData *recdata;
    hkbBehaviorGraphData *domdata;
    QString thiseventname;
    int eventindex;
    if (!getIsMerged() && dominantfile){
        //TO DO: Support character properties...
        recdata = static_cast<hkbBehaviorGraphData *>(static_cast<BehaviorFile *>(getParentFile())->getBehaviorGraphData());
        domdata = static_cast<hkbBehaviorGraphData *>(dominantfile->getBehaviorGraphData());
        if (recdata && domdata){
            auto fixIndex = [&](int & id){
                thiseventname = recdata->getEventNameAt(id);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                id = eventindex;
            };
            fixIndex(eventToCheckFor.id);
            fixIndex(eventToSend.id);
        }
        setIsMerged(true);
    }
}

void BSEventEveryNEventsModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (eventToCheckFor.payload.data()){
        ref++;
        eventToCheckFor.payload.data()->updateReferences(ref);
    }
    if (eventToSend.payload.data()){
        ref++;
        eventToSend.payload.data()->updateReferences(ref);
    }
}

bool BSEventEveryNEventsModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToCheckFor.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            WRITE_TO_LOG(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        eventToCheckFor.payload = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToSend.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            WRITE_TO_LOG(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        eventToSend.payload = *ptr;
    }
    return true;
}

void BSEventEveryNEventsModifier::unlink(){
    HkDynamicObject::unlink();
    eventToCheckFor.payload = HkxSharedPtr();
    eventToSend.payload = HkxSharedPtr();
}

bool BSEventEveryNEventsModifier::evaluateDataValidity(){    //Check if event id is valid???
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (eventToCheckFor.payload.data() && eventToCheckFor.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else if (eventToSend.payload.data() && eventToSend.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSEventEveryNEventsModifier::~BSEventEveryNEventsModifier(){
    refCount--;
}
