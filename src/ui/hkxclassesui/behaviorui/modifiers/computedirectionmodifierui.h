#ifndef COMPUTEDIRECTIONMODIFIERUI_H
#define COMPUTEDIRECTIONMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbComputeDirectionModifier;
class LineEdit;
class QuadVariableWidget;
class CheckBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class DoubleSpinBox;

class ComputeDirectionModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    ComputeDirectionModifierUI();
    virtual ~ComputeDirectionModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setPointIn();
    void setPointOut();
    void setGroundAngleOut();
    void setUpAngleOut();
    void setVerticalOffset();
    void setReverseGroundAngle();
    void setReverseUpAngle();
    void setProjectPoint();
    void setNormalizePoint();
    void setComputeOnlyOnce();
    void setComputedOutput();
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
    hkbComputeDirectionModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *pointIn;
    QuadVariableWidget *pointOut;
    DoubleSpinBox *groundAngleOut;
    DoubleSpinBox *upAngleOut;
    DoubleSpinBox *verticalOffset;
    CheckBox *reverseGroundAngle;
    CheckBox *reverseUpAngle;
    CheckBox *projectPoint;
    CheckBox *normalizePoint;
    CheckBox *computeOnlyOnce;
    CheckBox *computedOutput;
};

#endif // COMPUTEDIRECTIONMODIFIERUI_H
