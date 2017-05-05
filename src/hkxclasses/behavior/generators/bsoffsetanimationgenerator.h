#ifndef BSOFFSETANIMATIONGENERATOR_H
#define BSOFFSETANIMATIONGENERATOR_H

#include "hkbgenerator.h"

class BSOffsetAnimationGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSOffsetAnimationGenerator(HkxFile *parent, long ref = 0);
    virtual ~BSOffsetAnimationGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    int getIndexToInsertIcon() const;
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int index, DataIconManager *obj);
    bool removeObjectAt(int index);
    BSOffsetAnimationGenerator& operator=(const BSOffsetAnimationGenerator&);
    BSOffsetAnimationGenerator(const BSOffsetAnimationGenerator &);
private:
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    HkxSharedPtr pDefaultGenerator;
    HkxSharedPtr pOffsetClipGenerator;
    qreal fOffsetVariable;
    qreal fOffsetRangeStart;
    qreal fOffsetRangeEnd;
};

#endif // BSOFFSETANIMATIONGENERATOR_H
