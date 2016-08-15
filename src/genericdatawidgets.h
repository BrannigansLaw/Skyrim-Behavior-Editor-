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

#include "utility.h"

class BoolWidget: public QGroupBox
{
    Q_OBJECT
public:
    BoolWidget(const QString & name)
        : lyt(new QHBoxLayout),
          checkBox(new QCheckBox),
          boundVariable(new QComboBox)
    {
        setStyleSheet("background-color: white;");
        lyt->addWidget(new QLabel(name), 1);
        lyt->addWidget(new QLabel("Type: Bool"), 1);
        lyt->addWidget(new QLabel("Bound Variable:"), 1);
        lyt->addWidget(boundVariable, 1);
        lyt->addWidget(checkBox, 1);
        setLayout(lyt);
        connect(checkBox, SIGNAL(released()), this, SIGNAL(editingDone()));
        connect(boundVariable, SIGNAL(activated(int)), this, SIGNAL(editingDone()));
    }

    bool getValue() const{
        return checkBox->isChecked();
    }

    void setValue(bool value){
        checkBox->setChecked(value);
    }

    void setBoundVariableList(const QStringList & sl){
        boundVariable->clear();
        boundVariable->addItem("None");
        boundVariable->addItems(sl);
    }

    virtual ~BoolWidget(){}
signals:
    void editingDone();
private:
    QHBoxLayout *lyt;
    QCheckBox *checkBox;
    QComboBox *boundVariable;
};

class IntWidget: public QGroupBox
{
    Q_OBJECT
public:
    IntWidget(const QString & name)
        : lyt(new QHBoxLayout),
          spinBox(new QSpinBox),
          boundVariable(new QComboBox)
    {
        setStyleSheet("background-color: white;");
        spinBox->setMaximum(std::numeric_limits<int>::max());
        spinBox->setMinimum(std::numeric_limits<int>::min());
        lyt->addWidget(new QLabel(name), 1);
        lyt->addWidget(new QLabel("Type: Int"), 1);
        lyt->addWidget(new QLabel("Bound Variable:"), 1);
        lyt->addWidget(boundVariable, 1);
        lyt->addWidget(spinBox, 1);
        setLayout(lyt);
        connect(spinBox, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(boundVariable, SIGNAL(activated(int)), this, SIGNAL(editingDone()));
    }

    int getValue() const{
        return spinBox->value();
    }

    void setValue(int value){
        spinBox->setValue(value);
    }

    void setBoundVariableList(const QStringList & sl){
        boundVariable->clear();
        boundVariable->addItem("None");
        boundVariable->addItems(sl);
    }

    virtual ~IntWidget(){}
signals:
    void editingDone();
private:
    QHBoxLayout *lyt;
    QSpinBox *spinBox;
    QComboBox *boundVariable;
};

class DoubleWidget: public QGroupBox
{
    Q_OBJECT
public:
    DoubleWidget(const QString & name)
        : lyt(new QHBoxLayout),
          spinBox(new QDoubleSpinBox),
          boundVariable(new QComboBox)
    {
        setStyleSheet("background-color: white;");
        spinBox->setMaximum(std::numeric_limits<double>::max());
        spinBox->setMinimum(std::numeric_limits<double>::min());
        lyt->addWidget(new QLabel(name), 1);
        lyt->addWidget(new QLabel("Type: Double"), 1);
        lyt->addWidget(new QLabel("Bound Variable:"), 1);
        lyt->addWidget(boundVariable, 1);
        lyt->addWidget(spinBox, 1);
        setLayout(lyt);
        connect(spinBox, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(boundVariable, SIGNAL(activated(int)), this, SIGNAL(editingDone()));
    }

    qreal getValue() const{
        return spinBox->value();
    }

    void setValue(qreal value){
        spinBox->setValue(value);
    }

    void setBoundVariableList(const QStringList & sl){
        boundVariable->clear();
        boundVariable->addItem("None");
        boundVariable->addItems(sl);
    }

    virtual ~DoubleWidget(){}
signals:
    void editingDone();
private:
    QHBoxLayout *lyt;
    QDoubleSpinBox *spinBox;
    QComboBox *boundVariable;
};

class StringWidget: public QGroupBox
{
    Q_OBJECT
public:
    StringWidget(const QString & name)
        : lyt(new QHBoxLayout),
          lineEdit(new QLineEdit)
    {
        setStyleSheet("background-color: white;");
        lyt->addWidget(new QLabel(name), 1);
        lyt->addWidget(new QLabel("Type: String"), 1);
        lyt->addWidget(lineEdit, 1);
        setLayout(lyt);
        connect(lineEdit, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
    }

    QString getText()const{
        return lineEdit->text();
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
};

class PointerWidget: public QGroupBox
{
    Q_OBJECT
public:
    PointerWidget(const QString & name, const QString & type)
        : lyt(new QHBoxLayout),
          pointers(new QComboBox)
    {
        setStyleSheet("background-color: white;");
        lyt->addWidget(new QLabel(name), 1);
        lyt->addWidget(new QLabel("Type: "+type), 1);
        lyt->addWidget(pointers, 1);
        setLayout(lyt);
        connect(pointers, SIGNAL(activated(int)), this, SIGNAL(editingDone(int)));
    }

    void setStringList(const QStringList & sl){
        list.clear();
        list.append("None");
        list.append(sl);
        pointers->addItems(list);
    }

    void setSelectedItem(int index){
        pointers->setCurrentIndex(index);
    }

    virtual ~PointerWidget(){}
signals:
    void editingDone(int index);
private:
    QHBoxLayout *lyt;
    QComboBox *pointers;
    QStringList list;
};

class Vector4Widget: public QGroupBox
{
    Q_OBJECT
public:
    Vector4Widget(const QString & name)
        : lyt(new QHBoxLayout),
          spinBoxX(new QDoubleSpinBox),
          spinBoxY(new QDoubleSpinBox),
          spinBoxZ(new QDoubleSpinBox),
          spinBoxW(new QDoubleSpinBox),
          boundVariable(new QComboBox)
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
        lyt->addWidget(new QLabel(name));
        lyt->addWidget(new QLabel("Type: Vector4"));
        lyt->addWidget(new QLabel("Bound Variable:"), 1);
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
        connect(spinBoxX, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(spinBoxY, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(spinBoxZ, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(spinBoxW, SIGNAL(editingFinished()), this, SIGNAL(editingDone()));
        connect(boundVariable, SIGNAL(activated(int)), this, SIGNAL(editingDone()));
    }

    hkVector4 getValue() const{
        return hkVector4(spinBoxX->value(), spinBoxY->value(), spinBoxZ->value(), spinBoxW->value());
    }

    void setValue(const hkVector4 & value){
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

    virtual ~Vector4Widget(){}
signals:
    void editingDone();
private:
    QHBoxLayout *lyt;
    QDoubleSpinBox *spinBoxX;
    QDoubleSpinBox *spinBoxY;
    QDoubleSpinBox *spinBoxZ;
    QDoubleSpinBox *spinBoxW;
    QComboBox *boundVariable;
};

#endif // DATAWIDGETS_H
