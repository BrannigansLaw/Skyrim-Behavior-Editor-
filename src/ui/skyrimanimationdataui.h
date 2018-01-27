#ifndef SKYRIMANIMATIONDATAUI_H
#define SKYRIMANIMATIONDATAUI_H

#include <QStackedWidget>
#include <QVBoxLayout>

#include "src/utility.h"
#include "src/animData/skyrimanimdata.h"

class QGridLayout;
class TableWidget;
class SpinBox;
class LineEdit;
class ComboBox;
class QPushButton;
class DoubleSpinBox;
class CheckBox;
class QGroupBox;
class AnimationRotationUI;
class AnimationTranslationUI;

class SkyrimAnimationDataUI: public QStackedWidget
{
    Q_OBJECT
public:
    SkyrimAnimationDataUI();
    virtual ~SkyrimAnimationDataUI(){}
    void loadData(SkyrimAnimationMotionData * data);
signals:
    void returnToParent();
private slots:
    //void setName();
    void setDuration();
    void viewSelectedChild(int row, int column);
    void returnToWidget();
private:
    void connectSignals();
    void disconnectSignals();
    void addTranslation();
    void removeTranslation(int index);
    void addRotation();
    void removeRotation(int index);
    void loadDynamicTableRows();
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2);
private:
    enum ACTIVE_WIDGET{
        MAIN_WIDGET = 0,
        TRANSLATION_WIDGET = 1,
        ROTATION_WIDGET = 2
    };
    static QStringList headerLabels;
    int rotationsButtonRow;
    SkyrimAnimationMotionData *bsData;
    QGroupBox *groupBox;
    AnimationTranslationUI *translationUI;
    AnimationRotationUI *rotationUI;
    QGridLayout *topLyt;
    TableWidget *table;
    QPushButton *returnPB;
    //LineEdit *name;
    DoubleSpinBox *duration;
};

#endif // SKYRIMANIMATIONDATAUI_H
