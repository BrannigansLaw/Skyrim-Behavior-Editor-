#ifndef DATAWIDGETS_H
#define DATAWIDGETS_H

#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <limits.h>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QGroupBox>

#include "src/utility.h"
#include "src/filetypes/behaviorfile.h"

#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QHeaderView>
#include <QSizePolicy>

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

class LineEdit: public QLineEdit{
    Q_OBJECT
public:
    LineEdit(const QString & text = "", QWidget * par = 0)
        : QLineEdit(text, par)
    {
        setValidator(new Validator());
    }
};

class TableWidget: public QTableWidget
{
    Q_OBJECT
public:
    TableWidget(QWidget* parent = 0)
        : QTableWidget(parent)
    {
        verticalHeader()->setVisible(false);
        setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setSelectionMode(QAbstractItemView::SingleSelection);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

    QSize sizeHint() const{
        return QSize(100, 400);
    }
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
        connect(spinBoxX, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
        connect(spinBoxY, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
        connect(spinBoxZ, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
        connect(spinBoxW, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
    }

    /*QSize sizeHint() const{
        return QSize(800, 100);
    }*/

    hkQuadVariable getValue() const{
        return hkQuadVariable(spinBoxX->value(), spinBoxY->value(), spinBoxZ->value(), spinBoxW->value());
    }

    void setValue(const hkQuadVariable & value){
        spinBoxX->setValue(value.x);
        spinBoxY->setValue(value.y);
        spinBoxZ->setValue(value.z);
        spinBoxW->setValue(value.w);
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
    void setTypes(const QStringList & typeNames);
signals:
    void elementSelected(int index, const QString & name);
    void elementAdded(int index, const QString & type);
protected:
private slots:
    void itemSelected();
    void itemAdded();
    void showTable(int index);
private:
    QGridLayout *lyt;
    TableWidget *table;
    QPushButton *selectPB;
    QPushButton *cancelPB;
    QPushButton *newPB;
    ComboBox *typeSelector;
    int lastSelectedRow;
};

#endif // DATAWIDGETS_H
