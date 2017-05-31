#include "behaviorgraphui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbbehaviorgraph.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 3

#define NAME_ROW 0
#define VARIABLE_MODE_ROW 1
#define ROOT_GENERATOR_ROW 2

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define VALUE_COLUMN 2

QStringList BehaviorGraphUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

BehaviorGraphUI::BehaviorGraphUI()
    : behaviorView(NULL),
      bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::cyan))),
      name(new LineEdit),
      variableMode(new ComboBox)
{
    setTitle("hkbBehaviorGraph");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(VARIABLE_MODE_ROW, NAME_COLUMN, new TableWidgetItem("variableMode"));
    table->setItem(VARIABLE_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("VariableMode", Qt::AlignCenter));
    table->setCellWidget(VARIABLE_MODE_ROW, VALUE_COLUMN, variableMode);
    table->setItem(ROOT_GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("rootGenerator"));
    table->setItem(ROOT_GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ROOT_GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(219, 219, 219), QBrush(Qt::black), "Click to view the list of generators"));
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(variableMode, SIGNAL(activated(int)), this, SLOT(setVariableMode(int)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void BehaviorGraphUI::loadData(HkxObject *data){
    blockSignals(true);
    if (data){
        if (data->getSignature() == HKB_BEHAVIOR_GRAPH){
            bsData = static_cast<hkbBehaviorGraph *>(data);
            name->setText(bsData->name);
            if (variableMode->count() == 0){
                variableMode->insertItems(0, bsData->VariableMode);
            }
            variableMode->setCurrentIndex(bsData->VariableMode.indexOf(bsData->variableMode));
            if (bsData->rootGenerator.data()){
                table->item(ROOT_GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->rootGenerator.data())->getName());
            }else{
                table->item(ROOT_GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphUI::loadData(): The data is NULL!!"));
    }
    blockSignals(false);
}

void BehaviorGraphUI::setName(){
    if (bsData){
        bsData->name = name->text();//Make sure name is valid...
        ((DataIconManager *)(bsData))->updateIconNames();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphUI::setName(): The data is NULL!!"));
    }
}

void BehaviorGraphUI::setVariableMode(int index){
    if (bsData){
        bsData->variableMode = bsData->VariableMode.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphUI::setVariableMode(): The data is NULL!!"));
    }
}

void BehaviorGraphUI::viewSelectedChild(int row, int column){
    if (bsData){
        if (row == ROOT_GENERATOR_ROW && column == VALUE_COLUMN){
            if (bsData->getParentFile()){
                emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->rootGenerator) + 1);
            }else{
                CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphUI::viewSelectedChild(): The parent file is NULL!!"));
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void BehaviorGraphUI::setRootGenerator(int index, const QString &name){
    DataIconManager *ptr = NULL;
    if (bsData){
        if (behaviorView){
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            if (ptr){
                if (name != ptr->getName()){
                    CRITICAL_ERROR_MESSAGE(QString("The name of the selected object does not match it's name in the object selection table!!!"));
                }else if (ptr->getSignature() != HKB_STATE_MACHINE){
                    WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nThe selected object is not a hkbStateMachine!!!"));
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), (DataIconManager *)bsData->rootGenerator.data(), ptr, false)){
                    WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"));
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData((DataIconManager *)bsData->rootGenerator.data()), 0);
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphUI::setGenerator(): The selected icon is NULL!!"));
                }
                behaviorView->removeGeneratorData();
                table->item(ROOT_GENERATOR_ROW, VALUE_COLUMN)->setText(name);
                bsData->getParentFile()->toggleChanged(true);
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphUI::setGenerator(): The 'behaviorView' pointer is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphUI::setGenerator(): The 'bsData' pointer is NULL!!"));
    }
}

void BehaviorGraphUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}

void BehaviorGraphUI::connectToTables(GenericTableWidget *generators){
    if (generators){
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(setRootGenerator(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int)), generators, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphUI::connectToTables(): The argument is NULL!!"));
    }
}
