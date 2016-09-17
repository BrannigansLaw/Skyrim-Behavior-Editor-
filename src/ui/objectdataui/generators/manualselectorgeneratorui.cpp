#include "ManualSelectorGeneratorUI.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/generators/hkbmanualselectorgenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/ui/behaviorgraphui/behaviorgraphicons.h"
#include "src/hkxclasses/hkbvariablebindingset.h"

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
    : behaviorView(NULL),
      bsData(NULL),
      lyt(new QVBoxLayout),
      table(new QTableWidget),
      name(new QLineEdit),
      generators(new QTableWidget),
      addObjectPB(new QPushButton("Add Generator")),
      removeObjectPB(new QPushButton("Remove Selected Generator")),
      typeSelectorCB(new QComboBox),
      signalMapper(new QSignalMapper),
      buttonLyt(new QHBoxLayout),
      selectedGeneratorIndex(new QSpinBox),
      selectedGeneratorIndexBind(new QComboBox),
      currentGeneratorIndex(new QSpinBox),
      currentGeneratorIndexBind(new QComboBox)
{
    setTitle("hkbManualSelectorGenerator");
    buttonLyt->addWidget(addObjectPB, 1);
    buttonLyt->addWidget(typeSelectorCB, 2);
    buttonLyt->addWidget(removeObjectPB, 1);
    table->setRowCount(3);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(headerLabels1);
    table->setCellWidget(0, 0, name);
    table->setItem(0, 1, new QTableWidgetItem("String"));
    table->setItem(0, 2, new QTableWidgetItem("N/A"));
    table->setItem(0, 3, new QTableWidgetItem("N/A"));
    table->setItem(1, 0, new QTableWidgetItem("selectedGeneratorIndex"));
    table->setItem(1, 1, new QTableWidgetItem("Int"));
    table->setCellWidget(1, 2, selectedGeneratorIndexBind);
    table->setCellWidget(1, 3, selectedGeneratorIndex);
    table->setItem(2, 0, new QTableWidgetItem("currentGeneratorIndex"));
    table->setItem(2, 1, new QTableWidgetItem("Int"));
    table->setCellWidget(2, 2, currentGeneratorIndexBind);
    table->setCellWidget(2, 3, currentGeneratorIndex);
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    generators->setColumnCount(3);
    generators->setHorizontalHeaderLabels(headerLabels2);
    generators->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    generators->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    generators->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    generators->setSelectionBehavior(QAbstractItemView::SelectRows);
    typeSelectorCB->insertItems(0, types);
    lyt->addWidget(table, 3);
    lyt->addLayout(buttonLyt, 1);
    lyt->addWidget(generators, 5);
    setLayout(lyt);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    connect(addObjectPB, SIGNAL(released()), this, SLOT(addNewGenerator()));
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setGeneratorAt(int)));
    connect(removeObjectPB, SIGNAL(released()), this, SLOT(removeGenerator()));
    connect(selectedGeneratorIndex, SIGNAL(editingFinished()), this, SLOT(setSelectedGeneratorIndex(int)));
    connect(selectedGeneratorIndexBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelectedGeneratorIndexBind(int)));
    connect(currentGeneratorIndex, SIGNAL(editingFinished()), this, SLOT(setCurrentGeneratorIndex(int)));
    connect(currentGeneratorIndexBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentGeneratorIndexBind(int)));
}

void ManualSelectorGeneratorUI::addVariableToLists(const QString & name){
    disconnect(selectedGeneratorIndexBind, 0, this, 0);
    disconnect(currentGeneratorIndexBind, 0, this, 0);
    selectedGeneratorIndexBind->insertItem(selectedGeneratorIndexBind->count(), name);
    currentGeneratorIndexBind->insertItem(currentGeneratorIndexBind->count(), name);
    connect(selectedGeneratorIndexBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelectedGeneratorIndexBind(int)));
    connect(currentGeneratorIndexBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentGeneratorIndexBind(int)));
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

void ManualSelectorGeneratorUI::addGeneratorToLists(const QString & name){
    QComboBox *temp;
    for (int i = 0; i < generators->rowCount(); i++){
        temp = qobject_cast<QComboBox *>(generators->cellWidget(i, 2));
        disconnect(temp, 0, this, 0);
        temp->insertItem(temp->count(), name);
        connect(temp, SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));
    }
}

void ManualSelectorGeneratorUI::removeGeneratorFromLists(int index){
    QComboBox *temp;
    for (int i = 0; i < generators->rowCount(); i++){
        temp = qobject_cast<QComboBox *>(generators->cellWidget(i, 2));
        disconnect(temp, 0, this, 0);
        temp->removeItem(index);
        connect(temp, SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));
    }
}

void ManualSelectorGeneratorUI::renameGeneratorInLists(const QString & name, int index){
    index++;
    QComboBox *temp;
    for (int i = 0; i < generators->rowCount(); i++){
        temp = qobject_cast<QComboBox *>(generators->cellWidget(i, 2));
        temp->setItemText(index, name);
        if (temp->currentIndex() == index){
            generators->item(i, 0)->setText(name);
        }
    }
}

void ManualSelectorGeneratorUI::setName(){
    if (bsData){
        bsData->name = name->text();
        bsData->updateIconNames();
        emit generatorNameChanged(name->text(), bsData->getParentFile()->getIndexOfGenerator(bsData) + 1);
    }
}

void ManualSelectorGeneratorUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_MANUAL_SELECTOR_GENERATOR){
        bsData = static_cast<hkbManualSelectorGenerator *>(data);
        hkbGenerator *gen;
        QComboBox *comboBox;
        QStringList genList = bsData->getParentFile()->getGeneratorNames();
        genList.prepend("None");
        int index;
        disconnect(selectedGeneratorIndexBind, 0, this, 0);
        disconnect(currentGeneratorIndexBind, 0, this, 0);
        if (selectedGeneratorIndexBind->count() == 0){
            QStringList varList = bsData->getParentFile()->getVariableNames();
            varList.prepend("None");
            selectedGeneratorIndexBind->insertItems(0, varList);
            currentGeneratorIndexBind->insertItems(0, varList);
        }
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
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
        connect(selectedGeneratorIndexBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelectedGeneratorIndexBind(int)));
        connect(currentGeneratorIndexBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentGeneratorIndexBind(int)));
        name->setText(bsData->name);
        for (int i = 0; i < bsData->generators.size(); i++){
            gen = static_cast<hkbGenerator *>(bsData->generators.at(i).data());
            if (bsData->generators.at(i).data()){
                if (i >= generators->rowCount()){
                    generators->setRowCount(generators->rowCount() + 1);
                    generators->setItem(i, 0, new QTableWidgetItem(gen->getName()));
                    generators->setItem(i, 1, new QTableWidgetItem(gen->getClassname()));
                    comboBox = new QComboBox;
                    comboBox->addItems(genList);
                    generators->setCellWidget(i, 2, comboBox);
                    index = bsData->getParentFile()->getIndexOfGenerator(bsData->generators.at(i)) + 1;
                    if (index > -1 && index < comboBox->count()){
                        comboBox->setCurrentIndex(index);
                    }
                    connect(comboBox, SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));
                    signalMapper->setMapping(comboBox, i);
                }else{
                    generators->setRowHidden(i, false);
                    generators->item(i, 0)->setText(gen->getName());
                    generators->item(i, 1)->setText(gen->getClassname());
                    comboBox = qobject_cast<QComboBox *>(generators->cellWidget(i, 2));
                    index = bsData->getParentFile()->getIndexOfGenerator(bsData->generators.at(i)) + 1;
                    if (index > -1 && index < comboBox->count()){
                        disconnect(signalMapper, 0, this, 0);
                        comboBox->setCurrentIndex(index);
                        connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setGeneratorAt(int)));
                    }
                    signalMapper->setMapping(comboBox, i);
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
    }
}

void ManualSelectorGeneratorUI::setCurrentGeneratorIndex(){
    if (bsData){
        bsData->currentGeneratorIndex = currentGeneratorIndex->value();
    }
}

void ManualSelectorGeneratorUI::setSelectedGeneratorIndexBind(int index){
    if (bsData){
        disconnect(selectedGeneratorIndexBind, 0, this, 0);
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (behaviorView->behavior->getVariableTypeAt(index - 1) == VARIABLE_TYPE_INT32){
            if (!varBind){
                varBind = new hkbVariableBindingSet(behaviorView->behavior);
                bsData->variableBindingSet = HkxObjectExpSharedPtr(varBind);
                behaviorView->behavior->addObjectToFile(varBind);
            }
            varBind->addBinding("selectedGeneratorIndex", index - 1);
        }else{
            QMessageBox msg;
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
            msg.exec();
            if (varBind){
                disconnect(selectedGeneratorIndexBind, 0, this, 0);
                int ind = varBind->getVariableIndexOfBinding("selectedGeneratorIndex");
                if (ind > -1 && ind < selectedGeneratorIndexBind->count()){
                    selectedGeneratorIndexBind->setCurrentIndex(ind + 1);
                }
            }
        }
        connect(selectedGeneratorIndexBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelectedGeneratorIndexBind(int)));
    }
}

void ManualSelectorGeneratorUI::setCurrentGeneratorIndexBind(int index){
    if (bsData){
        disconnect(currentGeneratorIndexBind, 0, this, 0);
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (behaviorView->behavior->getVariableTypeAt(index - 1) == VARIABLE_TYPE_INT32){
            if (!varBind){
                varBind = new hkbVariableBindingSet(behaviorView->behavior);
                bsData->variableBindingSet = HkxObjectExpSharedPtr(varBind);
                behaviorView->behavior->addObjectToFile(varBind);
            }
            varBind->addBinding("currentGeneratorIndex", index - 1);
        }else{
            QMessageBox msg;
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
            msg.exec();
            if (varBind){
                disconnect(currentGeneratorIndexBind, 0, this, 0);
                int ind = varBind->getVariableIndexOfBinding("currentGeneratorIndex");
                if (ind > -1 && ind < currentGeneratorIndexBind->count()){
                    currentGeneratorIndexBind->setCurrentIndex(ind + 1);
                }
            }
        }
        connect(currentGeneratorIndexBind, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentGeneratorIndexBind(int)));
    }
}

void ManualSelectorGeneratorUI::setGeneratorAt(int index){
    if (behaviorView && index > -1 && index < bsData->generators.size() && index < generators->rowCount()){
        behaviorView->removeSelectedObjectBranch(behaviorView->selectedIcon->getChildIcon(bsData->generators.at(index).data()));
        QComboBox *comboBox = qobject_cast<QComboBox *>(generators->cellWidget(index, 2));
        HkxObject *ptr = bsData->getParentFile()->getGeneratorDataAt(comboBox->currentIndex() - 1);
        if (!behaviorView->selectedIcon->getChildIcon(ptr)){
            if (/*index < 1 || */!ptr || ptr == bsData || !behaviorView->reconnectBranch(NULL, ptr, behaviorView->getSelectedItem())){
                QMessageBox msg;
                msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
                msg.exec();
                int i = bsData->getParentFile()->getIndexOfGenerator(bsData->generators.at(index));
                i++;
                disconnect(signalMapper, 0, this, 0);
                comboBox->setCurrentIndex(i);
                connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setGeneratorAt(int)));
                return;
            }
        }
        if (index > 0){
            bsData->generators.insert(index, HkxObjectExpSharedPtr(ptr));
            behaviorView->removeGeneratorData();
            generators->item(index, 0)->setText(static_cast<hkbGenerator *>(ptr)->getName());
            generators->item(index, 1)->setText(static_cast<hkbGenerator *>(ptr)->getClassname());
        }
    }
}

void ManualSelectorGeneratorUI::addNewGenerator(){
    if (!behaviorView){
        return;
    }
    QComboBox *genSelector = new QComboBox;
    QStringList genList = bsData->getParentFile()->getGeneratorNames();
    genList.prepend("None");
    genSelector->insertItems(0, genList);
    //connect(genSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(setGeneratorAt(int)));
    connect(genSelector, SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));
    signalMapper->setMapping(genSelector, bsData->generators.size());
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
        delete genSelector;
        return;
    }
    if (!bsData->generators.isEmpty() && bsData->generators.last().data()){
        generators->setRowCount(generators->rowCount() + 1);
        generators->setItem(generators->rowCount(), 0, new QTableWidgetItem(static_cast<hkbGenerator *>(bsData->generators.last().data())->getName()));
        generators->setItem(generators->rowCount(), 1, new QTableWidgetItem(static_cast<hkbGenerator *>(bsData->generators.last().data())->getClassname()));
        generators->setCellWidget(generators->rowCount(), 2, genSelector);
    }
}

void ManualSelectorGeneratorUI::removeGenerator(){
    int index = generators->currentRow();
    if (!behaviorView || bsData->generators.size() <= index || index < 0){
        return;
    }
    GeneratorIcon *tempIcon = behaviorView->selectedIcon->getChildIcon(bsData->generators.at(index).data());
    if (bsData->generators.count(bsData->generators.at(index)) == 1){
        behaviorView->removeSelectedObjectBranch(tempIcon);
    }else{
        bsData->generators.removeAt(index);
    }
    delete generators->cellWidget(index, 2);
    generators->removeCellWidget(index, 2);
    generators->removeRow(index);
    behaviorView->removeGeneratorData();
    //loadData(bsData);
}
