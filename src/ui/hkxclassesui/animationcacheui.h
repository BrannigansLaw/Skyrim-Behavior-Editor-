#ifndef ANIMATIONCACHEUI_H
#define ANIMATIONCACHEUI_H

#include <QStackedWidget>
#include <QStackedLayout>
#include <QGroupBox>

class AnimCacheProjectData;
class AnimCacheVariable;
class AnimCacheClipInfo;
class QFormLayout;
class LineEdit;
class SpinBox;
class QPushButton;
class TableWidget;
class AnimCacheAnimSetData;
class ProjectFile;

class CacheVariableWidget: public QGroupBox
{
    Q_OBJECT
public:
    CacheVariableWidget();
signals:
    void returnToWidget();
public:
    void loadData(AnimCacheVariable *data);
private slots:
    void setName(const QString & newname);
    void setMinimumValue(int newvalue);
    void setMaximumValue(int newvalue);
private:
    void connectSignals();
    void disconnectSignals();
private:
    AnimCacheVariable *bsData;
    QVBoxLayout *lyt;
    QFormLayout *nameLyt;
    QFormLayout *minValueLyt;
    QFormLayout *maxValueLyt;
    LineEdit *name;
    SpinBox *minValue;
    SpinBox *maxValue;
    QPushButton *returnButton;
};

class CacheClipWidget: public QGroupBox
{
    Q_OBJECT
    friend class CacheWidget;
public:
    CacheClipWidget();
    void loadData(AnimCacheClipInfo *data);
signals:
    void returnToWidget();
private slots:
    void setEventName(const QString & name);
    void setUnknown(int newvalue);
    void addClipGenerator();
    void removeClipGenerator();
    void setClipGeneratorAt(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
private:
    AnimCacheClipInfo *bsData;
    QVBoxLayout *lyt;
    QFormLayout *eventLyt;
    QFormLayout *unknownLyt;
    LineEdit *eventName;
    SpinBox *unknown;
    QHBoxLayout *buttonLyt;
    QPushButton *addClipButton;
    QPushButton *removeClipButton;
    TableWidget *clipGenerators;
    QPushButton *returnButton;
};

class CacheWidget: public QStackedWidget
{
    friend class AnimationCacheUI;
    Q_OBJECT
public:
    CacheWidget();
signals:
    void returnToMainWidget();
private slots:
    void loadData(AnimCacheAnimSetData *data, ProjectFile *project);
    void setCacheEventNameAt(int row, int column);
    void addCacheEvent();
    void removeCacheEvent();
    void addBehaviorVariable();
    void removeBehaviorVariable();
    void addClipGenerator();
    void removeClipGenerator();
    void setAnimationNameAt(int row, int column);
    void addAnimation();
    void removeAnimation();
    void viewBehaviorVariable(int row, int column);
    void viewClipGenerator(int row, int column);
    void returnToWidget();
private:
    void connectSignals();
    void disconnectSignals();
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        VARIABLE_WIDGET,
        CLIP_WIDGET
    };
private:
    AnimCacheAnimSetData *bsData;
    ProjectFile *projectData;
    QVBoxLayout *lyt;
    QHBoxLayout *eventButtonLyt;
    QPushButton *addEventButton;
    QPushButton *removeEventButton;
    TableWidget *cacheEvents;
    QHBoxLayout *variableButtonLyt;
    QPushButton *addVariableButton;
    QPushButton *removeVariableButton;
    TableWidget *behaviorVariables;
    QHBoxLayout *clipButtonLyt;
    QPushButton *addClipButton;
    QPushButton *removeClipButton;
    TableWidget *clipGenerators;
    QHBoxLayout *animationButtonLyt;
    QPushButton *addAnimationButton;
    QPushButton *removeAnimationButton;
    TableWidget *animations;
    CacheVariableWidget *variableUI;
    CacheClipWidget *clipUI;
    QPushButton *returnButton;
};

class AnimationCacheUI: public QStackedWidget
{
    Q_OBJECT
public:
    AnimationCacheUI();
    void loadData(ProjectFile *project);
private slots:
    void viewSelectedFile(int row, int column);
    void addCacheFile();
    void removeCacheFile();
    void returnToWidget();
private:
    void connectSignals();
    void disconnectSignals();
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        CACHE_WIDGET
    };
private:
    ProjectFile *projectData;
    AnimCacheProjectData *bsData;
    QVBoxLayout *lyt;
    QGroupBox *groupBox;
    TableWidget *cacheFiles;
    CacheWidget *cacheUI;
    QHBoxLayout *cacheButtonLyt;
    QPushButton *addCacheButton;
    QPushButton *removeCacheButton;
};

#endif // ANIMATIONCACHEUI_H
