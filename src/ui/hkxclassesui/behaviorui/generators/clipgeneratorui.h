#ifndef CLIPGENERATORUI_H
#define CLIPGENERATORUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbClipGenerator;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class SpinBox;
class hkbVariableBindingSet;
class ClipTriggerUI;
class QGroupBox;
class MainWindow;

class ClipGeneratorUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    ClipGeneratorUI();
    virtual ~ClipGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewAnimations(const QString & name);
    void generatorNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setAnimationName(int index, const QString & name);
    void setCropStartAmountLocalTime();
    void setCropEndAmountLocalTime();
    void setStartTime();
    void setPlaybackSpeed();
    void setEnforcedDuration();
    void setUserControlledTimeFraction();
    void setAnimationBindingIndex();
    void setMode(int index);
    void setFlagContinueMotionAtEnd();
    void setFlagSyncHalfCycleInPingPongMode();
    void setFlagMirror();
    void setFlagForceDensePose();
    void setFlagDontConvertAnnotationsToTriggers();
    void setFlagIgnoreMotion();
    void viewSelectedChild(int row, int column);
    void setBindingVariable(int index, const QString & name);
    void returnToWidget();
private:
    void connectSignals();
    void disconnectSignals();
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2);
    void addTrigger();
    void removeTrigger(int index);
    void eventRenamed(const QString & name, int index);
    //void animationRenamed(const QString & name, int index);
    void loadDynamicTableRows();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events, GenericTableWidget *animations);
    void variableRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        CHILD_WIDGET = 1
    };
    static QStringList headerLabels;
    hkbClipGenerator *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    QGroupBox *groupBox;
    ClipTriggerUI *triggerUI;
    LineEdit *name;
    DoubleSpinBox *cropStartAmountLocalTime;
    DoubleSpinBox *cropEndAmountLocalTime;
    DoubleSpinBox *startTime;
    DoubleSpinBox *playbackSpeed;
    DoubleSpinBox *enforcedDuration;
    DoubleSpinBox *userControlledTimeFraction;
    SpinBox *animationBindingIndex;
    ComboBox *mode;
    CheckBox *flagContinueMotionAtEnd;
    CheckBox *flagSyncHalfCycleInPingPongMode;
    CheckBox *flagMirror;
    CheckBox *flagForceDensePose;
    CheckBox *flagDontConvertAnnotationsToTriggers;
    CheckBox *flagIgnoreMotion;
};

#endif // CLIPGENERATORUI_H
