#include "hkrootlevelcontainer.h"
#include "src/xml/hkxxmlreader.h"
#include "src/xml/hkxxmlwriter.h"
#include "src/filetypes/behaviorfile.h"
#include "src/filetypes/projectfile.h"
#include "src/filetypes/characterfile.h"
#include "src/filetypes/skeletonfile.h"

/**
 * hkRootLevelContainer
 */

uint hkRootLevelContainer::refCount = 0;

QString hkRootLevelContainer::classname = "hkRootLevelContainer";

hkRootLevelContainer::hkRootLevelContainer(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HK_ROOT_LEVEL_CONTAINER, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkRootLevelContainer::getClassname(){
    return classname;
}

bool hkRootLevelContainer::readData(const HkxXmlReader &reader, long index){
    bool ok;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        if (reader.getNthAttributeValueAt(index, 0) == "namedVariants"){
            int numVariants = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nAttempt to read the number of variants failed!");
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
                        index++;
                        break;
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
        QString refString;
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"namedVariants", QString::number(namedVariants.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < namedVariants.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("name"), namedVariants.at(i).name);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("className"), namedVariants.at(i).className);
            if (namedVariants.at(i).variant.data()){
                refString = namedVariants.at(i).variant.data()->getReferenceString();
            }else{
                refString = "null";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("variant"), refString);
            writer->writeLine(writer->object, false);
        }
        if (namedVariants.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        for (int i = 0; i < namedVariants.size(); i++){
            if (namedVariants.at(i).variant.data() && !namedVariants.at(i).variant.data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write variant at "+QString::number(i), true);
            }
        }
    }
    return true;
}

void hkRootLevelContainer::disconnect(){
    for (int i = 0; i < namedVariants.size(); i++){
        namedVariants.at(i).variant.~HkxObjectExpSharedPtr();
    }
}

bool hkRootLevelContainer::link(){
    if (!getParentFile()){
        return false;
    }
    for (int i = 0; i < namedVariants.size(); i++){//This is awful, I know. I'll sort it out later...
        HkxObjectExpSharedPtr *ptr = NULL;
        HkxFile *file = dynamic_cast<BehaviorFile *>(getParentFile());
        if (file){
            ptr = static_cast<BehaviorFile *>(getParentFile())->findBehaviorGraph(namedVariants.at(i).variant.getReference());
        }else{
            file = dynamic_cast<ProjectFile *>(getParentFile());
            if (file){
                ptr = static_cast<ProjectFile *>(getParentFile())->findProjectData(namedVariants.at(i).variant.getReference());
            }else{
                file = dynamic_cast<CharacterFile *>(getParentFile());
                if (file){
                    ptr = static_cast<CharacterFile *>(getParentFile())->findCharacterData(namedVariants.at(i).variant.getReference());
                }else{
                    file = dynamic_cast<SkeletonFile *>(getParentFile());
                    if (file){
                        ptr = static_cast<SkeletonFile *>(getParentFile())->findSkeleton(namedVariants.at(i).variant.getReference());
                    }else{
                        writeToLog(getClassname()+": link()!\nParent file type is invalid!!!", true);
                    }
                }
            }
        }
        if (!ptr){
            writeToLog(getClassname()+": link()!\nUnable to link variant reference "+QString::number(namedVariants.at(i).variant.getReference())+"!");
            setDataValidity(false);
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
        if (!namedVariants.at(i).variant.data()){
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
