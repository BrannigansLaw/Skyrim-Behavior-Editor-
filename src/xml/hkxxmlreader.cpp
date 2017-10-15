#include "hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

#include <mutex>

#define AVERAGE_ELEMENT_TAG_LENGTH 9
#define AVERAGE_ATTRIBUTE_LENGTH 5
#define AVERAGE_ATTRIBUTE_VALUE_LENGTH 14
#define AVERAGE_VALUE_LENGTH 9

HkxXmlReader::HkxXmlReader(HkxFile *file)
    :
      hkxXmlFile(file),
      isEOF(false),
      lineNumber(0)
{
    //
}

bool HkxXmlReader::parse(){
    //std::mutex mu;
    //mu.lock();
    lineNumber = 0;
    if (!hkxXmlFile->open(QIODevice::ReadOnly)){
        isEOF = true;
        //hkxXmlFile->writeToLog("HkxXmlReader: parse() failed!\nThe file "+hkxXmlFile->fileName()+" failed to open!", true);
        return false;
    }
    isEOF = false;
    QByteArray line = hkxXmlFile->readLine(MAX_HKXXML_LINE_LENGTH);
    lineNumber++;
    if (line != "<?xml version=\"1.0\" encoding=\"ascii\"?>\n" && line != "<?xml version=\"1.0\" encoding=\"ascii\"?>\r\n"){
        //hkxXmlFile->writeToLog("HkxXmlReader: parse() failed!\nThe file "+hkxXmlFile->fileName()+" is not in the correct XML format!", true);
        //mu.unlock();
        return false;
    }
    HkxXmlParseLine result = NoError;
    while (!atEnd()){
        //hkxXmlFile->//setProgressData("Parsing XML line number "+QString::number(lineNumber), 20);
        result = readNextLine();
        if (result != NoError && result != EmptyLine && result != EmptyLineEndFile){
            //hkxXmlFile->writeToLog("HkxXmlReader: parse() failed because readNextLine() failed!", true);
            //mu.unlock();
            return false;
        }
    }
    if (!indexOfElemTags.isEmpty()){
        //hkxXmlFile->writeToLog("HkxXmlReader: parse() failed because there are orphaned element tags!!!", true);
        return false;
    }
    //hkxXmlFile->//setProgressData("XML parsed successfully!", 40);
    //mu.unlock();
    return true;
}

int HkxXmlReader::readElementTag(const QByteArray & line, int startIndex, bool isIsolatedEndElemTag, bool isEndTag){
    if (startIndex < 0){
        return UnknownError;
    }
    QByteArray tag(AVERAGE_ELEMENT_TAG_LENGTH, '\0');
    int index = 0;
    for (; startIndex < line.size() && line.at(startIndex) != ' ' && line.at(startIndex) != '>'; startIndex++, index++){
        /*if ((line.at(startIndex) < '0') || (line.at(startIndex) > '9' && line.at(startIndex) < 'A') || (line.at(startIndex) > 'Z' && line.at(startIndex) < 'a') || (line.at(startIndex) > 'z')){
            return -1;
        }*/
        if (index >= tag.size()){
            tag.append(QByteArray(tag.size() * 2, '\0'));
        }
        tag[index] = line.at(startIndex);
    }
    tag.truncate(index);
    if (isIsolatedEndElemTag){
        if (!elementList.isEmpty() && !(indexOfElemTags.isEmpty() || indexOfElemTags.last() >= elementList.size())){
            if (!elementList.at(indexOfElemTags.last()).isContainedOnOneLine){
                if (qstrcmp(elementList.at(indexOfElemTags.last()).name.constData(), tag.constData()) != 0){
                    return OrphanedElementTag;
                }else{
                    if (indexOfElemTags.isEmpty()){
                        return OrphanedElementTag;
                    }else{
                        indexOfElemTags.removeLast();
                    }
                }
            }
        }else{
            return UnknownError;
        }
    }else if (!isEndTag){
        elementList.append(Element(tag));
    }
    if (startIndex < line.size()){
        if (startIndex < line.size()){
            if (line.at(startIndex) == '>'){
                startIndex++;
                if (!isEndTag && startIndex < line.size() && (line.at(startIndex) == '\n' || line.at(startIndex) == '\r')){
                    indexOfElemTags.append(elementList.size() - 1);
                    elementList.last().isContainedOnOneLine = false;
                }
                return startIndex - 1;
            }else if (line.at(startIndex) == ' '){
                return startIndex - 1;
            }
        }
    }
    return UnknownError;
}

int HkxXmlReader::readAttribute(const QByteArray & line, int startIndex){
    if (startIndex < 0){
        return -1;
    }
    QByteArray attribute(AVERAGE_ATTRIBUTE_LENGTH, '\0');
    QByteArray value(AVERAGE_ATTRIBUTE_VALUE_LENGTH, '\0');
    int index = 0;
    for (; startIndex < line.size(); startIndex++, index++){
        if (line.at(startIndex) == '='){
            startIndex++;
            break;
        }
        /*if ((line.at(startIndex) < '0') || (line.at(startIndex) > '9' && line.at(startIndex) < 'A') || (line.at(startIndex) > 'Z' && line.at(startIndex) < 'a') || (line.at(startIndex) > 'z')){
            return -1;
        }*/
        if (index >= attribute.size()){
            attribute.append(QByteArray(attribute.size() * 2, '\0'));
        }
        attribute[index] = line.at(startIndex);
    }
    attribute.truncate(index);
    if (startIndex >= line.size() || line.at(startIndex) != '\"'){
        return MalformedAttribute;
    }
    if (elementList.isEmpty()){
        return OrphanedAttribute;
    }
    elementList.last().attributeList.append(attribute);
    startIndex++;
    index = 0;
    for (; startIndex < line.size(); startIndex++, index++){
        if (line.at(startIndex) == '\"'){
            startIndex++;
            break;
        }
        /*if ((line.at(startIndex) < '0') || (line.at(startIndex) > '9' && line.at(startIndex) < 'A') || (line.at(startIndex) > 'Z' && line.at(startIndex) < 'a') || (line.at(startIndex) > 'z')){
            return -1;
        }*/
        if (index >= value.size()){
            value.append(QByteArray(value.size() * 2, '\0'));
        }
        value[index] = line.at(startIndex);
    }
    value.truncate(index);
    elementList.last().attributeList.last().value = value;
    if (startIndex < line.size()){
        if (startIndex < line.size()){
            if (line.at(startIndex) == '>'){
                return startIndex - 1;
            }else if (line.at(startIndex) == ' '){
                return startIndex - 1;
            }
        }
    }
    return startIndex;
}

int HkxXmlReader::readValue(const QByteArray & line, int startIndex, bool isValueSplitOnMultipleLines){
    QByteArray value(AVERAGE_VALUE_LENGTH, '\0');
    int index = 0;
    if (elementList.isEmpty() || startIndex < 0 || startIndex >= line.size()){
        return UnknownError;
    }
    //Fucking carriage return character...
    if (line.at(startIndex) == '\n' || line.at(startIndex) == '\r'){
        indexOfElemTags.append(elementList.size() - 1);
        elementList.last().isContainedOnOneLine = false;
        return startIndex;
    }else if (line.at(startIndex) == '<'){//empty element
        //elementList.last().isContainedOnOneLine = true;
        return startIndex - 1;
    }
    //.last().isContainedOnOneLine = true;
    for (; startIndex < line.size(); startIndex++, index++){
        //Need to deal with embedded comments...FFS
        if (line.at(startIndex) == '<'){
            startIndex++;
            if (startIndex < line.size()){
                if (line.at(startIndex) == '!'){
                    startIndex = skipComment(line, startIndex + 1);
                    if (startIndex < 0){
                        return -1;
                    }
                }else{
                    break;
                }
            }else{
                return InvalidElementValue;
            }
        }else if (line.at(startIndex) == '\n' || line.at(startIndex) == '\r'){
            if (elementList.isEmpty()){
                return UnknownError;
            }
            value.truncate(index);
            if (isValueSplitOnMultipleLines){
                elementList.last().value = elementList.last().value.append(' '+QByteArray(value.constData()));
            }
            return startIndex + 1;
        }
        /*if ((line.at(startIndex) < '0') || (line.at(startIndex) > '9' && line.at(startIndex) < 'A') || (line.at(startIndex) > 'Z' && line.at(startIndex) < 'a') || (line.at(startIndex) > 'z')){
            return -1;
        }*/
        if (index >= value.size()){
            value.append(QByteArray(value.size() * 2, '\0'));
        }
        value[index] = line.at(startIndex);
    }
    value.truncate(index);
    if (elementList.isEmpty()){
        return UnknownError;
    }
    elementList.last().value = value;
    return startIndex - 1;
}

int HkxXmlReader::skipComment(const QByteArray & line, int index){
    for (int j = 0; index < line.size(), j < 2; index++, j++){
        if (line.at(index) != '-'){
            return MalformedComment;
        }
    }
    for (; index < line.size(), line.at(index) != '-'; index++);
    if (index >= line.size()){
        return MalformedComment;
    }
    for (int j = 0; index < line.size(), j < 2; index++, j++){
        if (line.at(index) != '-'){
            return MalformedComment;
        }
    }
    if (line.at(index) != '>'){
        return MalformedComment;
    }
    return index + 1;
}

HkxXmlReader::HkxXmlParseLine HkxXmlReader::readNextLine(){
    QByteArray line = hkxXmlFile->readLine(MAX_HKXXML_LINE_LENGTH);
    if (line == "\t\t\t\t<hkcstring> iState_NPCSneaking</hkcstring>\n"){
        int o = 0;
    }
    lineNumber++;
    if (line.isEmpty()){
        isEOF = true;
        return EmptyLineEndFile;
    }else if (line.size() == 1){
        if (line.at(0) != '\n'){
            //hkxXmlFile->writeToLog("HkxXmlReader: readNextLine() failed because an orphaned character was found on line "+QString::number(lineNumber)+"!", true);
            return OrphanedCharacter;
        }else{
            return EmptyLine;
        }
    }
    bool isIsolatedEndElemTag = true;
    bool isValueSplitOnMultipleLines = true;
    for (int i = 0; i < line.size(); i++){
        if (line.at(i) == '<'){
            isValueSplitOnMultipleLines = false;
            i++;
            if (i < line.size()){
                if (line.at(i) == '/'){
                    i = readElementTag(line, i + 1, isIsolatedEndElemTag, true);
                }else if (line.at(i) == '!'){
                    i = skipComment(line, i + 1);
                }else{
                    isIsolatedEndElemTag = false;
                    i = readElementTag(line, i, isIsolatedEndElemTag, false);
                }
                if (i < 0){
                    return UnknownError;
                }
            }
        }else if (line.at(i) == '\t'){
            //continue...
        }else if (line.at(i) == '\n' || line.at(i) == '\r'){
            return NoError;
        }else if (line.at(i) == '>'){
            i = readValue(line, i + 1, false);
        }else if (line.at(i) == ' '){
            //isValueSplitOnMultipleLines = false;
            int temp = i - 1;
            if (temp >= 0 && line.at(temp) == '>'){
                i = readValue(line, i + 1, false);
            }else{
                i++;
                if (i < line.size()){
                    i = readAttribute(line, i);
                }
            }
        }else{
            i = readValue(line, i, isValueSplitOnMultipleLines);
        }
        if (i < 0){
            return UnknownError;
        }
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

void HkxXmlReader::setFile(HkxFile *file){
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
