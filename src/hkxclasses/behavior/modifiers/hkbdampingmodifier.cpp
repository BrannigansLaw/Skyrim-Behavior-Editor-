#include "hkbdampingmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint hkbDampingModifier::refCount = 0;

const QString hkbDampingModifier::classname = "hkbDampingModifier";

hkbDampingModifier::hkbDampingModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(1),
      enable(true),
      kP(0),
      kI(0),
      kD(0),
      enableScalarDamping(true),
      enableVectorDamping(false),
      rawValue(0),
      dampedValue(0),
      errorSum(0),
      previousError(0)
{
    setType(HKB_DAMPING_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "DampingModifier_"+QString::number(refCount);
}

const QString hkbDampingModifier::getClassname(){
    return classname;
}

QString hkbDampingModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool hkbDampingModifier::readData(const HkxXmlReader &reader, long & index){
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
        }else if (text == "kP"){
            kP = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "kP");
        }else if (text == "kI"){
            kI = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "kI");
        }else if (text == "kD"){
            kD = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "kD");
        }else if (text == "enableScalarDamping"){
            enableScalarDamping = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "enableScalarDamping");
        }else if (text == "enableVectorDamping"){
            enableVectorDamping = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "enableVectorDamping");
        }else if (text == "rawValue"){
            rawValue = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "rawValue");
        }else if (text == "dampedValue"){
            dampedValue = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "dampedValue");
        }else if (text == "rawVector"){
            rawVector = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "rawVector");
        }else if (text == "dampedVector"){
            dampedVector = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "dampedVector");
        }else if (text == "vecErrorSum"){
            vecErrorSum = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "vecErrorSum");
        }else if (text == "vecPreviousError"){
            vecPreviousError = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "vecPreviousError");
        }else if (text == "errorSum"){
            errorSum = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "errorSum");
        }else if (text == "previousError"){
            previousError = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "previousError");
        }
    }
    index--;
    return true;
}

bool hkbDampingModifier::write(HkxXMLWriter *writer){
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
        writedatafield("kP", QString::number(kP, char('f'), 6));
        writedatafield("kI", QString::number(kI, char('f'), 6));
        writedatafield("kD", QString::number(kD, char('f'), 6));
        writedatafield("enableScalarDamping", getBoolAsString(enableScalarDamping));
        writedatafield("enableVectorDamping", getBoolAsString(enableVectorDamping));
        writedatafield("rawValue", QString::number(rawValue, char('f'), 6));
        writedatafield("dampedValue", QString::number(dampedValue, char('f'), 6));
        writedatafield("rawVector", rawVector.getValueAsString());
        writedatafield("dampedVector", dampedVector.getValueAsString());
        writedatafield("vecErrorSum", vecErrorSum.getValueAsString());
        writedatafield("vecPreviousError", vecPreviousError.getValueAsString());
        writedatafield("errorSum", QString::number(errorSum, char('f'), 6));
        writedatafield("previousError", QString::number(previousError, char('f'), 6));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbDampingModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbDampingModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
}

QString hkbDampingModifier::evaluateDataValidity(){
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

hkbDampingModifier::~hkbDampingModifier(){
    refCount--;
}
