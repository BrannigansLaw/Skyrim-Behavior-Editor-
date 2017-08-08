#include "rigidbodyragdollcontrolsmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbRigidBodyRagdollControlsModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/behavior/hkbboneindexarray.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/hkxclassesui/behaviorui/boneindexarrayui.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 16

#define NAME_ROW 0
#define ENABLE_ROW 1
#define HIERARCHY_ROW 2
#define VELOCITY_DAMPING_ROW 3
#define ACCELERATION_GAIN_ROW 4
#define VELOCITY_GAIN_ROW 5
#define POSITION_GAIN_ROW 6
#define POSITION_MAX_LINEAR_VELOCITY_ROW 7
#define POSITION_MAX_ANGULAR_VELOCITY_ROW 8
#define SNAP_GAIN_ROW 9
#define SNAP_MAX_LINEAR_VELOCITY_ROW 10
#define SNAP_MAX_ANGULAR_VELOCITY_ROW 11
#define SNAP_MAX_LINEAR_DISTANCE_ROW 12
#define SNAP_MAX_ANGULAR_DISTANCE_ROW 13
#define DURATION_TO_BLEND_ROW 14
#define BONES_ROW 15

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList RigidBodyRagdollControlsModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

RigidBodyRagdollControlsModifierUI::RigidBodyRagdollControlsModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      groupBox(new QGroupBox("hkbRigidBodyRagdollControlsModifier")),
      table(new TableWidget(QColor(Qt::white))),
      boneIndexUI(new BoneIndexArrayUI),
      name(new LineEdit),
      enable(new CheckBox),
      hierarchyGain(new DoubleSpinBox),
      velocityDamping(new DoubleSpinBox),
      accelerationGain(new DoubleSpinBox),
      velocityGain(new DoubleSpinBox),
      positionGain(new DoubleSpinBox),
      positionMaxLinearVelocity(new DoubleSpinBox),
      positionMaxAngularVelocity(new DoubleSpinBox),
      snapGain(new DoubleSpinBox),
      snapMaxLinearVelocity(new DoubleSpinBox),
      snapMaxAngularVelocity(new DoubleSpinBox),
      snapMaxLinearDistance(new DoubleSpinBox),
      snapMaxAngularDistance(new DoubleSpinBox),
      durationToBlend(new DoubleSpinBox),
      bones(new CheckButtonCombo("Click to edit"))
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
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(HIERARCHY_ROW, NAME_COLUMN, new TableWidgetItem("hierarchyGain"));
    table->setItem(HIERARCHY_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(HIERARCHY_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(HIERARCHY_ROW, VALUE_COLUMN, hierarchyGain);
    table->setItem(VELOCITY_DAMPING_ROW, NAME_COLUMN, new TableWidgetItem("velocityDamping"));
    table->setItem(VELOCITY_DAMPING_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(VELOCITY_DAMPING_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(VELOCITY_DAMPING_ROW, VALUE_COLUMN, velocityDamping);
    table->setItem(ACCELERATION_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("accelerationGain"));
    table->setItem(ACCELERATION_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ACCELERATION_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ACCELERATION_GAIN_ROW, VALUE_COLUMN, accelerationGain);
    table->setItem(VELOCITY_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("velocityGain"));
    table->setItem(VELOCITY_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(VELOCITY_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(VELOCITY_GAIN_ROW, VALUE_COLUMN, velocityGain);
    table->setItem(POSITION_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("positionGain"));
    table->setItem(POSITION_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(POSITION_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(POSITION_GAIN_ROW, VALUE_COLUMN, positionGain);
    table->setItem(POSITION_MAX_LINEAR_VELOCITY_ROW, NAME_COLUMN, new TableWidgetItem("positionMaxLinearVelocity"));
    table->setItem(POSITION_MAX_LINEAR_VELOCITY_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(POSITION_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(POSITION_MAX_LINEAR_VELOCITY_ROW, VALUE_COLUMN, positionMaxLinearVelocity);
    table->setItem(POSITION_MAX_ANGULAR_VELOCITY_ROW, NAME_COLUMN, new TableWidgetItem("positionMaxAngularVelocity"));
    table->setItem(POSITION_MAX_ANGULAR_VELOCITY_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(POSITION_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(POSITION_MAX_ANGULAR_VELOCITY_ROW, VALUE_COLUMN, positionMaxAngularVelocity);
    table->setItem(SNAP_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("snapGain"));
    table->setItem(SNAP_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(SNAP_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SNAP_GAIN_ROW, VALUE_COLUMN, snapGain);
    table->setItem(SNAP_MAX_LINEAR_VELOCITY_ROW, NAME_COLUMN, new TableWidgetItem("snapMaxLinearVelocity"));
    table->setItem(SNAP_MAX_LINEAR_VELOCITY_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(SNAP_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SNAP_MAX_LINEAR_VELOCITY_ROW, VALUE_COLUMN, snapMaxLinearVelocity);
    table->setItem(SNAP_MAX_ANGULAR_VELOCITY_ROW, NAME_COLUMN, new TableWidgetItem("snapMaxAngularVelocity"));
    table->setItem(SNAP_MAX_ANGULAR_VELOCITY_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(SNAP_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SNAP_MAX_ANGULAR_VELOCITY_ROW, VALUE_COLUMN, snapMaxAngularVelocity);
    table->setItem(SNAP_MAX_LINEAR_DISTANCE_ROW, NAME_COLUMN, new TableWidgetItem("snapMaxLinearDistance"));
    table->setItem(SNAP_MAX_LINEAR_DISTANCE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(SNAP_MAX_LINEAR_DISTANCE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SNAP_MAX_LINEAR_DISTANCE_ROW, VALUE_COLUMN, snapMaxLinearDistance);
    table->setItem(SNAP_MAX_ANGULAR_DISTANCE_ROW, NAME_COLUMN, new TableWidgetItem("snapMaxAngularDistance"));
    table->setItem(SNAP_MAX_ANGULAR_DISTANCE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(SNAP_MAX_ANGULAR_DISTANCE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SNAP_MAX_ANGULAR_DISTANCE_ROW, VALUE_COLUMN, snapMaxAngularDistance);
    table->setItem(DURATION_TO_BLEND_ROW, NAME_COLUMN, new TableWidgetItem("durationToBlend"));
    table->setItem(DURATION_TO_BLEND_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DURATION_TO_BLEND_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DURATION_TO_BLEND_ROW, VALUE_COLUMN, durationToBlend);
    table->setItem(BONES_ROW, NAME_COLUMN, new TableWidgetItem("bones"));
    table->setItem(BONES_ROW, TYPE_COLUMN, new TableWidgetItem("hkbBoneIndexArray", Qt::AlignCenter));
    table->setItem(BONES_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter, QColor(Qt::gray)));
    table->setCellWidget(BONES_ROW, VALUE_COLUMN, bones);
    topLyt->addWidget(table, 0, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(boneIndexUI);
}

void RigidBodyRagdollControlsModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(hierarchyGain, SIGNAL(editingFinished()), this, SLOT(setHierarchyGain()), Qt::UniqueConnection);
    connect(velocityDamping, SIGNAL(editingFinished()), this, SLOT(setVelocityDamping()), Qt::UniqueConnection);
    connect(accelerationGain, SIGNAL(editingFinished()), this, SLOT(setAccelerationGain()), Qt::UniqueConnection);
    connect(velocityGain, SIGNAL(editingFinished()), this, SLOT(setVelocityGain()), Qt::UniqueConnection);
    connect(positionGain, SIGNAL(editingFinished()), this, SLOT(setPositionGain()), Qt::UniqueConnection);
    connect(positionMaxLinearVelocity, SIGNAL(editingFinished()), this, SLOT(setPositionMaxLinearVelocity()), Qt::UniqueConnection);
    connect(positionMaxAngularVelocity, SIGNAL(editingFinished()), this, SLOT(setPositionMaxAngularVelocity()), Qt::UniqueConnection);
    connect(snapGain, SIGNAL(editingFinished()), this, SLOT(setSnapGain()), Qt::UniqueConnection);
    connect(snapMaxLinearVelocity, SIGNAL(editingFinished()), this, SLOT(setSnapMaxLinearVelocity()), Qt::UniqueConnection);
    connect(snapMaxAngularVelocity, SIGNAL(editingFinished()), this, SLOT(setSnapMaxAngularVelocity()), Qt::UniqueConnection);
    connect(snapMaxLinearDistance, SIGNAL(editingFinished()), this, SLOT(setSnapMaxLinearDistance()), Qt::UniqueConnection);
    connect(snapMaxAngularDistance, SIGNAL(editingFinished()), this, SLOT(setSnapMaxAngularDistance()), Qt::UniqueConnection);
    connect(durationToBlend, SIGNAL(editingFinished()), this, SLOT(setDurationToBlend()), Qt::UniqueConnection);
    connect(bones, SIGNAL(pressed()), this, SLOT(viewBones()), Qt::UniqueConnection);
    connect(bones, SIGNAL(enabled(bool)), this, SLOT(toggleBones(bool)), Qt::UniqueConnection);
    connect(boneIndexUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void RigidBodyRagdollControlsModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(hierarchyGain, SIGNAL(editingFinished()), this, SLOT(setHierarchyGain()));
    disconnect(velocityDamping, SIGNAL(editingFinished()), this, SLOT(setVelocityDamping()));
    disconnect(accelerationGain, SIGNAL(editingFinished()), this, SLOT(setAccelerationGain()));
    disconnect(velocityGain, SIGNAL(editingFinished()), this, SLOT(setVelocityGain()));
    disconnect(positionGain, SIGNAL(editingFinished()), this, SLOT(setPositionGain()));
    disconnect(positionMaxLinearVelocity, SIGNAL(editingFinished()), this, SLOT(setPositionMaxLinearVelocity()));
    disconnect(positionMaxAngularVelocity, SIGNAL(editingFinished()), this, SLOT(setPositionMaxAngularVelocity()));
    disconnect(snapGain, SIGNAL(editingFinished()), this, SLOT(setSnapGain()));
    disconnect(snapMaxLinearVelocity, SIGNAL(editingFinished()), this, SLOT(setSnapMaxLinearVelocity()));
    disconnect(snapMaxAngularVelocity, SIGNAL(editingFinished()), this, SLOT(setSnapMaxAngularVelocity()));
    disconnect(snapMaxLinearDistance, SIGNAL(editingFinished()), this, SLOT(setSnapMaxLinearDistance()));
    disconnect(snapMaxAngularDistance, SIGNAL(editingFinished()), this, SLOT(setSnapMaxAngularDistance()));
    disconnect(durationToBlend, SIGNAL(editingFinished()), this, SLOT(setDurationToBlend()));
    disconnect(bones, SIGNAL(pressed()), this, SLOT(viewBones()));
    disconnect(bones, SIGNAL(enabled(bool)), this, SLOT(toggleBones(bool)));
    disconnect(boneIndexUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void RigidBodyRagdollControlsModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *ragdollBones){
    if (variables && properties && ragdollBones){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(ragdollBones, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(ragdollBones, SIGNAL(elementSelected(int,QString)), boneIndexUI, SLOT(setRagdollBone(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(boneIndexUI, SIGNAL(viewRagdollBones(int)), ragdollBones, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void RigidBodyRagdollControlsModifierUI::loadData(HkxObject *data){
    setCurrentIndex(MAIN_WIDGET);
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_RIGID_BODY_RAGDOLL_CONTROLS_MODIFIER){
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<hkbRigidBodyRagdollControlsModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            hierarchyGain->setValue(bsData->hierarchyGain);
            velocityDamping->setValue(bsData->velocityDamping);
            accelerationGain->setValue(bsData->accelerationGain);
            velocityGain->setValue(bsData->velocityGain);
            positionGain->setValue(bsData->positionGain);
            positionMaxLinearVelocity->setValue(bsData->positionMaxLinearVelocity);
            positionMaxAngularVelocity->setValue(bsData->positionMaxAngularVelocity);
            snapGain->setValue(bsData->snapGain);
            snapMaxLinearVelocity->setValue(bsData->snapMaxLinearVelocity);
            snapMaxAngularVelocity->setValue(bsData->snapMaxAngularVelocity);
            snapMaxLinearDistance->setValue(bsData->snapMaxLinearDistance);
            snapMaxAngularDistance->setValue(bsData->snapMaxAngularDistance);
            durationToBlend->setValue(bsData->durationToBlend);
            if (bsData->bones.data()){
                bones->setChecked(true);
                bones->setText("Click to Edit");
            }else{
                bones->setChecked(false);
                bones->setText("NULL");
            }
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(HIERARCHY_ROW, BINDING_COLUMN, varBind, "hierarchyGain");
                loadBinding(VELOCITY_DAMPING_ROW, BINDING_COLUMN, varBind, "velocityDamping");
                loadBinding(ACCELERATION_GAIN_ROW, BINDING_COLUMN, varBind, "accelerationGain");
                loadBinding(VELOCITY_GAIN_ROW, BINDING_COLUMN, varBind, "velocityGain");
                loadBinding(POSITION_GAIN_ROW, BINDING_COLUMN, varBind, "positionGain");
                loadBinding(POSITION_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN, varBind, "positionMaxLinearVelocity");
                loadBinding(POSITION_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN, varBind, "positionMaxAngularVelocity");
                loadBinding(SNAP_GAIN_ROW, BINDING_COLUMN, varBind, "snapGain");
                loadBinding(SNAP_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN, varBind, "snapMaxLinearVelocity");
                loadBinding(SNAP_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN, varBind, "snapMaxAngularVelocity");
                loadBinding(SNAP_MAX_LINEAR_DISTANCE_ROW, BINDING_COLUMN, varBind, "snapMaxLinearDistance");
                loadBinding(SNAP_MAX_ANGULAR_DISTANCE_ROW, BINDING_COLUMN, varBind, "snapMaxAngularDistance");
                loadBinding(DURATION_TO_BLEND_ROW, BINDING_COLUMN, varBind, "durationToBlend");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(HIERARCHY_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(VELOCITY_DAMPING_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ACCELERATION_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(VELOCITY_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(POSITION_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(POSITION_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(POSITION_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SNAP_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SNAP_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SNAP_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SNAP_MAX_LINEAR_DISTANCE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SNAP_MAX_ANGULAR_DISTANCE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DURATION_TO_BLEND_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void RigidBodyRagdollControlsModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setName(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setEnable(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setHierarchyGain(){
    if (bsData){
        if (bsData->hierarchyGain != hierarchyGain->value()){
            bsData->hierarchyGain = hierarchyGain->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::sethierarchyGain(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setVelocityDamping(){
    if (bsData){
        if (bsData->velocityDamping != velocityDamping->value()){
            bsData->velocityDamping = velocityDamping->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setvelocityDamping(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setAccelerationGain(){
    if (bsData){
        if (bsData->accelerationGain != accelerationGain->value()){
            bsData->accelerationGain = accelerationGain->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setaccelerationGain(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setVelocityGain(){
    if (bsData){
        if (bsData->velocityGain != velocityGain->value()){
            bsData->velocityGain = velocityGain->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setvelocityGain(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setPositionGain(){
    if (bsData){
        if (bsData->positionGain != positionGain->value()){
            bsData->positionGain = positionGain->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setpositionGain(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setPositionMaxLinearVelocity(){
    if (bsData){
        if (bsData->positionMaxLinearVelocity != positionMaxLinearVelocity->value()){
            bsData->positionMaxLinearVelocity = positionMaxLinearVelocity->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setpositionMaxLinearVelocity(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setPositionMaxAngularVelocity(){
    if (bsData){
        if (bsData->positionMaxAngularVelocity != positionMaxAngularVelocity->value()){
            bsData->positionMaxAngularVelocity = positionMaxAngularVelocity->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setpositionMaxAngularVelocity(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setSnapGain(){
    if (bsData){
        if (bsData->snapGain != snapGain->value()){
            bsData->snapGain = snapGain->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setsnapGain(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setSnapMaxLinearVelocity(){
    if (bsData){
        if (bsData->snapMaxLinearVelocity != snapMaxLinearVelocity->value()){
            bsData->snapMaxLinearVelocity = snapMaxLinearVelocity->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setsnapMaxLinearVelocity(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setSnapMaxAngularVelocity(){
    if (bsData){
        if (bsData->snapMaxAngularVelocity != snapMaxAngularVelocity->value()){
            bsData->snapMaxAngularVelocity = snapMaxAngularVelocity->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setsnapMaxAngularVelocity(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setSnapMaxLinearDistance(){
    if (bsData){
        if (bsData->snapMaxLinearDistance != snapMaxLinearDistance->value()){
            bsData->snapMaxLinearDistance = snapMaxLinearDistance->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setsnapMaxLinearDistance(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setSnapMaxAngularDistance(){
    if (bsData){
        if (bsData->snapMaxAngularDistance != snapMaxAngularDistance->value()){
            bsData->snapMaxAngularDistance = snapMaxAngularDistance->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setsnapMaxAngularDistance(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::setDurationToBlend(){
    if (bsData){
        if (bsData->durationToBlend != durationToBlend->value()){
            bsData->durationToBlend = durationToBlend->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setdurationToBlend(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::toggleBones(bool enable){
    if (bsData){
        if (!enable){
            bsData->bones = HkxSharedPtr();
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
        }else if (!bsData->bones.data()){
            hkbBoneIndexArray *indices = new hkbBoneIndexArray(bsData->getParentFile());
            bsData->getParentFile()->addObjectToFile(indices, -1);
            bsData->bones = HkxSharedPtr(indices);
            bones->setText("Click to Edit");
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::toggleBones(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::viewBones(){
    if (bsData){
        boneIndexUI->loadData(bsData->bones.data());
        setCurrentIndex(CHILD_WIDGET);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::viewBones(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::viewSelected(int row, int column){
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
            case HIERARCHY_ROW:
                if (table->item(HIERARCHY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "hierarchyGain");
                break;
            case VELOCITY_DAMPING_ROW:
                if (table->item(VELOCITY_DAMPING_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "velocityDamping");
                break;
            case ACCELERATION_GAIN_ROW:
                if (table->item(ACCELERATION_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "accelerationGain");
                break;
            case VELOCITY_GAIN_ROW:
                if (table->item(VELOCITY_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "velocityGain");
                break;
            case POSITION_GAIN_ROW:
                if (table->item(POSITION_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "positionGain");
                break;
            case POSITION_MAX_LINEAR_VELOCITY_ROW:
                if (table->item(POSITION_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "positionMaxLinearVelocity");
                break;
            case POSITION_MAX_ANGULAR_VELOCITY_ROW:
                if (table->item(POSITION_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "positionMaxAngularVelocity");
                break;
            case SNAP_GAIN_ROW:
                if (table->item(SNAP_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "snapGain");
                break;
            case SNAP_MAX_LINEAR_VELOCITY_ROW:
                if (table->item(SNAP_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "snapMaxLinearVelocity");
                break;
            case SNAP_MAX_ANGULAR_VELOCITY_ROW:
                if (table->item(SNAP_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "snapMaxAngularVelocity");
                break;
            case SNAP_MAX_LINEAR_DISTANCE_ROW:
                if (table->item(SNAP_MAX_LINEAR_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "snapMaxLinearDistance");
                break;
            case SNAP_MAX_ANGULAR_DISTANCE_ROW:
                if (table->item(SNAP_MAX_ANGULAR_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "snapMaxAngularDistance");
                break;
            case DURATION_TO_BLEND_ROW:
                if (table->item(DURATION_TO_BLEND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "durationToBlend");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void RigidBodyRagdollControlsModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("hierarchyGain");
            if (bindIndex == index){
                table->item(HIERARCHY_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("velocityDamping");
            if (bindIndex == index){
                table->item(VELOCITY_DAMPING_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("accelerationGain");
            if (bindIndex == index){
                table->item(ACCELERATION_GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("velocityGain");
            if (bindIndex == index){
                table->item(VELOCITY_GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("positionGain");
            if (bindIndex == index){
                table->item(POSITION_GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("positionMaxLinearVelocity");
            if (bindIndex == index){
                table->item(POSITION_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("positionMaxAngularVelocity");
            if (bindIndex == index){
                table->item(POSITION_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("snapGain");
            if (bindIndex == index){
                table->item(SNAP_GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("snapMaxLinearVelocity");
            if (bindIndex == index){
                table->item(SNAP_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("snapMaxAngularVelocity");
            if (bindIndex == index){
                table->item(SNAP_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("snapMaxLinearDistance");
            if (bindIndex == index){
                table->item(SNAP_MAX_LINEAR_DISTANCE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("snapMaxAngularDistance");
            if (bindIndex == index){
                table->item(SNAP_MAX_ANGULAR_DISTANCE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("durationToBlend");
            if (bindIndex == index){
                table->item(DURATION_TO_BLEND_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool RigidBodyRagdollControlsModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void RigidBodyRagdollControlsModifierUI::setBindingVariable(int index, const QString &name){
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
        case HIERARCHY_ROW:
            if (table->item(HIERARCHY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "hierarchyGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case VELOCITY_DAMPING_ROW:
            if (table->item(VELOCITY_DAMPING_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "velocityDamping", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ACCELERATION_GAIN_ROW:
            if (table->item(ACCELERATION_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "accelerationGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case VELOCITY_GAIN_ROW:
            if (table->item(VELOCITY_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "velocityGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case POSITION_GAIN_ROW:
            if (table->item(POSITION_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "positionGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case POSITION_MAX_LINEAR_VELOCITY_ROW:
            if (table->item(POSITION_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "positionMaxLinearVelocity", VARIABLE_TYPE_REAL, isProperty);
            break;
        case POSITION_MAX_ANGULAR_VELOCITY_ROW:
            if (table->item(POSITION_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "positionMaxAngularVelocity", VARIABLE_TYPE_REAL, isProperty);
            break;
        case SNAP_GAIN_ROW:
            if (table->item(SNAP_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "snapGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case SNAP_MAX_LINEAR_VELOCITY_ROW:
            if (table->item(SNAP_MAX_LINEAR_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "snapMaxLinearVelocity", VARIABLE_TYPE_REAL, isProperty);
            break;
        case SNAP_MAX_ANGULAR_VELOCITY_ROW:
            if (table->item(SNAP_MAX_ANGULAR_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "snapMaxAngularVelocity", VARIABLE_TYPE_REAL, isProperty);
            break;
        case SNAP_MAX_LINEAR_DISTANCE_ROW:
            if (table->item(SNAP_MAX_LINEAR_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "snapMaxLinearDistance", VARIABLE_TYPE_REAL, isProperty);
            break;
        case SNAP_MAX_ANGULAR_DISTANCE_ROW:
            if (table->item(SNAP_MAX_ANGULAR_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "snapMaxAngularDistance", VARIABLE_TYPE_REAL, isProperty);
            break;
        case DURATION_TO_BLEND_ROW:
            if (table->item(DURATION_TO_BLEND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "durationToBlend", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void RigidBodyRagdollControlsModifierUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}

void RigidBodyRagdollControlsModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("RigidBodyRagdollControlsModifierUI::loadBinding(): The data is NULL!!"));
    }
}
