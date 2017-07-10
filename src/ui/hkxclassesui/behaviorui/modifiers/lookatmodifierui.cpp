#include "lookatmodifierui.h"


#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbLookAtModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 20

#define NAME_ROW 0
#define ENABLE_ROW 1
#define TARGET_WS_ROW 2
#define HEAD_FORWARD_LS_ROW 3
#define NECK_FORWARD_LS_ROW 4
#define NECK_RIGHT_LS_ROW 5
#define EYE_POSITION_HS_ROW 6
#define NEW_TARGET_GAIN_ROW 7
#define ON_GAIN_ROW 8
#define OFF_GAIN_ROW 9
#define LIMIT_ANGLE_DEGREES_ROW 10
#define LIMIT_ANGLE_LEFT_ROW 11
#define LIMIT_ANGLE_RIGHT_ROW 12
#define LIMIT_ANGLE_UP_ROW 13
#define LIMIT_ANGLE_DOWN_ROW 14
#define HEAD_INDEX_ROW 15
#define NECK_INDEX_ROW 16
#define IS_ON_ROW 17
#define INDIVIDUAL_LIMITS_ON_ROW 18
#define IS_TARGET_INSIDE_LIMIT_CONE_ROW 19

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList LookAtModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

LookAtModifierUI::LookAtModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      targetWS(new QuadVariableWidget),
      headForwardLS(new QuadVariableWidget),
      neckForwardLS(new QuadVariableWidget),
      neckRightLS(new QuadVariableWidget),
      eyePositionHS(new QuadVariableWidget),
      newTargetGain(new DoubleSpinBox),
      onGain(new DoubleSpinBox),
      offGain(new DoubleSpinBox),
      limitAngleDegrees(new DoubleSpinBox),
      limitAngleLeft(new DoubleSpinBox),
      limitAngleRight(new DoubleSpinBox),
      limitAngleUp(new DoubleSpinBox),
      limitAngleDown(new DoubleSpinBox),
      headIndex(new ComboBox),
      neckIndex(new ComboBox),
      isOn(new CheckBox),
      individualLimitsOn(new CheckBox),
      isTargetInsideLimitCone(new CheckBox)
{
    setTitle("hkbLookAtModifier");
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
    table->setItem(TARGET_WS_ROW, NAME_COLUMN, new TableWidgetItem("targetWS"));
    table->setItem(TARGET_WS_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(TARGET_WS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TARGET_WS_ROW, VALUE_COLUMN, targetWS);
    table->setItem(HEAD_FORWARD_LS_ROW, NAME_COLUMN, new TableWidgetItem("headForwardLS"));
    table->setItem(HEAD_FORWARD_LS_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(HEAD_FORWARD_LS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(HEAD_FORWARD_LS_ROW, VALUE_COLUMN, headForwardLS);
    table->setItem(NECK_FORWARD_LS_ROW, NAME_COLUMN, new TableWidgetItem("neckForwardLS"));
    table->setItem(NECK_FORWARD_LS_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(NECK_FORWARD_LS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(NECK_FORWARD_LS_ROW, VALUE_COLUMN, neckForwardLS);
    table->setItem(NECK_RIGHT_LS_ROW, NAME_COLUMN, new TableWidgetItem("neckRightLS"));
    table->setItem(NECK_RIGHT_LS_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(NECK_RIGHT_LS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(NECK_RIGHT_LS_ROW, VALUE_COLUMN, neckRightLS);
    table->setItem(EYE_POSITION_HS_ROW, NAME_COLUMN, new TableWidgetItem("eyePositionHS"));
    table->setItem(EYE_POSITION_HS_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(EYE_POSITION_HS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(EYE_POSITION_HS_ROW, VALUE_COLUMN, eyePositionHS);
    table->setItem(NEW_TARGET_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("newTargetGain"));
    table->setItem(NEW_TARGET_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(NEW_TARGET_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(NEW_TARGET_GAIN_ROW, VALUE_COLUMN, newTargetGain);
    table->setItem(ON_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("onGain"));
    table->setItem(ON_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ON_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ON_GAIN_ROW, VALUE_COLUMN, onGain);
    table->setItem(OFF_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("offGain"));
    table->setItem(OFF_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(OFF_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(OFF_GAIN_ROW, VALUE_COLUMN, offGain);
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, NAME_COLUMN, new TableWidgetItem("limitAngleDegrees"));
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LIMIT_ANGLE_DEGREES_ROW, VALUE_COLUMN, limitAngleDegrees);
    table->setItem(LIMIT_ANGLE_LEFT_ROW, NAME_COLUMN, new TableWidgetItem("limitAngleLeft"));
    table->setItem(LIMIT_ANGLE_LEFT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_ANGLE_LEFT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LIMIT_ANGLE_LEFT_ROW, VALUE_COLUMN, limitAngleLeft);
    table->setItem(LIMIT_ANGLE_RIGHT_ROW, NAME_COLUMN, new TableWidgetItem("limitAngleRight"));
    table->setItem(LIMIT_ANGLE_RIGHT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_ANGLE_RIGHT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LIMIT_ANGLE_RIGHT_ROW, VALUE_COLUMN, limitAngleRight);
    table->setItem(LIMIT_ANGLE_UP_ROW, NAME_COLUMN, new TableWidgetItem("limitAngleUp"));
    table->setItem(LIMIT_ANGLE_UP_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_ANGLE_UP_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LIMIT_ANGLE_UP_ROW, VALUE_COLUMN, limitAngleUp);
    table->setItem(LIMIT_ANGLE_DOWN_ROW, NAME_COLUMN, new TableWidgetItem("limitAngleDown"));
    table->setItem(LIMIT_ANGLE_DOWN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_ANGLE_DOWN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LIMIT_ANGLE_DOWN_ROW, VALUE_COLUMN, limitAngleDown);
    table->setItem(HEAD_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("headIndex"));
    table->setItem(HEAD_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(HEAD_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(HEAD_INDEX_ROW, VALUE_COLUMN, headIndex);
    table->setItem(NECK_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("neckIndex"));
    table->setItem(NECK_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(NECK_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(NECK_INDEX_ROW, VALUE_COLUMN, neckIndex);
    table->setItem(IS_ON_ROW, NAME_COLUMN, new TableWidgetItem("isOn"));
    table->setItem(IS_ON_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IS_ON_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IS_ON_ROW, VALUE_COLUMN, isOn);
    table->setItem(INDIVIDUAL_LIMITS_ON_ROW, NAME_COLUMN, new TableWidgetItem("individualLimitsOn"));
    table->setItem(INDIVIDUAL_LIMITS_ON_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(INDIVIDUAL_LIMITS_ON_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INDIVIDUAL_LIMITS_ON_ROW, VALUE_COLUMN, individualLimitsOn);
    table->setItem(IS_TARGET_INSIDE_LIMIT_CONE_ROW, NAME_COLUMN, new TableWidgetItem("isTargetInsideLimitCone"));
    table->setItem(IS_TARGET_INSIDE_LIMIT_CONE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IS_TARGET_INSIDE_LIMIT_CONE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IS_TARGET_INSIDE_LIMIT_CONE_ROW, VALUE_COLUMN, isTargetInsideLimitCone);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void LookAtModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(targetWS, SIGNAL(released()), this, SLOT(setTargetWS()), Qt::UniqueConnection);
    connect(headForwardLS, SIGNAL(currentIndexChanged(int)), this, SLOT(setHeadForwardLS(int)), Qt::UniqueConnection);
    connect(neckForwardLS, SIGNAL(currentIndexChanged(int)), this, SLOT(setNeckForwardLS(int)), Qt::UniqueConnection);
    connect(neckRightLS, SIGNAL(currentIndexChanged(int)), this, SLOT(setNeckRightLS(int)), Qt::UniqueConnection);
    connect(eyePositionHS, SIGNAL(editingFinished()), this, SLOT(setEyePositionHS()), Qt::UniqueConnection);
    connect(newTargetGain, SIGNAL(editingFinished()), this, SLOT(setNewTargetGain()), Qt::UniqueConnection);
    connect(onGain, SIGNAL(editingFinished()), this, SLOT(setOnGain()), Qt::UniqueConnection);
    connect(offGain, SIGNAL(editingFinished()), this, SLOT(setOffGain()), Qt::UniqueConnection);
    connect(limitAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitAngleDegrees()), Qt::UniqueConnection);
    connect(limitAngleLeft, SIGNAL(editingFinished()), this, SLOT(setLimitAngleLeft()), Qt::UniqueConnection);
    connect(limitAngleRight, SIGNAL(editingFinished()), this, SLOT(setLimitAngleRight()), Qt::UniqueConnection);
    connect(limitAngleUp, SIGNAL(editingFinished()), this, SLOT(setLimitAngleUp()), Qt::UniqueConnection);
    connect(limitAngleDown, SIGNAL(released()), this, SLOT(setLimitAngleDown()), Qt::UniqueConnection);
    connect(headIndex, SIGNAL(editingFinished()), this, SLOT(setHeadIndex()), Qt::UniqueConnection);
    connect(neckIndex, SIGNAL(editingFinished()), this, SLOT(setNeckIndex()), Qt::UniqueConnection);
    connect(isOn, SIGNAL(editingFinished()), this, SLOT(setIsOn()), Qt::UniqueConnection);
    connect(individualLimitsOn, SIGNAL(released()), this, SLOT(setIndividualLimitsOn()), Qt::UniqueConnection);
    connect(isTargetInsideLimitCone, SIGNAL(editingFinished()), this, SLOT(setIsTargetInsideLimitCone()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void LookAtModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(targetWS, SIGNAL(released()), this, SLOT(setTargetWS()));
    disconnect(headForwardLS, SIGNAL(currentIndexChanged(int)), this, SLOT(setHeadForwardLS(int)));
    disconnect(neckForwardLS, SIGNAL(currentIndexChanged(int)), this, SLOT(setNeckForwardLS(int)));
    disconnect(neckRightLS, SIGNAL(currentIndexChanged(int)), this, SLOT(setNeckRightLS(int)));
    disconnect(eyePositionHS, SIGNAL(editingFinished()), this, SLOT(setEyePositionHS()));
    disconnect(newTargetGain, SIGNAL(editingFinished()), this, SLOT(setNewTargetGain()));
    disconnect(onGain, SIGNAL(editingFinished()), this, SLOT(setOnGain()));
    disconnect(offGain, SIGNAL(editingFinished()), this, SLOT(setOffGain()));
    disconnect(limitAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitAngleDegrees()));
    disconnect(limitAngleLeft, SIGNAL(editingFinished()), this, SLOT(setLimitAngleLeft()));
    disconnect(limitAngleRight, SIGNAL(editingFinished()), this, SLOT(setLimitAngleRight()));
    disconnect(limitAngleUp, SIGNAL(editingFinished()), this, SLOT(setLimitAngleUp()));
    disconnect(limitAngleDown, SIGNAL(released()), this, SLOT(setLimitAngleDown()));
    disconnect(headIndex, SIGNAL(editingFinished()), this, SLOT(setHeadIndex()));
    disconnect(neckIndex, SIGNAL(editingFinished()), this, SLOT(setNeckIndex()));
    disconnect(isOn, SIGNAL(editingFinished()), this, SLOT(setIsOn()));
    disconnect(individualLimitsOn, SIGNAL(released()), this, SLOT(setIndividualLimitsOn()));
    disconnect(isTargetInsideLimitCone, SIGNAL(editingFinished()), this, SLOT(setIsTargetInsideLimitCone()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void LookAtModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void LookAtModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_LOOK_AT_MODIFIER){
            QStringList boneNames("None");
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<hkbLookAtModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            targetWS->setValue(bsData->targetWS);
            headForwardLS->setValue(bsData->headForwardLS);
            neckForwardLS->setValue(bsData->neckForwardLS);
            neckRightLS->setValue(bsData->neckRightLS);
            eyePositionHS->setValue(bsData->eyePositionHS);
            newTargetGain->setValue(bsData->newTargetGain);
            onGain->setValue(bsData->onGain);
            offGain->setValue(bsData->offGain);
            limitAngleDegrees->setValue(bsData->onGain);
            limitAngleLeft->setValue(bsData->limitAngleLeft);
            limitAngleRight->setValue(bsData->limitAngleRight);
            limitAngleUp->setValue(bsData->limitAngleUp);
            limitAngleDown->setValue(bsData->limitAngleDown);
            if (headIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                headIndex->insertItems(0, boneNames);
            }
            headIndex->setCurrentIndex(bsData->headIndex + 1);
            if (neckIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                neckIndex->insertItems(0, boneNames);
            }
            neckIndex->setCurrentIndex(bsData->neckIndex + 1);
            isOn->setChecked(bsData->isOn);
            individualLimitsOn->setChecked(bsData->individualLimitsOn);
            isTargetInsideLimitCone->setChecked(bsData->isTargetInsideLimitCone);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(TARGET_WS_ROW, BINDING_COLUMN, varBind, "targetWS");
                loadBinding(HEAD_FORWARD_LS_ROW, BINDING_COLUMN, varBind, "headForwardLS");
                loadBinding(NECK_FORWARD_LS_ROW, BINDING_COLUMN, varBind, "neckForwardLS");
                loadBinding(NECK_RIGHT_LS_ROW, BINDING_COLUMN, varBind, "neckRightLS");
                loadBinding(EYE_POSITION_HS_ROW, BINDING_COLUMN, varBind, "eyePositionHS");
                loadBinding(NEW_TARGET_GAIN_ROW, BINDING_COLUMN, varBind, "newTargetGain");
                loadBinding(ON_GAIN_ROW, BINDING_COLUMN, varBind, "onGain");
                loadBinding(OFF_GAIN_ROW, BINDING_COLUMN, varBind, "offGain");
                loadBinding(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN, varBind, "limitAngleDegrees");
                loadBinding(LIMIT_ANGLE_LEFT_ROW, BINDING_COLUMN, varBind, "limitAngleLeft");
                loadBinding(LIMIT_ANGLE_RIGHT_ROW, BINDING_COLUMN, varBind, "limitAngleRight");
                loadBinding(LIMIT_ANGLE_UP_ROW, BINDING_COLUMN, varBind, "limitAngleUp");
                loadBinding(LIMIT_ANGLE_DOWN_ROW, BINDING_COLUMN, varBind, "limitAngleDown");
                loadBinding(HEAD_INDEX_ROW, BINDING_COLUMN, varBind, "headIndex");
                loadBinding(NECK_INDEX_ROW, BINDING_COLUMN, varBind, "neckIndex");
                loadBinding(IS_ON_ROW, BINDING_COLUMN, varBind, "isOn");
                loadBinding(INDIVIDUAL_LIMITS_ON_ROW, BINDING_COLUMN, varBind, "individualLimitsOn");
                loadBinding(IS_TARGET_INSIDE_LIMIT_CONE_ROW, BINDING_COLUMN, varBind, "isTargetInsideLimitCone");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TARGET_WS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(HEAD_FORWARD_LS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(NECK_FORWARD_LS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(NECK_RIGHT_LS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(EYE_POSITION_HS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(NEW_TARGET_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ON_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(OFF_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LIMIT_ANGLE_LEFT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LIMIT_ANGLE_RIGHT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LIMIT_ANGLE_UP_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LIMIT_ANGLE_DOWN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(HEAD_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(NECK_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(IS_ON_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(INDIVIDUAL_LIMITS_ON_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(IS_TARGET_INSIDE_LIMIT_CONE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void LookAtModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setName(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setEnable(){
    if (bsData){
        if (bsData->enable != enable->isChecked()){
            bsData->enable = enable->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setEnable(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setTargetWS(){
    if (bsData){
        if (bsData->targetWS != targetWS->value()){
            bsData->targetWS = targetWS->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::settargetWS(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setHeadForwardLS(){
    if (bsData){
        if (bsData->headForwardLS != headForwardLS->value()){
            bsData->headForwardLS = headForwardLS->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setheadForwardLS(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setNeckForwardLS(){
    if (bsData){
        if (bsData->neckForwardLS != neckForwardLS->value()){
            bsData->neckForwardLS = neckForwardLS->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setneckForwardLS(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setNeckRightLS(){
    if (bsData){
        if (bsData->neckRightLS != neckRightLS->value()){
            bsData->neckRightLS = neckRightLS->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setneckRightLS(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setEyePositionHS(){
    if (bsData){
        if (bsData->eyePositionHS != eyePositionHS->value()){
            bsData->eyePositionHS = eyePositionHS->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::seteyePositionHS(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setNewTargetGain(){
    if (bsData){
        if (bsData->newTargetGain != newTargetGain->value()){
            bsData->newTargetGain = newTargetGain->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setnewTargetGain(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setOnGain(){
    if (bsData){
        if (bsData->onGain != onGain->value()){
            bsData->onGain = onGain->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setonGain(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setOffGain(){
    if (bsData){
        if (bsData->offGain != offGain->value()){
            bsData->offGain = offGain->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setoffGain(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setLimitAngleDegrees(){
    if (bsData){
        if (bsData->limitAngleDegrees != limitAngleDegrees->value()){
            bsData->limitAngleDegrees = limitAngleDegrees->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setLimitAngleDegrees(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setLimitAngleLeft(){
    if (bsData){
        if (bsData->limitAngleLeft != limitAngleLeft->value()){
            bsData->limitAngleLeft = limitAngleLeft->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setlimitAngleLeft(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setLimitAngleRight(){
    if (bsData){
        if (bsData->limitAngleRight != limitAngleRight->value()){
            bsData->limitAngleRight = limitAngleRight->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setlimitAngleRight(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setLimitAngleUp(){
    if (bsData){
        if (bsData->limitAngleUp != limitAngleUp->value()){
            bsData->limitAngleUp = limitAngleUp->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setlimitAngleUp(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setLimitAngleDown(){
    if (bsData){
        if (bsData->limitAngleDown != limitAngleDown->value()){
            bsData->limitAngleDown = limitAngleDown->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setlimitAngleDown(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setHeadIndex(int index){
    if (bsData){
        bsData->headIndex = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setheadIndex(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setNeckIndex(int index){
    if (bsData){
        bsData->neckIndex = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setneckIndex(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setIsOn(){
    if (bsData){
        if (bsData->isOn != isOn->isChecked()){
            bsData->isOn = isOn->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setisOn(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setIndividualLimitsOn(){
    if (bsData){
        if (bsData->individualLimitsOn != individualLimitsOn->isChecked()){
            bsData->individualLimitsOn = individualLimitsOn->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setindividualLimitsOn(): The data is NULL!!"));
    }
}

void LookAtModifierUI::setIsTargetInsideLimitCone(){
    if (bsData){
        if (bsData->isTargetInsideLimitCone != isTargetInsideLimitCone->isChecked()){
            bsData->isTargetInsideLimitCone = isTargetInsideLimitCone->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setisTargetInsideLimitCone(): The data is NULL!!"));
    }
}

void LookAtModifierUI::viewSelected(int row, int column){
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
            case TARGET_WS_ROW:
                if (table->item(TARGET_WS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "targetWS");
                break;
            case HEAD_FORWARD_LS_ROW:
                if (table->item(HEAD_FORWARD_LS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "headForwardLS");
                break;
            case NECK_FORWARD_LS_ROW:
                if (table->item(NECK_FORWARD_LS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "neckForwardLS");
                break;
            case NECK_RIGHT_LS_ROW:
                if (table->item(NECK_RIGHT_LS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "neckRightLS");
                break;
            case EYE_POSITION_HS_ROW:
                if (table->item(EYE_POSITION_HS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "eyePositionHS");
                break;
            case NEW_TARGET_GAIN_ROW:
                if (table->item(NEW_TARGET_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "newTargetGain");
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
            case LIMIT_ANGLE_DEGREES_ROW:
                if (table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "limitAngleDegrees");
                break;
            case LIMIT_ANGLE_LEFT_ROW:
                if (table->item(LIMIT_ANGLE_LEFT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "limitAngleLeft");
                break;
            case LIMIT_ANGLE_RIGHT_ROW:
                if (table->item(LIMIT_ANGLE_RIGHT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "limitAngleRight");
                break;
            case LIMIT_ANGLE_UP_ROW:
                if (table->item(LIMIT_ANGLE_UP_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "limitAngleUp");
                break;
            case LIMIT_ANGLE_DOWN_ROW:
                if (table->item(LIMIT_ANGLE_DOWN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "limitAngleDown");
                break;
            case HEAD_INDEX_ROW:
                if (table->item(HEAD_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "headIndex");
                break;
            case NECK_INDEX_ROW:
                if (table->item(NECK_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "neckIndex");
                break;
            case IS_ON_ROW:
                if (table->item(IS_ON_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "isOn");
                break;
            case INDIVIDUAL_LIMITS_ON_ROW:
                if (table->item(INDIVIDUAL_LIMITS_ON_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "individualLimitsOn");
                break;
            case IS_TARGET_INSIDE_LIMIT_CONE_ROW:
                if (table->item(IS_TARGET_INSIDE_LIMIT_CONE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "isTargetInsideLimitCone");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void LookAtModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void LookAtModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("targetWS");
            if (bindIndex == index){
                table->item(TARGET_WS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("headForwardLS");
            if (bindIndex == index){
                table->item(HEAD_FORWARD_LS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("neckForwardLS");
            if (bindIndex == index){
                table->item(NECK_FORWARD_LS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("neckRightLS");
            if (bindIndex == index){
                table->item(NECK_RIGHT_LS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("eyePositionHS");
            if (bindIndex == index){
                table->item(EYE_POSITION_HS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("newTargetGain");
            if (bindIndex == index){
                table->item(NEW_TARGET_GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("onGain");
            if (bindIndex == index){
                table->item(ON_GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("offGain");
            if (bindIndex == index){
                table->item(OFF_GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("limitAngleDegrees");
            if (bindIndex == index){
                table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("limitAngleLeft");
            if (bindIndex == index){
                table->item(LIMIT_ANGLE_LEFT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("limitAngleRight");
            if (bindIndex == index){
                table->item(LIMIT_ANGLE_RIGHT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("limitAngleUp");
            if (bindIndex == index){
                table->item(LIMIT_ANGLE_UP_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("limitAngleDown");
            if (bindIndex == index){
                table->item(LIMIT_ANGLE_DOWN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("headIndex");
            if (bindIndex == index){
                table->item(HEAD_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("neckIndex");
            if (bindIndex == index){
                table->item(NECK_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("isOn");
            if (bindIndex == index){
                table->item(IS_ON_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("individualLimitsOn");
            if (bindIndex == index){
                table->item(INDIVIDUAL_LIMITS_ON_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("isTargetInsideLimitCone");
            if (bindIndex == index){
                table->item(IS_TARGET_INSIDE_LIMIT_CONE_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool LookAtModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void LookAtModifierUI::setBindingVariable(int index, const QString &name){
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
        case TARGET_WS_ROW:
            if (table->item(TARGET_WS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "targetWS", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case HEAD_FORWARD_LS_ROW:
            if (table->item(HEAD_FORWARD_LS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "headForwardLS", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case NECK_FORWARD_LS_ROW:
            if (table->item(NECK_FORWARD_LS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "neckForwardLS", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case NECK_RIGHT_LS_ROW:
            if (table->item(NECK_RIGHT_LS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "neckRightLS", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case EYE_POSITION_HS_ROW:
            if (table->item(EYE_POSITION_HS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "eyePositionHS", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case NEW_TARGET_GAIN_ROW:
            if (table->item(NEW_TARGET_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "newTargetGain", VARIABLE_TYPE_REAL, isProperty);
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
        case LIMIT_ANGLE_DEGREES_ROW:
            if (table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "limitAngleDegrees", VARIABLE_TYPE_REAL, isProperty);
            break;
        case LIMIT_ANGLE_LEFT_ROW:
            if (table->item(LIMIT_ANGLE_LEFT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "limitAngleLeft", VARIABLE_TYPE_REAL, isProperty);
            break;
        case LIMIT_ANGLE_RIGHT_ROW:
            if (table->item(LIMIT_ANGLE_RIGHT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "limitAngleRight", VARIABLE_TYPE_REAL, isProperty);
            break;
        case LIMIT_ANGLE_UP_ROW:
            if (table->item(LIMIT_ANGLE_UP_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "limitAngleUp", VARIABLE_TYPE_REAL, isProperty);
            break;
        case LIMIT_ANGLE_DOWN_ROW:
            if (table->item(LIMIT_ANGLE_DOWN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "limitAngleDown", VARIABLE_TYPE_REAL, isProperty);
            break;
        case HEAD_INDEX_ROW:
            if (table->item(HEAD_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "headIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case NECK_INDEX_ROW:
            if (table->item(NECK_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "neckIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case IS_ON_ROW:
            if (table->item(IS_ON_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "isOn", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case INDIVIDUAL_LIMITS_ON_ROW:
            if (table->item(INDIVIDUAL_LIMITS_ON_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "individualLimitsOn", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case IS_TARGET_INSIDE_LIMIT_CONE_ROW:
            if (table->item(IS_TARGET_INSIDE_LIMIT_CONE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "isTargetInsideLimitCone", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void LookAtModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("LookAtModifierUI::loadBinding(): The data is NULL!!"));
    }
}
