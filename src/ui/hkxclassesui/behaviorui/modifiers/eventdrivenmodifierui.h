#ifndef EVENTDRIVENMODIFIERUI_H
#define EVENTDRIVENMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class BehaviorGraphView;
class TableWidget;
class LineEdit;
class hkbEventDrivenModifier;
class QGridLayout;
class HkxObject;
class CheckBox;
class GenericTableWidget;
class hkbVariableBindingSet;

class EventDrivenModifierUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    EventDrivenModifierUI();
    virtual ~EventDrivenModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewModifiers(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setActivateEventId(int index, const QString & name);
    void setDeactivateEventId(int index, const QString & name);
    void setActiveByDefault();
    void setBindingVariable(int index, const QString & name);
    void eventTableElementSelected(int index, const QString &name);
    void viewSelected(int row, int column);
    void setModifier(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *modifiers, GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path);
    void variableRenamed(const QString & name, int index);
    void eventRenamed(const QString & name, int index);
    void modifierRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
private:
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    hkbEventDrivenModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    CheckBox *activeByDefault;
};

#endif // EVENTDRIVENMODIFIERUI_H
