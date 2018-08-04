#include "hkbcombinetransformsmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint hkbCombineTransformsModifier::refCount = 0;

const QString hkbCombineTransformsModifier::classname = "hkbCombineTransformsModifier";

hkbCombineTransformsModifier::hkbCombineTransformsModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      invertLeftTransform(false),
      invertRightTransform(false),
      invertResult(false)
{
    setType(HKB_COMBINE_TRANSFORMS_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "CombineTransformsModifier_"+QString::number(refCount);
}

const QString hkbCombineTransformsModifier::getClassname(){
    return classname;
}

QString hkbCombineTransformsModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool hkbCombineTransformsModifier::readData(const HkxXmlReader &reader, long & index){
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
        }else if (text == "translationOut"){
            translationOut = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "translationOut");
        }else if (text == "rotationOut"){
            rotationOut = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "rotationOut");
        }else if (text == "leftTranslation"){
            leftTranslation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "leftTranslation");
        }else if (text == "leftRotation"){
            leftRotation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "leftRotation");
        }else if (text == "rightTranslation"){
            rightTranslation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "rightTranslation");
        }else if (text == "rightRotation"){
            rightRotation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "rightRotation");
        }else if (text == "invertLeftTransform"){
            invertLeftTransform = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "invertLeftTransform");
        }else if (text == "invertRightTransform"){
            invertRightTransform = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "invertRightTransform");
        }else if (text == "invertResult"){
            invertResult = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "invertResult");
        }else{
            //LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nUnknown field '"+text+"' found!\nObject Reference: "+ref);
        }
    }
    index--;
    return true;
}

bool hkbCombineTransformsModifier::write(HkxXMLWriter *writer){
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
        writedatafield("translationOut", translationOut.getValueAsString());
        writedatafield("rotationOut", rotationOut.getValueAsString());
        writedatafield("leftTranslation", leftTranslation.getValueAsString());
        writedatafield("leftRotation", leftRotation.getValueAsString());
        writedatafield("rightTranslation", rightTranslation.getValueAsString());
        writedatafield("rightRotation", rightRotation.getValueAsString());
        writedatafield("invertLeftTransform", getBoolAsString(invertLeftTransform));
        writedatafield("invertRightTransform", getBoolAsString(invertRightTransform));
        writedatafield("invertResult", getBoolAsString(invertResult));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbCombineTransformsModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbCombineTransformsModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
}

QString hkbCombineTransformsModifier::evaluateDataValidity(){
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

hkbCombineTransformsModifier::~hkbCombineTransformsModifier(){
    refCount--;
}
