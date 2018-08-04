#ifndef COMPUTEROTATIONFROMAXISANGLEMODIFIERUI_H
#define COMPUTEROTATIONFROMAXISANGLEMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbComputeRotationFromAxisAngleModifier;
class LineEdit;
class QuadVariableWidget;
class CheckBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class DoubleSpinBox;

class ComputeRotationFromAxisAngleModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    ComputeRotationFromAxisAngleModifierUI();
    virtual ~ComputeRotationFromAxisAngleModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setRotationOut();
    void setAxis();
    void setAngleDegrees();
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties);
    void variableRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList headerLabels;
    hkbComputeRotationFromAxisAngleModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *rotationOut;
    QuadVariableWidget *axis;
    DoubleSpinBox *angleDegrees;
};

#endif // COMPUTEROTATIONFROMAXISANGLEMODIFIERUI_H
