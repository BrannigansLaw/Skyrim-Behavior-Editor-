#include "posematchinggeneratorui.h"

#include "blendergeneratorchildui.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbposematchinggenerator.h"
#include "src/hkxclasses/behavior/generators/hkbblendergeneratorchild.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include "src/ui/genericdatawidgets.h"
#include <QStackedLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 26

#define NAME_ROW 0
#define REFERENCE_POSE_WEIGHT_THRESHOLD_ROW 1
#define BLEND_PARAMETER_ROW 2
#define MIN_CYCLIC_BLEND_PARAMETER_ROW 3
#define MAX_CYCLIC_BLEND_PARAMETER_ROW 4
#define INDEX_OF_SYNC_MASTER_CHILD_ROW 5
#define FLAG_SYNC_ROW 6
#define FLAG_SMOOTH_GENERATOR_WEIGHTS_ROW 7
#define FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS_ROW 8
#define FLAG_PARAMETRIC_BLEND_ROW 9
#define FLAG_IS_PARAMETRIC_BLEND_CYCLIC_ROW 10
#define FLAG_FORCE_DENSE_POSE_ROW 11
#define SUBTRACT_LAST_CHILD_ROW 12
#define WORLD_FROM_MODEL_ROTATION_ROW 13
#define BLEND_SPEED_ROW 14
#define MIN_SPEED_TO_SWITCH_ROW 15
#define MIN_SWITCH_TIME_NO_ERROR_ROW 16
#define MIN_SWITCH_TIME_FULL_ERROR_ROW 17
#define START_PLAYING_EVENT_ID_ROW 18
#define START_MATCHING_EVENT_ID_ROW 19
#define ROOT_BONE_INDEX_ROW 20
#define OTHER_BONE_INDEX_ROW 21
#define ANOTHER_BONE_INDEX_ROW 22
#define PELVIS_INDEX_ROW 23
#define MODE_ROW 24
#define ADD_CHILD_ROW 25

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList PoseMatchingGeneratorUI::types = {
    "hkbStateMachine",
    "hkbManualSelectorGenerator",
    "hkbBlenderGenerator",
    "BSiStateTaggingGenerator",
    "BSBoneSwitchGenerator",
    "BSCyclicBlendTransitionGenerator",
    "BSSynchronizedClipGenerator",
    "hkbModifierGenerator",
    "BSOffsetAnimationGenerator",
    "hkbPoseMatchingGenerator",
    "hkbClipGenerator",
    "hkbBehaviorReferenceGenerator",
    "BGSGamebryoSequenceGenerator"
};

QStringList PoseMatchingGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

PoseMatchingGeneratorUI::PoseMatchingGeneratorUI()
    : behaviorView(NULL),
      bsData(NULL),
      groupBox(new QGroupBox("hkbBlenderGenerator")),
      childUI(new BlenderGeneratorChildUI),
      topLyt(new QGridLayout),
      typeSelectorCB(new ComboBox),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      referencePoseWeightThreshold(new DoubleSpinBox),
      blendParameter(new DoubleSpinBox),
      minCyclicBlendParameter(new DoubleSpinBox),
      maxCyclicBlendParameter(new DoubleSpinBox),
      indexOfSyncMasterChild(new SpinBox),
      flagSync(new CheckBox),
      flagSmoothGeneratorWeights(new CheckBox),
      flagDontDeactivateChildrenWithZeroWeights(new CheckBox),
      flagParametricBlend(new CheckBox),
      flagIsParametricBlendCyclic(new CheckBox),
      flagForceDensePose(new CheckBox),
      subtractLastChild(new CheckBox),
      worldFromModelRotation(new QuadVariableWidget),
      blendSpeed(new DoubleSpinBox),
      minSpeedToSwitch(new DoubleSpinBox),
      minSwitchTimeNoError(new DoubleSpinBox),
      minSwitchTimeFullError(new DoubleSpinBox),
      rootBoneIndex(new ComboBox),
      otherBoneIndex(new ComboBox),
      anotherBoneIndex(new ComboBox),
      pelvisIndex(new ComboBox),
      mode(new ComboBox)
{
    table->setAcceptDrops(true);
    table->viewport()->setAcceptDrops(true);
    table->setDragDropOverwriteMode(true);
    table->setDropIndicatorShown(true);
    table->setDragDropMode(QAbstractItemView::InternalMove);
    table->setRowSwapRange(BASE_NUMBER_OF_ROWS);
    typeSelectorCB->insertItems(0, types);
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, NAME_COLUMN, new TableWidgetItem("referencePoseWeightThreshold"));
    table->setItem(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, VALUE_COLUMN, referencePoseWeightThreshold);
    table->setItem(BLEND_PARAMETER_ROW, NAME_COLUMN, new TableWidgetItem("blendParameter"));
    table->setItem(BLEND_PARAMETER_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(BLEND_PARAMETER_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(BLEND_PARAMETER_ROW, VALUE_COLUMN, blendParameter);
    table->setItem(MIN_CYCLIC_BLEND_PARAMETER_ROW, NAME_COLUMN, new TableWidgetItem("minCyclicBlendParameter"));
    table->setItem(MIN_CYCLIC_BLEND_PARAMETER_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MIN_CYCLIC_BLEND_PARAMETER_ROW, VALUE_COLUMN, minCyclicBlendParameter);
    table->setItem(MAX_CYCLIC_BLEND_PARAMETER_ROW, NAME_COLUMN, new TableWidgetItem("maxCyclicBlendParameter"));
    table->setItem(MAX_CYCLIC_BLEND_PARAMETER_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MAX_CYCLIC_BLEND_PARAMETER_ROW, VALUE_COLUMN, maxCyclicBlendParameter);
    table->setItem(INDEX_OF_SYNC_MASTER_CHILD_ROW, NAME_COLUMN, new TableWidgetItem("indexOfSyncMasterChild"));
    table->setItem(INDEX_OF_SYNC_MASTER_CHILD_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INDEX_OF_SYNC_MASTER_CHILD_ROW, VALUE_COLUMN, indexOfSyncMasterChild);
    table->setItem(FLAG_SYNC_ROW, NAME_COLUMN, new TableWidgetItem("flagSync"));
    table->setItem(FLAG_SYNC_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_SYNC_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_SYNC_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_SYNC_ROW, VALUE_COLUMN, flagSync);
    table->setItem(FLAG_SMOOTH_GENERATOR_WEIGHTS_ROW, NAME_COLUMN, new TableWidgetItem("flagSmoothGeneratorWeights"));
    table->setItem(FLAG_SMOOTH_GENERATOR_WEIGHTS_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_SMOOTH_GENERATOR_WEIGHTS_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_SMOOTH_GENERATOR_WEIGHTS_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_SMOOTH_GENERATOR_WEIGHTS_ROW, VALUE_COLUMN, flagSmoothGeneratorWeights);
    table->setItem(FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS_ROW, NAME_COLUMN, new TableWidgetItem("flagDontDeactivateChildrenWithZeroWeights"));
    table->setItem(FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS_ROW, VALUE_COLUMN, flagDontDeactivateChildrenWithZeroWeights);
    table->setItem(FLAG_PARAMETRIC_BLEND_ROW, NAME_COLUMN, new TableWidgetItem("flagParametricBlend"));
    table->setItem(FLAG_PARAMETRIC_BLEND_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_PARAMETRIC_BLEND_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_PARAMETRIC_BLEND_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_PARAMETRIC_BLEND_ROW, VALUE_COLUMN, flagParametricBlend);
    table->setItem(FLAG_IS_PARAMETRIC_BLEND_CYCLIC_ROW, NAME_COLUMN, new TableWidgetItem("flagIsParametricBlendCyclic"));
    table->setItem(FLAG_IS_PARAMETRIC_BLEND_CYCLIC_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_IS_PARAMETRIC_BLEND_CYCLIC_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_IS_PARAMETRIC_BLEND_CYCLIC_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_IS_PARAMETRIC_BLEND_CYCLIC_ROW, VALUE_COLUMN, flagIsParametricBlendCyclic);
    table->setItem(FLAG_FORCE_DENSE_POSE_ROW, NAME_COLUMN, new TableWidgetItem("flagForceDensePose"));
    table->setItem(FLAG_FORCE_DENSE_POSE_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_FORCE_DENSE_POSE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_FORCE_DENSE_POSE_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_FORCE_DENSE_POSE_ROW, VALUE_COLUMN, flagForceDensePose);
    table->setItem(SUBTRACT_LAST_CHILD_ROW, NAME_COLUMN, new TableWidgetItem("subtractLastChild"));
    table->setItem(SUBTRACT_LAST_CHILD_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(SUBTRACT_LAST_CHILD_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(SUBTRACT_LAST_CHILD_ROW, VALUE_COLUMN, subtractLastChild);
    table->setItem(WORLD_FROM_MODEL_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("worldFromModelRotation"));
    table->setItem(WORLD_FROM_MODEL_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(WORLD_FROM_MODEL_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(WORLD_FROM_MODEL_ROTATION_ROW, VALUE_COLUMN, worldFromModelRotation);
    table->setItem(BLEND_SPEED_ROW, NAME_COLUMN, new TableWidgetItem("blendSpeed"));
    table->setItem(BLEND_SPEED_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(BLEND_SPEED_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(BLEND_SPEED_ROW, VALUE_COLUMN, blendSpeed);
    table->setItem(MIN_SPEED_TO_SWITCH_ROW, NAME_COLUMN, new TableWidgetItem("minSpeedToSwitch"));
    table->setItem(MIN_SPEED_TO_SWITCH_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MIN_SPEED_TO_SWITCH_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MIN_SPEED_TO_SWITCH_ROW, VALUE_COLUMN, minSpeedToSwitch);
    table->setItem(MIN_SWITCH_TIME_NO_ERROR_ROW, NAME_COLUMN, new TableWidgetItem("minSwitchTimeNoError"));
    table->setItem(MIN_SWITCH_TIME_NO_ERROR_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MIN_SWITCH_TIME_NO_ERROR_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MIN_SWITCH_TIME_NO_ERROR_ROW, VALUE_COLUMN, minSwitchTimeNoError);
    table->setItem(MIN_SWITCH_TIME_FULL_ERROR_ROW, NAME_COLUMN, new TableWidgetItem("minSwitchTimeFullError"));
    table->setItem(MIN_SWITCH_TIME_FULL_ERROR_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MIN_SWITCH_TIME_FULL_ERROR_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MIN_SWITCH_TIME_FULL_ERROR_ROW, VALUE_COLUMN, minSwitchTimeFullError);
    table->setItem(START_PLAYING_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("startPlayingEventId"));
    table->setItem(START_PLAYING_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(START_PLAYING_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(START_PLAYING_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(START_MATCHING_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("startMatchingEventId"));
    table->setItem(START_MATCHING_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(START_MATCHING_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(START_MATCHING_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(ROOT_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("rootBoneIndex"));
    table->setItem(ROOT_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(ROOT_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ROOT_BONE_INDEX_ROW, VALUE_COLUMN, rootBoneIndex);
    table->setItem(OTHER_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("otherBoneIndex"));
    table->setItem(OTHER_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(OTHER_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(OTHER_BONE_INDEX_ROW, VALUE_COLUMN, otherBoneIndex);
    table->setItem(ANOTHER_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("anotherBoneIndex"));
    table->setItem(ANOTHER_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ANOTHER_BONE_INDEX_ROW, VALUE_COLUMN, anotherBoneIndex);
    table->setItem(PELVIS_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("pelvisIndex"));
    table->setItem(PELVIS_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(PELVIS_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(PELVIS_INDEX_ROW, VALUE_COLUMN, pelvisIndex);
    table->setItem(MODE_ROW, NAME_COLUMN, new TableWidgetItem("mode"));
    table->setItem(MODE_ROW, TYPE_COLUMN, new TableWidgetItem("Mode", Qt::AlignCenter));
    table->setItem(MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(MODE_ROW, VALUE_COLUMN, mode);
    table->setItem(ADD_CHILD_ROW, NAME_COLUMN, new TableWidgetItem("Add Child With Generator", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new blender generator child with a generator of the type specified in the adjacent combo box"));
    table->setCellWidget(ADD_CHILD_ROW, TYPE_COLUMN, typeSelectorCB);
    table->setItem(ADD_CHILD_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Child", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected blender generator child"));
    table->setItem(ADD_CHILD_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Child", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected blender generator child"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(childUI);
    connectSignals();
}

void PoseMatchingGeneratorUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(referencePoseWeightThreshold, SIGNAL(editingFinished()), this, SLOT(setReferencePoseWeightThreshold()), Qt::UniqueConnection);
    connect(blendParameter, SIGNAL(editingFinished()), this, SLOT(setBlendParameter()), Qt::UniqueConnection);
    connect(minCyclicBlendParameter, SIGNAL(editingFinished()), this, SLOT(setMinCyclicBlendParameter()), Qt::UniqueConnection);
    connect(maxCyclicBlendParameter, SIGNAL(editingFinished()), this, SLOT(setMaxCyclicBlendParameter()), Qt::UniqueConnection);
    connect(indexOfSyncMasterChild, SIGNAL(editingFinished()), this, SLOT(setIndexOfSyncMasterChild()), Qt::UniqueConnection);
    connect(flagSync, SIGNAL(released()), this, SLOT(setFlagSync()), Qt::UniqueConnection);
    connect(flagSmoothGeneratorWeights, SIGNAL(released()), this, SLOT(setFlagSmoothGeneratorWeights()), Qt::UniqueConnection);
    connect(flagDontDeactivateChildrenWithZeroWeights, SIGNAL(released()), this, SLOT(setFlagDontDeactivateChildrenWithZeroWeights()), Qt::UniqueConnection);
    connect(flagParametricBlend, SIGNAL(released()), this, SLOT(setFlagParametricBlend()), Qt::UniqueConnection);
    connect(flagIsParametricBlendCyclic, SIGNAL(released()), this, SLOT(setFlagIsParametricBlendCyclic()), Qt::UniqueConnection);
    connect(flagForceDensePose, SIGNAL(released()), this, SLOT(setFlagForceDensePose()), Qt::UniqueConnection);
    connect(subtractLastChild, SIGNAL(released()), this, SLOT(setSubtractLastChild()), Qt::UniqueConnection);
    connect(worldFromModelRotation, SIGNAL(editingFinished()), this, SLOT(setWorldFromModelRotation()), Qt::UniqueConnection);
    connect(blendSpeed, SIGNAL(editingFinished()), this, SLOT(setBlendSpeed()), Qt::UniqueConnection);
    connect(minSpeedToSwitch, SIGNAL(editingFinished()), this, SLOT(setMinSpeedToSwitch()), Qt::UniqueConnection);
    connect(minSwitchTimeNoError, SIGNAL(editingFinished()), this, SLOT(setMinSwitchTimeNoError()), Qt::UniqueConnection);
    connect(minSwitchTimeFullError, SIGNAL(editingFinished()), this, SLOT(setMinSwitchTimeFullError()), Qt::UniqueConnection);
    connect(rootBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setRootBoneIndex(int)), Qt::UniqueConnection);
    connect(otherBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setOtherBoneIndex(int)), Qt::UniqueConnection);
    connect(anotherBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setAnotherBoneIndex(int)), Qt::UniqueConnection);
    connect(pelvisIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setPelvisIndex(int)), Qt::UniqueConnection);
    connect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(setMode(int)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(table, SIGNAL(itemDropped(int,int)), this, SLOT(swapGeneratorIndices(int,int)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(returnToParent(bool)), this, SLOT(returnToWidget(bool)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewGenerators(int)), this, SIGNAL(viewGenerators(int)), Qt::UniqueConnection);
}

void PoseMatchingGeneratorUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(referencePoseWeightThreshold, SIGNAL(editingFinished()), this, SLOT(setReferencePoseWeightThreshold()));
    disconnect(blendParameter, SIGNAL(editingFinished()), this, SLOT(setBlendParameter()));
    disconnect(minCyclicBlendParameter, SIGNAL(editingFinished()), this, SLOT(setMinCyclicBlendParameter()));
    disconnect(maxCyclicBlendParameter, SIGNAL(editingFinished()), this, SLOT(setMaxCyclicBlendParameter()));
    disconnect(indexOfSyncMasterChild, SIGNAL(editingFinished()), this, SLOT(setIndexOfSyncMasterChild()));
    disconnect(flagSync, SIGNAL(released()), this, SLOT(setFlagSync()));
    disconnect(flagSmoothGeneratorWeights, SIGNAL(released()), this, SLOT(setFlagSmoothGeneratorWeights()));
    disconnect(flagDontDeactivateChildrenWithZeroWeights, SIGNAL(released()), this, SLOT(setFlagDontDeactivateChildrenWithZeroWeights()));
    disconnect(flagParametricBlend, SIGNAL(released()), this, SLOT(setFlagParametricBlend()));
    disconnect(flagIsParametricBlendCyclic, SIGNAL(released()), this, SLOT(setFlagIsParametricBlendCyclic()));
    disconnect(flagForceDensePose, SIGNAL(released()), this, SLOT(setFlagForceDensePose()));
    disconnect(subtractLastChild, SIGNAL(released()), this, SLOT(setSubtractLastChild()));
    disconnect(worldFromModelRotation, SIGNAL(editingFinished()), this, SLOT(setWorldFromModelRotation()));
    disconnect(blendSpeed, SIGNAL(editingFinished()), this, SLOT(setBlendSpeed()));
    disconnect(minSpeedToSwitch, SIGNAL(editingFinished()), this, SLOT(setMinSpeedToSwitch()));
    disconnect(minSwitchTimeNoError, SIGNAL(editingFinished()), this, SLOT(setMinSwitchTimeNoError()));
    disconnect(minSwitchTimeFullError, SIGNAL(editingFinished()), this, SLOT(setMinSwitchTimeFullError()));
    disconnect(rootBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setRootBoneIndex(int)));
    disconnect(otherBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setOtherBoneIndex(int)));
    disconnect(anotherBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setAnotherBoneIndex(int)));
    disconnect(pelvisIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setPelvisIndex(int)));
    disconnect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(setMode(int)));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(table, SIGNAL(itemDropped(int,int)), this, SLOT(swapGeneratorIndices(int,int)));
    disconnect(childUI, SIGNAL(returnToParent(bool)), this, SLOT(returnToWidget(bool)));
    disconnect(childUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)));
    disconnect(childUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)));
    disconnect(childUI, SIGNAL(viewGenerators(int)), this, SIGNAL(viewGenerators(int)));
}

void PoseMatchingGeneratorUI::loadTableValue(int row, const QString &value){
    if (table->item(row, VALUE_COLUMN)){
        if (value != ""){
            table->item(row, VALUE_COLUMN)->setText(value);
        }else{
            table->item(row, VALUE_COLUMN)->setText("NONE");
        }
    }else{
        (qFatal("StateMachineUI::loadTableValue(): There is no table item here!!"));
    }
}

void PoseMatchingGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    hkbVariableBindingSet *varBind = NULL;
    if (data){
        if (data->getSignature() == HKB_POSE_MATCHING_GENERATOR){
            bsData = static_cast<hkbPoseMatchingGenerator *>(data);
            name->setText(bsData->name);
            referencePoseWeightThreshold->setValue(bsData->referencePoseWeightThreshold);
            blendParameter->setValue(bsData->blendParameter);
            minCyclicBlendParameter->setValue(bsData->minCyclicBlendParameter);
            maxCyclicBlendParameter->setValue(bsData->maxCyclicBlendParameter);
            indexOfSyncMasterChild->setValue(bsData->indexOfSyncMasterChild);
            bool ok = true;
            hkbPoseMatchingGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
            if (ok){
                if (flags.testFlag(hkbPoseMatchingGenerator::FLAG_SYNC)){
                    flagSync->setChecked(true);
                }else{
                    flagSync->setChecked(false);
                }
                if (flags.testFlag(hkbPoseMatchingGenerator::FLAG_SMOOTH_GENERATOR_WEIGHTS)){
                    flagSmoothGeneratorWeights->setChecked(true);
                }else{
                    flagSmoothGeneratorWeights->setChecked(false);
                }
                if (flags.testFlag(hkbPoseMatchingGenerator::FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS)){
                    flagDontDeactivateChildrenWithZeroWeights->setChecked(true);
                }else{
                    flagDontDeactivateChildrenWithZeroWeights->setChecked(false);
                }
                if (flags.testFlag(hkbPoseMatchingGenerator::FLAG_PARAMETRIC_BLEND)){
                    flagParametricBlend->setChecked(true);
                }else{
                    flagParametricBlend->setChecked(false);
                }
                if (flags.testFlag(hkbPoseMatchingGenerator::FLAG_IS_PARAMETRIC_BLEND_CYCLIC)){
                    flagIsParametricBlendCyclic->setChecked(true);
                }else{
                    flagIsParametricBlendCyclic->setChecked(false);
                }
                if (flags.testFlag(hkbPoseMatchingGenerator::FLAG_FORCE_DENSE_POSE)){
                    flagForceDensePose->setChecked(true);
                }else{
                    flagForceDensePose->setChecked(false);
                }
                subtractLastChild->setChecked(bsData->subtractLastChild);
                worldFromModelRotation->setValue(bsData->worldFromModelRotation);
                blendSpeed->setValue(bsData->blendSpeed);
                minSpeedToSwitch->setValue(bsData->minSpeedToSwitch);
                minSwitchTimeNoError->setValue(bsData->minSwitchTimeNoError);
                minSwitchTimeFullError->setValue(bsData->minSwitchTimeFullError);
                QString varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->startPlayingEventId);
                loadTableValue(START_PLAYING_EVENT_ID_ROW, varName);
                varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->startMatchingEventId);
                loadTableValue(START_MATCHING_EVENT_ID_ROW, varName);
                QStringList boneNames("None");
                if (rootBoneIndex->count() == 0){
                    boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                    rootBoneIndex->insertItems(0, boneNames);
                }
                rootBoneIndex->setCurrentIndex(bsData->rootBoneIndex + 1);
                if (otherBoneIndex->count() == 0){
                    boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                    otherBoneIndex->insertItems(0, boneNames);
                }
                otherBoneIndex->setCurrentIndex(bsData->otherBoneIndex + 1);
                if (anotherBoneIndex->count() == 0){
                    boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                    anotherBoneIndex->insertItems(0, boneNames);
                }
                anotherBoneIndex->setCurrentIndex(bsData->anotherBoneIndex + 1);
                if (pelvisIndex->count() == 0){
                    boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                    pelvisIndex->insertItems(0, boneNames);
                }
                pelvisIndex->setCurrentIndex(bsData->pelvisIndex + 1);
                if (mode->count() == 0){
                    mode->insertItems(0, bsData->Mode);
                }
                mode->setCurrentIndex(bsData->Mode.indexOf(bsData->mode));
                varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
                if (varBind){
                    loadBinding(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN, varBind, "referencePoseWeightThreshold");
                    loadBinding(BLEND_PARAMETER_ROW, BINDING_COLUMN, varBind, "blendParameter");
                    loadBinding(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN, varBind, "minCyclicBlendParameter");
                    loadBinding(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN, varBind, "maxCyclicBlendParameter");
                    loadBinding(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN, varBind, "indexOfSyncMasterChild");
                    loadBinding(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN, varBind, "subtractLastChild");
                    loadBinding(WORLD_FROM_MODEL_ROTATION_ROW, BINDING_COLUMN, varBind, "worldFromModelRotation");
                    loadBinding(BLEND_SPEED_ROW, BINDING_COLUMN, varBind, "blendSpeed");
                    loadBinding(MIN_SPEED_TO_SWITCH_ROW, BINDING_COLUMN, varBind, "minSpeedToSwitch");
                    loadBinding(MIN_SWITCH_TIME_NO_ERROR_ROW, BINDING_COLUMN, varBind, "minSwitchTimeNoError");
                    loadBinding(MIN_SWITCH_TIME_FULL_ERROR_ROW, BINDING_COLUMN, varBind, "minSwitchTimeFullError");
                    loadBinding(ROOT_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "rootBoneIndex");
                    loadBinding(OTHER_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "otherBoneIndex");
                    loadBinding(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "anotherBoneIndex");
                    loadBinding(PELVIS_INDEX_ROW, BINDING_COLUMN, varBind, "pelvisIndex");
                }else{
                    table->item(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(WORLD_FROM_MODEL_ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(BLEND_SPEED_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(MIN_SPEED_TO_SWITCH_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(MIN_SWITCH_TIME_NO_ERROR_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(MIN_SWITCH_TIME_FULL_ERROR_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(ROOT_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(OTHER_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(PELVIS_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                }
                loadDynamicTableRows();
            }else{
                (qFatal(QString("PoseMatchingGeneratorUI::loadData(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
            }
        }else{
            (qFatal(QString("PoseMatchingGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data()));
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::loadData(): Attempting to load a null pointer!!"));
    }
    connectSignals();
}

void PoseMatchingGeneratorUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_CHILD_ROW + bsData->getNumberOfChildren() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        hkbBlenderGeneratorChild *child = NULL;
        for (int i = ADD_CHILD_ROW + 1, j = 0; j < bsData->getNumberOfChildren(); i++, j++){
            child = static_cast<hkbBlenderGeneratorChild *>(bsData->children.at(j).data());
            if (child){
                setRowItems(i, "Child "+QString::number(j), child->getClassname(), "Remove", "Edit", "Double click to remove this child", "Double click to edit this child");
            }else{
                (qFatal("PoseMatchingGeneratorUI::loadData(): Null child found!!!"));
            }
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::loadDynamicTableRows(): The data is NULL!!"));
    }
    //table->setSortingEnabled(true);
}

void PoseMatchingGeneratorUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
    if (table->item(row, NAME_COLUMN)){
        table->item(row, NAME_COLUMN)->setText(name);
    }else{
        table->setItem(row, NAME_COLUMN, new TableWidgetItem(name));
    }
    if (table->item(row, TYPE_COLUMN)){
        table->item(row, TYPE_COLUMN)->setText(classname);
    }else{
        table->setItem(row, TYPE_COLUMN, new TableWidgetItem(classname, Qt::AlignCenter));
    }
    if (table->item(row, BINDING_COLUMN)){
        table->item(row, BINDING_COLUMN)->setText(bind);
    }else{
        table->setItem(row, BINDING_COLUMN, new TableWidgetItem(bind, Qt::AlignCenter, QColor(Qt::red), QBrush(Qt::black), tip1));
    }
    if (table->item(row, VALUE_COLUMN)){
        table->item(row, VALUE_COLUMN)->setText(value);
    }else{
        table->setItem(row, VALUE_COLUMN, new TableWidgetItem(value, Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), tip2));
    }
}

bool PoseMatchingGeneratorUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    (qFatal("PoseMatchingGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    (qFatal("PoseMatchingGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void PoseMatchingGeneratorUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case REFERENCE_POSE_WEIGHT_THRESHOLD_ROW:
            if (table->item(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "referencePoseWeightThreshold", VARIABLE_TYPE_REAL, isProperty);
            break;
        case BLEND_PARAMETER_ROW:
            if (table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "blendParameter", VARIABLE_TYPE_REAL, isProperty);
            break;
        case MIN_CYCLIC_BLEND_PARAMETER_ROW:
            if (table->item(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "minCyclicBlendParameter", VARIABLE_TYPE_REAL, isProperty);
            break;
        case MAX_CYCLIC_BLEND_PARAMETER_ROW:
            if (table->item(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "maxCyclicBlendParameter", VARIABLE_TYPE_REAL, isProperty);
            break;
        case INDEX_OF_SYNC_MASTER_CHILD_ROW:
            if (table->item(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "indexOfSyncMasterChild", VARIABLE_TYPE_INT32, isProperty);
            break;
        case SUBTRACT_LAST_CHILD_ROW:
            if (table->item(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "subtractLastChild", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case WORLD_FROM_MODEL_ROTATION_ROW:
            if (table->item(WORLD_FROM_MODEL_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "worldFromModelRotation", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case BLEND_SPEED_ROW:
            if (table->item(BLEND_SPEED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "blendSpeed", VARIABLE_TYPE_REAL, isProperty);
            break;
        case MIN_SPEED_TO_SWITCH_ROW:
            if (table->item(MIN_SPEED_TO_SWITCH_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "minSpeedToSwitch", VARIABLE_TYPE_REAL, isProperty);
            break;
        case MIN_SWITCH_TIME_NO_ERROR_ROW:
            if (table->item(MIN_SWITCH_TIME_NO_ERROR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "minSwitchTimeNoError", VARIABLE_TYPE_REAL, isProperty);
            break;
        case MIN_SWITCH_TIME_FULL_ERROR_ROW:
            if (table->item(MIN_SWITCH_TIME_FULL_ERROR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "minSwitchTimeFullError", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ROOT_BONE_INDEX_ROW:
            if (table->item(ROOT_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rootBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case OTHER_BONE_INDEX_ROW:
            if (table->item(OTHER_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "otherBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case ANOTHER_BONE_INDEX_ROW:
            if (table->item(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "anotherBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case PELVIS_INDEX_ROW:
            if (table->item(PELVIS_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "pelvisIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("PoseMatchingGeneratorUI::setBindingVariable(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            emit generatorNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setName(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setReferencePoseWeightThreshold(){
    if (bsData){
        if (bsData->referencePoseWeightThreshold != referencePoseWeightThreshold->value()){
            bsData->referencePoseWeightThreshold = referencePoseWeightThreshold->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setReferencePoseWeightThreshold(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setBlendParameter(){
    if (bsData){
        if (bsData->blendParameter != blendParameter->value()){
            bsData->blendParameter = blendParameter->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setBlendParameter(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setMinCyclicBlendParameter(){
    if (bsData){
        if (bsData->minCyclicBlendParameter != minCyclicBlendParameter->value()){
            bsData->minCyclicBlendParameter = minCyclicBlendParameter->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setMinCyclicBlendParameter(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setMaxCyclicBlendParameter(){
    if (bsData){
        if (bsData->maxCyclicBlendParameter != maxCyclicBlendParameter->value()){
            bsData->maxCyclicBlendParameter = maxCyclicBlendParameter->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setMaxCyclicBlendParameter(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setIndexOfSyncMasterChild(){
    if (bsData){
        if (bsData->indexOfSyncMasterChild != indexOfSyncMasterChild->value()){
            bsData->indexOfSyncMasterChild = indexOfSyncMasterChild->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setIndexOfSyncMasterChild(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setFlagSync(){
    if (bsData){
        bool ok = true;
        hkbPoseMatchingGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagSync->isChecked()){
                flags |= hkbPoseMatchingGenerator::FLAG_SYNC;
            }else{
                flags &= ~(hkbPoseMatchingGenerator::FLAG_SYNC);
            }
            bsData->flags = QString::number(flags);
        }else{
            (qFatal(QString("PoseMatchingGeneratorUI::setFlagSync(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setFlagSync(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setFlagSmoothGeneratorWeights(){
    if (bsData){
        bool ok = true;
        hkbPoseMatchingGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagSmoothGeneratorWeights->isChecked()){
                flags |= hkbPoseMatchingGenerator::FLAG_SMOOTH_GENERATOR_WEIGHTS;
            }else{
                flags &= ~(hkbPoseMatchingGenerator::FLAG_SMOOTH_GENERATOR_WEIGHTS);
            }
            bsData->flags = QString::number(flags);
        }else{
            (qFatal(QString("PoseMatchingGeneratorUI::setFlagSmoothGeneratorWeights(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setFlagSmoothGeneratorWeights(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setFlagDontDeactivateChildrenWithZeroWeights(){
    if (bsData){
        bool ok = true;
        hkbPoseMatchingGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagDontDeactivateChildrenWithZeroWeights->isChecked()){
                flags |= hkbPoseMatchingGenerator::FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS;
            }else{
                flags &= ~(hkbPoseMatchingGenerator::FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS);
            }
            bsData->flags = QString::number(flags);
        }else{
            (qFatal(QString("PoseMatchingGeneratorUI::setFlagDontDeactivateChildrenWithZeroWeights(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setFlagDontDeactivateChildrenWithZeroWeights(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setFlagParametricBlend(){
    if (bsData){
        bool ok = true;
        hkbPoseMatchingGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagParametricBlend->isChecked()){
                flags |= hkbPoseMatchingGenerator::FLAG_PARAMETRIC_BLEND;
            }else{
                flags &= ~(hkbPoseMatchingGenerator::FLAG_PARAMETRIC_BLEND);
            }
            bsData->flags = QString::number(flags);
        }else{
            (qFatal(QString("PoseMatchingGeneratorUI::setFlagParametricBlend(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setFlagParametricBlend(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setFlagIsParametricBlendCyclic(){
    if (bsData){
        bool ok = true;
        hkbPoseMatchingGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagIsParametricBlendCyclic->isChecked()){
                flags |= hkbPoseMatchingGenerator::FLAG_IS_PARAMETRIC_BLEND_CYCLIC;
            }else{
                flags &= ~(hkbPoseMatchingGenerator::FLAG_IS_PARAMETRIC_BLEND_CYCLIC);
            }
            bsData->flags = QString::number(flags);
        }else{
            (qFatal(QString("PoseMatchingGeneratorUI::setFlagIsParametricBlendCyclic(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setFlagIsParametricBlendCyclic(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setFlagForceDensePose(){
    if (bsData){
        bool ok = true;
        hkbPoseMatchingGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagForceDensePose->isChecked()){
                flags |= hkbPoseMatchingGenerator::FLAG_FORCE_DENSE_POSE;
            }else{
                flags &= ~(hkbPoseMatchingGenerator::FLAG_FORCE_DENSE_POSE);
            }
            bsData->flags = QString::number(flags);
        }else{
            (qFatal(QString("PoseMatchingGeneratorUI::setFlagForceDensePose(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setFlagForceDensePose(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setSubtractLastChild(){
    if (bsData){
        bsData->subtractLastChild = subtractLastChild->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("PoseMatchingGeneratorUI::setSubtractLastChild(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setWorldFromModelRotation(){
    if (bsData){
        if (bsData->worldFromModelRotation != worldFromModelRotation->value()){
            bsData->worldFromModelRotation = worldFromModelRotation->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setWorldFromModelRotation(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setBlendSpeed(){
    if (bsData){
        if (bsData->blendSpeed != blendSpeed->value()){
            bsData->blendSpeed = blendSpeed->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setBlendSpeed(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setMinSpeedToSwitch(){
    if (bsData){
        if (bsData->minSpeedToSwitch != minSpeedToSwitch->value()){
            bsData->minSpeedToSwitch = minSpeedToSwitch->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setMinSpeedToSwitch(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setMinSwitchTimeNoError(){
    if (bsData){
        if (bsData->minSwitchTimeNoError != minSwitchTimeNoError->value()){
            bsData->minSwitchTimeNoError = minSwitchTimeNoError->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setMinSwitchTimeNoError(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setMinSwitchTimeFullError(){
    if (bsData){
        if (bsData->minSwitchTimeFullError != minSwitchTimeFullError->value()){
            bsData->minSwitchTimeFullError = minSwitchTimeFullError->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::setMinSwitchTimeFullError(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setStartPlayingEventId(int index, const QString &name){
    if (bsData){
        bsData->startPlayingEventId = index - 1;
        table->item(START_PLAYING_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("PoseMatchingGeneratorUI::setStartPlayingEventId(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setStartMatchingEventId(int index, const QString &name){
    if (bsData){
        bsData->startMatchingEventId = index - 1;
        table->item(START_MATCHING_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("PoseMatchingGeneratorUI::setStartMatchingEventId(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setRootBoneIndex(int index){
    if (bsData){
        bsData->rootBoneIndex = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("PoseMatchingGeneratorUI::setRootBoneIndex(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setOtherBoneIndex(int index){
    if (bsData){
        bsData->otherBoneIndex = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("PoseMatchingGeneratorUI::setOtherBoneIndex(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setAnotherBoneIndex(int index){
    if (bsData){
        bsData->anotherBoneIndex = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("PoseMatchingGeneratorUI::setAnotherBoneIndex(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setPelvisIndex(int index){
    if (bsData){
        bsData->pelvisIndex = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("PoseMatchingGeneratorUI::setPelvisIndex(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::setMode(int index){
    if (bsData){
        bsData->mode = bsData->Mode.at(index);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("PoseMatchingGeneratorUI::setMode(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::swapGeneratorIndices(int index1, int index2){
    if (bsData){
        index1 = index1 - BASE_NUMBER_OF_ROWS;
        index2 = index2 - BASE_NUMBER_OF_ROWS;
        if (bsData->children.size() > index1 && bsData->children.size() > index2 && index1 != index2 && index1 >= 0 && index2 >= 0){
            bsData->children.swap(index1, index2);
            if (behaviorView->getSelectedItem()){
                behaviorView->getSelectedItem()->reorderChildren();
            }else{
                (qFatal("PoseMatchingGeneratorUI::swapGeneratorIndices(): No item selected!!"));
            }
            bsData->getParentFile()->setIsChanged(true);
        }else{
            (qWarning("PoseMatchingGeneratorUI::swapGeneratorIndices(): Cannot swap these rows!!"));
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::swapGeneratorIndices(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::addChildWithGenerator(){
    Generator_Type typeEnum;
    if (bsData && behaviorView){
        typeEnum = static_cast<Generator_Type>(typeSelectorCB->currentIndex());
        switch (typeEnum){
        case STATE_MACHINE:
            behaviorView->appendStateMachine();
            break;
        case MANUAL_SELECTOR_GENERATOR:
            behaviorView->appendManualSelectorGenerator();
            break;
        case BLENDER_GENERATOR:
            behaviorView->appendBlenderGenerator();
            break;
        case I_STATE_TAGGING_GENERATOR:
            behaviorView->appendIStateTaggingGenerator();
            break;
        case BONE_SWITCH_GENERATOR:
            behaviorView->appendBoneSwitchGenerator();
            break;
        case CYCLIC_BLEND_TRANSITION_GENERATOR:
            behaviorView->appendCyclicBlendTransitionGenerator();
            break;
        case SYNCHRONIZED_CLIP_GENERATOR:
            behaviorView->appendSynchronizedClipGenerator();
            break;
        case MODIFIER_GENERATOR:
            behaviorView->appendModifierGenerator();
            break;
        case OFFSET_ANIMATION_GENERATOR:
            behaviorView->appendOffsetAnimationGenerator();
            break;
        case POSE_MATCHING_GENERATOR:
            behaviorView->appendPoseMatchingGenerator();
            break;
        case CLIP_GENERATOR:
            behaviorView->appendClipGenerator();
            break;
        case BEHAVIOR_REFERENCE_GENERATOR:
            behaviorView->appendBehaviorReferenceGenerator();
            break;
        case GAMEBYRO_SEQUENCE_GENERATOR:
            behaviorView->appendBGSGamebryoSequenceGenerator();
            break;
        default:
            (qFatal("PoseMatchingGeneratorUI::addChild(): Invalid typeEnum!!"));
            return;
        }
        loadDynamicTableRows();
    }else{
        (qFatal("PoseMatchingGeneratorUI::addChild(): The data or behavior graph pointer is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::removeChild(int index){
    hkbBlenderGeneratorChild *child = NULL;
    if (bsData && behaviorView){
        if (index < bsData->children.size() && index >= 0){
            child = static_cast<hkbBlenderGeneratorChild *>(bsData->children.at(index).data());
            behaviorView->removeItemFromGraph(behaviorView->getSelectedIconsChildIcon(child->generator.data()), index);//Reorderchildren?
            behaviorView->removeObjects();
        }else{
            (qWarning("PoseMatchingGeneratorUI::removeChild(): Invalid index of child to remove!!"));
        }
        loadDynamicTableRows();
    }else{
        (qFatal("PoseMatchingGeneratorUI::removeChild(): The data or behavior graph pointer is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::viewSelectedChild(int row, int column){
    int result = -1;
    bool properties = false;
    if (bsData){
        if (row < ADD_CHILD_ROW && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case REFERENCE_POSE_WEIGHT_THRESHOLD_ROW:
                    if (table->item(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "referencePoseWeightThreshold");
                    break;
                case BLEND_PARAMETER_ROW:
                    if (table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "blendParameter");
                    break;
                case MIN_CYCLIC_BLEND_PARAMETER_ROW:
                    if (table->item(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "minCyclicBlendParameter");
                case MAX_CYCLIC_BLEND_PARAMETER_ROW:
                    if (table->item(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "maxCyclicBlendParameter");
                    break;
                case INDEX_OF_SYNC_MASTER_CHILD_ROW:
                    if (table->item(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "indexOfSyncMasterChild");
                    break;
                case SUBTRACT_LAST_CHILD_ROW:
                    if (table->item(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "subtractLastChild");
                    break;

                case WORLD_FROM_MODEL_ROTATION_ROW:
                    if (table->item(WORLD_FROM_MODEL_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "worldFromModelRotation");
                    break;
                case BLEND_SPEED_ROW:
                    if (table->item(BLEND_SPEED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "blendSpeed");
                    break;
                case MIN_SPEED_TO_SWITCH_ROW:
                    if (table->item(MIN_SPEED_TO_SWITCH_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "minSpeedToSwitch");
                    break;
                case MIN_SWITCH_TIME_NO_ERROR_ROW:
                    if (table->item(MIN_SWITCH_TIME_NO_ERROR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "minSwitchTimeNoError");
                    break;
                case MIN_SWITCH_TIME_FULL_ERROR_ROW:
                    if (table->item(MIN_SWITCH_TIME_FULL_ERROR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "minSwitchTimeFullError");
                    break;
                case ROOT_BONE_INDEX_ROW:
                    if (table->item(ROOT_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "rootBoneIndex");
                    break;
                case OTHER_BONE_INDEX_ROW:
                    if (table->item(OTHER_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "otherBoneIndex");
                    break;
                case ANOTHER_BONE_INDEX_ROW:
                    if (table->item(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "anotherBoneIndex");
                    break;
                case PELVIS_INDEX_ROW:
                    if (table->item(PELVIS_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "pelvisIndex");
                    break;
                }
            }else if (column == VALUE_COLUMN){
                if (row == START_PLAYING_EVENT_ID_ROW){
                    emit viewEvents(bsData->startPlayingEventId + 1);
                }else if (row == START_MATCHING_EVENT_ID_ROW){
                    emit viewEvents(bsData->startMatchingEventId + 1);
                }
            }
        }else if (row == ADD_CHILD_ROW && column == NAME_COLUMN){
            addChildWithGenerator();
        }else if (row > ADD_CHILD_ROW && row < ADD_CHILD_ROW + bsData->getNumberOfChildren() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->children.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    childUI->loadData(static_cast<hkbBlenderGeneratorChild *>(bsData->children.at(result).data()), result);
                    setCurrentIndex(CHILD_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the child \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeChild(result);
                    }
                }
            }else{
                (qFatal("PoseMatchingGeneratorUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::returnToWidget(bool reloadData){
    if (reloadData){
        loadDynamicTableRows();
    }
    setCurrentIndex(MAIN_WIDGET);
}

void PoseMatchingGeneratorUI::eventRenamed(const QString & name, int index){
    if (bsData){
        if (index == bsData->startPlayingEventId){
            table->item(START_PLAYING_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->startMatchingEventId){
            table->item(START_MATCHING_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::eventRenamed(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::eventTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        switch (table->currentRow()){
        case START_PLAYING_EVENT_ID_ROW:
            setStartPlayingEventId(index, name);
            break;
        case START_MATCHING_EVENT_ID_ROW:
            setStartMatchingEventId(index, name);
            break;
        default:
            (qWarning("PoseMatchingGeneratorUI::eventTableElementSelected(): An unwanted element selected event was recieved!!"));
            return;
        }
        break;
    default:
        (qWarning("PoseMatchingGeneratorUI::eventTableElementSelected(): An unwanted element selected event was recieved!!"));
    }
}

void PoseMatchingGeneratorUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    case CHILD_WIDGET:
        childUI->setBindingVariable(index, name);
        break;
    default:
        (qWarning("PoseMatchingGeneratorUI::variableTableElementSelected(): An unwanted element selected event was recieved!!"));
    }
}

void PoseMatchingGeneratorUI::generatorTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case CHILD_WIDGET:
        childUI->setGenerator(index, name);
        break;
    default:
        (qWarning("PoseMatchingGeneratorUI::generatorTableElementSelected(): An unwanted element selected event was recieved!!"));
    }
}

void PoseMatchingGeneratorUI::connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (generators && variables && events && properties){
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(eventTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(generatorTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int)), generators, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        (qFatal("PoseMatchingGeneratorUI::connectToTables(): One or more arguments are NULL!!"));
    }
}

void PoseMatchingGeneratorUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    table->item(row, colunm)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            (qFatal("PoseMatchingGeneratorUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::loadBinding(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (bsData->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewProperties(0);
            }
        }else{
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        (qFatal("PoseMatchingGeneratorUI::selectTableToView(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = NULL;
    if (name == ""){
        (qWarning("PoseMatchingGeneratorUI::variableRenamed(): The new variable name is the empty string!!"));
    }
    if (bsData){
        index--;
        bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("referencePoseWeightThreshold");
            if (bindIndex == index){
                table->item(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("blendParameter");
            if (bindIndex == index){
                table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("minCyclicBlendParameter");
            if (bindIndex == index){
                table->item(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("maxCyclicBlendParameter");
            if (bindIndex == index){
                table->item(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("indexOfSyncMasterChild");
            if (bindIndex == index){
                table->item(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("subtractLastChild");
            if (bindIndex == index){
                table->item(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN)->setText(name);
            }

            bindIndex = bind->getVariableIndexOfBinding("worldFromModelRotation");
            if (bindIndex == index){
                table->item(WORLD_FROM_MODEL_ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("blendSpeed");
            if (bindIndex == index){
                table->item(BLEND_SPEED_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("minSpeedToSwitch");
            if (bindIndex == index){
                table->item(MIN_SPEED_TO_SWITCH_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("minSwitchTimeNoError");
            if (bindIndex == index){
                table->item(MIN_SWITCH_TIME_NO_ERROR_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("minSwitchTimeFullError");
            if (bindIndex == index){
                table->item(MIN_SWITCH_TIME_FULL_ERROR_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("rootBoneIndex");
            if (bindIndex == index){
                table->item(ROOT_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("otherBoneIndex");
            if (bindIndex == index){
                table->item(OTHER_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("anotherBoneIndex");
            if (bindIndex == index){
                table->item(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("pelvisIndex");
            if (bindIndex == index){
                table->item(PELVIS_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
        }
        childUI->variableRenamed(name, index);
    }else{
        (qFatal("PoseMatchingGeneratorUI::variableRenamed(): The data is NULL!!"));
    }
}

void PoseMatchingGeneratorUI::generatorRenamed(const QString &name, int index){
    if (name == ""){
        (qWarning("PoseMatchingGeneratorUI::generatorRenamed(): The new variable name is the empty string!!"));
    }
    if (currentIndex() == CHILD_WIDGET){
        childUI->generatorRenamed(name, index);
    }
}

void PoseMatchingGeneratorUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
    setCurrentIndex(MAIN_WIDGET);
    childUI->setBehaviorView(view);
}

