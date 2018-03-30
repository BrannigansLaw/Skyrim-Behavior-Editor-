#ifndef EVENTSFROMRANGEMODIFIERUI_H
#define EVENTSFROMRANGEMODIFIERUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbEventsFromRangeModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class SpinBox;
class hkbVariableBindingSet;
class EventRangeDataUI;
class QGroupBox;

class EventsFromRangeModifierUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    EventsFromRangeModifierUI();
    virtual ~EventsFromRangeModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setInputValue();
    void setLowerBound();
    void viewSelectedChild(int row, int column);
    void variableTableElementSelected(int index, const QString &name);
    void returnToWidget();
private:
    void connectSignals();
    void disconnectSignals();
    void setBindingVariable(int index, const QString & name);
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2);
    void addRange();
    void removeRange(int index);
    void eventRenamed(const QString & name, int index);
    void loadDynamicTableRows();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events);
    void variableRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    enum ACTIVE_WIDGET{
        MAIN_WIDGET = 0,
        CHILD_WIDGET = 1
    };
    static QStringList headerLabels;
    hkbEventsFromRangeModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    QGroupBox *groupBox;
    EventRangeDataUI *rangeUI;
    LineEdit *name;
    CheckBox *enable;
    DoubleSpinBox *inputValue;
    DoubleSpinBox *lowerBound;
};

#endif // EVENTSFROMRANGEMODIFIERUI_H
