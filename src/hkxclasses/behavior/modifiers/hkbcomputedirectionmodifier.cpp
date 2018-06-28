#include "hkbcomputedirectionmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbComputeDirectionModifier
*/

uint hkbComputeDirectionModifier::refCount = 0;

QString hkbComputeDirectionModifier::classname = "hkbComputeDirectionModifier";

hkbComputeDirectionModifier::hkbComputeDirectionModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      groundAngleOut(0),
      upAngleOut(0),
      verticalOffset(0),
      reverseGroundAngle(false),
      reverseUpAngle(false),
      projectPoint(false),
      normalizePoint(false),
      computeOnlyOnce(false),
      computedOutput(false)
{
    setType(HKB_COMPUTE_DIRECTION_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "ComputeDirectionModifier"+QString::number(refCount);
}

QString hkbComputeDirectionModifier::getClassname(){
    return classname;
}

QString hkbComputeDirectionModifier::getName() const{
    return name;
}

bool hkbComputeDirectionModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "pointIn"){
            pointIn = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'pointIn' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pointOut"){
            pointOut = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'pointOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "groundAngleOut"){
            groundAngleOut = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'groundAngleOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "upAngleOut"){
            upAngleOut = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'upAngleOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "verticalOffset"){
            verticalOffset = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'verticalOffset' data field!\nObject Reference: "+ref);
            }
        }else if (text == "reverseGroundAngle"){
            reverseGroundAngle = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'reverseGroundAngle' data field!\nObject Reference: "+ref);
            }
        }else if (text == "reverseUpAngle"){
            reverseUpAngle = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'reverseUpAngle' data field!\nObject Reference: "+ref);
            }
        }else if (text == "projectPoint"){
            projectPoint = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'projectPoint' data field!\nObject Reference: "+ref);
            }
        }else if (text == "normalizePoint"){
            normalizePoint = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'normalizePoint' data field!\nObject Reference: "+ref);
            }
        }else if (text == "computeOnlyOnce"){
            computeOnlyOnce = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'computeOnlyOnce' data field!\nObject Reference: "+ref);
            }
        }else if (text == "computedOutput"){
            computedOutput = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'computedOutput' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbComputeDirectionModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pointIn"), pointIn.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pointOut"), pointOut.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("groundAngleOut"), QString::number(groundAngleOut, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("upAngleOut"), QString::number(upAngleOut, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("verticalOffset"), QString::number(verticalOffset, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("reverseGroundAngle"), getBoolAsString(reverseGroundAngle));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("reverseUpAngle"), getBoolAsString(reverseUpAngle));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("projectPoint"), getBoolAsString(projectPoint));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("normalizePoint"), getBoolAsString(normalizePoint));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("computeOnlyOnce"), getBoolAsString(computeOnlyOnce));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("computedOutput"), getBoolAsString(computedOutput));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbComputeDirectionModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbComputeDirectionModifier::unlink(){
    HkDynamicObject::unlink();
}

QString hkbComputeDirectionModifier::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity(); if (temp != ""){errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");}
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    setDataValidity(isvalid);
    return errors;
}

hkbComputeDirectionModifier::~hkbComputeDirectionModifier(){
    refCount--;
}
