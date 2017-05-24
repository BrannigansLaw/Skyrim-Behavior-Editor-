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
class QCheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;

class BSLimbIKModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSLimbIKModifierUI();
    void loadData(HkxObject *data);
signals:
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
    void tableItemSelected(int row, int column);
    void variableTableElementSelected(int index, const QString &name);
private:
    void variableRenamed(const QString & name, int index);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type);
    void setBindingVariable(int index, const QString & name);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList headerLabels;
    BSLimbIKModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    QCheckBox *enable;
    DoubleSpinBox *limitAngleDegrees;
    ComboBox *startBoneIndex;
    ComboBox *endBoneIndex;
    DoubleSpinBox *gain;
    DoubleSpinBox *boneRadius;
    DoubleSpinBox *castOffset;
};

#endif // BSLIMBIKMODIFIERUI_H
