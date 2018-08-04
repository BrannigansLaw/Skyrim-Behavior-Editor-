#include "bssynchronizedclipgenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint BSSynchronizedClipGenerator::refCount = 0;

const QString BSSynchronizedClipGenerator::classname = "BSSynchronizedClipGenerator";

BSSynchronizedClipGenerator::BSSynchronizedClipGenerator(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
    userData(0),
    bSyncClipIgnoreMarkPlacement(false),
    fGetToMarkTime(0),
    fMarkErrorThreshold(0),
    syncAnimPrefix("&#9216;"),
    bLeadCharacter(false),
    bReorientSupportChar(false),
    bApplyMotionFromRoot(false),
    sAnimationBindingIndex(-1)
{
    setType(BS_SYNCHRONIZED_CLIP_GENERATOR, TYPE_GENERATOR);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "SynchronizedClipGenerator_"+QString::number(refCount);
}

const QString BSSynchronizedClipGenerator::getClassname(){
    return classname;
}

QString BSSynchronizedClipGenerator::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool BSSynchronizedClipGenerator::insertObjectAt(int , DataIconManager *obj){
    std::lock_guard <std::mutex> guard(mutex);
    if (obj && obj->getSignature() == HKB_CLIP_GENERATOR){
        pClipGenerator = HkxSharedPtr(obj);
        return true;
    }else{
        return false;
    }
}

bool BSSynchronizedClipGenerator::removeObjectAt(int index){
    std::lock_guard <std::mutex> guard(mutex);
    if (index == 0 || index == -1){
        pClipGenerator = HkxSharedPtr();
        return true;
    }
    return false;
}

bool BSSynchronizedClipGenerator::hasChildren() const{
    std::lock_guard <std::mutex> guard(mutex);
    if (pClipGenerator.data()){
        return true;
    }
    return false;
}

QVector<DataIconManager *> BSSynchronizedClipGenerator::getChildren() const{
    std::lock_guard <std::mutex> guard(mutex);
    QVector<DataIconManager *> list;
    if (pClipGenerator.data()){
        list.append(static_cast<DataIconManager*>(pClipGenerator.data()));
    }
    return list;
}

int BSSynchronizedClipGenerator::getIndexOfObj(DataIconManager *obj) const{
    std::lock_guard <std::mutex> guard(mutex);
    if (pClipGenerator.data() == obj){
        return 0;
    }
    return -1;
}

bool BSSynchronizedClipGenerator::readData(const HkxXmlReader &reader, long & index){
    std::lock_guard <std::mutex> guard(mutex);
    bool ok;
    QByteArray text;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    auto checkvalue = [&](bool value, const QString & fieldname){
        (!value) ? LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\n'"+fieldname+"' has invalid data!\nObject Reference: "+ref) : NULL;
    };
    for (; index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"; index++){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            checkvalue(getVariableBindingSet().readShdPtrReference(index, reader), "variableBindingSet");
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            checkvalue(ok, "userData");
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            checkvalue((name != ""), "name");
        }else if (text == "pClipGenerator"){
            checkvalue(pClipGenerator.readShdPtrReference(index, reader), "pClipGenerator");
        }else if (text == "SyncAnimPrefix"){
            syncAnimPrefix = reader.getElementValueAt(index);
            checkvalue((syncAnimPrefix == ""), "syncAnimPrefix");
        }else if (text == "bSyncClipIgnoreMarkPlacement"){
            bSyncClipIgnoreMarkPlacement = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "bSyncClipIgnoreMarkPlacement");
        }else if (text == "fGetToMarkTime"){
            fGetToMarkTime = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "fGetToMarkTime");
        }else if (text == "fMarkErrorThreshold"){
            fMarkErrorThreshold = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "fMarkErrorThreshold");
        }else if (text == "bLeadCharacter"){
            bLeadCharacter = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "bLeadCharacter");
        }else if (text == "bReorientSupportChar"){
            bReorientSupportChar = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "bReorientSupportChar");
        }else if (text == "bApplyMotionFromRoot"){
            bApplyMotionFromRoot = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "bApplyMotionFromRoot");
        }else if (text == "sAnimationBindingIndex"){
            sAnimationBindingIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "sAnimationBindingIndex");
        }else{
            //LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nUnknown field '"+text+"' found!\nObject Reference: "+ref);
        }
    }
    index--;
    return true;
}

bool BSSynchronizedClipGenerator::write(HkxXMLWriter *writer){
    std::lock_guard <std::mutex> guard(mutex);
    auto writedatafield = [&](const QString & name, const QString & value, bool allownull){
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), value, allownull);
    };
    auto writeref = [&](const HkxSharedPtr & shdptr, const QString & name){
        QString refString = "null";
        (shdptr.data()) ? refString = shdptr->getReferenceString() : NULL;
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), refString);
    };
    auto writechild = [&](const HkxSharedPtr & shdptr, const QString & datafield){
        if (shdptr.data() && !shdptr->write(writer))
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write '"+datafield+"'!!!\n");
    };
    if (writer && !getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writeref(getVariableBindingSet(), "variableBindingSet");
        writedatafield("userData", QString::number(userData), false);
        writedatafield("name", name, false);
        writeref(pClipGenerator, "pClipGenerator");
        writedatafield("syncAnimPrefix", syncAnimPrefix, false);
        writedatafield("bSyncClipIgnoreMarkPlacement", getBoolAsString(bSyncClipIgnoreMarkPlacement), false);
        writedatafield("fGetToMarkTime", QString::number(fGetToMarkTime, char('f'), 6), false);
        writedatafield("fMarkErrorThreshold", QString::number(fMarkErrorThreshold, char('f'), 6), false);
        writedatafield("bLeadCharacter", getBoolAsString(bLeadCharacter), false);
        writedatafield("bReorientSupportChar", getBoolAsString(bReorientSupportChar), false);
        writedatafield("bApplyMotionFromRoot", getBoolAsString(bApplyMotionFromRoot), false);
        writedatafield("sAnimationBindingIndex", QString::number(sAnimationBindingIndex), false);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        writechild(getVariableBindingSet(), "variableBindingSet");
        writechild(pClipGenerator, "pClipGenerator");
    }
    return true;
}

bool BSSynchronizedClipGenerator::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(pClipGenerator.getShdPtrReference());
    if (!ptr || !ptr->data()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'pClipGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_CLIP_GENERATOR){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\n'pClipGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pClipGenerator = *ptr;
    }else{
        pClipGenerator = *ptr;
    }
    return true;
}

void BSSynchronizedClipGenerator::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
    pClipGenerator = HkxSharedPtr();
}

QString BSSynchronizedClipGenerator::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    auto appenderror = [&](const QString & fieldname, const QString & errortype, HkxSignature sig){
        QString sigstring;
        if (sig != NULL_SIGNATURE)
            sigstring = " Signature of invalid type: "+QString::number(sig, 16);
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+fieldname+": "+errortype+"!"+sigstring+"\n");
    };
    if (HkDynamicObject::evaluateDataValidity() != ""){
        appenderror("variableBindingSet", "Invalid data", NULL_SIGNATURE);
    }
    if (name == ""){
        appenderror("name", "Invalid name", NULL_SIGNATURE);
    }
    if (!pClipGenerator.data()){
        appenderror("pClipGenerator", "Null pClipGenerator!", NULL_SIGNATURE);
    }else if (pClipGenerator->getSignature() != HKB_CLIP_GENERATOR){
        appenderror("pClipGenerator", "pClipGenerator is invalid type! Setting null value!", pClipGenerator->getSignature());
        pClipGenerator = HkxSharedPtr();
    }
    setDataValidity(isvalid);
    return errors;
}

BSSynchronizedClipGenerator::~BSSynchronizedClipGenerator(){
    refCount--;
}
