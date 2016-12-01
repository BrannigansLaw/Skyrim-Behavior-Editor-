#include "hkbexpressiondataarray.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: hkbExpressionDataArray
*/

uint hkbExpressionDataArray::refCount = 0;

QString hkbExpressionDataArray::classname = "hkbExpressionDataArray";

QStringList hkbExpressionDataArray::EventMode = {"EVENT_MODE_SEND_ONCE", "EVENT_MODE_SEND_ON_TRUE", "EVENT_MODE_SEND_ON_FALSE_TO_TRUE", "EVENT_MODE_SEND_EVERY_FRAME_ONCE_TRUE"};

hkbExpressionDataArray::hkbExpressionDataArray(BehaviorFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKB_EXPRESSION_DATA_ARRAY, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbExpressionDataArray::getClassname(){
    return classname;
}

void hkbExpressionDataArray::addExpression(const QString & exp, ExpressionEventMode mode){
    expressionsData.append(hkExpression(exp, mode));
}

void hkbExpressionDataArray::removeExpression(const QString & exp){
    for (int i = 0; i < expressionsData.size(); i++){
        if (expressionsData.at(i).expression == exp){
            expressionsData.removeAt(i);
        }
    }
}

bool hkbExpressionDataArray::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "expressionsData"){
            int numexp = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numexp; j++){
                expressionsData.append(hkExpression());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    text = reader.getNthAttributeValueAt(index, 0);
                    if (text == "expression"){
                        expressionsData.last().expression = reader.getElementValueAt(index);
                        if (expressionsData.last().expression == ""){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'expression' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "assignmentVariableIndex"){
                        expressionsData.last().assignmentVariableIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'assignmentVariableIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "assignmentEventIndex"){
                        expressionsData.last().assignmentEventIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'assignmentEventIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "eventMode"){
                        expressionsData.last().eventMode = reader.getElementValueAt(index);
                        if (!EventMode.contains(expressionsData.last().eventMode)){
                            writeToLog(getClassname()+": readData()!\nInvalid 'eventMode' data!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
        }
        index++;
    }
    return true;
}

bool hkbExpressionDataArray::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"expressionsData", QString::number(expressionsData.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < expressionsData.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("expression"), expressionsData.at(i).expression);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("assignmentVariableIndex"), QString::number(expressionsData.at(i).assignmentVariableIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("assignmentEventIndex"), QString::number(expressionsData.at(i).assignmentEventIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eventMode"), expressionsData.at(i).eventMode);
            writer->writeLine(writer->object, false);
        }
        if (expressionsData.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

bool hkbExpressionDataArray::link(){
    return true;
}

bool hkbExpressionDataArray::evaulateDataValidity(){
    //Do later...
    return true;
}

hkbExpressionDataArray::~hkbExpressionDataArray(){
    refCount--;
}
