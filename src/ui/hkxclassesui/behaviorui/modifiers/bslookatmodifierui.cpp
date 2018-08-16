#include "bslookatmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/modifiers/BSLookAtModifier.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/hkxclassesui/behaviorui/bsboneui.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/mainwindow.h"

#include <QHeaderView>

#include "src/ui/genericdatawidgets.h"

#define BASE_NUMBER_OF_ROWS 19

#define NAME_ROW 0
#define ENABLE_ROW 1
#define LOOK_AT_TARGET_ROW 2
#define LIMIT_ANGLE_DEGREES_ROW 3
#define LIMIT_ANGLE_THRESHOLD_DEGREES_ROW 4
#define CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW 5
#define ON_GAIN_ROW 6
#define OFF_GAIN_ROW 7
#define USE_BONE_GAINS_ROW 8
#define TARGET_LOCATION_ROW 9
#define TARGET_OUTSIDE_LIMITS_ROW 10
#define TARGET_OUT_OF_LIMIT_EVENT_ID_ROW 11
#define TARGET_OUT_OF_LIMIT_EVENT_PAYLOAD_ROW 12
#define LOOK_AT_CAMERA_ROW 13
#define LOOK_AT_CAMERA_X_ROW 14
#define LOOK_AT_CAMERA_Y_ROW 15
#define LOOK_AT_CAMERA_Z_ROW 16
#define ADD_BONE_ROW 17
#define INITIAL_ADD_EYE_BONE_ROW 18

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSLookAtModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSLookAtModifierUI::BSLookAtModifierUI()
    : eyeBoneButtonRow(INITIAL_ADD_EYE_BONE_ROW),
      bsData(nullptr),
      groupBox(new QGroupBox("BSLookAtModifier")),
      topLyt(new QGridLayout),
      boneUI(new BSBoneUI),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      lookAtTarget(new CheckBox),
      limitAngleDegrees(new DoubleSpinBox),
      limitAngleThresholdDegrees(new DoubleSpinBox),
      continueLookOutsideOfLimit(new CheckBox),
      onGain(new DoubleSpinBox),
      offGain(new DoubleSpinBox),
      useBoneGains(new CheckBox),
      targetLocation(new QuadVariableWidget),
      targetOutsideLimits(new CheckBox),
      targetOutOfLimitEventPayload(new LineEdit),
      lookAtCamera(new CheckBox),
      lookAtCameraX(new DoubleSpinBox),
      lookAtCameraY(new DoubleSpinBox),
      lookAtCameraZ(new DoubleSpinBox)
{
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
    table->setItem(ENABLE_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::gray)));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(LOOK_AT_TARGET_ROW, NAME_COLUMN, new TableWidgetItem("lookAtTarget"));
    table->setItem(LOOK_AT_TARGET_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(LOOK_AT_TARGET_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(LOOK_AT_TARGET_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::gray)));
    table->setCellWidget(LOOK_AT_TARGET_ROW, VALUE_COLUMN, lookAtTarget);
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, NAME_COLUMN, new TableWidgetItem("limitAngleDegrees"));
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LIMIT_ANGLE_DEGREES_ROW, VALUE_COLUMN, limitAngleDegrees);
    table->setItem(LIMIT_ANGLE_THRESHOLD_DEGREES_ROW, NAME_COLUMN, new TableWidgetItem("limitAngleThresholdDegrees"));
    table->setItem(LIMIT_ANGLE_THRESHOLD_DEGREES_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_ANGLE_THRESHOLD_DEGREES_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LIMIT_ANGLE_THRESHOLD_DEGREES_ROW, VALUE_COLUMN, limitAngleThresholdDegrees);
    table->setItem(CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW, NAME_COLUMN, new TableWidgetItem("continueLookOutsideOfLimit"));
    table->setItem(CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::gray)));
    table->setCellWidget(CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW, VALUE_COLUMN, continueLookOutsideOfLimit);
    table->setItem(ON_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("onGain"));
    table->setItem(ON_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ON_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ON_GAIN_ROW, VALUE_COLUMN, onGain);
    table->setItem(OFF_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("offGain"));
    table->setItem(OFF_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(OFF_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(OFF_GAIN_ROW, VALUE_COLUMN, offGain);
    table->setItem(USE_BONE_GAINS_ROW, NAME_COLUMN, new TableWidgetItem("useBoneGains"));
    table->setItem(USE_BONE_GAINS_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(USE_BONE_GAINS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(USE_BONE_GAINS_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::gray)));
    table->setCellWidget(USE_BONE_GAINS_ROW, VALUE_COLUMN, useBoneGains);
    table->setItem(TARGET_LOCATION_ROW, NAME_COLUMN, new TableWidgetItem("targetLocation"));
    table->setItem(TARGET_LOCATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(TARGET_LOCATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(TARGET_LOCATION_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(TARGET_LOCATION_ROW, VALUE_COLUMN, targetLocation);
    table->setItem(TARGET_OUTSIDE_LIMITS_ROW, NAME_COLUMN, new TableWidgetItem("targetOutsideLimits"));
    table->setItem(TARGET_OUTSIDE_LIMITS_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(TARGET_OUTSIDE_LIMITS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(TARGET_OUTSIDE_LIMITS_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::gray)));
    table->setCellWidget(TARGET_OUTSIDE_LIMITS_ROW, VALUE_COLUMN, targetOutsideLimits);
    table->setItem(TARGET_OUT_OF_LIMIT_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("targetOutOfLimitEventId"));
    table->setItem(TARGET_OUT_OF_LIMIT_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(TARGET_OUT_OF_LIMIT_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(TARGET_OUT_OF_LIMIT_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(TARGET_OUT_OF_LIMIT_EVENT_PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("targetOutOfLimitEventPayload"));
    table->setItem(TARGET_OUT_OF_LIMIT_EVENT_PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setItem(TARGET_OUT_OF_LIMIT_EVENT_PAYLOAD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(TARGET_OUT_OF_LIMIT_EVENT_PAYLOAD_ROW, VALUE_COLUMN, targetOutOfLimitEventPayload);
    table->setItem(LOOK_AT_CAMERA_ROW, NAME_COLUMN, new TableWidgetItem("lookAtCamera"));
    table->setItem(LOOK_AT_CAMERA_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(LOOK_AT_CAMERA_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LOOK_AT_CAMERA_ROW, VALUE_COLUMN, lookAtCamera);
    table->setItem(LOOK_AT_CAMERA_X_ROW, NAME_COLUMN, new TableWidgetItem("lookAtCameraX"));
    table->setItem(LOOK_AT_CAMERA_X_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LOOK_AT_CAMERA_X_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LOOK_AT_CAMERA_X_ROW, VALUE_COLUMN, lookAtCameraX);
    table->setItem(LOOK_AT_CAMERA_Y_ROW, NAME_COLUMN, new TableWidgetItem("lookAtCameraY"));
    table->setItem(LOOK_AT_CAMERA_Y_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LOOK_AT_CAMERA_Y_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LOOK_AT_CAMERA_Y_ROW, VALUE_COLUMN, lookAtCameraY);
    table->setItem(LOOK_AT_CAMERA_Z_ROW, NAME_COLUMN, new TableWidgetItem("lookAtCameraZ"));
    table->setItem(LOOK_AT_CAMERA_Z_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LOOK_AT_CAMERA_Z_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LOOK_AT_CAMERA_Z_ROW, VALUE_COLUMN, lookAtCameraZ);
    table->setItem(ADD_BONE_ROW, NAME_COLUMN, new TableWidgetItem("Add Bone", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new Bone"));
    table->setItem(ADD_BONE_ROW, TYPE_COLUMN, new TableWidgetItem("BsBone", Qt::AlignCenter));
    table->setItem(ADD_BONE_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Bone", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected Bone"));
    table->setItem(ADD_BONE_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Bone", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected Bone"));
    table->setItem(INITIAL_ADD_EYE_BONE_ROW, NAME_COLUMN, new TableWidgetItem("Add Eye Bone", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new Eye Bone"));
    table->setItem(INITIAL_ADD_EYE_BONE_ROW, TYPE_COLUMN, new TableWidgetItem("BsBone", Qt::AlignCenter, QColor(Qt::gray)));
    table->setItem(INITIAL_ADD_EYE_BONE_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Eye Bone", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected Eye Bone"));
    table->setItem(INITIAL_ADD_EYE_BONE_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Eye Bone", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected Eye Bone"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(boneUI);
    connectSignals();
}

void BSLookAtModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(lookAtTarget, SIGNAL(released()), this, SLOT(setLookAtTarget()), Qt::UniqueConnection);
    connect(limitAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitAngleDegrees()), Qt::UniqueConnection);
    connect(limitAngleThresholdDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitAngleThresholdDegrees()), Qt::UniqueConnection);
    connect(continueLookOutsideOfLimit, SIGNAL(released()), this, SLOT(setContinueLookOutsideOfLimit()), Qt::UniqueConnection);
    connect(onGain, SIGNAL(editingFinished()), this, SLOT(setOnGain()), Qt::UniqueConnection);
    connect(offGain, SIGNAL(editingFinished()), this, SLOT(setOffGain()), Qt::UniqueConnection);
    connect(useBoneGains, SIGNAL(released()), this, SLOT(setUseBoneGains()), Qt::UniqueConnection);
    connect(targetLocation, SIGNAL(editingFinished()), this, SLOT(setTargetLocation()), Qt::UniqueConnection);
    connect(targetOutsideLimits, SIGNAL(released()), this, SLOT(setTargetOutsideLimits()), Qt::UniqueConnection);
    connect(targetOutOfLimitEventPayload, SIGNAL(editingFinished()), this, SLOT(setTargetOutOfLimitEventPayload()), Qt::UniqueConnection);
    connect(lookAtCamera, SIGNAL(released()), this, SLOT(setLookAtCamera()), Qt::UniqueConnection);
    connect(lookAtCameraX, SIGNAL(editingFinished()), this, SLOT(setLookAtCameraX()), Qt::UniqueConnection);
    connect(lookAtCameraY, SIGNAL(editingFinished()), this, SLOT(setLookAtCameraY()), Qt::UniqueConnection);
    connect(lookAtCameraZ, SIGNAL(editingFinished()), this, SLOT(setLookAtCameraZ()), Qt::UniqueConnection);
    connect(boneUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(boneUI, SIGNAL(viewVariables(int,QString,QStringList)), this, SIGNAL(viewVariables(int,QString,QStringList)), Qt::UniqueConnection);
    connect(boneUI, SIGNAL(viewProperties(int,QString,QStringList)), this, SIGNAL(viewProperties(int,QString,QStringList)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void BSLookAtModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(lookAtTarget, SIGNAL(released()), this, SLOT(setLookAtTarget()));
    disconnect(limitAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitAngleDegrees()));
    disconnect(limitAngleThresholdDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitAngleThresholdDegrees()));
    disconnect(continueLookOutsideOfLimit, SIGNAL(released()), this, SLOT(setContinueLookOutsideOfLimit()));
    disconnect(onGain, SIGNAL(editingFinished()), this, SLOT(setOnGain()));
    disconnect(offGain, SIGNAL(editingFinished()), this, SLOT(setOffGain()));
    disconnect(useBoneGains, SIGNAL(released()), this, SLOT(setUseBoneGains()));
    disconnect(targetLocation, SIGNAL(editingFinished()), this, SLOT(setTargetLocation()));
    disconnect(targetOutsideLimits, SIGNAL(released()), this, SLOT(setTargetOutsideLimits()));
    disconnect(targetOutOfLimitEventPayload, SIGNAL(editingFinished()), this, SLOT(setTargetOutOfLimitEventPayload()));
    disconnect(lookAtCamera, SIGNAL(released()), this, SLOT(setLookAtCamera()));
    disconnect(lookAtCameraX, SIGNAL(editingFinished()), this, SLOT(setLookAtCameraX()));
    disconnect(lookAtCameraY, SIGNAL(editingFinished()), this, SLOT(setLookAtCameraY()));
    disconnect(lookAtCameraZ, SIGNAL(editingFinished()), this, SLOT(setLookAtCameraZ()));
    disconnect(boneUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(boneUI, SIGNAL(viewVariables(int,QString,QStringList)), this, SIGNAL(viewVariables(int,QString,QStringList)));
    disconnect(boneUI, SIGNAL(viewProperties(int,QString,QStringList)), this, SIGNAL(viewProperties(int,QString,QStringList)));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void BSLookAtModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    if (data && data->getSignature() == BS_LOOK_AT_MODIFIER){
        bsData = static_cast<BSLookAtModifier *>(data);
        hkbVariableBindingSet *varBind = nullptr;
        hkbStringEventPayload *payload = static_cast<hkbStringEventPayload *>(bsData->payload.data());
        name->setText(bsData->getName());
        enable->setChecked(bsData->enable);
        lookAtTarget->setChecked(bsData->lookAtTarget);
        limitAngleDegrees->setValue(bsData->limitAngleDegrees);
        limitAngleThresholdDegrees->setValue(bsData->limitAngleThresholdDegrees);
        continueLookOutsideOfLimit->setChecked(bsData->continueLookOutsideOfLimit);
        onGain->setValue(bsData->onGain);
        offGain->setValue(bsData->offGain);
        useBoneGains->setChecked(bsData->useBoneGains);
        targetLocation->setValue(bsData->targetLocation);
        targetOutsideLimits->setChecked(bsData->targetOutsideLimits);
        QString text = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->id);
        if (text != ""){
            table->item(TARGET_OUT_OF_LIMIT_EVENT_ID_ROW, VALUE_COLUMN)->setText(text);
        }else{
            table->item(TARGET_OUT_OF_LIMIT_EVENT_ID_ROW, VALUE_COLUMN)->setText("None");
        }
        if (payload){
            targetOutOfLimitEventPayload->setText(payload->getData());
        }else{
            targetOutOfLimitEventPayload->setText("");
        }
        lookAtCamera->setChecked(bsData->lookAtCamera);
        lookAtCameraX->setValue(bsData->lookAtCameraX);
        lookAtCameraY->setValue(bsData->lookAtCameraY);
        lookAtCameraZ->setValue(bsData->lookAtCameraZ);
        varBind = bsData->getVariableBindingSetData();
        if (varBind){
            loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
            loadBinding(LOOK_AT_TARGET_ROW, BINDING_COLUMN, varBind, "lookAtTarget");
            loadBinding(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN, varBind, "limitAngleDegrees");
            loadBinding(LIMIT_ANGLE_THRESHOLD_DEGREES_ROW, BINDING_COLUMN, varBind, "limitAngleThresholdDegrees");
            loadBinding(CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW, BINDING_COLUMN, varBind, "continueLookOutsideOfLimit");
            loadBinding(ON_GAIN_ROW, BINDING_COLUMN, varBind, "onGain");
            loadBinding(OFF_GAIN_ROW, BINDING_COLUMN, varBind, "offGain");
            loadBinding(USE_BONE_GAINS_ROW, BINDING_COLUMN, varBind, "useBoneGains");
            loadBinding(TARGET_LOCATION_ROW, BINDING_COLUMN, varBind, "targetLocation");
            loadBinding(TARGET_OUTSIDE_LIMITS_ROW, BINDING_COLUMN, varBind, "targetOutsideLimits");
            loadBinding(LOOK_AT_CAMERA_ROW, BINDING_COLUMN, varBind, "lookAtCamera");
            loadBinding(LOOK_AT_CAMERA_X_ROW, BINDING_COLUMN, varBind, "lookAtCameraX");
            loadBinding(LOOK_AT_CAMERA_Y_ROW, BINDING_COLUMN, varBind, "lookAtCameraY");
            loadBinding(LOOK_AT_CAMERA_Z_ROW, BINDING_COLUMN, varBind, "lookAtCameraZ");
        }else{
            table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(LOOK_AT_TARGET_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(LIMIT_ANGLE_THRESHOLD_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(ON_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(OFF_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(USE_BONE_GAINS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(TARGET_LOCATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(TARGET_OUTSIDE_LIMITS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(LOOK_AT_CAMERA_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(LOOK_AT_CAMERA_X_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(LOOK_AT_CAMERA_Y_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(LOOK_AT_CAMERA_Z_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }
        if (boneList.isEmpty()){
            boneList.append("None");
            boneList = boneList + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::loadData(): The data is nullptr or an incorrect type!!");
    }
    connectSignals();
}

void BSLookAtModifierUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_BONE_ROW + bsData->getNumberOfBones() + 1 - eyeBoneButtonRow;
        if (temp > 0){
            for (auto i = 0; i < temp; i++){
                table->insertRow(eyeBoneButtonRow);
                eyeBoneButtonRow++;
            }
        }else if (temp < 0){
            for (auto i = temp; i < 0; i++){
                table->removeRow(eyeBoneButtonRow - 1);
                eyeBoneButtonRow--;
            }
        }
        eyeBoneButtonRow = ADD_BONE_ROW + bsData->getNumberOfBones() + 1;
        for (auto i = INITIAL_ADD_EYE_BONE_ROW, j = 0; i < eyeBoneButtonRow, j < bsData->getNumberOfBones(); i++, j++){
            if (bsData->bones.at(j).index > -2 && bsData->bones.at(j).index < boneList.size()){
                setRowItems(i, boneList.at(bsData->bones.at(j).index + 1), "BsBone", "Remove", "Edit", "Double click to remove this bone", "Double click to edit this bone");
            }else{
                CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::loadDynamicTableRows(): Bone index out of range!!");
            }
        }
        table->setRowCount(eyeBoneButtonRow + bsData->getNumberOfEyeBones() + 1);
        for (auto i = eyeBoneButtonRow + 1, j = 0; i < table->rowCount(), j < bsData->getNumberOfEyeBones(); i++, j++){
            if (bsData->eyeBones.at(j).index > -2 && bsData->eyeBones.at(j).index < boneList.size()){
                setRowItems(i, boneList.at(bsData->eyeBones.at(j).index + 1), "BsBone", "Remove", "Edit", "Double click to remove this eye bone", "Double click to edit this eye bone");
            }else{
                CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::loadDynamicTableRows(): Eye bone index out of range!!");
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::loadDynamicTableRows(): The data is nullptr!!");
    }
    //table->setSortingEnabled(true);
}

void BSLookAtModifierUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
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

void BSLookAtModifierUI::setName(){
    if (bsData){
        if (bsData->getName() != name->text()){
            bsData->getName() = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->setIsFileChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setName(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setEnable(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setLookAtTarget(){
    if (bsData){
        bsData->lookAtTarget = lookAtTarget->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setLookAtTarget(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setLimitAngleDegrees(){
    if (bsData){
        if (bsData->limitAngleDegrees != limitAngleDegrees->value()){
            bsData->limitAngleDegrees = limitAngleDegrees->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setLimitAngleDegrees(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setLimitAngleThresholdDegrees(){
    if (bsData){
        if (bsData->limitAngleThresholdDegrees != limitAngleThresholdDegrees->value()){
            bsData->limitAngleThresholdDegrees = limitAngleThresholdDegrees->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setLimitAngleThresholdDegrees(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setContinueLookOutsideOfLimit(){
    if (bsData){
        bsData->continueLookOutsideOfLimit = continueLookOutsideOfLimit->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setContinueLookOutsideOfLimit(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setOnGain(){
    if (bsData){
        if (bsData->onGain != onGain->value()){
            bsData->onGain = onGain->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setOnGain(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setOffGain(){
    if (bsData){
        if (bsData->offGain != offGain->value()){
            bsData->offGain = offGain->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setOffGain(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setUseBoneGains(){
    if (bsData){
        bsData->useBoneGains = useBoneGains->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setUseBoneGains(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setTargetLocation(){
    if (bsData){
        if (bsData->targetLocation != targetLocation->value()){
            bsData->targetLocation = targetLocation->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setTargetLocation(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setTargetOutsideLimits(){
    if (bsData){
        bsData->targetOutsideLimits = targetOutsideLimits->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setTargetOutsideLimits(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setTargetOutOfLimitEventId(int index, const QString &name){
    if (bsData){
        bsData->id = index - 1;
        table->item(TARGET_OUT_OF_LIMIT_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("StateMachineUI::setTargetOutOfLimitEventId(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setTargetOutOfLimitEventPayload(){
    hkbStringEventPayload *payload;
    if (bsData){
        payload = static_cast<hkbStringEventPayload *>(bsData->payload.data());
        if (targetOutOfLimitEventPayload->text() != ""){
            if (payload){
                payload->getData() = targetOutOfLimitEventPayload->text();
            }else{
                payload = new hkbStringEventPayload(bsData->getParentFile(), targetOutOfLimitEventPayload->text());
                //bsData->getParentFile()->addObjectToFile(payload, -1);
                bsData->payload = HkxSharedPtr(payload);
            }
        }else{
            bsData->payload = HkxSharedPtr();
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setTargetOutOfLimitEventPayload(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setLookAtCamera(){
    if (bsData){
        bsData->lookAtCamera = lookAtCamera->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setLookAtCamera(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setLookAtCameraX(){
    if (bsData){
        if (bsData->lookAtCameraX != lookAtCameraX->value()){
            bsData->lookAtCameraX = lookAtCameraX->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setLookAtCameraX(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setLookAtCameraY(){
    if (bsData){
        if (bsData->lookAtCameraY != lookAtCameraY->value()){
            bsData->lookAtCameraY = lookAtCameraY->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setLookAtCameraY(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::setLookAtCameraZ(){
    if (bsData){
        if (bsData->lookAtCameraZ != lookAtCameraZ->value()){
            bsData->lookAtCameraZ = lookAtCameraZ->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setLookAtCameraZ(): The data is nullptr!!");
    }
}

bool BSLookAtModifierUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = bsData->getVariableBindingSetData();
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->getVariableBindingSet() = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1), type)) ||
                  (isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1), type))){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->getVariableBindingSet() = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void BSLookAtModifierUI::setBindingVariable(int index, const QString & name){
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
        case LOOK_AT_TARGET_ROW:
            if (table->item(LOOK_AT_TARGET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "lookAtTarget", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case LIMIT_ANGLE_DEGREES_ROW:
            if (table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "limitAngleDegrees", VARIABLE_TYPE_REAL, isProperty);
            break;
        case LIMIT_ANGLE_THRESHOLD_DEGREES_ROW:
            if (table->item(LIMIT_ANGLE_THRESHOLD_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "limitAngleThresholdDegrees", VARIABLE_TYPE_REAL, isProperty);
            break;
        case CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW:
            if (table->item(CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "continueLookOutsideOfLimit", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case ON_GAIN_ROW:
            if (table->item(ON_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "onGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case OFF_GAIN_ROW:
            if (table->item(OFF_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "offGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case USE_BONE_GAINS_ROW:
            if (table->item(USE_BONE_GAINS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "useBoneGains", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case TARGET_LOCATION_ROW:
            if (table->item(TARGET_LOCATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "targetLocation", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case TARGET_OUTSIDE_LIMITS_ROW:
            if (table->item(TARGET_OUTSIDE_LIMITS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "targetOutsideLimits", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case LOOK_AT_CAMERA_ROW:
            if (table->item(LOOK_AT_CAMERA_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "lookAtCamera", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case LOOK_AT_CAMERA_X_ROW:
            if (table->item(LOOK_AT_CAMERA_X_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "lookAtCameraX", VARIABLE_TYPE_REAL, isProperty);
            break;
        case LOOK_AT_CAMERA_Y_ROW:
            if (table->item(LOOK_AT_CAMERA_Y_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "lookAtCameraY", VARIABLE_TYPE_REAL, isProperty);
            break;
        case LOOK_AT_CAMERA_Z_ROW:
            if (table->item(LOOK_AT_CAMERA_Z_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "lookAtCameraZ", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (bsData->getVariableBindingSetData()){
                emit viewProperties(bsData->getVariableBindingSetData()->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewProperties(0, QString(), QStringList());
            }
        }else{
            if (bsData->getVariableBindingSetData()){
                emit viewVariables(bsData->getVariableBindingSetData()->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewVariables(0, QString(), QStringList());
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::viewSelectedChild(int row, int column){
    int result;
    bool properties = false;
    if (bsData){
        if (row < ADD_BONE_ROW && row >= 0){
            if (column == VALUE_COLUMN && row == TARGET_OUT_OF_LIMIT_EVENT_ID_ROW){
                emit viewEvents(bsData->id + 1, QString(), QStringList());
            }else if (column == BINDING_COLUMN){
                switch (row){
                case ENABLE_ROW:
                    if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "enable");
                    break;
                case LOOK_AT_TARGET_ROW:
                    if (table->item(LOOK_AT_TARGET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "lookAtTarget");
                    break;
                case LIMIT_ANGLE_DEGREES_ROW:
                    if (table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "limitAngleDegrees");
                    break;
                case LIMIT_ANGLE_THRESHOLD_DEGREES_ROW:
                    if (table->item(LIMIT_ANGLE_THRESHOLD_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "limitAngleThresholdDegrees");
                    break;
                case CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW:
                    if (table->item(CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "continueLookOutsideOfLimit");
                    break;
                case ON_GAIN_ROW:
                    if (table->item(ON_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "onGain");
                    break;
                case OFF_GAIN_ROW:
                    if (table->item(OFF_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "offGain");
                    break;
                case USE_BONE_GAINS_ROW:
                    if (table->item(USE_BONE_GAINS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "useBoneGains");
                    break;
                case TARGET_LOCATION_ROW:
                    if (table->item(TARGET_LOCATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "targetLocation");
                    break;
                case TARGET_OUTSIDE_LIMITS_ROW:
                    if (table->item(TARGET_OUTSIDE_LIMITS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "targetOutsideLimits");
                    break;
                case LOOK_AT_CAMERA_ROW:
                    if (table->item(LOOK_AT_CAMERA_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "lookAtCamera");
                    break;
                case LOOK_AT_CAMERA_X_ROW:
                    if (table->item(LOOK_AT_CAMERA_X_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "lookAtCameraX");
                    break;
                case LOOK_AT_CAMERA_Y_ROW:
                    if (table->item(LOOK_AT_CAMERA_Y_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "lookAtCameraY");
                    break;
                case LOOK_AT_CAMERA_Z_ROW:
                    if (table->item(LOOK_AT_CAMERA_Z_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "lookAtCameraZ");
                    break;
                }
            }
        }else if (row == ADD_BONE_ROW && column == NAME_COLUMN){
            addBone();
        }else if (row == eyeBoneButtonRow && column == NAME_COLUMN){
            addEyeBone();
        }else if (row > ADD_BONE_ROW && row < eyeBoneButtonRow){
            result = row - BASE_NUMBER_OF_ROWS + 1;
            if (bsData->getNumberOfBones() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    boneUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &bsData->bones[result], bsData, result, false);
                    setCurrentIndex(BONE_UI_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the bone \""+boneList.at(bsData->bones.at(result).index + 1)+"\"?") == QMessageBox::Yes){    //Unsafe...
                        removeBone(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::viewSelectedChild(): Invalid index of child to view!!");
            }
        }else if (row > eyeBoneButtonRow && row < table->rowCount()){
            result = row - BASE_NUMBER_OF_ROWS - bsData->getNumberOfBones();
            if (result < bsData->getNumberOfEyeBones() && result >= 0){
                if (column == VALUE_COLUMN){
                    boneUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &bsData->eyeBones[result], bsData, result, true);
                    setCurrentIndex(BONE_UI_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the eye bone \""+boneList.at(bsData->eyeBones.at(result).index + 1)+"\"?") == QMessageBox::Yes){    //Unsafe...
                        removeEyeBone(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::viewSelectedChild(): Invalid index of child to view!!");
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    case BONE_UI_WIDGET:
        boneUI->setBindingVariable(index, name);
        break;
    default:
        WARNING_MESSAGE("BSLookAtModifierUI::variableTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void BSLookAtModifierUI::addBone(){
    if (bsData){
        bsData->bones.append(BSLookAtModifier::BsBone());
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::addBone(): The data or behavior graph pointer is nullptr!!");
    }
}

void BSLookAtModifierUI::removeBone(int index){
    if (bsData){
        if (index >= 0 && index < bsData->bones.size()){
            bsData->bones.removeAt(index);
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::removeBone(): The data or behavior graph pointer is nullptr!!");
    }
}

void BSLookAtModifierUI::removeEyeBone(int index){
    if (bsData){
        if (index >= 0 && index < bsData->eyeBones.size()){
            bsData->eyeBones.removeAt(index);
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::removeEyeBone(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::addEyeBone(){
    if (bsData){
        bsData->eyeBones.append(BSLookAtModifier::BsBone());
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::addEyeBone(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::returnToWidget(){
    loadDynamicTableRows();
    setCurrentIndex(MAIN_WIDGET);
}

void BSLookAtModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && events && properties){
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(setTargetOutOfLimitEventId(int,QString)), Qt::UniqueConnection);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int,QString,QStringList)), events, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void BSLookAtModifierUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("BSLookAtModifierUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        index--;
        bind = bsData->getVariableBindingSetData();
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("lookAtTarget");
            if (bindIndex == index){
                table->item(LOOK_AT_TARGET_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("limitAngleDegrees");
            if (bindIndex == index){
                table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("limitAngleThresholdDegrees");
            if (bindIndex == index){
                table->item(LIMIT_ANGLE_THRESHOLD_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("continueLookOutsideOfLimit");
            if (bindIndex == index){
                table->item(CONTINUE_LOOK_OUTSIDE_OF_LIMIT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("onGain");
            if (bindIndex == index){
                table->item(ON_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("offGain");
            if (bindIndex == index){
                table->item(OFF_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("useBoneGains");
            if (bindIndex == index){
                table->item(USE_BONE_GAINS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("targetLocation");
            if (bindIndex == index){
                table->item(TARGET_LOCATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("targetOutsideLimits");
            if (bindIndex == index){
                table->item(TARGET_OUTSIDE_LIMITS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("lookAtCamera");
            if (bindIndex == index){
                table->item(LOOK_AT_CAMERA_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("lookAtCameraX");
            if (bindIndex == index){
                table->item(LOOK_AT_CAMERA_X_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("lookAtCameraY");
            if (bindIndex == index){
                table->item(LOOK_AT_CAMERA_Y_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("lookAtCameraZ");
            if (bindIndex == index){
                table->item(LOOK_AT_CAMERA_Z_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::variableRenamed(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (currentIndex() == MAIN_WIDGET){
            if (index == bsData->id){
                table->item(TARGET_OUT_OF_LIMIT_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::eventRenamed(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    table->item(row, column)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, column)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::loadBinding(): The data is nullptr!!");
    }
}

void BSLookAtModifierUI::loadTableValue(int row, const QString &value){
    if (table->item(row, VALUE_COLUMN)){
        if (value != ""){
            table->item(row, VALUE_COLUMN)->setText(value);
        }else{
            table->item(row, VALUE_COLUMN)->setText("NONE");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSLookAtModifierUI::loadTableValue(): There is no table item here!!");
    }
}

