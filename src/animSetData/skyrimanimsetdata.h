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
    struct AnimationInfo{
        AnimationInfo(const QString & path = "", const QString & name = "", bool compute = false){
            if (compute){
                HkCRC crcGen;
                crcPath = crcGen.compute(path.toLower().toLocal8Bit());
                crcAnimationName = crcGen.compute(name.toLower().toLocal8Bit());
            }else{
                crcPath = path;
                crcAnimationName = name;
            }
        }

        AnimationInfo(const AnimationInfo & other){
            crcPath = other.crcPath;
            crcAnimationName = other.crcAnimationName;
        }

        bool operator ==(const AnimationInfo & other) const{
            if (crcPath != other.crcPath || crcAnimationName != other.crcAnimationName){
                return false;
            }
            return true;
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

        bool operator ==(const ClipInfo & other) const{
            if (eventName != other.eventName || unknown != other.unknown || clipGenerators != other.clipGenerators){
                return false;
            }
            return true;
        }

        ClipInfo(const ClipInfo & other){
            eventName = other.eventName;
            unknown = other.unknown;
            clipGenerators = other.clipGenerators;
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

        BehaviorVariable(const BehaviorVariable & other){
            name = other.name;
            value1 = other.value1;
            value2 = other.value2;
        }

        bool operator ==(const BehaviorVariable & other) const{
            if (name != other.name || value1 != other.value1 || value2 != other.value2){
                return false;
            }
            return true;
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
        bool addAnimationToCache(const QString & event, const QVector <AnimationInfo> & anims, const QVector <BehaviorVariable> & vars = QVector <BehaviorVariable> (), const QVector <ClipInfo> & clips = QVector <ClipInfo> ());
        void removeAnimationFromCache(const QString & animationname, const QString & clipname = "", const QString & variablename = "");

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
        bool write(const QString & projectfilename) const;

        QStringList fileNames;
        QVector <AnimSetData> animSetData;
    };

public:
    SkyrimAnimSetData();
    bool parse(QFile * file);
    bool write(const QString & filename);
    bool addAnimationToCache(const QString & projectname, const QString & event, const QVector <AnimationInfo> & animations, const QVector <BehaviorVariable> & vars = QVector <BehaviorVariable> (), const QVector <ClipInfo> & clips = QVector <ClipInfo> ());
    bool removeAnimationFromCache(const QString & projectname, const QString & animationname, const QString & variablename = "", const QString & clipname = "");
    bool extractProject(const QString & projectname);
private:
    QStringList projectNames;
    QVector <ProjectAnimSetData> projects;
};

#endif // SKYRIMANIMSETDATA_H
