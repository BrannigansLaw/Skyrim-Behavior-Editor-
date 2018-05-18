#include "poweredragdollcontrolsmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbPoweredRagdollControlsModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/behavior/hkbboneindexarray.h"
#include "src/hkxclasses/behavior/hkbboneweightarray.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/hkxclassesui/behaviorui/boneindexarrayui.h"
#include "src/ui/hkxclassesui/behaviorui/boneweightarrayui.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 13

#define NAME_ROW 0
#define ENABLE_ROW 1
#define MAX_FORCE_ROW 2
#define TAU_ROW 3
#define DAMPING_ROW 4
#define PROPERTIONAL_RECOVERY_VELOCITY_ROW 5
#define CONSTANT_RECOVERY_VELOCITY_ROW 6
#define BONES_ROW 7
#define POSE_MATCHING_BONE_0_ROW 8
#define POSE_MATCHING_BONE_1_ROW 9
#define POSE_MATCHING_BONE_2_ROW 10
#define MODE_ROW 11
#define BONE_WEIGHTS_ROW 12

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList PoweredRagdollControlsModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

PoweredRagdollControlsModifierUI::PoweredRagdollControlsModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      groupBox(new QGroupBox("hkbPoweredRagdollControlsModifier")),
      table(new TableWidget(QColor(Qt::white))),
      boneIndexUI(new BoneIndexArrayUI),
      boneWeightsUI(new BoneWeightArrayUI),
      name(new LineEdit),
      enable(new CheckBox),
      maxForce(new DoubleSpinBox),
      tau(new DoubleSpinBox),
      damping(new DoubleSpinBox),
      proportionalRecoveryVelocity(new DoubleSpinBox),
      constantRecoveryVelocity(new DoubleSpinBox),
      bones(new CheckButtonCombo("Edit")),
      poseMatchingBone0(new ComboBox),
      poseMatchingBone1(new ComboBox),
      poseMatchingBone2(new ComboBox),
      mode(new ComboBox),
      boneWeights(new CheckButtonCombo("Edit"))
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
    table->setItem(MAX_FORCE_ROW, NAME_COLUMN, new TableWidgetItem("maxForce"));
    table->setItem(MAX_FORCE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MAX_FORCE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MAX_FORCE_ROW, VALUE_COLUMN, maxForce);
    table->setItem(TAU_ROW, NAME_COLUMN, new TableWidgetItem("tau"));
    table->setItem(TAU_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(TAU_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TAU_ROW, VALUE_COLUMN, tau);
    table->setItem(DAMPING_ROW, NAME_COLUMN, new TableWidgetItem("damping"));
    table->setItem(DAMPING_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DAMPING_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DAMPING_ROW, VALUE_COLUMN, damping);
    table->setItem(PROPERTIONAL_RECOVERY_VELOCITY_ROW, NAME_COLUMN, new TableWidgetItem("proportionalRecoveryVelocity"));
    table->setItem(PROPERTIONAL_RECOVERY_VELOCITY_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(PROPERTIONAL_RECOVERY_VELOCITY_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(PROPERTIONAL_RECOVERY_VELOCITY_ROW, VALUE_COLUMN, proportionalRecoveryVelocity);
    table->setItem(CONSTANT_RECOVERY_VELOCITY_ROW, NAME_COLUMN, new TableWidgetItem("constantRecoveryVelocity"));
    table->setItem(CONSTANT_RECOVERY_VELOCITY_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(CONSTANT_RECOVERY_VELOCITY_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(CONSTANT_RECOVERY_VELOCITY_ROW, VALUE_COLUMN, constantRecoveryVelocity);
    table->setItem(BONES_ROW, NAME_COLUMN, new TableWidgetItem("bones"));
    table->setItem(BONES_ROW, TYPE_COLUMN, new TableWidgetItem("hkBoneIndexArray", Qt::AlignCenter));
    table->setItem(BONES_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(BONES_ROW, VALUE_COLUMN, bones);
    table->setItem(POSE_MATCHING_BONE_0_ROW, NAME_COLUMN, new TableWidgetItem("poseMatchingBone0"));
    table->setItem(POSE_MATCHING_BONE_0_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(POSE_MATCHING_BONE_0_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(POSE_MATCHING_BONE_0_ROW, VALUE_COLUMN, poseMatchingBone0);
    table->setItem(POSE_MATCHING_BONE_1_ROW, NAME_COLUMN, new TableWidgetItem("poseMatchingBone1"));
    table->setItem(POSE_MATCHING_BONE_1_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(POSE_MATCHING_BONE_1_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(POSE_MATCHING_BONE_1_ROW, VALUE_COLUMN, poseMatchingBone1);
    table->setItem(POSE_MATCHING_BONE_2_ROW, NAME_COLUMN, new TableWidgetItem("poseMatchingBone2"));
    table->setItem(POSE_MATCHING_BONE_2_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(POSE_MATCHING_BONE_2_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(POSE_MATCHING_BONE_2_ROW, VALUE_COLUMN, poseMatchingBone2);
    table->setItem(MODE_ROW, NAME_COLUMN, new TableWidgetItem("mode"));
    table->setItem(MODE_ROW, TYPE_COLUMN, new TableWidgetItem("Mode", Qt::AlignCenter));
    table->setItem(MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(MODE_ROW, VALUE_COLUMN, mode);
    table->setItem(BONE_WEIGHTS_ROW, NAME_COLUMN, new TableWidgetItem("boneWeights"));
    table->setItem(BONE_WEIGHTS_ROW, TYPE_COLUMN, new TableWidgetItem("hkbBoneWeightArray", Qt::AlignCenter));
    table->setItem(BONE_WEIGHTS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(BONE_WEIGHTS_ROW, VALUE_COLUMN, boneWeights);
    topLyt->addWidget(table, 0, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(boneIndexUI);
    addWidget(boneWeightsUI);
}

void PoweredRagdollControlsModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(maxForce, SIGNAL(editingFinished()), this, SLOT(setMaxForce()), Qt::UniqueConnection);
    connect(tau, SIGNAL(editingFinished()), this, SLOT(setTau()), Qt::UniqueConnection);
    connect(damping, SIGNAL(editingFinished()), this, SLOT(setDamping()), Qt::UniqueConnection);
    connect(proportionalRecoveryVelocity, SIGNAL(released()), this, SLOT(setProportionalRecoveryVelocity()), Qt::UniqueConnection);
    connect(constantRecoveryVelocity, SIGNAL(released()), this, SLOT(setConstantRecoveryVelocity()), Qt::UniqueConnection);
    connect(poseMatchingBone0, SIGNAL(editingFinished()), this, SLOT(setPoseMatchingBone0()), Qt::UniqueConnection);
    connect(poseMatchingBone1, SIGNAL(editingFinished()), this, SLOT(setPoseMatchingBone1()), Qt::UniqueConnection);
    connect(poseMatchingBone2, SIGNAL(editingFinished()), this, SLOT(setPoseMatchingBone2()), Qt::UniqueConnection);
    connect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(setMode(int)), Qt::UniqueConnection);
    connect(bones, SIGNAL(pressed()), this, SLOT(viewBones()), Qt::UniqueConnection);
    connect(bones, SIGNAL(enabled(bool)), this, SLOT(toggleBones(bool)), Qt::UniqueConnection);
    connect(boneIndexUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(boneWeights, SIGNAL(pressed()), this, SLOT(viewBoneWeights()), Qt::UniqueConnection);
    connect(boneWeights, SIGNAL(enabled(bool)), this, SLOT(toggleBoneWeights(bool)), Qt::UniqueConnection);
    connect(boneWeightsUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void PoweredRagdollControlsModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(maxForce, SIGNAL(editingFinished()), this, SLOT(setMaxForce()));
    disconnect(tau, SIGNAL(editingFinished()), this, SLOT(setTau()));
    disconnect(damping, SIGNAL(editingFinished()), this, SLOT(setDamping()));
    disconnect(proportionalRecoveryVelocity, SIGNAL(released()), this, SLOT(setProportionalRecoveryVelocity()));
    disconnect(constantRecoveryVelocity, SIGNAL(released()), this, SLOT(setConstantRecoveryVelocity()));
    disconnect(poseMatchingBone0, SIGNAL(editingFinished()), this, SLOT(setPoseMatchingBone0()));
    disconnect(poseMatchingBone1, SIGNAL(editingFinished()), this, SLOT(setPoseMatchingBone1()));
    disconnect(poseMatchingBone2, SIGNAL(editingFinished()), this, SLOT(setPoseMatchingBone2()));
    disconnect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(setMode(int)));
    disconnect(bones, SIGNAL(pressed()), this, SLOT(viewBones()));
    disconnect(bones, SIGNAL(enabled(bool)), this, SLOT(toggleBones(bool)));
    disconnect(boneIndexUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(boneWeights, SIGNAL(pressed()), this, SLOT(viewBoneWeights()));
    disconnect(boneWeights, SIGNAL(enabled(bool)), this, SLOT(toggleBoneWeights(bool)));
    disconnect(boneWeightsUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void PoweredRagdollControlsModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *ragdollBones){
    if (variables && properties && ragdollBones){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(ragdollBones, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(ragdollBones, SIGNAL(elementSelected(int,QString)), boneIndexUI, SLOT(setRagdollBone(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(boneIndexUI, SIGNAL(viewRagdollBones(int)), ragdollBones, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::loadData(HkxObject *data){
    setCurrentIndex(MAIN_WIDGET);
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_POWERED_RAGDOLL_CONTROLS_MODIFIER){
            QStringList boneNames("None");
            hkbVariableBindingSet *varBind = nullptr;
            bsData = static_cast<hkbPoweredRagdollControlsModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            maxForce->setValue(bsData->maxForce);
            tau->setValue(bsData->tau);
            damping->setValue(bsData->damping);
            proportionalRecoveryVelocity->setValue(bsData->proportionalRecoveryVelocity);
            constantRecoveryVelocity->setValue(bsData->constantRecoveryVelocity);
            if (bsData->bones.data()){
                bones->setChecked(true);
                bones->setText("Edit");
            }else{
                bones->setChecked(false);
                bones->setText("nullptr");
            }
            if (poseMatchingBone0->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRagdollBoneNames();
                poseMatchingBone0->insertItems(0, boneNames);
            }
            poseMatchingBone0->setCurrentIndex(bsData->poseMatchingBone0 + 1);
            if (poseMatchingBone1->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRagdollBoneNames();
                poseMatchingBone1->insertItems(0, boneNames);
            }
            poseMatchingBone1->setCurrentIndex(bsData->poseMatchingBone1 + 1);
            if (poseMatchingBone2->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRagdollBoneNames();
                poseMatchingBone2->insertItems(0, boneNames);
            }
            poseMatchingBone2->setCurrentIndex(bsData->poseMatchingBone2 + 1);
            if (mode->count() == 0){
                mode->insertItems(0, bsData->Mode);
            }
            mode->setCurrentIndex(bsData->Mode.indexOf(bsData->mode));
            if (bsData->boneWeights.data()){
                boneWeights->setChecked(true);
                boneWeights->setText("Edit");
            }else{
                boneWeights->setChecked(false);
                boneWeights->setText("nullptr");
            }
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(MAX_FORCE_ROW, BINDING_COLUMN, varBind, "maxForce");
                loadBinding(TAU_ROW, BINDING_COLUMN, varBind, "tau");
                loadBinding(DAMPING_ROW, BINDING_COLUMN, varBind, "damping");
                loadBinding(PROPERTIONAL_RECOVERY_VELOCITY_ROW, BINDING_COLUMN, varBind, "proportionalRecoveryVelocity");
                loadBinding(CONSTANT_RECOVERY_VELOCITY_ROW, BINDING_COLUMN, varBind, "constantRecoveryVelocity");
                loadBinding(POSE_MATCHING_BONE_0_ROW, BINDING_COLUMN, varBind, "poseMatchingBone0");
                loadBinding(POSE_MATCHING_BONE_1_ROW, BINDING_COLUMN, varBind, "poseMatchingBone1");
                loadBinding(POSE_MATCHING_BONE_2_ROW, BINDING_COLUMN, varBind, "poseMatchingBone2");
                loadBinding(BONE_WEIGHTS_ROW, BINDING_COLUMN, varBind, "boneWeights");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(MAX_FORCE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TAU_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DAMPING_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(PROPERTIONAL_RECOVERY_VELOCITY_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(CONSTANT_RECOVERY_VELOCITY_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(POSE_MATCHING_BONE_0_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(POSE_MATCHING_BONE_1_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(POSE_MATCHING_BONE_2_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void PoweredRagdollControlsModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->setIsChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setName(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setEnable(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::setMaxForce(){
    if (bsData){
        if (bsData->maxForce != maxForce->value()){
            bsData->maxForce = maxForce->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setmaxForce(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::setTau(){
    if (bsData){
        if (bsData->tau != tau->value()){
            bsData->tau = tau->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::settau(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::setDamping(){
    if (bsData){
        if (bsData->damping != damping->value()){
            bsData->damping = damping->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setdamping(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::setProportionalRecoveryVelocity(){
    if (bsData){
        if (bsData->proportionalRecoveryVelocity != proportionalRecoveryVelocity->value()){
            bsData->proportionalRecoveryVelocity = proportionalRecoveryVelocity->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setproportionalRecoveryVelocity(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::setConstantRecoveryVelocity(){
    if (bsData){
        if (bsData->constantRecoveryVelocity != constantRecoveryVelocity->value()){
            bsData->constantRecoveryVelocity = constantRecoveryVelocity->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setconstantRecoveryVelocity(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::setPoseMatchingBone0(int index){
    if (bsData){
        bsData->poseMatchingBone0 = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setPoseMatchingBone0(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::setPoseMatchingBone1(int index){
    if (bsData){
        bsData->poseMatchingBone1 = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setPoseMatchingBone1(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::setPoseMatchingBone2(int index){
    if (bsData){
        bsData->poseMatchingBone2 = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setPoseMatchingBone2(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::setMode(int index){
    if (bsData){
        bsData->mode = bsData->Mode.at(index);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setMode(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::toggleBones(bool enable){
    if (bsData){
        if (!enable){
            bsData->bones = HkxSharedPtr();
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
        }else if (!bsData->bones.data()){
            hkbBoneIndexArray *indices = new hkbBoneIndexArray(bsData->getParentFile());
            bsData->getParentFile()->addObjectToFile(indices, -1);
            bsData->bones = HkxSharedPtr(indices);
            //bones->setText(indices->getName());
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::toggleBones(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::viewBones(){
    if (bsData){
        boneIndexUI->loadData(bsData->bones.data());
        setCurrentIndex(BONE_INDEX_WIDGET);
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::viewBones(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::toggleBoneWeights(bool enable){
    if (bsData){
        if (!enable){
            bsData->boneWeights = HkxSharedPtr();
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
        }else if (enable && !bsData->boneWeights.data()){
            bsData->boneWeights = HkxSharedPtr(new hkbBoneWeightArray(bsData->getParentFile(), -1, static_cast<BehaviorFile *>(bsData->getParentFile())->getNumberOfBones()));
            boneWeights->setText("Edit");
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BlenderGeneratorChildUI::toggleBoneWeights(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::viewBoneWeights(){
    if (bsData){
        boneWeightsUI->loadData(bsData->boneWeights.data());
        setCurrentIndex(BONE_WEIGHT_WIDGET);
    }else{
        CRITICAL_ERROR_MESSAGE("BlenderGeneratorChildUI::viewBoneWeights(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::viewSelected(int row, int column){
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
            case MAX_FORCE_ROW:
                if (table->item(MAX_FORCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "maxForce");
                break;
            case TAU_ROW:
                if (table->item(TAU_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "tau");
                break;
            case DAMPING_ROW:
                if (table->item(DAMPING_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "damping");
                break;
            case PROPERTIONAL_RECOVERY_VELOCITY_ROW:
                if (table->item(PROPERTIONAL_RECOVERY_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "proportionalRecoveryVelocity");
                break;
            case CONSTANT_RECOVERY_VELOCITY_ROW:
                if (table->item(CONSTANT_RECOVERY_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "constantRecoveryVelocity");
                break;
            case POSE_MATCHING_BONE_0_ROW:
                if (table->item(POSE_MATCHING_BONE_0_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "poseMatchingBone0");
                break;
            case POSE_MATCHING_BONE_1_ROW:
                if (table->item(POSE_MATCHING_BONE_1_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "poseMatchingBone1");
                break;
            case POSE_MATCHING_BONE_2_ROW:
                if (table->item(POSE_MATCHING_BONE_2_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "poseMatchingBone2");
                break;
            case BONE_WEIGHTS_ROW:
                if (table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "boneWeights");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
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
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("maxForce");
            if (bindIndex == index){
                table->item(MAX_FORCE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("tau");
            if (bindIndex == index){
                table->item(TAU_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("damping");
            if (bindIndex == index){
                table->item(DAMPING_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("proportionalRecoveryVelocity");
            if (bindIndex == index){
                table->item(PROPERTIONAL_RECOVERY_VELOCITY_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("constantRecoveryVelocity");
            if (bindIndex == index){
                table->item(CONSTANT_RECOVERY_VELOCITY_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("poseMatchingBone0");
            if (bindIndex == index){
                table->item(POSE_MATCHING_BONE_0_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("poseMatchingBone1");
            if (bindIndex == index){
                table->item(POSE_MATCHING_BONE_1_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("poseMatchingBone2");
            if (bindIndex == index){
                table->item(POSE_MATCHING_BONE_2_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("boneWeights");
            if (bindIndex == index){
                table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool PoweredRagdollControlsModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->variableBindingSet = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1), type)) ||
                  (isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1), type))){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void PoweredRagdollControlsModifierUI::setBindingVariable(int index, const QString &name){
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
        case MAX_FORCE_ROW:
            if (table->item(MAX_FORCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "maxForce", VARIABLE_TYPE_REAL, isProperty);
            break;
        case TAU_ROW:
            if (table->item(TAU_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "tau", VARIABLE_TYPE_REAL, isProperty);
            break;
        case DAMPING_ROW:
            if (table->item(DAMPING_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "damping", VARIABLE_TYPE_REAL, isProperty);
            break;
        case PROPERTIONAL_RECOVERY_VELOCITY_ROW:
            if (table->item(PROPERTIONAL_RECOVERY_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "proportionalRecoveryVelocity", VARIABLE_TYPE_REAL, isProperty);
            break;
        case CONSTANT_RECOVERY_VELOCITY_ROW:
            if (table->item(CONSTANT_RECOVERY_VELOCITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "constantRecoveryVelocity", VARIABLE_TYPE_REAL, isProperty);
            break;
        case POSE_MATCHING_BONE_0_ROW:
            if (table->item(POSE_MATCHING_BONE_0_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "poseMatchingBone0", VARIABLE_TYPE_INT32, isProperty);
            break;
        case POSE_MATCHING_BONE_1_ROW:
            if (table->item(POSE_MATCHING_BONE_1_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "poseMatchingBone1", VARIABLE_TYPE_INT32, isProperty);
            break;
        case POSE_MATCHING_BONE_2_ROW:
            if (table->item(POSE_MATCHING_BONE_2_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "poseMatchingBone2", VARIABLE_TYPE_INT32, isProperty);
            break;
        case BONE_WEIGHTS_ROW:
            if (table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "boneWeights", VARIABLE_TYPE_POINTER, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void PoweredRagdollControlsModifierUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}

void PoweredRagdollControlsModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("PoweredRagdollControlsModifierUI::loadBinding(): The data is nullptr!!");
    }
}
