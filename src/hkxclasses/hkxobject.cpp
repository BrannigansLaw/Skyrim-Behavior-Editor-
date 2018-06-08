#include "src/hkxclasses/hkxobject.h"
#include "src/filetypes/behaviorfile.h"
#include "src/xml/hkxxmlreader.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/modifiers/hkbmodifier.h"

#include <QGraphicsScene>

/**
 * HkxObject
 */

HkxObject::HkxObject(HkxFile *parent, long ref)
    : parentFile(parent),
      dataValid(true),
      isWritten(false),
      isMerged(false),
      reference(ref)
{
    //
}

HkxSignature HkxObject::getSignature() const{
    return signature;
}

void HkxObject::setReference(int ref){
    reference = ref;
}

long HkxObject::getReference() const{
    return reference;
}

bool HkxObject::getIsMerged() const
{
    return isMerged;
}

void HkxObject::setIsMerged(bool value)
{
    isMerged = value;
}

QString HkxObject::getReferenceString() const{
    /*QString referenceString;
    if (reference > 999){
        referenceString = "#";
    }else if (reference > 99){
        referenceString = "#0";
    }else if (reference > 9){
        referenceString = "#00";
    }else{
        referenceString = "#000";
    }
    return referenceString+QString::number(reference);*/
    if (reference < 0){
        return "null";
    }
    return "#"+QString::number(reference);
}

QString HkxObject::getBoolAsString(bool b) const{
    if (b){
        return "true";
    }else{
        return "false";
    }
}

HkxObject::HkxType HkxObject::getType() const{
    return typeCheck;
}

void HkxObject::setDataValidity(bool isValid){
    dataValid = isValid;
    if (!dataValid && (getType() == TYPE_GENERATOR || getType() == TYPE_MODIFIER)){
        static_cast<DataIconManager *>(this)->setDataInvalid();
    }
}

bool HkxObject::isDataValid()const{
    return dataValid;
}

bool HkxObject::evaluateDataValidity(){
    //isDataValid = true;
    return true;
}

void HkxObject::setIsWritten(bool written){
    isWritten = written;
}

bool HkxObject::getIsWritten() const{
    return isWritten;
}

void HkxObject::unlink(){
    //
}

bool HkxObject::readData(const HkxXmlReader & , long ){
    return false;
}

bool HkxObject::merge(HkxObject *){
    return false;
}

void HkxObject::mergeEventIndex(int, int){
    //
}

bool HkxObject::isEventReferenced(int) const{
    return false;
}

bool HkxObject::isVariableReferenced(int) const{
    return false;
}

void HkxObject::updateEventIndices(int ){
    //
}

void HkxObject::updateReferences(long & ref){
    reference = ref;
}

void HkxObject::fixMergedEventIndices(BehaviorFile *){
    //
}

bool HkxObject::write(HkxXMLWriter *){
    return false;
}

HkxFile * HkxObject::getParentFile() const{
    return parentFile;
}

void HkxObject::setType(HkxSignature sig, HkxType type){
    signature = sig;typeCheck = type;
}

bool HkxObject::readReferences(const QByteArray &line, QList <HkxSharedPtr> & children){
    int size = 0;
    int start;
    bool ok = false;
    for (int i = 0; i < line.size(); i++){
        if (line.at(i) == '#'){
            i++;
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r');
            QByteArray value(size, '\0');
            for (int j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            children.append(HkxSharedPtr(nullptr, value.toLong(&ok)));
            if (!ok){
                return false;
            }
        }else if (line.at(i) == 'n'){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r');
            QByteArray value(size, '\0');
            for (int j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            if (value == "null"){
                children.append(HkxSharedPtr(nullptr));
                ok = true;
            } else {
                return false;
            }
        }
    }
    return ok;
}

bool HkxObject::readIntegers(const QByteArray &line, QVector<int> & ints){
    int size = 0;
    int start;
    bool ok = true;
    for (int i = 0; i < line.size(); i++){
        if ((line.at(i) >= '0' && line.at(i) <= '9') || (line.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r');
            QByteArray value(size, '\0');
            for (int j = 0; j < size; j++){
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

bool HkxObject::toBool(const QByteArray &line, bool *ok){
    *ok = true;
    if (line == "true"){
        return true;
    }else if (line == "false"){
        return false;
    }else {
        *ok = false;
        return false;
    }
}

bool HkxObject::readDoubles(const QByteArray &line, QVector<qreal> & doubles){
    int size = 0;
    int start;
    bool ok = false;
    for (int i = 0; i < line.size(); i++){
        if ((line.at(i) >= '0' && line.at(i) <= '9') || (line.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r');
            QByteArray value(size, '\0');
            for (int j = 0; j < size; j++){
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

hkVector3 HkxObject::readVector3(const QByteArray &lineIn, bool *ok){
    enum {X = 1, Y = 2, Z = 3};
    int size = 0;
    int start;
    ushort axisVar = 0;
    hkVector3 vector3;
    for (int i = 0; i < lineIn.size(); i++){
        if ((lineIn.at(i) >= '0' && lineIn.at(i) <= '9') || (lineIn.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < lineIn.size() && lineIn.at(i) != ' ' && lineIn.at(i) != ')');
            QByteArray value(size, '\0');
            for (int j = 0; j < size; j++){
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

hkQuadVariable HkxObject::readVector4(const QByteArray &lineIn, bool *ok){
    enum {X = 1, Y = 2, Z = 3, W = 4};
    int size = 0;
    int start;
    ushort axisVar = 0;
    hkQuadVariable vector;
    for (int i = 0; i < lineIn.size(); i++){
        if ((lineIn.at(i) >= '0' && lineIn.at(i) <= '9') || (lineIn.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < lineIn.size() && lineIn.at(i) != ' ' && lineIn.at(i) != ')');
            QByteArray value(size, '\0');
            for (int j = 0; j < size; j++){
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
                    return hkQuadVariable();
                }
                break;
            default:
                *ok = false;
                return hkQuadVariable();
            }
            if (!*ok){
                return hkQuadVariable();
            }
        }
    }
    return vector;
}

bool HkxObject::readMultipleVector4(const QByteArray &lineIn,  QVector <hkQuadVariable> & vectors){
    enum {X = 1, Y = 2, Z = 3, W = 4};
    int size = 0;
    int start;
    ushort axisVar = 0;
    hkQuadVariable vector;
    bool ok = false;
    bool loop = false;
    for (int i = 0; i < lineIn.size(); i++){
        if (lineIn.at(i) == '('){
            loop = true;
            i++;
            while (loop && i < lineIn.size()){
                if ((lineIn.at(i) >= '0' && lineIn.at(i) <= '9') || (lineIn.at(i) == '-')){
                    start = i;
                    size = 0;
                    do{
                        size++;
                        i++;
                    }while (i < lineIn.size() && lineIn.at(i) != ' ' && lineIn.at(i) != ')');
                    QByteArray value(size, '\0');
                    for (int j = 0; j < size; j++){
                        value[j] = lineIn[start];
                        start++;
                    }
                    axisVar++;
                    switch (axisVar){
                    case X:
                        vector.x = value.toDouble(&ok);
                        break;
                    case Y:
                        vector.y = value.toDouble(&ok);
                        break;
                    case Z:
                        vector.z = value.toDouble(&ok);
                        break;
                    case W:
                        vector.w = value.toDouble(&ok);
                        if (!ok){
                            return false;
                        }
                        loop = false;
                        vectors.append(vector);
                        axisVar = 0;
                        if (lineIn.at(i) != ')'){
                            return false;
                        }
                        break;
                    default:
                        return false;
                    }
                }
                i++;
            }
        }
    }
    return ok;
}

HkxObject::~HkxObject(){
    //
}

/**
 * HkxObjectExpSharedPtr
 */

HkxSharedPtr::HkxSharedPtr(HkxObject *obj, long ref)
    :QExplicitlySharedDataPointer(obj), reference(ref)
{
    //
}

bool HkxSharedPtr::operator==(const HkxSharedPtr & other) const{
    if (data() == other.data()){
        return true;
    }
    return false;
}
void HkxSharedPtr::setShdPtrReference(long ref){
    reference = ref;
}

long HkxSharedPtr::getShdPtrReference() const{
    return reference;
}

bool HkxSharedPtr::readShdPtrReference(long index, const HkxXmlReader & reader){
    bool ok = true;
    //need to remove the '#' from the reference string
    QByteArray temp = reader.getElementValueAt(index);
    if (temp.at(0) == '#'){
        temp.remove(0, 1);
    }
    if (temp == "null"){
        setShdPtrReference(-1);
    }else{
        setShdPtrReference(temp.toLong(&ok));
    }
    if (!ok){
        return false;
    }
    return true;
}

/**
 * HkDynamicObject
 */

HkDynamicObject::HkDynamicObject(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    //
}

void HkDynamicObject::addBinding(const QString & path, int varIndex, bool isProperty){
    if (variableBindingSet.data()){
        static_cast<hkbVariableBindingSet *>(variableBindingSet.data())->addBinding(path, varIndex,(hkbVariableBindingSet::hkBinding::BindingType)(isProperty));
    }else{
        hkbVariableBindingSet *bind = new hkbVariableBindingSet(getParentFile());
        bind->addBinding(path, varIndex,(hkbVariableBindingSet::hkBinding::BindingType)(isProperty));
        variableBindingSet = HkxSharedPtr(bind);
    }
}

void HkDynamicObject::removeBinding(const QString & path){
    if (variableBindingSet.data()){
        static_cast<hkbVariableBindingSet *>(variableBindingSet.data())->removeBinding(path);
    }
}

void HkDynamicObject::removeBinding(int varIndex){
    if (variableBindingSet.data()){
        static_cast<hkbVariableBindingSet *>(variableBindingSet.data())->removeBinding(varIndex);
    }
}

bool HkDynamicObject::isVariableReferenced(int variableindex) const{
    if (variableBindingSet.constData() && static_cast<const hkbVariableBindingSet *>(variableBindingSet.constData())->isVariableRefed(variableindex)){
        return true;
    }
    return false;
}

bool HkDynamicObject::merge(HkxObject *recessiveObject){
    hkbVariableBindingSet *obj = nullptr;
    if (recessiveObject && recessiveObject->getSignature() == HKB_VARIABLE_BINDING_SET){
        obj = static_cast<hkbVariableBindingSet *>(recessiveObject);
        if (variableBindingSet.data()){
            variableBindingSet.data()->merge(obj);
        }else if (obj){
            variableBindingSet = HkxSharedPtr(obj);
            getParentFile()->addObjectToFile(obj, -1);
        }
        return true;
    }else{
        return false;
    }
}

void HkDynamicObject::setBindingReference(int ref){
    if (variableBindingSet.data()){
        variableBindingSet.data()->setReference(ref);
    }
}

void HkDynamicObject::updateReferences(long &ref){
    setReference(ref);
    if (variableBindingSet.data()){
        ref++;
        variableBindingSet.data()->setReference(ref);
    }
}

void HkDynamicObject::mergeVariableIndices(int oldindex, int newindex){
    if (variableBindingSet.data()){
        static_cast<hkbVariableBindingSet *>(variableBindingSet.data())->mergeVariableIndex(oldindex, newindex);
    }
}

bool HkDynamicObject::linkVar(){
    if (!getParentFile()){
        return false;
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(variableBindingSet.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_VARIABLE_BINDING_SET){
            WRITE_TO_LOG("HkDynamicObject: linkVar()!\nThe linked object is not a HKB_VARIABLE_BINDING_SET!\nRemoving the link to the invalid object!");
            variableBindingSet = nullptr;
            return false;
        }
        variableBindingSet = *ptr;
    }
    return true;
}

void HkDynamicObject::unlink(){
    variableBindingSet = HkxSharedPtr();
}

bool HkDynamicObject::evaluateDataValidity(){
    if (variableBindingSet.data() && variableBindingSet.data()->getSignature() != HKB_VARIABLE_BINDING_SET){
        setDataValidity(false);
        return false;
    }
    setDataValidity(true);
    return true;
}

HkDynamicObject::~HkDynamicObject(){
    //
}
