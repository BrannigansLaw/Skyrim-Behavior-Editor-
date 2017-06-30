#include "behaviorreferencegeneratorui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbbehaviorreferencegenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/genericdatawidgets.h"

#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 2

#define NAME_ROW 0
#define BEHAVIOR_NAME_ROW 1

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define VALUE_COLUMN 2

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BehaviorReferenceGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

BehaviorReferenceGeneratorUI::BehaviorReferenceGeneratorUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      behaviorName(new ComboBox)
{
    setTitle("hkbBehaviorReferenceGenerator");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(BEHAVIOR_NAME_ROW, NAME_COLUMN, new TableWidgetItem("behaviorName"));
    table->setItem(BEHAVIOR_NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setCellWidget(BEHAVIOR_NAME_ROW, VALUE_COLUMN, behaviorName);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void BehaviorReferenceGeneratorUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(behaviorName, SIGNAL(currentTextChanged(QString)), this, SLOT(setBehaviorName(QString)), Qt::UniqueConnection);
}

void BehaviorReferenceGeneratorUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(behaviorName, SIGNAL(currentTextChanged(QString)), this, SLOT(setBehaviorName(QString)));
}

void BehaviorReferenceGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_BEHAVIOR_REFERENCE_GENERATOR){
            bsData = static_cast<hkbBehaviorReferenceGenerator *>(data);
            name->setText(bsData->name);
            QStringList behaviors = bsData->getParentFile()->getAllBehaviorFileNames();
            if (behaviorName->count() != behaviors.size()){
                behaviorName->clear();
                behaviorName->insertItems(0, behaviors);
            }
            bsData->behaviorName.replace("\\", "/");    //Do this once on object read?
            int index = behaviorName->findText(bsData->behaviorName, Qt::MatchFixedString);
            if (index < 0){
                WARNING_MESSAGE(QString("BehaviorReferenceGeneratorUI::loadData(): The data has an invalid behavior name!!!"));
            }else{
                behaviorName->setCurrentIndex(index);
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BehaviorReferenceGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BehaviorReferenceGeneratorUI::loadData(): The data passed to the UI is NULL!!!"));
    }
    connectSignals();
}

void BehaviorReferenceGeneratorUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BehaviorReferenceGeneratorUI::setName(): The data is NULL!!"));
    }
}

void BehaviorReferenceGeneratorUI::setBehaviorName(const QString & text){
    if (bsData){
        if (bsData->getParentFile()->fileName().contains(text)){
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch!!!"));
            disconnectSignals();
            behaviorName->setCurrentIndex(behaviorName->findText(bsData->behaviorName, Qt::MatchFixedString));
            connectSignals();
        }else{
            bsData->behaviorName = text;
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BehaviorReferenceGeneratorUI::setBehaviorName(): The data is NULL!!"));
    }
}

void BehaviorReferenceGeneratorUI::behaviorRenamed(const QString &name, int index){
    if (name == ""){
        WARNING_MESSAGE(QString("BehaviorReferenceGeneratorUI::behaviorRenamed(): The new variable name is the empty string!!"))
    }
    //index--;
    if (index == behaviorName->currentIndex()){
        bsData->behaviorName = name;
    }
    disconnectSignals();
    behaviorName->removeItem(index);
    index--;
    behaviorName->insertItem(index, name);
    connectSignals();
}
