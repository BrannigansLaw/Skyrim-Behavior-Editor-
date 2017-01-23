#ifndef BLENDERGENERATORUI_H
#define BLENDERGENERATORUI_H

#include <QStackedWidget>

#include "src/utility.h"

class QuadVariableWidget;
class TableWidget;
class hkbBlenderGenerator;
class QGridLayout;
class HkxObject;
class DoubleSpinBox;
class QCheckBox;
class QStackedLayout;
class SpinBox;
class QPushButton;
class QGroupBox;
class BlenderGeneratorChildUI;
class QLineEdit;
class BehaviorGraphView;
class ComboBox;
class GenericTableWidget;

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
    void setBindingVariable(int index, const QString & name);
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
    void addChild();
    void removeSelectedChild();
    void viewSelectedChild(int row, int column);
    void returnToWidget();
private:
    void connectChildUI(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *generators);
    void variableRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type);
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
    int rowIndexOfChildToRemove;
    BehaviorGraphView *behaviorView;
    hkbBlenderGenerator *bsData;
    QGroupBox *groupBox;
    BlenderGeneratorChildUI *childUI;
    QGridLayout *topLyt;
    QPushButton *addChildPB;
    ComboBox *typeSelectorCB;
    QPushButton *removeChildPB;
    TableWidget *table;
    QLineEdit *name;
    DoubleSpinBox *referencePoseWeightThreshold;
    DoubleSpinBox *blendParameter;
    DoubleSpinBox *minCyclicBlendParameter;
    DoubleSpinBox *maxCyclicBlendParameter;
    SpinBox *indexOfSyncMasterChild;
    QCheckBox *flagSync;
    QCheckBox *flagSmoothGeneratorWeights;
    QCheckBox *flagDontDeactivateChildrenWithZeroWeights;
    QCheckBox *flagParametricBlend;
    QCheckBox *flagIsParametricBlendCyclic;
    QCheckBox *flagForceDensePose;
    QCheckBox *subtractLastChild;
};

#endif // BLENDERGENERATORUI_H
