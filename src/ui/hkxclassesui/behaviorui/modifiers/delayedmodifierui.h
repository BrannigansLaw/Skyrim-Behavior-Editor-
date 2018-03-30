#ifndef DELAYEDMODIFIERUI_H
#define DELAYEDMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class BehaviorGraphView;
class TableWidget;
class LineEdit;
class hkbDelayedModifier;
class QGridLayout;
class HkxObject;
class CheckBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class DoubleSpinBox;

class DelayedModifierUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    DelayedModifierUI();
    virtual ~DelayedModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewModifiers(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setDelaySeconds();
    void setDurationSeconds();
    void setSecondsElapsed();
    void setBindingVariable(int index, const QString & name);
    void viewSelected(int row, int column);
    void setModifier(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *modifiers, GenericTableWidget *variables, GenericTableWidget *properties);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path);
    void variableRenamed(const QString & name, int index);
    void modifierRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
private:
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    hkbDelayedModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    DoubleSpinBox *delaySeconds;
    DoubleSpinBox *durationSeconds;
    DoubleSpinBox *secondsElapsed;
};

#endif // DELAYEDMODIFIERUI_H
