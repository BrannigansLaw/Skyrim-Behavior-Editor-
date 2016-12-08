#include "hkxxmlreader.h"
#include "hkxfile.h"

HkxXmlReader::HkxXmlReaderError HkxXmlReader::readNextLine(QByteArray &elementName, QByteArrayList &attributeNames, QByteArrayList &attributeText, QByteArrayList &elementData){
    QByteArray line = hkxXmlFile->readLine(MAX_HKXXML_LINE_LENGHT);
    if (line.isEmpty()){
        return EmptyLine;
    }else if (line.size() == 1){
        if (line.at(0) != '\n'){
            return OrphanedCharacter;
        }else{
            return NoError;
        }
    }
    uint i = 0;
    while (i < line.size()){
        if (line.at(i) == '<'){
            break;
        }else if (line.at(i) != '\t'){
            return OrphanedCharacter;
        }
    }
    while (i < line.size()){
        if (line.at(i) == '<'){//get the element name
            i++;
            QByteArray elem(10, '\0');
            uint index = 0;
            while (i < line.size()){
                if (line.at(i) == '/'){//end element
                    i++;
                    while (i < line.size() && line.at(i) != ' ' && line.at(i) != '>'){
                        if ((line.at(i) < 'A') || (line.at(i) > 'Z' && line.at(i) < 'a') || (line.at(i) > 'z')) return InvalidElementName;
                        if (index >= elem.size()){
                            elem.resize(elem.size() * 2);
                        }
                        elem[index] = line.at(i);
                        index++;
                        i++;
                    }//set element name, check if it exists in elementlist
                    elementName = elem;
                    int j = 0;
                    for (; j < elementList.size(); j++){
                        if (elementName == elementList.at(j).name){
                            if (elementList.at(j).nestCount < 1) return OrphanedElementTag;
                            elementList.at(j).nestCount--;
                            break;
                        }
                    }
                    if (j >= elementList.size()) return OrphanedElementTag;
                    break;
                }else{
                    while (i < line.size() && line.at(i) != ' ' && line.at(i) != '>'){
                        if ((line.at(i) < 'A') || (line.at(i) > 'Z' && line.at(i) < 'a') || (line.at(i) > 'z')) return InvalidElementName;
                        if (index >= elem.size()){
                            elem.resize(elem.size() * 2);
                        }
                        elem[index] = line.at(i);
                        index++;
                        i++;
                    }
                    elementName = elem;
                    int j = 0;
                    for (; j < elementList.size(); j++){
                        if (elementName == elementList.at(j).name){
                            elementList.at(j).nestCount++;
                            break;
                        }
                    }
                    if (j < elementList.size()) break;
                    elementList.append(Element(elementName));
                    break;
                }
                i++;//not sure this is needed...
            }
        }else if (line.at(i) != ' ' && line.at(i) != '\t'){//get attribute name
            QByteArray attrib(15, '\0');
            uint index = 0;
            while (i < line.size()){
                i++;
                while (i < line.size() && line.at(i) != '='){
                    if ((line.at(i) < 'A') || (line.at(i) > 'Z' && line.at(i) < 'a') || (line.at(i) > 'z')) return InvalidAttributeName;
                    if (index >= attrib.size()){
                        attrib.resize(elem.size() * 2);
                    }
                    attrib[index] = line.at(i);
                    index++;
                    i++;
                }//set attribute name, check if it exists in attributeList
                attributeNames.append(attrib);
                i++;
            }
        }
        i++;
    }
    return NoError;
}
