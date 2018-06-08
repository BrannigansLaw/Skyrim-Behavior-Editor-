#include "hkbbehaviorgraph.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

#include <QExplicitlySharedDataPointer>
/*
 * CLASS: hkbBehaviorGraph
*/

uint hkbBehaviorGraph::refCount = 0;

QString hkbBehaviorGraph::classname = "hkbBehaviorGraph";

QStringList hkbBehaviorGraph::VariableMode = {"VARIABLE_MODE_DISCARD_WHEN_INACTIVE", "VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE"};

hkbBehaviorGraph::hkbBehaviorGraph(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(0),
      variableMode(VariableMode.first())
{
    setType(HKB_BEHAVIOR_GRAPH, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "BehaviorGraph"+QString::number(refCount);
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

bool hkbBehaviorGraph::insertObjectAt(int , DataIconManager *obj){
    if (((HkxObject *)obj)->getSignature() == HKB_STATE_MACHINE){
        rootGenerator = HkxSharedPtr((HkxObject *)obj);
        return true;
    }else{
        return false;
    }
}

bool hkbBehaviorGraph::removeObjectAt(int index){
    if (index == 0 || index == -1){
        rootGenerator = HkxSharedPtr();
    }else{
        return false;
    }
    return true;
}

bool hkbBehaviorGraph::hasChildren() const{
    if (rootGenerator.data()){
        return true;
    }
    return false;
}

QList<DataIconManager *> hkbBehaviorGraph::getChildren() const{
    QList<DataIconManager *> list;
    if (rootGenerator.data()){
        list.append(static_cast<DataIconManager*>(rootGenerator.data()));
    }
    return list;
}

int hkbBehaviorGraph::getIndexOfObj(DataIconManager *obj) const{
    if (rootGenerator.data() == (HkxObject *)obj){
        return 0;
    }
    return -1;
}

bool hkbBehaviorGraph::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                WRITE_TO_LOG("hkbBehaviorGraph: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                WRITE_TO_LOG("hkbBehaviorGraph: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                WRITE_TO_LOG("hkbBehaviorGraph: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "variableMode"){
            variableMode = reader.getElementValueAt(index);
            if (variableMode == ""){
                WRITE_TO_LOG("hkbBehaviorGraph: readData()!\nFailed to properly read 'variableMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rootGenerator"){
            if (!rootGenerator.readShdPtrReference(index, reader)){
                WRITE_TO_LOG("hkbBehaviorGraph: readData()!\nFailed to properly read 'rootGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "data"){
            if (!data.readShdPtrReference(index, reader)){
                WRITE_TO_LOG("hkbBehaviorGraph: readData()!\nFailed to properly read 'data' reference!\nObject Reference: "+ref);
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
            WRITE_TO_LOG("hkbBehaviorGraph: write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (rootGenerator.data() && !rootGenerator.data()->write(writer)){
            WRITE_TO_LOG("hkbBehaviorGraph: write()!\nUnable to write 'rootGenerator'!!!");
        }
        if (data.data() && !data.data()->write(writer)){
            WRITE_TO_LOG("hkbBehaviorGraph: write()!\nUnable to write 'data'!!!");
        }
    }
    return true;
}

bool hkbBehaviorGraph::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG("hkbBehaviorGraph: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(rootGenerator.getShdPtrReference());
    if (!ptr){
        WRITE_TO_LOG("hkbBehaviorGraph: link()!\nFailed to properly link 'rootGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_STATE_MACHINE){
        WRITE_TO_LOG("hkbBehaviorGraph: link()!\n'rootGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        rootGenerator = *ptr;
    }else{
        rootGenerator = *ptr;
    }
    ptr = &static_cast<BehaviorFile *>(getParentFile())->graphData;
    if (!ptr){
        WRITE_TO_LOG("hkbBehaviorGraph: link()!\nFailed to properly link 'data' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_BEHAVIOR_GRAPH_DATA){
        WRITE_TO_LOG("hkbBehaviorGraph: link()!\n'data' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        data = *ptr;
    }else{
        data = *ptr;
    }
    return true;
}

void hkbBehaviorGraph::unlink(){
    HkDynamicObject::unlink();
    rootGenerator = HkxSharedPtr();
    data = HkxSharedPtr();
}

bool hkbBehaviorGraph::evaluateDataValidity(){
    if (!HkDynamicObject::evaluateDataValidity() || (!rootGenerator.data() || rootGenerator.data()->getSignature() != HKB_STATE_MACHINE) || (!data.data() || data.data()->getSignature() != HKB_BEHAVIOR_GRAPH_DATA) || (!VariableMode.contains(variableMode))){
        setDataValidity(false);
        return false;
    }else{
        setDataValidity(true);
        return true;
    }
}

hkbBehaviorGraph::~hkbBehaviorGraph(){
    refCount--;
}
