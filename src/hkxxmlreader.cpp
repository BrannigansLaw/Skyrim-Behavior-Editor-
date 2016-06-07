#include "hkxxmlreader.h"
#include "hkxfile.h"

HkxXmlReader::HkxXmlReader(BehaviorFile *file): hkxXmlFile(file), isEOF(false){
}

bool HkxXmlReader::parse(){
    if (!hkxXmlFile->open(QIODevice::ReadOnly)){
        isEOF = true;
        return false;
    }
    isEOF = false;
    QByteArray line = hkxXmlFile->readLine(MAX_HKXXML_LINE_LENGHT);
    if (line != "<?xml version=\"1.0\" encoding=\"ascii\"?>\n" && line != "<?xml version=\"1.0\" encoding=\"ascii\"?>\r\n"){
        return false;
    }
    HkxXmlParseLine result = NoError;
    while (!atEnd()){
        result = readNextLine();
        if (result != NoError && result != EmptyLine && result != EmptyLineEndFile){
            return false;
        }
    }
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
        }else if (line.at(i) != '\t' && line.at(i) != '\r'){
            if (line.at(i) == '\n'){
                break;
            }
            if (elementList.isEmpty()){
                return UnknownError;
            }
            if (elementList.last().isContainedOnOneLine == true){
                return OrphanedCharacter;
            }
            break;
        }
        i++;
    }
    bool isIsolatedEndElemTag = true;
    while (i < line.size()){
        if (line.at(i) == '<'){//get the element name
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
                    }//check if elem end tag is correctly paired and remove null characters
                    elem.remove(index, elem.size() - index);
                    if (!elementList.isEmpty()){
                        if (indexOfElemTags.isEmpty() || indexOfElemTags.last() >= elementList.size()){
                            return UnknownError;
                        }
                        if (isIsolatedEndElemTag && !elementList.at(indexOfElemTags.last()).isContainedOnOneLine){
                            if (qstrcmp(elementList.at(indexOfElemTags.last()).name.constData(), elem.constData()) != 0){
                                //elementList[k].isClosed = true;
                                return OrphanedElementTag;
                            }else{
                                if (indexOfElemTags.isEmpty()){
                                    return UnknownError;
                                }else{
                                    indexOfElemTags.removeLast();
                                }
                            }
                        }
                    }
                    if (line.at(i) == '>'){
                        i++;
                        if (i >= line.size()){
                            return UnknownError;
                        }
                        if (line.at(i) == '\n'){
                            return NoError;
                        }else{
                            break;
                        }
                    }else{
                        return MalformedEndElementTag;
                    }
                }else if (line.at(i) == '!'){//check comment
                    i++;
                    for (int y = 0; y < 2; y++){
                        int c = 0;
                        while (i < line.size() && line.at(i) == '-'){
                            i++;
                            c++;
                        }
                        if (c != 2){
                            return MalformedComment;
                        }
                        if (y == 1 && line.at(i) != '>'){
                            return MalformedComment;
                        }
                        while (i < line.size() && line.at(i) != '-'){
                            i++;
                        }
                    }
                    i++;
                    break;
                }else{//is start of an element tag
                    isIsolatedEndElemTag = false;
                    if (i >= line.size()){
                        return UnknownError;
                    }
                    while (i < line.size() && line.at(i) != ' ' && line.at(i) != '>'){
                        //if ((line.at(i) < '0') || (line.at(i) > '9' && line.at(i) < 'A') || (line.at(i) > 'Z' && line.at(i) < 'a') || (line.at(i) > 'z')) return InvalidElementName;
                        if (index >= elem.size()){
                            elem.append(QByteArray(elem.size() * 2, '\0'));
                        }
                        elem[index] = line.at(i);
                        index++;
                        i++;
                    }
                    elem.truncate(index);
                    elementList.append(Element(elem));
                    if (line.at(i) == '>'){
                        break;
                    }else if (line.at(i) == ' '){//get attributes
                        while (i < line.size() && line.at(i) != '>'){
                            QByteArray attrib(15, '\0');
                            int index = 0;
                            if (line.at(i) == '\n'){
                                return UnexpectedNewlineCharacter;
                            }
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
                                attrib.truncate(index);
                                if (i >= line.size()){
                                    return UnknownError;
                                }
                                if (elementList.isEmpty()){
                                    return OrphanedAttribute;
                                }
                                elementList.last().attributeList.append(attrib);
                                i++;
                                if (i >= line.size()){
                                    return UnknownError;
                                }
                                if (line.at(i) != '\"'){
                                    return OrphanedCharacter;
                                }
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
                                value.truncate(index);
                                if (line.at(i) != '"'){
                                    return OrphanedCharacter;
                                }
                                elementList.last().attributeList.last().value = value;
                            }
                            i++;
                        }
                    }
                    break;
                }
            }
            continue;
        }else if (line.at(i) == '>'){//get element value
            i++;
            if (i >= line.size()){
                return UnknownError;
            }
            if (elementList.isEmpty()){
                return UnknownError;
            }
            //Fucking carriage return character...
            if (line.at(i) == '\n'){
                indexOfElemTags.append(elementList.size() - 1);
                elementList.last().isContainedOnOneLine = false;
                return NoError;
            }else if (line.at(i) == '\r'){
                i++;
                if (i >= line.size()){
                    return UnknownError;
                }
                if (line.at(i) == '\n'){
                    indexOfElemTags.append(elementList.size() - 1);
                    elementList.last().isContainedOnOneLine = false;
                    return NoError;
                }
            }else if (line.at(i) == '<'){//empty element
                elementList.last().isContainedOnOneLine = true;
                continue;
            }
            elementList.last().isContainedOnOneLine = true;
            //elementList.last().isClosed = true;
            QByteArray value(9, '\0');
            int index = 0;
            //Need to deal with embedded comments...FFS
            while (i < line.size()){
                if (line.at(i) == '\n'){
                    if (elementList.isEmpty()){
                        return OrphanedAttribute;
                    }
                    elementList.last().value = value;
                    return NoError;
                }else if (line.at(i) == '<'){
                    int h = i + 1;
                    if (h >= line.size()){
                        return UnknownError;
                    }
                    if (line.at(h) == '!'){
                        for (int g =0; g < 2; g++){
                            if (index >= value.size()){
                                value.append(QByteArray(value.size() * 2, '\0'));
                            }
                            if (i >= line.size()){
                                return UnknownError;
                            }
                            value[index] = line.at(i);
                            index++;
                            i++;
                        }
                        for (int y = 0; y < 2; y++){
                            int c = 0;
                            while (i < line.size() && line.at(i) == '-'){
                                if (index >= value.size()){
                                    value.append(QByteArray(value.size() * 2, '\0'));
                                }
                                value[index] = line.at(i);
                                index++;
                                i++;
                                c++;
                            }
                            if (c != 2){
                                return MalformedComment;
                            }
                            if (y == 1){
                                if (line.at(i) != '>'){
                                    return MalformedComment;
                                }
                                if (index >= value.size()){
                                    value.append(QByteArray(value.size() * 2, '\0'));
                                }
                                value[index] = line.at(i);
                                index++;
                                i++;
                                continue;
                            }
                            while (i < line.size() && line.at(i) != '-'){
                                if (index >= value.size()){
                                    value.append(QByteArray(value.size() * 2, '\0'));
                                }
                                value[index] = line.at(i);
                                index++;
                                i++;
                            }
                        }
                    }else{
                        break;
                    }
                }
                //if (line.at(i) < 'A' || line.at(i) > '~') return InvalidElementValue;
                if (index >= value.size()){
                    value.append(QByteArray(value.size() * 2, '\0'));
                }
                value[index] = line.at(i);
                index++;
                i++;
            }
            /*while (i < line.size() && line.at(i) != '<'){
                if (line.at(i) == '\n'){
                    if (elementList.isEmpty()){
                        return OrphanedAttribute;
                    }
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
            }*/
            value.truncate(index);
            if (elementList.isEmpty()){
                return OrphanedAttribute;
            }
            elementList.last().value = value;
            continue;
        }else if (line.at(i) != '\n' && line.at(i) != '\r'){//get element data on separate line
            if (elementList.isEmpty()){
                return UnknownError;
            }
            if (elementList.last().isContainedOnOneLine == true){
                return OrphanedCharacter;
            }
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
            value.truncate(index);
            if (line.at(i) != '\n'){
                return UnknownError;
            }
            if (elementList.isEmpty()){
                return OrphanedAttribute;
            }
            if (elementList.last().value.endsWith('\n')){
                //elementList.last().value.remove(elementList.last().value.size() - 1, 1);
            }
            elementList.last().value = QByteArray(elementList.last().value.constData());
            elementList.last().value.append(' '+QByteArray(value.constData()));
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



























