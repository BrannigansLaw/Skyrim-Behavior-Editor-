#include "hkbevaluateexpressionmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: hkbEvaluateExpressionModifier
*/

uint hkbEvaluateExpressionModifier::refCount = 0;

QString hkbEvaluateExpressionModifier::classname = "hkbEvaluateExpressionModifier";

hkbEvaluateExpressionModifier::hkbEvaluateExpressionModifier(BehaviorFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true)
{
    setType(HKB_EVALUATE_EXPRESSION_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "hkbEvaluateExpressionModifier"+QString::number(refCount);
}

QString hkbEvaluateExpressionModifier::getClassname(){
    return classname;
}

QString hkbEvaluateExpressionModifier::getName() const{
    return name;
}

bool hkbEvaluateExpressionModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "expressions"){
            if (!expressions.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'expressions' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbEvaluateExpressionModifier::write(HkxXMLWriter *writer){
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
        if (expressions.data()){
            refString = expressions.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("expressions"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (expressions.data() && !expressions.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'expressions'!!!", true);
        }
    }
    return true;
}

bool hkbEvaluateExpressionModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<DataIconManager *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxObjectExpSharedPtr *ptr = getParentFile()->findHkxObject(expressions.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_EXPRESSION_DATA_ARRAY){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'expressions' is not a HKB_EXPRESSION_DATA_ARRAY!");
            setDataValidity(false);
        }
        expressions = *ptr;
    }
    return true;
}

void hkbEvaluateExpressionModifier::unlink(){
    HkDynamicObject::unlink();
    expressions = HkxObjectExpSharedPtr();
}

bool hkbEvaluateExpressionModifier::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (!expressions.data()){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbEvaluateExpressionModifier::~hkbEvaluateExpressionModifier(){
    refCount--;
}
