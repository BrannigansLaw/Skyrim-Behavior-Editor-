#include "genericdatawidgets.h"

#include <QApplication>

GenericTableWidget::GenericTableWidget(const QString & title)
    : lyt(new QGridLayout),
      table(new QTableWidget),
      selectPB(new QPushButton("Select")),
      cancelPB(new QPushButton("Cancel")),
      //newPB(new QPushButton("New")),
      //typeSelector(new ComboBox),
      lastSelectedRow(-1)
{
    setWindowTitle(title);
    resize(500, 800);
    setWindowModality(Qt::ApplicationModal);
    QStringList list = {"Name", "Type"};
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(list);
    table->setShowGrid(false);
    //table->setMouseTracking(true);
    //table->setStyleSheet("QTableWidget { background:cyan }");
    table->setStyleSheet("QHeaderView::section { background-color:grey }");
    table->verticalHeader()->setVisible(true);
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    //table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionsClickable(false);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    lyt->addWidget(table, 0, 0, 8, 10);
    lyt->addWidget(selectPB, 8, 2, 1, 2);
    lyt->addWidget(cancelPB, 8, 7, 1, 2);
    //lyt->addWidget(newPB, 9, 2, 1, 2);
    //lyt->addWidget(typeSelector, 9, 5, 1, 4);
    setLayout(lyt);
    connect(selectPB, SIGNAL(released()), this, SLOT(itemSelected()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellChanged(int,int)), this, SLOT(itemSelected()), Qt::UniqueConnection);
    //connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(itemSelected()), Qt::UniqueConnection);
    connect(cancelPB, SIGNAL(released()), this, SLOT(hide()), Qt::UniqueConnection);
    //connect(newPB, SIGNAL(released()), this, SLOT(itemAdded()), Qt::UniqueConnection);
}

void GenericTableWidget::loadTable(const QStringList & names, const QStringList & types, const QString & firstElement){
    int start = 0;
    table->setRowCount(0);
    if (names.size() == types.size()){
        if (firstElement != ""){
            start = 1;
            if (table->rowCount() > 0){
                if (table->item(0, 0)){
                    table->item(0, 0)->setText(firstElement);
                }else{
                    table->setItem(0, 0, new QTableWidgetItem(firstElement));
                }
                if (table->item(0, 1)){
                    table->item(0, 1)->setText(firstElement);
                }else{
                    table->setItem(0, 1, new QTableWidgetItem(firstElement));
                }
            }else{
                table->setRowCount(table->rowCount() + 1);
                table->setItem(0, 0, new QTableWidgetItem(firstElement));
                table->setItem(0, 1, new QTableWidgetItem(firstElement));
            }
        }
        for (int i = start, j = 0; j < names.size(); i++, j++){
            if (i < table->rowCount()){
                if (table->item(i, 0)){
                    table->item(i, 0)->setText(names.at(j));
                }else{
                    table->setItem(i, 0, new QTableWidgetItem(names.at(j)));
                }
                if (table->item(i, 1)){
                    table->item(i, 1)->setText(types.at(j));
                }else{
                    table->setItem(i, 1, new QTableWidgetItem(types.at(j)));
                }
            }else{
                table->setRowCount(table->rowCount() + 1);
                table->setItem(i, 0, new QTableWidgetItem(names.at(j)));
                table->setItem(i, 1, new QTableWidgetItem(types.at(j)));
            }
        }
    }else{
        WARNING_MESSAGE(QString("GenericTableWidget: loadTable()\nThe stringlist arguments have different sizes!!!"))
    }
}

void GenericTableWidget::loadTable(const QStringList & names, const QString & type, const QString & firstElement){
    int start = 0;
    table->setRowCount(0);
    if (firstElement != ""){
        start = 1;
        if (table->rowCount() > 0){
            if (table->item(0, 0)){
                table->item(0, 0)->setText(firstElement);
            }else{
                table->setItem(0, 0, new QTableWidgetItem(firstElement));
            }
            if (table->item(0, 1)){
                table->item(0, 1)->setText(firstElement);
            }else{
                table->setItem(0, 1, new QTableWidgetItem(firstElement));
            }
        }else{
            table->setRowCount(table->rowCount() + 1);
            table->setItem(0, 0, new QTableWidgetItem(firstElement));
            table->setItem(0, 1, new QTableWidgetItem(firstElement));
        }
    }
    for (int i = start, j = 0; j < names.size(); i++, j++){
        if (i < table->rowCount()){
            if (table->item(i, 0)){
                table->item(i, 0)->setText(names.at(j));
            }else{
                table->setItem(i, 0, new QTableWidgetItem(names.at(j)));
            }
            if (table->item(i, 1)){
                table->item(i, 1)->setText(type);
            }else{
                table->setItem(i, 1, new QTableWidgetItem(type));
            }
        }else{
            table->setRowCount(table->rowCount() + 1);
            table->setItem(i, 0, new QTableWidgetItem(names.at(j)));
            table->setItem(i, 1, new QTableWidgetItem(type));
        }
    }
}

int GenericTableWidget::getNumRows() const{
    return table->rowCount();
}

void GenericTableWidget::addItem(const QString & name, const QString & type){
    table->blockSignals(true);
    int i = table->rowCount();
    table->setRowCount(table->rowCount() + 1);
    table->setItem(i, 0, new QTableWidgetItem(name));
    table->setItem(i, 1, new QTableWidgetItem(type));
    table->setCurrentCell(table->rowCount() - 1, 0);
    table->blockSignals(false);
}

void GenericTableWidget::renameItem(int index, const QString & newname){
    table->blockSignals(true);
    if (index < table->rowCount() && index >= 0){
        table->item(index, 0)->setText(newname);
    }
    table->blockSignals(false);
}

void GenericTableWidget::removeItem(int index){
    if (index < table->rowCount() && index >= 0){
        if (table->currentRow() == index){
            lastSelectedRow = -1;
        }
        table->removeRow(index);
    }
}

/*void GenericTableWidget::itemAdded(){
    addItem("New"+typeSelector->currentText(), typeSelector->currentText());
    emit elementAdded(table->rowCount() - 1, typeSelector->currentText());
}*/

void GenericTableWidget::showTable(int index){
    lastSelectedRow = index;
    if (index < table->rowCount() && index >= 0){
        table->setCurrentCell(index, 0);
    }
    show();
}

void GenericTableWidget::showTable(const QString & name){
    int index = -1;
    for (int i = 0; i < table->rowCount(); i++){
        if (table->item(i, 0) && table->item(i, 0)->text() == name){
            lastSelectedRow = i;
            break;
        }
    }
    index = lastSelectedRow;
    if (index < table->rowCount() && index >= 0){
        table->setCurrentCell(index, 0);
    }
    show();
}

void GenericTableWidget::itemSelected(){
    QString name;
    int row = table->currentRow();
    if (row != lastSelectedRow){
        if (table->item(row, 0)){
            name = table->item(row, 0)->text();
        }
        lastSelectedRow = row;
        emit elementSelected(row, name);
        hide();
    }
}

/*void GenericTableWidget::setTypes(const QStringList & typeNames){
    typeSelector->clear();
    typeSelector->addItems(typeNames);
}*/

