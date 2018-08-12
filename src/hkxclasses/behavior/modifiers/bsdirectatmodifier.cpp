#include "bsdirectatmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint BSDirectAtModifier::refCount = 0;

const QString BSDirectAtModifier::classname = "BSDirectAtModifier";

BSDirectAtModifier::BSDirectAtModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(2),
      enable(true),
      directAtTarget(true),
      sourceBoneIndex(-1),
      startBoneIndex(-1),
      endBoneIndex(-1),
      limitHeadingDegrees(0),
      limitPitchDegrees(0),
      offsetHeadingDegrees(0),
      offsetPitchDegrees(0),
      onGain(0),
      offGain(0),
      userInfo(0),
      directAtCamera(false),
      directAtCameraX(0),
      directAtCameraY(0),
      directAtCameraZ(0),
      active(false),
      currentHeadingOffset(0),
      currentPitchOffset(0)
{
    setType(BS_DIRECT_AT_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "DirectAtModifier_"+QString::number(refCount);
}

const QString BSDirectAtModifier::getClassname(){
    return classname;
}

QString BSDirectAtModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool BSDirectAtModifier::readData(const HkxXmlReader &reader, long & index){
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
        }else if (text == "directAtTarget"){
            directAtTarget = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "directAtTarget");
        }else if (text == "sourceBoneIndex"){
            sourceBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "sourceBoneIndex");
        }else if (text == "startBoneIndex"){
            startBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "startBoneIndex");
        }else if (text == "endBoneIndex"){
            endBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "endBoneIndex");
        }else if (text == "limitHeadingDegrees"){
            limitHeadingDegrees = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "limitHeadingDegrees");
        }else if (text == "limitPitchDegrees"){
            limitPitchDegrees = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "limitPitchDegrees");
        }else if (text == "offsetHeadingDegrees"){
            offsetHeadingDegrees = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "offsetHeadingDegrees");
        }else if (text == "offsetPitchDegrees"){
            offsetPitchDegrees = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "offsetPitchDegrees");
        }else if (text == "onGain"){
            onGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "onGain");
        }else if (text == "offGain"){
            offGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "offGain");
        }else if (text == "targetLocation"){
            targetLocation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "targetLocation");
        }else if (text == "userInfo"){
            userInfo = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "userInfo");
        }else if (text == "directAtCamera"){
            directAtCamera = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "directAtCamera");
        }else if (text == "directAtCameraX"){
            directAtCameraX = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "directAtCameraX");
        }else if (text == "directAtCameraY"){
            directAtCameraY = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "directAtCameraY");
        }else if (text == "directAtCameraZ"){
            directAtCameraZ = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "directAtCameraZ");
        }else if (text == "active"){
            active = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "active");
        }else if (text == "currentHeadingOffset"){
            currentHeadingOffset = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "currentHeadingOffset");
        }else if (text == "currentPitchOffset"){
            currentPitchOffset = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "currentPitchOffset");
        }
    }
    index--;
    return true;
}

bool BSDirectAtModifier::write(HkxXMLWriter *writer){
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
        writedatafield("directAtTarget", getBoolAsString(directAtTarget), false);
        writedatafield("sourceBoneIndex", QString::number(sourceBoneIndex), false);
        writedatafield("startBoneIndex", QString::number(startBoneIndex), false);
        writedatafield("endBoneIndex", QString::number(endBoneIndex), false);
        writedatafield("limitHeadingDegrees", QString::number(limitHeadingDegrees, char('f'), 6), false);
        writedatafield("limitPitchDegrees", QString::number(limitPitchDegrees, char('f'), 6), false);
        writedatafield("offsetHeadingDegrees", QString::number(offsetHeadingDegrees, char('f'), 6), false);
        writedatafield("offsetPitchDegrees", QString::number(offsetPitchDegrees, char('f'), 6), false);
        writedatafield("onGain", QString::number(onGain, char('f'), 6), false);
        writedatafield("offGain", QString::number(offGain, char('f'), 6), false);
        writedatafield("targetLocation", targetLocation.getValueAsString(), false);
        writedatafield("userInfo", QString::number(userInfo), false);
        writedatafield("directAtCamera", getBoolAsString(directAtCamera), false);
        writedatafield("directAtCameraX", QString::number(directAtCameraX, char('f'), 6), false);
        writedatafield("directAtCameraY", QString::number(directAtCameraY, char('f'), 6), false);
        writedatafield("directAtCameraZ", QString::number(directAtCameraZ, char('f'), 6), false);
        writedatafield("active", getBoolAsString(active), false);
        writedatafield("currentHeadingOffset", QString::number(currentHeadingOffset, char('f'), 6), false);
        writedatafield("currentPitchOffset", QString::number(currentPitchOffset, char('f'), 6), false);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSDirectAtModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSDirectAtModifier::unlink(){
    HkDynamicObject::unlink();
}

QString BSDirectAtModifier::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    auto evaluateindices = [&](int & bone, const QString & fieldname){
        if (bone >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
            isvalid = false;
            errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": "+fieldname+" out of range! Setting to last bone index!");
            bone = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones() - 1;
        }
    };
    QString temp = HkDynamicObject::evaluateDataValidity();
    (temp != "") ? errors.append(temp+getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid variable binding set!\n"): NULL;
    if (name == ""){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid name!");
    }
    evaluateindices(sourceBoneIndex, "sourceBoneIndex");
    evaluateindices(startBoneIndex, "startBoneIndex");
    evaluateindices(endBoneIndex, "endBoneIndex");
    setDataValidity(isvalid);
    return errors;
}

BSDirectAtModifier::~BSDirectAtModifier(){
    refCount--;
}
