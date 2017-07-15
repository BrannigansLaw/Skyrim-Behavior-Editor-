#ifndef BSDISTTRIGGERMODIFIERUI_H
#define BSDISTTRIGGERMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class BSDistTriggerModifier;
class LineEdit;
class QLineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;

class BSDistTriggerModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSDistTriggerModifierUI();
    virtual ~BSDistTriggerModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index);
    void viewProperties(int index);
    void viewEvents(int index);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setTargetPosition();
    void setDistance();
    void setDistanceTrigger();
    void setTriggerEventId(int index, const QString & name);
    void setTriggerEventPayload();
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events);
    void variableRenamed(const QString & name, int index);
    void eventRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList headerLabels;
    BSDistTriggerModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *targetPosition;
    DoubleSpinBox *distance;
    DoubleSpinBox *distanceTrigger;
    QLineEdit *triggerEventPayload;
};

#endif // BSDISTTRIGGERMODIFIERUI_H