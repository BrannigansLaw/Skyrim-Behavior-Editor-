#ifndef ANIMATIONSUI_H
#define ANIMATIONSUI_H

#include <QGroupBox>
//#include "src/animData/skyrimanimdata.h"

class HkDataUI;
class QVBoxLayout;
class QHBoxLayout;
class TableWidget;
class QPushButton;
class QSignalMapper;
class hkbCharacterStringData;
class HkxObject;
class SkyrimAnimationDataUI;
class CheckBox;
class QStackedLayout;
class ProjectAnimData;

class AnimationsUI: public QGroupBox
{
    Q_OBJECT
public:
    AnimationsUI(const QString & title);
    virtual ~AnimationsUI(){}
    void setHkDataUI(HkDataUI *ui);
    void loadData(HkxObject *data, ProjectAnimData *animdata);
    void clear();
private slots:
    void addAnimation();
    void removeAnimation();
    void renameSelectedAnimation();
    void viewAnimation(int row, int column);
    void returnToTable();
signals:
    void openAnimationFile(const QString & filename);
    void animationNameChanged(const QString & newName, int index);
    void animationAdded(const QString & name);
    void animationRemoved(int index);
private:
    AnimationsUI& operator=(const AnimationsUI&);
    AnimationsUI(const AnimationsUI &);
private:
    enum View {
        TABLE_WIDGET = 0,
        ANIMATION_WIDGET = 1
    };
    static QStringList headerLabels;
    HkDataUI *dataUI;
    QVBoxLayout *verLyt;
    ProjectAnimData *animData;
    hkbCharacterStringData *loadedData;
    TableWidget *table;
    QPushButton *addObjectPB;
    QHBoxLayout *buttonLyt;
    QPushButton *removeObjectPB;
    QStackedLayout *stackLyt;
    SkyrimAnimationDataUI *animationUI;
};
#endif // ANIMATIONSUI_H
