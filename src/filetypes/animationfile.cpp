#include "animationfile.h"

AnimationFile::AnimationFile(MainWindow *window, const QString & name)
    : HkxFile(window, name),
      duration(0)
{
    getReader().setFile(this);
}

qreal AnimationFile::getDuration() const{
    return duration;
}

bool AnimationFile::addObjectToFile(HkxObject *, long )
{
    return true;
}

bool AnimationFile::parse(){
    if (!getReader().parse()){
        return false;
    }
    QByteArray value = getReader().findFirstValueWithAttributeValue("duration");
    bool ok = true;
    duration = value.toFloat(&ok);
    if (!ok){
        (qFatal("AnimationFile::parse(): The animation duration was not found!"));
    }
}
