#ifndef BSCYCLICBLENDTRANSITIONGENERATORUI_H
#define BSCYCLICBLENDTRANSITIONGENERATORUI_H

#include <QStackedWidget>

#include "src/utility.h"

class TableWidget;
class BSCyclicBlendTransitionGenerator;
class QGridLayout;
class HkxObject;
class DoubleSpinBox;
class CheckButtonCombo;
class QPushButton;
class QGroupBox;
class LineEdit;
class BehaviorGraphView;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class EventUI;

class BSCyclicBlendTransitionGeneratorUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSCyclicBlendTransitionGeneratorUI();
    virtual ~BSCyclicBlendTransitionGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewGenerators(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
private slots:
    void setName();
    void setBlenderGenerator(int index, const QString & name);
    void setBlendParameter();
    void viewEventToFreezeBlendValue();
    void toggleEventToFreezeBlendValue(bool toggled);
    void viewEventToCrossBlend();
    void toggleEventToCrossBlend(bool toggled);
    void setTransitionDuration();
    void setBlendCurve(int index);
    void viewSelectedChild(int row, int column);
    void returnToWidget();
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void selectTableToView(bool viewproperties, const QString & path);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events);
    void variableRenamed(const QString & name, int index);
    void eventRenamed(const QString & name, int index);
    void generatorRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        EVENT_WIDGET = 1
    };
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    BSCyclicBlendTransitionGenerator *bsData;
    QGroupBox *groupBox;
    EventUI *eventUI;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckButtonCombo *eventToFreezeBlendValue;
    CheckButtonCombo *eventToCrossBlend;
    DoubleSpinBox *fBlendParameter;
    DoubleSpinBox *fTransitionDuration;
    ComboBox *eBlendCurve;
};

#endif // BSCYCLICBLENDTRANSITIONGENERATORUI_H
