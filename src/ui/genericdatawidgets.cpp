#include "genericdatawidgets.h"

#include <QApplication>

GenericTableWidget::GenericTableWidget(const QString & title)
    : lyt(new QGridLayout),
      table(new TableWidget),
      selectPB(new QPushButton("Select")),
      cancelPB(new QPushButton("Cancel")),
      newPB(new QPushButton("New")),
      typeSelector(new ComboBox),
      lastSelectedRow(-1)
{
    setWindowTitle(title);
    resize(500, 800);
    setWindowModality(Qt::WindowModal);
    QStringList list = {"Name", "Type"};
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(list);
    lyt->addWidget(table, 0, 0, 8, 10);
    lyt->addWidget(selectPB, 8, 2, 1, 2);
    lyt->addWidget(cancelPB, 8, 7, 1, 2);
    lyt->addWidget(newPB, 9, 2, 1, 2);
    lyt->addWidget(typeSelector, 9, 5, 1, 4);
    setLayout(lyt);
    connect(selectPB, SIGNAL(released()), this, SLOT(itemSelected()));
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(itemSelected()));
    connect(cancelPB, SIGNAL(released()), this, SLOT(hide()));
    connect(newPB, SIGNAL(released()), this, SLOT(itemAdded()));
}

void GenericTableWidget::loadTable(const QStringList & list, const QString & empty){
    if (table->rowCount() > 0){
        if (table->itemAt(0, 0)){
            table->itemAt(0, 0)->setText(empty);
        }else{
            table->setItem(0, 0, new QTableWidgetItem(empty));
        }
        if (table->itemAt(0, 1)){
            table->itemAt(0, 1)->setText(empty);
        }else{
            table->setItem(0, 1, new QTableWidgetItem(empty));
        }
    }else{
        table->setRowCount(table->rowCount() + 1);
        table->setItem(0, 0, new QTableWidgetItem(empty));
        table->setItem(0, 1, new QTableWidgetItem(empty));
    }
    for (int i = 1, j = 0, k = 1; i < list.size(), j < list.size(), k < list.size(); i++, j+=2, k+=2){
        if (i < table->rowCount()){
            if (table->itemAt(i, 0)){
                table->itemAt(i, 0)->setText(list.at(j));
            }else{
                table->setItem(i, 0, new QTableWidgetItem(list.at(j)));
            }
            if (table->itemAt(i, 1)){
                table->itemAt(i, 1)->setText(list.at(k));
            }else{
                table->setItem(i, 1, new QTableWidgetItem(list.at(k)));
            }
        }else{
            table->setRowCount(table->rowCount() + 1);
            table->setItem(i, 0, new QTableWidgetItem(list.at(j)));
            table->setItem(i, 1, new QTableWidgetItem(list.at(k)));
        }
    }
}

void GenericTableWidget::loadTable(const QStringList & names, const QStringList & types, const QString & empty){
    if (names.size() == types.size()){
        if (table->rowCount() > 0){
            if (table->itemAt(0, 0)){
                table->itemAt(0, 0)->setText(empty);
            }else{
                table->setItem(0, 0, new QTableWidgetItem(empty));
            }
            if (table->itemAt(0, 1)){
                table->itemAt(0, 1)->setText(empty);
            }else{
                table->setItem(0, 1, new QTableWidgetItem(empty));
            }
        }else{
            table->setRowCount(table->rowCount() + 1);
            table->setItem(0, 0, new QTableWidgetItem(empty));
            table->setItem(0, 1, new QTableWidgetItem(empty));
        }
        for (int i = 1, j = 0; i < table->rowCount(), j < names.size(); i++, j++){
            if (i < table->rowCount()){
                if (table->itemAt(i, 0)){
                    table->itemAt(i, 0)->setText(names.at(j));
                }else{
                    table->setItem(i, 0, new QTableWidgetItem(names.at(j)));
                }
                if (table->itemAt(i, 1)){
                    table->itemAt(i, 1)->setText(types.at(j));
                }else{
                    table->setItem(i, 1, new QTableWidgetItem(types.at(j)));
                }
            }else{
                table->setRowCount(table->rowCount() + 1);
                table->setItem(i, 0, new QTableWidgetItem(names.at(j)));
                table->setItem(i, 1, new QTableWidgetItem(types.at(j)));
            }
        }
    }
}

void GenericTableWidget::loadTable(const QStringList & names, const QString & type, const QString & empty){
    if (table->rowCount() > 0){
        if (table->itemAt(0, 0)){
            table->itemAt(0, 0)->setText(empty);
        }else{
            table->setItem(0, 0, new QTableWidgetItem(empty));
        }
        if (table->itemAt(0, 1)){
            table->itemAt(0, 1)->setText(empty);
        }else{
            table->setItem(0, 1, new QTableWidgetItem(empty));
        }
    }else{
        table->setRowCount(table->rowCount() + 1);
        table->setItem(0, 0, new QTableWidgetItem(empty));
        table->setItem(0, 1, new QTableWidgetItem(empty));
    }
    for (int i = 1, j = 0; i < table->rowCount(), j < names.size(); i++, j++){
        if (i < table->rowCount()){
            if (table->itemAt(i, 0)){
                table->itemAt(i, 0)->setText(names.at(j));
            }else{
                table->setItem(i, 0, new QTableWidgetItem(names.at(j)));
            }
            if (table->itemAt(i, 1)){
                table->itemAt(i, 1)->setText(type);
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
    int i = table->rowCount();
    table->setRowCount(table->rowCount() + 1);
    table->setItem(i, 0, new QTableWidgetItem(name));
    table->setItem(i, 1, new QTableWidgetItem(type));
    table->setCurrentCell(table->rowCount() - 1, 0);
}

void GenericTableWidget::renameItem(int index, const QString & newname){
    if (index < table->rowCount() && index >= 0){
        table->itemAt(index, 0)->setText(newname);
    }
}

void GenericTableWidget::removeItem(int index){
    if (index < table->rowCount() && index >= 0){
        if (table->currentRow() == index){
            lastSelectedRow = -1;
        }
        table->removeRow(index);
    }
}

void GenericTableWidget::itemAdded(){
    addItem("New"+typeSelector->currentText(), typeSelector->currentText());
    emit elementAdded(table->rowCount() - 1);
}

void GenericTableWidget::showTable(int index){
    lastSelectedRow = index;
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

void GenericTableWidget::setTypes(const QStringList & typeNames){
    typeSelector->clear();
    typeSelector->addItems(typeNames);
}

