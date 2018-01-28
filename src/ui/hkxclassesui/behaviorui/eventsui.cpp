#include "eventsui.h"

#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphstringdata.h"
#include "src/ui/genericdatawidgets.h"

#include <QPushButton>
#include <QMessageBox>
#include <QStackedLayout>
#include <QStringList>
#include <QSignalMapper>
#include <QTableWidget>
#include <QHeaderView>
#include <QCoreApplication>

QStringList EventsUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

EventsUI::EventsUI(const QString &title)
    : dataUI(nullptr),
      verLyt(new QVBoxLayout),
      loadedData(nullptr),
      table(new TableWidget),
      addObjectPB(new QPushButton("Add Event")),
      removeObjectPB(new QPushButton("Remove Selected Event")),
      buttonLyt(new QHBoxLayout),
      eventName(new LineEdit),
      flag(new CheckBox("Flag As Sync Point")),
      eventWidget(new TableWidget),
      stackLyt(new QStackedLayout),
      returnPB(new QPushButton("Return To Parent"))
{
    setTitle(title);
    stackLyt->addWidget(table);
    stackLyt->addWidget(eventWidget);
    stackLyt->setCurrentIndex(TABLE_WIDGET);
    eventWidget->setRowCount(1);
    eventWidget->setColumnCount(3);
    eventWidget->setCellWidget(0, 0, eventName);
    eventWidget->setCellWidget(0, 1, flag);
    eventWidget->setCellWidget(0, 2, returnPB);
    buttonLyt->addWidget(addObjectPB, 1);
    buttonLyt->addSpacing(2);
    buttonLyt->addWidget(removeObjectPB, 1);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels);
    verLyt->addLayout(buttonLyt, 1);
    verLyt->addLayout(stackLyt, 10);
    setLayout(verLyt);
    connect(flag, SIGNAL(released()), this, SLOT(setBoolVariableValue()));
    connect(removeObjectPB, SIGNAL(pressed()), this, SLOT(removeEvent()));
    connect(addObjectPB, SIGNAL(pressed()), this, SLOT(addEvent()));
    connect(eventName, SIGNAL(editingFinished()), this, SLOT(renameSelectedEvent()));
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewEvent(int,int)));
    connect(returnPB, SIGNAL(released()), this, SLOT(returnToTable()));
}

void EventsUI::viewEvent(int row, int column){
    hkbBehaviorGraphStringData *events = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
    if (column == 2 && loadedData && loadedData->eventInfos.size() > row && events->eventNames.size() > row){
        disconnect(eventName, 0, this, 0);
        disconnect(flag, 0, this, 0);
        eventName->setText(events->eventNames.at(row));
        if (loadedData->eventInfos.at(row) == "FLAG_SYNC_POINT"){
            flag->setChecked(true);
        }else{
            flag->setChecked(false);
        }
        connect(flag, SIGNAL(released()), this, SLOT(setBoolVariableValue()));
        connect(eventName, SIGNAL(editingFinished()), this, SLOT(renameSelectedEvent()));
        stackLyt->setCurrentIndex(EVENT_WIDGET);
    }
}

void EventsUI::returnToTable(){
    stackLyt->setCurrentIndex(TABLE_WIDGET);
}

void EventsUI::setBoolVariableValue(){
    loadedData->setEventFlagAt(table->currentRow(), flag->isChecked());
    loadedData->getParentFile()->setIsChanged(true);
}

void EventsUI::renameSelectedEvent(){
    QString newName = eventName->text();
    table->item(table->currentRow(), 0)->setText(newName);
    loadedData->setEventNameAt(table->currentRow(), newName);
    loadedData->getParentFile()->setIsChanged(true);
    emit eventNameChanged(newName, table->currentRow());
}

void EventsUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_BEHAVIOR_GRAPH_DATA){
        loadedData = static_cast<hkbBehaviorGraphData *>(data);
        int row;
        hkbBehaviorGraphStringData *events = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
        for (int i = 0; i < events->eventNames.size(); i++){
            row = table->rowCount();
            if (table->rowCount() > i){
                table->setRowHidden(i, false);
                if (table->item(row, 0)){
                    table->item(row, 0)->setText(events->eventNames.at(i));
                }else{
                    table->setItem(row, 0, new QTableWidgetItem(events->eventNames.at(i)));
                }
            }else{
                table->setRowCount(row + 1);
                table->setItem(row, 0, new QTableWidgetItem(events->eventNames.at(i)));
                table->setItem(row, 1, new QTableWidgetItem("hkEvent"));
                table->setItem(row, 2, new QTableWidgetItem("Edit"));
            }
        }
        for (int j = events->eventNames.size(); j < table->rowCount(); j++){
            table->setRowHidden(j, true);
        }
    }
}

void EventsUI::clear(){
    for (int i = table->rowCount() - 1; i >= 0; i--){
        table->removeRow(i);
    }
}

void EventsUI::addEvent(){
    if (loadedData){
        loadedData->addEvent();
        hkbBehaviorGraphStringData *events = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
        int row = table->rowCount();
        table->setRowCount(row + 1);
        table->setItem(row, 0, new QTableWidgetItem(events->eventNames.last()));
        table->setItem(row, 1, new QTableWidgetItem("hkEvent"));
        table->setItem(row, 2, new QTableWidgetItem("Edit"));
        if (stackLyt->currentIndex() == EVENT_WIDGET){
            stackLyt->setCurrentIndex(TABLE_WIDGET);
        }
        table->setCurrentCell(row, 0);
        emit eventAdded(events->eventNames.last());
    }
}

void EventsUI::removeEvent(){
    if (loadedData){
        int index = table->currentRow();
        loadedData->removeEvent(index);
        if (index < table->rowCount()){
            table->removeRow(index);
        }
        if (stackLyt->currentIndex() == EVENT_WIDGET){
            stackLyt->setCurrentIndex(TABLE_WIDGET);
        }
        loadedData->getParentFile()->setIsChanged(true);
        emit eventRemoved(index);
    }
}

void EventsUI::setHkDataUI(HkDataUI *ui){
    dataUI = ui;
}
