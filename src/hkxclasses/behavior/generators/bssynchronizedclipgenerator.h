#ifndef BSSYNCHRONIZEDCLIPGENERATOR_H
#define BSSYNCHRONIZEDCLIPGENERATOR_H

#include "hkbgenerator.h"

class BSSynchronizedClipGenerator final: public hkbGenerator
{
    friend class BSSynchronizedClipGeneratorUI;
public:
    BSSynchronizedClipGenerator(HkxFile *parent, long ref = 0);
    BSSynchronizedClipGenerator& operator=(const BSSynchronizedClipGenerator&) = delete;
    BSSynchronizedClipGenerator(const BSSynchronizedClipGenerator &) = delete;
    ~BSSynchronizedClipGenerator();
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
    QVector <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int, DataIconManager *obj);
    bool removeObjectAt(int index);
private:
    static uint refCount;
    static const QString classname;
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
    mutable std::mutex mutex;
};

#endif // BSSYNCHRONIZEDCLIPGENERATOR_H
