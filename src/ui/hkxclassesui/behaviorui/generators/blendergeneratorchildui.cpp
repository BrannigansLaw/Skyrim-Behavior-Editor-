#include "blendergeneratorchildui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbblendergeneratorchild.h"
#include "src/hkxclasses/behavior/generators/hkbblendergenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/hkxclassesui/behaviorui/boneweightarrayui.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/behavior/hkbboneweightarray.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"

#include "src/ui/genericdatawidgets.h"
#include <QStackedLayout>
#include <QHeaderView>
#include <QGroupBox>

#define BASE_NUMBER_OF_ROWS 4

#define BONE_WEIGHTS_ROW 0
#define WEIGHT_ROW 1
#define WORLD_FROM_MODEL_WEIGHT_ROW 2
#define GENERATOR_ROW 3

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BlenderGeneratorChildUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BlenderGeneratorChildUI::BlenderGeneratorChildUI()
    : behaviorView(nullptr),
      bsData(nullptr),
      childIndex(0),
      topLyt(new QGridLayout),
      groupBox(new QGroupBox("hkbBlenderGeneratorChild")),
      returnPB(new QPushButton("Return")),
      table(new TableWidget(QColor(Qt::white))),
      boneWeights(new CheckButtonCombo("Edit")),
      boneWeightArrayUI(new BoneWeightArrayUI),
      weight(new DoubleSpinBox),
      worldFromModelWeight(new DoubleSpinBox)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(BONE_WEIGHTS_ROW, NAME_COLUMN, new TableWidgetItem("boneWeights"));
    table->setItem(BONE_WEIGHTS_ROW, TYPE_COLUMN, new TableWidgetItem("hkbBoneWeightArray", Qt::AlignCenter));
    table->setItem(BONE_WEIGHTS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(BONE_WEIGHTS_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(BONE_WEIGHTS_ROW, VALUE_COLUMN, boneWeights);
    table->setItem(WEIGHT_ROW, NAME_COLUMN, new TableWidgetItem("weight"));
    table->setItem(WEIGHT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(WEIGHT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(WEIGHT_ROW, VALUE_COLUMN, weight);
    table->setItem(WORLD_FROM_MODEL_WEIGHT_ROW, NAME_COLUMN, new TableWidgetItem("worldFromModelWeight"));
    table->setItem(WORLD_FROM_MODEL_WEIGHT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(WORLD_FROM_MODEL_WEIGHT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(WORLD_FROM_MODEL_WEIGHT_ROW, VALUE_COLUMN, worldFromModelWeight);
    table->setItem(GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("generator"));
    table->setItem(GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbGenerator", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_GENERATORS_TABLE_TIP));
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(boneWeightArrayUI);
    connectSignals();
}

void BlenderGeneratorChildUI::connectSignals(){
    connect(boneWeightArrayUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(weight, SIGNAL(editingFinished()), this, SLOT(setWeight()), Qt::UniqueConnection);
    connect(worldFromModelWeight, SIGNAL(editingFinished()), this, SLOT(setWorldFromModelWeight()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
    connect(boneWeights, SIGNAL(pressed()), this, SLOT(viewBoneWeights()), Qt::UniqueConnection);
    connect(boneWeights, SIGNAL(enabled(bool)), this, SLOT(toggleBoneWeights(bool)), Qt::UniqueConnection);
    connect(returnPB, SIGNAL(clicked(bool)), this, SIGNAL(returnToParent(bool)), Qt::UniqueConnection);
}

void BlenderGeneratorChildUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(clicked(bool)), this, SIGNAL(returnToParent(bool)));
    disconnect(boneWeightArrayUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(weight, SIGNAL(editingFinished()), this, SLOT(setWeight()));
    disconnect(worldFromModelWeight, SIGNAL(editingFinished()), this, SLOT(setWorldFromModelWeight()));
    disconnect(boneWeights, SIGNAL(pressed()), this, SLOT(viewBoneWeights()));
    disconnect(boneWeights, SIGNAL(enabled(bool)), this, SLOT(toggleBoneWeights(bool)));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BlenderGeneratorChildUI::loadData(HkxObject *data, int childindex){
    disconnectSignals();
    hkbVariableBindingSet *varBind = nullptr;
    if (data){
        if (data->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
            childIndex = childindex;
            bsData = static_cast<hkbBlenderGeneratorChild *>(data);
            if (bsData->boneWeights.data()){
                boneWeights->setChecked(true);
            }else{
                boneWeights->setChecked(false);
            }
            weight->setValue(bsData->weight);
            worldFromModelWeight->setValue(bsData->worldFromModelWeight);
            if (bsData->generator.data()){
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->generator.data())->getName());
            }else{
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(BONE_WEIGHTS_ROW, BINDING_COLUMN, varBind, "boneWeights");
                loadBinding(WEIGHT_ROW, BINDING_COLUMN, varBind, "weight");
                loadBinding(WORLD_FROM_MODEL_WEIGHT_ROW, BINDING_COLUMN, varBind, "worldFromModelWeight");
            }else{
                table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(WEIGHT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(WORLD_FROM_MODEL_WEIGHT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
            if (bsData->isParametricBlend()){
                weight->setMaximum(1);
                weight->setMinimum(0);
            }else{
                weight->setMaximum(std::numeric_limits<int>::max());
                weight->setMinimum(std::numeric_limits<int>::min());
            }
        }else{
            FATAL_RUNTIME_ERROR(QString("BlenderGeneratorChildUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::loadData(): The data passed to the UI is nullptr!!!");
    }
    connectSignals();
}

void BlenderGeneratorChildUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::loadBinding(): The data is nullptr!!");
    }
}

void BlenderGeneratorChildUI::setGenerator(int index, const QString & name){
    hkbBlenderGenerator *gen = nullptr;
    DataIconManager *ptr = nullptr;
    if (bsData){
        if (behaviorView){
            gen = static_cast<hkbBlenderGenerator *>(bsData->getParentGenerator());
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            if (ptr){
                if (name != ptr->getName()){
                    FATAL_RUNTIME_ERROR("::setDefaultGenerator():The name of the selected object does not match it's name in the object selection table!!!");
                    return;
                }else if (!gen){
                    FATAL_RUNTIME_ERROR("The currently loaded 'hkbBlenderGeneratorChild' has no parent 'hkbBlenderGenerator' or 'hkbPoseMatchingGenerator'!!!");
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->generator.data()), ptr, false)){
                    WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!");
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->generator.data())), childIndex);
                }else{
                    FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::setGenerator(): The selected icon is nullptr!!");
                    return;
                }
            }
            behaviorView->removeGeneratorData();
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->setIsChanged(true);
            emit returnToParent(true);
        }else{
            FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::setGenerator(): The 'behaviorView' pointer is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::setGenerator(): The 'bsData' pointer is nullptr!!");
    }
}

void BlenderGeneratorChildUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                varBind->addBinding(path, index - 1,hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY);
            }else{
                varBind->addBinding(path, index - 1,hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE);
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::setBinding(): The 'bsData' pointer is nullptr!!");
    }
}

void BlenderGeneratorChildUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case BONE_WEIGHTS_ROW:
            if (table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "boneWeights", VARIABLE_TYPE_POINTER, isProperty);
            break;
        case WEIGHT_ROW:
            if (table->item(WEIGHT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "weight", VARIABLE_TYPE_REAL, isProperty);
            break;
        case WORLD_FROM_MODEL_WEIGHT_ROW:
            if (table->item(WORLD_FROM_MODEL_WEIGHT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "worldFromModelWeight", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::setBindingVariable(): The 'bsData' pointer is nullptr!!");
    }
}

void BlenderGeneratorChildUI::setWeight(){
    if (bsData){
        if (bsData->weight != weight->value()){
            bsData->weight = weight->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::setWeight(): The 'bsData' pointer is nullptr!!");
    }
}

void BlenderGeneratorChildUI::setWorldFromModelWeight(){
    if (bsData){
        if (bsData->worldFromModelWeight != worldFromModelWeight->value()){
            bsData->worldFromModelWeight = worldFromModelWeight->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::setWorldFromModelWeight(): The 'bsData' pointer is nullptr!!");
    }
}

void BlenderGeneratorChildUI::viewBoneWeights(){
    if (bsData){
        boneWeightArrayUI->loadData(bsData->boneWeights.data());
        setCurrentIndex(BONE_WEIGHT_ARRAY_WIDGET);
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::viewBoneWeights(): The data is nullptr!!");
    }
}

void BlenderGeneratorChildUI::toggleBoneWeights(bool enable){
    if (bsData){
        if (!enable){
            bsData->boneWeights = HkxSharedPtr();
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
        }else if (enable && !bsData->boneWeights.data()){
            bsData->boneWeights = HkxSharedPtr(new hkbBoneWeightArray(bsData->getParentFile(), -1, static_cast<BehaviorFile *>(bsData->getParentFile())->getNumberOfBones()));
        }
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::toggleBoneWeights(): The data is nullptr!!");
    }
}

void BlenderGeneratorChildUI::selectTableToView(bool viewproperties, const QString & path){
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
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::selectTableToView(): The data is nullptr!!");
    }
}

void BlenderGeneratorChildUI::viewSelected(int row, int column){
    if (bsData){
        bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case BONE_WEIGHTS_ROW:
                if (table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "boneWeights");
                break;
            case WEIGHT_ROW:
                if (table->item(WEIGHT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "weight");
                break;
            case WORLD_FROM_MODEL_WEIGHT_ROW:
                if (table->item(WORLD_FROM_MODEL_WEIGHT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "worldFromModelWeight");
                break;
            }
        }else if (row == GENERATOR_ROW && column == VALUE_COLUMN){
            emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator) + 1);
        }
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void BlenderGeneratorChildUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}

void BlenderGeneratorChildUI::variableRenamed(const QString & name, int index){
    if (bsData){
        //index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("boneWeights");
            if (bindIndex == index){
                table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("weight");
            if (bindIndex == index){
                table->item(WEIGHT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("worldFromModelWeight");
            if (bindIndex == index){
                table->item(WORLD_FROM_MODEL_WEIGHT_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

void BlenderGeneratorChildUI::generatorRenamed(const QString &name, int index){
    if (bsData){
        //index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator)){
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        FATAL_RUNTIME_ERROR("BlenderGeneratorChildUI::generatorRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

void BlenderGeneratorChildUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}
