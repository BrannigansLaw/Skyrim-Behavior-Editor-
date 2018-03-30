#ifndef COMPUTEROTATIONTOTARGETMODIFIERUI_H
#define COMPUTEROTATIONTOTARGETMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbComputeRotationToTargetModifier;
class LineEdit;
class QuadVariableWidget;
class CheckBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class DoubleSpinBox;

class ComputeRotationToTargetModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    ComputeRotationToTargetModifierUI();
    virtual ~ComputeRotationToTargetModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setRotationOut();
    void setTargetPosition();
    void setCurrentPosition();
    void setCurrentRotation();
    void setLocalAxisOfRotation();
    void setLocalFacingDirection();
    void setResultIsDelta();
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties);
    void variableRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList headerLabels;
    hkbComputeRotationToTargetModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *rotationOut;
    QuadVariableWidget *targetPosition;
    QuadVariableWidget *currentPosition;
    QuadVariableWidget *currentRotation;
    QuadVariableWidget *localAxisOfRotation;
    QuadVariableWidget *localFacingDirection;
    CheckBox *resultIsDelta;
};

#endif // COMPUTEROTATIONTOTARGETMODIFIERUI_H
