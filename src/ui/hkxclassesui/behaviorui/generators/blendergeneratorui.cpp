#include "blendergeneratorui.h"
#include "blendergeneratorchildui.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbblendergenerator.h"
#include "src/hkxclasses/behavior/generators/hkbblendergeneratorchild.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"

#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <QStackedLayout>
#include <QHeaderView>
#include <QSpinBox>

#define BASE_NUMBER_OF_ROWS 14

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
#define ADD_CHILD_ROW 13

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

/*
 * BlenderGeneratorUI
 */

QStringList BlenderGeneratorUI::types = {
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
    "hkbBehaviorReferenceGenerator"
};

QStringList BlenderGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BlenderGeneratorUI::BlenderGeneratorUI()
    : rowIndexOfChildToRemove(-1),
      behaviorView(NULL),
      bsData(NULL),
      groupBox(new QGroupBox("hkbBlenderGenerator")),
      childUI(new BlenderGeneratorChildUI),
      topLyt(new QGridLayout),
      addChildPB(new QPushButton("Add Child With")),
      typeSelectorCB(new ComboBox),
      removeChildPB(new QPushButton("Remove Selected Child")),
      table(new TableWidget),
      name(new LineEdit),
      referencePoseWeightThreshold(new DoubleSpinBox),
      blendParameter(new DoubleSpinBox),
      minCyclicBlendParameter(new DoubleSpinBox),
      maxCyclicBlendParameter(new DoubleSpinBox),
      indexOfSyncMasterChild(new SpinBox),
      flagSync(new QCheckBox),
      flagSmoothGeneratorWeights(new QCheckBox),
      flagDontDeactivateChildrenWithZeroWeights(new QCheckBox),
      flagParametricBlend(new QCheckBox),
      flagIsParametricBlendCyclic(new QCheckBox),
      flagForceDensePose(new QCheckBox),
      subtractLastChild(new QCheckBox)
{
    typeSelectorCB->insertItems(0, types);
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new QTableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new QTableWidgetItem("hkStringPtr"));
    table->setItem(NAME_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, NAME_COLUMN, new QTableWidgetItem("referencePoseWeightThreshold"));
    table->setItem(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, TYPE_COLUMN, new QTableWidgetItem("hkReal"));
    table->setItem(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN, new QTableWidgetItem("NONE"));
    table->setCellWidget(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, VALUE_COLUMN, referencePoseWeightThreshold);
    table->setItem(BLEND_PARAMETER_ROW, NAME_COLUMN, new QTableWidgetItem("blendParameter"));
    table->setItem(BLEND_PARAMETER_ROW, TYPE_COLUMN, new QTableWidgetItem("hkReal"));
    table->setItem(BLEND_PARAMETER_ROW, BINDING_COLUMN, new QTableWidgetItem("NONE"));
    table->setCellWidget(BLEND_PARAMETER_ROW, VALUE_COLUMN, blendParameter);
    table->setItem(MIN_CYCLIC_BLEND_PARAMETER_ROW, NAME_COLUMN, new QTableWidgetItem("minCyclicBlendParameter"));
    table->setItem(MIN_CYCLIC_BLEND_PARAMETER_ROW, TYPE_COLUMN, new QTableWidgetItem("hkReal"));
    table->setItem(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN, new QTableWidgetItem("NONE"));
    table->setCellWidget(MIN_CYCLIC_BLEND_PARAMETER_ROW, VALUE_COLUMN, minCyclicBlendParameter);
    table->setItem(MAX_CYCLIC_BLEND_PARAMETER_ROW, NAME_COLUMN, new QTableWidgetItem("maxCyclicBlendParameter"));
    table->setItem(MAX_CYCLIC_BLEND_PARAMETER_ROW, TYPE_COLUMN, new QTableWidgetItem("hkReal"));
    table->setItem(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN, new QTableWidgetItem("NONE"));
    table->setCellWidget(MAX_CYCLIC_BLEND_PARAMETER_ROW, VALUE_COLUMN, maxCyclicBlendParameter);
    table->setItem(INDEX_OF_SYNC_MASTER_CHILD_ROW, NAME_COLUMN, new QTableWidgetItem("indexOfSyncMasterChild"));
    table->setItem(INDEX_OF_SYNC_MASTER_CHILD_ROW, TYPE_COLUMN, new QTableWidgetItem("hkInt32"));
    table->setItem(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN, new QTableWidgetItem("NONE"));
    table->setCellWidget(INDEX_OF_SYNC_MASTER_CHILD_ROW, VALUE_COLUMN, indexOfSyncMasterChild);
    table->setItem(FLAG_SYNC_ROW, NAME_COLUMN, new QTableWidgetItem("flagSync"));
    table->setItem(FLAG_SYNC_ROW, TYPE_COLUMN, new QTableWidgetItem("hkInt16"));
    table->setItem(FLAG_SYNC_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(FLAG_SYNC_ROW, VALUE_COLUMN, flagSync);
    table->setItem(FLAG_SMOOTH_GENERATOR_WEIGHTS_ROW, NAME_COLUMN, new QTableWidgetItem("flagSmoothGeneratorWeights"));
    table->setItem(FLAG_SMOOTH_GENERATOR_WEIGHTS_ROW, TYPE_COLUMN, new QTableWidgetItem("hkInt16"));
    table->setItem(FLAG_SMOOTH_GENERATOR_WEIGHTS_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(FLAG_SMOOTH_GENERATOR_WEIGHTS_ROW, VALUE_COLUMN, flagSmoothGeneratorWeights);
    table->setItem(FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS_ROW, NAME_COLUMN, new QTableWidgetItem("flagDontDeactivateChildrenWithZeroWeights"));
    table->setItem(FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS_ROW, TYPE_COLUMN, new QTableWidgetItem("hkInt16"));
    table->setItem(FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS_ROW, VALUE_COLUMN, flagDontDeactivateChildrenWithZeroWeights);
    table->setItem(FLAG_PARAMETRIC_BLEND_ROW, NAME_COLUMN, new QTableWidgetItem("flagParametricBlend"));
    table->setItem(FLAG_PARAMETRIC_BLEND_ROW, TYPE_COLUMN, new QTableWidgetItem("hkInt16"));
    table->setItem(FLAG_PARAMETRIC_BLEND_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(FLAG_PARAMETRIC_BLEND_ROW, VALUE_COLUMN, flagParametricBlend);
    table->setItem(FLAG_IS_PARAMETRIC_BLEND_CYCLIC_ROW, NAME_COLUMN, new QTableWidgetItem("flagIsParametricBlendCyclic"));
    table->setItem(FLAG_IS_PARAMETRIC_BLEND_CYCLIC_ROW, TYPE_COLUMN, new QTableWidgetItem("hkInt16"));
    table->setItem(FLAG_IS_PARAMETRIC_BLEND_CYCLIC_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(FLAG_IS_PARAMETRIC_BLEND_CYCLIC_ROW, VALUE_COLUMN, flagIsParametricBlendCyclic);
    table->setItem(FLAG_FORCE_DENSE_POSE_ROW, NAME_COLUMN, new QTableWidgetItem("flagForceDensePose"));
    table->setItem(FLAG_FORCE_DENSE_POSE_ROW, TYPE_COLUMN, new QTableWidgetItem("hkInt16"));
    table->setItem(FLAG_FORCE_DENSE_POSE_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(FLAG_FORCE_DENSE_POSE_ROW, VALUE_COLUMN, flagForceDensePose);
    table->setItem(SUBTRACT_LAST_CHILD_ROW, NAME_COLUMN, new QTableWidgetItem("subtractLastChild"));
    table->setItem(SUBTRACT_LAST_CHILD_ROW, TYPE_COLUMN, new QTableWidgetItem("hkBool"));
    table->setItem(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN, new QTableWidgetItem("NONE"));
    table->setCellWidget(SUBTRACT_LAST_CHILD_ROW, VALUE_COLUMN, subtractLastChild);
    table->setCellWidget(ADD_CHILD_ROW, NAME_COLUMN, addChildPB);
    table->setCellWidget(ADD_CHILD_ROW, TYPE_COLUMN, typeSelectorCB);
    table->setCellWidget(ADD_CHILD_ROW, BINDING_COLUMN, removeChildPB);
    table->setItem(ADD_CHILD_ROW, VALUE_COLUMN, new QTableWidgetItem("N/A"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    addWidget(groupBox);
    addWidget(childUI);
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
    connect(addChildPB, SIGNAL(released()), this, SLOT(addChild()), Qt::UniqueConnection);
    connect(removeChildPB, SIGNAL(released()), this, SLOT(removeSelectedChild()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewGenerators(int)), this, SIGNAL(viewGenerators(int)), Qt::UniqueConnection);
}

void BlenderGeneratorUI::loadData(HkxObject *data){
    setCurrentIndex(MAIN_WIDGET);
    hkbVariableBindingSet *bind = NULL;
    if (data){
        if (data->getSignature() == HKB_BLENDER_GENERATOR){
            bsData = static_cast<hkbBlenderGenerator *>(data);
            rowIndexOfChildToRemove = -1;
            name->setText(bsData->name);
            referencePoseWeightThreshold->setValue(bsData->referencePoseWeightThreshold);
            blendParameter->setValue(bsData->blendParameter);
            minCyclicBlendParameter->setValue(bsData->minCyclicBlendParameter);
            maxCyclicBlendParameter->setValue(bsData->maxCyclicBlendParameter);
            indexOfSyncMasterChild->setValue(bsData->indexOfSyncMasterChild);
            QStringList flags = bsData->flags.split("|");
            flagSync->setChecked(false);
            flagSmoothGeneratorWeights->setChecked(false);
            flagDontDeactivateChildrenWithZeroWeights->setChecked(false);
            flagParametricBlend->setChecked(false);
            flagIsParametricBlendCyclic->setChecked(false);
            flagForceDensePose->setChecked(false);
            if (flags.isEmpty()){
                if (bsData->flags == "FLAG_SYNC"){
                    flagSync->setChecked(true);
                }else if (bsData->flags == "FLAG_SMOOTH_GENERATOR_WEIGHTS"){
                    flagSmoothGeneratorWeights->setChecked(true);
                }else if (bsData->flags == "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS"){
                    flagDontDeactivateChildrenWithZeroWeights->setChecked(true);
                }else if (bsData->flags == "FLAG_PARAMETRIC_BLEND"){
                    flagParametricBlend->setChecked(true);
                }else if (bsData->flags == "FLAG_IS_PARAMETRIC_BLEND_CYCLIC"){
                    flagIsParametricBlendCyclic->setChecked(true);
                }else if (bsData->flags == "FLAG_FORCE_DENSE_POSE"){
                    flagForceDensePose->setChecked(true);
                }
            }else{
                for (int i = 0; i < flags.size(); i++){
                    if (flags.at(i) == "FLAG_SYNC"){
                        flagSync->setChecked(true);
                    }else if (flags.at(i) == "FLAG_SMOOTH_GENERATOR_WEIGHTS"){
                        flagSmoothGeneratorWeights->setChecked(true);
                    }else if (flags.at(i) == "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS"){
                        flagDontDeactivateChildrenWithZeroWeights->setChecked(true);
                    }else if (flags.at(i) == "FLAG_PARAMETRIC_BLEND"){
                        flagParametricBlend->setChecked(true);
                    }else if (flags.at(i) == "FLAG_IS_PARAMETRIC_BLEND_CYCLIC"){
                        flagIsParametricBlendCyclic->setChecked(true);
                    }else if (flags.at(i) == "FLAG_FORCE_DENSE_POSE"){
                        flagForceDensePose->setChecked(true);
                    }
                }
            }
            subtractLastChild->setChecked(bsData->subtractLastChild);
            for (int i = 0, k; i < bsData->children.size(); i++){
                k = i + BASE_NUMBER_OF_ROWS;
                if (k >= table->rowCount()){
                    table->setRowCount(table->rowCount() + 1);
                    table->setItem(k, NAME_COLUMN, new QTableWidgetItem("Child "+QString::number(i)));
                    table->setItem(k, TYPE_COLUMN, new QTableWidgetItem("hkbBlenderGeneratorChild"));
                    table->setItem(k, BINDING_COLUMN, new QTableWidgetItem("N/A"));
                    table->setItem(k, VALUE_COLUMN, new QTableWidgetItem("Click to Edit"));
                }else{
                    table->setRowHidden(k, false);
                    table->item(k, NAME_COLUMN)->setText("Child "+QString::number(i));
                }
            }
            for (int j = bsData->children.size() + BASE_NUMBER_OF_ROWS; j < table->rowCount(); j++){
                table->setRowHidden(j, true);
            }
            bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (bind){
                int bindIndex = bind->getVariableIndexOfBinding("referencePoseWeightThreshold");
                if (bindIndex >= 0){
                    table->item(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN)->setText(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(bindIndex));
                }else{
                    table->item(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN)->setText("NONE");
                }
                bindIndex = bind->getVariableIndexOfBinding("blendParameter");
                if (bindIndex >= 0){
                    table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(bindIndex));
                }else{
                    table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText("NONE");
                }
                bindIndex = bind->getVariableIndexOfBinding("minCyclicBlendParameter");
                if (bindIndex >= 0){
                    table->item(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(bindIndex));
                }else{
                    table->item(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText("NONE");
                }
                bindIndex = bind->getVariableIndexOfBinding("maxCyclicBlendParameter");
                if (bindIndex >= 0){
                    table->item(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(bindIndex));
                }else{
                    table->item(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText("NONE");
                }
                bindIndex = bind->getVariableIndexOfBinding("indexOfSyncMasterChild");
                if (bindIndex >= 0){
                    table->item(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN)->setText(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(bindIndex));
                }else{
                    table->item(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN)->setText("NONE");
                }
                bindIndex = bind->getVariableIndexOfBinding("subtractLastChild");
                if (bindIndex >= 0){
                    table->item(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN)->setText(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(bindIndex));
                }else{
                    table->item(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN)->setText("NONE");
                }
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::loadData(): Attempting to load a null pointer!!"))
    }
}

bool BlenderGeneratorUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type){
    hkbVariableBindingSet *varBind = NULL;
    if (bsData){
        varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText("None");
        }else if (static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
                bsData->getParentFile()->addObjectToFile(varBind, -1);
            }
            if (type == VARIABLE_TYPE_POINTER){
                varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY);
            }else{
                varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE);
            }
            table->item(row, BINDING_COLUMN)->setText(variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setBinding(): The data is NULL!!"))
        return false;
    }
    return true;
}

void BlenderGeneratorUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        int row = table->currentRow();
        switch (row){
        case REFERENCE_POSE_WEIGHT_THRESHOLD_ROW:
            setBinding(index, row, name, "referencePoseWeightThreshold", VARIABLE_TYPE_REAL);
            break;
        case BLEND_PARAMETER_ROW:
            setBinding(index, row, name, "blendParameter", VARIABLE_TYPE_REAL);
            break;
        case MIN_CYCLIC_BLEND_PARAMETER_ROW:
            setBinding(index, row, name, "minCyclicBlendParameter", VARIABLE_TYPE_REAL);
            break;
        case MAX_CYCLIC_BLEND_PARAMETER_ROW:
            setBinding(index, row, name, "maxCyclicBlendParameter", VARIABLE_TYPE_REAL);
            break;
        case INDEX_OF_SYNC_MASTER_CHILD_ROW:
            setBinding(index, row, name, "indexOfSyncMasterChild", VARIABLE_TYPE_INT32);
            break;
        case SUBTRACT_LAST_CHILD_ROW:
            setBinding(index, row, name, "subtractLastChild", VARIABLE_TYPE_BOOL);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setBindingVariable(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setName(){
    if (bsData){
        bsData->name = name->text();
        emit generatorNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData) + 1);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setName(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setReferencePoseWeightThreshold(){
    if (bsData){
        bsData->referencePoseWeightThreshold = referencePoseWeightThreshold->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setReferencePoseWeightThreshold(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setBlendParameter(){
    if (bsData){
        bsData->blendParameter = blendParameter->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setBlendParameter(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setMinCyclicBlendParameter(){
    if (bsData){
        bsData->minCyclicBlendParameter = minCyclicBlendParameter->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setMinCyclicBlendParameter(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setMaxCyclicBlendParameter(){
    if (bsData){
        bsData->maxCyclicBlendParameter = maxCyclicBlendParameter->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setMaxCyclicBlendParameter(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setIndexOfSyncMasterChild(){
    if (bsData){
        bsData->indexOfSyncMasterChild = indexOfSyncMasterChild->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setIndexOfSyncMasterChild(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setFlagSync(){
    if (bsData){
        if (flagSync->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_SYNC");
            }else if (!bsData->flags.contains("FLAG_SYNC")){
                bsData->flags.append("|FLAG_SYNC");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_SYNC");
            }else{
                bsData->flags.remove("|FLAG_SYNC");
            }
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagSync(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setFlagSmoothGeneratorWeights(){
    if (bsData){
        if (flagSmoothGeneratorWeights->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_SMOOTH_GENERATOR_WEIGHTS");
            }else if (!bsData->flags.contains("FLAG_SMOOTH_GENERATOR_WEIGHTS")){
                bsData->flags.append("|FLAG_SMOOTH_GENERATOR_WEIGHTS");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_SMOOTH_GENERATOR_WEIGHTS");
            }else{
                bsData->flags.remove("|FLAG_SMOOTH_GENERATOR_WEIGHTS");
            }
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagSmoothGeneratorWeights(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setFlagDontDeactivateChildrenWithZeroWeights(){
    if (bsData){
        if (flagDontDeactivateChildrenWithZeroWeights->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS");
            }else if (!bsData->flags.contains("FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS")){
                bsData->flags.append("|FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS");
            }else{
                bsData->flags.remove("|FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS");
            }
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagDontDeactivateChildrenWithZeroWeights(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setFlagParametricBlend(){
    if (bsData){
        if (flagParametricBlend->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_PARAMETRIC_BLEND");
            }else if (!bsData->flags.contains("FLAG_PARAMETRIC_BLEND")){
                bsData->flags.append("|FLAG_PARAMETRIC_BLEND");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_PARAMETRIC_BLEND");
            }else{
                bsData->flags.remove("|FLAG_PARAMETRIC_BLEND");
            }
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagParametricBlend(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setFlagIsParametricBlendCyclic(){
    if (bsData){
        if (flagIsParametricBlendCyclic->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_IS_PARAMETRIC_BLEND_CYCLIC");
            }else if (!bsData->flags.contains("FLAG_IS_PARAMETRIC_BLEND_CYCLIC")){
                bsData->flags.append("|FLAG_IS_PARAMETRIC_BLEND_CYCLIC");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_IS_PARAMETRIC_BLEND_CYCLIC");
            }else{
                bsData->flags.remove("|FLAG_IS_PARAMETRIC_BLEND_CYCLIC");
            }
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagIsParametricBlendCyclic(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setFlagForceDensePose(){
    if (bsData){
        if (flagForceDensePose->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_FORCE_DENSE_POSE");
            }else if (!bsData->flags.contains("FLAG_FORCE_DENSE_POSE")){
                bsData->flags.append("|FLAG_FORCE_DENSE_POSE");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_FORCE_DENSE_POSE");
            }else{
                bsData->flags.remove("|FLAG_FORCE_DENSE_POSE");
            }
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagForceDensePose(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setSubtractLastChild(){
    if (bsData){
        bsData->subtractLastChild = subtractLastChild->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setSubtractLastChild(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::addChild(){
    Generator_Type typeEnum;
    int result;
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
        default:
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::addChild(): Invalid typeEnum!!"))
            return;
        }
        result = BASE_NUMBER_OF_ROWS + bsData->children.size() - 1;
        if (result >= table->rowCount()){
            table->setRowCount(table->rowCount() + 1);
            table->setItem(result, NAME_COLUMN, new QTableWidgetItem("Child "+QString::number(bsData->children.size() - 1)));
            table->setItem(result, TYPE_COLUMN, new QTableWidgetItem("hkbBlenderGeneratorChild"));
            table->setItem(result, BINDING_COLUMN, new QTableWidgetItem("N/A"));
            table->setItem(result, VALUE_COLUMN, new QTableWidgetItem("Click to Edit"));
        }else{
            table->setRowHidden(result, false);
            table->item(result, NAME_COLUMN)->setText("Child "+QString::number(bsData->children.size() - 1));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::addChild(): The data or behavior graph pointer is NULL!!"))
    }
}

void BlenderGeneratorUI::removeSelectedChild(){
    int result = rowIndexOfChildToRemove - BASE_NUMBER_OF_ROWS;
    hkbBlenderGeneratorChild *child = NULL;
    if (bsData && behaviorView){
        if (result < bsData->children.size() && result >= 0){
            child = static_cast<hkbBlenderGeneratorChild *>(bsData->children.at(result).data());
            behaviorView->removeItemFromGraph(behaviorView->getSelectedIconsChildIcon(child->generator.data()), result);//Reorderchildren?
            behaviorView->removeObjects();
            delete table->takeItem(rowIndexOfChildToRemove, NAME_COLUMN);
            delete table->takeItem(rowIndexOfChildToRemove, TYPE_COLUMN);
            delete table->takeItem(rowIndexOfChildToRemove, BINDING_COLUMN);
            delete table->takeItem(rowIndexOfChildToRemove, VALUE_COLUMN);
            table->removeRow(rowIndexOfChildToRemove);
            for (int i = BASE_NUMBER_OF_ROWS, k = 0; i < table->rowCount(); i++, k++){
                if (!table->isRowHidden(i)){
                    if (!table->item(i, NAME_COLUMN)){
                        table->setItem(i, NAME_COLUMN, new QTableWidgetItem("Child "+QString::number(k)));
                    }else{
                        table->item(i, NAME_COLUMN)->setText("Child "+QString::number(k));
                    }
                }
            }
            rowIndexOfChildToRemove = -1;
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::removeSelectedChild(): Invalid index of child to remove!!"))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::removeSelectedChild(): The data or behavior graph pointer is NULL!!"))
    }
}

void BlenderGeneratorUI::viewSelectedChild(int row, int column){
    int result;
    if (bsData){
        if (row < BASE_NUMBER_OF_ROWS && row >= 0){
            if (column == BINDING_COLUMN){
                QString path;
                switch (row){
                case REFERENCE_POSE_WEIGHT_THRESHOLD_ROW:
                    path = "referencePoseWeightThreshold";
                    break;
                case BLEND_PARAMETER_ROW:
                    path = "blendParameter";
                    break;
                case MIN_CYCLIC_BLEND_PARAMETER_ROW:
                    path = "minCyclicBlendParameter";
                    break;
                case MAX_CYCLIC_BLEND_PARAMETER_ROW:
                    path = "maxCyclicBlendParameter";
                    break;
                case INDEX_OF_SYNC_MASTER_CHILD_ROW:
                    path = "indexOfSyncMasterChild";
                    break;
                case SUBTRACT_LAST_CHILD_ROW:
                    path = "subtractLastChild";
                    break;
                default:
                    return;
                }
                if (bsData->variableBindingSet.data()){
                    emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
                }else{
                    emit viewVariables(0);
                }
            }
        }else{
            result = row - BASE_NUMBER_OF_ROWS;
            rowIndexOfChildToRemove = row;
            if (bsData->children.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    childUI->loadData(static_cast<hkbBlenderGeneratorChild *>(bsData->children.at(result).data()));
                    setCurrentIndex(CHILD_WIDGET);
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::viewSelectedChild(): Invalid index of child to view!!"))
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::viewSelectedChild(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}

void BlenderGeneratorUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *generators){
    if (variables && properties && generators){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        //connect(properties, SIGNAL(elementSelected(int,QString)), childUI, SLOT(setBindingVariable(int,QString))); Right click to choose character property???
        connect(generators, SIGNAL(elementSelected(int,QString)), childUI, SLOT(setGenerator(int,QString)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void BlenderGeneratorUI::renameVariable(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = NULL;
    if (name == ""){
        WARNING_MESSAGE(QString("BlenderGeneratorUI::variableRenamed(): The new variable name is the empty string!!"))
    }
    if (bsData){
        index--;
        bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("referencePoseWeightThreshold");
            if (bindIndex == index){
                table->item(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN)->setText(name);
            }else{
                bindIndex = bind->getVariableIndexOfBinding("blendParameter");
                if (bindIndex == index){
                    table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(name);
                }else{
                    bindIndex = bind->getVariableIndexOfBinding("minCyclicBlendParameter");
                    if (bindIndex == index){
                        table->item(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(name);
                    }else{
                        bindIndex = bind->getVariableIndexOfBinding("maxCyclicBlendParameter");
                        if (bindIndex == index){
                            table->item(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(name);
                        }else{
                            bindIndex = bind->getVariableIndexOfBinding("indexOfSyncMasterChild");
                            if (bindIndex == index){
                                table->item(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN)->setText(name);
                            }else{
                                bindIndex = bind->getVariableIndexOfBinding("subtractLastChild");
                                if (bindIndex == index){
                                    table->item(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN)->setText(name);
                                }
                            }
                        }
                    }
                }
            }
        }
        childUI->variableRenamed(name, index);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::variableRenamed(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::generatorRenamed(const QString &name, int index){
    if (name == ""){
        WARNING_MESSAGE(QString("BlenderGeneratorUI::generatorRenamed(): The new variable name is the empty string!!"))
    }
    childUI->generatorRenamed(name, index);
}

void BlenderGeneratorUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
    setCurrentIndex(MAIN_WIDGET);
    childUI->setBehaviorView(view);
}

