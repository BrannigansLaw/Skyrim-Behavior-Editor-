#ifndef SKYRIMANIMSETDATA_H
#define SKYRIMANIMSETDATA_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QFile>
#include <QTextStream>

#include "src/animSetData/hkcrc.h"

class SkyrimAnimSetData
{
public:
    SkyrimAnimSetData();
    bool parse(QFile * file);
    bool addAnimationToCache(const QString & projectname, const QStringList & events, const QVector <AnimationInfo> & animations, const QVector <BehaviorVariable> & vars = QVector <BehaviorVariable> (), const QVector <ClipInfo> & clips = QVector <ClipInfo> (ClipInfo));
    bool removeAnimationFromCache(const QString & projectname, const QString & animationname, const QString & variablename = "", const QString & clipname = "");
private:
    struct AnimationInfo{
        AnimationInfo(const QString & path = "", const QString & name = "", bool compute = false){
            if (compute){
                HkCRC crcGen;
                crcPath = crcGen.compute(path.toLocal8Bit());
                crcAnimationName = crcGen.compute(name.toLocal8Bit());
            }else{
                crcPath = path;
                crcAnimationName = name;
            }
        }

        AnimationInfo & operator =(const AnimationInfo & other){
            crcPath = other.crcPath;
            crcAnimationName = other.crcAnimationName;
            return *this;
        }

        bool read(QFile * file);
        bool write(QFile * file, QTextStream & out) const;

        QString crcPath;
        QString crcAnimationName;
        //const QString xkh = "7891816";
    };

    struct ClipInfo{
        ClipInfo(const QString & eventname = "", const QStringList & clipgens = QStringList(), uint unk = 0)
            : eventName(eventname), clipGenerators(clipgens), unknown(unk)
        {
            //
        }

        ClipInfo & operator =(const ClipInfo & other){
            eventName = other.eventName;
            unknown = other.unknown;
            clipGenerators = other.clipGenerators;
            return *this;
        }

        bool read(QFile * file);
        bool write(QFile * file, QTextStream & out) const;

        QString eventName;
        uint unknown;   //Appears to be 1 for sprinting and normal attacks, zero otherwise in defaultmale...
        QStringList clipGenerators;
    };

    struct BehaviorVariable{
        BehaviorVariable(const QString & name = "", uint val1 = 0, uint val2 = 0)
            : name(name), value1(val1), value2(val2)
        {
            //
        }

        BehaviorVariable & operator =(const BehaviorVariable & other){
            name = other.name;
            value1 = other.value1;
            value2 = other.value2;
            return *this;
        }

        bool read(QFile * file);
        bool write(QFile * file, QTextStream & out) const;

        QString name;
        uint value1;
        uint value2;
    };

    struct AnimSetData{
        AnimSetData(const QStringList & events = QStringList(), const QVector <BehaviorVariable> & vars = QVector <BehaviorVariable>(),
                    const QVector <ClipInfo> & clips = QVector <ClipInfo>(), const QVector <AnimationInfo> & anims = QVector <AnimationInfo>())
            : cacheEvents(events), behaviorVariables(vars), clipGenerators(clips), animations(anims)
        {
            //
        }

        bool read(QFile * file);
        bool write(QFile *file, QTextStream &out) const;
        bool addAnimationToCache(const QStringList & events, const QVector <AnimationInfo> & animations, const QVector <BehaviorVariable> & vars = QVector <BehaviorVariable> (), const QVector <ClipInfo> & clips = QVector <ClipInfo> (ClipInfo));
        bool removeAnimationFromCache(const QString & animationname, const QString & variablename = "", const QString & clipname = "");

        QStringList cacheEvents;
        QVector <BehaviorVariable> behaviorVariables;
        QVector <ClipInfo> clipGenerators;
        QVector <AnimationInfo> animations;
    };

    struct ProjectAnimSetData{
        ProjectAnimSetData(const QStringList & files = QStringList(), const QVector <AnimSetData> & animdata = QVector <AnimSetData>())
            : fileNames(files), animSetData(animdata)
        {
            //
        }

        bool read(QFile * file);
        bool write(const QString & projectname) const;

        QStringList fileNames;
        QVector <AnimSetData> animSetData;
    };

private:
    bool extractProject(const QString & projectname);
    //bool extractSubProject(const QString & projectname, const QString & subprojectname);
private:
    QStringList projectNames;
    QVector <ProjectAnimSetData> projects;
};

#endif // SKYRIMANIMSETDATA_H
