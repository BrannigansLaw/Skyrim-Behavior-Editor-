#ifndef EVENTRANGEDATAUI_H
#define EVENTRANGEDATAUI_H

#include <QGroupBox>

#include "src/hkxclasses/behavior/hkbEventRangeDataArray.h"

class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class BehaviorFile;
class DoubleSpinBox;
class CheckBox;
class TableWidget;
class hkbVariableBindingSet;
class ComboBox;

class EventRangeDataUI: public QGroupBox
{
    Q_OBJECT
    friend class EventsFromRangeModifierUI;
public:
    EventRangeDataUI();
    void loadData(BehaviorFile *parentFile, hkbEventRangeDataArray::hkbEventRangeData *ranges, hkbEventRangeDataArray *par, int index);
signals:
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    //void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    //void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void returnToParent();
private slots:
    void setEventId(int index, const QString &name);
    void setEventPayload();
    void setUpperBound();
    void setEventMode(int index);
    void viewSelectedChild(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    //bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    //void setBindingVariable(int index, const QString & name);
    //void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path);
    //void selectTableToView(bool viewproperties, const QString & path);
    void eventRenamed(const QString & name, int index);
    //void variableRenamed(const QString & name, int index);
private:
    static QStringList headerLabels;
    BehaviorFile *file;
    hkbEventRangeDataArray::hkbEventRangeData *bsData;
    hkbEventRangeDataArray *parent;
    int rangeIndex;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    TableWidget *table;
    QLineEdit *payload;
    DoubleSpinBox *upperBound;
    ComboBox *eventMode;
};

#endif // EVENTRANGEDATAUI_H
