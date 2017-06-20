#ifndef HKBCLIPGENERATOR_H
#define HKBCLIPGENERATOR_H

#include "hkbgenerator.h"

class hkbClipGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class ClipGeneratorUI;
public:
    hkbClipGenerator(HkxFile *parent, long ref = 0, const QString & animationname = "/");
    virtual ~hkbClipGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    void removeData(){}
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    int getNumberOfTriggers() const;
    enum ClipFlag{
        FLAG_NONE = 0,
        FLAG_CONTINUE_MOTION_AT_END = 1,
        FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE = 2,
        FLAG_MIRROR = 4,
        FLAG_FORCE_DENSE_POSE = 8,
        FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS = 16,
        FLAG_IGNORE_MOTION = 32,
        INVALID_FLAG = 128
    };
    Q_DECLARE_FLAGS(ClipFlags, ClipFlag)
private:
    hkbClipGenerator& operator=(const hkbClipGenerator&);
    hkbClipGenerator(const hkbClipGenerator &);
private:
    static QStringList PlaybackMode;    //{MODE_SINGLE_PLAY=0,MODE_LOOPING=1,MODE_USER_CONTROLLED=2,MODE_PING_PONG=3,MODE_COUNT=4};
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    QString animationName;
    HkxSharedPtr triggers;
    qreal cropStartAmountLocalTime;
    qreal cropEndAmountLocalTime;
    qreal startTime;
    qreal playbackSpeed;
    qreal enforcedDuration;
    qreal userControlledTimeFraction;
    int animationBindingIndex;
    QString mode;
    QString flags;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(hkbClipGenerator::ClipFlags)

#endif // HKBCLIPGENERATOR_H
