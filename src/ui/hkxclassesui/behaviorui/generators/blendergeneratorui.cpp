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
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include "src/ui/genericdatawidgets.h"
#include <QStackedLayout>
#include <QHeaderView>

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

#define BINDING_ITEM_LABEL QString("Use Property     ")

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
    : behaviorView(NULL),
      bsData(NULL),
      groupBox(new QGroupBox("hkbBlenderGenerator")),
      childUI(new BlenderGeneratorChildUI),
      topLyt(new QGridLayout),
      typeSelectorCB(new ComboBox),
      table(new TableWidget(QColor(Qt::cyan))),
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
      subtractLastChild(new CheckBox)
{
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

void BlenderGeneratorUI::connectSignals(){
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
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(returnToParent(bool)), this, SLOT(returnToWidget(bool)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewGenerators(int)), this, SIGNAL(viewGenerators(int)), Qt::UniqueConnection);
}

void BlenderGeneratorUI::disconnectSignals(){
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
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(childUI, SIGNAL(returnToParent(bool)), this, SLOT(returnToWidget(bool)));
    disconnect(childUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)));
    disconnect(childUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)));
    disconnect(childUI, SIGNAL(viewGenerators(int)), this, SIGNAL(viewGenerators(int)));
}

void BlenderGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    hkbVariableBindingSet *varBind = NULL;
    if (data){
        if (data->getSignature() == HKB_BLENDER_GENERATOR){
            bsData = static_cast<hkbBlenderGenerator *>(data);
            name->setText(bsData->name);
            referencePoseWeightThreshold->setValue(bsData->referencePoseWeightThreshold);
            blendParameter->setValue(bsData->blendParameter);
            minCyclicBlendParameter->setValue(bsData->minCyclicBlendParameter);
            maxCyclicBlendParameter->setValue(bsData->maxCyclicBlendParameter);
            indexOfSyncMasterChild->setValue(bsData->indexOfSyncMasterChild);
            bool ok = true;
            hkbBlenderGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
            if (ok){
                if (flags.testFlag(hkbBlenderGenerator::FLAG_SYNC)){
                    flagSync->setChecked(true);
                }else{
                    flagSync->setChecked(false);
                }
                if (flags.testFlag(hkbBlenderGenerator::FLAG_SMOOTH_GENERATOR_WEIGHTS)){
                    flagSmoothGeneratorWeights->setChecked(true);
                }else{
                    flagSmoothGeneratorWeights->setChecked(false);
                }
                if (flags.testFlag(hkbBlenderGenerator::FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS)){
                    flagDontDeactivateChildrenWithZeroWeights->setChecked(true);
                }else{
                    flagDontDeactivateChildrenWithZeroWeights->setChecked(false);
                }
                if (flags.testFlag(hkbBlenderGenerator::FLAG_PARAMETRIC_BLEND)){
                    flagParametricBlend->setChecked(true);
                }else{
                    flagParametricBlend->setChecked(false);
                }
                if (flags.testFlag(hkbBlenderGenerator::FLAG_IS_PARAMETRIC_BLEND_CYCLIC)){
                    flagIsParametricBlendCyclic->setChecked(true);
                }else{
                    flagIsParametricBlendCyclic->setChecked(false);
                }
                if (flags.testFlag(hkbBlenderGenerator::FLAG_FORCE_DENSE_POSE)){
                    flagForceDensePose->setChecked(true);
                }else{
                    flagForceDensePose->setChecked(false);
                }
                subtractLastChild->setChecked(bsData->subtractLastChild);
                varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
                if (varBind){
                    loadBinding(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN, varBind, "referencePoseWeightThreshold");
                    loadBinding(BLEND_PARAMETER_ROW, BINDING_COLUMN, varBind, "blendParameter");
                    loadBinding(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN, varBind, "minCyclicBlendParameter");
                    loadBinding(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN, varBind, "maxCyclicBlendParameter");
                    loadBinding(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN, varBind, "indexOfSyncMasterChild");
                    loadBinding(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN, varBind, "subtractLastChild");
                }else{
                    table->item(REFERENCE_POSE_WEIGHT_THRESHOLD_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(MIN_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(MAX_CYCLIC_BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(INDEX_OF_SYNC_MASTER_CHILD_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(SUBTRACT_LAST_CHILD_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                }
                loadDynamicTableRows();
            }else{
                CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::loadData(): The flags string is invalid!!!\nString: "+bsData->flags))
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::loadData(): Attempting to load a null pointer!!"))
    }
    connectSignals();
}

void BlenderGeneratorUI::loadDynamicTableRows(){
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
                CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::loadData(): Null child found!!!"));
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::loadDynamicTableRows(): The data is NULL!!"));
    }
    //table->setSortingEnabled(true);
}

void BlenderGeneratorUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
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

bool BlenderGeneratorUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BlenderGeneratorUI::setBindingVariable(int index, const QString & name){
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
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            emit generatorNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setName(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setReferencePoseWeightThreshold(){
    if (bsData){
        if (bsData->referencePoseWeightThreshold != referencePoseWeightThreshold->value()){
            bsData->referencePoseWeightThreshold = referencePoseWeightThreshold->value();
            bsData->getParentFile()->toggleChanged(true);
        }
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
        bool ok = true;
        hkbBlenderGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagSync->isChecked()){
                flags |= hkbBlenderGenerator::FLAG_SYNC;
            }else{
                flags &= ~(hkbBlenderGenerator::FLAG_SYNC);
            }
            bsData->flags = QString::number(flags);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagSync(): The flags string is invalid!!!\nString: "+bsData->flags))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagSync(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setFlagSmoothGeneratorWeights(){
    if (bsData){
        bool ok = true;
        hkbBlenderGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagSmoothGeneratorWeights->isChecked()){
                flags |= hkbBlenderGenerator::FLAG_SMOOTH_GENERATOR_WEIGHTS;
            }else{
                flags &= ~(hkbBlenderGenerator::FLAG_SMOOTH_GENERATOR_WEIGHTS);
            }
            bsData->flags = QString::number(flags);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagSmoothGeneratorWeights(): The flags string is invalid!!!\nString: "+bsData->flags))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagSmoothGeneratorWeights(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setFlagDontDeactivateChildrenWithZeroWeights(){
    if (bsData){
        bool ok = true;
        hkbBlenderGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagDontDeactivateChildrenWithZeroWeights->isChecked()){
                flags |= hkbBlenderGenerator::FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS;
            }else{
                flags &= ~(hkbBlenderGenerator::FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS);
            }
            bsData->flags = QString::number(flags);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagDontDeactivateChildrenWithZeroWeights(): The flags string is invalid!!!\nString: "+bsData->flags))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagDontDeactivateChildrenWithZeroWeights(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setFlagParametricBlend(){
    if (bsData){
        bool ok = true;
        hkbBlenderGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagParametricBlend->isChecked()){
                flags |= hkbBlenderGenerator::FLAG_PARAMETRIC_BLEND;
            }else{
                flags &= ~(hkbBlenderGenerator::FLAG_PARAMETRIC_BLEND);
            }
            bsData->flags = QString::number(flags);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagParametricBlend(): The flags string is invalid!!!\nString: "+bsData->flags))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagParametricBlend(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setFlagIsParametricBlendCyclic(){
    if (bsData){
        bool ok = true;
        hkbBlenderGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagIsParametricBlendCyclic->isChecked()){
                flags |= hkbBlenderGenerator::FLAG_IS_PARAMETRIC_BLEND_CYCLIC;
            }else{
                flags &= ~(hkbBlenderGenerator::FLAG_IS_PARAMETRIC_BLEND_CYCLIC);
            }
            bsData->flags = QString::number(flags);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagIsParametricBlendCyclic(): The flags string is invalid!!!\nString: "+bsData->flags))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagIsParametricBlendCyclic(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::setFlagForceDensePose(){
    if (bsData){
        bool ok = true;
        hkbBlenderGenerator::BlenderFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagForceDensePose->isChecked()){
                flags |= hkbBlenderGenerator::FLAG_FORCE_DENSE_POSE;
            }else{
                flags &= ~(hkbBlenderGenerator::FLAG_FORCE_DENSE_POSE);
            }
            bsData->flags = QString::number(flags);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::setFlagForceDensePose(): The flags string is invalid!!!\nString: "+bsData->flags))
        }
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

void BlenderGeneratorUI::addChildWithGenerator(){
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
        default:
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::addChild(): Invalid typeEnum!!"))
            return;
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::addChild(): The data or behavior graph pointer is NULL!!"))
    }
}

void BlenderGeneratorUI::removeChild(int index){
    hkbBlenderGeneratorChild *child = NULL;
    if (bsData && behaviorView){
        if (index < bsData->children.size() && index >= 0){
            child = static_cast<hkbBlenderGeneratorChild *>(bsData->children.at(index).data());
            behaviorView->removeItemFromGraph(behaviorView->getSelectedIconsChildIcon(child->generator.data()), index);//Reorderchildren?
            behaviorView->removeObjects();
        }else{
            WARNING_MESSAGE(QString("BlenderGeneratorUI::removeChild(): Invalid index of child to remove!!"));
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::removeChild(): The data or behavior graph pointer is NULL!!"));
    }
}

void BlenderGeneratorUI::viewSelectedChild(int row, int column){
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
                CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::viewSelectedChild(): Invalid index of child to view!!"))
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::viewSelectedChild(): The data is NULL!!"))
    }
}

void BlenderGeneratorUI::returnToWidget(bool reloadData){
    if (reloadData){
        loadDynamicTableRows();
    }
    setCurrentIndex(MAIN_WIDGET);
}

void BlenderGeneratorUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    case CHILD_WIDGET:
        childUI->setBindingVariable(index, name);
        break;
    default:
        WARNING_MESSAGE(QString("BlenderGeneratorUI::variableTableElementSelected(): An unwanted element selected event was recieved!!"));;
    }
}

void BlenderGeneratorUI::generatorTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case CHILD_WIDGET:
        childUI->setGenerator(index, name);
        break;
    default:
        WARNING_MESSAGE(QString("BlenderGeneratorUI::generatorTableElementSelected(): An unwanted element selected event was recieved!!"));;
    }
}

void BlenderGeneratorUI::connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties && generators){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(generatorTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int)), generators, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void BlenderGeneratorUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::loadBinding(): The data is NULL!!"));
    }
}

void BlenderGeneratorUI::selectTableToView(bool viewproperties, const QString & path){
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
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorUI::selectTableToView(): The data is NULL!!"));
    }
}

void BlenderGeneratorUI::variableRenamed(const QString & name, int index){
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

