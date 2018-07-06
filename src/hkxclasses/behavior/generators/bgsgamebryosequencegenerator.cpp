#include "bgsgamebryosequencegenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: BGSGamebryoSequenceGenerator
*/

uint BGSGamebryoSequenceGenerator::refCount = 0;

QString BGSGamebryoSequenceGenerator::classname = "BGSGamebryoSequenceGenerator";

QStringList BGSGamebryoSequenceGenerator::BlendModeFunction = {"BMF_NONE", "BMF_PERCENT", "BMF_ONE_MINUS_PERCENT"};

BGSGamebryoSequenceGenerator::BGSGamebryoSequenceGenerator(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(0)
{
    setType(BGS_GAMEBYRO_SEQUENCE_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "BGSGamebryoSequenceGenerator"+QString::number(refCount);
}

QString BGSGamebryoSequenceGenerator::getClassname(){
    return classname;
}

bool BGSGamebryoSequenceGenerator::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "pSequence"){
            pSequence = reader.getElementValueAt(index);
            if (pSequence == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'pSequence' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eBlendModeFunction"){
            eBlendModeFunction = reader.getElementValueAt(index);
            if (!BlendModeFunction.contains(eBlendModeFunction)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'eBlendModeFunction' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fPercent"){
            fPercent = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'fPercent' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BGSGamebryoSequenceGenerator::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pSequence"), pSequence, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eBlendModeFunction"), eBlendModeFunction);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fPercent"), QString::number(fPercent, char('f'), 6));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BGSGamebryoSequenceGenerator::link(){
    return true;
}

QString BGSGamebryoSequenceGenerator::getName() const{
    return name;
}

QString BGSGamebryoSequenceGenerator::evaluateDataValidity(){
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
    if (pSequence == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid pSequence!\n");
    }
    if (!BlendModeFunction.contains(eBlendModeFunction)){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid eBlendModeFunction! Setting default value!\n");
        eBlendModeFunction = BlendModeFunction.first();
    }
    setDataValidity(isvalid);
    return errors;
}

BGSGamebryoSequenceGenerator::~BGSGamebryoSequenceGenerator(){
    refCount--;
}
