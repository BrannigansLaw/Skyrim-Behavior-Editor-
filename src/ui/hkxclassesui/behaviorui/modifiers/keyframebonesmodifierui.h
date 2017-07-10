#ifndef KEYFRAMEBONESMODIFIERUI_H
#define KEYFRAMEBONESMODIFIERUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbKeyframeBonesModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class SpinBox;
class hkbVariableBindingSet;
class BoneIndexArrayUI;
class QGroupBox;
class KeyframeInfoUI;
class QuadVariableWidget;
class CheckButtonCombo;

class KeyframeBonesModifierUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    KeyframeBonesModifierUI();
    virtual ~KeyframeBonesModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index);
    void viewProperties(int index);
    void viewRagdollBones(int index);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void toggleKeyframedBonesList(bool enable);
    void viewKeyframedBonesList();
    void viewSelectedChild(int row, int column);
    void variableTableElementSelected(int index, const QString &name);
    void returnToWidget();
private:
    void connectSignals();
    void disconnectSignals();
    void setBindingVariable(int index, const QString & name);
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2);
    void addKeyframeInfo();
    void removeKeyframeInfo(int index);
    void loadDynamicTableRows();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *ragdollBones);
    void variableRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        KEYFRAME_WIDGET,
        BONE_INDEX_WIDGET
    };
    static QStringList headerLabels;
    hkbKeyframeBonesModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    QGroupBox *groupBox;
    BoneIndexArrayUI *boneIndexUI;
    KeyframeInfoUI *keyframeInfoUI;
    LineEdit *name;
    CheckBox *enable;
    CheckButtonCombo *keyframedBonesList;
};

#endif // KEYFRAMEBONESMODIFIERUI_H
