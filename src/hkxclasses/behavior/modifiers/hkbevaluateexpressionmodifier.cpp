#include "hkbevaluateexpressionmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbexpressiondataarray.h"

/*
 * CLASS: hkbEvaluateExpressionModifier
*/

uint hkbEvaluateExpressionModifier::refCount = 0;

QString hkbEvaluateExpressionModifier::classname = "hkbEvaluateExpressionModifier";

hkbEvaluateExpressionModifier::hkbEvaluateExpressionModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(2),
      enable(true)
{
    setType(HKB_EVALUATE_EXPRESSION_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "EvaluateExpressionModifier"+QString::number(refCount);
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
        }else if (text == "expressions"){
            if (!expressions.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'expressions' reference!\nObject Reference: "+ref);
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
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (expressions.data() && !expressions.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'expressions'!!!");
        }
    }
    return true;
}

int hkbEvaluateExpressionModifier::getNumberOfExpressions() const{
    if (expressions.data()){
        return static_cast<hkbExpressionDataArray *>(expressions.data())->expressionsData.size();
    }
    return 0;
}

bool hkbEvaluateExpressionModifier::isEventReferenced(int eventindex) const{
    if (expressions.constData() && expressions.constData()->isEventReferenced(eventindex)){
        return true;
    }
    return false;
}

void hkbEvaluateExpressionModifier::updateEventIndices(int eventindex){
    if (expressions.data()){
        expressions.data()->updateEventIndices(eventindex);
    }
}

void hkbEvaluateExpressionModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
    if (expressions.data()){
        expressions.data()->fixMergedEventIndices(dominantfile);
    }
}

bool hkbEvaluateExpressionModifier::merge(HkxObject *recessiveObject){
    hkbEvaluateExpressionModifier *obj = nullptr;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == HKB_EVALUATE_EXPRESSION_MODIFIER){
        obj = static_cast<hkbEvaluateExpressionModifier *>(recessiveObject);
        if (expressions.data()){
            if (obj->expressions.data()){
                expressions.data()->merge(obj->expressions.data());
            }
        }else if (obj->expressions.data()){
            expressions = obj->expressions;
            getParentFile()->addObjectToFile(obj->expressions.data(), 0);   //TO DO: fix merged event indices???
        }
        injectWhileMerging(obj);
        return true;
    }else{
        return false;
    }
}

void hkbEvaluateExpressionModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (expressions.data()){
        ref++;
        expressions.data()->updateReferences(ref);
    }
}

QVector<HkxObject *> hkbEvaluateExpressionModifier::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (expressions.data()){
        list.append(expressions.data());
    }
    return list;
}

bool hkbEvaluateExpressionModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(expressions.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_EXPRESSION_DATA_ARRAY){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'expressions' is not a HKB_EXPRESSION_DATA_ARRAY!");
            setDataValidity(false);
        }
        expressions = *ptr;
    }
    return true;
}

void hkbEvaluateExpressionModifier::unlink(){
    HkDynamicObject::unlink();
    expressions = HkxSharedPtr();
}

QString hkbEvaluateExpressionModifier::evaluateDataValidity(){
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
    if (expressions.data()){
        if (expressions.data()->getSignature() != HKB_EXPRESSION_DATA_ARRAY){
            isvalid = false;
            errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid expressions type! Signature: "+QString::number(expressions.data()->getSignature(), 16)+" Setting default value!\n");
            expressions = HkxSharedPtr();
        }else if (static_cast<hkbExpressionDataArray *>(expressions.data())->expressionsData.size() < 1){
            errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": expressions has no expressionsData! Setting null value!\n");
            expressions = HkxSharedPtr();
        }else if (expressions.data()->isDataValid() && expressions.data()->evaluateDataValidity() != ""){
            isvalid = false;
            //errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid expressions data!\n");
        }
    }else if (!expressions.data()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null expressions!\n");
    }
    setDataValidity(isvalid);
    return errors;
}

hkbEvaluateExpressionModifier::~hkbEvaluateExpressionModifier(){
    refCount--;
}
