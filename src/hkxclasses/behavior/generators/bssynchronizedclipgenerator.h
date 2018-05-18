#ifndef BSSYNCHRONIZEDCLIPGENERATOR_H
#define BSSYNCHRONIZEDCLIPGENERATOR_H

#include "hkbgenerator.h"

class BSSynchronizedClipGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class BSSynchronizedClipGeneratorUI;
public:
    BSSynchronizedClipGenerator(HkxFile *parent, long ref = 0);
    virtual ~BSSynchronizedClipGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int, DataIconManager *obj);
    bool removeObjectAt(int index);
    BSSynchronizedClipGenerator& operator=(const BSSynchronizedClipGenerator&);
    BSSynchronizedClipGenerator(const BSSynchronizedClipGenerator &);
private:
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    HkxSharedPtr pClipGenerator;
    QString syncAnimPrefix;
    bool bSyncClipIgnoreMarkPlacement;
    qreal fGetToMarkTime;
    qreal fMarkErrorThreshold;
    bool bLeadCharacter;
    bool bReorientSupportChar;
    bool bApplyMotionFromRoot;
    int sAnimationBindingIndex;
};

#endif // BSSYNCHRONIZEDCLIPGENERATOR_H
