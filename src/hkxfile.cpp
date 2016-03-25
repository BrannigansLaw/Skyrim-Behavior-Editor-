#include "hkxfile.h"
#include "hkobject.h"
//#include "utility.h"
#include "hkxxmlreader.h"
#include "modifiers.h"

template <typename T>
bool BehaviorFile::appendAndReadData(int index, T * obj){
    bool ok;
    QByteArray temp = reader.getNthAttributeValueAt(index, 0);
    if (temp.at(0) == '#'){
        temp.remove(0, 1);
    }
    if (obj->getType() == HkObject::TYPE_GENERATOR){
        generators.append(hkbGeneratorExpSharedPtr(reinterpret_cast<hkbGenerator*>(obj), temp.toLong(&ok)));
    }else if (obj->getType() == HkObject::TYPE_MODIFIER){
        modifiers.append(HkbModifierExpSharedPtr(reinterpret_cast<hkbModifier*>(obj), temp.toLong(&ok)));
    }else if (obj->getType() == HkObject::TYPE_OTHER){
        otherTypes.append(HkObjectExpSharedPtr(obj, temp.toLong(&ok)));
    }else{
        return false;
    }
    if (!ok){
        return false;
    }
    obj->readData(reader, index);
    return true;
}

bool BehaviorFile::parse(){
    if (!reader.parse()){
        return false;
    }
    int index = 0;
    bool ok = true;
    qlonglong signature;
    while (index < reader.getNumElements()){
        signature = reader.getNthAttributeValueAt(index, 2).toLongLong(&ok, 16);
        if (signature == HK_ROOT_LEVEL_CONTAINER){
            if (!appendAndReadData(index, new hkRootLevelContainer(this))){
                return false;
            }
            setRootObject(otherTypes.last());
        }else if (signature == HKB_STATE_MACHINE_STATE_INFO){
            if (!appendAndReadData(index, new hkbStateMachineStateInfo(this))){
                return false;
            }
        }else{
            //return false;
        }
        index++;
    }
    return true;
}
