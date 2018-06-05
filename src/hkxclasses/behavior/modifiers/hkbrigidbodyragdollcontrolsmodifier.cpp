#include "hkbrigidbodyragdollcontrolsmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbRigidBodyRagdollControlsModifier
*/

uint hkbRigidBodyRagdollControlsModifier::refCount = 0;

QString hkbRigidBodyRagdollControlsModifier::classname = "hkbRigidBodyRagdollControlsModifier";

hkbRigidBodyRagdollControlsModifier::hkbRigidBodyRagdollControlsModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      hierarchyGain(0),
      velocityDamping(0),
      accelerationGain(0),
      velocityGain(0),
      positionGain(0),
      positionMaxLinearVelocity(0),
      positionMaxAngularVelocity(0),
      snapGain(0),
      snapMaxLinearVelocity(0),
      snapMaxAngularVelocity(0),
      snapMaxLinearDistance(0),
      snapMaxAngularDistance(0),
      durationToBlend(0)
{
    setType(HKB_RIGID_BODY_RAGDOLL_CONTROLS_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "RigidBodyRagdollControlsModifier"+QString::number(refCount);
}

QString hkbRigidBodyRagdollControlsModifier::getClassname(){
    return classname;
}

QString hkbRigidBodyRagdollControlsModifier::getName() const{
    return name;
}

bool hkbRigidBodyRagdollControlsModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "hierarchyGain"){
            hierarchyGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'hierarchyGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "velocityDamping"){
            velocityDamping = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'velocityDamping' data field!\nObject Reference: "+ref);
            }
        }else if (text == "accelerationGain"){
            accelerationGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'accelerationGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "velocityGain"){
            velocityGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'velocityGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "positionGain"){
            positionGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'positionGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "positionMaxLinearVelocity"){
            positionMaxLinearVelocity = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'positionMaxLinearVelocity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "positionMaxAngularVelocity"){
            positionMaxAngularVelocity = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'positionMaxAngularVelocity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "snapGain"){
            snapGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'snapGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "snapMaxLinearVelocity"){
            snapMaxLinearVelocity = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'snapMaxLinearVelocity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "snapMaxAngularVelocity"){
            snapMaxAngularVelocity = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'snapMaxAngularVelocity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "snapMaxLinearDistance"){
            snapMaxLinearDistance = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'snapMaxLinearDistance' data field!\nObject Reference: "+ref);
            }
        }else if (text == "snapMaxAngularDistance"){
            snapMaxAngularDistance = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'snapMaxAngularDistance' data field!\nObject Reference: "+ref);
            }
        }else if (text == "durationToBlend"){
            durationToBlend = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'durationToBlend' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bones"){
            if (!bones.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'bones' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbRigidBodyRagdollControlsModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("controlData"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("keyFrameHierarchyControlData"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("hierarchyGain"), QString::number(hierarchyGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("velocityDamping"), QString::number(velocityDamping, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("accelerationGain"), QString::number(accelerationGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("velocityGain"), QString::number(velocityGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("positionGain"), QString::number(positionGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("positionMaxLinearVelocity"), QString::number(positionMaxLinearVelocity, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("positionMaxAngularVelocity"), QString::number(positionMaxAngularVelocity, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("snapGain"), QString::number(snapGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("snapMaxLinearVelocity"), QString::number(snapMaxLinearVelocity, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("snapMaxAngularVelocity"), QString::number(snapMaxAngularVelocity, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("snapMaxLinearDistance"), QString::number(snapMaxLinearDistance, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("snapMaxAngularDistance"), QString::number(snapMaxAngularDistance, char('f'), 6));
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("durationToBlend"), QString::number(durationToBlend, char('f'), 6));
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        if (bones.data()){
            refString = bones.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bones"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (bones.data() && !bones.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'bones'!!!");
        }
    }
    return true;
}

void hkbRigidBodyRagdollControlsModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (bones.data()){
        ref++;
        bones.data()->updateReferences(ref);
    }
}

bool hkbRigidBodyRagdollControlsModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(bones.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_INDEX_ARRAY){
            WRITE_TO_LOG(getClassname()+": linkVar()!\nThe linked object 'bones' is not a HKB_BONE_INDEX_ARRAY!");
            setDataValidity(false);
        }
        bones = *ptr;
    }
    return true;
}

void hkbRigidBodyRagdollControlsModifier::unlink(){
    HkDynamicObject::unlink();
    bones = HkxSharedPtr();
}

bool hkbRigidBodyRagdollControlsModifier::evaluateDataValidity(){
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (!bones.data()){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbRigidBodyRagdollControlsModifier::~hkbRigidBodyRagdollControlsModifier(){
    refCount--;
}
