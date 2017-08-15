#ifndef SKYRIMANIMSETDATA_H
#define SKYRIMANIMSETDATA_H

#include <QString>
#include <QStringList>
#include <QVector>

class SkyrimAnimSetData
{
public:
    SkyrimAnimSetData();
private:
    struct AnimationInfo{
        QString crcPath;
        QString crcAnimationName;
    };

    struct ClipInfo{
        QString eventName;
        uint unknown = 0;
        QStringList clipGeneraators;
    };

    struct BehaviorVariable{
        QString name;
        uint value1 = 0;
        uint value2 = 0;
    };

    struct AnimationData{
        QStringList equipEvents;
        QVector <BehaviorVariable> behaviorVariables;
        QVector <AnimationInfo> animations;
    };
private:
    const QString xkh = "7891816";
    QVector <AnimationData> animSetData;
};

#endif // SKYRIMANIMSETDATA_H
