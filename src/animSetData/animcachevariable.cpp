#include "animcachevariable.h"

AnimCacheVariable::AnimCacheVariable(const QString & name, uint val1, uint val2)
    : name(name), value1(val1), value2(val2)
{
    //
}

AnimCacheVariable::AnimCacheVariable(const AnimCacheVariable & other){
    name = other.name;
    value1 = other.value1;
    value2 = other.value2;
}

bool AnimCacheVariable::operator ==(const AnimCacheVariable & other) const{
    if (name != other.name || value1 != other.value1 || value2 != other.value2){
        return false;
    }
    return true;
}

bool AnimCacheVariable::read(QFile *file){
    if (!file || !file->isOpen()){
        return false;
    }
    QByteArray line;
    bool ok = false;
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    name = line;
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    value1 = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    value2 = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    return true;
}

bool AnimCacheVariable::write(QFile *file, QTextStream &out) const{
    if (!file || !file->isOpen()){
        return false;
    }
    out << name << "\n";
    out << QString::number(value1) << "\n";
    out << QString::number(value2) << "\n";
    return true;
}
