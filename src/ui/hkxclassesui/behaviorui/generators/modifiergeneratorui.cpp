#include "modifiergeneratorui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbmodifiergenerator.h"
#include "src/hkxclasses/behavior/modifiers/hkbmodifier.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"

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

ModifierGeneratorUI::ModifierGeneratorUI(GenericTableWidget *genTable, GenericTableWidget *modTable)
    : generatorTable(genTable),
      modifiersTable(modTable),
      behaviorView(NULL),
      bsData(NULL),
      lyt(new QVBoxLayout),
      table(new TableWidget(QColor(Qt::cyan))),
      name(new LineEdit),
      modifier(new QPushButton("NULL")),
      generator(new QPushButton("NULL"))
{
    setTitle("hkbModifierGenerator");
    table->setRowCount(3);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels1);
    table->setCellWidget(0, 0, name);
    table->setItem(0, 1, new TableWidgetItem("hkStringPtr"));
    table->setItem(0, 2, new TableWidgetItem("N/A"));
    table->setItem(0, 3, new TableWidgetItem("N/A"));
    table->setItem(1, 0, new TableWidgetItem("modifier"));
    table->setItem(1, 1, new TableWidgetItem("hkbModifier"));
    table->setCellWidget(1, 2, modifier);
    table->setItem(2, 0, new TableWidgetItem("generator"));
    table->setItem(2, 1, new TableWidgetItem("hkbGenerator"));
    table->setCellWidget(2, 2, generator);
    lyt->addWidget(table);
    setLayout(lyt);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    connect(modifier, SIGNAL(activated(int)), this, SLOT(setModifier(int)));
    connect(generator, SIGNAL(activated(int)), this, SLOT(setGenerator(int)));
}

void ModifierGeneratorUI::setGeneratorTable(GenericTableWidget *genTable){
    if (genTable){
        generatorTable = genTable;
        connect(generatorTable, SIGNAL(elementSelected(int)), this, SLOT(setGenerator(int)));
        connect(generatorTable, SIGNAL(hideWindow()), this, SLOT(viewGenerators()));
    }
}

void ModifierGeneratorUI::setModifierTable(GenericTableWidget *modTable){
    if (modTable){
        modifiersTable = modTable;
        connect(modifiersTable, SIGNAL(elementSelected(int)), this, SLOT(setModifier(int)));
        connect(modifiersTable, SIGNAL(hideWindow()), this, SLOT(viewModifiers()));
    }
}

void ModifierGeneratorUI::viewGenerators(){
    if (!generatorTable->isVisible()){
        generatorTable->show();
    }else{
        generatorTable->hide();
    }
}

void ModifierGeneratorUI::viewModifiers(){
    if (!modifiersTable->isVisible()){
        modifiersTable->show();
    }else{
        modifiersTable->hide();
    }
}

void ModifierGeneratorUI::setName(){
    if (bsData){
        bsData->name = name->text();
        ((DataIconManager *)(bsData))->updateIconNames();
        bsData->getParentFile()->toggleChanged(true);
        emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData) + 1);
    }
}

void ModifierGeneratorUI::setModifier(int index){
    /*if (behaviorView && index > -1){
        DataIconManager *ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getModifierDataAt(index);
        if (!behaviorView->selectedIcon->getChildWithData(ptr)){
            if (!ptr || ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), ptr,  reinterpret_cast<DataIconManager *>(bsData->modifier.data()))){
                QMessageBox msg;
                msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
                msg.exec();
                return;
            }
        }
        modifier->setText(ptr->getName());
        if (index > 0){
            bsData->modifier = HkxObjectExpSharedPtr(ptr);
            behaviorView->removeModifierData();
            bsData->getParentFile()->toggleChanged(true);
        }
    }
    modifiersTable->hide();*/
}

void ModifierGeneratorUI::setGenerator(int index){
    /*if (behaviorView && index > -1){
        DataIconManager *ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index);
        if (!behaviorView->selectedIcon->getChildWithData(ptr)){
            if (!ptr || ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), ptr,  reinterpret_cast<DataIconManager *>(bsData->generator.data()))){
                QMessageBox msg;
                msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
                msg.exec();
                return;
            }
        }
        generator->setText(ptr->getName());
        if (index > 0){
            bsData->generator = HkxObjectExpSharedPtr(ptr);
            behaviorView->removeGeneratorData();
            bsData->getParentFile()->toggleChanged(true);
        }
    }
    generatorTable->hide();*/
}

void ModifierGeneratorUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_MODIFIER_GENERATOR){
        bsData = static_cast<hkbModifierGenerator *>(data);
        name->setText(bsData->name);
        if (bsData->modifier.data()){
            modifier->setText(static_cast<hkbModifier *>(bsData->modifier.data())->getName());
        }
        if (bsData->generator.data()){
            generator->setText(static_cast<hkbGenerator *>(bsData->generator.data())->getName());
        }
    }
}
