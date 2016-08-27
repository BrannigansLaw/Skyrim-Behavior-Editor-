#include "bssynchronizedclipgenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: BSSynchronizedClipGenerator
*/

uint BSSynchronizedClipGenerator::refCount = 0;

BSSynchronizedClipGenerator::BSSynchronizedClipGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
    userData(0),
    bSyncClipIgnoreMarkPlacement(false),
    fGetToMarkTime(0),
    fMarkErrorThreshold(0),
    bLeadCharacter(false),
    bReorientSupportChar(false),
    bApplyMotionFromRoot(false),
    sAnimationBindingIndex(-1)
{
    refCount++;
    setType(BS_SYNCHRONIZED_CLIP_GENERATOR, TYPE_GENERATOR);
    name = "BS Synchronized Clip Generator "+QString::number(refCount);
}

QString BSSynchronizedClipGenerator::getName() const{
    return name;
}

bool BSSynchronizedClipGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pClipGenerator"){
            if (!pClipGenerator.readReference(index, reader)){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'pClipGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "SyncAnimPrefix"){
            SyncAnimPrefix = reader.getElementValueAt(index);
            if (SyncAnimPrefix == ""){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'SyncAnimPrefix' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bSyncClipIgnoreMarkPlacement"){
            bSyncClipIgnoreMarkPlacement = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'bSyncClipIgnoreMarkPlacement' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fGetToMarkTime"){
            fGetToMarkTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'fGetToMarkTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fMarkErrorThreshold"){
            fMarkErrorThreshold = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'fMarkErrorThreshold' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bLeadCharacter"){
            bLeadCharacter = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'bLeadCharacter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bReorientSupportChar"){
            bReorientSupportChar = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'bReorientSupportChar' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bApplyMotionFromRoot"){
            bApplyMotionFromRoot = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'bApplyMotionFromRoot' data field!\nObject Reference: "+ref);
            }
        }else if (text == "sAnimationBindingIndex"){
            sAnimationBindingIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'sAnimationBindingIndex' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSSynchronizedClipGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("BSSynchronizedClipGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //pClipGenerator
    HkxObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pClipGenerator.getReference());
    if (!ptr){
        writeToLog("BSSynchronizedClipGenerator: link()!\nFailed to properly link 'pClipGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_CLIP_GENERATOR){
        writeToLog("BSSynchronizedClipGenerator: link()!\n'pClipGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pClipGenerator = *ptr;
    }else{
        pClipGenerator = *ptr;
    }
    return true;
}

void BSSynchronizedClipGenerator::unlink(){
    HkDynamicObject::unlink();
    pClipGenerator = HkxObjectExpSharedPtr();
}

BSSynchronizedClipGenerator::~BSSynchronizedClipGenerator(){
    refCount--;
}
