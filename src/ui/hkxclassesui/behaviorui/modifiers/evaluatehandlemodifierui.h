#ifndef EVALUATEHANDLEMODIFIERUI_H
#define EVALUATEHANDLEMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbEvaluateHandleModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;

class EvaluateHandleModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    EvaluateHandleModifierUI();
    virtual ~EvaluateHandleModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index);
    void viewProperties(int index);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setHandlePositionOut();
    void setHandleRotationOut();
    void setIsValidOut();
    void setExtrapolationTimeStep();
    void setHandleChangeSpeed();
    void setHandleChangeMode(int index);
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
    hkbEvaluateHandleModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *handlePositionOut;
    QuadVariableWidget *handleRotationOut;
    CheckBox *isValidOut;
    DoubleSpinBox *extrapolationTimeStep;
    DoubleSpinBox *handleChangeSpeed;
    ComboBox *handleChangeMode;
};

#endif // EVALUATEHANDLEMODIFIERUI_H
