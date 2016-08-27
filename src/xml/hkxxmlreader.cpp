#include "hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

HkxXmlReader::HkxXmlReader(BehaviorFile *file)
    :
      hkxXmlFile(file),
      isEOF(false),
      lineNumber(0)
{
    //
}

bool HkxXmlReader::parse(){
    if (!hkxXmlFile->open(QIODevice::ReadOnly)){
        isEOF = true;
        hkxXmlFile->writeToLog("HkxXmlReader: parse() failed!\nThe file "+hkxXmlFile->fileName()+" failed to open!", true);
        return false;
    }
    isEOF = false;
    QByteArray line = hkxXmlFile->readLine(MAX_HKXXML_LINE_LENGHT);
    lineNumber++;
    if (line != "<?xml version=\"1.0\" encoding=\"ascii\"?>\n" && line != "<?xml version=\"1.0\" encoding=\"ascii\"?>\r\n"){
        hkxXmlFile->writeToLog("HkxXmlReader: parse() failed!\nThe file "+hkxXmlFile->fileName()+" is not in the correct XML format!", true);
        return false;
    }
    HkxXmlParseLine result = NoError;
    while (!atEnd()){
        //hkxXmlFile->setProgressData("Parsing XML line number "+QString::number(lineNumber), 20);
        result = readNextLine();
        if (result != NoError && result != EmptyLine && result != EmptyLineEndFile){
            hkxXmlFile->writeToLog("HkxXmlReader: parse() failed because readNextLine() failed!", true);
            return false;
        }
    }
    hkxXmlFile->setProgressData("XML parsed successfully!", 40);
    return true;
}

HkxXmlReader::HkxXmlParseLine HkxXmlReader::readNextLine(){
    QByteArray line = hkxXmlFile->readLine(MAX_HKXXML_LINE_LENGHT);
    lineNumber++;
    if (line.isEmpty()){
        isEOF = true;
        return EmptyLineEndFile;
    }else if (line.size() == 1){
        if (line.at(0) != '\n'){
            hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because an orphaned character was found on line "+QString::number(lineNumber)+"!", true);
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
                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because 'elementList' is empty!\nLine number: "+QString::number(lineNumber), true);
                return UnknownError;
            }
            if (elementList.last().isContainedOnOneLine == true){
                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because an orphaned character was found on line "+QString::number(lineNumber)+"!", true);
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
                            hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because 'elementList' is empty!\nLine number: "+QString::number(lineNumber), true);
                            return UnknownError;
                        }
                        if (indexOfElemTags.last() >= elementList.size()){
                            hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because 'indexOfElemTags' last value is corrupted!\nLine number: "+QString::number(lineNumber), true);
                            return UnknownError;
                        }
                        if (isIsolatedEndElemTag && !elementList.at(indexOfElemTags.last()).isContainedOnOneLine){
                            if (qstrcmp(elementList.at(indexOfElemTags.last()).name.constData(), elem.constData()) != 0){
                                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because an orphaned element tag was found on line "+QString::number(lineNumber)+"!", true);
                                return OrphanedElementTag;
                            }else{
                                if (indexOfElemTags.isEmpty()){
                                    hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because 'indexOfElemTags' is empty!\nLine number: "+QString::number(lineNumber), true);
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
                            hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because index of the current line is out of bounds!\nLine number: "+QString::number(lineNumber), true);
                            return UnknownError;
                        }
                        if (line.at(i) == '\n'){
                            return NoError;
                        }else{
                            break;
                        }
                    }else{
                        hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because index of the current line is out of bounds!\nLine number: "+QString::number(lineNumber), true);
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
                            hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because the current comment is malformed!\nLine number: "+QString::number(lineNumber), true);
                            return MalformedComment;
                        }
                        if (y == 1 && line.at(i) != '>'){
                            hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because the current comment is malformed!\nLine number: "+QString::number(lineNumber), true);
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
                        hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because index of the current line is out of bounds!\nLine number: "+QString::number(lineNumber), true);
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
                                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because there is a newline character within an attribute!\nLine number: "+QString::number(lineNumber), true);
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
                                    hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because index of the current line is out of bounds!\nLine number: "+QString::number(lineNumber), true);
                                    return UnknownError;
                                }
                                if (elementList.isEmpty()){
                                    hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because an orphaned attribute was found!\nLine number: "+QString::number(lineNumber), true);
                                    return OrphanedAttribute;
                                }
                                elementList.last().attributeList.append(attrib);
                                i++;
                                if (i >= line.size()){
                                    hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because index of the current line is out of bounds!\nLine number: "+QString::number(lineNumber), true);
                                    return UnknownError;
                                }
                                if (line.at(i) != '\"'){
                                    hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because an orphaned character was found!\nLine number: "+QString::number(lineNumber), true);
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
                                    hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because an orphaned character was found!\nLine number: "+QString::number(lineNumber), true);
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
                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because index of the current line is out of bounds!\nLine number: "+QString::number(lineNumber), true);
                return UnknownError;
            }
            if (elementList.isEmpty()){
                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because 'elementList' is empty!\nLine number: "+QString::number(lineNumber), true);
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
                    hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because index of the current line is out of bounds!\nLine number: "+QString::number(lineNumber), true);
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
                        hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because an orphaned attribute was found!\nLine number: "+QString::number(lineNumber), true);
                        return OrphanedAttribute;
                    }
                    elementList.last().value = value;
                    return NoError;
                }else if (line.at(i) == '<'){
                    int h = i + 1;
                    if (h >= line.size()){
                        hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because index of the current line is out of bounds!\nLine number: "+QString::number(lineNumber), true);
                        return UnknownError;
                    }
                    if (line.at(h) == '!'){
                        for (int g =0; g < 2; g++){
                            if (index >= value.size()){
                                value.append(QByteArray(value.size() * 2, '\0'));
                            }
                            if (i >= line.size()){
                                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because index of the current line is out of bounds!\nLine number: "+QString::number(lineNumber), true);
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
                                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because the current comment is malformed!\nLine number: "+QString::number(lineNumber), true);
                                return MalformedComment;
                            }
                            if (y == 1){
                                if (line.at(i) != '>'){
                                    hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because the current comment is malformed!\nLine number: "+QString::number(lineNumber), true);
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
            value.truncate(index);
            if (elementList.isEmpty()){
                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because an orphaned attribute was found!\nLine number: "+QString::number(lineNumber), true);
                return OrphanedAttribute;
            }
            elementList.last().value = value;
            continue;
        }else if (line.at(i) != '\n' && line.at(i) != '\r'){//get element data on separate line
            if (elementList.isEmpty()){
                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed for some unknown reason!\nLine number: "+QString::number(lineNumber), true);
                return UnknownError;
            }
            if (elementList.last().isContainedOnOneLine == true){
                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because an orphaned character was found!\nLine number: "+QString::number(lineNumber), true);
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
                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed for some unknown reason!\nLine number: "+QString::number(lineNumber), true);
                return UnknownError;
            }
            if (elementList.isEmpty()){
                hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because an orphaned attribute was found!\nLine number: "+QString::number(lineNumber), true);
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

void HkxXmlReader::setBehaviorFile(BehaviorFile *file){
    hkxXmlFile = file;
}
bool HkxXmlReader::atEnd() const{
    return isEOF;
}
int HkxXmlReader::getLastElementIndex() const{
    return elementList.size() - 1;
}
int HkxXmlReader::getNumElements() const{
    return elementList.size();
}

void HkxXmlReader::clear(){
    elementList.clear();
}

HkxXmlReader::~HkxXmlReader(){
    /*if (hkxXmlFile){hkxXmlFile->closeFile();}*/
}
