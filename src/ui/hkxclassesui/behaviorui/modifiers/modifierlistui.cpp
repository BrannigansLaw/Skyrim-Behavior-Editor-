#include "modifierlistui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbmodifierlist.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/mainwindow.h"

#include <QHeaderView>

#include "src/ui/genericdatawidgets.h"

#define BASE_NUMBER_OF_ROWS 3

#define NAME_ROW 0
#define ENABLE_ROW 1
#define ADD_MODIFIER_ROW 2

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList ModifierListUI::types = {
    "hkbModifierList",
    "hkbTwistModifier",
    "hkbEventDrivenModifier",
    "BSIsActiveModifier",
    "BSLimbIKModifier",
    "BSInterpValueModifier",
    "BSGetTimeStepModifier",
    "hkbFootIkControlsModifier",
    "hkbGetHandleOnBoneModifier",
    "hkbTransformVectorModifier",
    "hkbProxyModifier",
    "hkbLookAtModifier",
    "hkbMirrorModifier",
    "hkbGetWorldFromModelModifier",
    "hkbSenseHandleModifier",
    "hkbEvaluateExpressionModifier",
    "hkbEvaluateHandleModifier",
    "hkbAttachmentModifier",
    "hkbAttributeModifier",
    "hkbCombineTransformsModifier",
    "hkbComputeRotationFromAxisAngleModifier",
    "hkbComputeRotationToTargetModifier",
    "hkbEventsFromRangeModifier",
    "hkbMoveCharacterModifier",
    "hkbExtractRagdollPoseModifier",
    "BSModifyOnceModifier",
    "BSEventOnDeactivateModifier",
    "BSEventEveryNEventsModifier",
    "BSRagdollContactListenerModifier",
    "hkbPoweredRagdollControlsModifier",
    "BSEventOnFalseToTrueModifier",
    "BSDirectAtModifier",
    "BSDistTriggerModifier",
    "BSDecomposeVectorModifier",
    "BSComputeAddBoneAnimModifier",
    "BSTweenerModifier",
    "BSIStateManagerModifier",
    "hkbTimerModifier",
    "hkbRotateCharacterModifier",
    "hkbDampingModifier",
    "hkbDelayedModifier",
    "hkbGetUpModifier",
    "hkbKeyframeBonesModifier",
    "hkbComputeDirectionModifier",
    "hkbRigidBodyRagdollControlsModifier",
    "BSSpeedSamplerModifier",
    "hkbDetectCloseToGroundModifier",
    "BSLookAtModifier",
    "BSTimerModifier",
    "BSPassByTargetTriggerModifier",
    "hkbHandIKControlsModifier"
};

QStringList ModifierListUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

ModifierListUI::ModifierListUI()
    : behaviorView(nullptr),
      bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      typeSelectorCB(new ComboBox)
{
    table->setAcceptDrops(true);
    table->viewport()->setAcceptDrops(true);
    table->setDragDropOverwriteMode(true);
    table->setDropIndicatorShown(true);
    table->setDragDropMode(QAbstractItemView::InternalMove);
    table->setRowSwapRange(BASE_NUMBER_OF_ROWS);
    setTitle("hkbModifierList");
    typeSelectorCB->insertItems(0, types);
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
    //table->setItem(ENABLE_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(ADD_MODIFIER_ROW, NAME_COLUMN, new TableWidgetItem("Add Modifier", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new modifier of the type specified in the adjacent combo box"));
    table->setCellWidget(ADD_MODIFIER_ROW, TYPE_COLUMN, typeSelectorCB);
    table->setItem(ADD_MODIFIER_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Modifier", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected modifier"));
    table->setItem(ADD_MODIFIER_ROW, VALUE_COLUMN, new TableWidgetItem("Change Selected Modifier", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to change the selected modifier"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void ModifierListUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(table, SIGNAL(itemDropped(int,int)), this, SLOT(swapGeneratorIndices(int,int)), Qt::UniqueConnection);
}

void ModifierListUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(table, SIGNAL(itemDropped(int,int)), this, SLOT(swapGeneratorIndices(int,int)));
}

void ModifierListUI::loadData(HkxObject *data){
    disconnectSignals();
    hkbVariableBindingSet *varBind = nullptr;
    if (data){
        if (data->getSignature() == HKB_MODIFIER_LIST){
            bsData = static_cast<hkbModifierList *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            varBind = bsData->getVariableBindingSetData();
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
            loadDynamicTableRows();
        }else{
            CRITICAL_ERROR_MESSAGE(QString("ModifierListUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::loadData(): The data passed to the UI is nullptr!!!");
    }
    connectSignals();
}

void ModifierListUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_MODIFIER_ROW + bsData->modifiers.size() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        hkbModifier *child = nullptr;
        for (auto i = ADD_MODIFIER_ROW + 1, j = 0; j < bsData->modifiers.size(); i++, j++){
            child = static_cast<hkbModifier *>(bsData->modifiers.at(j).data());
            if (child){
                setRowItems(i, child->getName(), child->getClassname(), "Remove", "Edit", "Double click to remove this modifier", VIEW_MODIFIERS_TABLE_TIP);
            }else{
                CRITICAL_ERROR_MESSAGE("ModifierListUI::loadData(): Null state found!!!");
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::loadDynamicTableRows(): The data is nullptr!!");
    }
    //table->setSortingEnabled(true);
}

void ModifierListUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
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

void ModifierListUI::connectToTables(GenericTableWidget *modifiers, GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties && modifiers){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(modifiers, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(modifiers, SIGNAL(elementSelected(int,QString)), this, SLOT(setModifier(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewModifiers(int,QString,QStringList)), modifiers, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

bool ModifierListUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE("ModifierListUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("ModifierListUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void ModifierListUI::setBindingVariable(int index, const QString & name){
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
        default:
            return;
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::setBindingVariable(): The data is nullptr!!");
    }
}

void ModifierListUI::loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE("ModifierListUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::loadBinding(): The data is nullptr!!");
    }
}

void ModifierListUI::variableRenamed(const QString &name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("ModifierListUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        index--;
        bind = bsData->getVariableBindingSetData();
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::variableRenamed(): The data is nullptr!!");
    }
}

void ModifierListUI::modifierRenamed(const QString &name, int index){
    int modifierIndex = table->currentRow() - BASE_NUMBER_OF_ROWS;
    if (bsData){
        if (modifierIndex >= 0 && modifierIndex < bsData->modifiers.size()){
            index--;
            if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData->modifiers.at(modifierIndex))){
                table->item(table->currentRow(), VALUE_COLUMN)->setText(name);
            }
        }else{
            WARNING_MESSAGE("ModifierListUI::generatorRenamed(): Invalid modifier index selected!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::generatorRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

void ModifierListUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}

void ModifierListUI::selectTableToView(bool viewproperties, const QString &path){
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
        CRITICAL_ERROR_MESSAGE("ModifierListUI::selectTableToView(): The data is nullptr!!");
    }
}

void ModifierListUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->setIsFileChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::setName(): The data is nullptr!!");
    }
}

void ModifierListUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::setEnable(): The data is nullptr!!");
    }
}

void ModifierListUI::viewSelectedChild(int row, int column){
    int result = -1;
    bool properties = false;
    if (bsData){
        if (row < ADD_MODIFIER_ROW && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case ENABLE_ROW:
                    if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "enable");
                    break;
                }
            }
        }else if (row == ADD_MODIFIER_ROW && column == NAME_COLUMN){
            addModifier();
        }else if (row > ADD_MODIFIER_ROW && row < ADD_MODIFIER_ROW + bsData->modifiers.size() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->modifiers.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    emit viewModifiers(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData->modifiers.at(result)) + 1, QString(), QStringList());
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the modifier \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeModifier(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE("ModifierListUI::viewSelectedChild(): Invalid index of child to view!!");
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void ModifierListUI::swapGeneratorIndices(int index1, int index2){
    HkxObject *gen1;
    HkxObject *gen2;
    if (bsData){
        index1 = index1 - BASE_NUMBER_OF_ROWS;
        index2 = index2 - BASE_NUMBER_OF_ROWS;
        if (bsData->modifiers.size() > index1 && bsData->modifiers.size() > index2 && index1 != index2 && index1 >= 0 && index2 >= 0){
            gen1 = bsData->modifiers.at(index1).data();
            gen2 = bsData->modifiers.at(index2).data();
            bsData->modifiers[index1] = HkxSharedPtr(gen2);
            bsData->modifiers[index2] = HkxSharedPtr(gen1);
            behaviorView->getSelectedItem()->reorderChildren();
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("ModifierListUI::swapGeneratorIndices(): Cannot swap these rows!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::swapGeneratorIndices(): The data is nullptr!!");
    }
}

void ModifierListUI::setModifier(int index, const QString &name){
    DataIconManager *ptr = nullptr;
    int modifierIndex = table->currentRow() - BASE_NUMBER_OF_ROWS;
    if (bsData){
        if (behaviorView){
            if (modifierIndex >= 0 && modifierIndex < bsData->modifiers.size()){
                ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getModifierDataAt(index - 1);
                if (ptr){
                    if (name != ptr->getName()){
                        CRITICAL_ERROR_MESSAGE("::setDefaultGenerator():The name of the selected object does not match it's name in the object selection table!!!");
                        return;
                    }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->modifiers.at(modifierIndex).data()), modifierIndex, ptr, false)){
                        WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!");
                        return;
                    }
                }else{
                    if (behaviorView->getSelectedItem()){
                        behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->modifiers.at(modifierIndex).data())), modifierIndex);
                    }else{
                        CRITICAL_ERROR_MESSAGE("ModifierListUI::setModifier(): The selected icon is nullptr!!");
                        return;
                    }
                }
                behaviorView->removeModifierData();
                table->item(table->currentRow(), NAME_COLUMN)->setText(name);
                bsData->setIsFileChanged(true);
                loadDynamicTableRows();
            }else{
                CRITICAL_ERROR_MESSAGE("ModifierListUI::setModifier(): Invalid modifier index selected!!");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("ModifierListUI::setModifier(): The 'behaviorView' pointer is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::setModifier(): The 'bsData' pointer is nullptr!!");
    }
}

void ModifierListUI::addModifier(){
    QString text;
    if (bsData && behaviorView){
        text = typeSelectorCB->currentText();
        if (text == "hkbModifierList"){
            behaviorView->appendModifierList();
        }else if (text == "hkbTwistModifier"){
            behaviorView->appendTwistModifier();
        }else if (text == "hkbEventDrivenModifier"){
            behaviorView->appendEventDrivenModifier();
        }else if (text == "BSIsActiveModifier"){
            behaviorView->appendIsActiveModifier();
        }else if (text == "BSLimbIKModifier"){
            behaviorView->appendLimbIKModifier();
        }else if (text == "BSInterpValueModifier"){
            behaviorView->appendInterpValueModifier();
        }else if (text == "BSGetTimeStepModifier"){
            behaviorView->appendGetTimeStepModifier();
        }else if (text == "hkbFootIkControlsModifier"){
            behaviorView->appendFootIkControlsModifier();
        }else if (text == "hkbGetHandleOnBoneModifier"){
            behaviorView->appendGetHandleOnBoneModifier();
        }else if (text == "hkbTransformVectorModifier"){
            behaviorView->appendTransformVectorModifier();
        }else if (text == "hkbProxyModifier"){
            behaviorView->appendProxyModifier();
        }else if (text == "hkbLookAtModifier"){
            behaviorView->appendLookAtModifier();
        }else if (text == "hkbMirrorModifier"){
            behaviorView->appendMirrorModifier();
        }else if (text == "hkbGetWorldFromModelModifier"){
            behaviorView->appendGetWorldFromModelModifier();
        }else if (text == "hkbSenseHandleModifier"){
            behaviorView->appendSenseHandleModifier();
        }else if (text == "hkbEvaluateExpressionModifier"){
            behaviorView->appendEvaluateExpressionModifier();
        }else if (text == "hkbEvaluateHandleModifier"){
            behaviorView->appendEvaluateHandleModifier();
        }else if (text == "hkbAttachmentModifier"){
            behaviorView->appendAttachmentModifier();
        }else if (text == "hkbAttributeModifier"){
            behaviorView->appendAttributeModifier();
        }else if (text == "hkbCombineTransformsModifier"){
            behaviorView->appendCombineTransformsModifier();
        }else if (text == "hkbComputeRotationFromAxisAngleModifier"){
            behaviorView->appendComputeRotationFromAxisAngleModifier();
        }else if (text == "hkbComputeRotationToTargetModifier"){
            behaviorView->appendComputeRotationToTargetModifier();
        }else if (text == "hkbEventsFromRangeModifier"){
            behaviorView->appendEventsFromRangeModifier();
        }else if (text == "hkbMoveCharacterModifier"){
            behaviorView->appendMoveCharacterModifier();
        }else if (text == "hkbExtractRagdollPoseModifier"){
            behaviorView->appendExtractRagdollPoseModifier();
        }else if (text == "BSModifyOnceModifier"){
            behaviorView->appendModifyOnceModifier();
        }else if (text == "BSEventOnDeactivateModifier"){
            behaviorView->appendEventOnDeactivateModifier();
        }else if (text == "BSEventEveryNEventsModifier"){
            behaviorView->appendEventEveryNEventsModifier();
        }else if (text == "BSRagdollContactListenerModifier"){
            behaviorView->appendRagdollContactListenerModifier();
        }else if (text == "hkbPoweredRagdollControlsModifier"){
            behaviorView->appendPoweredRagdollControlsModifier();
        }else if (text == "BSEventOnFalseToTrueModifier"){
            behaviorView->appendEventOnFalseToTrueModifier();
        }else if (text == "BSDirectAtModifier"){
            behaviorView->appendDirectAtModifier();
        }else if (text == "BSDistTriggerModifier"){
            behaviorView->appendDistTriggerModifier();
        }else if (text == "BSDecomposeVectorModifier"){
            behaviorView->appendDecomposeVectorModifier();
        }else if (text == "BSComputeAddBoneAnimModifier"){
            behaviorView->appendComputeAddBoneAnimModifier();
        }else if (text == "BSTweenerModifier"){
            behaviorView->appendTweenerModifier();
        }else if (text == "BSIStateManagerModifier"){
            behaviorView->appendBSIStateManagerModifier();
        }else if (text == "hkbTimerModifier"){
            behaviorView->appendTimerModifier();
        }else if (text == "hkbRotateCharacterModifier"){
            behaviorView->appendRotateCharacterModifier();
        }else if (text == "hkbDampingModifier"){
            behaviorView->appendDampingModifier();
        }else if (text == "hkbDelayedModifier"){
            behaviorView->appendDelayedModifier();
        }else if (text == "hkbGetUpModifier"){
            behaviorView->appendGetUpModifier();
        }else if (text == "hkbKeyframeBonesModifier"){
            behaviorView->appendKeyframeBonesModifier();
        }else if (text == "hkbComputeDirectionModifier"){
            behaviorView->appendComputeDirectionModifier();
        }else if (text == "hkbRigidBodyRagdollControlsModifier"){
            behaviorView->appendRigidBodyRagdollControlsModifier();
        }else if (text == "BSSpeedSamplerModifier"){
            behaviorView->appendBSSpeedSamplerModifier();
        }else if (text == "hkbDetectCloseToGroundModifier"){
            behaviorView->appendDetectCloseToGroundModifier();
        }else if (text == "BSLookAtModifier"){
            behaviorView->appendBSLookAtModifier();
        }else if (text == "BSTimerModifier"){
            behaviorView->appendBSTimerModifier();
        }else if (text == "BSPassByTargetTriggerModifier"){
            behaviorView->appendBSPassByTargetTriggerModifier();
        }else if (text == "hkbHandIKControlsModifier"){
            behaviorView->appendHandIKControlsModifier();
        }else{
            CRITICAL_ERROR_MESSAGE("ModifierListUI::addModifier(): Invalid type!!");
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::addModifier(): The data or behavior graph pointer is nullptr!!");
    }
}

void ModifierListUI::removeModifier(int index){
    if (bsData && behaviorView){
        if (index < bsData->modifiers.size() && index >= 0){
            behaviorView->removeItemFromGraph(behaviorView->getSelectedIconsChildIcon(bsData->modifiers.at(index).data()), index);//Reorderchildren?
            behaviorView->removeObjects();
        }else{
            WARNING_MESSAGE("ModifierListUI::removeModifier(): Invalid index of generator to remove!!");
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("ModifierListUI::removeModifier(): The data or behavior graph pointer is nullptr!!");
    }
}
