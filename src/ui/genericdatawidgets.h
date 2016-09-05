#ifndef DATAWIDGETS_H
#define DATAWIDGETS_H

#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <limits.h>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QGroupBox>

#include "src/utility.h"

#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>

class BoolWidget: public QGroupBox
{
    Q_OBJECT
public:
    BoolWidget(const QString & name)
        : lyt(new QHBoxLayout),
          checkBox(new QCheckBox),
          boundVariable(new QComboBox),
          nameLE(new QLineEdit(name)),
          boundVariableL(new QLabel("Bound Variable: "))
    {
        setStyleSheet("background-color: white;");
        lyt->addWidget(nameLE, 1);
        lyt->addWidget(new QLabel("Type: Bool"), 1);
        lyt->addWidget(boundVariableL, 1);
        lyt->addWidget(boundVariable, 1);
        lyt->addWidget(checkBox, 1);
        setLayout(lyt);
        nameLE->setReadOnly(true);
        connect(nameLE, SIGNAL(textEdited(QString)), this, SIGNAL(textEdited(QString)));
        connect(checkBox, SIGNAL(released()), this, SIGNAL(editingDone()));
        connect(boundVariable, SIGNAL(activated(int)), this, SIGNAL(editingDone()));
    }

    bool getValue() const{
        return checkBox->isChecked();
    }

    void setName(const QString & name){
        nameLE->setText(name);
    }

    void setValue(bool value){
        checkBox->setChecked(value);
    }

    void setBoundVariableList(const QStringList & sl){
        boundVariable->clear();
        boundVariable->addItem("None");
        boundVariable->addItems(sl);
    }

    void setBoundVariableVisbility(bool visable){
        boundVariable->setVisible(visable);
        boundVariableL->setVisible(false);
    }

    void setNameReadOnly(bool readOnly){
        nameLE->setReadOnly(readOnly);
    }

    virtual ~BoolWidget(){}
signals:
    void editingDone();
    void textEdited(QString newText);
private:
    QHBoxLayout *lyt;
    QCheckBox *checkBox;
    QComboBox *boundVariable;
    QLineEdit *nameLE;
    QLabel *boundVariableL;
};

class IntWidget: public QGroupBox
{
    Q_OBJECT
public:
    IntWidget(const QString & name)
        : lyt(new QHBoxLayout),
          spinBox(new QSpinBox),
          boundVariable(new QComboBox),
          nameLE(new QLineEdit(name)),
          boundVariableL(new QLabel("Bound Variable: "))
    {
        setStyleSheet("background-color: white;");
        spinBox->setMaximum(std::numeric_limits<int>::max());
        spinBox->setMinimum(std::numeric_limits<int>::min());
        lyt->addWidget(nameLE, 1);
        lyt->addWidget(new QLabel("Type: Int"), 1);
        lyt->addWidget(boundVariableL, 1);
        lyt->addWidget(boundVariable, 1);
        lyt->addWidget(spinBox, 1);
        setLayout(lyt);
        nameLE->setReadOnly(true);
        connect(nameLE, SIGNAL(textEdited(QString)), this, SIGNAL(textEdited(QString)));
        connect(spinBox, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(boundVariable, SIGNAL(activated(int)), this, SIGNAL(editingDone()));
    }

    int getValue() const{
        return spinBox->value();
    }

    void setValue(int value){
        spinBox->setValue(value);
    }

    void setName(const QString & name){
        nameLE->setText(name);
    }

    void setBoundVariableList(const QStringList & sl){
        boundVariable->clear();
        boundVariable->addItem("None");
        boundVariable->addItems(sl);
    }

    void setBoundVariableVisbility(bool visable){
        boundVariable->setVisible(visable);
        boundVariableL->setVisible(false);
    }

    void setNameReadOnly(bool readOnly){
        nameLE->setReadOnly(readOnly);
    }

    virtual ~IntWidget(){}
signals:
    void editingDone();
    void textEdited(QString newText);
private:
    QHBoxLayout *lyt;
    QSpinBox *spinBox;
    QComboBox *boundVariable;
    QLineEdit *nameLE;
    QLabel *boundVariableL;
};

class DoubleWidget: public QGroupBox
{
    Q_OBJECT
public:
    DoubleWidget(const QString & name)
        : lyt(new QHBoxLayout),
          spinBox(new QDoubleSpinBox),
          boundVariable(new QComboBox),
          nameLE(new QLineEdit(name)),
          boundVariableL(new QLabel("Bound Variable: "))
    {
        setStyleSheet("background-color: white;");
        spinBox->setMaximum(std::numeric_limits<double>::max());
        spinBox->setMinimum(std::numeric_limits<double>::min());
        lyt->addWidget(nameLE, 1);
        lyt->addWidget(new QLabel("Type: Double"), 1);
        lyt->addWidget(boundVariableL, 1);
        lyt->addWidget(boundVariable, 1);
        lyt->addWidget(spinBox, 1);
        setLayout(lyt);
        nameLE->setReadOnly(true);
        connect(nameLE, SIGNAL(textEdited(QString)), this, SIGNAL(textEdited(QString)));
        connect(spinBox, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(boundVariable, SIGNAL(activated(int)), this, SIGNAL(editingDone()));
    }

    qreal getValue() const{
        return spinBox->value();
    }

    void setValue(qreal value){
        spinBox->setValue(value);
    }

    void setName(const QString & name){
        nameLE->setText(name);
    }

    void setBoundVariableList(const QStringList & sl){
        boundVariable->clear();
        boundVariable->addItem("None");
        boundVariable->addItems(sl);
    }

    void setBoundVariableVisbility(bool visable){
        boundVariable->setVisible(visable);
        boundVariableL->setVisible(false);
    }

    void setNameReadOnly(bool readOnly){
        nameLE->setReadOnly(readOnly);
    }

    virtual ~DoubleWidget(){}
signals:
    void editingDone();
    void textEdited(QString newText);
private:
    QHBoxLayout *lyt;
    QDoubleSpinBox *spinBox;
    QComboBox *boundVariable;
    QLineEdit *nameLE;
    QLabel *boundVariableL;
};

class StringWidget: public QGroupBox
{
    Q_OBJECT
public:
    StringWidget(const QString & name)
        : lyt(new QHBoxLayout),
          lineEdit(new QLineEdit),
          nameLE(new QLineEdit(name)),
          boundVariableL(new QLabel("Bound Variable: "))
    {
        setStyleSheet("background-color: white;");
        lyt->addWidget(nameLE, 1);
        lyt->addWidget(new QLabel("Type: String"), 1);
        lyt->addWidget(lineEdit, 1);
        setLayout(lyt);
        connect(lineEdit, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
    }

    QString getText()const{
        return lineEdit->text();
    }

    void setName(const QString & name){
        nameLE->setText(name);
    }

    void setText(const QString & text){
        lineEdit->setText(text);
    }

    virtual ~StringWidget(){}
signals:
    void editingDone();
private:
    QHBoxLayout *lyt;
    QLineEdit *lineEdit;
    QLineEdit *nameLE;
    QLabel *boundVariableL;
};

class PointerWidget: public QGroupBox
{
    Q_OBJECT
public:
    PointerWidget(const QString & name, const QString & type)
        : lyt(new QHBoxLayout),
          pointers(new QComboBox),
          lastIndex(1),
          nameLE(new QLineEdit(name)),
          boundVariableL(new QLabel("Bound Variable: "))
    {
        setStyleSheet("background-color: white;");
        lyt->addWidget(nameLE, 1);
        lyt->addWidget(new QLabel("Type: "+type), 1);
        lyt->addWidget(pointers, 3);
        setLayout(lyt);
        connect(pointers, SIGNAL(currentIndexChanged(int)), this, SIGNAL(editingDone(int)));
    }

    void setStringList(const QStringList & sl){
        list.clear();
        pointers->clear();
        list.append("None");
        list.append(sl);
        pointers->addItems(list);
    }

    void setSelectedItem(int index){
        if (pointers->currentIndex() > 0){
            lastIndex = pointers->currentIndex();
        }
        pointers->setCurrentIndex(index);
    }

    void setName(const QString & name){
        nameLE->setText(name);
    }

    int currentIndex() const{
        return pointers->currentIndex();
    }

    void setLastIndex(int i){
        lastIndex = i;
    }

    int getLastIndex() const{
        return lastIndex;
    }

    void silence(){
        disconnect(pointers, SIGNAL(currentIndexChanged(int)), this, SIGNAL(editingDone(int)));
    }

    void reconnect(){
        connect(pointers, SIGNAL(currentIndexChanged(int)), this, SIGNAL(editingDone(int)));
    }

    virtual ~PointerWidget(){}
signals:
    void editingDone(int index);
private:
    QHBoxLayout *lyt;
    QComboBox *pointers;
    QStringList list;
    int lastIndex;
    QLineEdit *nameLE;
    QLabel *boundVariableL;
};

class QuadVariableWidget: public QGroupBox
{
    Q_OBJECT
public:
    QuadVariableWidget(const QString & name, const QString & type = "hkQuadVariable")
        : lyt(new QHBoxLayout),
          spinBoxX(new QDoubleSpinBox),
          spinBoxY(new QDoubleSpinBox),
          spinBoxZ(new QDoubleSpinBox),
          spinBoxW(new QDoubleSpinBox),
          boundVariable(new QComboBox),
          nameLE(new QLineEdit(name)),
          boundVariableL(new QLabel("Bound Variable: "))
    {
        setStyleSheet("background-color: white;");
        spinBoxX->setMaximum(std::numeric_limits<double>::max());
        spinBoxX->setMinimum(std::numeric_limits<double>::min());
        spinBoxY->setMaximum(std::numeric_limits<double>::max());
        spinBoxY->setMinimum(std::numeric_limits<double>::min());
        spinBoxZ->setMaximum(std::numeric_limits<double>::max());
        spinBoxZ->setMinimum(std::numeric_limits<double>::min());
        spinBoxW->setMaximum(std::numeric_limits<double>::max());
        spinBoxW->setMinimum(std::numeric_limits<double>::min());
        lyt->addWidget(nameLE);
        lyt->addWidget(new QLabel("Type: "+type));
        lyt->addWidget(boundVariableL, 1);
        lyt->addWidget(boundVariable, 1);
        lyt->addWidget(new QLabel("X"));
        lyt->addWidget(spinBoxX);
        lyt->addWidget(new QLabel("Y"));
        lyt->addWidget(spinBoxY);
        lyt->addWidget(new QLabel("Z"));
        lyt->addWidget(spinBoxZ);
        lyt->addWidget(new QLabel("W"));
        lyt->addWidget(spinBoxW);
        setLayout(lyt);
        nameLE->setReadOnly(true);
        connect(nameLE, SIGNAL(textEdited(QString)), this, SIGNAL(textEdited(QString)));
        connect(spinBoxX, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(spinBoxY, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(spinBoxZ, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(spinBoxW, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(boundVariable, SIGNAL(activated(int)), this, SIGNAL(editingDone()));
    }

    void setName(const QString & name){
        nameLE->setText(name);
    }

    hkQuadVariable getValue() const{
        return hkQuadVariable(spinBoxX->value(), spinBoxY->value(), spinBoxZ->value(), spinBoxW->value());
    }

    void setValue(const hkQuadVariable & value){
        spinBoxX->setValue(value.x);
        spinBoxY->setValue(value.y);
        spinBoxZ->setValue(value.z);
        spinBoxW->setValue(value.w);
    }

    void setBoundVariableList(const QStringList & sl){
        boundVariable->clear();
        boundVariable->addItem("None");
        boundVariable->addItems(sl);
    }

    void setBoundVariableVisbility(bool visable){
        boundVariable->setVisible(visable);
        boundVariableL->setVisible(false);
    }

    void setNameReadOnly(bool readOnly){
        nameLE->setReadOnly(readOnly);
    }

    virtual ~QuadVariableWidget(){}
signals:
    void editingDone();
    void textEdited(QString newText);
private:
    QHBoxLayout *lyt;
    QDoubleSpinBox *spinBoxX;
    QDoubleSpinBox *spinBoxY;
    QDoubleSpinBox *spinBoxZ;
    QDoubleSpinBox *spinBoxW;
    QComboBox *boundVariable;
    QLineEdit *nameLE;
    QLabel *boundVariableL;
};

class GenericTabelWidget: public QGroupBox
{
    Q_OBJECT
public:
    GenericTabelWidget(const QString & title)
        : lyt(new QGridLayout),
          view(new QTableWidget),
          addNewItemPB(new QPushButton("Add new Object")),
          removeSelectedItemPB(new QPushButton("Remove selected Object")),
          typeSelectorCB(new QComboBox),
          addExistingObjectPB(new QPushButton("Add selected existing Object")),
          existingObjectListCB(new QComboBox)
    {
        setTitle(title);
        QStringList list = {"Name", "Type", "Value"};
        view->setColumnCount(3);
        view->setHorizontalHeaderLabels(list);
        view->setSelectionBehavior(QAbstractItemView::SelectRows);
        view->setEditTriggers(QAbstractItemView::NoEditTriggers);
        view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        lyt->addWidget(view, 1, 0, 5, 6);
        lyt->addWidget(addNewItemPB, 0, 0, 1, 2);
        lyt->addWidget(typeSelectorCB, 0, 2, 1, 2);
        lyt->addWidget(removeSelectedItemPB, 0, 4, 1, 2);
        lyt->addWidget(addExistingObjectPB, 6, 0, 1, 2);
        lyt->addWidget(existingObjectListCB, 6, 2, 1, 4);
        setLayout(lyt);
        connect(view, SIGNAL(cellClicked(int,int)), this, SIGNAL(itemSelected(int, int)));
        connect(addNewItemPB, SIGNAL(pressed()), this, SLOT(added()));
        connect(removeSelectedItemPB, SIGNAL(pressed()), this, SLOT(remove()));
        connect(addExistingObjectPB, SIGNAL(pressed()), this, SLOT(addExisting()));
    }

    void clear(){
        //view->clearContents();
        for (int i = view->rowCount() - 1; i >= 0; i--){
            view->removeRow(i);
        }
    }

    void renameItemAt(int row, const QString & name){
        if (view->rowCount() > row && row > -1){
            view->item(row, 0)->setText(name);
        }
    }

    int getCurrentType() const{
        return typeSelectorCB->currentIndex();
    }

    void setTypes(const QStringList & types){
        typeSelectorCB->clear();
        typeSelectorCB->addItems(types);
    }

    void setObjectList(const QStringList & list){
        existingObjectListCB->clear();
        existingObjectListCB->addItems(list);
    }

    void addItem(const QString & name){
        view->setRowCount(view->rowCount() + 1);
        if (view->columnCount() < 3) return;
        view->setItem(view->rowCount() - 1, 0, new QTableWidgetItem(name));
        view->setItem(view->rowCount() - 1, 1, new QTableWidgetItem(typeSelectorCB->currentText()));
        view->setItem(view->rowCount() - 1, 2, new QTableWidgetItem(tr("Click to Edit")));
        view->setCurrentItem(view->item(view->rowCount() - 1, 0));
        view->setVisible(false);
        view->resizeColumnsToContents();
        view->setVisible(true);
    }

    void addItem(const QString & name, const QString & type){
        view->setRowCount(view->rowCount() + 1);
        if (view->columnCount() < 3) return;
        view->setItem(view->rowCount() - 1, 0, new QTableWidgetItem(name));
        view->setItem(view->rowCount() - 1, 1, new QTableWidgetItem(type));
        view->setItem(view->rowCount() - 1, 2, new QTableWidgetItem(tr("Click to Edit")));
        view->setCurrentItem(view->item(view->rowCount() - 1, 0));
        view->setVisible(false);
        view->resizeColumnsToContents();
        view->setVisible(true);
    }

    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE{
        view->setColumnWidth(0, view->viewport()->size().width()*0.4);
        view->setColumnWidth(1, view->viewport()->size().width()*0.4);
        view->setColumnWidth(2, view->viewport()->size().width()*0.2);
    }

    virtual ~GenericTabelWidget(){
        //
    }
signals:
    void itemAdded(int type);
    void existingItemAdded(int index);
    void itemRemoved(int row);
    void itemSelected(int row, int column);
private slots:
    void added(){
        int num = 0;
        QString name = "NEW_"+typeSelectorCB->currentText();
        for (int i = 0; i < view->rowCount(); i++){
            if (view->item(i, 0)->data(Qt::DisplayRole) == name){
                name.append("_"+QString::number(num));
                num++;
                i = 0;
                if (num > 1000){
                    return;
                }
            }
        }
        addItem(name);
        emit itemAdded(typeSelectorCB->currentIndex());
    }

    void addExisting(){
        addItem(existingObjectListCB->currentText().split("Type:").first());
        emit existingItemAdded(existingObjectListCB->currentIndex());
    }

    void remove(){
        QModelIndex index = view->currentIndex();
        if (!index.isValid()) return;
        if (!view->item(index.row(), index.column())->isSelected()) return;
        emit itemRemoved(view->currentRow());
        view->removeRow(view->currentRow());
        view->setVisible(false);
        view->resizeColumnsToContents();
        view->setVisible(true);
    }
private:
    QGridLayout *lyt;
    QTableWidget *view;
    QPushButton *addNewItemPB;
    QPushButton *removeSelectedItemPB;
    QComboBox *typeSelectorCB;
    QPushButton *addExistingObjectPB;
    QComboBox *existingObjectListCB;
};

class GenericListWidget: public QGroupBox
{
    Q_OBJECT
public:
    GenericListWidget(const QString & title)
        : lyt(new QGridLayout),
          view(new QTableWidget),
          addNewItemPB(new QPushButton("Add new Object")),
          removeSelectedItemPB(new QPushButton("Remove selected Object")),
          typeSelectorCB(new QComboBox)
    {
        setTitle(title);
        QStringList list = {"Name", "Type", "Value"};
        view->setColumnCount(3);
        view->setHorizontalHeaderLabels(list);
        view->setSelectionBehavior(QAbstractItemView::SelectRows);
        view->setEditTriggers(QAbstractItemView::NoEditTriggers);
        view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        lyt->addWidget(view, 1, 0, 5, 6);
        lyt->addWidget(addNewItemPB, 0, 0, 1, 2);
        lyt->addWidget(typeSelectorCB, 0, 2, 1, 2);
        lyt->addWidget(removeSelectedItemPB, 0, 4, 1, 2);
        setLayout(lyt);
        connect(view, SIGNAL(cellClicked(int,int)), this, SIGNAL(itemSelected(int, int)));
        connect(addNewItemPB, SIGNAL(pressed()), this, SLOT(added()));
        connect(removeSelectedItemPB, SIGNAL(pressed()), this, SLOT(remove()));
    }

    void clear(){
        //view->clearContents();
        for (int i = view->rowCount() - 1; i >= 0; i--){
            view->removeRow(i);
        }
    }

    void renameItemAt(int row, const QString & name){
        if (view->rowCount() > row && row > -1){
            view->item(row, 0)->setText(name);
        }
    }

    QString getItemNameAt(int index){
        if (view->rowCount() > index){
            return view->item(index, 0)->text();
        }
        return "";
    }

    QString getLastItemName(){
        if (view->rowCount() > 0){
            return view->item(view->rowCount() - 1, 0)->text();
        }
        return "";
    }

    int getSelectedRow() const{
        return view->currentRow();
    }

    int getCurrentType() const{
        return typeSelectorCB->currentIndex();
    }

    void setTypes(const QStringList & types){
        typeSelectorCB->clear();
        typeSelectorCB->addItems(types);
    }

    void addItem(const QString & name){
        view->setRowCount(view->rowCount() + 1);
        if (view->columnCount() < 3) return;
        view->setItem(view->rowCount() - 1, 0, new QTableWidgetItem(name));
        view->setItem(view->rowCount() - 1, 1, new QTableWidgetItem(typeSelectorCB->currentText()));
        view->setItem(view->rowCount() - 1, 2, new QTableWidgetItem(tr("Click to Edit")));
        view->setCurrentItem(view->item(view->rowCount() - 1, 0));
        view->setVisible(false);
        view->resizeColumnsToContents();
        view->setVisible(true);
    }

    void addItem(const QString & name, const QString & type){
        view->setRowCount(view->rowCount() + 1);
        if (view->columnCount() < 3) return;
        view->setItem(view->rowCount() - 1, 0, new QTableWidgetItem(name));
        view->setItem(view->rowCount() - 1, 1, new QTableWidgetItem(type));
        view->setItem(view->rowCount() - 1, 2, new QTableWidgetItem(tr("Click to Edit")));
        view->setCurrentItem(view->item(view->rowCount() - 1, 0));
        view->setVisible(false);
        view->resizeColumnsToContents();
        view->setVisible(true);
    }

    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE{
        view->setColumnWidth(0, view->viewport()->size().width()*0.4);
        view->setColumnWidth(1, view->viewport()->size().width()*0.4);
        view->setColumnWidth(2, view->viewport()->size().width()*0.2);
    }

    virtual ~GenericListWidget(){
        //
    }
signals:
    void itemAdded(int type);
    void itemRemoved(int row);
    void itemSelected(int row, int column);
private slots:
    void added(){
        int num = 0;
        QString name = "NEW_"+typeSelectorCB->currentText();
        for (int i = 0; i < view->rowCount(); i++){
            if (view->item(i, 0)->data(Qt::DisplayRole) == name){
                name.append("_"+QString::number(num));
                num++;
                i = 0;
                if (num > 1000){
                    return;
                }
            }
        }
        addItem(name);
        emit itemAdded(typeSelectorCB->currentIndex());
    }

    void remove(){
        QModelIndex index = view->currentIndex();
        if (!index.isValid()) return;
        if (!view->item(index.row(), index.column())->isSelected()) return;
        emit itemRemoved(view->currentRow());
        view->removeRow(view->currentRow());
        view->setVisible(false);
        view->resizeColumnsToContents();
        view->setVisible(true);
    }
private:
    QGridLayout *lyt;
    QTableWidget *view;
    QPushButton *addNewItemPB;
    QPushButton *removeSelectedItemPB;
    QComboBox *typeSelectorCB;
    QPushButton *addExistingObjectPB;
    QComboBox *existingObjectListCB;
};

#endif // DATAWIDGETS_H
