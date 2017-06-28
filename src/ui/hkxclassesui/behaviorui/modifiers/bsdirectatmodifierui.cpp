#include "bsdirectatmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/bsdirectatmodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 21

#define NAME_ROW 0
#define ENABLE_ROW 1
#define DIRECT_AT_TARGET_ROW 2
#define SOURCE_BONE_INDEX_ROW 3
#define START_BONE_INDEX_ROW 4
#define END_BONE_INDEX_ROW 5
#define LIMIT_HEADING_DEGREES_ROW 6
#define LIMIT_PITCH_DEGREES_ROW 7
#define OFFSET_HEADING_DEGREES_ROW 8
#define OFFSET_PITCH_DEGREES_ROW 9
#define ON_GAIN_ROW 10
#define OFF_GAIN_ROW 11
#define TARGET_LOCATION_ROW 12
#define USER_INFO_ROW 13
#define DIRECT_AT_CAMERA_ROW 14
#define DIRECT_AT_CAMERA_X_ROW 15
#define DIRECT_AT_CAMERA_Y_ROW 16
#define DIRECT_AT_CAMERA_Z_ROW 17
#define ACTIVE_ROW 18
#define CURRENT_HEADING_OFFSET_ROW 19
#define CURRENT_PITCH_OFFSET_ROW 20

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSDirectAtModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSDirectAtModifierUI::BSDirectAtModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::cyan))),
      name(new LineEdit),
      enable(new CheckBox),
      directAtTarget(new CheckBox),
      sourceBoneIndex(new ComboBox),
      startBoneIndex(new ComboBox),
      endBoneIndex(new ComboBox),
      limitHeadingDegrees(new DoubleSpinBox),
      limitPitchDegrees(new DoubleSpinBox),
      offsetHeadingDegrees(new DoubleSpinBox),
      offsetPitchDegrees(new DoubleSpinBox),
      onGain(new DoubleSpinBox),
      offGain(new DoubleSpinBox),
      targetLocation(new QuadVariableWidget),
      userInfo(new SpinBox),
      directAtCamera(new CheckBox),
      directAtCameraX(new DoubleSpinBox),
      directAtCameraY(new DoubleSpinBox),
      directAtCameraZ(new DoubleSpinBox),
      active(new CheckBox),
      currentHeadingOffset(new DoubleSpinBox),
      currentPitchOffset(new DoubleSpinBox)
{
    setTitle("BSDirectAtModifier");
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
    table->setItem(DIRECT_AT_TARGET_ROW, NAME_COLUMN, new TableWidgetItem("directAtTarget"));
    table->setItem(DIRECT_AT_TARGET_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(DIRECT_AT_TARGET_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DIRECT_AT_TARGET_ROW, VALUE_COLUMN, directAtTarget);
    table->setItem(SOURCE_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("sourceBoneIndex"));
    table->setItem(SOURCE_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(SOURCE_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SOURCE_BONE_INDEX_ROW, VALUE_COLUMN, sourceBoneIndex);
    table->setItem(START_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("startBoneIndex"));
    table->setItem(START_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(START_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(START_BONE_INDEX_ROW, VALUE_COLUMN, startBoneIndex);
    table->setItem(END_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("endBoneIndex"));
    table->setItem(END_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(END_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(END_BONE_INDEX_ROW, VALUE_COLUMN, endBoneIndex);
    table->setItem(LIMIT_HEADING_DEGREES_ROW, NAME_COLUMN, new TableWidgetItem("limitHeadingDegrees"));
    table->setItem(LIMIT_HEADING_DEGREES_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_HEADING_DEGREES_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LIMIT_HEADING_DEGREES_ROW, VALUE_COLUMN, limitHeadingDegrees);
    table->setItem(LIMIT_PITCH_DEGREES_ROW, NAME_COLUMN, new TableWidgetItem("limitPitchDegrees"));
    table->setItem(LIMIT_PITCH_DEGREES_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_PITCH_DEGREES_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LIMIT_PITCH_DEGREES_ROW, VALUE_COLUMN, limitPitchDegrees);
    table->setItem(OFFSET_HEADING_DEGREES_ROW, NAME_COLUMN, new TableWidgetItem("offsetHeadingDegrees"));
    table->setItem(OFFSET_HEADING_DEGREES_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(OFFSET_HEADING_DEGREES_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(OFFSET_HEADING_DEGREES_ROW, VALUE_COLUMN, offsetHeadingDegrees);
    table->setItem(OFFSET_PITCH_DEGREES_ROW, NAME_COLUMN, new TableWidgetItem("offsetPitchDegrees"));
    table->setItem(OFFSET_PITCH_DEGREES_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(OFFSET_PITCH_DEGREES_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(OFFSET_PITCH_DEGREES_ROW, VALUE_COLUMN, offsetPitchDegrees);
    table->setItem(ON_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("onGain"));
    table->setItem(ON_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ON_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ON_GAIN_ROW, VALUE_COLUMN, onGain);
    table->setItem(OFF_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("offGain"));
    table->setItem(OFF_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(OFF_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(OFF_GAIN_ROW, VALUE_COLUMN, offGain);
    table->setItem(TARGET_LOCATION_ROW, NAME_COLUMN, new TableWidgetItem("targetLocation"));
    table->setItem(TARGET_LOCATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(TARGET_LOCATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TARGET_LOCATION_ROW, VALUE_COLUMN, targetLocation);
    table->setItem(USER_INFO_ROW, NAME_COLUMN, new TableWidgetItem("userInfo"));
    table->setItem(USER_INFO_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(USER_INFO_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(USER_INFO_ROW, VALUE_COLUMN, userInfo);
    table->setItem(DIRECT_AT_CAMERA_ROW, NAME_COLUMN, new TableWidgetItem("directAtCamera"));
    table->setItem(DIRECT_AT_CAMERA_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(DIRECT_AT_CAMERA_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DIRECT_AT_CAMERA_ROW, VALUE_COLUMN, directAtCamera);
    table->setItem(DIRECT_AT_CAMERA_X_ROW, NAME_COLUMN, new TableWidgetItem("directAtCameraX"));
    table->setItem(DIRECT_AT_CAMERA_X_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DIRECT_AT_CAMERA_X_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DIRECT_AT_CAMERA_X_ROW, VALUE_COLUMN, directAtCameraX);
    table->setItem(DIRECT_AT_CAMERA_Y_ROW, NAME_COLUMN, new TableWidgetItem("directAtCameraY"));
    table->setItem(DIRECT_AT_CAMERA_Y_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DIRECT_AT_CAMERA_Y_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DIRECT_AT_CAMERA_Y_ROW, VALUE_COLUMN, directAtCameraY);
    table->setItem(DIRECT_AT_CAMERA_Z_ROW, NAME_COLUMN, new TableWidgetItem("directAtCameraZ"));
    table->setItem(DIRECT_AT_CAMERA_Z_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DIRECT_AT_CAMERA_Z_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DIRECT_AT_CAMERA_Z_ROW, VALUE_COLUMN, directAtCameraZ);
    table->setItem(ACTIVE_ROW, NAME_COLUMN, new TableWidgetItem("active"));
    table->setItem(ACTIVE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ACTIVE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ACTIVE_ROW, VALUE_COLUMN, active);
    table->setItem(CURRENT_HEADING_OFFSET_ROW, NAME_COLUMN, new TableWidgetItem("currentHeadingOffset"));
    table->setItem(CURRENT_HEADING_OFFSET_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(CURRENT_HEADING_OFFSET_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(CURRENT_HEADING_OFFSET_ROW, VALUE_COLUMN, currentHeadingOffset);
    table->setItem(CURRENT_PITCH_OFFSET_ROW, NAME_COLUMN, new TableWidgetItem("currentPitchOffset"));
    table->setItem(CURRENT_PITCH_OFFSET_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(CURRENT_PITCH_OFFSET_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(CURRENT_PITCH_OFFSET_ROW, VALUE_COLUMN, currentPitchOffset);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void BSDirectAtModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(directAtTarget, SIGNAL(released()), this, SLOT(setDirectAtTarget()), Qt::UniqueConnection);
    connect(sourceBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setSourceBoneIndex(int)), Qt::UniqueConnection);
    connect(startBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartBoneIndex(int)), Qt::UniqueConnection);
    connect(endBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setEndBoneIndex(int)), Qt::UniqueConnection);
    connect(limitHeadingDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitHeadingDegrees()), Qt::UniqueConnection);
    connect(limitPitchDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitPitchDegrees()), Qt::UniqueConnection);
    connect(offsetHeadingDegrees, SIGNAL(editingFinished()), this, SLOT(setOffsetHeadingDegrees()), Qt::UniqueConnection);
    connect(offsetPitchDegrees, SIGNAL(editingFinished()), this, SLOT(setOffsetPitchDegrees()), Qt::UniqueConnection);
    connect(onGain, SIGNAL(editingFinished()), this, SLOT(setOnGain()), Qt::UniqueConnection);
    connect(offGain, SIGNAL(editingFinished()), this, SLOT(setOffGain()), Qt::UniqueConnection);
    connect(targetLocation, SIGNAL(editingFinished()), this, SLOT(setTargetLocation()), Qt::UniqueConnection);
    connect(userInfo, SIGNAL(editingFinished()), this, SLOT(setUserInfo()), Qt::UniqueConnection);
    connect(directAtCamera, SIGNAL(released()), this, SLOT(setDirectAtCamera()), Qt::UniqueConnection);
    connect(directAtCameraX, SIGNAL(editingFinished()), this, SLOT(setDirectAtCameraX()), Qt::UniqueConnection);
    connect(directAtCameraY, SIGNAL(editingFinished()), this, SLOT(setDirectAtCameraY()), Qt::UniqueConnection);
    connect(directAtCameraZ, SIGNAL(editingFinished()), this, SLOT(setDirectAtCameraZ()), Qt::UniqueConnection);
    connect(active, SIGNAL(released()), this, SLOT(setActive()), Qt::UniqueConnection);
    connect(currentHeadingOffset, SIGNAL(editingFinished()), this, SLOT(setCurrentHeadingOffset()), Qt::UniqueConnection);
    connect(currentPitchOffset, SIGNAL(editingFinished()), this, SLOT(setCurrentPitchOffset()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSDirectAtModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(directAtTarget, SIGNAL(released()), this, SLOT(setDirectAtTarget()));
    disconnect(sourceBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setSourceBoneIndex(int)));
    disconnect(startBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartBoneIndex(int)));
    disconnect(endBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setEndBoneIndex(int)));
    disconnect(limitHeadingDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitHeadingDegrees()));
    disconnect(limitPitchDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitPitchDegrees()));
    disconnect(offsetHeadingDegrees, SIGNAL(editingFinished()), this, SLOT(setOffsetHeadingDegrees()));
    disconnect(offsetPitchDegrees, SIGNAL(editingFinished()), this, SLOT(setOffsetPitchDegrees()));
    disconnect(onGain, SIGNAL(editingFinished()), this, SLOT(setOnGain()));
    disconnect(offGain, SIGNAL(editingFinished()), this, SLOT(setOffGain()));
    disconnect(targetLocation, SIGNAL(editingFinished()), this, SLOT(setTargetLocation()));
    disconnect(userInfo, SIGNAL(editingFinished()), this, SLOT(setUserInfo()));
    disconnect(directAtCamera, SIGNAL(released()), this, SLOT(setDirectAtCamera()));
    disconnect(directAtCameraX, SIGNAL(editingFinished()), this, SLOT(setDirectAtCameraX()));
    disconnect(directAtCameraY, SIGNAL(editingFinished()), this, SLOT(setDirectAtCameraY()));
    disconnect(directAtCameraZ, SIGNAL(editingFinished()), this, SLOT(setDirectAtCameraZ()));
    disconnect(active, SIGNAL(released()), this, SLOT(setActive()));
    disconnect(currentHeadingOffset, SIGNAL(editingFinished()), this, SLOT(setCurrentHeadingOffset()));
    disconnect(currentPitchOffset, SIGNAL(editingFinished()), this, SLOT(setCurrentPitchOffset()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSDirectAtModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void BSDirectAtModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_DIRECT_AT_MODIFIER){
            QStringList boneNames;
            boneNames.append("None");
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<BSDirectAtModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            directAtTarget->setChecked(bsData->directAtTarget);
            if (sourceBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                sourceBoneIndex->insertItems(0, boneNames);
            }
            sourceBoneIndex->setCurrentIndex(bsData->sourceBoneIndex + 1);
            if (startBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                startBoneIndex->insertItems(0, boneNames);
            }
            startBoneIndex->setCurrentIndex(bsData->startBoneIndex + 1);
            if (endBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                endBoneIndex->insertItems(0, boneNames);
            }
            endBoneIndex->setCurrentIndex(bsData->endBoneIndex + 1);
            limitHeadingDegrees->setValue(bsData->limitHeadingDegrees);
            limitPitchDegrees->setValue(bsData->limitPitchDegrees);
            offsetHeadingDegrees->setValue(bsData->offsetHeadingDegrees);
            offsetPitchDegrees->setValue(bsData->offsetPitchDegrees);
            onGain->setValue(bsData->onGain);
            offGain->setValue(bsData->offGain);
            targetLocation->setValue(bsData->targetLocation);
            userInfo->setValue(bsData->userInfo);
            directAtCamera->setChecked(bsData->directAtCamera);
            directAtCameraX->setValue(bsData->directAtCameraX);
            directAtCameraY->setValue(bsData->directAtCameraY);
            directAtCameraZ->setValue(bsData->directAtCameraZ);
            active->setChecked(bsData->active);
            currentHeadingOffset->setValue(bsData->currentHeadingOffset);
            currentPitchOffset->setValue(bsData->currentPitchOffset);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(DIRECT_AT_TARGET_ROW, BINDING_COLUMN, varBind, "directAtTarget");
                loadBinding(SOURCE_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "sourceBoneIndex");
                loadBinding(START_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "startBoneIndex");
                loadBinding(END_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "endBoneIndex");
                loadBinding(LIMIT_HEADING_DEGREES_ROW, BINDING_COLUMN, varBind, "limitHeadingDegrees");
                loadBinding(LIMIT_PITCH_DEGREES_ROW, BINDING_COLUMN, varBind, "limitPitchDegrees");
                loadBinding(OFFSET_HEADING_DEGREES_ROW, BINDING_COLUMN, varBind, "offsetHeadingDegrees");
                loadBinding(OFFSET_PITCH_DEGREES_ROW, BINDING_COLUMN, varBind, "offsetPitchDegrees");
                loadBinding(ON_GAIN_ROW, BINDING_COLUMN, varBind, "onGain");
                loadBinding(OFF_GAIN_ROW, BINDING_COLUMN, varBind, "offGain");
                loadBinding(TARGET_LOCATION_ROW, BINDING_COLUMN, varBind, "targetLocation");
                loadBinding(USER_INFO_ROW, BINDING_COLUMN, varBind, "userInfo");
                loadBinding(DIRECT_AT_CAMERA_ROW, BINDING_COLUMN, varBind, "directAtCamera");
                loadBinding(DIRECT_AT_CAMERA_X_ROW, BINDING_COLUMN, varBind, "directAtCameraX");
                loadBinding(DIRECT_AT_CAMERA_Y_ROW, BINDING_COLUMN, varBind, "directAtCameraY");
                loadBinding(DIRECT_AT_CAMERA_Z_ROW, BINDING_COLUMN, varBind, "directAtCameraZ");
                loadBinding(ACTIVE_ROW, BINDING_COLUMN, varBind, "active");
                loadBinding(CURRENT_HEADING_OFFSET_ROW, BINDING_COLUMN, varBind, "currentHeadingOffset");
                loadBinding(CURRENT_PITCH_OFFSET_ROW, BINDING_COLUMN, varBind, "currentPitchOffset");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DIRECT_AT_TARGET_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SOURCE_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(START_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(END_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LIMIT_HEADING_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LIMIT_PITCH_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(OFFSET_HEADING_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(OFFSET_PITCH_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ON_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(OFF_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TARGET_LOCATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(USER_INFO_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DIRECT_AT_CAMERA_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DIRECT_AT_CAMERA_X_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DIRECT_AT_CAMERA_Y_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DIRECT_AT_CAMERA_Z_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ACTIVE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(CURRENT_HEADING_OFFSET_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(CURRENT_PITCH_OFFSET_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void BSDirectAtModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setName(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setEnable(){
    if (bsData){
        if (bsData->enable != enable->isChecked()){
            bsData->enable = enable->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setEnable(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setDirectAtTarget(){
    if (bsData){
        if (bsData->directAtTarget != directAtTarget->isChecked()){
            bsData->directAtTarget = directAtTarget->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setDirectAtTarget(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setSourceBoneIndex(int index){
    if (bsData){
        bsData->sourceBoneIndex = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setSourceBoneIndex(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setStartBoneIndex(int index){
    if (bsData){
        bsData->startBoneIndex = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setStartBoneIndex(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setEndBoneIndex(int index){
    if (bsData){
        bsData->endBoneIndex = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setEndBoneIndex(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setLimitHeadingDegrees(){
    if (bsData){
        if (bsData->limitHeadingDegrees != limitHeadingDegrees->value()){
            bsData->limitHeadingDegrees = limitHeadingDegrees->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setLimitHeadingDegrees(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setLimitPitchDegrees(){
    if (bsData){
        if (bsData->limitPitchDegrees != limitPitchDegrees->value()){
            bsData->limitPitchDegrees = limitPitchDegrees->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setLimitPitchDegrees(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setOffsetHeadingDegrees(){
    if (bsData){
        if (bsData->offsetHeadingDegrees != offsetHeadingDegrees->value()){
            bsData->offsetHeadingDegrees = offsetHeadingDegrees->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setOffsetHeadingDegrees(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setOffsetPitchDegrees(){
    if (bsData){
        if (bsData->offsetPitchDegrees != offsetPitchDegrees->value()){
            bsData->offsetPitchDegrees = offsetPitchDegrees->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setOffsetPitchDegrees(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setOnGain(){
    if (bsData){
        if (bsData->onGain != onGain->value()){
            bsData->onGain = onGain->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setOnGain(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setOffGain(){
    if (bsData){
        if (bsData->offGain != offGain->value()){
            bsData->offGain = offGain->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setOffGain(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setTargetLocation(){
    if (bsData){
        if (bsData->targetLocation != targetLocation->value()){
            bsData->targetLocation = targetLocation->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setTargetLocation(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setUserInfo(){
    if (bsData){
        if (bsData->userInfo != userInfo->value()){
            bsData->userInfo = userInfo->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setUserInfo(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setDirectAtCamera(){
    if (bsData){
        if (bsData->directAtCamera != directAtCamera->isChecked()){
            bsData->directAtCamera = directAtCamera->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setDirectAtCamera(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setDirectAtCameraX(){
    if (bsData){
        if (bsData->directAtCameraX != directAtCameraX->value()){
            bsData->directAtCameraX = directAtCameraX->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setDirectAtCameraX(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setDirectAtCameraY(){
    if (bsData){
        if (bsData->directAtCameraY != directAtCameraY->value()){
            bsData->directAtCameraY = directAtCameraY->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setDirectAtCameraY(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setDirectAtCameraZ(){
    if (bsData){
        if (bsData->directAtCameraZ != directAtCameraZ->value()){
            bsData->directAtCameraZ = directAtCameraZ->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setDirectAtCameraZ(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setActive(){
    if (bsData){
        if (bsData->active != active->isChecked()){
            bsData->active = active->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setActive(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::setCurrentHeadingOffset(){
    if (bsData){
        if (bsData->currentHeadingOffset != currentHeadingOffset->value()){
            bsData->currentHeadingOffset = currentHeadingOffset->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setCurrentHeadingOffset(): The data is NULL!!"));
    }
}
void BSDirectAtModifierUI::setCurrentPitchOffset(){
    if (bsData){
        if (bsData->currentPitchOffset != currentPitchOffset->value()){
            bsData->currentPitchOffset = currentPitchOffset->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setCurrentPitchOffset(): The data is NULL!!"));
    }
}
void BSDirectAtModifierUI::viewSelected(int row, int column){
    if (bsData){
        bool isProperty = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case ENABLE_ROW:
                if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "enable");
                break;
            case DIRECT_AT_TARGET_ROW:
                if (table->item(DIRECT_AT_TARGET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "directAtTarget");
                break;
            case SOURCE_BONE_INDEX_ROW:
                if (table->item(SOURCE_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "sourceBoneIndex");
                break;
            case START_BONE_INDEX_ROW:
                if (table->item(START_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "startBoneIndex");
                break;
            case END_BONE_INDEX_ROW:
                if (table->item(END_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "endBoneIndex");
                break;
            case LIMIT_HEADING_DEGREES_ROW:
                if (table->item(LIMIT_HEADING_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "limitHeadingDegrees");
                break;
            case LIMIT_PITCH_DEGREES_ROW:
                if (table->item(LIMIT_PITCH_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "limitPitchDegrees");
                break;
            case OFFSET_HEADING_DEGREES_ROW:
                if (table->item(OFFSET_HEADING_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "offsetHeadingDegrees");
                break;
            case OFFSET_PITCH_DEGREES_ROW:
                if (table->item(OFFSET_PITCH_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "offsetPitchDegrees");
                break;
            case ON_GAIN_ROW:
                if (table->item(ON_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "onGain");
                break;
            case OFF_GAIN_ROW:
                if (table->item(OFF_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "offGain");
                break;
            case TARGET_LOCATION_ROW:
                if (table->item(TARGET_LOCATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "targetLocation");
                break;
            case USER_INFO_ROW:
                if (table->item(USER_INFO_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "userInfo");
                break;
            case DIRECT_AT_CAMERA_ROW:
                if (table->item(DIRECT_AT_CAMERA_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "directAtCamera");
                break;
            case DIRECT_AT_CAMERA_X_ROW:
                if (table->item(DIRECT_AT_CAMERA_X_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "directAtCameraX");
                break;
            case DIRECT_AT_CAMERA_Y_ROW:
                if (table->item(DIRECT_AT_CAMERA_Y_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "directAtCameraY");
                break;
            case DIRECT_AT_CAMERA_Z_ROW:
                if (table->item(DIRECT_AT_CAMERA_Z_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "directAtCameraZ");
                break;
            case ACTIVE_ROW:
                if (table->item(ACTIVE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "active");
                break;
            case CURRENT_HEADING_OFFSET_ROW:
                if (table->item(CURRENT_HEADING_OFFSET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "currentHeadingOffset");
                break;
            case CURRENT_PITCH_OFFSET_ROW:
                if (table->item(CURRENT_PITCH_OFFSET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "currentPitchOffset");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void BSDirectAtModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
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
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("directAtTarget");
            if (bindIndex == index){
                table->item(DIRECT_AT_TARGET_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("sourceBoneIndex");
            if (bindIndex == index){
                table->item(SOURCE_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("startBoneIndex");
            if (bindIndex == index){
                table->item(START_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("endBoneIndex");
            if (bindIndex == index){
                table->item(END_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("limitHeadingDegrees");
            if (bindIndex == index){
                table->item(LIMIT_HEADING_DEGREES_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("limitPitchDegrees");
            if (bindIndex == index){
                table->item(LIMIT_PITCH_DEGREES_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("offsetHeadingDegrees");
            if (bindIndex == index){
                table->item(OFFSET_HEADING_DEGREES_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("offsetPitchDegrees");
            if (bindIndex == index){
                table->item(OFFSET_PITCH_DEGREES_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("onGain");
            if (bindIndex == index){
                table->item(ON_GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("offGain");
            if (bindIndex == index){
                table->item(OFF_GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("targetLocation");
            if (bindIndex == index){
                table->item(TARGET_LOCATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("userInfo");
            if (bindIndex == index){
                table->item(USER_INFO_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("directAtCamera");
            if (bindIndex == index){
                table->item(DIRECT_AT_CAMERA_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("directAtCameraX");
            if (bindIndex == index){
                table->item(DIRECT_AT_CAMERA_X_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("directAtCameraY");
            if (bindIndex == index){
                table->item(DIRECT_AT_CAMERA_Y_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("directAtCameraZ");
            if (bindIndex == index){
                table->item(DIRECT_AT_CAMERA_Z_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("active");
            if (bindIndex == index){
                table->item(ACTIVE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("currentHeadingOffset");
            if (bindIndex == index){
                table->item(CURRENT_HEADING_OFFSET_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("currentPitchOffset");
            if (bindIndex == index){
                table->item(CURRENT_PITCH_OFFSET_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool BSDirectAtModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BSDirectAtModifierUI::setBindingVariable(int index, const QString &name){
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
        case DIRECT_AT_TARGET_ROW:
            if (table->item(DIRECT_AT_TARGET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "directAtTarget", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case SOURCE_BONE_INDEX_ROW:
            if (table->item(SOURCE_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "sourceBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case START_BONE_INDEX_ROW:
            if (table->item(START_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "startBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case END_BONE_INDEX_ROW:
            if (table->item(END_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "endBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case LIMIT_HEADING_DEGREES_ROW:
            if (table->item(LIMIT_HEADING_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "limitHeadingDegrees", VARIABLE_TYPE_REAL, isProperty);
            break;
        case LIMIT_PITCH_DEGREES_ROW:
            if (table->item(LIMIT_PITCH_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "limitPitchDegrees", VARIABLE_TYPE_REAL, isProperty);
            break;
        case OFFSET_HEADING_DEGREES_ROW:
            if (table->item(OFFSET_HEADING_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "offsetHeadingDegrees", VARIABLE_TYPE_REAL, isProperty);
            break;
        case OFFSET_PITCH_DEGREES_ROW:
            if (table->item(OFFSET_PITCH_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "offsetPitchDegrees", VARIABLE_TYPE_REAL, isProperty);
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
        case TARGET_LOCATION_ROW:
            if (table->item(TARGET_LOCATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "targetLocation", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case USER_INFO_ROW:
            if (table->item(USER_INFO_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "userInfo", VARIABLE_TYPE_INT32, isProperty);
            break;
        case DIRECT_AT_CAMERA_ROW:
            if (table->item(DIRECT_AT_CAMERA_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "directAtCamera", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case DIRECT_AT_CAMERA_X_ROW:
            if (table->item(DIRECT_AT_CAMERA_X_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "directAtCameraX", VARIABLE_TYPE_REAL, isProperty);
            break;
        case DIRECT_AT_CAMERA_Y_ROW:
            if (table->item(DIRECT_AT_CAMERA_Y_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "directAtCameraY", VARIABLE_TYPE_REAL, isProperty);
            break;
        case DIRECT_AT_CAMERA_Z_ROW:
            if (table->item(DIRECT_AT_CAMERA_Z_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "directAtCameraZ", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ACTIVE_ROW:
            if (table->item(ACTIVE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "active", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case CURRENT_HEADING_OFFSET_ROW:
            if (table->item(CURRENT_HEADING_OFFSET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "currentHeadingOffset", VARIABLE_TYPE_REAL, isProperty);
            break;
        case CURRENT_PITCH_OFFSET_ROW:
            if (table->item(CURRENT_PITCH_OFFSET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "currentPitchOffset", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void BSDirectAtModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSDirectAtModifierUI::loadBinding(): The data is NULL!!"));
    }
}
