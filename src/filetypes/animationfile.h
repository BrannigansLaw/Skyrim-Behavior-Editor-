#ifndef ANIMATIONFILE_H
#define ANIMATIONFILE_H

#include "src/filetypes/hkxfile.h"

class AnimationFile: public HkxFile
{
    friend class MainWindow;
public:
    AnimationFile(MainWindow *window, const QString & name);
    qreal getDuration() const;
protected:
    bool addObjectToFile(HkxObject *, long);
    bool parse();
private:
    qreal duration;
    //QVector <SkyrimAnimationTranslation> translations;
    //QVector <SkyrimAnimationRotation> rotations;
};

#endif // ANIMATIONFILE_H
