#include "bssynchronizedclipgenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: BSSynchronizedClipGenerator
*/

uint BSSynchronizedClipGenerator::refCount = 0;

QString BSSynchronizedClipGenerator::classname = "BSSynchronizedClipGenerator";

BSSynchronizedClipGenerator::BSSynchronizedClipGenerator(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
    userData(0),
    bSyncClipIgnoreMarkPlacement(false),
    fGetToMarkTime(0),
    fMarkErrorThreshold(0),
    bLeadCharacter(false),
    bReorientSupportChar(false),
    bApplyMotionFromRoot(false),
    sAnimationBindingIndex(-1)
{
    setType(BS_SYNCHRONIZED_CLIP_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "SynchronizedClipGenerator"+QString::number(refCount);
}

QString BSSynchronizedClipGenerator::getClassname(){
    return classname;
}

QString BSSynchronizedClipGenerator::getName() const{
    return name;
}

bool BSSynchronizedClipGenerator::insertObjectAt(int , DataIconManager *obj){
    if (((HkxObject *)obj)->getSignature() == HKB_CLIP_GENERATOR){
        pClipGenerator = HkxSharedPtr((HkxObject *)obj);
        return true;
    }else{
        return false;
    }
}

bool BSSynchronizedClipGenerator::removeObjectAt(int index){
    if (index == 0 || index == -1){
        pClipGenerator = HkxSharedPtr();
    }else{
        return false;
    }
    return true;
}

bool BSSynchronizedClipGenerator::hasChildren() const{
    if (pClipGenerator.data()){
        return true;
    }
    return false;
}

QList<DataIconManager *> BSSynchronizedClipGenerator::getChildren() const{
    QList<DataIconManager *> list;
    if (pClipGenerator.data()){
        list.append(static_cast<DataIconManager*>(pClipGenerator.data()));
    }
    return list;
}

int BSSynchronizedClipGenerator::getIndexOfObj(DataIconManager *obj) const{
    if (pClipGenerator.data() == (HkxObject *)obj){
        return 0;
    }
    return -1;
}

bool BSSynchronizedClipGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pClipGenerator"){
            if (!pClipGenerator.readShdPtrReference(index, reader)){
                LogFile::writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'pClipGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "SyncAnimPrefix"){
            syncAnimPrefix = reader.getElementValueAt(index);
            if (syncAnimPrefix == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'SyncAnimPrefix' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bSyncClipIgnoreMarkPlacement"){
            bSyncClipIgnoreMarkPlacement = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'bSyncClipIgnoreMarkPlacement' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fGetToMarkTime"){
            fGetToMarkTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'fGetToMarkTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fMarkErrorThreshold"){
            fMarkErrorThreshold = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'fMarkErrorThreshold' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bLeadCharacter"){
            bLeadCharacter = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'bLeadCharacter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bReorientSupportChar"){
            bReorientSupportChar = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'bReorientSupportChar' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bApplyMotionFromRoot"){
            bApplyMotionFromRoot = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'bApplyMotionFromRoot' data field!\nObject Reference: "+ref);
            }
        }else if (text == "sAnimationBindingIndex"){
            sAnimationBindingIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'sAnimationBindingIndex' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSSynchronizedClipGenerator::write(HkxXMLWriter *writer){
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
        if (pClipGenerator.data()){
            refString = pClipGenerator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pClipGenerator"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("SyncAnimPrefix"), syncAnimPrefix);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bSyncClipIgnoreMarkPlacement"), getBoolAsString(bSyncClipIgnoreMarkPlacement));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fGetToMarkTime"), QString::number(fGetToMarkTime, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fMarkErrorThreshold"), QString::number(fMarkErrorThreshold, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bLeadCharacter"), getBoolAsString(bLeadCharacter));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bReorientSupportChar"), getBoolAsString(bReorientSupportChar));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bApplyMotionFromRoot"), getBoolAsString(bApplyMotionFromRoot));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sAnimationBindingIndex"), QString::number(sAnimationBindingIndex));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog("hkbClipGenerator: write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (pClipGenerator.data() && !pClipGenerator.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'pClipGenerator'!!!");
        }
    }
    return true;
}

bool BSSynchronizedClipGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(pClipGenerator.getShdPtrReference());
    if (!ptr){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'pClipGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_CLIP_GENERATOR){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\n'pClipGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pClipGenerator = *ptr;
    }else{
        pClipGenerator = *ptr;
    }
    return true;
}

void BSSynchronizedClipGenerator::unlink(){
    HkDynamicObject::unlink();
    pClipGenerator = HkxSharedPtr();
}

QString BSSynchronizedClipGenerator::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (!pClipGenerator.data()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null pClipGenerator!\n");
    }else if (pClipGenerator.data()->getSignature() != HKB_CLIP_GENERATOR){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid pClipGenerator type! Signature: "+QString::number(pClipGenerator.data()->getSignature(), 16)+" Setting null value!\n");
        pClipGenerator = HkxSharedPtr();
    }
    setDataValidity(isvalid);
    return errors;
}

BSSynchronizedClipGenerator::~BSSynchronizedClipGenerator(){
    refCount--;
}
