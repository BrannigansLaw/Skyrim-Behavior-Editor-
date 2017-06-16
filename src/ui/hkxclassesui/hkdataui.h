#ifndef HKDATAUI_H
#define HKDATAUI_H

#include <QGroupBox>

class BehaviorGraphView;
class AnimationsUI;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QStackedLayout;
class HkxObject;
class QLabel;
class BSiStateTaggingGeneratorUI;
class ModifierGeneratorUI;
class ManualSelectorGeneratorUI;
class StateMachineUI;
class StateUI;
class TreeGraphicsItem;
class BehaviorVariablesUI;
class EventsUI;
class MainWindow;
class TransitionsUI;
class HkTransition;
class hkbStateMachineStateInfo;
class hkbStateMachine;
class GenericTableWidget;
class BlenderGeneratorUI;
class BehaviorGraphUI;
class BSLimbIKModifierUI;
class BSBoneSwitchGeneratorUI;
class BSOffsetAnimationGeneratorUI;
class BSCyclicBlendTransitionGeneratorUI;
class PoseMatchingGeneratorUI;

/**
 * To add support for a new class we need to add it to the "DATA_TYPE_LOADED" enum, add it to the stacked layout in the correct order,
 * connect it's name change signal to this, deal with any variable event name changes, add it to the list of widgets to be loaded and
 * add it to the set behavior view function if necessary.
 */

class HkDataUI: public QGroupBox
{
    Q_OBJECT
public:
    HkDataUI(const QString & title);
    virtual ~HkDataUI(){}
    BehaviorGraphView * loadBehaviorView(BehaviorGraphView *view);
    void setEventsVariablesAnimationsUI(EventsUI *events, BehaviorVariablesUI *variables, AnimationsUI *animations);
    void unloadDataWidget();
public slots:
    void changeCurrentDataWidget(TreeGraphicsItem *icon);
    void modifierAdded(const QString & name, const QString & type);
    void modifierNameChanged(const QString & newName, int index);
    void modifierRemoved(int index);
    void generatorAdded(const QString & name, const QString & type);
    void generatorNameChanged(const QString & newName, int index);
    void generatorRemoved(int index);
    void eventNameChanged(const QString & newName, int index);
    void eventAdded(const QString & name);
    void eventRemoved(int index);
    void animationNameChanged(const QString & newName, int index);
    void animationAdded(const QString & name);
    void animationRemoved(int index);
    void variableNameChanged(const QString & newName, int index);
    void variableAdded(const QString & name, const QString & type);
    void variableRemoved(int index);
private:
    HkDataUI& operator=(const HkDataUI&);
    HkDataUI(const HkDataUI &);
private:
    enum DATA_TYPE_LOADED{
        NO_DATA_SELECTED = 0,
        BS_I_STATE_TAG_GEN = 1,
        MODIFIER_GENERATOR = 2,
        MANUAL_SELECTOR_GENERATOR = 3,
        STATE_MACHINE = 4,
        BLENDER_GENERATOR = 5,
        BEHAVIOR_GRAPH,
        BS_LIMB_IK_MOD,
        BS_BONE_SWITCH_GENERATOR,
        BS_OFFSET_ANIMATION_GENERATOR,
        BS_CYCLIC_BLEND_TRANSITION_GENERATOR,
        POSE_MATCHING_GENERATOR
    };
    static QStringList generatorTypes;
    static QStringList modifierTypes;
    static QStringList variableTypes;
    EventsUI *eventsUI;
    BehaviorVariablesUI *variablesUI;
    AnimationsUI *animationsUI;
    BehaviorGraphView *behaviorView;
    QVBoxLayout *verLyt;
    QStackedLayout *stack;
    HkxObject *loadedData;
    GenericTableWidget *generatorsTable;
    GenericTableWidget *modifiersTable;
    GenericTableWidget *variablesTable;
    GenericTableWidget *eventsTable;
    GenericTableWidget *characterPropertiesTable;
    GenericTableWidget *animationsTable;
    QLabel *noDataL;
    BSiStateTaggingGeneratorUI *iStateTagGenUI;
    ModifierGeneratorUI *modGenUI;
    ManualSelectorGeneratorUI *manSelGenUI;
    StateMachineUI *stateMachineUI;
    BlenderGeneratorUI *blenderGeneratorUI;
    BehaviorGraphUI *behaviorGraphUI;
    BSLimbIKModifierUI *limbIKModUI;
    BSBoneSwitchGeneratorUI *boneSwitchUI;
    BSOffsetAnimationGeneratorUI *offsetAnimGenUI;
    BSCyclicBlendTransitionGeneratorUI *cyclicBlendTransGenUI;
    PoseMatchingGeneratorUI *poseMatchGenUI;
};

#endif // HKDATAUI_H
