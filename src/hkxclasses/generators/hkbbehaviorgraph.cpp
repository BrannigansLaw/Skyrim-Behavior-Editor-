#include "hkbbehaviorgraph.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: hkbBehaviorGraph
*/

uint hkbBehaviorGraph::refCount = 0;

QString hkbBehaviorGraph::classname = "hkbBehaviorGraph";

QStringList hkbBehaviorGraph::VariableMode = {"VARIABLE_MODE_DISCARD_WHEN_INACTIVE", "VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE"};

hkbBehaviorGraph::hkbBehaviorGraph(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(0)
{
    setType(HKB_BEHAVIOR_GRAPH, TYPE_GENERATOR);
    refCount++;
    name = "Behavior Graph "+QString::number(refCount);
}

QString hkbBehaviorGraph::getClassname(){
    return classname;
}

QString hkbBehaviorGraph::getName() const{
    return name;
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

bool hkbBehaviorGraph::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
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
