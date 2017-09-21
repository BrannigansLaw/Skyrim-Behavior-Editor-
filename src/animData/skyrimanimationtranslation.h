#ifndef SKYRIMANIMATIONTRANSLATION_H
#define SKYRIMANIMATIONTRANSLATION_H

#include <QtCore>

class SkyrimAnimationTranslation
{
    friend class AnimationTranslationUI;
    friend class SkyrimAnimationMotionData;
public:
    SkyrimAnimationTranslation(qreal x = 0, qreal y = 0, qreal z = 0, qreal w = 0);
private:
    qreal localTime;
    qreal x;
    qreal y;
    qreal z;
};

#endif // SKYRIMANIMATIONTRANSLATION_H
