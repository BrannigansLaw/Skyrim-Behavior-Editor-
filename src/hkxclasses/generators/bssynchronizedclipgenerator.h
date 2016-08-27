#ifndef BSSYNCHRONIZEDCLIPGENERATOR_H
#define BSSYNCHRONIZEDCLIPGENERATOR_H

#include "hkbgenerator.h"

class BSSynchronizedClipGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSSynchronizedClipGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~BSSynchronizedClipGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
private:
    BSSynchronizedClipGenerator& operator=(const BSSynchronizedClipGenerator&);
    BSSynchronizedClipGenerator(const BSSynchronizedClipGenerator &);
private:
    static uint refCount;
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
