#include "hkbexpressioncondition.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

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
    getParentFile()->addObjectToFile(this, ref);refCount++;
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
                writeToLog("hkbExpressionCondition: readData()!\nFailed to properly read 'expression' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbExpressionCondition::evaulateDataValidity(){
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
