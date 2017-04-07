#include "ManualSelectorGeneratorUI.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbmanualselectorgenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QHeaderView>
#include <QSignalMapper>

/*
 * ManualSelectorGeneratorUI
 */

QStringList ManualSelectorGeneratorUI::types = {
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

QStringList ManualSelectorGeneratorUI::headerLabels1 = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

QStringList ManualSelectorGeneratorUI::headerLabels2 = {
    "Name",
    "Type",
    "Value"
};

ManualSelectorGeneratorUI::ManualSelectorGeneratorUI()
    : generatorTable(new GenericTableWidget("Select a hkbGenerator!")),
      behaviorView(NULL),
      bsData(NULL),
      lyt(new QVBoxLayout),
      table(new TableWidget),
      name(new QLineEdit),
      generators(new TableWidget),
      addObjectPB(new QPushButton("Add Generator")),
      removeObjectPB(new QPushButton("Remove Selected Generator")),
      typeSelectorCB(new ComboBox),
      buttonLyt(new QHBoxLayout),
      selectedGeneratorIndex(new SpinBox),
      selectedGeneratorIndexBind(new ComboBox),
      currentGeneratorIndex(new SpinBox),
      currentGeneratorIndexBind(new ComboBox)
{
    setTitle("hkbManualSelectorGenerator");
    generatorTable->hide();
    buttonLyt->addWidget(addObjectPB, 1);
    buttonLyt->addWidget(typeSelectorCB, 2);
    buttonLyt->addWidget(removeObjectPB, 1);
    table->setRowCount(3);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(headerLabels1);
    table->setCellWidget(0, 0, name);
    table->setItem(0, 1, new QTableWidgetItem("hkStringPtr"));
    table->setItem(0, 2, new QTableWidgetItem("N/A"));
    table->setItem(0, 3, new QTableWidgetItem("N/A"));
    table->setItem(1, 0, new QTableWidgetItem("selectedGeneratorIndex"));
    table->setItem(1, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(1, 2, selectedGeneratorIndexBind);
    table->setCellWidget(1, 3, selectedGeneratorIndex);
    table->setItem(2, 0, new QTableWidgetItem("currentGeneratorIndex"));
    table->setItem(2, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(2, 2, currentGeneratorIndexBind);
    table->setCellWidget(2, 3, currentGeneratorIndex);
    generators->setColumnCount(3);
    generators->setHorizontalHeaderLabels(headerLabels2);
    typeSelectorCB->insertItems(0, types);
    lyt->addWidget(table, 3);
    lyt->addLayout(buttonLyt, 1);
    lyt->addWidget(generators, 5);
    setLayout(lyt);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    connect(addObjectPB, SIGNAL(released()), this, SLOT(addNewGenerator()));
    connect(removeObjectPB, SIGNAL(released()), this, SLOT(removeGenerator()));
    connect(generators, SIGNAL(cellClicked(int,int)), this, SLOT(viewGenerators()));
    connect(generatorTable, SIGNAL(elementSelected(int,QString)), this, SLOT(setGenerator(int)));
    connect(generatorTable, SIGNAL(hideWindow()), this, SLOT(viewGenerators()));
    connect(selectedGeneratorIndex, SIGNAL(editingFinished()), this, SLOT(setSelectedGeneratorIndex(int)));
    connect(selectedGeneratorIndexBind, SIGNAL(activated(int)), this, SLOT(setSelectedGeneratorIndexBind(int)));
    connect(currentGeneratorIndex, SIGNAL(editingFinished()), this, SLOT(setCurrentGeneratorIndex(int)));
    connect(currentGeneratorIndexBind, SIGNAL(activated(int)), this, SLOT(setCurrentGeneratorIndexBind(int)));
}

/*void ManualSelectorGeneratorUI::setGeneratorTable(GenericTableWidget *genTable){
    if (genTable){
        generatorTable = genTable;
        connect(generatorTable, SIGNAL(elementSelected(int)), this, SLOT(setGenerator(int)));
        connect(generatorTable, SIGNAL(hideWindow()), this, SLOT(viewGenerators()));
    }
}*/

void ManualSelectorGeneratorUI::viewGenerators(){
    if (!generatorTable->isVisible()){
        //connect(generatorTable, SIGNAL(elementSelected(int)), this, SLOT(setGenerator(int)));
        generatorTable->show();
    }else{
        //disconnect(generatorTable, 0, this, 0);
        generatorTable->hide();
    }
}

void ManualSelectorGeneratorUI::addVariableToLists(const QString & name){
    selectedGeneratorIndexBind->insertItem(selectedGeneratorIndexBind->count(), name);
    currentGeneratorIndexBind->insertItem(currentGeneratorIndexBind->count(), name);
}

void ManualSelectorGeneratorUI::removeVariableFromLists(int index){
    index++;
    if (selectedGeneratorIndexBind->currentIndex() == index){
        selectedGeneratorIndexBind->setCurrentIndex(0);
    }
    selectedGeneratorIndexBind->removeItem(index);
    if (currentGeneratorIndexBind->currentIndex() == index){
        currentGeneratorIndexBind->setCurrentIndex(0);
    }
    currentGeneratorIndexBind->removeItem(index);
}

void ManualSelectorGeneratorUI::renameVariableInLists(const QString & newName, int index){
    index++;
    selectedGeneratorIndexBind->setItemText(index, newName);
    currentGeneratorIndexBind->setItemText(index, newName);
}

void ManualSelectorGeneratorUI::setName(){
    if (bsData){
        bsData->name = name->text();
        bsData->updateIconNames();
        emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData) + 1);
    }
}

void ManualSelectorGeneratorUI::loadComboBoxes(){
    QStringList varList = behaviorView->behavior->getVariableNames();
    varList.prepend("None");
    selectedGeneratorIndexBind->insertItems(0, varList);
    currentGeneratorIndexBind->insertItems(0, varList);
}

void ManualSelectorGeneratorUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_MANUAL_SELECTOR_GENERATOR){
        bsData = static_cast<hkbManualSelectorGenerator *>(data);
        hkbGenerator *gen;
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (generatorTable->getNumRows() < 1){
            generatorTable->loadTable(static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorNamesAndTypeNames(), "NULL");
        }
        if (varBind){
            int ind = varBind->getVariableIndexOfBinding("selectedGeneratorIndex");
            if (ind > -1 && ind < selectedGeneratorIndexBind->count()){
                selectedGeneratorIndexBind->setCurrentIndex(ind + 1);
            }else{
                selectedGeneratorIndexBind->setCurrentIndex(0);
            }
            ind = varBind->getVariableIndexOfBinding("currentGeneratorIndex");
            if (ind > -1 && ind < currentGeneratorIndexBind->count()){
                currentGeneratorIndexBind->setCurrentIndex(ind + 1);
            }else{
                currentGeneratorIndexBind->setCurrentIndex(0);
            }
        }else{
            selectedGeneratorIndexBind->setCurrentIndex(0);
            currentGeneratorIndexBind->setCurrentIndex(0);
        }
        name->setText(bsData->name);
        for (int i = 0; i < bsData->generators.size(); i++){
            gen = static_cast<hkbGenerator *>(bsData->generators.at(i).data());
            if (bsData->generators.at(i).data()){
                if (i >= generators->rowCount()){
                    generators->setRowCount(generators->rowCount() + 1);
                    generators->setItem(i, 0, new QTableWidgetItem(gen->getName()));
                    generators->setItem(i, 1, new QTableWidgetItem(gen->getClassname()));
                    generators->setItem(i, 2, new QTableWidgetItem("Click to Edit"));
                }else{
                    generators->setRowHidden(i, false);
                    generators->item(i, 0)->setText(gen->getName());
                    generators->item(i, 1)->setText(gen->getClassname());
                }
            }
        }
        for (int j = bsData->generators.size(); j < generators->rowCount(); j++){
            generators->setRowHidden(j, true);
        }
        selectedGeneratorIndex->setValue(bsData->selectedGeneratorIndex);
        currentGeneratorIndex->setValue(bsData->currentGeneratorIndex);
    }
}

void ManualSelectorGeneratorUI::setSelectedGeneratorIndex(){
    if (bsData){
        bsData->selectedGeneratorIndex = selectedGeneratorIndex->value();
        bsData->getParentFile()->toggleChanged(true);
    }
}

void ManualSelectorGeneratorUI::setCurrentGeneratorIndex(){
    if (bsData){
        bsData->currentGeneratorIndex = currentGeneratorIndex->value();
        bsData->getParentFile()->toggleChanged(true);
    }
}

void ManualSelectorGeneratorUI::setSelectedGeneratorIndexBind(int index){
    if (bsData){
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (behaviorView->behavior->getVariableTypeAt(index - 1) == VARIABLE_TYPE_INT32){
            if (!varBind){
                varBind = new hkbVariableBindingSet(behaviorView->behavior);
                bsData->variableBindingSet = HkxObjectExpSharedPtr(varBind);
                behaviorView->behavior->addObjectToFile(varBind);
            }
            varBind->addBinding("selectedGeneratorIndex", index - 1);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            QMessageBox msg;
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
            msg.exec();
            if (varBind){
                int ind = varBind->getVariableIndexOfBinding("selectedGeneratorIndex");
                if (ind > -1 && ind < selectedGeneratorIndexBind->count()){
                    selectedGeneratorIndexBind->setCurrentIndex(ind + 1);
                }
            }
        }
    }
}

void ManualSelectorGeneratorUI::setCurrentGeneratorIndexBind(int index){
    if (bsData){
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (behaviorView->behavior->getVariableTypeAt(index - 1) == VARIABLE_TYPE_INT32){
            if (!varBind){
                varBind = new hkbVariableBindingSet(behaviorView->behavior);
                bsData->variableBindingSet = HkxObjectExpSharedPtr(varBind);
                behaviorView->behavior->addObjectToFile(varBind);
            }
            varBind->addBinding("currentGeneratorIndex", index - 1);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            QMessageBox msg;
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
            msg.exec();
            if (varBind){
                int ind = varBind->getVariableIndexOfBinding("currentGeneratorIndex");
                if (ind > -1 && ind < currentGeneratorIndexBind->count()){
                    currentGeneratorIndexBind->setCurrentIndex(ind + 1);
                }
            }
        }
    }
}

void ManualSelectorGeneratorUI::setGenerator(int index){
    int row = generators->currentRow();
    if (behaviorView && index > -1 && row > -1 && row < generators->rowCount() && row < bsData->generators.size()){
        DataIconManager *ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index);
        if (!behaviorView->getSelectedItem()->getChildIcon(ptr)){
            if (!ptr || ptr == bsData || !behaviorView->reconnectBranch(static_cast<DataIconManager *>(bsData->generators.at(row).data()), ptr, behaviorView->getSelectedItem())){
                QMessageBox msg;
                msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
                msg.exec();
                return;
            }
        }else{
            behaviorView->removeSelectedObjectBranch(behaviorView->getSelectedItem()->getChildIcon(bsData->generators.at(row).data()), NULL, false);
        }
        generators->item(row, 0)->setText(static_cast<hkbGenerator *>(ptr)->getName());
        generators->item(row, 1)->setText(static_cast<hkbGenerator *>(ptr)->getClassname());
        if (row > 0/* && row < bsData->generators.size()*/){
            bsData->generators[row] = HkxObjectExpSharedPtr(ptr);
            behaviorView->removeGeneratorData();
            bsData->getParentFile()->toggleChanged(true);
        }
    }
    generatorTable->hide();
}

void ManualSelectorGeneratorUI::addNewGenerator(){
    if (!behaviorView){
        return;
    }
    Type typeEnum = static_cast<Type>(typeSelectorCB->currentIndex());
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
        return;
    }
    if (!bsData->generators.isEmpty() && bsData->generators.last().data()){
        generators->setRowCount(generators->rowCount() + 1);
        generators->setItem(generators->rowCount(), 0, new QTableWidgetItem(static_cast<hkbGenerator *>(bsData->generators.last().data())->getName()));
        generators->setItem(generators->rowCount(), 1, new QTableWidgetItem(static_cast<hkbGenerator *>(bsData->generators.last().data())->getClassname()));
        generators->setItem(generators->rowCount(), 2, new QTableWidgetItem("Click to Edit"));
        bsData->getParentFile()->toggleChanged(true);
    }
}

void ManualSelectorGeneratorUI::removeGenerator(){
    int index = generators->currentRow();
    if (!behaviorView || bsData->generators.size() <= index || index < 0){
        return;
    }
    TreeGraphicsItem *tempIcon = behaviorView->selectedIcon->getChildIcon(bsData->generators.at(index).data());
    if (bsData->generators.count(bsData->generators.at(index)) == 1){
        behaviorView->removeSelectedObjectBranch(tempIcon, NULL, false);
    }else{
        bsData->generators.removeAt(index);
    }
    delete generators->cellWidget(index, 2);
    generators->removeCellWidget(index, 2);
    generators->removeRow(index);
    behaviorView->removeGeneratorData();
    bsData->getParentFile()->toggleChanged(true);
}
