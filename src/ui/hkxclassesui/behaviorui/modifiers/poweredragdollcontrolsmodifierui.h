#ifndef POWEREDRAGDOLLCONTROLSMODIFIERUI_H
#define POWEREDRAGDOLLCONTROLSMODIFIERUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbPoweredRagdollControlsModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;
class SpinBox;
class CheckButtonCombo;
class BoneIndexArrayUI;
class QGroupBox;
class BoneWeightArrayUI;

class PoweredRagdollControlsModifierUI: QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    PoweredRagdollControlsModifierUI();
    virtual ~PoweredRagdollControlsModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewRagdollBones(int index);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setMaxForce();
    void setTau();
    void setDamping();
    void setProportionalRecoveryVelocity();
    void setConstantRecoveryVelocity();
    void toggleBones(bool enable);
    void viewBones();
    void setPoseMatchingBone0(int index);
    void setPoseMatchingBone1(int index);
    void setPoseMatchingBone2(int index);
    void setMode(int index);
    void toggleBoneWeights(bool enable);
    void viewBoneWeights();
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
    void returnToWidget();
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *ragdollBones);
    void variableRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        BONE_INDEX_WIDGET = 1,
        BONE_WEIGHT_WIDGET = 2
    };
    static QStringList headerLabels;
    hkbPoweredRagdollControlsModifier *bsData;
    QGridLayout *topLyt;
    QGroupBox *groupBox;
    TableWidget *table;
    BoneIndexArrayUI *boneIndexUI;
    BoneWeightArrayUI *boneWeightsUI;
    LineEdit *name;
    CheckBox *enable;
    DoubleSpinBox *maxForce;
    DoubleSpinBox *tau;
    DoubleSpinBox *damping;
    DoubleSpinBox *proportionalRecoveryVelocity;
    DoubleSpinBox *constantRecoveryVelocity;
    CheckButtonCombo *bones;
    ComboBox *poseMatchingBone0;
    ComboBox *poseMatchingBone1;
    ComboBox *poseMatchingBone2;
    ComboBox *mode;
    CheckButtonCombo *boneWeights;
};

#endif // POWEREDRAGDOLLCONTROLSMODIFIERUI_H
