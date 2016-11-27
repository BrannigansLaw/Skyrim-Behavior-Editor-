#ifndef BSSYNCHRONIZEDCLIPGENERATOR_H
#define BSSYNCHRONIZEDCLIPGENERATOR_H

#include "hkbgenerator.h"

class BSSynchronizedClipGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSSynchronizedClipGenerator(BehaviorFile *parent, long ref = 0);
    virtual ~BSSynchronizedClipGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    bool setChildAt(HkxObject *newChild, ushort index = 0);
    bool appendObject(hkbGenerator *objToAppend);
    bool removeObject(hkbGenerator *objToRemove, bool removeAll = true);
    int addChildrenToList(QList <HkxObjectExpSharedPtr> & list, bool reverseOrder);
    BSSynchronizedClipGenerator& operator=(const BSSynchronizedClipGenerator&);
    BSSynchronizedClipGenerator(const BSSynchronizedClipGenerator &);
private:
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    HkxObjectExpSharedPtr pClipGenerator;
    QString SyncAnimPrefix;
    bool bSyncClipIgnoreMarkPlacement;
    qreal fGetToMarkTime;
    qreal fMarkErrorThreshold;
    bool bLeadCharacter;
    bool bReorientSupportChar;
    bool bApplyMotionFromRoot;
    int sAnimationBindingIndex;
};

#endif // BSSYNCHRONIZEDCLIPGENERATOR_H
