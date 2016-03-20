#include "hkxxmlreader.h"
#include "hkxfile.h"

HkxXmlReader::HkxXmlReader(BehaviorFile *file): hkxXmlFile(file), isElementValueSplitOnMutipleLines(false){
}

bool HkxXmlReader::beginParse(){
    if (!hkxXmlFile->open(QIODevice::ReadOnly)){
        isEOF = true;
        return false;
    }
    isEOF = false;
    QByteArray line = hkxXmlFile->readLine(MAX_HKXXML_LINE_LENGHT);
    if (line != "<?xml version=\"1.0\" encoding=\"ascii\"?>\n") return false;
    return true;
}

HkxXmlReader::HkxXmlParseLine HkxXmlReader::readNextLine(){
    QByteArray line = hkxXmlFile->readLine(MAX_HKXXML_LINE_LENGHT);
    if (line.isEmpty()){
        isEOF = true;
        return EmptyLineEndFile;
    }else if (line.size() == 1){
        if (line.at(0) != '\n'){
            return OrphanedCharacter;
        }else{
            return EmptyLine;
        }
    }
    int i = 0;
    while (i < line.size()){
        if (line.at(i) == '<'){
            break;
        }else if (line.at(i) != '\t'){
            if (isElementValueSplitOnMutipleLines){
                break;
            }else{
                return OrphanedCharacter;
            }
        }
        i++;
    }
    while (i < line.size()){
        if (line.at(i) == '<'){//get the element name
            isElementValueSplitOnMutipleLines = false;
            i++;
            QByteArray elem(10, '\0');
            int index = 0;
            while (i < line.size()){
                if (line.at(i) == '/'){//end element
                    i++;
                    while (i < line.size() && line.at(i) != ' ' && line.at(i) != '>'){
                        if ((line.at(i) < '0') || (line.at(i) > '9' && line.at(i) < 'A') || (line.at(i) > 'Z' && line.at(i) < 'a') || (line.at(i) > 'z')) return InvalidElementName;
                        if (index >= elem.size()){
                            elem.append(QByteArray(elem.size() * 2, '\0'));
                        }
                        elem[index] = line.at(i);
                        index++;
                        i++;
                    }//check if elem exists at the back of elementlist
                    if (!elementList.isEmpty()){
                        if (elementList.last().name != elem.remove(0, 1)) return OrphanedElementTag;
                    }
                    if (line.at(i) == '>'){
                        i++;
                        if (i >= line.size()) return UnknownError;
                        if (line.at(i) == '\n'){
                            //isElementValueSplitOnMutipleLines = false;
                            return NoError;
                        }else{
                            break;
                        }
                    }else{
                        return MalformedEndElementTag;
                    }
                }else if (line.at(i) == '!'){//check comment
                    isElementValueSplitOnMutipleLines = false;
                    i++;
                    if (i >= line.size()) return UnknownError;
                    if (line.at(i) != '-') return MalformedComment;
                    i++;
                    if (i >= line.size()) return UnknownError;
                    if (line.at(i) != '-') return MalformedComment;
                    i++;
                    while (i < line.size() && line.at(i) != '-'){
                        i++;
                    }
                    i++;
                    if (i >= line.size()) return UnknownError;
                    if (line.at(i) != '-') return MalformedComment;
                    i++;
                    if (i >= line.size()) return UnknownError;
                    if (line.at(i) != '>') return MalformedComment;
                    break;
                }else{//is start of an element tag
                    if (i >= line.size()) return UnknownError;
                    while (i < line.size() && line.at(i) != ' ' && line.at(i) != '>'){
                        //if ((line.at(i) < '0') || (line.at(i) > '9' && line.at(i) < 'A') || (line.at(i) > 'Z' && line.at(i) < 'a') || (line.at(i) > 'z')) return InvalidElementName;
                        if (index >= elem.size()){
                            elem.append(QByteArray(elem.size() * 2, '\0'));
                        }
                        elem[index] = line.at(i);
                        index++;
                        i++;
                    }
                    elementList.append(Element(elem));
                    if (line.at(i) == '>'){
                        i++;
                        if (i >= line.size()) return UnknownError;
                        if (line.at(i) == '\n'){
                            isElementValueSplitOnMutipleLines = true;
                            return NoError;
                        }
                        QByteArray value(9, '\0');
                        int index = 0;
                        while (i < line.size() && line.at(i) != '<'){
                            if (line.at(i) == '\n'){
                                if (elementList.isEmpty()) return OrphanedAttribute;
                                elementList.last().value = value;
                                return NoError;
                            }
                            //if (line.at(i) < 'A' || line.at(i) > '~') return InvalidElementValue;
                            if (index >= value.size()){
                                value.append(QByteArray(value.size() * 2, '\0'));
                            }
                            value[index] = line.at(i);
                            index++;
                            i++;
                        }
                        if (elementList.isEmpty()) return OrphanedAttribute;
                        elementList.last().value = value;
                    }else if (line.at(i) == ' '){//get attributes
                        while (i < line.size() && line.at(i) != '>'){
                            QByteArray attrib(15, '\0');
                            int index = 0;
                            if (line.at(i) == '\n') return UnexpectedNewlineCharacter;
                            if (line.at(i) == ' '){
                                i++;
                                while (i < line.size() && line.at(i) != '='){
                                    //if ((line.at(i) < 'A') || (line.at(i) > 'Z' && line.at(i) < 'a') || (line.at(i) > 'z')) return InvalidAttributeName;
                                    if (index >= attrib.size()){
                                        attrib.append(QByteArray(attrib.size() * 2, '\0'));
                                    }
                                    attrib[index] = line.at(i);
                                    index++;
                                    i++;
                                }//set attribute name, get attribute value
                                if (i >= line.size()) return UnknownError;
                                if (elementList.isEmpty()) return OrphanedAttribute;
                                elementList.last().attributeList.append(attrib);
                                i++;
                                if (i >= line.size()) return UnknownError;
                                if (line.at(i) != '"') return OrphanedCharacter;
                                QByteArray value(9, '\0');
                                int index = 0;
                                i++;
                                while (i < line.size() && line.at(i) != '"'){
                                    //if ((line.at(i) < '0') || (line.at(i) > '9' && line.at(i) < 'A') || (line.at(i) > 'Z' && line.at(i) < 'a') || (line.at(i) > 'z' && line.at(i) != '_'  && line.at(i) != '-'  && line.at(i) != '.')) return InvalidAttributeValue;
                                    //if ((line.at(i) < '0') || (line.at(i) > 'z')) return InvalidAttributeValue;
                                    if (index >= value.size()){
                                        value.append(QByteArray(value.size() * 2, '\0'));
                                    }
                                    value[index] = line.at(i);
                                    index++;
                                    i++;
                                }
                                if (line.at(i) != '"') return OrphanedCharacter;
                                elementList.last().attributeList.last().value = value;
                            }
                            i++;
                        }
                        //get element value?
                        i++;
                        if (i >= line.size()) return UnknownError;
                        if (line.at(i) == '\n'){
                            isElementValueSplitOnMutipleLines = true;
                            return NoError;
                        }
                        QByteArray value(9, '\0');
                        int index = 0;
                        while (i < line.size() && line.at(i) != '<'){
                            if (line.at(i) == '\n'){
                                if (elementList.isEmpty()) return OrphanedAttribute;
                                elementList.last().value = value;
                                return NoError;
                            }
                            //if (line.at(i) < 'A' || line.at(i) > '~') return InvalidElementValue;
                            if (index >= value.size()){
                                value.append(QByteArray(value.size() * 2, '\0'));
                            }
                            value[index] = line.at(i);
                            index++;
                            i++;
                        }
                        if (elementList.isEmpty()) return OrphanedAttribute;
                        elementList.last().value = value;
                    }
                    break;
                }
            }
        }else if (isElementValueSplitOnMutipleLines && line.at(i) != '\n'){//get element data on separate line
            QByteArray value(9, '\0');
            int index = 0;
            while (i < line.size() && line.at(i) != '\n'){
                //if (line.at(i) < 'A' || line.at(i) > '~') return InvalidElementValue;
                if (index >= value.size()){
                    value.append(QByteArray(value.size() * 2, '\0'));
                }
                value[index] = line.at(i);
                index++;
                i++;
            }
            if (elementList.isEmpty()) return OrphanedAttribute;
            elementList.last().value = value;
        }else{
            return OrphanedCharacter;
        }
        i++;
    }
    return NoError;
}

int HkxXmlReader::getNumAttributesAt(int index) const{
    if (index < elementList.size()){
        return elementList.at(index).attributeList.size();
    }else{
        return 0;
    }
}

QByteArray HkxXmlReader::getElementNameAt(int index) const{
    if (index < elementList.size()){
        return elementList.at(index).name;
    }else{
        return "";
    }
}

QByteArray HkxXmlReader::getElementValueAt(int index) const{
    if (index < elementList.size()){
        return elementList.at(index).value;
    }else{
        return "";
    }
}

QByteArray HkxXmlReader::getNthAttributeNameAt(int index, int nth) const{
    if (index < elementList.size() && nth < elementList.at(index).attributeList.size()){
        return elementList.at(index).attributeList.at(nth).name;
    }else{
        return "";
    }
}

QByteArray HkxXmlReader::getNthAttributeValueAt(int index, int nth) const{
    if (index < 0 || nth < 0){
        return "";
    }
    if (index < elementList.size() && nth < elementList.at(index).attributeList.size()){
        return elementList.at(index).attributeList.at(nth).value;
    }else{
        return "";
    }
}


























