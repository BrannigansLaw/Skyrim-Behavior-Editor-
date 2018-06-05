#include "hkbproxymodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbProxyModifier
*/

uint hkbProxyModifier::refCount = 0;

QString hkbProxyModifier::classname = "hkbProxyModifier";

QStringList hkbProxyModifier::PhantomType = {"PHANTOM_TYPE_SIMPLE", "PHANTOM_TYPE_CACHING"};
QStringList hkbProxyModifier::LinearVelocityMode = {"LINEAR_VELOCITY_MODE_WORLD", "LINEAR_VELOCITY_MODE_MODEL"};

hkbProxyModifier::hkbProxyModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      horizontalGain(0),
      verticalGain(0),
      maxHorizontalSeparation(0),
      limitHeadingDegrees(0),
      maxVerticalSeparation(0),
      verticalDisplacementError(0),
      verticalDisplacementErrorGain(0),
      maxVerticalDisplacement(0),
      minVerticalDisplacement(0),
      capsuleHeight(0),
      capsuleRadius(0),
      maxSlopeForRotation(0),
      collisionFilterInfo(0),
      ignoreIncomingRotation(true),
      ignoreCollisionDuringRotation(true),
      ignoreIncomingTranslation(true),
      includeDownwardMomentum(true),
      followWorldFromModel(true),
      isTouchingGround(true),
      phantomType(PhantomType.first()),
      linearVelocityMode(LinearVelocityMode.first())
{
    setType(HKB_PROXY_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "ProxyModifier"+QString::number(refCount);
}

QString hkbProxyModifier::getClassname(){
    return classname;
}

QString hkbProxyModifier::getName() const{
    return name;
}

bool hkbProxyModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "dynamicFriction"){
            proxyInfo.dynamicFriction = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'dynamicFriction' data field!\nObject Reference: "+ref);
            }
        }else if (text == "staticFriction"){
            proxyInfo.staticFriction = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'staticFriction' data field!\nObject Reference: "+ref);
            }
        }else if (text == "keepContactTolerance"){
            proxyInfo.keepContactTolerance = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'keepContactTolerance' data field!\nObject Reference: "+ref);
            }
        }else if (text == "up"){
            proxyInfo.up = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'up' data field!\nObject Reference: "+ref);
            }
        }else if (text == "keepDistance"){
            proxyInfo.keepDistance = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'keepDistance' data field!\nObject Reference: "+ref);
            }
        }else if (text == "contactAngleSensitivity"){
            proxyInfo.contactAngleSensitivity = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'contactAngleSensitivity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "userPlanes"){
            proxyInfo.userPlanes = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'userPlanes' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxCharacterSpeedForSolver"){
            proxyInfo.maxCharacterSpeedForSolver = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'maxCharacterSpeedForSolver' data field!\nObject Reference: "+ref);
            }
        }else if (text == "characterStrength"){
            proxyInfo.characterStrength = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'characterStrength' data field!\nObject Reference: "+ref);
            }
        }else if (text == "characterMass"){
            proxyInfo.characterMass = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'characterMass' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxSlope"){
            proxyInfo.maxSlope = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'maxSlope' data field!\nObject Reference: "+ref);
            }
        }else if (text == "penetrationRecoverySpeed"){
            proxyInfo.penetrationRecoverySpeed = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'penetrationRecoverySpeed' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxCastIterations"){
            proxyInfo.maxCastIterations = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'maxCastIterations' data field!\nObject Reference: "+ref);
            }
        }else if (text == "refreshManifoldInCheckSupport"){
            proxyInfo.refreshManifoldInCheckSupport = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'refreshManifoldInCheckSupport' data field!\nObject Reference: "+ref);
            }
        }else if (text == "linearVelocity"){
            linearVelocity = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'linearVelocity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "horizontalGain"){
            horizontalGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'horizontalGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "verticalGain"){
            verticalGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'verticalGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxHorizontalSeparation"){
            maxHorizontalSeparation = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'maxHorizontalSeparation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitHeadingDegrees"){
            limitHeadingDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'limitHeadingDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxVerticalSeparation"){
            maxVerticalSeparation = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'maxVerticalSeparation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "verticalDisplacementError"){
            verticalDisplacementError = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'verticalDisplacementError' data field!\nObject Reference: "+ref);
            }
        }else if (text == "verticalDisplacementErrorGain"){
            verticalDisplacementErrorGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'verticalDisplacementErrorGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxVerticalDisplacement"){
            maxVerticalDisplacement = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'maxVerticalDisplacement' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minVerticalDisplacement"){
            minVerticalDisplacement = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'minVerticalDisplacement' data field!\nObject Reference: "+ref);
            }
        }else if (text == "capsuleHeight"){
            capsuleHeight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'capsuleHeight' data field!\nObject Reference: "+ref);
            }
        }else if (text == "capsuleRadius"){
            capsuleRadius = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'capsuleRadius' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxSlopeForRotation"){
            maxSlopeForRotation = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'maxSlopeForRotation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "collisionFilterInfo"){
            collisionFilterInfo = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'collisionFilterInfo' data field!\nObject Reference: "+ref);
            }
        }else if (text == "phantomType"){
            phantomType = reader.getElementValueAt(index);
            if (!PhantomType.contains(phantomType)){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'phantomType' data field!\nObject Reference: "+ref);
            }
        }else if (text == "linearVelocityMode"){
            linearVelocityMode = reader.getElementValueAt(index);
            if (!LinearVelocityMode.contains(linearVelocityMode)){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'linearVelocityMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "ignoreIncomingRotation"){
            ignoreIncomingRotation = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'ignoreIncomingRotation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "ignoreCollisionDuringRotation"){
            ignoreCollisionDuringRotation = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'ignoreCollisionDuringRotation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "ignoreIncomingTranslation"){
            ignoreIncomingTranslation = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'ignoreIncomingTranslation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "includeDownwardMomentum"){
            includeDownwardMomentum = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'includeDownwardMomentum' data field!\nObject Reference: "+ref);
            }
        }else if (text == "followWorldFromModel"){
            followWorldFromModel = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'followWorldFromModel' data field!\nObject Reference: "+ref);
            }
        }else if (text == "isTouchingGround"){
            isTouchingGround = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'isTouchingGround' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbProxyModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("proxyInfo"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("dynamicFriction"), QString::number(proxyInfo.dynamicFriction, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("staticFriction"), QString::number(proxyInfo.staticFriction, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("keepContactTolerance"), QString::number(proxyInfo.keepContactTolerance));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("up"), proxyInfo.up.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("keepDistance"), QString::number(proxyInfo.keepDistance, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("contactAngleSensitivity"), QString::number(proxyInfo.contactAngleSensitivity, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("userPlanes"), QString::number(proxyInfo.userPlanes));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxCharacterSpeedForSolver"), QString::number(proxyInfo.maxCharacterSpeedForSolver, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("characterStrength"), QString::number(proxyInfo.characterStrength, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("characterMass"), QString::number(proxyInfo.characterMass, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxSlope"), QString::number(proxyInfo.maxSlope, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("penetrationRecoverySpeed"), QString::number(proxyInfo.penetrationRecoverySpeed, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxCastIterations"), QString::number(proxyInfo.maxCastIterations));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), getBoolAsString(proxyInfo.refreshManifoldInCheckSupport));
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("linearVelocity"), linearVelocity.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("horizontalGain"), QString::number(horizontalGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("verticalGain"), QString::number(verticalGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxHorizontalSeparation"), QString::number(maxHorizontalSeparation, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitHeadingDegrees"), QString::number(limitHeadingDegrees, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxVerticalSeparation"), QString::number(maxVerticalSeparation, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("verticalDisplacementError"), QString::number(verticalDisplacementError, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("verticalDisplacementErrorGain"), QString::number(verticalDisplacementErrorGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxVerticalDisplacement"), QString::number(maxVerticalDisplacement, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minVerticalDisplacement"), QString::number(minVerticalDisplacement, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("capsuleHeight"), QString::number(capsuleHeight, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("capsuleRadius"), QString::number(capsuleRadius, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxSlopeForRotation"), QString::number(maxSlopeForRotation, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("collisionFilterInfo"), QString::number(collisionFilterInfo));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("phantomType"), phantomType);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("linearVelocityMode"), linearVelocityMode);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("ignoreIncomingRotation"), getBoolAsString(ignoreIncomingRotation));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("ignoreCollisionDuringRotation"), getBoolAsString(ignoreCollisionDuringRotation));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("ignoreIncomingTranslation"), getBoolAsString(ignoreIncomingTranslation));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("includeDownwardMomentum"), getBoolAsString(includeDownwardMomentum));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("followWorldFromModel"), getBoolAsString(followWorldFromModel));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("isTouchingGround"), getBoolAsString(isTouchingGround));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbProxyModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbProxyModifier::unlink(){
    HkDynamicObject::unlink();
}

bool hkbProxyModifier::evaluateDataValidity(){
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (!PhantomType.contains(phantomType)){
    }else if (!LinearVelocityMode.contains(linearVelocityMode)){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbProxyModifier::~hkbProxyModifier(){
    refCount--;
}
