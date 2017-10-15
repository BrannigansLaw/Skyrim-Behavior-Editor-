#include "bgsgamebryosequencegeneratorui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/BGSGamebryoSequenceGenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/genericdatawidgets.h"

#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 2

#define NAME_ROW 0
#define SEQUENCE_ROW 1
#define BLEND_MODE_FUNCTION_ROW 1
#define PERCENT_ROW 1

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define VALUE_COLUMN 2

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BGSGamebryoSequenceGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

BGSGamebryoSequenceGeneratorUI::BGSGamebryoSequenceGeneratorUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      pSequence(new LineEdit),
      eBlendModeFunction(new ComboBox),
      fPercent(new DoubleSpinBox)
{
    setTitle("BGSGamebryoSequenceGenerator");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(SEQUENCE_ROW, NAME_COLUMN, new TableWidgetItem("pSequence"));
    table->setItem(SEQUENCE_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setCellWidget(SEQUENCE_ROW, VALUE_COLUMN, pSequence);
    table->setItem(BLEND_MODE_FUNCTION_ROW, NAME_COLUMN, new TableWidgetItem("eBlendModeFunction"));
    table->setItem(BLEND_MODE_FUNCTION_ROW, TYPE_COLUMN, new TableWidgetItem("BlendModeFunction", Qt::AlignCenter));
    table->setCellWidget(BLEND_MODE_FUNCTION_ROW, VALUE_COLUMN, eBlendModeFunction);
    table->setItem(PERCENT_ROW, NAME_COLUMN, new TableWidgetItem("fPercent"));
    table->setItem(PERCENT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(PERCENT_ROW, VALUE_COLUMN, fPercent);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void BGSGamebryoSequenceGeneratorUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(pSequence, SIGNAL(editingFinished()), this, SLOT(setSequence()), Qt::UniqueConnection);
    connect(eBlendModeFunction, SIGNAL(currentIndexChanged(int)), this, SLOT(setBlendModeFunction(int)), Qt::UniqueConnection);
    connect(fPercent, SIGNAL(editingFinished()), this, SLOT(setPercent()), Qt::UniqueConnection);
}

void BGSGamebryoSequenceGeneratorUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(pSequence, SIGNAL(editingFinished()), this, SLOT(setSequence()));
    disconnect(eBlendModeFunction, SIGNAL(currentIndexChanged(int)), this, SLOT(setBlendModeFunction(int)));
    disconnect(fPercent, SIGNAL(editingFinished()), this, SLOT(setPercent()));
}

void BGSGamebryoSequenceGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BGS_GAMEBYRO_SEQUENCE_GENERATOR){
            bsData = static_cast<BGSGamebryoSequenceGenerator *>(data);
            name->setText(bsData->name);
            pSequence->setText(bsData->pSequence);
            if (eBlendModeFunction->count() == 0){
                eBlendModeFunction->insertItems(0, bsData->BlendModeFunction);
            }
            eBlendModeFunction->setCurrentIndex(bsData->BlendModeFunction.indexOf(bsData->eBlendModeFunction));
            fPercent->setValue(bsData->fPercent);
        }else{
            (qFatal(QString("BGSGamebryoSequenceGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data()));
        }
    }else{
        (qFatal("BGSGamebryoSequenceGeneratorUI::loadData(): The data passed to the UI is NULL!!!"));
    }
    connectSignals();
}

void BGSGamebryoSequenceGeneratorUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
        }
    }else{
        (qFatal("BGSGamebryoSequenceGeneratorUI::setName(): The data is NULL!!"));
    }
}

void BGSGamebryoSequenceGeneratorUI::setSequence(){
    if (bsData){
        if (bsData->pSequence != pSequence->text()){
            bsData->pSequence = pSequence->text();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("BGSGamebryoSequenceGeneratorUI::setSequence(): The data is NULL!!"));
    }
}

void BGSGamebryoSequenceGeneratorUI::setBlendModeFunction(int index){
    if (bsData){
        bsData->eBlendModeFunction = bsData->BlendModeFunction.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        (qFatal("BGSGamebryoSequenceGeneratorUI::setBlendModeFunction(): The data is NULL!!"));
    }
}

void BGSGamebryoSequenceGeneratorUI::setPercent(){
    if (bsData){
        if (bsData->fPercent != fPercent->value()){
            bsData->fPercent = fPercent->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("BGSGamebryoSequenceGeneratorUI::setPercent(): The data is NULL!!"));
    }
}

