#include "bstweenermodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint BSTweenerModifier::refCount = 0;

const QString BSTweenerModifier::classname = "BSTweenerModifier";

BSTweenerModifier::BSTweenerModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      tweenPosition(true),
      tweenRotation(true),
      useTweenDuration(true),
      tweenDuration(0)
{
    setType(BS_TWEENER_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "TweenerModifier_"+QString::number(refCount);
}

const QString BSTweenerModifier::getClassname(){
    return classname;
}

QString BSTweenerModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool BSTweenerModifier::readData(const HkxXmlReader &reader, long & index){
    std::lock_guard <std::mutex> guard(mutex);
    bool ok;
    QByteArray text;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    auto checkvalue = [&](bool value, const QString & fieldname){
        (!value) ? LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\n'"+fieldname+"' has invalid data!\nObject Reference: "+ref) : NULL;
    };
    for (; index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"; index++){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            checkvalue(getVariableBindingSet().readShdPtrReference(index, reader), "variableBindingSet");
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            checkvalue(ok, "userData");
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            checkvalue((name != ""), "name");
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "enable");
        }else if (text == "tweenPosition"){
            tweenPosition = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "tweenPosition");
        }else if (text == "tweenRotation"){
            tweenRotation = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "tweenRotation");
        }else if (text == "useTweenDuration"){
            useTweenDuration = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "useTweenDuration");
        }else if (text == "tweenDuration"){
            tweenDuration = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "tweenDuration");
        }else if (text == "targetPosition"){
            targetPosition = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "targetPosition");
        }else if (text == "targetRotation"){
            targetRotation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "targetRotation");
        }else{
            //LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nUnknown field '"+text+"' found!\nObject Reference: "+ref);
        }
    }
    index--;
    return true;
}

bool BSTweenerModifier::write(HkxXMLWriter *writer){
    std::lock_guard <std::mutex> guard(mutex);
    auto writedatafield = [&](const QString & name, const QString & value){
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), value);
    };
    if (writer && !getIsWritten()){
        QString refString = "null";
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        if (getVariableBindingSetData()){
            refString = getVariableBindingSet()->getReferenceString();
        }
        writedatafield("variableBindingSet", refString);
        writedatafield("userData", QString::number(userData));
        writedatafield("name", name);
        writedatafield("enable", getBoolAsString(enable));
        writedatafield("tweenPosition", getBoolAsString(tweenPosition));
        writedatafield("tweenRotation", getBoolAsString(tweenRotation));
        writedatafield("useTweenDuration", getBoolAsString(useTweenDuration));
        writedatafield("tweenDuration", QString::number(tweenDuration, char('f'), 6));
        writedatafield("targetPosition", targetPosition.getValueAsString());
        writedatafield("targetRotation", targetRotation.getValueAsString());
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSTweenerModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSTweenerModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
}

QString BSTweenerModifier::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid name!\n");
    }
    setDataValidity(isvalid);
    return errors;
}

BSTweenerModifier::~BSTweenerModifier(){
    refCount--;
}
