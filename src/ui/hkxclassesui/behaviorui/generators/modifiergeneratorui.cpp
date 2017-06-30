#include "modifiergeneratorui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbmodifiergenerator.h"
#include "src/hkxclasses/behavior/modifiers/hkbmodifier.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/genericdatawidgets.h"

#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 3

#define NAME_ROW 0
#define MODIFIER_ROW 1
#define GENERATOR_ROW 2

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

//#define INDEX_OF_MODIFIER 0
//#define INDEX_OF_GENERATOR 1 //See hkbModifierGenerator getIndexOfObj()

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList ModifierGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

ModifierGeneratorUI::ModifierGeneratorUI()
    : behaviorView(NULL),
      bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit)
{
    setTitle("hkbModifierGenerator");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(MODIFIER_ROW, NAME_COLUMN, new TableWidgetItem("modifier"));
    table->setItem(MODIFIER_ROW, TYPE_COLUMN, new TableWidgetItem("hkbModifier", Qt::AlignCenter));
    table->setItem(MODIFIER_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(MODIFIER_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_MODIFIERS_TABLE_TIP));
    table->setItem(GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("generator"));
    table->setItem(GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbGenerator", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_GENERATORS_TABLE_TIP));
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void ModifierGeneratorUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void ModifierGeneratorUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void ModifierGeneratorUI::connectToTables(GenericTableWidget *modifiers, GenericTableWidget *generators){
    if (modifiers && generators){
        disconnect(modifiers, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(modifiers, SIGNAL(elementSelected(int,QString)), this, SLOT(setModifier(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(setGenerator(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int)), generators, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewModifiers(int)), modifiers, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void ModifierGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_MODIFIER_GENERATOR){
            bsData = static_cast<hkbModifierGenerator *>(data);
            name->setText(bsData->name);
            if (bsData->modifier.data()){
                table->item(MODIFIER_ROW, VALUE_COLUMN)->setText(static_cast<hkbModifier *>(bsData->modifier.data())->getName());
            }else{
                table->item(MODIFIER_ROW, VALUE_COLUMN)->setText("NONE");
            }
            if (bsData->generator.data()){
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->generator.data())->getName());
            }else{
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::loadData(): The data passed to the UI is NULL!!!"));
    }
    connectSignals();
}

void ModifierGeneratorUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::setName(): The data is NULL!!"));
    }
}

void ModifierGeneratorUI::setModifier(int index, const QString &name){
    DataIconManager *ptr = NULL;
    int indexOfModifier = -1;
    if (bsData){
        if (behaviorView){
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getModifierDataAt(index - 1);
            indexOfModifier = bsData->getIndexOfObj(static_cast<DataIconManager*>(bsData->modifier.data()));
            if (ptr){
                if (name != ptr->getName()){
                    CRITICAL_ERROR_MESSAGE(QString("The name of the selected object does not match it's name in the object selection table!!!"));
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->modifier.data()), ptr, false)){
                    WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"));
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->modifier.data())), indexOfModifier);
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::setModifier(): The selected icon is NULL!!"));
                    return;
                }
            }
            behaviorView->removeModifierData();
            table->item(MODIFIER_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::setModifier(): The 'behaviorView' pointer is NULL!!"))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::setModifier(): The 'bsData' pointer is NULL!!"))
    }
}

void ModifierGeneratorUI::setGenerator(int index, const QString & name){
    DataIconManager *ptr = NULL;
    int indexOfGenerator = -1;
    if (bsData){
        if (behaviorView){
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            indexOfGenerator = bsData->getIndexOfObj(static_cast<DataIconManager*>(bsData->generator.data()));
            if (ptr){
                if (name != ptr->getName()){
                    CRITICAL_ERROR_MESSAGE(QString("The name of the selected object does not match it's name in the object selection table!!!"));
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->generator.data()), ptr, false)){
                    WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"));
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->generator.data())), indexOfGenerator);
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::setGenerator(): The selected icon is NULL!!"));
                    return;
                }
            }
            behaviorView->removeGeneratorData();
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::setGenerator(): The 'behaviorView' pointer is NULL!!"))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::setGenerator(): The 'bsData' pointer is NULL!!"))
    }
}

void ModifierGeneratorUI::viewSelected(int row, int column){
    if (bsData){
        if (column == VALUE_COLUMN){
            if (row == MODIFIER_ROW){
                emit viewModifiers(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator) + 1);
            }else if (row == GENERATOR_ROW){
                emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator) + 1);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void ModifierGeneratorUI::modifierRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData->modifier)){
            table->item(MODIFIER_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::modifierRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void ModifierGeneratorUI::generatorRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator)){
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ModifierGeneratorUI::generatorRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void ModifierGeneratorUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}
