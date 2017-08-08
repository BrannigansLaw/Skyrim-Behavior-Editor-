#ifndef POSEMATCHINGGENERATORUI_H
#define POSEMATCHINGGENERATORUI_H

#include <QStackedWidget>

#include "src/utility.h"

class TableWidget;
class hkbPoseMatchingGenerator;
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
class QuadVariableWidget;

class PoseMatchingGeneratorUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    PoseMatchingGeneratorUI();
    virtual ~PoseMatchingGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
    void viewVariables(int index);
    void viewGenerators(int index);
    void viewProperties(int index);
    void viewEvents(int index);
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
    void setWorldFromModelRotation();
    void setBlendSpeed();
    void setMinSpeedToSwitch();
    void setMinSwitchTimeNoError();
    void setMinSwitchTimeFullError();
    void setStartPlayingEventId(int index, const QString &name);
    void setStartMatchingEventId(int index, const QString &name);
    void setRootBoneIndex(int index);
    void setOtherBoneIndex(int index);
    void setAnotherBoneIndex(int index);
    void setPelvisIndex(int index);
    void setMode(int index);
    void viewSelectedChild(int row, int column);
    void returnToWidget(bool reloadData);
    void eventTableElementSelected(int index, const QString &name);
    void variableTableElementSelected(int index, const QString &name);
    void generatorTableElementSelected(int index, const QString &name);
    void swapGeneratorIndices(int index1, int index2);
private:
    void connectSignals();
    void disconnectSignals();
    void eventRenamed(const QString & name, int index);
    void addChildWithGenerator();
    void removeChild(int index);
    void selectTableToView(bool viewproperties, const QString & path);
    void loadTableValue(int row, const QString &value);
    void loadDynamicTableRows();
    void setBindingVariable(int index, const QString & name);
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString &tip1, const QString &tip2);
    void connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events);
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
        BEHAVIOR_REFERENCE_GENERATOR,
        GAMEBYRO_SEQUENCE_GENERATOR
    };
    static QStringList types;
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    hkbPoseMatchingGenerator *bsData;
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
    QuadVariableWidget *worldFromModelRotation;
    DoubleSpinBox *blendSpeed;
    DoubleSpinBox *minSpeedToSwitch;
    DoubleSpinBox *minSwitchTimeNoError;
    DoubleSpinBox *minSwitchTimeFullError;
    ComboBox *rootBoneIndex;
    ComboBox *otherBoneIndex;
    ComboBox *anotherBoneIndex;
    ComboBox *pelvisIndex;
    ComboBox *mode;
};

#endif // POSEMATCHINGGENERATORUI_H
