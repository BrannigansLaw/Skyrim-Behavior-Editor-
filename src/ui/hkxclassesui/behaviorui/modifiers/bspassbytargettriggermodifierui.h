#ifndef BSPASSBYTARGETTRIGGERMODIFIERUI_H
#define BSPASSBYTARGETTRIGGERMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class BSPassByTargetTriggerModifier;
class LineEdit;
class QLineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;

class BSPassByTargetTriggerModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSPassByTargetTriggerModifierUI();
    virtual ~BSPassByTargetTriggerModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setTargetPosition();
    void setRadius();
    void setMovementDirection();
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
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList headerLabels;
    BSPassByTargetTriggerModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *targetPosition;
    DoubleSpinBox *radius;
    QuadVariableWidget *movementDirection;
    QLineEdit *triggerEventPayload;
};

#endif // BSPASSBYTARGETTRIGGERMODIFIERUI_H
