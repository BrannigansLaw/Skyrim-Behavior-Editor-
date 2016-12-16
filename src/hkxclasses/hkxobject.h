#ifndef HKXOBJECT_H
#define HKXOBJECT_H

#include <QSharedData>

#include "src/utility.h"

class HkxFile;
class HkxXmlReader;
class HkxObjectExpSharedPtr;
class HkxXMLWriter;
class CustomTreeGraphicsViewIcon;
class hkbGenerator;

class HkxObject: public QSharedData
{
    friend class BehaviorFile;
    friend class ProjectFile;
    friend class CharacterFile;
    friend class SkeletonFile;
    friend class BSIStateManagerModifier;
    friend class BoneWeightArrayUI;
public:
    enum HkxType {
        TYPE_OTHER=0,
        TYPE_GENERATOR=1,
        TYPE_MODIFIER=2
    };
public:
    virtual ~HkxObject();
    void writeToLog(const QString & message, bool isError = false);
    void setProgressData(const QString & message, int value);
    HkxSignature getSignature() const;
    HkxType getType() const;
    virtual bool evaulateDataValidity();
    bool isDataValid() const;
    virtual bool link() = 0;
    virtual void unlink();
    virtual bool write(HkxXMLWriter *writer);
    virtual bool readData(const HkxXmlReader & reader, long index);
    QString getReferenceString() const;
    QString getBoolAsString(bool b) const;
    virtual bool setChildAt(HkxObject *newChild, ushort index = 0);
protected:
    HkxObject(HkxFile *parent, long ref = -1);
    HkxFile * getParentFile() const;
    void setDataValidity(bool isValid);
    void setType(HkxSignature sig, HkxType type);
    bool readMultipleVector4(const QByteArray &lineIn,  QVector <hkQuadVariable> & vectors);
    bool readReferences(const QByteArray &line, QList <HkxObjectExpSharedPtr> & children);
    bool readIntegers(const QByteArray &line, QVector<int> & ints);
    bool toBool(const QByteArray &line, bool *ok);
    bool readDoubles(const QByteArray &line, QVector<qreal> & doubles);
    hkVector3 readVector3(const QByteArray &lineIn, bool *ok);
    hkQuadVariable readVector4(const QByteArray &lineIn, bool *ok);
    void setIsWritten(bool written = true);
    bool getIsWritten() const;
    void setReference(int ref);
    long getReference() const;
private:
    HkxObject(const HkxObject &obj);
    HkxObject& operator=(const HkxObject&);
private:
    HkxFile *parentFile;
    long reference;
    HkxSignature signature;
    HkxType typeCheck;
    bool dataValid;
    bool isWritten;
};

class HkxObjectExpSharedPtr: public QExplicitlySharedDataPointer <HkxObject>
{
public:
    HkxObjectExpSharedPtr(HkxObject *obj = NULL, long ref = -1);
    bool operator==(const HkxObjectExpSharedPtr & other) const;
    void setReference(long ref);
    long getReference() const;
    bool readReference(long index, const HkxXmlReader & reader);
private:
    long reference;
};

class HkDynamicObject: public HkxObject
{
public:
    virtual ~HkDynamicObject();
    bool linkVar();
    void unlink();
    bool evaulateDataValidity();
    void addBinding(const QString & path, int varIndex, bool isProperty = false);
    void removeBinding(const QString & path);
    void removeBinding(int varIndex);
protected:
    HkDynamicObject(HkxFile *parent, long ref = -1);
protected:
    HkxObjectExpSharedPtr variableBindingSet;
private:
    HkDynamicObject& operator=(const HkDynamicObject&);
    HkDynamicObject(const HkDynamicObject &);

};

class DataIconManager: public HkDynamicObject
{
    friend class CustomTreeGraphicsView;
    friend class CustomTreeGraphicsViewIcon;
    friend class BehaviorFile;
public:
    virtual ~DataIconManager();
    void updateIconNames();
    virtual int getIndexToInsertIcon(HkxObject *child) const;
    QString getName() const;
    virtual bool hasChildren() const;
protected:
    virtual bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    virtual bool appendObject(DataIconManager *objToAppend);
    virtual bool removeObject(DataIconManager *objToRemove, bool removeAll = true);
    virtual int addChildrenToList(QList<DataIconManager *> & list, bool reverseOrder = true);
    DataIconManager(HkxFile *parent, long ref = -1);
    void appendIcon(CustomTreeGraphicsViewIcon * icon);
private:
    QList <CustomTreeGraphicsViewIcon *> icons;
};

#endif // HKXOBJECT_H
