#include "genericdatawidgets.h"

HkxObjectTableWidget::HkxObjectTableWidget(const QString & title)
    : lyt(new QGridLayout),
      table(new TableWidget),
      selectPB(new QPushButton("Select")),
      cancelPB(new QPushButton("Cancel"))
{
    setWindowTitle(title);
    setWindowModality(Qt::WindowModal);
    QStringList list = {"Name", "Type"};
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(list);
    lyt->addWidget(table, 0, 0, 8, 10);
    lyt->addWidget(selectPB, 8, 5, 1, 2);
    lyt->addWidget(cancelPB, 8, 7, 1, 2);
    setLayout(lyt);
    connect(selectPB, SIGNAL(released()), this, SLOT(itemSelected()));
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(itemSelected()));
    connect(cancelPB, SIGNAL(released()), this, SIGNAL(hideWindow()));
}

void HkxObjectTableWidget::loadTable(BehaviorFile *file, bool isGenerator){
    if (file){
        if (isGenerator){
            QStringList list = file->getGeneratorNamesAndTypeNames();
            for (int i = 0, j = 0, k = 1; i < list.size(), j < list.size(), k < list.size(); i++, j+=2, k+=2){
                if (i < table->rowCount()){
                    table->itemAt(i, 0)->setText(list.at(j));
                    table->itemAt(i, 1)->setText(list.at(k));
                }else{
                    table->setRowCount(table->rowCount() + 1);
                    table->setItem(i, 0, new QTableWidgetItem(list.at(j)));
                    table->setItem(i, 1, new QTableWidgetItem(list.at(k)));
                }
            }
        }
    }
}

int HkxObjectTableWidget::getNumRows() const{
    return table->rowCount();
}

void HkxObjectTableWidget::addItem(const QString & name, const QString & type){
    int i = table->rowCount();
    table->setRowCount(table->rowCount() + 1);
    table->setItem(i, 0, new QTableWidgetItem(name));
    table->setItem(i, 1, new QTableWidgetItem(type));
}

void HkxObjectTableWidget::renameItem(int index, const QString & newname){
    if (index < table->rowCount()){
        table->itemAt(index, 0)->setText(newname);
    }
}

void HkxObjectTableWidget::removeItem(int index){
    if (index < table->rowCount()){
        //delete table->itemAt(index, 0);
        //delete table->itemAt(index, 1);
        table->removeRow(index);
    }
}

void HkxObjectTableWidget::setSelectedRow(int row){
    table->setCurrentCell(row, 0);
}

/*void HkxObjectTableWidget::showWidget(){
    setVisible(true);
}

void HkxObjectTableWidget::hideWidget(){
    hide();
}*/

void HkxObjectTableWidget::itemSelected(){
    emit elementSelected(table->currentRow());
}
