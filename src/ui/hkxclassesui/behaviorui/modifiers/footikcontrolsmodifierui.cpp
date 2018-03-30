#include "footikcontrolsmodifierui.h"

#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbFootIkControlsModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/hkxclassesui/behaviorui/legui.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include <QStackedLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 17

#define NAME_ROW 0
#define ENABLE_ROW 1
#define ON_OFF_GAIN_ROW 2
#define GROUND_ASCENDING_GAIN_ROW 3
#define GROUND_DESCENDING_GAIN_ROW 4
#define FOOT_PLANTED_GAIN_ROW 5
#define FOOT_RAISED_GAIN_ROW 6
#define FOOT_UNLOCK_GAIN_ROW 7
#define WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW 8
#define ERROR_UP_DOWN_BIAS_ROW 9
#define ALIGN_WORLD_FROM_MODEL_GAIN_ROW 10
#define HIP_ORIENTATION_GAIN_ROW 11
#define MAX_KNEE_ANGLE_DIFFERENCE_ROW 12
#define ANKLE_ORIENTATION_GAIN_ROW 13
#define ERROR_OUT_TRANSLATION_ROW 14
#define ALIGN_WITH_GROUND_ROTATION 15
#define ADD_LEG_ROW 16

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList FootIkControlsModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

FootIkControlsModifierUI::FootIkControlsModifierUI()
    : bsData(nullptr),
      legUI(new LegUI),
      groupBox(new QGroupBox),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      onOffGain(new DoubleSpinBox),
      groundAscendingGain(new DoubleSpinBox),
      groundDescendingGain(new DoubleSpinBox),
      footPlantedGain(new DoubleSpinBox),
      footRaisedGain(new DoubleSpinBox),
      footUnlockGain(new DoubleSpinBox),
      worldFromModelFeedbackGain(new DoubleSpinBox),
      errorUpDownBias(new DoubleSpinBox),
      alignWorldFromModelGain(new DoubleSpinBox),
      hipOrientationGain(new DoubleSpinBox),
      maxKneeAngleDifference(new DoubleSpinBox),
      ankleOrientationGain(new DoubleSpinBox),
      errorOutTranslation(new QuadVariableWidget),
      alignWithGroundRotation(new QuadVariableWidget)
{
    groupBox->setTitle("hkbFootIkControlsModifier");
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
    table->setItem(ON_OFF_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("onOffGain"));
    table->setItem(ON_OFF_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ON_OFF_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ON_OFF_GAIN_ROW, VALUE_COLUMN, onOffGain);
    table->setItem(GROUND_ASCENDING_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("groundAscendingGain"));
    table->setItem(GROUND_ASCENDING_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(GROUND_ASCENDING_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(GROUND_ASCENDING_GAIN_ROW, VALUE_COLUMN, groundAscendingGain);
    table->setItem(GROUND_DESCENDING_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("groundDescendingGain"));
    table->setItem(GROUND_DESCENDING_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(GROUND_DESCENDING_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(GROUND_DESCENDING_GAIN_ROW, VALUE_COLUMN, groundDescendingGain);
    table->setItem(FOOT_PLANTED_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("footPlantedGain"));
    table->setItem(FOOT_PLANTED_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(FOOT_PLANTED_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(FOOT_PLANTED_GAIN_ROW, VALUE_COLUMN, footPlantedGain);
    table->setItem(FOOT_RAISED_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("footRaisedGain"));
    table->setItem(FOOT_RAISED_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(FOOT_RAISED_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(FOOT_RAISED_GAIN_ROW, VALUE_COLUMN, footRaisedGain);
    table->setItem(FOOT_UNLOCK_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("footUnlockGain"));
    table->setItem(FOOT_UNLOCK_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(FOOT_UNLOCK_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(FOOT_UNLOCK_GAIN_ROW, VALUE_COLUMN, footUnlockGain);
    table->setItem(WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("worldFromModelFeedbackGain"));
    table->setItem(WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW, VALUE_COLUMN, worldFromModelFeedbackGain);
    table->setItem(ERROR_UP_DOWN_BIAS_ROW, NAME_COLUMN, new TableWidgetItem("errorUpDownBias"));
    table->setItem(ERROR_UP_DOWN_BIAS_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ERROR_UP_DOWN_BIAS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ERROR_UP_DOWN_BIAS_ROW, VALUE_COLUMN, errorUpDownBias);
    table->setItem(ALIGN_WORLD_FROM_MODEL_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("alignWorldFromModelGain"));
    table->setItem(ALIGN_WORLD_FROM_MODEL_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ALIGN_WORLD_FROM_MODEL_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ALIGN_WORLD_FROM_MODEL_GAIN_ROW, VALUE_COLUMN, alignWorldFromModelGain);
    table->setItem(HIP_ORIENTATION_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("hipOrientationGain"));
    table->setItem(HIP_ORIENTATION_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(HIP_ORIENTATION_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(HIP_ORIENTATION_GAIN_ROW, VALUE_COLUMN, hipOrientationGain);
    table->setItem(MAX_KNEE_ANGLE_DIFFERENCE_ROW, NAME_COLUMN, new TableWidgetItem("maxKneeAngleDifference"));
    table->setItem(MAX_KNEE_ANGLE_DIFFERENCE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MAX_KNEE_ANGLE_DIFFERENCE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MAX_KNEE_ANGLE_DIFFERENCE_ROW, VALUE_COLUMN, maxKneeAngleDifference);
    table->setItem(ANKLE_ORIENTATION_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("ankleOrientationGain"));
    table->setItem(ANKLE_ORIENTATION_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ANKLE_ORIENTATION_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ANKLE_ORIENTATION_GAIN_ROW, VALUE_COLUMN, ankleOrientationGain);
    table->setItem(ERROR_OUT_TRANSLATION_ROW, NAME_COLUMN, new TableWidgetItem("errorOutTranslation"));
    table->setItem(ERROR_OUT_TRANSLATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(ERROR_OUT_TRANSLATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ERROR_OUT_TRANSLATION_ROW, VALUE_COLUMN, errorOutTranslation);
    table->setItem(ALIGN_WITH_GROUND_ROTATION, NAME_COLUMN, new TableWidgetItem("alignWithGroundRotation"));
    table->setItem(ALIGN_WITH_GROUND_ROTATION, TYPE_COLUMN, new TableWidgetItem("hkQuaternion", Qt::AlignCenter));
    table->setItem(ALIGN_WITH_GROUND_ROTATION, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ALIGN_WITH_GROUND_ROTATION, VALUE_COLUMN, alignWithGroundRotation);
    table->setItem(ADD_LEG_ROW, NAME_COLUMN, new TableWidgetItem("Add Leg", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new Leg"));
    table->setItem(ADD_LEG_ROW, TYPE_COLUMN, new TableWidgetItem("hkLeg", Qt::AlignCenter));
    table->setItem(ADD_LEG_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Leg", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected Leg"));
    table->setItem(ADD_LEG_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Leg", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected Leg"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(legUI);
    connectSignals();
}

void FootIkControlsModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(onOffGain, SIGNAL(editingFinished()), this, SLOT(setonOffGain()), Qt::UniqueConnection);
    connect(groundAscendingGain, SIGNAL(editingFinished()), this, SLOT(setgroundAscendingGain()), Qt::UniqueConnection);
    connect(groundDescendingGain, SIGNAL(editingFinished()), this, SLOT(setgroundDescendingGain()), Qt::UniqueConnection);
    connect(footPlantedGain, SIGNAL(editingFinished()), this, SLOT(setfootPlantedGain()), Qt::UniqueConnection);
    connect(footRaisedGain, SIGNAL(editingFinished()), this, SLOT(setfootRaisedGain()), Qt::UniqueConnection);
    connect(footUnlockGain, SIGNAL(editingFinished()), this, SLOT(setfootUnlockGain()), Qt::UniqueConnection);
    connect(worldFromModelFeedbackGain, SIGNAL(editingFinished()), this, SLOT(setworldFromModelFeedbackGain()), Qt::UniqueConnection);
    connect(errorUpDownBias, SIGNAL(editingFinished()), this, SLOT(seterrorUpDownBias()), Qt::UniqueConnection);
    connect(alignWorldFromModelGain, SIGNAL(editingFinished()), this, SLOT(setalignWorldFromModelGain()), Qt::UniqueConnection);
    connect(hipOrientationGain, SIGNAL(editingFinished()), this, SLOT(sethipOrientationGain()), Qt::UniqueConnection);
    connect(maxKneeAngleDifference, SIGNAL(editingFinished()), this, SLOT(setmaxKneeAngleDifference()), Qt::UniqueConnection);
    connect(ankleOrientationGain, SIGNAL(editingFinished()), this, SLOT(setankleOrientationGain()), Qt::UniqueConnection);
    connect(errorOutTranslation, SIGNAL(editingFinished()), this, SLOT(seterrorOutTranslation()), Qt::UniqueConnection);
    connect(alignWithGroundRotation, SIGNAL(editingFinished()), this, SLOT(setalignWithGroundRotation()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(legUI, SIGNAL(viewEvents(int,QString,QStringList)), this, SIGNAL(viewEvents(int,QString,QStringList)), Qt::UniqueConnection);
    connect(legUI, SIGNAL(viewVariables(int,QString,QStringList)), this, SIGNAL(viewVariables(int,QString,QStringList)), Qt::UniqueConnection);
    connect(legUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
}

void FootIkControlsModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(onOffGain, SIGNAL(editingFinished()), this, SLOT(setonOffGain()));
    disconnect(groundAscendingGain, SIGNAL(editingFinished()), this, SLOT(setgroundAscendingGain()));
    disconnect(groundDescendingGain, SIGNAL(editingFinished()), this, SLOT(setgroundDescendingGain()));
    disconnect(footPlantedGain, SIGNAL(editingFinished()), this, SLOT(setfootPlantedGain()));
    disconnect(footRaisedGain, SIGNAL(editingFinished()), this, SLOT(setfootRaisedGain()));
    disconnect(footUnlockGain, SIGNAL(editingFinished()), this, SLOT(setfootUnlockGain()));
    disconnect(worldFromModelFeedbackGain, SIGNAL(editingFinished()), this, SLOT(setworldFromModelFeedbackGain()));
    disconnect(errorUpDownBias, SIGNAL(editingFinished()), this, SLOT(seterrorUpDownBias()));
    disconnect(alignWorldFromModelGain, SIGNAL(editingFinished()), this, SLOT(setalignWorldFromModelGain()));
    disconnect(hipOrientationGain, SIGNAL(editingFinished()), this, SLOT(sethipOrientationGain()));
    disconnect(maxKneeAngleDifference, SIGNAL(editingFinished()), this, SLOT(setmaxKneeAngleDifference()));
    disconnect(ankleOrientationGain, SIGNAL(editingFinished()), this, SLOT(setankleOrientationGain()));
    disconnect(errorOutTranslation, SIGNAL(editingFinished()), this, SLOT(seterrorOutTranslation()));
    disconnect(alignWithGroundRotation, SIGNAL(editingFinished()), this, SLOT(setalignWithGroundRotation()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(legUI, SIGNAL(viewEvents(int,QString,QStringList)), this, SIGNAL(viewEvents(int,QString,QStringList)));
    disconnect(legUI, SIGNAL(viewVariables(int,QString,QStringList)), this, SIGNAL(viewVariables(int,QString,QStringList)));
    disconnect(legUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
}

void FootIkControlsModifierUI::addLeg(){
    if (bsData){
        bsData->legs.append(hkbFootIkControlsModifier::hkLeg());
        bsData->getParentFile()->setIsChanged(true);
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::addRange(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::removeLeg(int index){
    if (bsData){
        if (!bsData->legs.isEmpty()){
            if (index < bsData->legs.size() && index >= 0){
                bsData->legs.removeAt(index);
            }else{
                WARNING_MESSAGE("FootIkControlsModifierUI::removeExpression(): Invalid row index selected!!");
                return;
            }
            bsData->getParentFile()->setIsChanged(true);
            loadDynamicTableRows();
        }else{
            WARNING_MESSAGE("FootIkControlsModifierUI::removeExpression(): Ranges is empty!!");
            return;
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::removeExpression(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    if (data){
        if (data->getSignature() == HKB_FOOT_IK_CONTROLS_MODIFIER){
            hkbVariableBindingSet *varBind = nullptr;
            bsData = static_cast<hkbFootIkControlsModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            onOffGain->setValue(bsData->gains.onOffGain);
            groundAscendingGain->setValue(bsData->gains.groundAscendingGain);
            groundDescendingGain->setValue(bsData->gains.groundDescendingGain);
            footPlantedGain->setValue(bsData->gains.footPlantedGain);
            footRaisedGain->setValue(bsData->gains.footRaisedGain);
            footUnlockGain->setValue(bsData->gains.footUnlockGain);
            worldFromModelFeedbackGain->setValue(bsData->gains.worldFromModelFeedbackGain);
            errorUpDownBias->setValue(bsData->gains.errorUpDownBias);
            alignWorldFromModelGain->setValue(bsData->gains.alignWorldFromModelGain);
            hipOrientationGain->setValue(bsData->gains.hipOrientationGain);
            maxKneeAngleDifference->setValue(bsData->gains.maxKneeAngleDifference);
            ankleOrientationGain->setValue(bsData->gains.ankleOrientationGain);
            errorOutTranslation->setValue(bsData->errorOutTranslation);
            alignWithGroundRotation->setValue(bsData->alignWithGroundRotation);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(ON_OFF_GAIN_ROW, BINDING_COLUMN, varBind, "onOffGain");
                loadBinding(GROUND_ASCENDING_GAIN_ROW, BINDING_COLUMN, varBind, "groundAscendingGain");
                loadBinding(GROUND_DESCENDING_GAIN_ROW, BINDING_COLUMN, varBind, "groundDescendingGain");
                loadBinding(FOOT_PLANTED_GAIN_ROW, BINDING_COLUMN, varBind, "footPlantedGain");
                loadBinding(FOOT_RAISED_GAIN_ROW, BINDING_COLUMN, varBind, "footRaisedGain");
                loadBinding(FOOT_UNLOCK_GAIN_ROW, BINDING_COLUMN, varBind, "footUnlockGain");
                loadBinding(WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW, BINDING_COLUMN, varBind, "worldFromModelFeedbackGain");
                loadBinding(ERROR_UP_DOWN_BIAS_ROW, BINDING_COLUMN, varBind, "errorUpDownBias");
                loadBinding(ALIGN_WORLD_FROM_MODEL_GAIN_ROW, BINDING_COLUMN, varBind, "alignWorldFromModelGain");
                loadBinding(HIP_ORIENTATION_GAIN_ROW, BINDING_COLUMN, varBind, "hipOrientationGain");
                loadBinding(MAX_KNEE_ANGLE_DIFFERENCE_ROW, BINDING_COLUMN, varBind, "maxKneeAngleDifference");
                loadBinding(ANKLE_ORIENTATION_GAIN_ROW, BINDING_COLUMN, varBind, "ankleOrientationGain");
                loadBinding(ERROR_OUT_TRANSLATION_ROW, BINDING_COLUMN, varBind, "errorOutTranslation");
                loadBinding(ALIGN_WITH_GROUND_ROTATION, BINDING_COLUMN, varBind, "alignWithGroundRotation");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ON_OFF_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(GROUND_ASCENDING_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(GROUND_DESCENDING_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(FOOT_PLANTED_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(FOOT_RAISED_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(FOOT_UNLOCK_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ERROR_UP_DOWN_BIAS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ALIGN_WORLD_FROM_MODEL_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(HIP_ORIENTATION_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(MAX_KNEE_ANGLE_DIFFERENCE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ANKLE_ORIENTATION_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ERROR_OUT_TRANSLATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ALIGN_WITH_GROUND_ROTATION, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
            loadDynamicTableRows();
        }else{
            FATAL_RUNTIME_ERROR(QString("FootIkControlsModifierUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::loadData(): Attempting to load a null pointer!!");
    }
    connectSignals();
}

void FootIkControlsModifierUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_LEG_ROW + bsData->getNumberOfLegs() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        for (int i = ADD_LEG_ROW + 1, j = 0; j < bsData->getNumberOfLegs(); i++, j++){
            setRowItems(i, "Leg "+QString::number(j), "hkLeg", "Remove", "Edit", "Double click to remove this Leg", "Double click to edit this Leg");
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::loadDynamicTableRows(): The data is nullptr!!");
    }
    //table->setSortingEnabled(true);
}

void FootIkControlsModifierUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
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

bool FootIkControlsModifierUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->variableBindingSet = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void FootIkControlsModifierUI::setBindingVariable(int index, const QString & name){
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
        case ON_OFF_GAIN_ROW:
            if (table->item(ON_OFF_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "onOffGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case GROUND_ASCENDING_GAIN_ROW:
            if (table->item(GROUND_ASCENDING_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "groundAscendingGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case GROUND_DESCENDING_GAIN_ROW:
            if (table->item(GROUND_DESCENDING_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "groundDescendingGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case FOOT_PLANTED_GAIN_ROW:
            if (table->item(FOOT_PLANTED_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "footPlantedGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case FOOT_RAISED_GAIN_ROW:
            if (table->item(FOOT_RAISED_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "footRaisedGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case FOOT_UNLOCK_GAIN_ROW:
            if (table->item(FOOT_UNLOCK_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "footUnlockGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW:
            if (table->item(WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "worldFromModelFeedbackGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ERROR_UP_DOWN_BIAS_ROW:
            if (table->item(ERROR_UP_DOWN_BIAS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "errorUpDownBias", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ALIGN_WORLD_FROM_MODEL_GAIN_ROW:
            if (table->item(ALIGN_WORLD_FROM_MODEL_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "alignWorldFromModelGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case HIP_ORIENTATION_GAIN_ROW:
            if (table->item(HIP_ORIENTATION_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hipOrientationGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case MAX_KNEE_ANGLE_DIFFERENCE_ROW:
            if (table->item(MAX_KNEE_ANGLE_DIFFERENCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "maxKneeAngleDifference", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ANKLE_ORIENTATION_GAIN_ROW:
            if (table->item(ANKLE_ORIENTATION_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "ankleOrientationGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ERROR_OUT_TRANSLATION_ROW:
            if (table->item(ERROR_OUT_TRANSLATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "errorOutTranslation", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case ALIGN_WITH_GROUND_ROTATION:
            if (table->item(ALIGN_WITH_GROUND_ROTATION, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "errorOutTranslation", VARIABLE_TYPE_QUATERNION, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::returnToWidget(){
    loadDynamicTableRows();
    setCurrentIndex(MAIN_WIDGET);
}

void FootIkControlsModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            emit modifierNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setName(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setEnable(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setonOffGain(){
    if (bsData){
        if (bsData->gains.onOffGain != onOffGain->value()){
            bsData->gains.onOffGain = onOffGain->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setonOffGain(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setfootPlantedGain(){
    if (bsData){
        if (bsData->gains.footPlantedGain != footPlantedGain->value()){
            bsData->gains.footPlantedGain = footPlantedGain->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setfootPlantedGain(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setfootRaisedGain(){
    if (bsData){
        if (bsData->gains.footRaisedGain != footRaisedGain->value()){
            bsData->gains.footRaisedGain = footRaisedGain->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setfootRaisedGain(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setfootUnlockGain(){
    if (bsData){
        if (bsData->gains.footUnlockGain != footUnlockGain->value()){
            bsData->gains.footUnlockGain = footUnlockGain->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setfootUnlockGain(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setgroundAscendingGain(){
    if (bsData){
        if (bsData->gains.groundAscendingGain != groundAscendingGain->value()){
            bsData->gains.groundAscendingGain = groundAscendingGain->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setgroundAscendingGain(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setgroundDescendingGain(){
    if (bsData){
        if (bsData->gains.groundDescendingGain != groundDescendingGain->value()){
            bsData->gains.groundDescendingGain = groundDescendingGain->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setgroundDescendingGain(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setworldFromModelFeedbackGain(){
    if (bsData){
        if (bsData->gains.worldFromModelFeedbackGain != worldFromModelFeedbackGain->value()){
            bsData->gains.worldFromModelFeedbackGain = worldFromModelFeedbackGain->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setworldFromModelFeedbackGain(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::seterrorUpDownBias(){
    if (bsData){
        if (bsData->gains.errorUpDownBias != errorUpDownBias->value()){
            bsData->gains.errorUpDownBias = errorUpDownBias->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::seterrorUpDownBias(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setalignWorldFromModelGain(){
    if (bsData){
        if (bsData->gains.alignWorldFromModelGain != alignWorldFromModelGain->value()){
            bsData->gains.alignWorldFromModelGain = alignWorldFromModelGain->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setalignWorldFromModelGain(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::sethipOrientationGain(){
    if (bsData){
        if (bsData->gains.hipOrientationGain != hipOrientationGain->value()){
            bsData->gains.hipOrientationGain = hipOrientationGain->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::sethipOrientationGain(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setmaxKneeAngleDifference(){
    if (bsData){
        if (bsData->gains.maxKneeAngleDifference != maxKneeAngleDifference->value()){
            bsData->gains.maxKneeAngleDifference = maxKneeAngleDifference->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setmaxKneeAngleDifference(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setankleOrientationGain(){
    if (bsData){
        if (bsData->gains.ankleOrientationGain != ankleOrientationGain->value()){
            bsData->gains.ankleOrientationGain = ankleOrientationGain->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setankleOrientationGain(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::seterrorOutTranslation(){
    if (bsData){
        if (bsData->errorOutTranslation != errorOutTranslation->value()){
            bsData->errorOutTranslation = errorOutTranslation->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::seterrorOutTranslation(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::setalignWithGroundRotation(){
    if (bsData){
        if (bsData->alignWithGroundRotation != alignWithGroundRotation->value()){
            bsData->alignWithGroundRotation = alignWithGroundRotation->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::setalignWithGroundRotation(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::viewSelectedChild(int row, int column){
    int result = -1;
    bool properties = false;
    if (bsData){
        if (row < ADD_LEG_ROW && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case ENABLE_ROW:
                    if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "enable");
                    break;
                case ON_OFF_GAIN_ROW:
                    if (table->item(ON_OFF_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "onOffGain");
                    break;
                case GROUND_ASCENDING_GAIN_ROW:
                    if (table->item(GROUND_ASCENDING_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "groundAscendingGain");
                    break;
                case GROUND_DESCENDING_GAIN_ROW:
                    if (table->item(GROUND_DESCENDING_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "groundDescendingGain");
                    break;
                case FOOT_PLANTED_GAIN_ROW:
                    if (table->item(FOOT_PLANTED_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "footPlantedGain");
                    break;
                case FOOT_RAISED_GAIN_ROW:
                    if (table->item(FOOT_RAISED_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "footRaisedGain");
                    break;
                case FOOT_UNLOCK_GAIN_ROW:
                    if (table->item(FOOT_UNLOCK_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "footUnlockGain");
                    break;
                case WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW:
                    if (table->item(WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "worldFromModelFeedbackGain");
                    break;
                case ERROR_UP_DOWN_BIAS_ROW:
                    if (table->item(ERROR_UP_DOWN_BIAS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "errorUpDownBias");
                    break;
                case ALIGN_WORLD_FROM_MODEL_GAIN_ROW:
                    if (table->item(ALIGN_WORLD_FROM_MODEL_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "alignWorldFromModelGain");
                    break;
                case HIP_ORIENTATION_GAIN_ROW:
                    if (table->item(HIP_ORIENTATION_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "hipOrientationGain");
                    break;
                case MAX_KNEE_ANGLE_DIFFERENCE_ROW:
                    if (table->item(MAX_KNEE_ANGLE_DIFFERENCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "maxKneeAngleDifference");
                    break;
                case ANKLE_ORIENTATION_GAIN_ROW:
                    if (table->item(ANKLE_ORIENTATION_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "ankleOrientationGain");
                    break;
                case ERROR_OUT_TRANSLATION_ROW:
                    if (table->item(ERROR_OUT_TRANSLATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "errorOutTranslation");
                    break;
                case ALIGN_WITH_GROUND_ROTATION:
                    if (table->item(ALIGN_WITH_GROUND_ROTATION, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "alignWithGroundRotation");
                    break;
                }
            }
        }else if (row == ADD_LEG_ROW && column == NAME_COLUMN){
            addLeg();
        }else if (row > ADD_LEG_ROW && row < ADD_LEG_ROW + bsData->getNumberOfLegs() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->getNumberOfLegs() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    legUI->loadData(((BehaviorFile *)(bsData->getParentFile())), &bsData->legs[result], bsData, result);
                    setCurrentIndex(CHILD_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the range \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeLeg(result);
                    }
                }
            }else{
                FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::viewSelectedChild(): Invalid index of range to view!!");
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    case CHILD_WIDGET:
        legUI->setBindingVariable(index, name);
        break;
    default:
        WARNING_MESSAGE("FootIkControlsModifierUI::variableTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void FootIkControlsModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && events && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), legUI, SLOT(setEventId(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int,QString,QStringList)), events, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void FootIkControlsModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::loadBinding(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (bsData->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewProperties(0, QString(), QStringList());
            }
        }else{
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewVariables(0, QString(), QStringList());
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void FootIkControlsModifierUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("FootIkControlsModifierUI::variableRenamed(): The new variable name is the empty string!!");
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
                bindIndex = bind->getVariableIndexOfBinding("onOffGain");
                if (bindIndex == index){
                    table->item(ON_OFF_GAIN_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("groundAscendingGain");
                if (bindIndex == index){
                    table->item(GROUND_ASCENDING_GAIN_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("groundDescendingGain");
                if (bindIndex == index){
                    table->item(GROUND_DESCENDING_GAIN_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("footPlantedGain");
                if (bindIndex == index){
                    table->item(FOOT_PLANTED_GAIN_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("footRaisedGain");
                if (bindIndex == index){
                    table->item(FOOT_RAISED_GAIN_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("footUnlockGain");
                if (bindIndex == index){
                    table->item(FOOT_UNLOCK_GAIN_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("worldFromModelFeedbackGain");
                if (bindIndex == index){
                    table->item(WORLD_FROM_MODEL_FEEDBACK_GAIN_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("errorUpDownBias");
                if (bindIndex == index){
                    table->item(ERROR_UP_DOWN_BIAS_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("alignWorldFromModelGain");
                if (bindIndex == index){
                    table->item(ALIGN_WORLD_FROM_MODEL_GAIN_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("hipOrientationGain");
                if (bindIndex == index){
                    table->item(HIP_ORIENTATION_GAIN_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("maxKneeAngleDifference");
                if (bindIndex == index){
                    table->item(MAX_KNEE_ANGLE_DIFFERENCE_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("ankleOrientationGain");
                if (bindIndex == index){
                    table->item(ANKLE_ORIENTATION_GAIN_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("errorOutTranslation");
                if (bindIndex == index){
                    table->item(ERROR_OUT_TRANSLATION_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("alignWithGroundRotation");
                if (bindIndex == index){
                    table->item(ALIGN_WITH_GROUND_ROTATION, BINDING_COLUMN)->setText(name);
                }
            }
        }else{
            legUI->variableRenamed(name, index);
        }
    }else{
        FATAL_RUNTIME_ERROR("FootIkControlsModifierUI::variableRenamed(): The data is nullptr!!");
    }
}

