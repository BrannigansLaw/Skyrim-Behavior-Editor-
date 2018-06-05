#include "hkbblendergenerator.h"
#include "hkbblendergeneratorchild.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbBlenderGenerator
*/
//{FLAG_SYNC=1, FLAG_SMOOTH_GENERATOR_WEIGHTS=4, FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS=8, FLAG_PARAMETRIC_BLEND=16, FLAG_IS_PARAMETRIC_BLEND_CYCLIC=32, FLAG_FORCE_DENSE_POSE=64};
uint hkbBlenderGenerator::refCount = 0;

QString hkbBlenderGenerator::classname = "hkbBlenderGenerator";

hkbBlenderGenerator::hkbBlenderGenerator(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(0),
      referencePoseWeightThreshold(0),
      blendParameter(1),
      minCyclicBlendParameter(0),
      maxCyclicBlendParameter(0),
      indexOfSyncMasterChild(-1),
      flags("0"),
      subtractLastChild(false)
{
    setType(HKB_BLENDER_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "BlenderGenerator"+QString::number(refCount);
}

QString hkbBlenderGenerator::getClassname(){
    return classname;
}

QString hkbBlenderGenerator::getName() const{
    return name;
}

int hkbBlenderGenerator::getNumberOfChildren() const{
    return children.size();
}

int hkbBlenderGenerator::getIndexToInsertIcon() const{
    for (int i = 0; i < children.size(); i++){
        if (!children.at(i).data() || !static_cast<hkbBlenderGeneratorChild *>(children.at(i).data())->generator.data()){
            return i;
        }
    }
    return -1;
}

bool hkbBlenderGenerator::insertObjectAt(int index, DataIconManager *obj){
    if (obj->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        if (index >= children.size() || index == -1){
            children.append(HkxSharedPtr(obj));
        }else if (index == 0 || !children.isEmpty()){
            children.replace(index, HkxSharedPtr(obj));
        }
        return true;
    }else{
        return false;
    }
}

bool hkbBlenderGenerator::removeObjectAt(int index){
    hkbBlenderGeneratorChild *objChild;
    if (index > -1 && index < children.size()){
        objChild = static_cast<hkbBlenderGeneratorChild *>(children.at(index).data());
        objChild->unlink();
        children.removeAt(index);
    }else if (index == -1){
        for (int i = 0; i < children.size(); i++){
            objChild = static_cast<hkbBlenderGeneratorChild *>(children.at(i).data());
            objChild->unlink();
        }
        children.clear();
    }else{
        return false;
    }
    return true;
}

bool hkbBlenderGenerator::hasChildren() const{
    for (int i = 0; i < children.size(); i++){
        if (static_cast<hkbBlenderGeneratorChild *>(children.at(i).data())){
            return true;
        }
    }
    return false;
}

int hkbBlenderGenerator::getIndexOfObj(DataIconManager *obj) const{
    for (int i = 0; i < children.size(); i++){
        if (children.at(i).data() == (HkxObject *)obj){
            return i;
        }
    }
    return -1;
}

int hkbBlenderGenerator::getIndexOfChild(hkbBlenderGeneratorChild *child) const{
    for (int i = 0; i < children.size(); i++){
        if (children.at(i).data() == child){
            return i;
        }
    }
    return -1;
}

bool hkbBlenderGenerator::isParametricBlend() const{
    if (flags.contains(FLAG_PARAMETRIC_BLEND)){
        return true;
    }
    return false;
}

bool hkbBlenderGenerator::merge(HkxObject *recessiveObject){
    hkbBlenderGenerator *recobj;
    hkbBlenderGeneratorChild *domchild;
    hkbBlenderGeneratorChild *recchild;
    if (recessiveObject && recessiveObject->getSignature() == HKB_BLENDER_GENERATOR){
        recobj = static_cast<hkbBlenderGenerator *>(recessiveObject);
        for (auto i = 0; i < children.size(); i++){
            domchild = static_cast<hkbBlenderGeneratorChild *>(children.at(i).data());
            for (auto j = 0; j < recobj->children.size(); j++){
                recchild = static_cast<hkbBlenderGeneratorChild *>(recobj->children.at(j).data());
                if (*recchild == *domchild){
                    domchild->injectWhileMerging((recchild));
                    break;
                }
            }
        }
    }
    return true;
}

QList<DataIconManager *> hkbBlenderGenerator::getChildren() const{
    QList<DataIconManager *> list;
    for (int i = 0; i < children.size(); i++){
        if (children.at(i).data()){
            list.append(static_cast<DataIconManager*>(children.at(i).data()));
        }
    }
    return list;
}

/*int hkbBlenderGenerator::generatorCount(hkbGenerator *gen){
    hkbBlenderGeneratorChild *child;
    int count = 0;
    for (int i = 0; i < children.size(); i++){
        child = static_cast<hkbBlenderGeneratorChild *>(children.at(i).data());
        if (child->generator.data() == gen){
            count++;
        }
    }
    return count;
}*/

bool hkbBlenderGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "referencePoseWeightThreshold"){
            referencePoseWeightThreshold = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'referencePoseWeightThreshold' data field!\nObject Reference: "+ref);
            }
        }else if (text == "blendParameter"){
            blendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'blendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minCyclicBlendParameter"){
            minCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'minCyclicBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxCyclicBlendParameter"){
            maxCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'maxCyclicBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "indexOfSyncMasterChild"){
            indexOfSyncMasterChild = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'indexOfSyncMasterChild' data field!\nObject Reference: "+ref);
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'flags' data field!\nObject Reference: "+ref);
            }
        }else if (text == "subtractLastChild"){
            subtractLastChild = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'subtractLastChild' data field!\nObject Reference: "+ref);
            }
        }else if (text == "children"){
            if (!readReferences(reader.getElementValueAt(index), children)){
                WRITE_TO_LOG(getClassname()+":  readData()!\nFailed to properly read 'children' references!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbBlenderGenerator::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("referencePoseWeightThreshold"), QString::number(referencePoseWeightThreshold, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("blendParameter"), QString::number(blendParameter, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minCyclicBlendParameter"), QString::number(minCyclicBlendParameter, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxCyclicBlendParameter"), QString::number(maxCyclicBlendParameter, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("indexOfSyncMasterChild"), QString::number(indexOfSyncMasterChild));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("flags"), flags);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("subtractLastChild"), getBoolAsString(subtractLastChild));
        refString = "";
        list1 = {writer->name, writer->numelements};
        list2 = {"children", QString::number(children.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0, j = 1; i < children.size(); i++, j++){
            refString.append(children.at(i).data()->getReferenceString());
            if (j % 16 == 0){
                refString.append("\n");
            }else{
                refString.append(" ");
            }
        }
        if (children.size() > 0){
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
            WRITE_TO_LOG(getClassname()+":  write()!\nUnable to write 'variableBindingSet'!!!");
        }
        for (int i = 0; i < children.size(); i++){
            if (children.at(i).data() && !children.at(i).data()->write(writer)){
                WRITE_TO_LOG(getClassname()+":  write()!\nUnable to write 'children' at: "+QString::number(i)+"!!!");
            }
        }
    }
    return true;
}

bool hkbBlenderGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG(getClassname()+":  link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < children.size(); i++){
        //ptr = static_cast<BehaviorFile *>(getParentFile())->findGeneratorChild(children.at(i).getReference());
        ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(children.at(i).getShdPtrReference());
        if (!ptr){
            WRITE_TO_LOG(getClassname()+":  link()!\nFailed to properly link 'children' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR_CHILD){
            WRITE_TO_LOG(getClassname()+":  link()!\n'children' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            children[i] = *ptr;
        }else{
            children[i] = *ptr;
            static_cast<hkbBlenderGeneratorChild *>(children[i].data())->parentBG = this;
        }
    }
    return true;
}

void hkbBlenderGenerator::unlink(){
    HkDynamicObject::unlink();
    for (int i = 0; i < children.size(); i++){
        if (children.at(i).data()){
            children[i].data()->unlink(); //Do here since this is not stored in the hkx file for long...
        }
        children[i] = HkxSharedPtr();
    }
}

bool hkbBlenderGenerator::evaluateDataValidity(){
    bool valid = true;
    for (int i = 0; i < children.size(); i++){
        if (!children.at(i).data() || children.at(i).data()->getSignature() != HKB_BLENDER_GENERATOR_CHILD){
            valid = false;
        }
    }
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (flags.toUInt(&valid) >= INVALID_FLAG || !valid){
    }else if (children.isEmpty()){
    }else if (valid){
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbBlenderGenerator::~hkbBlenderGenerator(){
    refCount--;
}
