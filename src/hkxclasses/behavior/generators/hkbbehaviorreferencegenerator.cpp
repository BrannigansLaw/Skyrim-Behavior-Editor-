#include "hkbbehaviorreferencegenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbBehaviorReferenceGenerator
*/

uint hkbBehaviorReferenceGenerator::refCount = 0;

QString hkbBehaviorReferenceGenerator::classname = "hkbBehaviorReferenceGenerator";

hkbBehaviorReferenceGenerator::hkbBehaviorReferenceGenerator(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(0)
{
    setType(HKB_BEHAVIOR_REFERENCE_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "BehaviorReferenceGenerator"+QString::number(refCount);
}

QString hkbBehaviorReferenceGenerator::getClassname(){
    return classname;
}

bool hkbBehaviorReferenceGenerator::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "behaviorName"){
            behaviorName = reader.getElementValueAt(index);
            if (behaviorName == ""){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'behaviorName' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbBehaviorReferenceGenerator::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("behaviorName"), QString(behaviorName).replace("/", "\\"));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

QString hkbBehaviorReferenceGenerator::getBehaviorName() const{
    if (behaviorName.contains("\\")){
        return behaviorName.section("\\", -1, -1);
    }else if (behaviorName.contains("/")){
        return behaviorName.section("/", -1, -1);
    }
    WRITE_TO_LOG(getClassname()+": getBehaviorName()!\nInvalid behaviorName!!!");
    return behaviorName;
}

bool hkbBehaviorReferenceGenerator::link(){
    return true;
}

QString hkbBehaviorReferenceGenerator::getName() const{
    return name;
}

bool hkbBehaviorReferenceGenerator::evaluateDataValidity(){
    if (!HkDynamicObject::evaluateDataValidity() || name == "" || behaviorName == ""){
        setDataValidity(false);
        return false;
    }else{
        setDataValidity(true);
        return true;
    }
}

hkbBehaviorReferenceGenerator::~hkbBehaviorReferenceGenerator(){
    refCount--;
}
