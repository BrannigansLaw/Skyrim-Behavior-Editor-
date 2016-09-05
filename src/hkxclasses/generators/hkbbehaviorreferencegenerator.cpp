#include "hkbbehaviorreferencegenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbBehaviorReferenceGenerator
*/

uint hkbBehaviorReferenceGenerator::refCount = 0;

QString hkbBehaviorReferenceGenerator::classname = "hkbBehaviorReferenceGenerator";

hkbBehaviorReferenceGenerator::hkbBehaviorReferenceGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(0)
{
    setType(HKB_BEHAVIOR_REFERENCE_GENERATOR, TYPE_GENERATOR);
    refCount++;
    name = "Behavior Reference Generator "+QString::number(refCount);
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
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbBehaviorReferenceGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbBehaviorReferenceGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbBehaviorReferenceGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "behaviorName"){
            behaviorName = reader.getElementValueAt(index);
            if (behaviorName == ""){
                writeToLog("hkbBehaviorReferenceGenerator: readData()!\nFailed to properly read 'behaviorName' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbBehaviorReferenceGenerator::link(){
    return true;
}

QString hkbBehaviorReferenceGenerator::getName() const{
    return name;
}

bool hkbBehaviorReferenceGenerator::evaulateDataValidity(){
    if (name == ""){
    }else if (behaviorName == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbBehaviorReferenceGenerator::~hkbBehaviorReferenceGenerator(){
    refCount--;
}
