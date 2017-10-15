#include "bsboneswitchgeneratorbonedataui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/bsboneswitchgenerator.h"
#include "src/hkxclasses/behavior/generators/bsboneswitchgeneratorbonedata.h"
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

#define BASE_NUMBER_OF_ROWS 2

#define BONE_WEIGHTS_ROW 0
#define GENERATOR_ROW 1

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSBoneSwitchGeneratorBoneDataUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSBoneSwitchGeneratorBoneDataUI::BSBoneSwitchGeneratorBoneDataUI()
    : behaviorView(NULL),
      bsData(NULL),
      childIndex(0),
      topLyt(new QGridLayout),
      groupBox(new QGroupBox("BSBoneSwitchGeneratorBoneData")),
      returnPB(new QPushButton("Return")),
      table(new TableWidget(QColor(Qt::white))),
      spBoneWeight(new CheckButtonCombo("Click to edit")),
      boneWeightArrayUI(new BoneWeightArrayUI)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(BONE_WEIGHTS_ROW, NAME_COLUMN, new TableWidgetItem("spBoneWeight"));
    table->setItem(BONE_WEIGHTS_ROW, TYPE_COLUMN, new TableWidgetItem("hkbBoneWeightArray", Qt::AlignCenter));
    table->setItem(BONE_WEIGHTS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(BONE_WEIGHTS_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(BONE_WEIGHTS_ROW, VALUE_COLUMN, spBoneWeight);
    table->setItem(GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("pGenerator"));
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

void BSBoneSwitchGeneratorBoneDataUI::connectSignals(){
    connect(boneWeightArrayUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
    connect(spBoneWeight, SIGNAL(pressed()), this, SLOT(viewBoneWeights()), Qt::UniqueConnection);
    connect(spBoneWeight, SIGNAL(enabled(bool)), this, SLOT(toggleBoneWeights(bool)), Qt::UniqueConnection);
    connect(returnPB, SIGNAL(clicked(bool)), this, SIGNAL(returnToParent(bool)), Qt::UniqueConnection);
}

void BSBoneSwitchGeneratorBoneDataUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(clicked(bool)), this, SIGNAL(returnToParent(bool)));
    disconnect(boneWeightArrayUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(spBoneWeight, SIGNAL(pressed()), this, SLOT(viewBoneWeights()));
    disconnect(spBoneWeight, SIGNAL(enabled(bool)), this, SLOT(toggleBoneWeights(bool)));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSBoneSwitchGeneratorBoneDataUI::loadData(HkxObject *data, int childindex){
    disconnectSignals();
    hkbVariableBindingSet *varBind = NULL;
    if (data){
        if (data->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA){
            childIndex = childindex;
            bsData = static_cast<BSBoneSwitchGeneratorBoneData *>(data);
            if (bsData->spBoneWeight.data()){
                spBoneWeight->setChecked(true);
            }else{
                spBoneWeight->setChecked(false);
            }
            if (bsData->pGenerator.data()){
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->pGenerator.data())->getName());
            }else{
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(BONE_WEIGHTS_ROW, BINDING_COLUMN, varBind, "spBoneWeight");
            }else{
                table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            (qFatal(QString("BSBoneSwitchGeneratorBoneDataUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data()));
        }
    }else{
        (qFatal("BSBoneSwitchGeneratorBoneDataUI::loadData(): The data passed to the UI is NULL!!!"));
    }
    connectSignals();
}

void BSBoneSwitchGeneratorBoneDataUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, false);
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
            (qFatal("BSBoneSwitchGeneratorBoneDataUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        (qFatal("BSBoneSwitchGeneratorBoneDataUI::loadBinding(): The data is NULL!!"));
    }
}

void BSBoneSwitchGeneratorBoneDataUI::setGenerator(int index, const QString & name){
    //BSBoneSwitchGenerator *gen = NULL;
    DataIconManager *ptr = NULL;
    if (bsData){
        if (behaviorView){
            //gen = static_cast<BSBoneSwitchGenerator *>(bsData->getParentGenerator());
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            if (ptr){
                if (name != ptr->getName()){
                    (qFatal("The name of the selected object does not match it's name in the object selection table!!!"));
                    return;
                }/*else if (!gen){
                    (qFatal("The currently loaded 'hkbBlenderGeneratorChild' has no parent 'hkbBlenderGenerator' or 'hkbPoseMatchingGenerator'!!!"));
                    return;
                }*/else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->pGenerator.data()), ptr, false)){
                    (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"));
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->pGenerator.data())), childIndex);
                }else{
                    (qFatal("BSBoneSwitchGeneratorBoneDataUI::setGenerator(): The selected icon is NULL!!"));
                    return;
                }
            }
            behaviorView->removeGeneratorData();
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
            emit returnToParent(true);
        }else{
            (qFatal("BSBoneSwitchGeneratorBoneDataUI::setGenerator(): The 'behaviorView' pointer is NULL!!"));
        }
    }else{
        (qFatal("BSBoneSwitchGeneratorBoneDataUI::setGenerator(): The 'bsData' pointer is NULL!!"));
    }
}

bool BSBoneSwitchGeneratorBoneDataUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                varBind->addBinding(path, variableName, index - 1,hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY);
            }else{
                varBind->addBinding(path, variableName, index - 1,hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE);
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        (qFatal("BSBoneSwitchGeneratorBoneDataUI::setBinding(): The 'bsData' pointer is NULL!!"));
        return false;
    }
    return true;
}

void BSBoneSwitchGeneratorBoneDataUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case BONE_WEIGHTS_ROW:
            if (table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "spBoneWeight", VARIABLE_TYPE_POINTER, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        (qFatal("BSBoneSwitchGeneratorBoneDataUI::setBindingVariable(): The 'bsData' pointer is NULL!!"));
    }
}

void BSBoneSwitchGeneratorBoneDataUI::viewBoneWeights(){
    if (bsData){
        boneWeightArrayUI->loadData(bsData->spBoneWeight.data());
        setCurrentIndex(BONE_WEIGHT_ARRAY_WIDGET);
    }else{
        (qFatal("BSBoneSwitchGeneratorBoneDataUI::viewBoneWeights(): The data is NULL!!"));
    }
}

void BSBoneSwitchGeneratorBoneDataUI::toggleBoneWeights(bool enable){
    if (bsData){
        if (!enable){
            bsData->spBoneWeight = HkxSharedPtr();
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
        }else if (enable && !bsData->spBoneWeight.data()){
            bsData->spBoneWeight = HkxSharedPtr(new hkbBoneWeightArray(bsData->getParentFile(), -1, static_cast<BehaviorFile *>(bsData->getParentFile())->getNumberOfBones()));
        }
    }else{
        (qFatal("BSBoneSwitchGeneratorBoneDataUI::toggleBoneWeights(): The data is NULL!!"));
    }
}

void BSBoneSwitchGeneratorBoneDataUI::selectTableToView(bool viewproperties, const QString & path){
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
        (qFatal("BSBoneSwitchGeneratorBoneDataUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSBoneSwitchGeneratorBoneDataUI::viewSelected(int row, int column){
    if (bsData){
        bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case BONE_WEIGHTS_ROW:
                if (table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "spBoneWeight");
                break;
            }
        }else if (row == GENERATOR_ROW && column == VALUE_COLUMN){
            emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pGenerator) + 1);
        }
    }else{
        (qFatal("BSBoneSwitchGeneratorBoneDataUI::viewSelected(): The 'bsData' pointer is NULL!!"));
    }
}

void BSBoneSwitchGeneratorBoneDataUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}

void BSBoneSwitchGeneratorBoneDataUI::variableRenamed(const QString & name, int index){
    if (bsData){
        //index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("spBoneWeight");
            if (bindIndex == index){
                table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        (qFatal("BSBoneSwitchGeneratorBoneDataUI::variableRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void BSBoneSwitchGeneratorBoneDataUI::generatorRenamed(const QString &name, int index){
    if (bsData){
        //index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pGenerator)){
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        (qFatal("BSBoneSwitchGeneratorBoneDataUI::generatorRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void BSBoneSwitchGeneratorBoneDataUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}
