#include "bsoffsetanimationgenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: BSOffsetAnimationGenerator
*/

uint BSOffsetAnimationGenerator::refCount = 0;

QString BSOffsetAnimationGenerator::classname = "BSOffsetAnimationGenerator";

BSOffsetAnimationGenerator::BSOffsetAnimationGenerator(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(0),
      fOffsetVariable(0),
      fOffsetRangeStart(0),
      fOffsetRangeEnd(0)
{
    setType(BS_OFFSET_ANIMATION_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "OffsetAnimationGenerator"+QString::number(refCount);
}

QString BSOffsetAnimationGenerator::getClassname(){
    return classname;
}

QString BSOffsetAnimationGenerator::getName() const{
    return name;
}

int BSOffsetAnimationGenerator::getIndexToInsertIcon() const{
    if (!pDefaultGenerator.constData()){    //Not sure about this...
        return 0;
    }else if (!pOffsetClipGenerator.constData()){
        return 1;
    }
    return -1;
}

bool BSOffsetAnimationGenerator::insertObjectAt(int index, DataIconManager *obj){
    if (((HkxObject *)obj)->getSignature() == HKB_CLIP_GENERATOR && index == 1){
        pOffsetClipGenerator = HkxSharedPtr((HkxObject *)obj);
    }else if (((HkxObject *)obj)->getType() == TYPE_GENERATOR){
        pDefaultGenerator = HkxSharedPtr((HkxObject *)obj);
    }else{
        return false;
    }
    return true;
}

bool BSOffsetAnimationGenerator::removeObjectAt(int index){
    if (index == 0){
        pDefaultGenerator = HkxSharedPtr();
    }else if (index == 1){
        pOffsetClipGenerator = HkxSharedPtr();
    }else if (index == -1){
        pDefaultGenerator = HkxSharedPtr();
        pOffsetClipGenerator = HkxSharedPtr();
    }else{
        return false;
    }
    return true;
}

bool BSOffsetAnimationGenerator::hasChildren() const{
    if (pDefaultGenerator.data() || pOffsetClipGenerator.data()){
        return true;
    }
    return false;
}

QList<DataIconManager *> BSOffsetAnimationGenerator::getChildren() const{
    QList<DataIconManager *> list;
    if (pDefaultGenerator.data()){
        list.append(static_cast<DataIconManager*>(pDefaultGenerator.data()));
    }
    if (pOffsetClipGenerator.data()){
        list.append(static_cast<DataIconManager*>(pOffsetClipGenerator.data()));
    }
    return list;
}

int BSOffsetAnimationGenerator::getIndexOfObj(DataIconManager *obj) const{
    if (pDefaultGenerator.data() == (HkxObject *)obj){
        return 0;
    }else if (pOffsetClipGenerator.data() == (HkxObject *)obj){
        return 1;
    }
    return -1;
}

bool BSOffsetAnimationGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pDefaultGenerator"){
            if (!pDefaultGenerator.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'pDefaultGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "pOffsetClipGenerator"){
            if (!pOffsetClipGenerator.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'pOffsetClipGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetVariable"){
            fOffsetVariable = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'fOffsetVariable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetRangeStart"){
            fOffsetRangeStart = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'fOffsetRangeStart' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetRangeEnd"){
            fOffsetRangeEnd = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'fOffsetRangeEnd' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSOffsetAnimationGenerator::write(HkxXMLWriter *writer){
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
        if (pDefaultGenerator.data()){
            refString = pDefaultGenerator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pDefaultGenerator"), refString);
        if (pOffsetClipGenerator.data()){
            refString = pOffsetClipGenerator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pOffsetClipGenerator"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fOffsetVariable"), QString::number(fOffsetVariable, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fOffsetRangeStart"), QString::number(fOffsetRangeStart, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fOffsetRangeEnd"), QString::number(fOffsetRangeEnd, char('f'), 6));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (pDefaultGenerator.data() && !pDefaultGenerator.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": write()!\nUnable to write 'pDefaultGenerator'!!!");
        }
        if (pOffsetClipGenerator.data() && !pOffsetClipGenerator.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": write()!\nUnable to write 'pOffsetClipGenerator'!!!");
        }
    }
    return true;
}

bool BSOffsetAnimationGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(pDefaultGenerator.getShdPtrReference());
    if (!ptr){
        LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": link()!\nFailed to properly link 'pDefaultGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": link()!\n'pDefaultGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pDefaultGenerator = *ptr;
    }else{
        pDefaultGenerator = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(pOffsetClipGenerator.getShdPtrReference());
    if (!ptr){
        LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": link()!\nFailed to properly link 'pOffsetClipGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_CLIP_GENERATOR){
        LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": link()!\n'pOffsetClipGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pOffsetClipGenerator = *ptr;
    }else{
        pOffsetClipGenerator = *ptr;
    }
    return true;
}

void BSOffsetAnimationGenerator::unlink(){
    HkDynamicObject::unlink();
    pDefaultGenerator = HkxSharedPtr();
    pOffsetClipGenerator = HkxSharedPtr();
}

bool BSOffsetAnimationGenerator::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    if (!HkDynamicObject::evaluateDataValidity()){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (!pDefaultGenerator.data()){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null pDefaultGenerator!\n");
    }else if (pDefaultGenerator.data()->getType() != HkxObject::TYPE_GENERATOR){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid pDefaultGenerator type! Signature: "+QString::number(pDefaultGenerator.data()->getSignature(), 16)+"\n");
    }
    if (!pOffsetClipGenerator.data()){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null pOffsetClipGenerator!\n");
    }else if (pOffsetClipGenerator.data()->getSignature() != HKB_CLIP_GENERATOR){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid pOffsetClipGenerator type! Signature: "+QString::number(pOffsetClipGenerator.data()->getSignature(), 16)+"\n");
    }
    if (errors != ""){
        LogFile::writeToLog(errors);
    }
    setDataValidity(isvalid);
    return isvalid;
}

BSOffsetAnimationGenerator::~BSOffsetAnimationGenerator(){
    refCount--;
}
