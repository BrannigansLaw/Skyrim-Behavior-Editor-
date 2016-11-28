#include "hkbbehaviorgraph.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: hkbBehaviorGraph
*/

uint hkbBehaviorGraph::refCount = 0;

QString hkbBehaviorGraph::classname = "hkbBehaviorGraph";

QStringList hkbBehaviorGraph::VariableMode = {"VARIABLE_MODE_DISCARD_WHEN_INACTIVE", "VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE"};

hkbBehaviorGraph::hkbBehaviorGraph(BehaviorFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(0)
{
    setType(HKB_BEHAVIOR_GRAPH, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);refCount++;
    name = "Behavior Graph "+QString::number(refCount);
}

QString hkbBehaviorGraph::getClassname(){
    return classname;
}

QString hkbBehaviorGraph::getName() const{
    return name;
}

int hkbBehaviorGraph::getIndexToInsertIcon() const{
    if (!rootGenerator.constData()){
        return 0;
    }
    return -1;
}

bool hkbBehaviorGraph::setChildAt(HkxObject *newChild, ushort index){
    if (index == 0 && (!newChild || newChild->getSignature() == HKB_STATE_MACHINE)){
        rootGenerator = HkxObjectExpSharedPtr(newChild);
        return true;
    }else{
        return false;
    }
}

bool hkbBehaviorGraph::wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace){
    if (rootGenerator.data() == childToReplace && (!objToInject || objToInject->getSignature() == HKB_STATE_MACHINE)){
        if (!objToInject->setChildAt(rootGenerator.data())){
            return false;
        }
        rootGenerator = HkxObjectExpSharedPtr(objToInject);
        return true;
    }
    return false;
}

bool hkbBehaviorGraph::appendObject(DataIconManager *objToAppend){
    if (objToAppend->getSignature() == HKB_STATE_MACHINE){
        rootGenerator = HkxObjectExpSharedPtr(objToAppend);
        return true;
    }
    return false;
}

bool hkbBehaviorGraph::removeObject(DataIconManager *objToRemove, bool removeAll){
    if (rootGenerator.data() == objToRemove){
        rootGenerator = HkxObjectExpSharedPtr();
        return true;
    }
    return false;
}

bool hkbBehaviorGraph::hasChildren() const{
    if (rootGenerator.data()){
        return true;
    }
    return false;
}

int hkbBehaviorGraph::addChildrenToList(QList <HkxObjectExpSharedPtr> & list, bool reverseOrder){
    int objectChildCount = 0;
    if (rootGenerator.data()){
        list.append(rootGenerator);
        objectChildCount++;
    }
    return objectChildCount;
}

bool hkbBehaviorGraph::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "variableMode"){
            variableMode = reader.getElementValueAt(index);
            if (variableMode == ""){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'variableMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rootGenerator"){
            if (!rootGenerator.readReference(index, reader)){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'rootGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "data"){
            if (!data.readReference(index, reader)){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'data' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbBehaviorGraph::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("variableMode"), variableMode);
        if (rootGenerator.data()){
            refString = rootGenerator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rootGenerator"), refString);
        if (data.data()){
            refString = data.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("data"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog("hkbBehaviorGraph: write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (rootGenerator.data() && !rootGenerator.data()->write(writer)){
            getParentFile()->writeToLog("hkbBehaviorGraph: write()!\nUnable to write 'rootGenerator'!!!", true);
        }
        if (data.data() && !data.data()->write(writer)){
            getParentFile()->writeToLog("hkbBehaviorGraph: write()!\nUnable to write 'data'!!!", true);
        }
    }
    return true;
}

bool hkbBehaviorGraph::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<DataIconManager *>(this)->linkVar()){
        writeToLog("hkbBehaviorGraph: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxObjectExpSharedPtr *ptr = getParentFile()->findGenerator(rootGenerator.getReference());
    if (!ptr){
        writeToLog("hkbBehaviorGraph: link()!\nFailed to properly link 'rootGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_STATE_MACHINE){
        writeToLog("hkbBehaviorGraph: link()!\n'rootGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        rootGenerator = *ptr;
    }else{
        rootGenerator = *ptr;
    }
    ptr = &getParentFile()->graphData;
    if (!ptr){
        writeToLog("hkbBehaviorGraph: link()!\nFailed to properly link 'data' data field!\nObject Name: "+name, true);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_BEHAVIOR_GRAPH_DATA){
        writeToLog("hkbBehaviorGraph: link()!\n'data' data field is linked to invalid child!\nObject Name: "+name, true);
        setDataValidity(false);
        data = *ptr;
    }else{
        data = *ptr;
    }
    return true;
}

void hkbBehaviorGraph::unlink(){
    HkDynamicObject::unlink();
    rootGenerator = HkxObjectExpSharedPtr();
    data = HkxObjectExpSharedPtr();
}

bool hkbBehaviorGraph::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (!rootGenerator.data() || rootGenerator.data()->getSignature() != HKB_STATE_MACHINE){
    }else if (!data.data() || data.data()->getSignature() != HKB_BEHAVIOR_GRAPH_DATA){
    }else if (!VariableMode.contains(variableMode)){
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbBehaviorGraph::~hkbBehaviorGraph(){
    refCount--;
}
