#include "hkbstringcondition.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/**
 * hkbStringCondition
 */

uint hkbStringCondition::refCount = 0;

QString hkbStringCondition::classname = "hkbStringCondition";

hkbStringCondition::hkbStringCondition(BehaviorFile *parent, const QString &string, long ref)
    : HkxObject(parent, ref),
      conditionString(string)
{
    setType(HKB_STRING_CONDITION, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbStringCondition::getClassname(){
    return classname;
}

QString hkbStringCondition::getExpression() const{
    return conditionString;
}

bool hkbStringCondition::readData(const HkxXmlReader & reader, long index){
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "conditionString"){
            conditionString = reader.getElementValueAt(index);
            if (conditionString == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'conditionString' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbStringCondition::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("conditionString"), conditionString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

bool hkbStringCondition::evaluateDataValidity(){
    if (conditionString == ""){
        setDataValidity(false);
        return false;
    }
    setDataValidity(true);
    return true;
}

hkbStringCondition::~hkbStringCondition(){
    refCount--;
}
