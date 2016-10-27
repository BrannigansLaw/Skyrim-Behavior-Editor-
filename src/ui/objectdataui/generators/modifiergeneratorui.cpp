#include "modifiergeneratorui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/generators/hkbmodifiergenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/ui/behaviorgraphui/behaviorgraphicons.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QHeaderView>

/*
 * ModifierGeneratorUI
 */

QStringList ModifierGeneratorUI::headerLabels1 = {
    "Name",
    "Type",
    "Value"
};

ModifierGeneratorUI::ModifierGeneratorUI()
    : behaviorView(NULL),
      bsData(NULL),
      lyt(new QVBoxLayout),
      table(new TableWidget),
      name(new QLineEdit),
      modifier(new ComboBox),
      generator(new ComboBox)
{
    setTitle("hkbModifierGenerator");
    table->setRowCount(3);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels1);
    table->setCellWidget(0, 0, name);
    table->setItem(0, 1, new QTableWidgetItem("String"));
    table->setItem(0, 2, new QTableWidgetItem("N/A"));
    table->setItem(0, 3, new QTableWidgetItem("N/A"));
    table->setItem(1, 0, new QTableWidgetItem("modifier"));
    table->setItem(1, 1, new QTableWidgetItem("hkbModifier"));
    table->setCellWidget(1, 2, modifier);
    table->setItem(2, 0, new QTableWidgetItem("generator"));
    table->setItem(2, 1, new QTableWidgetItem("hkbGenerator"));
    table->setCellWidget(2, 2, generator);
    lyt->addWidget(table);
    setLayout(lyt);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    connect(modifier, SIGNAL(activated(int)), this, SLOT(setModifier(int)));
    connect(generator, SIGNAL(activated(int)), this, SLOT(setGenerator(int)));
}

void ModifierGeneratorUI::addModifierToLists(const QString & name){
    modifier->insertItem(modifier->count(), name);
}

void ModifierGeneratorUI::removeModifierFromLists(int index){
    modifier->removeItem(index);
}

void ModifierGeneratorUI::renameModifierInLists(const QString & name, int index){
    index++;
    modifier->setItemText(index, name);
}

void ModifierGeneratorUI::addGeneratorToLists(const QString & name){
    generator->insertItem(generator->count(), name);
}

void ModifierGeneratorUI::removeGeneratorFromLists(int index){
    generator->removeItem(index);
}

void ModifierGeneratorUI::renameGeneratorInLists(const QString & name, int index){
    index++;
    generator->setItemText(index, name);
}

void ModifierGeneratorUI::setName(){
    if (bsData){
        bsData->name = name->text();
        bsData->updateIconNames();
        emit generatorNameChanged(name->text(), bsData->getParentFile()->getIndexOfGenerator(bsData) + 1);
    }
}

void ModifierGeneratorUI::setModifier(int index){
    if (behaviorView && index > -1 && index < modifier->count()){
        HkxObject *ptr = bsData->getParentFile()->getModifierDataAt(index - 1);
        if (!ptr || ptr == bsData || !behaviorView->reconnectBranch(bsData->modifier.data(), ptr, behaviorView->getSelectedItem())){
            QMessageBox msg;
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
            msg.exec();
            int i = bsData->getParentFile()->getIndexOfModifier(bsData->modifier);
            i++;
            modifier->setCurrentIndex(i);
            return;
        }
        if (index > 0){
            bsData->modifier = HkxObjectExpSharedPtr(ptr);
            behaviorView->removeModifierData();
        }
    }
}

void ModifierGeneratorUI::setGenerator(int index){
    if (behaviorView && index > -1 && index < generator->count()){
        HkxObject *ptr = bsData->getParentFile()->getGeneratorDataAt(index - 1);
        if (!ptr || ptr == bsData || !behaviorView->reconnectBranch(bsData->generator.data(), ptr, behaviorView->getSelectedItem())){
            QMessageBox msg;
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
            msg.exec();
            int i = bsData->getParentFile()->getIndexOfGenerator(bsData->generator);
            i++;
            generator->setCurrentIndex(i);
            return;
        }
        if (index > 0){
            bsData->generator = HkxObjectExpSharedPtr(ptr);
            behaviorView->removeGeneratorData();
        }
    }
}

void ModifierGeneratorUI::loadComboBoxes(){
    QStringList modList = behaviorView->behavior->getModifierNames();
    modList.prepend("None");
    modifier->insertItems(0, modList);
    QStringList genList = behaviorView->behavior->getGeneratorNames();
    genList.prepend("None");
    generator->insertItems(0, genList);
}

void ModifierGeneratorUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_MODIFIER_GENERATOR){
        bsData = static_cast<hkbModifierGenerator *>(data);
        name->setText(bsData->name);
        int index = bsData->getParentFile()->getIndexOfModifier(bsData->modifier) + 1;
        modifier->setCurrentIndex(index);
        index = bsData->getParentFile()->getIndexOfGenerator(bsData->generator) + 1;
        generator->setCurrentIndex(index);
    }
}
