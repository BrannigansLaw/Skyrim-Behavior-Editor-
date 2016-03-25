#include "hkobject.h"
#include "hkxxmlreader.h"

/**
 * HkObject
 */

bool HkObject::readIntegers(const QByteArray &line, QVector<qint16> & ints){
    qint16 size = 0;
    qint16 start;
    bool ok = true;
    for (qint16 i = 0; i < line.size(); i++){
        if ((line.at(i) >= '0' && line.at(i) <= '9') || (line.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            ints.append(value.toInt(&ok));
            if (!ok){
                return false;
            }
        }
    }
    return ok;
}

bool HkObject::readSingleBool(const QByteArray &line, bool *ok){
    *ok = true;
    qint16 start = 0;
    qint16 end = line.indexOf('\0', start);
    if (end == -1){
        *ok = false;
        return false;
    }
    qint16 size = end - start;
    QByteArray value(size, '\0');
    for (qint16 i = 0; i < size; i++){
        value[i] = line.at(start);
        start++;
    }
    if (value == "true"){
        return true;
    }else if (value == "false"){
        return false;
    }else {
        *ok = false;
        return false;
    }
}

qint16 HkObject::readSingleInt(const QByteArray &line, bool *ok){
    qint16 start = 0;
    qint16 end = line.indexOf('\0', start);
    if (end == -1){
        *ok = false;
        return -1;
    }
    qint16 size = end - start;
    QByteArray value(size, '\0');
    for (qint16 i = 0; i < size; i++){
        value[i] = line.at(start);
        start++;
    }
    return value.toLong(ok);
}

qreal HkObject::readSingleDouble(const QByteArray &line, bool *ok){
    qint16 start = 0;
    qint16 end = line.indexOf('\0', start);
    if (end == -1){
        *ok = false;
        return -1;
    }
    qint16 size = end - start;
    QByteArray value(size, '\0');
    for (qint16 i = 0; i < size; i++){
        value[i] = line.at(start);
        start++;
    }
    return value.toDouble(ok);
}

bool HkObject::readDoubles(const QByteArray &line, QVector<qreal> & doubles){
    qint16 size = 0;
    qint16 start;
    bool ok = true;
    for (qint16 i = 0; i < line.size(); i++){
        if ((line.at(i) >= '0' && line.at(i) <= '9') || (line.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            doubles.append(value.toDouble(&ok));
            if (!ok){
                return false;
            }
        }
    }
    return ok;
}

qint16 HkObject::readNumberElements(const QByteArray &line, bool *ok){
    qint16 start = 0;
    qint16 end = line.indexOf('\0', start);
    if (end == -1){
        *ok = false;
        return -1;
    }
    qint16 size = end - start;
    QByteArray value(size, '\0');
    for (qint16 i = 0; i < size; i++){
        value[i] = line.at(start);
        start++;
    }
    end = value.toInt(ok);
    if (!*ok){
        return -1;
    }
    return end;
}

qint16 HkObject::readReference(const QByteArray &lineIn, bool *ok){
    qint16 start = 0;
    qint16 end = lineIn.indexOf('\0', start);
    if (end == -1){
        *ok = false;
        return -1;
    }
    if (lineIn.at(start) == '#'){
        start++;
    }
    qint16 size = end - start;
    QString value(size, '\0');
    for (qint16 i = 0; i < size; i++){
        value[i] = lineIn.at(start);
        start++;
    }
    if (value == "null"){
        return 0;
    }
    end = value.toInt(ok);
    if (!*ok){
        return -1;
    }
    return end;
}

bool HkObject::readReferences(const QByteArray &line, QVector<qint16> & refs){
    qint16 size = 0;
    qint16 start;
    bool ok = true;
    for (qint16 i = 0; i < line.size(); i++){
        if (line.at(i) == '#'){
            i++;
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            refs.append(value.toInt(&ok));
            if (!ok){
                return false;
            }
        }else if (line.at(i) == 'n'){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            if (value == "null"){
                refs.append(0);
            } else {
                return false;
            }
        }
    }
    return ok;
}

hkVector3 HkObject::readVector3(const QByteArray &lineIn, bool *ok){
    enum {X = 1, Y = 2, Z = 3};
    qint16 size = 0;
    qint16 start;
    ushort axisVar = 0;
    hkVector3 vector3;
    for (qint16 i = 0; i < lineIn.size(); i++){
        if ((lineIn.at(i) >= '0' && lineIn.at(i) <= '9') || (lineIn.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < lineIn.size() && lineIn.at(i) != ' ' && lineIn.at(i) != ')');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = lineIn[start];
                start++;
            }
            axisVar++;
            switch (axisVar){
            case X:
                vector3.x = value.toDouble(ok);
                break;
            case Y:
                vector3.y = value.toDouble(ok);
                break;
            case Z:
                vector3.z = value.toDouble(ok);
                if (lineIn.at(i) != ')'){
                    *ok = false;
                    return hkVector3();
                }
                break;
            default:
                *ok = false;
                return hkVector3();
            }
            if (!*ok){
                return hkVector3();
            }
        }
    }
    return vector3;
}

hkVector4 HkObject::readVector4(const QByteArray &lineIn, bool *ok){
    enum {X = 1, Y = 2, Z = 3, W = 4};
    qint16 size = 0;
    qint16 start;
    ushort axisVar = 0;
    hkVector4 vector;
    for (qint16 i = 0; i < lineIn.size(); i++){
        if ((lineIn.at(i) >= '0' && lineIn.at(i) <= '9') || (lineIn.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < lineIn.size() && lineIn.at(i) != ' ' && lineIn.at(i) != ')');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = lineIn[start];
                start++;
            }
            axisVar++;
            switch (axisVar){
            case X:
                vector.x = value.toDouble(ok);
                break;
            case Y:
                vector.y = value.toDouble(ok);
                break;
            case Z:
                vector.z = value.toDouble(ok);
                break;
            case W:
                vector.w = value.toDouble(ok);
                if (lineIn.at(i) != ')'){
                    *ok = false;
                    return hkVector4();
                }
                break;
            default:
                *ok = false;
                return hkVector4();
            }
            if (!*ok){
                return hkVector4();
            }
        }
    }
    return vector;
}

/*
 * CLASS: HkObjectExpSharedPtr
*/

bool HkObjectExpSharedPtr::readReference(long index, const HkxXmlReader & reader){
    bool ok = true;
    //need to remove the '#' from the reference string
    QByteArray temp = reader.getElementValueAt(index);
    if (temp.at(0) == '#'){
        temp.remove(0, 1);
    }
    if (qstrcmp(temp.constData(), "null") == 0){
        setReference(-1);
    }else{
        setReference(temp.toLong(&ok));
    }
    if (!ok){
        return false;
    }
    return true;
}

/**
 * hkRootLevelContainer
 */

uint hkRootLevelContainer::refCount = 0;

bool hkRootLevelContainer::readData(const HkxXmlReader &reader, int startIndex){
    bool ok;
    while (startIndex < reader.getNumElements() && reader.getNthAttributeNameAt(startIndex, 1) != "class"){
        //need to use strcmp for dealing with dynamically sized qbytearrays
        if (qstrcmp(reader.getNthAttributeValueAt(startIndex, 0).constData(), "namedVariants") == 0){
            int numVariants = reader.getNthAttributeValueAt(startIndex, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numVariants; j++){
                namedVariants.append(hkRootLevelContainerNamedVariant());
                while (startIndex < reader.getNumElements() && reader.getNthAttributeNameAt(startIndex, 1) != "class"){
                    if (qstrcmp(reader.getNthAttributeValueAt(startIndex, 0).constData(), "name") == 0){
                        namedVariants.last().name = reader.getElementValueAt(startIndex);
                    }else if (qstrcmp(reader.getNthAttributeValueAt(startIndex, 0).constData(), "className") == 0){
                        namedVariants.last().className = reader.getElementValueAt(startIndex);
                    }else if (qstrcmp(reader.getNthAttributeValueAt(startIndex, 0).constData(), "variant") == 0){
                        //need to remove the '#' from the reference string
                        namedVariants.last().variant.setReference(reader.getElementValueAt(startIndex).remove(0,1).toLong(&ok));
                        if (!ok){
                            return false;
                        }
                    }
                    startIndex++;
                }
            }
        }
        startIndex++;
    }
    return true;
}











































