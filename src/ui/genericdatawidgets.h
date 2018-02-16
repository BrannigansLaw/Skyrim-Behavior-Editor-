#ifndef GENERICDATAWIDGETS_H
#define GENERICDATAWIDGETS_H

#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <limits.h>
#include <math.h>

#include <QSpinBox>
#include <QLineEdit>
#include <QGroupBox>

#include "src/utility.h"
#include "src/animData/skyrimanimdata.h"
#include "src/filetypes/behaviorfile.h"

#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QHeaderView>
#include <QSizePolicy>
#include <QLabel>
#include <QProgressDialog>

class CheckBox: public QWidget
{
    Q_OBJECT
signals:
    void released();
    void clicked(bool b);
public:
    CheckBox(const QString & text = "", QWidget *parent = nullptr)
        : QWidget(parent),
          checkbox(new QCheckBox(text, parent))
    {
        QHBoxLayout *lyt = new QHBoxLayout;
        lyt->addSpacing(1);
        lyt->addWidget(checkbox, Qt::AlignCenter);
        lyt->addSpacing(1);
        setLayout(lyt);
        connect(checkbox, SIGNAL(released()), this,  SIGNAL(released()));
        connect(checkbox, SIGNAL(clicked(bool)), this, SIGNAL(clicked(bool)));
    }

    void setChecked(bool checked){
        checkbox->setChecked(checked);
    }

    bool isChecked() const{
        return checkbox->isChecked();
    }
private:
    QCheckBox *checkbox;
};

class ProgressDialog: public QProgressDialog
{
public:
    ProgressDialog(const QString &labelText, const QString &cancelButtonText, int minimum, int maximum, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags())
        : QProgressDialog(labelText, cancelButtonText, minimum, maximum, parent, f)
    {
        setMinimumSize(400, 50);
        setMinimumDuration(0);
        setWindowModality(Qt::WindowModal);
    }

    void setProgress(const QString &labelText, int value){
        setLabelText(labelText);
        setValue(value);
    }
};

class Validator: public QValidator
{
public:
    QValidator::State validate(QString & input, int &) const Q_DECL_OVERRIDE{
        if (input == ""){
            return QValidator::Invalid;
        }else{
            return QValidator::Acceptable;
        }
    }
};

class LineEdit: public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(const QString & text = "", QWidget * par = 0)
        : QLineEdit(text, par)
    {
        setValidator(new Validator());
    }
};

class CheckButtonCombo: public QWidget{
    Q_OBJECT
public:
    CheckButtonCombo(const QString & buttontip = "", const QString & boxtext = "Enable:", bool disablebutton = true, const QString & buttontext = "Edit", QWidget * par = 0)
        : QWidget(par),
          label(new QLabel(boxtext)),
          checkBox(new CheckBox),
          pushButton(new QPushButton(buttontext)),
          disableButton(disablebutton)
    {
        QPalette p = palette();
        p.setColor(QPalette::Base, Qt::lightGray);
        setPalette(p);
        QHBoxLayout *lyt = new QHBoxLayout;
        pushButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        pushButton->setDisabled(disablebutton);
        pushButton->setToolTip(buttontip);
        lyt->addWidget(label, 1);
        lyt->addWidget(checkBox, 1);
        lyt->addWidget(pushButton, 6);
        lyt->setContentsMargins(0,0,0,0);
        setLayout(lyt);
        connect(checkBox, SIGNAL(clicked(bool)), this, SLOT(setChecked(bool)));
        connect(pushButton, SIGNAL(clicked(bool)), this, SIGNAL(pressed()));
    }

    void setText(const QString & buttontext){
        pushButton->setText(buttontext);
    }

    bool isChecked() const{
        return checkBox->isChecked();
    }
signals:
    void pressed();
    void enabled(bool enabled);
public slots:
    void setChecked(bool checked){
        if (disableButton){
            pushButton->setEnabled(checked);
        }
        disconnect(checkBox, SIGNAL(clicked(bool)), this, SLOT(setChecked(bool)));
        checkBox->setChecked(checked);
        connect(checkBox, SIGNAL(clicked(bool)), this, SLOT(setChecked(bool)));
        emit enabled(checked);
    }
private:
    QLabel *label;
    CheckBox *checkBox;
    QPushButton *pushButton;
    bool disableButton;
};

class ConditionValidator: public QValidator
{
public:
    QValidator::State validate(QString & input, int &) const Q_DECL_OVERRIDE{
        if (input.contains(">") || input.contains("<")){
            return QValidator::Invalid;
        }else{
            return QValidator::Acceptable;
        }
    }

    void fixup(QString & input) const{
        input.replace(">", "gt;");
        input.replace("<", "lt;");
    }
};

class ConditionLineEdit: public QLineEdit{
    Q_OBJECT
public:
    ConditionLineEdit(const QString & text = "", QWidget * par = 0)
        : QLineEdit(text, par)
    {
        setValidator(new ConditionValidator());
    }
};

class TableWidgetItem: public QTableWidgetItem
{
public:
    TableWidgetItem(const QString & text, int align = Qt::AlignLeft | Qt::AlignVCenter, const QColor & backgroundColor = QColor(Qt::white), const QBrush & textColor = QBrush(Qt::black), const QString & tip = "", bool checkable = false)
        : QTableWidgetItem(text)
    {
        if (checkable){
            setCheckState(Qt::Unchecked);
        }
        setTextAlignment(align);
        setBackgroundColor(backgroundColor);
        setForeground(textColor);
        setToolTip(tip);
    }
};

class TableWidget: public QTableWidget
{
    Q_OBJECT
signals:
    void itemDropped(int row1, int row2);
public:
    TableWidget(const QColor & background = QColor(Qt::white), QWidget *parent = 0)
        : QTableWidget(parent)
    {
        QPalette p = palette();
        p.setColor(QPalette::Base, background);
        setPalette(p);
        setMouseTracking(true);
        //setStyleSheet("QTableWidget { background:cyan }");
        setStyleSheet("QHeaderView::section { background-color:grey }");
        //verticalHeader()->setVisible(false);
        setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        horizontalHeader()->setSectionsClickable(false);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //setSelectionBehavior(QAbstractItemView::SelectRows);
        setSelectionMode(QAbstractItemView::SingleSelection);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE{
        QTableWidgetItem *item = itemAt(event->pos());
        int oldRow = currentRow();
        int newRow = row(item);
        if ((oldRow >= range.min && newRow >= range.min) && (oldRow <= range.max && newRow <= range.max)){
            if (swapRowItems(oldRow, newRow)){
                emit itemDropped(oldRow, newRow);
                return;
            }
        }
        emit itemDropped(-1, -1);
    }

    bool swapRowItems(int row1, int row2){
        if (rowCount() > row1 && rowCount() > row2 && row1 != row2 && row1 >= 0 && row2 >= 0){
            QTableWidgetItem *item1 = nullptr;
            QTableWidgetItem *item2 = nullptr;
            for (int i = 0; i < columnCount(); i++){
                item1 = takeItem(row1, i);
                item2 = takeItem(row2, i);
                setItem(row2, i, item1);
                setItem(row1, i, item2);
            }
            return true;
        }else{
            return false;
        }
    }

    QSize sizeHint() const{
        return QSize(100, 400);
    }

    void setRowSwapRange(int min, int max = std::numeric_limits<int>::max()){
        range.min = min;
        range.max = max;
    }

private:
    struct RowSwapRange{
        RowSwapRange(): min(0), max(std::numeric_limits<int>::max()){}
        int min;
        int max;
    };

    RowSwapRange range;
};

class SpinBox: public QSpinBox
{
    Q_OBJECT
public:
    SpinBox(QWidget* parent = 0)
        : QSpinBox(parent)
    {
        setMaximum(std::numeric_limits<int>::max());
        setMinimum(std::numeric_limits<int>::min());
        setFocusPolicy(Qt::StrongFocus);
    }

    void wheelEvent(QWheelEvent *e)
    {
        if(hasFocus()){
            QSpinBox::wheelEvent(e);
        }
    }
};

class DoubleSpinBox: public QDoubleSpinBox
{
    Q_OBJECT
public:
    DoubleSpinBox(QWidget* parent = 0, qreal value = 0, int precision = 6, qreal step = 1.0, qreal max = std::numeric_limits<double>::max(), qreal min = -std::numeric_limits<double>::max())
        : QDoubleSpinBox(parent)
    {
        setMaximum(max);
        setMinimum(min);
        setDecimals(precision);
        setSingleStep(step);
        setValue(value);
        setFocusPolicy(Qt::StrongFocus);
    }

    void wheelEvent(QWheelEvent *e)
    {
        if(hasFocus()){
            QDoubleSpinBox::wheelEvent(e);
        }
    }
};

class ComboBox: public QComboBox
{
    Q_OBJECT
public:
    ComboBox(QWidget* parent = 0)
        : QComboBox(parent)
    {
        //setStyleSheet("QComboBox:hover { background-color:red }");
        setFocusPolicy(Qt::StrongFocus);
    }

    void wheelEvent(QWheelEvent *e)
    {
        if(hasFocus()){
            QComboBox::wheelEvent(e);
        }
    }
};

class QuadVariableWidget: public QWidget
{
    Q_OBJECT
public:
    QuadVariableWidget()
        : lyt(new QHBoxLayout),
          spinBoxX(new DoubleSpinBox()),
          spinBoxY(new DoubleSpinBox),
          spinBoxZ(new DoubleSpinBox),
          spinBoxW(new DoubleSpinBox)
    {
        setStyleSheet("background-color: white;");
        spinBoxX->setPrefix("X: ");
        spinBoxY->setPrefix("Y: ");
        spinBoxZ->setPrefix("Z: ");
        spinBoxW->setPrefix("W: ");
        spinBoxX->setMaximum(std::numeric_limits<double>::max());
        spinBoxX->setMinimum(-std::numeric_limits<double>::max());
        spinBoxY->setMaximum(std::numeric_limits<double>::max());
        spinBoxY->setMinimum(-std::numeric_limits<double>::max());
        spinBoxZ->setMaximum(std::numeric_limits<double>::max());
        spinBoxZ->setMinimum(-std::numeric_limits<double>::max());
        spinBoxW->setMaximum(std::numeric_limits<double>::max());
        spinBoxW->setMinimum(-std::numeric_limits<double>::max());
        lyt->addWidget(spinBoxX);
        lyt->addWidget(spinBoxY);
        lyt->addWidget(spinBoxZ);
        lyt->addWidget(spinBoxW);
        setLayout(lyt);
        lyt->setMargin(0);
        spinBoxX->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        spinBoxY->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        spinBoxZ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        spinBoxW->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        connect(spinBoxX, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
        connect(spinBoxY, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
        connect(spinBoxZ, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
        connect(spinBoxW, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
    }

    /*QSize sizeHint() const{
        return QSize(800, 100);
    }*/

    void setVector4(){
        spinBoxW->setVisible(true);
    }

    void setVector3(){
        spinBoxW->setVisible(false);
    }

    hkQuadVariable value() const{
        return hkQuadVariable(spinBoxX->value(), spinBoxY->value(), spinBoxZ->value(), spinBoxW->value());
    }

    hkVector3 valueAsVector3() const{
        return hkVector3(spinBoxX->value(), spinBoxY->value(), spinBoxZ->value());
    }

    void setValue(const hkQuadVariable & value){
        spinBoxX->setValue(value.x);
        spinBoxY->setValue(value.y);
        spinBoxZ->setValue(value.z);
        spinBoxW->setValue(value.w);
    }

    void setValue(const hkVector3 & value){
        spinBoxX->setValue(value.x);
        spinBoxY->setValue(value.y);
        spinBoxZ->setValue(value.z);
    }

    virtual ~QuadVariableWidget(){}
signals:
    void editingFinished();
private:
    QHBoxLayout *lyt;
    DoubleSpinBox *spinBoxX;
    DoubleSpinBox *spinBoxY;
    DoubleSpinBox *spinBoxZ;
    DoubleSpinBox *spinBoxW;
};

class GenericTableWidget: public QWidget
{
    Q_OBJECT
public:
    GenericTableWidget(const QString & title);
    void loadTable(const QStringList & names, const QStringList & types, const QString & firstElement = "");
    void loadTable(const QStringList & names, const QString & type, const QString & firstElement = "");
    void addItem(const QString & name, const QString & type);
    void renameItem(int index, const QString & newname);
    void removeItem(int index);
    int getNumRows() const;
    //void setTypes(const QStringList & typeNames);
signals:
    void elementSelected(int index, const QString & name);
    void elementAdded(int index, const QString & type);
protected:
private slots:
    void itemSelected();
    //void itemAdded();
    void showTable(int index);
    void showTable(const QString & name);
private:
    QGridLayout *lyt;
    QTableWidget *table;
    QPushButton *selectPB;
    QPushButton *cancelPB;
    //QPushButton *newPB;
    //ComboBox *typeSelector;
    int lastSelectedRow;
};

#endif // GENERICDATAWIDGETS_H
