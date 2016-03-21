#include "hkxfile.h"
#include "hkobject.h"
#include "utility.h"
#include "hkxxmlreader.h"

bool BehaviorFile::parse(){
    if (!reader.parse()){
        return false;
    }
    int index = 0;
    bool ok = true;
    while (index < reader.getNumElements()){
        if (reader.getNthAttributeValueAt(index, 2).toLongLong(&ok, 16) == HK_ROOT_LEVEL_CONTAINER){
            if (!ok) return false;
            //need to remove the '#' from the reference string
            otherTypes.append(HkObjectExpSharedPtr(new hkRootLevelContainer(this, reader.getNthAttributeValueAt(index, 0).remove(0,1).toLong(&ok))));
            if (!ok) return false;
            setRootObject(otherTypes.last());
            ((hkRootLevelContainer*)(otherTypes.last().constData()))->readData(reader, index);
        }
    }
    return true;
}
