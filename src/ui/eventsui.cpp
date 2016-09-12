#include "eventsui.h"

#include "src/ui/behaviorgraphui/behaviorgraphicons.h"
#include "src/ui/genericdatawidgets.h"
#include "src/hkxclasses/generators/hkbgenerator.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/hkxclasses/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/generators/hkbmodifiergenerator.h"
#include "src/ui/objectdataui/generators/bsistatetagginggeneratorui.h"
#include "src/ui/objectdataui/generators/modifiergeneratorui.h"
#include "src/ui/objectdataui/generators/manualselectorgeneratorui.h"

#include "src/hkxclasses/hkbbehaviorgraphdata.h"
#include "src/hkxclasses/hkbbehaviorgraphstringdata.h"
#include "src/hkxclasses/hkbvariablevalueset.h"

#include <QPushButton>
#include <QMessageBox>
#include <QStackedLayout>
#include <QStringList>
#include <QSignalMapper>
#include <QTableWidget>
#include <QHeaderView>

QStringList EventsUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

EventsUI::EventsUI(const QString &title)
    : dataUI(NULL),
      verLyt(new QVBoxLayout),
      loadedData(NULL),
      table(new QTableWidget),
      addObjectPB(new QPushButton("Add Event")),
      removeObjectPB(new QPushButton("Remove Selected Event")),
      buttonLyt(new QHBoxLayout),
      flagsMapper(new QSignalMapper),
      nameMapper(new QSignalMapper)
{
    setTitle(title);
    buttonLyt->addWidget(addObjectPB, 1);
    buttonLyt->addSpacing(2);
    buttonLyt->addWidget(removeObjectPB, 1);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels);
    verLyt->addLayout(buttonLyt, 1);
    verLyt->addWidget(table, 10);
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    setLayout(verLyt);
    connect(flagsMapper, SIGNAL(mapped(int)), this, SLOT(setBoolVariableValue(int)));
    connect(removeObjectPB, SIGNAL(pressed()), this, SLOT(removeEvent()));
    connect(addObjectPB, SIGNAL(pressed()), this, SLOT(addEvent()));
    connect(nameMapper, SIGNAL(mapped(int)), this, SLOT(renameSelectedEvent(int)));
}

void EventsUI::setBoolVariableValue(int index){
    loadedData->setEventFlagAt(index, qobject_cast<QCheckBox *>(table->cellWidget(index, 2))->isChecked());
}

void EventsUI::renameSelectedEvent(int index){
    loadedData->setEventNameAt(index, qobject_cast<QLineEdit *>(table->cellWidget(index, 0))->text());
}

void EventsUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_BEHAVIOR_GRAPH_DATA){
        loadedData = static_cast<hkbBehaviorGraphData *>(data);
        hkbBehaviorGraphStringData *events = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
        table->setRowCount(0);
        for (int i = 0; i < events->eventNames.size(); i++){
            addEventToTable(events->eventNames.at(i), loadedData->eventInfos.at(i));
        }
    }
}

void EventsUI::addEventToTable(const QString & name, const QString & flags){
    int row = table->rowCount();
    QLineEdit *lineEdit = new QLineEdit(name);
    table->setRowCount(row + 1);
    table->setCellWidget(row, 0, lineEdit);
    table->setItem(row, 1, new QTableWidgetItem("hkEvent"));
    QCheckBox *checkBox = new QCheckBox("Flag As Sync Point");
    table->setCellWidget(row, 2, checkBox);
    if (flags == "FLAG_SYNC_POINT"){
        checkBox->setChecked(true);
    }
    connect(checkBox, SIGNAL(clicked(bool)), flagsMapper, SLOT(map()));
    connect(lineEdit, SIGNAL(editingFinished()), nameMapper, SLOT(map()));
    flagsMapper->setMapping(checkBox, row);
    nameMapper->setMapping(lineEdit, row);
}

void EventsUI::removeEventFromTable(int row){
    if (row < table->rowCount()){
        table->removeRow(row);
        for (int i = row; i < table->rowCount(); i++){
            flagsMapper->setMapping(table->cellWidget(i, 2), i);
            nameMapper->setMapping(table->cellWidget(i, 0), i);
        }
    }
}

void EventsUI::addEvent(){
    loadedData->addEvent();
    hkbBehaviorGraphStringData *events = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
    addEventToTable(events->eventNames.last(), loadedData->eventInfos.last());
}

void EventsUI::removeEvent(){
    loadedData->removeEvent(table->currentRow());
    removeEventFromTable(table->currentRow());
}

void EventsUI::setHkDataUI(HkDataUI *ui){
    dataUI = ui;
}
