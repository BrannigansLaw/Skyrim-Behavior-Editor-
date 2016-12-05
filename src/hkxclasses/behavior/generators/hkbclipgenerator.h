#ifndef HKBCLIPGENERATOR_H
#define HKBCLIPGENERATOR_H

#include "hkbgenerator.h"

class hkbClipGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbClipGenerator(HkxFile *parent, long ref = 0);
    virtual ~hkbClipGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    void removeData(){}
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbClipGenerator& operator=(const hkbClipGenerator&);
    hkbClipGenerator(const hkbClipGenerator &);
private:
    static QStringList PlaybackMode;    //{MODE_SINGLE_PLAY=0,MODE_LOOPING=1,MODE_USER_CONTROLLED=2,MODE_PING_PONG=3,MODE_COUNT=4};
    static QStringList ClipFlags;   //(FLAG_CONTINUE_MOTION_AT_END=1;FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE=2;FLAG_MIRROR=4;FLAG_FORCE_DENSE_POSE=8;FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS=16;FLAG_IGNORE_MOTION=32)*/
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    QString animationName;
    HkxObjectExpSharedPtr triggers;
    qreal cropStartAmountLocalTime;
    qreal cropEndAmountLocalTime;
    qreal startTime;
    qreal playbackSpeed;
    qreal enforcedDuration;
    qreal userControlledTimeFraction;
    qint16 animationBindingIndex;
    QString mode;
    QString flags;
};

#endif // HKBCLIPGENERATOR_H
