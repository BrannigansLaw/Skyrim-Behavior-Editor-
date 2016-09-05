#include "bscyclicblendtransitiongenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: BSCyclicBlendTransitionGenerator
*/

uint BSCyclicBlendTransitionGenerator::refCount = 0;

QString BSCyclicBlendTransitionGenerator::classname = "BSCyclicBlendTransitionGenerator";

QStringList BSCyclicBlendTransitionGenerator::BlendCurve = {"BLEND_CURVE_SMOOTH", "BLEND_CURVE_LINEAR", "BLEND_CURVE_LINEAR_TO_SMOOTH", "BLEND_CURVE_SMOOTH_TO_LINEAR"};

BSCyclicBlendTransitionGenerator::BSCyclicBlendTransitionGenerator(BehaviorFile *parent/*, qint16 ref*/)
    :hkbGenerator(parent/*, ref*/),
    userData(0),
    fBlendParameter(1),
    fTransitionDuration(0),
    eBlendCurve(BlendCurve.first())
{
    refCount++;
    setType(BS_CYCLIC_BLEND_TRANSITION_GENERATOR, TYPE_GENERATOR);
    name = "BS Cyclic Blend Transition Generator "+QString::number(refCount);
}

QString BSCyclicBlendTransitionGenerator::getClassname(){
    return classname;
}

QString BSCyclicBlendTransitionGenerator::getName() const{
    return name;
}

bool BSCyclicBlendTransitionGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pBlenderGenerator"){
            if (!pBlenderGenerator.readReference(index, reader)){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'pBlenderGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "EventToFreezeBlendValue"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToFreezeBlendValueId = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'eventToFreezeBlendValueId' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToFreezeBlendValuePayload.readReference(index, reader)){
                        writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'eventToFreezeBlendValuePayload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "EventToCrossBlend"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToCrossBlendId = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'eventToCrossBlendId' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToCrossBlendPayload.readReference(index, reader)){
                        writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'eventToCrossBlendPayload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "fBlendParameter"){
            fBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'fBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fTransitionDuration"){
            fTransitionDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'fTransitionDuration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eBlendCurve"){
            eBlendCurve = reader.getElementValueAt(index);
            if (eBlendCurve == ""){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'eBlendCurve' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSCyclicBlendTransitionGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("BSCyclicBlendTransitionGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //pBlenderGenerator
    HkxObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pBlenderGenerator.getReference());
    if (!ptr){
        writeToLog("BSCyclicBlendTransitionGenerator: link()!\nFailed to properly link 'pBlenderGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR){
        writeToLog("BSCyclicBlendTransitionGenerator: link()!\n'pBlenderGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pBlenderGenerator = *ptr;
    }else{
        pBlenderGenerator = *ptr;
    }
    return true;
}

void BSCyclicBlendTransitionGenerator::unlink(){
    HkDynamicObject::unlink();
    pBlenderGenerator = HkxObjectExpSharedPtr();
    eventToFreezeBlendValuePayload = HkxObjectExpSharedPtr();
    eventToCrossBlendPayload = HkxObjectExpSharedPtr();
}

bool BSCyclicBlendTransitionGenerator::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (!pBlenderGenerator.data() || pBlenderGenerator.data()->getSignature() != HKB_BLENDER_GENERATOR){
    }else if (eventToFreezeBlendValuePayload.data() && eventToFreezeBlendValuePayload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else if (eventToCrossBlendPayload.data() && eventToCrossBlendPayload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else if (!BlendCurve.contains(eBlendCurve)){
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSCyclicBlendTransitionGenerator::~BSCyclicBlendTransitionGenerator(){
    refCount--;
}
