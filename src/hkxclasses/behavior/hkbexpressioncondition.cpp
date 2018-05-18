#include "hkbexpressioncondition.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/**
 * hkbExpressionCondition
 */

uint hkbExpressionCondition::refCount = 0;

QString hkbExpressionCondition::classname = "hkbExpressionCondition";

hkbExpressionCondition::hkbExpressionCondition(BehaviorFile *parent, const QString &string, long ref)
    : HkxObject(parent, ref),
      expression(string)
{
    setType(HKB_EXPRESSION_CONDITION, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbExpressionCondition::getClassname(){
    return classname;
}

QString hkbExpressionCondition::getExpression() const{
    return expression;
}

bool hkbExpressionCondition::readData(const HkxXmlReader & reader, long index){
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "expression"){
            expression = reader.getElementValueAt(index);
            if (expression == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'expression' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbExpressionCondition::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("expression"), expression);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

bool hkbExpressionCondition::evaluateDataValidity(){
    if (expression == ""){
        setDataValidity(false);
        return false;
    }
    setDataValidity(true);
    return true;
}

hkbExpressionCondition::~hkbExpressionCondition(){
    refCount--;
}
