#include "bsinterpvaluemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint BSInterpValueModifier::refCount = 0;

const QString BSInterpValueModifier::classname = "BSInterpValueModifier";

BSInterpValueModifier::BSInterpValueModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      source(0),
      target(0),
      result(0),
      gain(0)
{
    setType(BS_INTERP_VALUE_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "InterpValueModifier_"+QString::number(refCount);
}

const QString BSInterpValueModifier::getClassname(){
    return classname;
}

QString BSInterpValueModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool BSInterpValueModifier::readData(const HkxXmlReader &reader, long & index){
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
        }else if (text == "source"){
            source = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "source");
        }else if (text == "target"){
            target = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "target");
        }else if (text == "result"){
            result = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "result");
        }else if (text == "gain"){
            gain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "gain");
        }
    }
    index--;
    return true;
}

bool BSInterpValueModifier::write(HkxXMLWriter *writer){
    std::lock_guard <std::mutex> guard(mutex);
    auto writedatafield = [&](const QString & name, const QString & value, bool allownull){
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), value, allownull);
    };
    if (writer && !getIsWritten()){
        QString refString = "null";
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        if (getVariableBindingSetData()){
            refString = getVariableBindingSet()->getReferenceString();
        }
        writedatafield("variableBindingSet", refString, false);
        writedatafield("userData", QString::number(userData), false);
        writedatafield("name", name, false);
        writedatafield("enable", getBoolAsString(enable), false);
        writedatafield("source", QString::number(source, char('f'), 6), false);
        writedatafield("target", QString::number(target, char('f'), 6), false);
        writedatafield("result", QString::number(result, char('f'), 6), false);
        writedatafield("gain", QString::number(gain, char('f'), 6), false);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSInterpValueModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSInterpValueModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
}

QString BSInterpValueModifier::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    auto setinvalid = [&](const QString & message){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": "+message+"!");
    };
    QString temp = HkDynamicObject::evaluateDataValidity();
    (temp != "") ? errors.append(temp+getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid variable binding set!\n"): NULL;
    (name == "") ? setinvalid("Invalid name") : NULL;
    setDataValidity(isvalid);
    return errors;
}

BSInterpValueModifier::~BSInterpValueModifier(){
    refCount--;
}
