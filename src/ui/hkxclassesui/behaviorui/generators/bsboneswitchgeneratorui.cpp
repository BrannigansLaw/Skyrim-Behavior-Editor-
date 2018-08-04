#include "bsboneswitchgeneratorui.h"
#include "bsboneswitchgeneratorbonedataui.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/bsboneswitchgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbblendergeneratorchild.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/generators/bsboneswitchgeneratorbonedata.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include "src/ui/genericdatawidgets.h"
#include <QStackedLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 3

#define NAME_ROW 0
#define DEFAULT_GENERATOR_ROW 1
#define ADD_CHILD_ROW 2

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSBoneSwitchGeneratorUI::types = {
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

QStringList BSBoneSwitchGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSBoneSwitchGeneratorUI::BSBoneSwitchGeneratorUI()
    : behaviorView(nullptr),
      bsData(nullptr),
      groupBox(new QGroupBox("BSBoneSwitchGenerator")),
      childUI(new BSBoneSwitchGeneratorBoneDataUI),
      topLyt(new QGridLayout),
      typeSelectorCB(new ComboBox),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit)
{
    table->setAcceptDrops(true);
    table->viewport()->setAcceptDrops(true);
    table->setDragDropOverwriteMode(true);
    table->setDropIndicatorShown(true);
    table->setDragDropMode(QAbstractItemView::InternalMove);
    table->setRowSwapRange(BASE_NUMBER_OF_ROWS);
    typeSelectorCB->insertItems(0, types);
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(DEFAULT_GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("pDefaultGenerator"));
    table->setItem(DEFAULT_GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbGenerator", Qt::AlignCenter));
    table->setItem(DEFAULT_GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(DEFAULT_GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_GENERATORS_TABLE_TIP));
    table->setItem(ADD_CHILD_ROW, NAME_COLUMN, new TableWidgetItem("Add Bone Data With Generator", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new bone switch generator bone data with a generator of the type specified in the adjacent combo box"));
    table->setCellWidget(ADD_CHILD_ROW, TYPE_COLUMN, typeSelectorCB);
    table->setItem(ADD_CHILD_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Bone Data", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected bone switch generator bone data"));
    table->setItem(ADD_CHILD_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Bone Data", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected bone switch generator bone data"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(childUI);
    childUI->returnPB->setVisible(true);
    connectSignals();
}

void BSBoneSwitchGeneratorUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(table, SIGNAL(itemDropped(int,int)), this, SLOT(swapGeneratorIndices(int,int)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(returnToParent(bool)), this, SLOT(returnToWidget(bool)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewVariables(int,QString,QStringList)), this, SIGNAL(viewVariables(int,QString,QStringList)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewProperties(int,QString,QStringList)), this, SIGNAL(viewProperties(int,QString,QStringList)), Qt::UniqueConnection);
    connect(childUI, SIGNAL(viewGenerators(int,QString,QStringList)), this, SIGNAL(viewGenerators(int,QString,QStringList)), Qt::UniqueConnection);
}

void BSBoneSwitchGeneratorUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(table, SIGNAL(itemDropped(int,int)), this, SLOT(swapGeneratorIndices(int,int)));
    disconnect(childUI, SIGNAL(returnToParent(bool)), this, SLOT(returnToWidget(bool)));
    disconnect(childUI, SIGNAL(viewVariables(int,QString,QStringList)), this, SIGNAL(viewVariables(int,QString,QStringList)));
    disconnect(childUI, SIGNAL(viewProperties(int,QString,QStringList)), this, SIGNAL(viewProperties(int,QString,QStringList)));
    disconnect(childUI, SIGNAL(viewGenerators(int,QString,QStringList)), this, SIGNAL(viewGenerators(int,QString,QStringList)));
}

void BSBoneSwitchGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    if (data){
        if (data->getSignature() == BS_BONE_SWITCH_GENERATOR){
            bsData = static_cast<BSBoneSwitchGenerator *>(data);
            name->setText(bsData->name);
            if (bsData->pDefaultGenerator.data()){
                table->item(DEFAULT_GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->pDefaultGenerator.data())->getName());
            }else{
                table->item(DEFAULT_GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
            loadDynamicTableRows();
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSBoneSwitchGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::loadData(): Attempting to load a null pointer!!");
    }
    connectSignals();
}

void BSBoneSwitchGeneratorUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_CHILD_ROW + bsData->ChildrenA.size() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        BSBoneSwitchGeneratorBoneData *child = nullptr;
        for (auto i = ADD_CHILD_ROW + 1, j = 0; j < bsData->ChildrenA.size(); i++, j++){
            child = static_cast<BSBoneSwitchGeneratorBoneData *>(bsData->ChildrenA.at(j).data());
            if (child){
                setRowItems(i, "Bone Data "+QString::number(j), child->getClassname(), "Remove", "Edit", "Double click to remove this bone data", "Double click to edit this bone data");
            }else{
                CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::loadData(): Null bone data found!!!");
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::loadDynamicTableRows(): The data is nullptr!!");
    }
    //table->setSortingEnabled(true);
}

void BSBoneSwitchGeneratorUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
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

void BSBoneSwitchGeneratorUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            for (auto i = 0; i < bsData->ChildrenA.size(); i++){
                if (bsData->ChildrenA.at(i).data()){
                    static_cast<DataIconManager*>(bsData->ChildrenA.at(i).data())->updateIconNames();
                }else{
                    CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::setName():\n Children contain nullptr's!!!");
                }
            }
            emit generatorNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::setName(): The data is nullptr!!");
    }
}

void BSBoneSwitchGeneratorUI::swapGeneratorIndices(int index1, int index2){
    HkxObject *gen1;
    HkxObject *gen2;
    if (bsData){
        index1 = index1 - BASE_NUMBER_OF_ROWS;
        index2 = index2 - BASE_NUMBER_OF_ROWS;
        if (bsData->ChildrenA.size() > index1 && bsData->ChildrenA.size() > index2 && index1 != index2 && index1 >= 0 && index2 >= 0){
            gen1 = bsData->ChildrenA.at(index1).data();
            gen2 = bsData->ChildrenA.at(index2).data();
            bsData->ChildrenA[index1] = HkxSharedPtr(gen2);
            bsData->ChildrenA[index2] = HkxSharedPtr(gen1);
            if (behaviorView->getSelectedItem()){
                behaviorView->getSelectedItem()->reorderChildren();
            }else{
                CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::swapGeneratorIndices(): No item selected!!");
            }
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("BSBoneSwitchGeneratorUI::swapGeneratorIndices(): Cannot swap these rows!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::swapGeneratorIndices(): The data is nullptr!!");
    }
}

void BSBoneSwitchGeneratorUI::addChildWithGenerator(){
    Generator_Type typeEnum;
    if (bsData && behaviorView){
        typeEnum = static_cast<Generator_Type>(typeSelectorCB->currentIndex());
        behaviorView->appendBoneSwitchGeneratorChild();
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
        case GAMEBYRO_SEQUENCE_GENERATOR:
            behaviorView->appendBGSGamebryoSequenceGenerator();
            break;
        default:
            CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::addChild(): Invalid typeEnum!!");
            return;
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::addChild(): The data or behavior graph pointer is nullptr!!");
    }
}

void BSBoneSwitchGeneratorUI::removeChild(int index){
    BSBoneSwitchGeneratorBoneData *child = nullptr;
    if (bsData && behaviorView){
        if (index < bsData->ChildrenA.size() && index >= 0){
            child = static_cast<BSBoneSwitchGeneratorBoneData *>(bsData->ChildrenA.at(index).data());
            behaviorView->removeItemFromGraph(behaviorView->getSelectedIconsChildIcon(child->getChildren().first()), index);//Reorderchildren?
            behaviorView->removeObjects();
        }else{
            WARNING_MESSAGE("BSBoneSwitchGeneratorUI::removeChild(): Invalid index of child to remove!!");
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::removeChild(): The data or behavior graph pointer is nullptr!!");
    }
}

void BSBoneSwitchGeneratorUI::viewSelectedChild(int row, int column){
    int result = -1;
    if (bsData){
        if (row == DEFAULT_GENERATOR_ROW && column == VALUE_COLUMN){
            QStringList list = {hkbStateMachineStateInfo::getClassname(), hkbBlenderGeneratorChild::getClassname(), BSBoneSwitchGeneratorBoneData::getClassname()};
            emit viewGenerators(bsData->getIndexOfGenerator(bsData->pDefaultGenerator) + 1, QString(), list);
        }else if (row == ADD_CHILD_ROW && column == NAME_COLUMN){
            addChildWithGenerator();
        }else if (row > ADD_CHILD_ROW && row < ADD_CHILD_ROW + bsData->ChildrenA.size() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->ChildrenA.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    childUI->loadData(static_cast<BSBoneSwitchGeneratorBoneData *>(bsData->ChildrenA.at(result).data()), result);
                    setCurrentIndex(CHILD_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the bone data \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeChild(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::viewSelectedChild(): Invalid index of bone data to view!!");
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void BSBoneSwitchGeneratorUI::setDefaultGenerator(int index, const QString & name){
    DataIconManager *ptr = nullptr;
    int indexOfGenerator = -1;
    if (bsData){
        if (currentIndex() == MAIN_WIDGET){//Not neccessary???
            if (behaviorView){
                ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
                indexOfGenerator = bsData->getIndexOfObj(static_cast<DataIconManager*>(bsData->pDefaultGenerator.data()));
                if (ptr){
                    if (name != ptr->getName()){
                        CRITICAL_ERROR_MESSAGE("::setDefaultGenerator():The name of the selected object does not match it's name in the object selection table!!!");
                        return;
                    }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->pDefaultGenerator.data()), 0, ptr, false)){
                        WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!");
                        return;
                    }
                }else{
                    if (behaviorView->getSelectedItem()){
                        behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->pDefaultGenerator.data())), indexOfGenerator);
                    }else{
                        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::setDefaultGenerator(): The selected icon is nullptr!!");
                        return;
                    }
                }
                behaviorView->removeGeneratorData();
                table->item(DEFAULT_GENERATOR_ROW, VALUE_COLUMN)->setText(name);
                bsData->setIsFileChanged(true);
            }else{
                CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::setDefaultGenerator(): The 'behaviorView' pointer is nullptr!!");
            }
        }else{
            childUI->setGenerator(index, name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::setDefaultGenerator(): The 'bsData' pointer is nullptr!!");
    }
}

void BSBoneSwitchGeneratorUI::returnToWidget(bool reloadData){
    if (reloadData){
        loadDynamicTableRows();
    }
    setCurrentIndex(MAIN_WIDGET);
}

void BSBoneSwitchGeneratorUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case CHILD_WIDGET:
        childUI->setBindingVariable(index, name);
        break;
    default:
        WARNING_MESSAGE("BSBoneSwitchGeneratorUI::variableTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void BSBoneSwitchGeneratorUI::generatorTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setDefaultGenerator(index, name);
        break;
    case CHILD_WIDGET:
        childUI->setGenerator(index, name);
        break;
    default:
        WARNING_MESSAGE("BSBoneSwitchGeneratorUI::generatorTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void BSBoneSwitchGeneratorUI::connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties && generators){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(generatorTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int,QString,QStringList)), generators, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void BSBoneSwitchGeneratorUI::variableRenamed(const QString & name, int index){
    if (name == ""){
        WARNING_MESSAGE("BSBoneSwitchGeneratorUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        index--;
        if (currentIndex() == CHILD_WIDGET){
            childUI->variableRenamed(name, index);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorUI::variableRenamed(): The data is nullptr!!");
    }
}

void BSBoneSwitchGeneratorUI::generatorRenamed(const QString &name, int index){
    if (name == ""){
        WARNING_MESSAGE("BSBoneSwitchGeneratorUI::generatorRenamed(): The new variable name is the empty string!!");
    }
    index--;
    if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pDefaultGenerator)){
        table->item(DEFAULT_GENERATOR_ROW, VALUE_COLUMN)->setText(name);
    }
    switch (currentIndex()){
    case CHILD_WIDGET:
        childUI->generatorRenamed(name, index);
        break;
    }
}

void BSBoneSwitchGeneratorUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
    setCurrentIndex(MAIN_WIDGET);
    childUI->setBehaviorView(view);
}

