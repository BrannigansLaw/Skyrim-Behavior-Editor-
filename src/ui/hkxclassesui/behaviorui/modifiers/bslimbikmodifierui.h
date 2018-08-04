#ifndef BSLIMBIKMODIFIERUI_H
#define BSLIMBIKMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class BSLimbIKModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;

class BSLimbIKModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSLimbIKModifierUI();
    virtual ~BSLimbIKModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setLimitAngleDegrees();
    void setStartBoneIndex(int index);
    void setEndBoneIndex(int index);
    void setGain();
    void setBoneRadius();
    void setCastOffset();
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
    BSLimbIKModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    DoubleSpinBox *limitAngleDegrees;
    ComboBox *startBoneIndex;
    ComboBox *endBoneIndex;
    DoubleSpinBox *gain;
    DoubleSpinBox *boneRadius;
    DoubleSpinBox *castOffset;
};

#endif // BSLIMBIKMODIFIERUI_H
