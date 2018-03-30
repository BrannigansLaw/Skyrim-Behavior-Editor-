#ifndef BSTWEENERMODIFIERUI_H
#define BSTWEENERMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class BSTweenerModifier;
class LineEdit;
class QuadVariableWidget;
class CheckBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class DoubleSpinBox;

class BSTweenerModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSTweenerModifierUI();
    virtual ~BSTweenerModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setTweenPosition();
    void setTweenRotation();
    void setUseTweenDuration();
    void setTweenDuration();
    void setTargetPosition();
    void setTargetRotation();
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
    BSTweenerModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    CheckBox *tweenPosition;
    CheckBox *tweenRotation;
    CheckBox *useTweenDuration;
    DoubleSpinBox *tweenDuration;
    QuadVariableWidget *targetPosition;
    QuadVariableWidget *targetRotation;
};

#endif // BSTWEENERMODIFIERUI_H
