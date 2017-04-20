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
    name = "BSOffsetAnimationGenerator"+QString::number(refCount);
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
        pOffsetClipGenerator = HkxObjectExpSharedPtr((HkxObject *)obj);
    }else if (((HkxObject *)obj)->getType() == TYPE_GENERATOR){
        pDefaultGenerator = HkxObjectExpSharedPtr((HkxObject *)obj);
    }else{
        return false;
    }
    return true;
}

bool BSOffsetAnimationGenerator::removeObjectAt(int index){
    if (index == 0){
        pDefaultGenerator = HkxObjectExpSharedPtr();
    }else if (index == 1){
        pOffsetClipGenerator = HkxObjectExpSharedPtr();
    }else if (index == -1){
        pDefaultGenerator = HkxObjectExpSharedPtr();
        pOffsetClipGenerator = HkxObjectExpSharedPtr();
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
        list.append((DataIconManager *)pDefaultGenerator.data());
    }
    if (pOffsetClipGenerator.data()){
        list.append((DataIconManager *)pOffsetClipGenerator.data());
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
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pDefaultGenerator"){
            if (!pDefaultGenerator.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'pDefaultGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "pOffsetClipGenerator"){
            if (!pOffsetClipGenerator.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'pOffsetClipGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetVariable"){
            fOffsetVariable = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'fOffsetVariable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetRangeStart"){
            fOffsetRangeStart = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'fOffsetRangeStart' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetRangeEnd"){
            fOffsetRangeEnd = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'fOffsetRangeEnd' data field!\nObject Reference: "+ref);
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fOffsetVariable"), QString::number(fOffsetVariable));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fOffsetVariable"), QString::number(fOffsetRangeStart));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fOffsetVariable"), QString::number(fOffsetRangeEnd));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (pDefaultGenerator.data() && !pDefaultGenerator.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'pDefaultGenerator'!!!", true);
        }
        if (pOffsetClipGenerator.data() && !pOffsetClipGenerator.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'pOffsetClipGenerator'!!!", true);
        }
    }
    return true;
}

bool BSOffsetAnimationGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxObjectExpSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(pDefaultGenerator.getReference());
    if (!ptr){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'pDefaultGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog(getClassname()+": link()!\n'pDefaultGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pDefaultGenerator = *ptr;
    }else{
        pDefaultGenerator = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(pOffsetClipGenerator.getReference());
    if (!ptr){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'pOffsetClipGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_CLIP_GENERATOR){
        writeToLog(getClassname()+": link()!\n'pOffsetClipGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pOffsetClipGenerator = *ptr;
    }else{
        pOffsetClipGenerator = *ptr;
    }
    return true;
}

void BSOffsetAnimationGenerator::unlink(){
    HkDynamicObject::unlink();
    pDefaultGenerator = HkxObjectExpSharedPtr();
    pOffsetClipGenerator = HkxObjectExpSharedPtr();
}

bool BSOffsetAnimationGenerator::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (!pDefaultGenerator.data() || pDefaultGenerator.data()->getType() != HkxObject::TYPE_GENERATOR){
    }else if (!pOffsetClipGenerator.data() || pOffsetClipGenerator.data()->getSignature() != HKB_CLIP_GENERATOR){
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSOffsetAnimationGenerator::~BSOffsetAnimationGenerator(){
    refCount--;
}
