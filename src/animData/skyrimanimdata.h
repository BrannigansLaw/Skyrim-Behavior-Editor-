#ifndef SKYRIMANIMDATA_H
#define SKYRIMANIMDATA_H

#include <QString>
#include <QFile>
#include <QVector>

class QTextStream;

bool chopLine(QFile * file, QByteArray & line, uint & linecount);

class SkyrimAnimData
{
public:
    SkyrimAnimData();
    bool parse(QFile * file);
    //bool extract(const QString & projectname);
    bool write(const QString & filename);
    int getProjectIndex(const QString & projectname) const;
    bool appendClipGenerator(const QString &projectname, const AnimData & animdata);
    bool removeClipGenerator(const QString & projectname, const QString & clipname);
    bool appendAnimation(const QString & projectname, const SkyrimAnimData::AnimMotionData &motiondata);
    bool removeAnimation(const QString & projectname, int animationindex);
private:
    struct Trigger{
        Trigger(qreal t = 0, const QString & trigname = "")
            : time(t), name(trigname)
        {
            //
        }
        QString name;
        qreal time;
    };

    struct AnimData{
        AnimData(uint ind = 0, qreal speed = 0, qreal startcrop = 0, qreal endcrop = 0, const QVector <Trigger> & trigs = QVector <Trigger>())
            : animationIndex(ind), playbackSpeed(speed), cropStartTime(startcrop), cropEndTime(endcrop), triggers(trigs)
        {
            //
        }

        AnimData & operator =(const AnimData & other){
            clipGeneratorName = other.clipGeneratorName;
            animationIndex = other.animationIndex;
            playbackSpeed = other.playbackSpeed;
            cropStartTime = other.cropStartTime;
            cropEndTime = other.cropEndTime;
            triggers = other.triggers;
            return *this;
        }

        bool read(QFile * file, uint & lineCount);
        bool write(QFile * file, QTextStream & out) const;
        uint lineCount() const;

        QString clipGeneratorName;
        uint animationIndex;
        qreal playbackSpeed;
        qreal cropStartTime;
        qreal cropEndTime;
        QVector <Trigger> triggers;
    };

    struct Translation{
        Translation(qreal x = 0, qreal y = 0, qreal z = 0, qreal w = 0)
            : x(x), y(y), z(z), w(w)
        {
            //
        }
        qreal x;
        qreal y;
        qreal z;
        qreal w;
    };

    struct Rotation{
        Rotation(qreal c1 = 0, qreal c2 = 0, qreal c3 = 0, qreal c4 = 0, qreal c5 = 0)
            : coord1(c1), coord2(c2), coord3(c3), coord4(c4), coord5(c5)
        {
            //
        }
        qreal coord1;
        qreal coord2;
        qreal coord3;
        qreal coord4;
        qreal coord5;
    };

    struct AnimMotionData{
        AnimMotionData(uint ind = 0, qreal dur = 0, const QVector <Translation> & trans = QVector <Translation>(), const QVector <Rotation> & rots = QVector <Rotation>())
            : animationIndex(ind), duration(dur), translations(trans), rotations(rots)
        {
            //
        }

        AnimMotionData & operator =(const AnimMotionData & other){
            animationIndex = other.animationIndex;
            duration = other.duration;
            translations = other.translations;
            rotations = other.rotations;
            return *this;
        }

        bool read(QFile * file, uint &lineCount);
        bool write(QFile * file, QTextStream & out) const;
        uint lineCount() const;

        uint animationIndex;
        qreal duration;
        QVector <Translation> translations;
        QVector <Rotation> rotations;
    };

    struct ProjectAnimData{
        ProjectAnimData()
            : animationDataLines(0), animationMotionDataLines(0)
        {
            //
        }

        bool read(QFile * file);
        bool write(QFile &file, QTextStream &output) const;
        bool appendClipGenerator(const AnimData & animdata);
        bool removeClipGenerator(const QString & clipname);
        bool appendAnimation(const SkyrimAnimData::AnimMotionData &motiondata);
        bool removeAnimation(int animationindex);
        uint animationDataLines;
        uint animationMotionDataLines;
        QStringList projectFiles;
        QVector <AnimData> animationData;
        QVector <AnimMotionData> animationMotionData;
    };
private:
    QStringList projectNames;
    QVector <ProjectAnimData> animData;
};

#endif // SKYRIMANIMDATA_H
