#include "hkbbehaviorgraphstringdata.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbBehaviorGraphStringData
*/

uint hkbBehaviorGraphStringData::refCount = 0;

hkbBehaviorGraphStringData::hkbBehaviorGraphStringData(BehaviorFile *parent/*, qint16 ref*/)
    : HkxObject(parent/*, ref*/)
{
    setType(HKB_BEHAVIOR_GRAPH_STRING_DATA, TYPE_OTHER);
    refCount++;
}

bool hkbBehaviorGraphStringData::readData(const HkxXmlReader &reader, long index){
    bool ok;
    int numElems = 0;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "eventNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'eventNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'eventNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                eventNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "attributeNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'attributeNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'attributeNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                attributeNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "variableNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'variableNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'variableNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                variableNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "characterPropertyNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'characterPropertyNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'characterPropertyNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                characterPropertyNames.append(reader.getElementValueAt(index));
            }
            continue;
        }
        index++;
    }
    return true;
}

bool hkbBehaviorGraphStringData::link(){
    return true;
}

hkbBehaviorGraphStringData::~hkbBehaviorGraphStringData(){refCount--;}
