#include "hkbevaluatehandlemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint hkbEvaluateHandleModifier::refCount = 0;

const QString hkbEvaluateHandleModifier::classname = "hkbEvaluateHandleModifier";

const QStringList hkbEvaluateHandleModifier::HandleChangeMode = {"HANDLE_CHANGE_MODE_ABRUPT", "HANDLE_CHANGE_MODE_CONSTANT_VELOCITY"};

hkbEvaluateHandleModifier::hkbEvaluateHandleModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      isValidOut(false),
      extrapolationTimeStep(0),
      handleChangeSpeed(0),
      handleChangeMode(HandleChangeMode.first())
{
    setType(HKB_EVALUATE_HANDLE_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "EvaluateHandleModifier_"+QString::number(refCount);
}

const QString hkbEvaluateHandleModifier::getClassname(){
    return classname;
}

QString hkbEvaluateHandleModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool hkbEvaluateHandleModifier::readData(const HkxXmlReader &reader, long & index){
    std::lock_guard <std::mutex> guard(mutex);
    bool ok;
    QByteArray text;
    auto ref = reader.getNthAttributeValueAt(index - 1, 0);
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
        }else if (text == "handle"){
            checkvalue(handle.readShdPtrReference(index, reader), "handle");
        }else if (text == "handlePositionOut"){
            handlePositionOut = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "handlePositionOut");
        }else if (text == "handleRotationOut"){
            handleRotationOut = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "handleRotationOut");
        }else if (text == "isValidOut"){
            isValidOut = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "isValidOut");
        }else if (text == "extrapolationTimeStep"){
            extrapolationTimeStep = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "extrapolationTimeStep");
        }else if (text == "handleChangeSpeed"){
            handleChangeSpeed = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "handleChangeSpeed");
        }else if (text == "handleChangeMode"){
            handleChangeMode = reader.getElementValueAt(index);
            checkvalue(HandleChangeMode.contains(handleChangeMode), "handleChangeMode");
        }
    }
    index--;
    return true;
}

bool hkbEvaluateHandleModifier::write(HkxXMLWriter *writer){
    std::lock_guard <std::mutex> guard(mutex);
    auto writedatafield = [&](const QString & name, const QString & value){
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), value);
    };
    auto writeref = [&](const HkxSharedPtr & shdptr, const QString & name){
        QString refString = "null";
        (shdptr.data()) ? refString = shdptr->getReferenceString() : NULL;
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), refString);
    };
    auto writechild = [&](const HkxSharedPtr & shdptr, const QString & datafield){
        if (shdptr.data() && !shdptr->write(writer))
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write '"+datafield+"'!!!");
    };
    if (writer && !getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writeref(getVariableBindingSet(), "variableBindingSet");
        writedatafield("userData", QString::number(userData));
        writedatafield("name", name);
        writedatafield("enable", getBoolAsString(enable));
        writeref(handle, "handle");
        writedatafield("handlePositionOut", handlePositionOut.getValueAsString());
        writedatafield("handleRotationOut", handleRotationOut.getValueAsString());
        writedatafield("isValidOut", getBoolAsString(isValidOut));
        writedatafield("extrapolationTimeStep", QString::number(extrapolationTimeStep, char('f'), 6));
        writedatafield("handleChangeSpeed", QString::number(handleChangeSpeed, char('f'), 6));
        writedatafield("handleChangeMode", handleChangeMode);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        writechild(getVariableBindingSet(), "variableBindingSet");
        writechild(handle, "handle");
    }
    return true;
}

bool hkbEvaluateHandleModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbEvaluateHandleModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
}

QString hkbEvaluateHandleModifier::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    auto temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid variable binding set!");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid name!");
    }
    setDataValidity(isvalid);
    return errors;
}

hkbEvaluateHandleModifier::~hkbEvaluateHandleModifier(){
    refCount--;
}
