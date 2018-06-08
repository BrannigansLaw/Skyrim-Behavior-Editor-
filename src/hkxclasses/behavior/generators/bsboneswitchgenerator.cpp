#include "bsboneswitchgenerator.h"
#include "bsboneswitchgeneratorbonedata.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: BSBoneSwitchGenerator
*/

uint BSBoneSwitchGenerator::refCount = 0;

QString BSBoneSwitchGenerator::classname = "BSBoneSwitchGenerator";

BSBoneSwitchGenerator::BSBoneSwitchGenerator(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(1)
{
    setType(BS_BONE_SWITCH_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "BoneSwitchGenerator"+QString::number(refCount);
}

QString BSBoneSwitchGenerator::getClassname(){
    return classname;
}

QString BSBoneSwitchGenerator::getName() const{
    return name;
}

int BSBoneSwitchGenerator::getIndexToInsertIcon() const{
    if (!pDefaultGenerator.constData()){    //Not sure... Need to determine source of change???
        return 0;
    }else{
        for (int i = 0; i < ChildrenA.size(); i++){
            if (!ChildrenA.at(i).constData()){
                return 1 + i;
            }
        }
    }
    return -1;
}

bool BSBoneSwitchGenerator::insertObjectAt(int index, DataIconManager *obj){
    if (((HkxObject *)obj)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA){
        if (index == 0){
            return false;
        }else if (index >= ChildrenA.size() || index == -1){
            ChildrenA.append(HkxSharedPtr(obj));
        }else if (index == 1 || !ChildrenA.isEmpty()){
            ChildrenA.replace(index - 1, HkxSharedPtr(obj));
        }
    }else if (((HkxObject *)obj)->getType() == TYPE_GENERATOR){
        if (index == 0){
            pDefaultGenerator = HkxSharedPtr((HkxObject *)obj);
        }else{
            return false;
        }
    }
    return true;
}

bool BSBoneSwitchGenerator::hasChildren() const{
    if (pDefaultGenerator.data()){
        return true;
    }
    for (int i = 0; i < ChildrenA.size(); i++){
        if (ChildrenA.at(i).data()){
            return true;
        }
    }
    return false;
}

bool BSBoneSwitchGenerator::hasGenerator() const{
    if (pDefaultGenerator.data()){
        return true;
    }
    return false;
}

QList<DataIconManager *> BSBoneSwitchGenerator::getChildren() const{
    QList<DataIconManager *> list;
    if (pDefaultGenerator.data()){
        list.append(static_cast<DataIconManager*>(pDefaultGenerator.data()));
    }
    for (int i = 0; i < ChildrenA.size(); i++){
        if (ChildrenA.at(i).data()){
            list.append(static_cast<DataIconManager*>(ChildrenA.at(i).data()));
        }
    }
    return list;
}

int BSBoneSwitchGenerator::getIndexOfObj(DataIconManager *obj) const{
    if (pDefaultGenerator.data() == (HkxObject *)obj){
        return 0;
    }else{
        for (int i = 0; i < ChildrenA.size(); i++){
            if (static_cast<BSBoneSwitchGeneratorBoneData *>(ChildrenA.at(i).data()) == (HkxObject *)obj){
                return i + 1;
            }
        }
    }
    return -1;
}

bool BSBoneSwitchGenerator::removeObjectAt(int index){
    BSBoneSwitchGeneratorBoneData *objChild;
    if (index == 0){
        pDefaultGenerator = HkxSharedPtr();
    }else if (index > -1 && index <= ChildrenA.size()){
        objChild = static_cast<BSBoneSwitchGeneratorBoneData *>(ChildrenA.at(index - 1).data());
        ChildrenA.removeAt(index - 1);
    }else if (index == -1){
        pDefaultGenerator = HkxSharedPtr();
        ChildrenA.clear();
    }else{
        return false;
    }
    return true;
}

bool BSBoneSwitchGenerator::merge(HkxObject *recessiveObject){
    BSBoneSwitchGenerator *recobj;
    BSBoneSwitchGeneratorBoneData *domchild;
    BSBoneSwitchGeneratorBoneData *recchild;
    hkbGenerator *domgen;
    hkbGenerator *recgen;
    DataIconManager *temp;
    QList <DataIconManager *> recchildren;
    if (recessiveObject && recessiveObject->getSignature() == BS_BONE_SWITCH_GENERATOR){
        recobj = static_cast<BSBoneSwitchGenerator *>(recessiveObject);
        domgen = static_cast<hkbGenerator *>(pDefaultGenerator.data());
        recgen = static_cast<hkbGenerator *>(recobj->pDefaultGenerator.data());
        if (domgen){
            if (recgen && (domgen->getName() != recgen->getName()) || (domgen->getSignature() != recgen->getSignature())){
                recchildren = static_cast<DataIconManager *>(recgen)->getChildren();
                for (auto i = 0; i < recchildren.size(); i++){
                    temp = recchildren.at(i);
                    if ((domgen->getName() == temp->getName()) && (domgen->getSignature() == temp->getSignature())){
                        pDefaultGenerator = HkxSharedPtr(recgen);
                        break;
                    }
                }
            }
        }else if (recgen){
            pDefaultGenerator = HkxSharedPtr(recgen);
        }
        for (auto i = 0; i < ChildrenA.size(); i++){
            domchild = static_cast<BSBoneSwitchGeneratorBoneData *>(ChildrenA.at(i).data());
            for (auto j = 0; j < recobj->ChildrenA.size(); j++){
                recchild = static_cast<BSBoneSwitchGeneratorBoneData *>(recobj->ChildrenA.at(j).data());
                if (*recchild == *domchild){
                    domchild->injectWhileMerging((recchild));
                    break;
                }
            }
        }
    }
    return true;
}

bool BSBoneSwitchGenerator::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "pDefaultGenerator"){
            if (!pDefaultGenerator.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'pDefaultGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "ChildrenA"){
            if (!readReferences(reader.getElementValueAt(index), ChildrenA)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'ChildrenA' references!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSBoneSwitchGenerator::write(HkxXMLWriter *writer){
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
        refString = "";
        list1 = {writer->name, writer->numelements};
        list2 = {"ChildrenA", QString::number(ChildrenA.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0, j = 1; i < ChildrenA.size(); i++, j++){
            refString.append(ChildrenA.at(i).data()->getReferenceString());
            if (j % 16 == 0){
                refString.append("\n");
            }else{
                refString.append(" ");
            }
        }
        if (ChildrenA.size() > 0){
            if (refString.endsWith(" \0")){
                refString.remove(refString.lastIndexOf(" "), 1);
            }
            writer->writeLine(refString);
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (pDefaultGenerator.data() && !pDefaultGenerator.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'pDefaultGenerator'!!!");
        }
        for (int i = 0; i < ChildrenA.size(); i++){
            if (ChildrenA.at(i).data() && !ChildrenA.at(i).data()->write(writer)){
                WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'ChildrenA' at: "+QString::number(i)+"!!!");
            }
        }
    }
    return true;
}

bool BSBoneSwitchGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!");
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(pDefaultGenerator.getShdPtrReference());
    if (!ptr){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'pDefaultGenerator' data field!");
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        WRITE_TO_LOG(getClassname()+": link()!\n'pDefaultGenerator' data field is linked to invalid child!");
        setDataValidity(false);
        pDefaultGenerator = *ptr;
    }else{
        pDefaultGenerator = *ptr;
    }
    for (int i = 0; i < ChildrenA.size(); i++){
        //ptr = static_cast<BehaviorFile *>(getParentFile())->findGeneratorChild(ChildrenA.at(i).getReference());
        ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(ChildrenA.at(i).getShdPtrReference());
        if (!ptr){
            WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'ChildrenA' data field!");
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != BS_BONE_SWITCH_GENERATOR_BONE_DATA){
            WRITE_TO_LOG(getClassname()+": link()!\n'ChildrenA' data field is linked to invalid child!");
            setDataValidity(false);
            ChildrenA[i] = *ptr;
        }else{
            ChildrenA[i] = *ptr;
            static_cast<BSBoneSwitchGeneratorBoneData *>(ChildrenA[i].data())->parentBSG = this;
        }
    }
    return true;
}

void BSBoneSwitchGenerator::unlink(){
    HkDynamicObject::unlink();
    pDefaultGenerator = HkxSharedPtr();
    for (int i = 0; i < ChildrenA.size(); i++){
        if (ChildrenA.at(i).data()){
            ChildrenA[i].data()->unlink(); //Do here since this is not stored in the hkx file for long...
        }
        ChildrenA[i] = HkxSharedPtr();
    }
}

bool BSBoneSwitchGenerator::evaluateDataValidity(){
    bool valid = true;
    for (int i = 0; i < ChildrenA.size(); i++){
        if (!ChildrenA.at(i).data() || ChildrenA.at(i).data()->getSignature() != BS_BONE_SWITCH_GENERATOR_BONE_DATA){
            valid = false;
        }
    }
    if (!HkDynamicObject::evaluateDataValidity() || name == "" || (!pDefaultGenerator.data() || pDefaultGenerator.data()->getType() != HkxObject::TYPE_GENERATOR) || ChildrenA.isEmpty() || !valid){
        setDataValidity(false);
        return false;
    }else{
        setDataValidity(true);
        return true;
    }
}

BSBoneSwitchGenerator::~BSBoneSwitchGenerator(){
    refCount--;
}
