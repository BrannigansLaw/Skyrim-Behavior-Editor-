#include "handui.h"

#include "src/utility.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#define BASE_NUMBER_OF_ROWS 14

#define TARGET_POSITION_ROW 0
#define TARGET_ROTATION_ROW 1
#define TARGET_NORMAL_ROW 2
#define TARGET_HANDLE_ROW 3
#define TRANSFORM_ON_FRACTION_ROW 4
#define NORMAL_ON_FRACTION_ROW 5
#define FADE_IN_DURATION_ROW 6
#define FADE_OUT_DURATION_ROW 7
#define EXTRAPOLATION_TIME_STEP_ROW 8
#define HANDLE_CHANGE_SPEED_ROW 9
#define HANDLE_CHANGE_MODE_ROW 10
#define FIXUP_ROW 11
#define HAND_INDEX_ROW 12
#define ENABLE_ROW 13

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList HandUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

HandUI::HandUI()
    : file(nullptr),
      bsBoneIndex(-1),
      bsData(nullptr),
      parent(nullptr),
      topLyt(new QGridLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      targetPosition(new QuadVariableWidget),
      targetRotation(new QuadVariableWidget),
      targetNormal(new QuadVariableWidget),
      transformOnFraction(new DoubleSpinBox),
      normalOnFraction(new DoubleSpinBox),
      fadeInDuration(new DoubleSpinBox),
      fadeOutDuration(new DoubleSpinBox),
      extrapolationTimeStep(new DoubleSpinBox),
      handleChangeSpeed(new DoubleSpinBox),
      handleChangeMode(new ComboBox),
      fixUp(new CheckBox),
      handIndex(new ComboBox),
      enable(new CheckBox)
{
    setTitle("HandUI");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(HAND_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("index"));
    table->setItem(HAND_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(HAND_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(HAND_INDEX_ROW, VALUE_COLUMN, handIndex);
    table->setItem(TARGET_POSITION_ROW, NAME_COLUMN, new TableWidgetItem("targetPosition"));
    table->setItem(TARGET_POSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(TARGET_POSITION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TARGET_POSITION_ROW, VALUE_COLUMN, targetPosition);
    table->setItem(TARGET_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("targetRotation"));
    table->setItem(TARGET_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(TARGET_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TARGET_ROTATION_ROW, VALUE_COLUMN, targetRotation);
    table->setItem(TARGET_NORMAL_ROW, NAME_COLUMN, new TableWidgetItem("targetNormal"));
    table->setItem(TARGET_NORMAL_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(TARGET_NORMAL_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TARGET_NORMAL_ROW, VALUE_COLUMN, targetNormal);
    table->setItem(TARGET_HANDLE_ROW, NAME_COLUMN, new TableWidgetItem("targetHandle"));
    table->setItem(TARGET_HANDLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkHandle", Qt::AlignCenter));
    table->setItem(TARGET_HANDLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(TARGET_HANDLE_ROW, VALUE_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(TRANSFORM_ON_FRACTION_ROW, NAME_COLUMN, new TableWidgetItem("transformOnFraction"));
    table->setItem(TRANSFORM_ON_FRACTION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(TRANSFORM_ON_FRACTION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TRANSFORM_ON_FRACTION_ROW, VALUE_COLUMN, transformOnFraction);
    table->setItem(NORMAL_ON_FRACTION_ROW, NAME_COLUMN, new TableWidgetItem("normalOnFraction"));
    table->setItem(NORMAL_ON_FRACTION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(NORMAL_ON_FRACTION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(NORMAL_ON_FRACTION_ROW, VALUE_COLUMN, normalOnFraction);
    table->setItem(FADE_IN_DURATION_ROW, NAME_COLUMN, new TableWidgetItem("fadeInDuration"));
    table->setItem(FADE_IN_DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(FADE_IN_DURATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(FADE_IN_DURATION_ROW, VALUE_COLUMN, fadeInDuration);
    table->setItem(FADE_OUT_DURATION_ROW, NAME_COLUMN, new TableWidgetItem("fadeOutDuration"));
    table->setItem(FADE_OUT_DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(FADE_OUT_DURATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(FADE_OUT_DURATION_ROW, VALUE_COLUMN, fadeOutDuration);
    table->setItem(EXTRAPOLATION_TIME_STEP_ROW, NAME_COLUMN, new TableWidgetItem("extrapolationTimeStep"));
    table->setItem(EXTRAPOLATION_TIME_STEP_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(EXTRAPOLATION_TIME_STEP_ROW, VALUE_COLUMN, extrapolationTimeStep);
    table->setItem(HANDLE_CHANGE_SPEED_ROW, NAME_COLUMN, new TableWidgetItem("handleChangeSpeed"));
    table->setItem(HANDLE_CHANGE_SPEED_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(HANDLE_CHANGE_SPEED_ROW, VALUE_COLUMN, handleChangeSpeed);
    table->setItem(HANDLE_CHANGE_MODE_ROW, NAME_COLUMN, new TableWidgetItem("handleChangeMode"));
    table->setItem(HANDLE_CHANGE_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("HandleChangeMode", Qt::AlignCenter));
    table->setItem(HANDLE_CHANGE_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(HANDLE_CHANGE_MODE_ROW, VALUE_COLUMN, handleChangeMode);
    table->setItem(FIXUP_ROW, NAME_COLUMN, new TableWidgetItem("fixUp"));
    table->setItem(FIXUP_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(FIXUP_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(FIXUP_ROW, VALUE_COLUMN, fixUp);
    table->setItem(HAND_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("handIndex"));
    table->setItem(HAND_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(HAND_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(HAND_INDEX_ROW, VALUE_COLUMN, handIndex);
    table->setItem(ENABLE_ROW, NAME_COLUMN, new TableWidgetItem("enable"));
    table->setItem(ENABLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 6, 3);
    setLayout(topLyt);
    connectSignals();
}

void HandUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(targetPosition, SIGNAL(editingFinished()), this, SLOT(setTargetPosition()), Qt::UniqueConnection);
    connect(targetRotation, SIGNAL(editingFinished()), this, SLOT(setTargetRotation()), Qt::UniqueConnection);
    connect(targetNormal, SIGNAL(editingFinished()), this, SLOT(setTargetNormal()), Qt::UniqueConnection);
    connect(transformOnFraction, SIGNAL(editingFinished()), this, SLOT(setTransformOnFraction()), Qt::UniqueConnection);
    connect(normalOnFraction, SIGNAL(editingFinished()), this, SLOT(setNormalOnFraction()), Qt::UniqueConnection);
    connect(fadeInDuration, SIGNAL(editingFinished()), this, SLOT(setFadeInDuration()), Qt::UniqueConnection);
    connect(fadeOutDuration, SIGNAL(editingFinished()), this, SLOT(setFadeOutDuration()), Qt::UniqueConnection);
    connect(extrapolationTimeStep, SIGNAL(editingFinished()), this, SLOT(setExtrapolationTimeStep()), Qt::UniqueConnection);
    connect(handleChangeSpeed, SIGNAL(editingFinished()), this, SLOT(setHandleChangeSpeed()), Qt::UniqueConnection);
    connect(handleChangeMode, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHandleChangeMode(QString)), Qt::UniqueConnection);
    connect(fixUp, SIGNAL(released()), this, SLOT(setFixUp()), Qt::UniqueConnection);
    connect(handIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setHandIndex(int)), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void HandUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(targetPosition, SIGNAL(editingFinished()), this, SLOT(setTargetPosition()));
    disconnect(targetRotation, SIGNAL(editingFinished()), this, SLOT(setTargetRotation()));
    disconnect(targetNormal, SIGNAL(editingFinished()), this, SLOT(setTargetNormal()));
    disconnect(transformOnFraction, SIGNAL(editingFinished()), this, SLOT(setTransformOnFraction()));
    disconnect(normalOnFraction, SIGNAL(editingFinished()), this, SLOT(setNormalOnFraction()));
    disconnect(fadeInDuration, SIGNAL(editingFinished()), this, SLOT(setFadeInDuration()));
    disconnect(fadeOutDuration, SIGNAL(editingFinished()), this, SLOT(setFadeOutDuration()));
    disconnect(extrapolationTimeStep, SIGNAL(editingFinished()), this, SLOT(setExtrapolationTimeStep()));
    disconnect(handleChangeSpeed, SIGNAL(editingFinished()), this, SLOT(setHandleChangeSpeed()));
    disconnect(handleChangeMode, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHandleChangeMode(QString)));
    disconnect(fixUp, SIGNAL(released()), this, SLOT(setFixUp()));
    disconnect(handIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setHandIndex(int)));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void HandUI::loadData(BehaviorFile *parentFile, hkbHandIkControlsModifier::hkHand *bon, hkbHandIkControlsModifier *par, int ind){
    disconnectSignals();
    if (parentFile && bon && par && ind > -1){
        parent = par;
        bsBoneIndex = ind;
        file = parentFile;
        bsData = bon;
        targetRotation->setValue(bsData->controlData.targetRotation);
        targetPosition->setValue(bsData->controlData.targetPosition);
        targetNormal->setValue(bsData->controlData.targetNormal);
        transformOnFraction->setValue(bsData->controlData.transformOnFraction);
        normalOnFraction->setValue(bsData->controlData.normalOnFraction);
        fadeInDuration->setValue(bsData->controlData.fadeInDuration);
        fadeOutDuration->setValue(bsData->controlData.fadeOutDuration);
        extrapolationTimeStep->setValue(bsData->controlData.extrapolationTimeStep);
        handleChangeSpeed->setValue(bsData->controlData.handleChangeSpeed);
        if (handleChangeMode->count() == 0){
            handleChangeMode->insertItems(0, parent->HandleChangeMode);
        }
        handleChangeMode->setCurrentIndex(parent->HandleChangeMode.indexOf(bsData->controlData.handleChangeMode));
        fixUp->setChecked(bsData->controlData.fixUp);
        QStringList boneNames("None");
        if (handIndex->count() == 0){
            boneNames = boneNames + file->getRigBoneNames();
            handIndex->insertItems(0, boneNames);
        }
        handIndex->setCurrentIndex(bsData->handIndex + 1);
        enable->setChecked(bsData->enable);
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (varBind){
            loadBinding(TARGET_POSITION_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/targetPosition");
            loadBinding(TARGET_ROTATION_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/boneIndex");
            loadBinding(TARGET_NORMAL_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/targetNormal");
            loadBinding(TRANSFORM_ON_FRACTION_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/transformOnFraction");
            loadBinding(NORMAL_ON_FRACTION_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/normalOnFraction");
            loadBinding(FADE_IN_DURATION_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/fadeInDuration");
            loadBinding(FADE_OUT_DURATION_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/fadeOutDuration");
            loadBinding(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/extrapolationTimeStep");
            loadBinding(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/handleChangeSpeed");
            loadBinding(FIXUP_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/fixUp");
            loadBinding(HAND_INDEX_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/handIndex");
            loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "hands:"+QString::number(bsBoneIndex)+"/enable");
        }else{
            table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(TARGET_ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(TARGET_NORMAL_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(TRANSFORM_ON_FRACTION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(NORMAL_ON_FRACTION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(FADE_IN_DURATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(FADE_OUT_DURATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(FIXUP_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(HAND_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::loadData(): Behavior file, bind or event data is null!!!");
    }
    connectSignals();
}

void HandUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(file)->getCharacterPropertyNameAt(index, true);
                    table->item(row, colunm)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(file)->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            FATAL_RUNTIME_ERROR("HandUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::loadBinding(): The data is nullptr!!");
    }
}

bool HandUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(file)->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(file)->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(file);
                parent->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            file->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void HandUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case TARGET_POSITION_ROW:
            if (table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/targetPosition", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case TARGET_ROTATION_ROW:
            if (table->item(TARGET_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/targetRotation", VARIABLE_TYPE_QUATERNION, isProperty);
            break;
        case TARGET_NORMAL_ROW:
            if (table->item(TARGET_NORMAL_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/targetNormal", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case TRANSFORM_ON_FRACTION_ROW:
            if (table->item(TRANSFORM_ON_FRACTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/transformOnFraction", VARIABLE_TYPE_REAL, isProperty);
            break;
        case NORMAL_ON_FRACTION_ROW:
            if (table->item(NORMAL_ON_FRACTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/normalOnFraction", VARIABLE_TYPE_REAL, isProperty);
            break;
        case FADE_IN_DURATION_ROW:
            if (table->item(FADE_IN_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/fadeInDuration", VARIABLE_TYPE_REAL, isProperty);
            break;
        case FADE_OUT_DURATION_ROW:
            if (table->item(FADE_OUT_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/fadeOutDuration", VARIABLE_TYPE_REAL, isProperty);
            break;
        case EXTRAPOLATION_TIME_STEP_ROW:
            if (table->item(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/extrapolationTimeStep", VARIABLE_TYPE_REAL, isProperty);
            break;
        case HANDLE_CHANGE_SPEED_ROW:
            if (table->item(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/handleChangeSpeed", VARIABLE_TYPE_REAL, isProperty);
            break;
        case FIXUP_ROW:
            if (table->item(FIXUP_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/fixUp", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case HAND_INDEX_ROW:
            if (table->item(HAND_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/handIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case ENABLE_ROW:
            if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hands:"+QString::number(bsBoneIndex)+"/enable", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        file->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("HandUI::setBindingVariable(): The data is nullptr!!");
    }
}

void HandUI::setTargetPosition(){
    if (bsData && file){
        if (bsData->controlData.targetPosition != targetPosition->value()){
            bsData->controlData.targetPosition = targetPosition->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::settargetPosition(): Behavior file or event data is null!!!");
    }
}

void HandUI::setTargetRotation(){
    if (bsData && file){
        if (bsData->controlData.targetRotation != targetRotation->value()){
            bsData->controlData.targetRotation = targetRotation->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::settargetRotation(): Behavior file or event data is null!!!");
    }
}

void HandUI::setTargetNormal(){
    if (bsData && file){
        if (bsData->controlData.targetNormal != targetNormal->value()){
            bsData->controlData.targetNormal = targetNormal->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::settargetNormal(): Behavior file or event data is null!!!");
    }
}

void HandUI::setTransformOnFraction(){
    if (bsData && file){
        if (bsData->controlData.transformOnFraction != transformOnFraction->value()){
            bsData->controlData.transformOnFraction = transformOnFraction->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::settransformOnFraction(): Behavior file or event data is null!!!");
    }
}

void HandUI::setNormalOnFraction(){
    if (bsData && file){
        if (bsData->controlData.normalOnFraction != normalOnFraction->value()){
            bsData->controlData.normalOnFraction = normalOnFraction->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::setnormalOnFraction(): Behavior file or event data is null!!!");
    }
}

void HandUI::setFadeInDuration(){
    if (bsData && file){
        if (bsData->controlData.fadeInDuration != fadeInDuration->value()){
            bsData->controlData.fadeInDuration = fadeInDuration->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::setFadeInDuration(): Behavior file or event data is null!!!");
    }
}

void HandUI::setFadeOutDuration(){
    if (bsData && file){
        if (bsData->controlData.fadeOutDuration != fadeOutDuration->value()){
            bsData->controlData.fadeOutDuration = fadeOutDuration->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::setFadeOutDuration(): Behavior file or event data is null!!!");
    }
}

void HandUI::setExtrapolationTimeStep(){
    if (bsData && file){
        if (bsData->controlData.extrapolationTimeStep != extrapolationTimeStep->value()){
            bsData->controlData.extrapolationTimeStep = extrapolationTimeStep->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::setExtrapolationTimeStep(): Behavior file or event data is null!!!");
    }
}

void HandUI::setHandleChangeSpeed(){
    if (bsData && file){
        if (bsData->controlData.handleChangeSpeed != handleChangeSpeed->value()){
            bsData->controlData.handleChangeSpeed = handleChangeSpeed->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::setHandleChangeSpeed(): Behavior file or event data is null!!!");
    }
}

void HandUI::setHandleChangeMode(const QString &mode){
    if (bsData && file){
        bsData->controlData.handleChangeMode = mode;
        file->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("HandUI::setHandleChangeSpeed(): Behavior file or event data is null!!!");
    }
}

void HandUI::setFixUp(){
    if (bsData && file){
        if (bsData->controlData.fixUp != fixUp->isChecked()){
            bsData->controlData.fixUp = fixUp->isChecked();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::setFixUp(): Behavior file or event data is null!!!");
    }
}

void HandUI::setHandIndex(int index){
    if (bsData && file){
        bsData->handIndex = index - 1;
        file->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("HandUI::setindex(): Behavior file or event data is null!!!");
    }
}

void HandUI::setEnable(){
    if (bsData && file){
        bsData->enable = enable->isChecked();
        file->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("HandUI::setEnable(): Behavior file or event data is null!!!");
    }
}

void HandUI::viewSelectedChild(int row, int column){
    if (bsData){
        bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case TARGET_POSITION_ROW:
                if (table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/targetPosition");
                break;
            case TARGET_ROTATION_ROW:
                if (table->item(TARGET_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/targetRotation");
                break;
            case TARGET_NORMAL_ROW:
                if (table->item(TARGET_NORMAL_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/targetNormal");
                break;
            case TRANSFORM_ON_FRACTION_ROW:
                if (table->item(TRANSFORM_ON_FRACTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/transformOnFraction");
                break;
            case NORMAL_ON_FRACTION_ROW:
                if (table->item(NORMAL_ON_FRACTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/normalOnFraction");
                break;
            case FADE_IN_DURATION_ROW:
                if (table->item(FADE_IN_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/fadeInDuration");
                break;
            case FADE_OUT_DURATION_ROW:
                if (table->item(FADE_OUT_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/fadeOutDuration");
                break;
            case EXTRAPOLATION_TIME_STEP_ROW:
                if (table->item(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/extrapolationTimeStep");
                break;
            case HANDLE_CHANGE_SPEED_ROW:
                if (table->item(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/handleChangeSpeed");
                break;
            case FIXUP_ROW:
                if (table->item(FIXUP_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/fixUp");
                break;
            case HAND_INDEX_ROW:
                if (table->item(HAND_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/handIndex");
                break;
            case ENABLE_ROW:
                if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "hands:"+QString::number(bsBoneIndex)+"/enable");
                break;
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void HandUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (parent->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewProperties(0);
            }
        }else{
            if (parent->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::selectTableToView(): The data is nullptr!!");
    }
}

void HandUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("HandUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        //index--;
        bind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/targetPosition");
            if (bindIndex == index){
                table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/targetRotation");
            if (bindIndex == index){
                table->item(TARGET_ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/targetNormal");
            if (bindIndex == index){
                table->item(TARGET_NORMAL_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/transformOnFraction");
            if (bindIndex == index){
                table->item(TRANSFORM_ON_FRACTION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/normalOnFraction");
            if (bindIndex == index){
                table->item(NORMAL_ON_FRACTION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/fadeInDuration");
            if (bindIndex == index){
                table->item(FADE_IN_DURATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/fadeOutDuration");
            if (bindIndex == index){
                table->item(FADE_OUT_DURATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/extrapolationTimeStep");
            if (bindIndex == index){
                table->item(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/handleChangeSpeed");
            if (bindIndex == index){
                table->item(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/fixUp");
            if (bindIndex == index){
                table->item(FIXUP_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/handIndex");
            if (bindIndex == index){
                table->item(HAND_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hands:"+QString::number(bsBoneIndex)+"/enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("HandUI::variableRenamed(): The data is nullptr!!");
    }
}
