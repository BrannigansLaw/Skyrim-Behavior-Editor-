#include "hkbtransformvectormodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint hkbTransformVectorModifier::refCount = 0;

const QString hkbTransformVectorModifier::classname = "hkbTransformVectorModifier";

hkbTransformVectorModifier::hkbTransformVectorModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      rotateOnly(true),
      inverse(true),
      computeOnActivate(true),
      computeOnModify(true)
{
    setType(HKB_TRANSFORM_VECTOR_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "TransformVectorModifier_"+QString::number(refCount);
}

const QString hkbTransformVectorModifier::getClassname(){
    return classname;
}

QString hkbTransformVectorModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool hkbTransformVectorModifier::readData(const HkxXmlReader &reader, long & index){
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
        }else if (text == "rotation"){
            rotation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "rotation");
        }else if (text == "translation"){
            translation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "translation");
        }else if (text == "vectorIn"){
            vectorIn = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "vectorIn");
        }else if (text == "vectorOut"){
            vectorOut = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "vectorOut");
        }else if (text == "rotateOnly"){
            rotateOnly = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "rotateOnly");
        }else if (text == "inverse"){
            inverse = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "inverse");
        }else if (text == "computeOnActivate"){
            computeOnActivate = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "computeOnActivate");
        }else if (text == "computeOnModify"){
            computeOnModify = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "computeOnModify");
        }else{
            //LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nUnknown field '"+text+"' found!\nObject Reference: "+ref);
        }
    }
    index--;
    return true;
}

bool hkbTransformVectorModifier::write(HkxXMLWriter *writer){
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
        writedatafield("rotation", rotation.getValueAsString());
        writedatafield("translation", translation.getValueAsString());
        writedatafield("vectorIn", vectorIn.getValueAsString());
        writedatafield("vectorOut", vectorOut.getValueAsString());
        writedatafield("rotateOnly", getBoolAsString(rotateOnly));
        writedatafield("inverse", getBoolAsString(inverse));
        writedatafield("computeOnActivate", getBoolAsString(computeOnActivate));
        writedatafield("computeOnModify", getBoolAsString(computeOnModify));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbTransformVectorModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbTransformVectorModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
}

QString hkbTransformVectorModifier::evaluateDataValidity(){
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

hkbTransformVectorModifier::~hkbTransformVectorModifier(){
    refCount--;
}
