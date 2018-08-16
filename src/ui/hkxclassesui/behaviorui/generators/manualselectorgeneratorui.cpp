#include "ManualSelectorGeneratorUI.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbblendergeneratorchild.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/generators/bsboneswitchgeneratorbonedata.h"
#include "src/hkxclasses/behavior/generators/hkbmanualselectorgenerator.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/mainwindow.h"

#include <QHeaderView>

#include "src/ui/genericdatawidgets.h"

#define BASE_NUMBER_OF_ROWS 4

#define NAME_ROW 0
#define SELECTED_GENERATOR_INDEX_ROW 1
#define CURRENT_GENERATOR_INDEX_ROW 2
#define ADD_GENERATOR_ROW 3

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList ManualSelectorGeneratorUI::types = {
    "None",
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
    "hkbBehaviorReferenceGenerator",
    "BGSGamebryoSequenceGenerator"
};

QStringList ManualSelectorGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

ManualSelectorGeneratorUI::ManualSelectorGeneratorUI()
    : behaviorView(nullptr),
      bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      typeSelectorCB(new ComboBox),
      selectedGeneratorIndex(new SpinBox),
      currentGeneratorIndex(new SpinBox)
{
    table->setAcceptDrops(true);
    table->viewport()->setAcceptDrops(true);
    table->setDragDropOverwriteMode(true);
    table->setDropIndicatorShown(true);
    table->setDragDropMode(QAbstractItemView::InternalMove);
    table->setRowSwapRange(BASE_NUMBER_OF_ROWS);
    setTitle("hkbManualSelectorGenerator");
    typeSelectorCB->insertItems(0, types);
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(SELECTED_GENERATOR_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("selectedGeneratorIndex"));
    table->setItem(SELECTED_GENERATOR_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(SELECTED_GENERATOR_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(SELECTED_GENERATOR_INDEX_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(SELECTED_GENERATOR_INDEX_ROW, VALUE_COLUMN, selectedGeneratorIndex);
    table->setItem(CURRENT_GENERATOR_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("currentGeneratorIndex"));
    table->setItem(CURRENT_GENERATOR_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(CURRENT_GENERATOR_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(CURRENT_GENERATOR_INDEX_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(CURRENT_GENERATOR_INDEX_ROW, VALUE_COLUMN, currentGeneratorIndex);
    table->setItem(ADD_GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("Add Generator", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new generator of the type specified in the adjacent combo box"));
    table->setCellWidget(ADD_GENERATOR_ROW, TYPE_COLUMN, typeSelectorCB);
    table->setItem(ADD_GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Generator", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected generator"));
    table->setItem(ADD_GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("Change Selected Generator", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to change the selected generator"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void ManualSelectorGeneratorUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(selectedGeneratorIndex, SIGNAL(editingFinished()), this, SLOT(setSelectedGeneratorIndex()), Qt::UniqueConnection);
    connect(currentGeneratorIndex, SIGNAL(editingFinished()), this, SLOT(setCurrentGeneratorIndex()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(table, SIGNAL(itemDropped(int,int)), this, SLOT(swapGeneratorIndices(int,int)), Qt::UniqueConnection);
}

void ManualSelectorGeneratorUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(selectedGeneratorIndex, SIGNAL(editingFinished()), this, SLOT(setSelectedGeneratorIndex()));
    disconnect(currentGeneratorIndex, SIGNAL(editingFinished()), this, SLOT(setCurrentGeneratorIndex()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(table, SIGNAL(itemDropped(int,int)), this, SLOT(swapGeneratorIndices(int,int)));
}

void ManualSelectorGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    hkbVariableBindingSet *varBind = nullptr;
    if (data){
        if (data->getSignature() == HKB_MANUAL_SELECTOR_GENERATOR){
            bsData = static_cast<hkbManualSelectorGenerator *>(data);
            name->setText(bsData->getName());
            selectedGeneratorIndex->setValue(bsData->selectedGeneratorIndex);
            currentGeneratorIndex->setValue(bsData->currentGeneratorIndex);
            varBind = bsData->getVariableBindingSetData();
            if (varBind){
                loadBinding(SELECTED_GENERATOR_INDEX_ROW, BINDING_COLUMN, varBind, "selectedGeneratorIndex");
                loadBinding(CURRENT_GENERATOR_INDEX_ROW, BINDING_COLUMN, varBind, "currentGeneratorIndex");
            }else{
                table->item(SELECTED_GENERATOR_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(CURRENT_GENERATOR_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
            loadDynamicTableRows();
        }else{
            CRITICAL_ERROR_MESSAGE(QString("ManualSelectorGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::loadData(): The data passed to the UI is nullptr!!!");
    }
    connectSignals();
}

void ManualSelectorGeneratorUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_GENERATOR_ROW + bsData->generators.size() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        hkbGenerator *child = nullptr;
        for (auto i = ADD_GENERATOR_ROW + 1, j = 0; j < bsData->generators.size(); i++, j++){
            child = static_cast<hkbGenerator *>(bsData->generators.at(j).data());
            if (child){
                setRowItems(i, child->getName(), child->getClassname(), "Remove", "Edit", "Double click to remove this generator", VIEW_GENERATORS_TABLE_TIP);
            }else{
                setRowItems(i, "NONE", "NONE", "Remove", "Edit", "Double click to remove this generator", VIEW_GENERATORS_TABLE_TIP);
                //CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::loadData(): Null state found!!!");
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::loadDynamicTableRows(): The data is nullptr!!");
    }
    //table->setSortingEnabled(true);
}

void ManualSelectorGeneratorUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
    if (table->item(row, NAME_COLUMN)){
        table->item(row, NAME_COLUMN)->setText(name);
    }else{
        table->setItem(row, NAME_COLUMN, new TableWidgetItem(name, Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::white), QBrush(Qt::black), DRAG_DROP_N_SWAP));
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

void ManualSelectorGeneratorUI::connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties && generators){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(setGenerator(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int,QString,QStringList)), generators, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void ManualSelectorGeneratorUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = bsData->getVariableBindingSetData();
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->getVariableBindingSet() = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1), type)) ||
                  (isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1), type))){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->getVariableBindingSet() = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setBinding(): The data is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case SELECTED_GENERATOR_INDEX_ROW:
            if (table->item(SELECTED_GENERATOR_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "selectedGeneratorIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case CURRENT_GENERATOR_INDEX_ROW:
            if (table->item(CURRENT_GENERATOR_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "currentGeneratorIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        default:
            return;
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setBindingVariable(): The data is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    table->item(row, column)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, column)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::loadBinding(): The data is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::variableRenamed(const QString &name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("ManualSelectorGeneratorUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        index--;
        bind = bsData->getVariableBindingSetData();
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("selectedGeneratorIndex");
            if (bindIndex == index){
                table->item(SELECTED_GENERATOR_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("currentGeneratorIndex");
            if (bindIndex == index){
                table->item(CURRENT_GENERATOR_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BlenderGeneratorUI::variableRenamed(): The data is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::generatorRenamed(const QString &name, int index){
    int generatorIndex = table->currentRow() - BASE_NUMBER_OF_ROWS;
    if (bsData){
        if (generatorIndex >= 0 && generatorIndex < bsData->generators.size()){
            index--;
            if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generators.at(generatorIndex))){
                table->item(table->currentRow(), VALUE_COLUMN)->setText(name);
            }
        }else{
            WARNING_MESSAGE("ManualSelectorGeneratorUI::generatorRenamed(): Invalid generator index selected!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::generatorRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}

void ManualSelectorGeneratorUI::selectTableToView(bool viewproperties, const QString &path){
    if (bsData){
        if (viewproperties){
            if (bsData->getVariableBindingSetData()){
                emit viewProperties(bsData->getVariableBindingSetData()->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewProperties(0, QString(), QStringList());
            }
        }else{
            if (bsData->getVariableBindingSetData()){
                emit viewVariables(bsData->getVariableBindingSetData()->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewVariables(0, QString(), QStringList());
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::selectTableToView(): The data is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::setName(){
    if (bsData){
        if (bsData->getName() != name->text()){
            bsData->getName() = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->setIsFileChanged(true);
            emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setName(): The data is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::setSelectedGeneratorIndex(){
    if (bsData){
        if (bsData->selectedGeneratorIndex != selectedGeneratorIndex->value()){
            bsData->selectedGeneratorIndex = selectedGeneratorIndex->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setSelectedGeneratorIndex(): The data is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::setCurrentGeneratorIndex(){
    if (bsData){
        if (bsData->currentGeneratorIndex != currentGeneratorIndex->value()){
            bsData->currentGeneratorIndex = currentGeneratorIndex->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setCurrentGeneratorIndex(): The data is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::viewSelectedChild(int row, int column){
    int result = -1;
    bool properties = false;
    if (bsData){
        if (row < ADD_GENERATOR_ROW && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case SELECTED_GENERATOR_INDEX_ROW:
                    if (table->item(SELECTED_GENERATOR_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "selectedGeneratorIndex");
                    break;
                case CURRENT_GENERATOR_INDEX_ROW:
                    if (table->item(CURRENT_GENERATOR_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "currentGeneratorIndex");
                    break;
                }
            }
        }else if (row == ADD_GENERATOR_ROW && column == NAME_COLUMN){
            addGenerator();
        }else if (row > ADD_GENERATOR_ROW && row < ADD_GENERATOR_ROW + bsData->generators.size() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->generators.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    QStringList list = {hkbStateMachineStateInfo::getClassname(), hkbBlenderGeneratorChild::getClassname(), BSBoneSwitchGeneratorBoneData::getClassname()};
                    emit viewGenerators(bsData->getIndexOfGenerator(bsData->generators.at(result)) + 1, QString(), list);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the generator \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeGenerator(result);
                    }
                }
            }else{
                WARNING_MESSAGE("ManualSelectorGeneratorUI::viewSelectedChild(): Invalid index of child to view!!");
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::swapGeneratorIndices(int index1, int index2){
    HkxObject *gen1;
    HkxObject *gen2;
    if (bsData){
        index1 = index1 - BASE_NUMBER_OF_ROWS;
        index2 = index2 - BASE_NUMBER_OF_ROWS;
        if (bsData->generators.size() > index1 && bsData->generators.size() > index2 && index1 != index2 && index1 >= 0 && index2 >= 0){
            gen1 = bsData->generators.at(index1).data();
            gen2 = bsData->generators.at(index2).data();
            bsData->generators[index1] = HkxSharedPtr(gen2);
            bsData->generators[index2] = HkxSharedPtr(gen1);
            if (behaviorView->getSelectedItem()){
                behaviorView->getSelectedItem()->reorderChildren();
            }else{
                CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::swapGeneratorIndices(): No item selected!!");
            }
            if (bsData->selectedGeneratorIndex == index1){
                bsData->selectedGeneratorIndex = index2;
            }else if (bsData->selectedGeneratorIndex == index2){
                bsData->selectedGeneratorIndex = index1;
            }
            if (bsData->currentGeneratorIndex == index1){
                bsData->currentGeneratorIndex = index2;
            }else if (bsData->currentGeneratorIndex == index2){
                bsData->currentGeneratorIndex = index1;
            }
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("ManualSelectorGeneratorUI::swapGeneratorIndices(): Cannot swap these rows!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::swapGeneratorIndices(): The data is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::setGenerator(int index, const QString &name){
    DataIconManager *ptr = nullptr;
    int generatorIndex = table->currentRow() - BASE_NUMBER_OF_ROWS;
    if (bsData){
        if (behaviorView){
            if (generatorIndex >= 0 && generatorIndex < bsData->generators.size()){
                ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
                if (ptr){
                    if (name != ptr->getName()){
                        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setGenerator():The name of the selected object does not match it's name in the object selection table!!!");
                        return;
                    }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->generators.at(generatorIndex).data()), generatorIndex, ptr, false)){
                        WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!");
                        return;
                    }
                }else{
                    if (behaviorView->getSelectedItem()){
                        behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->generators.at(generatorIndex).data())), generatorIndex);
                    }else{
                        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setGenerator(): The selected icon is nullptr!!");
                        return;
                    }
                }
                behaviorView->removeGeneratorData();
                table->item(table->currentRow(), NAME_COLUMN)->setText(name);
                bsData->setIsFileChanged(true);
                loadDynamicTableRows();
            }else{
                CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setGenerator(): Invalid generator index selected!!");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setGenerator(): The 'behaviorView' pointer is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::setGenerator(): The 'bsData' pointer is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::addGenerator(){
    Generator_Type typeEnum;
    if (bsData && behaviorView){
        typeEnum = static_cast<Generator_Type>(typeSelectorCB->currentIndex());
        switch (typeEnum){
        case NONE:
            bsData->generators.append(HkxSharedPtr());
            break;
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
        case GAMEBYRO_SEQUENCE_GENERATOR:
            behaviorView->appendBGSGamebryoSequenceGenerator();
            break;
        default:
            CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::addGenerator(): Invalid typeEnum!!");
            return;
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::addGenerator(): The data or behavior graph pointer is nullptr!!");
    }
}

void ManualSelectorGeneratorUI::removeGenerator(int index){
    if (bsData && behaviorView){
        if (index < bsData->generators.size() && index >= 0){
            if (!bsData->generators.at(index).data()){
                bsData->generators.removeAt(index);
            }else{
                behaviorView->removeItemFromGraph(behaviorView->getSelectedIconsChildIcon(bsData->generators.at(index).data()), index);//Reorderchildren?
                behaviorView->removeObjects();
            }
        }else{
            WARNING_MESSAGE("ManualSelectorGeneratorUI::removeGenerator(): Invalid index of generator to remove!!");
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("ManualSelectorGeneratorUI::removeGenerator(): The data or behavior graph pointer is nullptr!!");
    }
}
