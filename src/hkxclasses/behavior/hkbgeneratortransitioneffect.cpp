#include "hkbgeneratortransitioneffect.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbGeneratorTransitionEffect
*/

uint hkbGeneratorTransitionEffect::refCount = 0;

QString hkbGeneratorTransitionEffect::classname = "hkbGeneratorTransitionEffect";

QStringList hkbGeneratorTransitionEffect::SelfTransitionMode = {"SELF_TRANSITION_MODE_CONTINUE_IF_CYCLIC_BLEND_IF_ACYCLIC", "SELF_TRANSITION_MODE_CONTINUE", "SELF_TRANSITION_MODE_RESET", "SELF_TRANSITION_MODE_BLEND"};
QStringList hkbGeneratorTransitionEffect::EventMode = {"EVENT_MODE_DEFAULT", "EVENT_MODE_PROCESS_ALL", "EVENT_MODE_IGNORE_FROM_GENERATOR", "EVENT_MODE_IGNORE_TO_GENERATOR"};

hkbGeneratorTransitionEffect::hkbGeneratorTransitionEffect(HkxFile *parent, long ref)
    : HkDynamicObject(parent, ref),
      userData(0),
      selfTransitionMode(SelfTransitionMode.first()),
      eventMode(EventMode.first()),
      blendInDuration(0),
      blendOutDuration(0),
      syncToGeneratorStartTime(false)
{
    setType(HKB_GENERATOR_TRANSITION_EFFECT, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "GeneratorTransitionEffect"+QString::number(refCount);
}

QString hkbGeneratorTransitionEffect::getClassname(){
    return classname;
}

QString hkbGeneratorTransitionEffect::getName() const{
    return name;
}

bool hkbGeneratorTransitionEffect::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+":  readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+":  readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+":  readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "selfTransitionMode"){
            selfTransitionMode = reader.getElementValueAt(index);
            if (!SelfTransitionMode.contains(selfTransitionMode)){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+":  readData()!\nFailed to properly read 'selfTransitionMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eventMode"){
            eventMode = reader.getElementValueAt(index);
            if (!EventMode.contains(eventMode)){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+":  readData()!\nFailed to properly read 'eventMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "transitionGenerator"){
            if (!transitionGenerator.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+":  readData()!\nFailed to properly read 'transitionGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "blendInDuration"){
            blendInDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+":  readData()!\nFailed to properly read 'blendInDuration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "blendOutDuration"){
            blendOutDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+":  readData()!\nFailed to properly read 'blendOutDuration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "syncToGeneratorStartTime"){
            syncToGeneratorStartTime = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'syncToGeneratorStartTime' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbGeneratorTransitionEffect::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("selfTransitionMode"), selfTransitionMode);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eventMode"), eventMode);
        if (transitionGenerator.data()){
            refString = transitionGenerator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("transitionGenerator"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("blendInDuration"), QString::number(blendInDuration, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("blendOutDuration"), QString::number(blendOutDuration, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("syncToGeneratorStartTime"), getBoolAsString(syncToGeneratorStartTime));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+":  write()!\nUnable to write 'variableBindingSet'!!!");
        }
        /*if (transitionGenerator.data() && !transitionGenerator.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+":  write()!\nUnable to write 'transitionGenerator'!!!");
        }*/
    }
    return true;
}

bool hkbGeneratorTransitionEffect::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+":  link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(transitionGenerator.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getType() != TYPE_GENERATOR){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": linkVar()!\nThe linked object is not a TYPE_GENERATOR!");
            setDataValidity(false);
        }
        transitionGenerator = *ptr;
    }
    return true;
}

void hkbGeneratorTransitionEffect::unlink(){
    HkDynamicObject::unlink();
    transitionGenerator = HkxSharedPtr();
}

bool hkbGeneratorTransitionEffect::evaluateDataValidity(){
    if (!HkDynamicObject::evaluateDataValidity() || (!SelfTransitionMode.contains(selfTransitionMode)) || (!EventMode.contains(eventMode)) || (!transitionGenerator.data() || transitionGenerator.data()->getType() != TYPE_GENERATOR) || (name == "")){
        setDataValidity(false);
        return false;
    }else{
        setDataValidity(true);
        return true;
    }
}

hkbGeneratorTransitionEffect::~hkbGeneratorTransitionEffect(){
    refCount--;
}
