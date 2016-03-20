#include "hkxfile.h"
#include "hkobject.h"
#include "utility.h"
#include "hkxxmlreader.h"

bool BehaviorFile::parse(){
    if (!reader.beginParse()) return false;
    bool ok = true;
    while (!reader.atEnd()){
        reader.readNextLine();//if (reader.readNextLine() != NoError)/*return false*/;
        if (reader.getNthAttributeValueAt(reader.getLastElementIndex(), 2).toLongLong(&ok, 16) == HK_ROOT_LEVEL_CONTAINER){
            if (!ok) return false;
            //need to remove the '#' from the reference string
            otherTypes.append(HkObjectExpSharedPtr(new hkRootLevelContainer(this, reader.getNthAttributeValueAt(reader.getLastElementIndex(), 0).remove(0,1).toLong(&ok))));
            if (!ok) return false;
            setRootObject(otherTypes.last());
            int index = reader.getLastElementIndex();
            while (reader.readNextLine() != HkxXmlReader::HkxXmlParseLine::EmptyLine){
                if (reader.atEnd()) return true;
                reader.readNextLine();
            }
            ((hkRootLevelContainer*)(otherTypes.last().constData()))->readData(reader, index);
        }
    }
    return true;
}
