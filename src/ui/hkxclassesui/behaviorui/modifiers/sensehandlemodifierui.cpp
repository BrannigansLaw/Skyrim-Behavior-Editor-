#include "sensehandlemodifierui.h"

#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbsensehandlemodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/hkxclassesui/behaviorui/rangesui.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include <QStackedLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 18

#define NAME_ROW 0
#define ENABLE_ROW 1
#define SENSOR_LOCAL_OFFSET_ROW 2
#define HANDLE_OUT_ROW 3
#define HANDLE_IN_ROW 4
#define LOCAL_FRAME_NAME_ROW 5
#define SENSOR_LOCAL_FRAME_NAME_ROW 6
#define MIN_DISTANCE_ROW 7
#define MAX_DISTANCE_ROW 8
#define DISTANCE_OUT_ROW 9
#define COLLISION_FILTER_INFO_ROW 10
#define SENSOR_RAGDOLL_BONE_INDEX_ROW 11
#define SENSOR_ANIMATION_BONE_INDEX_ROW 12
#define SENSING_MODE_ROW 13
#define EXTRAPOLATE_SENSOR_POSITION_ROW 14
#define KEEP_FIRST_SENSED_HANDLE_ROW 15
#define FOUND_HANDLE_OUT_ROW 16
#define ADD_RANGE_ROW 17

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList SenseHandleModifierUI::collisionLayers = {
    "NONE",
    "L_UNIDENTIFIED",
    "L_STATIC",
    "L_ANIMSTATIC",
    "L_TRANSPARENT",
    "L_CLUTTER",
    "L_WEAPON",
    "L_PROJECTILE",
    "L_SPELL",
    "L_BIPED",
    "L_TREES",
    "L_PROPS",
    "L_WATER",
    "L_TRIGGER",
    "L_TERRAIN",
    "L_TRAP",
    "L_NONCOLLIDABLE",
    "L_CLOUDTRAP",
    "L_GROUND",
    "L_PORTAL",
    "L_DEBRIS_SMALL",
    "L_DEBRIS_LARGE",
    "L_ACOUSTIC_SPACE",
    "L_ACTOR_ZONE",
    "L_PROJECTILEZONE",
    "L_GASTRAP",
    "L_SHELLCASING",
    "L_TRANSPARENT_SMALL",
    "L_INVISABLE_WALL",
    "L_TRANSPARENT_SMALL_ANIM",
    "L_WARD",
    "L_CHARCONTROLLER",
    "L_STAIRHELPER",
    "L_DEADBIP",
    "L_BIPED_NO_CC",
    "L_AVOIDBOX",
    "L_COLLISIONBOX",
    "L_CAMERASPHERE",
    "L_DOORDETECTION",
    "L_CONEPROJECTILE",
    "L_CAMERA",
    "L_ITEMPICKER",
    "L_LOS",
    "L_PATHINGPICK",
    "L_CUSTOMPICK1",
    "L_CUSTOMPICK2",
    "L_SPELLEXPLOSION",
    "L_DROPPINGPICK",
    "L_DEADACTORZONE",
    "L_CRITTER",
    "L_TRIGGERFALLINGTRAP",
    "L_NAVCUT",
    "L_SPELLTRIGGER",
    "L_LIVING_AND_DEAD_ACTORS",
    "L_DETECTION",
    "L_TRAP_TRIGGER"
};

QStringList SenseHandleModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

SenseHandleModifierUI::SenseHandleModifierUI()
    : bsData(NULL),
      rangeUI(new RangesUI),
      groupBox(new QGroupBox),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      sensorLocalOffset(new QuadVariableWidget),
      localFrameName(new ComboBox),
      sensorLocalFrameName(new ComboBox),
      minDistance(new DoubleSpinBox),
      maxDistance(new DoubleSpinBox),
      distanceOut(new DoubleSpinBox),
      collisionFilterInfo(new ComboBox),
      sensorRagdollBoneIndex(new ComboBox),
      sensorAnimationBoneIndex(new ComboBox),
      sensingMode(new ComboBox),
      extrapolateSensorPosition(new CheckBox),
      keepFirstSensedHandle(new CheckBox),
      foundHandleOut(new CheckBox)
{
    collisionFilterInfo->addItems(collisionLayers);
    groupBox->setTitle("hkbSenseHandleModifier");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(ENABLE_ROW, NAME_COLUMN, new TableWidgetItem("enable"));
    table->setItem(ENABLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(SENSOR_LOCAL_OFFSET_ROW, NAME_COLUMN, new TableWidgetItem("sensorLocalOffset"));
    table->setItem(SENSOR_LOCAL_OFFSET_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(SENSOR_LOCAL_OFFSET_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SENSOR_LOCAL_OFFSET_ROW, VALUE_COLUMN, sensorLocalOffset);
    table->setItem(HANDLE_OUT_ROW, NAME_COLUMN, new TableWidgetItem("handleOut"));
    table->setItem(HANDLE_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkHandle", Qt::AlignCenter));
    table->setItem(HANDLE_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(HANDLE_OUT_ROW, VALUE_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(HANDLE_IN_ROW, NAME_COLUMN, new TableWidgetItem("handleIn"));
    table->setItem(HANDLE_IN_ROW, TYPE_COLUMN, new TableWidgetItem("hkHandle", Qt::AlignCenter));
    table->setItem(HANDLE_IN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(HANDLE_IN_ROW, VALUE_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(LOCAL_FRAME_NAME_ROW, NAME_COLUMN, new TableWidgetItem("localFrameName"));
    table->setItem(LOCAL_FRAME_NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(LOCAL_FRAME_NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(LOCAL_FRAME_NAME_ROW, VALUE_COLUMN, localFrameName);
    table->setItem(SENSOR_LOCAL_FRAME_NAME_ROW, NAME_COLUMN, new TableWidgetItem("sensorLocalFrameName"));
    table->setItem(SENSOR_LOCAL_FRAME_NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(SENSOR_LOCAL_FRAME_NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(SENSOR_LOCAL_FRAME_NAME_ROW, VALUE_COLUMN, sensorLocalFrameName);
    table->setItem(MIN_DISTANCE_ROW, NAME_COLUMN, new TableWidgetItem("minDistance"));
    table->setItem(MIN_DISTANCE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MIN_DISTANCE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MIN_DISTANCE_ROW, VALUE_COLUMN, minDistance);
    table->setItem(MAX_DISTANCE_ROW, NAME_COLUMN, new TableWidgetItem("maxDistance"));
    table->setItem(MAX_DISTANCE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MAX_DISTANCE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MAX_DISTANCE_ROW, VALUE_COLUMN, maxDistance);
    table->setItem(DISTANCE_OUT_ROW, NAME_COLUMN, new TableWidgetItem("distanceOut"));
    table->setItem(DISTANCE_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DISTANCE_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DISTANCE_OUT_ROW, VALUE_COLUMN, distanceOut);
    table->setItem(COLLISION_FILTER_INFO_ROW, NAME_COLUMN, new TableWidgetItem("collisionFilterInfo"));
    table->setItem(COLLISION_FILTER_INFO_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(COLLISION_FILTER_INFO_ROW, VALUE_COLUMN, collisionFilterInfo);
    table->setItem(SENSOR_RAGDOLL_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("sensorRagdollBoneIndex"));
    table->setItem(SENSOR_RAGDOLL_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(SENSOR_RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SENSOR_RAGDOLL_BONE_INDEX_ROW, VALUE_COLUMN, sensorRagdollBoneIndex);
    table->setItem(SENSOR_ANIMATION_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("sensorAnimationBoneIndex"));
    table->setItem(SENSOR_ANIMATION_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(SENSOR_ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SENSOR_ANIMATION_BONE_INDEX_ROW, VALUE_COLUMN, sensorAnimationBoneIndex);
    table->setItem(SENSING_MODE_ROW, NAME_COLUMN, new TableWidgetItem("sensingMode"));
    table->setItem(SENSING_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("SensingMode", Qt::AlignCenter));
    table->setItem(SENSING_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(SENSING_MODE_ROW, VALUE_COLUMN, sensingMode);
    table->setItem(EXTRAPOLATE_SENSOR_POSITION_ROW, NAME_COLUMN, new TableWidgetItem("extrapolateSensorPosition"));
    table->setItem(EXTRAPOLATE_SENSOR_POSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(EXTRAPOLATE_SENSOR_POSITION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(EXTRAPOLATE_SENSOR_POSITION_ROW, VALUE_COLUMN, extrapolateSensorPosition);
    table->setItem(KEEP_FIRST_SENSED_HANDLE_ROW, NAME_COLUMN, new TableWidgetItem("keepFirstSensedHandle"));
    table->setItem(KEEP_FIRST_SENSED_HANDLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(KEEP_FIRST_SENSED_HANDLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(KEEP_FIRST_SENSED_HANDLE_ROW, VALUE_COLUMN, keepFirstSensedHandle);
    table->setItem(FOUND_HANDLE_OUT_ROW, NAME_COLUMN, new TableWidgetItem("foundHandleOut"));
    table->setItem(FOUND_HANDLE_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(FOUND_HANDLE_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(FOUND_HANDLE_OUT_ROW, VALUE_COLUMN, foundHandleOut);
    table->setItem(ADD_RANGE_ROW, NAME_COLUMN, new TableWidgetItem("Add Range", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new range"));
    table->setItem(ADD_RANGE_ROW, TYPE_COLUMN, new TableWidgetItem("hkRange", Qt::AlignCenter));
    table->setItem(ADD_RANGE_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Range", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected range"));
    table->setItem(ADD_RANGE_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Range", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected range"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(rangeUI);
    connectSignals();
}

void SenseHandleModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(sensorLocalOffset, SIGNAL(editingFinished()), this, SLOT(setSensorLocalOffset()), Qt::UniqueConnection);
    connect(localFrameName, SIGNAL(currentTextChanged(QString)), this, SLOT(setLocalFrameName(QString)), Qt::UniqueConnection);
    connect(sensorLocalFrameName, SIGNAL(currentTextChanged(QString)), this, SLOT(setSensorLocalFrameName(QString)), Qt::UniqueConnection);
    connect(minDistance, SIGNAL(editingFinished()), this, SLOT(setMinDistance()), Qt::UniqueConnection);
    connect(maxDistance, SIGNAL(editingFinished()), this, SLOT(setMaxDistance()), Qt::UniqueConnection);
    connect(distanceOut, SIGNAL(editingFinished()), this, SLOT(setDistanceOut()), Qt::UniqueConnection);
    connect(collisionFilterInfo, SIGNAL(currentIndexChanged(int)), this, SLOT(setCollisionFilterInfo(int)), Qt::UniqueConnection);
    connect(sensorRagdollBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setSensorRagdollBoneIndex(int)), Qt::UniqueConnection);
    connect(sensorAnimationBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setSensorAnimationBoneIndex(int)), Qt::UniqueConnection);
    connect(sensingMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setSensingMode(int)), Qt::UniqueConnection);
    connect(extrapolateSensorPosition, SIGNAL(released()), this, SLOT(setExtrapolateSensorPosition()), Qt::UniqueConnection);
    connect(keepFirstSensedHandle, SIGNAL(released()), this, SLOT(setKeepFirstSensedHandle()), Qt::UniqueConnection);
    connect(foundHandleOut, SIGNAL(released()), this, SLOT(setFoundHandleOut()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(rangeUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(rangeUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(rangeUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
}

void SenseHandleModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(sensorLocalOffset, SIGNAL(editingFinished()), this, SLOT(setSensorLocalOffset()));
    disconnect(localFrameName, SIGNAL(currentTextChanged(QString)), this, SLOT(setLocalFrameName(QString)));
    disconnect(sensorLocalFrameName, SIGNAL(currentTextChanged(QString)), this, SLOT(setSensorLocalFrameName(QString)));
    disconnect(minDistance, SIGNAL(editingFinished()), this, SLOT(setMinDistance()));
    disconnect(maxDistance, SIGNAL(editingFinished()), this, SLOT(setMaxDistance()));
    disconnect(distanceOut, SIGNAL(editingFinished()), this, SLOT(setDistanceOut()));
    disconnect(collisionFilterInfo, SIGNAL(currentIndexChanged(int)), this, SLOT(setCollisionFilterInfo(int)));
    disconnect(sensorRagdollBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setSensorRagdollBoneIndex(int)));
    disconnect(sensorAnimationBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setSensorAnimationBoneIndex(int)));
    disconnect(sensingMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setSensingMode(int)));
    disconnect(extrapolateSensorPosition, SIGNAL(released()), this, SLOT(setExtrapolateSensorPosition()));
    disconnect(keepFirstSensedHandle, SIGNAL(released()), this, SLOT(setKeepFirstSensedHandle()));
    disconnect(foundHandleOut, SIGNAL(released()), this, SLOT(setFoundHandleOut()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(rangeUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
    disconnect(rangeUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)));
    disconnect(rangeUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
}

void SenseHandleModifierUI::addRange(){
    if (bsData){
        bsData->ranges.append(hkbSenseHandleModifier::hkRanges());
        bsData->getParentFile()->toggleChanged(true);
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::addRange(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::removeRange(int index){
    if (bsData){
        if (!bsData->ranges.isEmpty()){
            if (index < bsData->ranges.size() && index >= 0){
                bsData->ranges.removeAt(index);
            }else{
                WARNING_MESSAGE(QString("SenseHandleModifierUI::removeExpression(): Invalid row index selected!!"));
                return;
            }
            bsData->getParentFile()->toggleChanged(true);
            loadDynamicTableRows();
        }else{
            WARNING_MESSAGE(QString("SenseHandleModifierUI::removeExpression(): Ranges is empty!!"));
            return;
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::removeExpression(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    if (data){
        if (data->getSignature() == HKB_SENSE_HANDLE_MODIFIER){
            int index = -1;
            QStringList localFrames;
            localFrames.append("None");
            QStringList boneNames;
            boneNames.append("None");
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<hkbSenseHandleModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            sensorLocalOffset->setValue(bsData->sensorLocalOffset);
            if (localFrameName->count() == 0){
                localFrames = localFrames + static_cast<BehaviorFile *>(bsData->getParentFile())->getLocalFrameNames();
                localFrameName->insertItems(0, localFrames);
            }
            index = localFrameName->findText(bsData->localFrameName);
            if (index < 0){
                localFrameName->setCurrentIndex(0);
            }else{
                localFrameName->setCurrentIndex(localFrameName->findText(bsData->localFrameName));
            }
            if (sensorLocalFrameName->count() == 0){
                sensorLocalFrameName->insertItems(0, localFrames);
            }
            index = sensorLocalFrameName->findText(bsData->sensorLocalFrameName);
            if (index < 0){
                sensorLocalFrameName->setCurrentIndex(0);
            }else{
                sensorLocalFrameName->setCurrentIndex(sensorLocalFrameName->findText(bsData->sensorLocalFrameName));
            }
            minDistance->setValue(bsData->minDistance);
            maxDistance->setValue(bsData->maxDistance);
            distanceOut->setValue(bsData->distanceOut);
            if (bsData->collisionFilterInfo < collisionLayers.size() && bsData->collisionFilterInfo > -1){
                index = collisionFilterInfo->findText(collisionLayers.at(bsData->collisionFilterInfo));
                if (index < 0){
                    CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::loadData(): Invalid collisionFilterInfo!!!"));
                }
                collisionFilterInfo->setCurrentIndex(index);
            }else{
                CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::loadData(): Invalid collisionFilterInfo!!!"));
            }
            if (sensorRagdollBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRagdollBoneNames();
                sensorRagdollBoneIndex->insertItems(0, boneNames);
            }
            sensorRagdollBoneIndex->setCurrentIndex(bsData->sensorRagdollBoneIndex + 1);
            boneNames.clear();
            boneNames.append("None");
            if (sensorAnimationBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                sensorAnimationBoneIndex->insertItems(0, boneNames);
            }
            sensorAnimationBoneIndex->setCurrentIndex(bsData->sensorAnimationBoneIndex + 1);
            if (sensingMode->count() == 0){
                sensingMode->insertItems(0, bsData->SensingMode);
            }
            sensingMode->setCurrentIndex(bsData->SensingMode.indexOf(bsData->sensingMode));
            extrapolateSensorPosition->setChecked(bsData->extrapolateSensorPosition);
            keepFirstSensedHandle->setChecked(bsData->keepFirstSensedHandle);
            foundHandleOut->setChecked(bsData->foundHandleOut);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(SENSOR_LOCAL_OFFSET_ROW, BINDING_COLUMN, varBind, "sensorLocalOffset");
                loadBinding(HANDLE_OUT_ROW, BINDING_COLUMN, varBind, "handleOut");
                loadBinding(HANDLE_IN_ROW, BINDING_COLUMN, varBind, "handleIn");
                loadBinding(MIN_DISTANCE_ROW, BINDING_COLUMN, varBind, "minDistance");
                loadBinding(MAX_DISTANCE_ROW, BINDING_COLUMN, varBind, "maxDistance");
                loadBinding(DISTANCE_OUT_ROW, BINDING_COLUMN, varBind, "distanceOut");
                loadBinding(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN, varBind, "collisionFilterInfo");
                loadBinding(SENSOR_RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "sensorRagdollBoneIndex");
                loadBinding(SENSOR_ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "sensorAnimationBoneIndex");
                loadBinding(EXTRAPOLATE_SENSOR_POSITION_ROW, BINDING_COLUMN, varBind, "extrapolateSensorPosition");
                loadBinding(KEEP_FIRST_SENSED_HANDLE_ROW, BINDING_COLUMN, varBind, "keepFirstSensedHandle");
                loadBinding(FOUND_HANDLE_OUT_ROW, BINDING_COLUMN, varBind, "foundHandleOut");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SENSOR_LOCAL_OFFSET_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(HANDLE_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(HANDLE_IN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(MIN_DISTANCE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(MAX_DISTANCE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DISTANCE_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SENSOR_RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SENSOR_ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(EXTRAPOLATE_SENSOR_POSITION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(KEEP_FIRST_SENSED_HANDLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(FOUND_HANDLE_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
            loadDynamicTableRows();
        }else{
            CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::loadData(): Attempting to load a null pointer!!"))
    }
    connectSignals();
}

void SenseHandleModifierUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_RANGE_ROW + bsData->getNumberOfRanges() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        for (int i = ADD_RANGE_ROW + 1, j = 0; j < bsData->getNumberOfRanges(); i++, j++){
            setRowItems(i, "Range "+QString::number(j), "hkRange", "Remove", "Edit", "Double click to remove this range", "Double click to edit this range");
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::loadDynamicTableRows(): The data is NULL!!"));
    }
    //table->setSortingEnabled(true);
}

void SenseHandleModifierUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
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

bool SenseHandleModifierUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void SenseHandleModifierUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case ENABLE_ROW:
            if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "enable", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case SENSOR_LOCAL_OFFSET_ROW:
            if (table->item(SENSOR_LOCAL_OFFSET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "sensorLocalOffset", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case HANDLE_OUT_ROW:
            if (table->item(HANDLE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "handleOut", VARIABLE_TYPE_POINTER, isProperty);
            break;
        case HANDLE_IN_ROW:
            if (table->item(HANDLE_IN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "handleIn", VARIABLE_TYPE_POINTER, isProperty);
            break;
        case MIN_DISTANCE_ROW:
            if (table->item(MIN_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "minDistance", VARIABLE_TYPE_REAL, isProperty);
            break;
        case MAX_DISTANCE_ROW:
            if (table->item(MAX_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "maxDistance", VARIABLE_TYPE_REAL, isProperty);
            break;
        case DISTANCE_OUT_ROW:
            if (table->item(DISTANCE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "distanceOut", VARIABLE_TYPE_REAL, isProperty);
            break;
        case COLLISION_FILTER_INFO_ROW:
            if (table->item(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "collisionFilterInfo", VARIABLE_TYPE_INT32, isProperty);
            break;
        case SENSOR_RAGDOLL_BONE_INDEX_ROW:
            if (table->item(SENSOR_RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "sensorRagdollBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case SENSOR_ANIMATION_BONE_INDEX_ROW:
            if (table->item(SENSOR_ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "sensorAnimationBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case EXTRAPOLATE_SENSOR_POSITION_ROW:
            if (table->item(EXTRAPOLATE_SENSOR_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "extrapolateSensorPosition", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case KEEP_FIRST_SENSED_HANDLE_ROW:
            if (table->item(KEEP_FIRST_SENSED_HANDLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "keepFirstSensedHandle", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case FOUND_HANDLE_OUT_ROW:
            if (table->item(FOUND_HANDLE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "foundHandleOut", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setBindingVariable(): The data is NULL!!"))
    }
}

void SenseHandleModifierUI::returnToWidget(){
    loadDynamicTableRows();
    setCurrentIndex(MAIN_WIDGET);
}

void SenseHandleModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            emit modifierNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setName(): The data is NULL!!"))
    }
}

void SenseHandleModifierUI::setEnable(){
    if (bsData){
        if (bsData->enable != enable->isChecked()){
            bsData->enable = enable->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setEnable(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setSensorLocalOffset(){
    if (bsData){
        if (bsData->sensorLocalOffset != sensorLocalOffset->value()){
            bsData->sensorLocalOffset = sensorLocalOffset->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setSensorLocalOffset(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setMinDistance(){
    if (bsData){
        if (bsData->minDistance != minDistance->value()){
            bsData->minDistance = minDistance->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setMinDistance(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setMaxDistance(){
    if (bsData){
        if (bsData->maxDistance != maxDistance->value()){
            bsData->maxDistance = maxDistance->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setMaxDistance(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setDistanceOut(){
    if (bsData){
        if (bsData->distanceOut != distanceOut->value()){
            bsData->distanceOut = distanceOut->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setDistanceOut(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setLocalFrameName(const QString & text){
    if (bsData){
        bsData->localFrameName = text;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setLocalFrameName(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setSensorLocalFrameName(const QString & text){
    if (bsData){
        bsData->sensorLocalFrameName = text;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setSensorLocalFrameName(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setCollisionFilterInfo(int index){
    if (bsData){
        bsData->collisionFilterInfo = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setCollisionFilterInfo(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setSensorRagdollBoneIndex(int index){
    if (bsData){
        bsData->sensorRagdollBoneIndex = index - 1;
        if (bsData->sensorRagdollBoneIndex > -1){   //sensorRagdollBoneIndex and sensorAnimationBoneIndex cannot simultaneously have nonnegative values!!!
            setSensorAnimationBoneIndex(-1);
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setSensorRagdollBoneIndex(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setSensorAnimationBoneIndex(int index){
    if (bsData){
        bsData->sensorAnimationBoneIndex = index - 1;
        if (bsData->sensorAnimationBoneIndex > -1){   //sensorRagdollBoneIndex and sensorAnimationBoneIndex cannot simultaneously have nonnegative values!!!
            setSensorRagdollBoneIndex(-1);
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setSensorAnimationBoneIndex(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setSensingMode(int index){
    if (bsData){
        bsData->sensingMode = bsData->SensingMode.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setSensingMode(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setExtrapolateSensorPosition(){
    if (bsData){
        if (bsData->extrapolateSensorPosition != extrapolateSensorPosition->isChecked()){
            bsData->extrapolateSensorPosition = extrapolateSensorPosition->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setExtrapolateSensorPosition(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setKeepFirstSensedHandle(){
    if (bsData){
        if (bsData->keepFirstSensedHandle != keepFirstSensedHandle->isChecked()){
            bsData->keepFirstSensedHandle = keepFirstSensedHandle->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setKeepFirstSensedHandle(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::setFoundHandleOut(){
    if (bsData){
        if (bsData->foundHandleOut != foundHandleOut->isChecked()){
            bsData->foundHandleOut = foundHandleOut->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::setFoundHandleOut(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::viewSelectedChild(int row, int column){
    int result = -1;
    bool properties = false;
    if (bsData){
        if (row < ADD_RANGE_ROW && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case ENABLE_ROW:
                    if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "enable");
                    break;
                case SENSOR_LOCAL_OFFSET_ROW:
                    if (table->item(SENSOR_LOCAL_OFFSET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "sensorLocalOffset");
                    break;
                case HANDLE_OUT_ROW:
                    if (table->item(HANDLE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "handleOut");
                    break;
                case HANDLE_IN_ROW:
                    if (table->item(HANDLE_IN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "handleIn");
                    break;
                case MIN_DISTANCE_ROW:
                    if (table->item(MIN_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "minDistance");
                    break;
                case MAX_DISTANCE_ROW:
                    if (table->item(MAX_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "maxDistance");
                    break;
                case DISTANCE_OUT_ROW:
                    if (table->item(DISTANCE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "distanceOut");
                    break;
                case COLLISION_FILTER_INFO_ROW:
                    if (table->item(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "collisionFilterInfo");
                    break;
                case SENSOR_RAGDOLL_BONE_INDEX_ROW:
                    if (table->item(SENSOR_RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "sensorRagdollBoneIndex");
                    break;
                case SENSOR_ANIMATION_BONE_INDEX_ROW:
                    if (table->item(SENSOR_ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "sensorAnimationBoneIndex");
                    break;
                case EXTRAPOLATE_SENSOR_POSITION_ROW:
                    if (table->item(EXTRAPOLATE_SENSOR_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "extrapolateSensorPosition");
                    break;
                case KEEP_FIRST_SENSED_HANDLE_ROW:
                    if (table->item(KEEP_FIRST_SENSED_HANDLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "keepFirstSensedHandle");
                    break;
                case FOUND_HANDLE_OUT_ROW:
                    if (table->item(FOUND_HANDLE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "foundHandleOut");
                    break;
                }
            }
        }else if (row == ADD_RANGE_ROW && column == NAME_COLUMN){
            addRange();
        }else if (row > ADD_RANGE_ROW && row < ADD_RANGE_ROW + bsData->getNumberOfRanges() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->getNumberOfRanges() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    rangeUI->loadData(((BehaviorFile *)(bsData->getParentFile())), &bsData->ranges[result], bsData, result);
                    setCurrentIndex(CHILD_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the range \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeRange(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::viewSelectedChild(): Invalid index of range to view!!"))
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::viewSelectedChild(): The data is NULL!!"))
    }
}

void SenseHandleModifierUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    case CHILD_WIDGET:
        rangeUI->setBindingVariable(index, name);
        break;
    default:
        WARNING_MESSAGE(QString("SenseHandleModifierUI::variableTableElementSelected(): An unwanted element selected event was recieved!!"));;
    }
}

void SenseHandleModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && events && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), rangeUI, SLOT(setEventId(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void SenseHandleModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
                if (varName == ""){
                    varName = "NONE";
                }
                table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::loadBinding(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::selectTableToView(bool viewproperties, const QString & path){
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
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        rangeUI->eventRenamed(name, index);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::eventRenamed(): The data is NULL!!"));
    }
}

void SenseHandleModifierUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = NULL;
    if (name == ""){
        WARNING_MESSAGE(QString("SenseHandleModifierUI::variableRenamed(): The new variable name is the empty string!!"))
    }
    if (bsData){
        index--;
        if (currentIndex() == MAIN_WIDGET){
            bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (bind){
                bindIndex = bind->getVariableIndexOfBinding("enable");
                if (bindIndex == index){
                    table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("sensorLocalOffset");
                if (bindIndex == index){
                    table->item(SENSOR_LOCAL_OFFSET_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("handleOut");
                if (bindIndex == index){
                    table->item(HANDLE_OUT_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("handleIn");
                if (bindIndex == index){
                    table->item(HANDLE_IN_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("minDistance");
                if (bindIndex == index){
                    table->item(MIN_DISTANCE_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("maxDistance");
                if (bindIndex == index){
                    table->item(MAX_DISTANCE_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("distanceOut");
                if (bindIndex == index){
                    table->item(DISTANCE_OUT_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("collisionFilterInfo");
                if (bindIndex == index){
                    table->item(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("sensorRagdollBoneIndex");
                if (bindIndex == index){
                    table->item(SENSOR_RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("sensorAnimationBoneIndex");
                if (bindIndex == index){
                    table->item(SENSOR_ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("extrapolateSensorPosition");
                if (bindIndex == index){
                    table->item(EXTRAPOLATE_SENSOR_POSITION_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("keepFirstSensedHandle");
                if (bindIndex == index){
                    table->item(KEEP_FIRST_SENSED_HANDLE_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("foundHandleOut");
                if (bindIndex == index){
                    table->item(FOUND_HANDLE_OUT_ROW, BINDING_COLUMN)->setText(name);
                }
            }
        }else{
            rangeUI->variableRenamed(name, index);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SenseHandleModifierUI::variableRenamed(): The data is NULL!!"))
    }
}

