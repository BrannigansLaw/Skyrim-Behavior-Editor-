#ifndef SKYRIMANIMATIONDATAUI_H
#define SKYRIMANIMATIONDATAUI_H

#include <QStackedWidget>
#include <QVBoxLayout>

#include "src/utility.h"
#include "src/animData/skyrimanimdata.h"
#include "src/ui/genericdatawidgets.h"

class HkxObject;
class BehaviorGraphView;
class RotationUI;
class hkbStateMachineStateInfo;
class QGridLayout;
class TableWidget;
class SpinBox;
class LineEdit;
class ComboBox;
class QPushButton;
class DoubleSpinBox;
class CheckBox;
class EventUI;
class QGroupBox;
class QuadVariableWidget;

class TranslationUI: public QWidget{
    Q_OBJECT
    TranslationUI()
        : lyt(new QVBoxLayout), returnPB(new QPushButton), translation(new QuadVariableWidget), time(new DoubleSpinBox)
    {
        lyt->addWidget(returnPB);
        lyt->addWidget(time);
        lyt->addWidget(translation);
        time->setPrefix("Time: ");
        translation->setVector3();
        setLayout(lyt);
    }
    QVBoxLayout *lyt;
    QPushButton *returnPB;
    QuadVariableWidget *translation;
    DoubleSpinBox *time;
};

class SkyrimAnimationDataUI: public QStackedWidget
{
    Q_OBJECT
public:
    SkyrimAnimationDataUI();
    virtual ~SkyrimAnimationDataUI(){}
    void loadData(SkyrimAnimData::AnimMotionData & data);
private slots:
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
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        TRANSLATION_WIDGET = 1,
        ROTATION_WIDGET = 2
    };
    static QStringList headerLabels;
    int rotationsButtonRow;
    SkyrimAnimData::AnimMotionData &bsData;
    //int animationIndex;
    QGroupBox *groupBox;
    TranslationUI *translationUI;
    RotationUI *rotationUI;
    QGridLayout *topLyt;
    TableWidget *table;
    QPushButton *returnPB;
    DoubleSpinBox *duration;
};

#endif // SKYRIMANIMATIONDATAUI_H
