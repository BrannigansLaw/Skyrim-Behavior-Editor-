#ifndef RIGIDBODYRAGDOLLCONTROLSMODIFIERUI_H
#define RIGIDBODYRAGDOLLCONTROLSMODIFIERUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbRigidBodyRagdollControlsModifier;
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

class RigidBodyRagdollControlsModifierUI: QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    RigidBodyRagdollControlsModifierUI();
    virtual ~RigidBodyRagdollControlsModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewRagdollBones(int index);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setHierarchyGain();
    void setVelocityDamping();
    void setAccelerationGain();
    void setVelocityGain();
    void setPositionGain();
    void setPositionMaxLinearVelocity();
    void setPositionMaxAngularVelocity();
    void setSnapGain();
    void setSnapMaxLinearVelocity();
    void setSnapMaxAngularVelocity();
    void setSnapMaxLinearDistance();
    void setSnapMaxAngularDistance();
    void setDurationToBlend();
    void toggleBones(bool enable);
    void viewBones();
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
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString & path);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        CHILD_WIDGET = 1
    };
    static QStringList headerLabels;
    hkbRigidBodyRagdollControlsModifier *bsData;
    QGridLayout *topLyt;
    QGroupBox *groupBox;
    TableWidget *table;
    BoneIndexArrayUI *boneIndexUI;
    LineEdit *name;
    CheckBox *enable;
    DoubleSpinBox *hierarchyGain;
    DoubleSpinBox *velocityDamping;
    DoubleSpinBox *accelerationGain;
    DoubleSpinBox *velocityGain;
    DoubleSpinBox *positionGain;
    DoubleSpinBox *positionMaxLinearVelocity;
    DoubleSpinBox *positionMaxAngularVelocity;
    DoubleSpinBox *snapGain;
    DoubleSpinBox *snapMaxLinearVelocity;
    DoubleSpinBox *snapMaxAngularVelocity;
    DoubleSpinBox *snapMaxLinearDistance;
    DoubleSpinBox *snapMaxAngularDistance;
    DoubleSpinBox *durationToBlend;
    CheckButtonCombo *bones;
};

#endif // RIGIDBODYRAGDOLLCONTROLSMODIFIERUI_H
