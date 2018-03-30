#ifndef LOOKATMODIFIERUI_H
#define LOOKATMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbLookAtModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;
class SpinBox;

class LookAtModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    LookAtModifierUI();
    virtual ~LookAtModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setTargetWS();
    void setHeadForwardLS();
    void setNeckForwardLS();
    void setNeckRightLS();
    void setEyePositionHS();
    void setNewTargetGain();
    void setOnGain();
    void setOffGain();
    void setLimitAngleDegrees();
    void setLimitAngleLeft();
    void setLimitAngleRight();
    void setLimitAngleUp();
    void setLimitAngleDown();
    void setHeadIndex(int index);
    void setNeckIndex(int index);
    void setIsOn();
    void setIndividualLimitsOn();
    void setIsTargetInsideLimitCone();
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
    hkbLookAtModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *targetWS;
    QuadVariableWidget *headForwardLS;
    QuadVariableWidget *neckForwardLS;
    QuadVariableWidget *neckRightLS;
    QuadVariableWidget *eyePositionHS;
    DoubleSpinBox *newTargetGain;
    DoubleSpinBox *onGain;
    DoubleSpinBox *offGain;
    DoubleSpinBox *limitAngleDegrees;
    DoubleSpinBox *limitAngleLeft;
    DoubleSpinBox *limitAngleRight;
    DoubleSpinBox *limitAngleUp;
    DoubleSpinBox *limitAngleDown;
    ComboBox *headIndex;
    ComboBox *neckIndex;
    CheckBox *isOn;
    CheckBox *individualLimitsOn;
    CheckBox *isTargetInsideLimitCone;
};

#endif // LOOKATMODIFIERUI_H
