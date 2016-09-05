#include "hkrootlevelcontainer.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/**
 * hkRootLevelContainer
 */

uint hkRootLevelContainer::refCount = 0;

QString hkRootLevelContainer::classname = "hkRootLevelContainer";

hkRootLevelContainer::hkRootLevelContainer(BehaviorFile *parent/*, long ref = 0*/)
    : HkxObject(parent/*, ref*/)
{
    refCount++;
    setType(HK_ROOT_LEVEL_CONTAINER, TYPE_OTHER);
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
