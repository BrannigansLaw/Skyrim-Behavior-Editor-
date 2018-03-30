#ifndef EXTRACTRAGDOLLPOSEMODIFIERUI_H
#define EXTRACTRAGDOLLPOSEMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbExtractRagdollPoseModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;

class ExtractRagdollPoseModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    ExtractRagdollPoseModifierUI();
    virtual ~ExtractRagdollPoseModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setPoseMatchingBone0(int index);
    void setPoseMatchingBone1(int index);
    void setPoseMatchingBone2(int index);
    void setEnableComputeWorldFromModel();
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
    hkbExtractRagdollPoseModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    ComboBox *poseMatchingBone0;
    ComboBox *poseMatchingBone1;
    ComboBox *poseMatchingBone2;
    CheckBox *enableComputeWorldFromModel;
};

#endif // EXTRACTRAGDOLLPOSEMODIFIERUI_H
