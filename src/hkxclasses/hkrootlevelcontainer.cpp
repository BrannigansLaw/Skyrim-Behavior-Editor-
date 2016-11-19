#include "hkrootlevelcontainer.h"
#include "src/xml/hkxxmlreader.h"
#include "src/xml/hkxxmlwriter.h"
#include "src/filetypes/hkxfile.h"

/**
 * hkRootLevelContainer
 */

uint hkRootLevelContainer::refCount = 0;

QString hkRootLevelContainer::classname = "hkRootLevelContainer";

hkRootLevelContainer::hkRootLevelContainer(BehaviorFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HK_ROOT_LEVEL_CONTAINER, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);refCount++;
}

QString hkRootLevelContainer::getClassname(){
    return classname;
}

bool hkRootLevelContainer::readData(const HkxXmlReader &reader, int index){
    bool ok;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){

        if (reader.getNthAttributeValueAt(index, 0) == "namedVariants"){
            int numVariants = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkRootLevelContainer: readData()!\nAttempt to read the number of variants failed!");
                //return false;
            }
            for (int j = 0; j < numVariants; j++){
                namedVariants.append(hkRootLevelContainerNamedVariant());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "name"){
                        namedVariants.last().name = reader.getElementValueAt(index);
                    }else if (reader.getNthAttributeValueAt(index, 0) == "className"){
                        namedVariants.last().className = reader.getElementValueAt(index);
                    }else if (reader.getNthAttributeValueAt(index, 0) == "variant"){
                        if (!namedVariants.last().variant.readReference(index, reader)){
                            return false;
                        }
                    }
                    index++;
                }
            }
        }
        index++;
    }
    return true;
}

bool hkRootLevelContainer::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        QStringList list3 = {writer->name, writer->numelements};
        QStringList list4 = {"namedVariants", QString::number(namedVariants.size())};
        writer->writeLine(writer->object, list3, list4, "");
        for (int i = 0; i < namedVariants.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->object, QStringList(writer->name), QStringList("name"), namedVariants.at(i).name);
            writer->writeLine(writer->object, QStringList(writer->name), QStringList("className"), namedVariants.at(i).className);
            writer->writeLine(writer->object, QStringList(writer->name), QStringList("variant"), namedVariants.at(i).variant.data()->getReferenceString());
            writer->writeLine(writer->object, false);
        }
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->object, false);
        setIsWritten();
        for (int i = 0; i < namedVariants.size(); i++){
            namedVariants.at(i).variant.data()->write(writer);
        }
    }
    return true;
}

bool hkRootLevelContainer::link(){
    if (!getParentFile()){
        return false;
    }
    for (int i = 0; i < namedVariants.size(); i++){
        HkxObjectExpSharedPtr *ptr = getParentFile()->findBehaviorGraph(namedVariants.at(i).variant.getReference());//getParentFile()->findGenerator(namedVariants.at(i).variant.getReference());
        if (!ptr){
            writeToLog("hkRootLevelContainer: link()!\nUnable to link variant reference "+QString::number(namedVariants.at(i).variant.getReference())+"!");
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_BEHAVIOR_GRAPH){
            writeToLog("hkRootLevelContainer: link()!\nThe linked object is not a HKB_BEHAVIOR_GRAPH!");
            setDataValidity(false);
            namedVariants[i].variant = *ptr;
        }else{
            namedVariants[i].variant = *ptr;
        }
    }
    return true;
}

void hkRootLevelContainer::unlink(){
    for (int i = 0; i < namedVariants.size(); i++){
        namedVariants[i].variant = HkxObjectExpSharedPtr();
    }
}

bool hkRootLevelContainer::evaulateDataValidity(){
    for (int i = 0; i < namedVariants.size(); i++){
        if (!namedVariants.at(i).variant.data() || (namedVariants.at(i).className == "hkbBehaviorGraph" && namedVariants.at(i).variant.data()->getSignature() != HKB_BEHAVIOR_GRAPH)){
            setDataValidity(false);
            return false;
        }
    }
    setDataValidity(true);
    return true;
}

hkRootLevelContainer::~hkRootLevelContainer(){
    refCount--;
}
