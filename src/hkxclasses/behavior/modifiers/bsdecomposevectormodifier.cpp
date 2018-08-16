#include "bsdecomposevectormodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint BSDecomposeVectorModifier::refCount = 0;

const QString BSDecomposeVectorModifier::classname = "BSDecomposeVectorModifier";

BSDecomposeVectorModifier::BSDecomposeVectorModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      x(0),
      y(0),
      z(0),
      w(0)
{
    setType(BS_DECOMPOSE_VECTOR_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "DecomposeVectorModifier_"+QString::number(refCount);
}

const QString BSDecomposeVectorModifier::getClassname(){
    return classname;
}

QString BSDecomposeVectorModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool BSDecomposeVectorModifier::readData(const HkxXmlReader &reader, long & index){
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
        }else if (text == "vector"){
            vector = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "vector");
        }else if (text == "x"){
            x = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "x");
        }else if (text == "y"){
            y = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "y");
        }else if (text == "z"){
            z = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "z");
        }else if (text == "w"){
            w = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "w");
        }
    }
    index--;
    return true;
}

bool BSDecomposeVectorModifier::write(HkxXMLWriter *writer){
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
        writedatafield("vector", vector.getValueAsString(), false);
        writedatafield("x", QString::number(x, char('f'), 6), false);
        writedatafield("y", QString::number(y, char('f'), 6), false);
        writedatafield("z", QString::number(z, char('f'), 6), false);
        writedatafield("w", QString::number(w, char('f'), 6), false);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSDecomposeVectorModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSDecomposeVectorModifier::unlink(){
    HkDynamicObject::unlink();
}

QString BSDecomposeVectorModifier::evaluateDataValidity(){
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

BSDecomposeVectorModifier::~BSDecomposeVectorModifier(){
    refCount--;
}
