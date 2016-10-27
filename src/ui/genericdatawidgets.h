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

#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QHeaderView>
#include <QSizePolicy>

class TableWidget: public QTableWidget
{
    Q_OBJECT
public:
    TableWidget(QWidget* parent = 0)
        : QTableWidget(parent)
    {
        setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

    /*QSize minimumSizeHint() const
    {
        QSize size(QTableWidget::sizeHint());
        int width = 0;
        for (int a = 0; a < columnCount(); ++a)
        {
            width += columnWidth(a);
        }
        size.setWidth(width + (columnCount() * 1));
        int height = horizontalHeader()->height();
        for (int a = 0; a < rowCount(); ++a)
        {
            height += rowHeight(a);
        }
        size.setHeight(height + (rowCount() * 1));
        return size;
    }*/

    /*QSize sizeHint() const
    {
        return QSize(250, 250);
    }*/
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
    DoubleSpinBox(QWidget* parent = 0)
        : QDoubleSpinBox(parent)
    {
        setMaximum(std::numeric_limits<double>::max());
        setMinimum(std::numeric_limits<double>::min());
        setDecimals(6);
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
          spinBoxX(new DoubleSpinBox),
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
        spinBoxX->setMinimum(std::numeric_limits<double>::min());
        spinBoxY->setMaximum(std::numeric_limits<double>::max());
        spinBoxY->setMinimum(std::numeric_limits<double>::min());
        spinBoxZ->setMaximum(std::numeric_limits<double>::max());
        spinBoxZ->setMinimum(std::numeric_limits<double>::min());
        spinBoxW->setMaximum(std::numeric_limits<double>::max());
        spinBoxW->setMinimum(std::numeric_limits<double>::min());
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

#endif // DATAWIDGETS_H