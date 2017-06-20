#ifndef BLENDERGENERATORUI_H
#define BLENDERGENERATORUI_H

#include <QStackedWidget>

#include "src/utility.h"

class TableWidget;
class hkbBlenderGenerator;
class QGridLayout;
class HkxObject;
class DoubleSpinBox;
class CheckBox;
class SpinBox;
class QPushButton;
class QGroupBox;
class BlenderGeneratorChildUI;
class LineEdit;
class BehaviorGraphView;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;

class BlenderGeneratorUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BlenderGeneratorUI();
    virtual ~BlenderGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
    void viewVariables(int index);
    void viewGenerators(int index);
    void viewProperties(int index);
private slots:
    void setName();
    void setReferencePoseWeightThreshold();
    void setBlendParameter();
    void setMinCyclicBlendParameter();
    void setMaxCyclicBlendParameter();
    void setIndexOfSyncMasterChild();
    void setFlagSync();
    void setFlagSmoothGeneratorWeights();
    void setFlagDontDeactivateChildrenWithZeroWeights();
    void setFlagParametricBlend();
    void setFlagIsParametricBlendCyclic();
    void setFlagForceDensePose();
    void setSubtractLastChild();
    void viewSelectedChild(int row, int column);
    void returnToWidget(bool reloadData);
    void variableTableElementSelected(int index, const QString &name);
    void generatorTableElementSelected(int index, const QString &name);
private:
    void connectSignals();
    void disconnectSignals();
    void addChildWithGenerator();
    void removeChild(int index);
    void selectTableToView(bool viewproperties, const QString & path);
    void loadDynamicTableRows();
    void setBindingVariable(int index, const QString & name);
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString &tip1, const QString &tip2);
    void connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties);
    void variableRenamed(const QString & name, int index);
    void generatorRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        CHILD_WIDGET = 1
    };
    enum Generator_Type {
        STATE_MACHINE,
        MANUAL_SELECTOR_GENERATOR,
        BLENDER_GENERATOR,
        I_STATE_TAGGING_GENERATOR,
        BONE_SWITCH_GENERATOR,
        CYCLIC_BLEND_TRANSITION_GENERATOR,
        SYNCHRONIZED_CLIP_GENERATOR,
        MODIFIER_GENERATOR,
        OFFSET_ANIMATION_GENERATOR,
        POSE_MATCHING_GENERATOR,
        CLIP_GENERATOR,
        BEHAVIOR_REFERENCE_GENERATOR
    };
    static QStringList types;
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    hkbBlenderGenerator *bsData;
    QGroupBox *groupBox;
    BlenderGeneratorChildUI *childUI;
    QGridLayout *topLyt;
    ComboBox *typeSelectorCB;
    TableWidget *table;
    LineEdit *name;
    DoubleSpinBox *referencePoseWeightThreshold;
    DoubleSpinBox *blendParameter;
    DoubleSpinBox *minCyclicBlendParameter;
    DoubleSpinBox *maxCyclicBlendParameter;
    SpinBox *indexOfSyncMasterChild;
    CheckBox *flagSync;
    CheckBox *flagSmoothGeneratorWeights;
    CheckBox *flagDontDeactivateChildrenWithZeroWeights;
    CheckBox *flagParametricBlend;
    CheckBox *flagIsParametricBlendCyclic;
    CheckBox *flagForceDensePose;
    CheckBox *subtractLastChild;
};

#endif // BLENDERGENERATORUI_H
