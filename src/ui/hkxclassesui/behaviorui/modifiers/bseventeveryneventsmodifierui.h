#ifndef BSEVENTEVERYNEVENTSMODIFIERUI_H
#define BSEVENTEVERYNEVENTSMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class BSEventEveryNEventsModifier;
class LineEdit;
class QLineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class SpinBox;

class BSEventEveryNEventsModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSEventEveryNEventsModifierUI();
    virtual ~BSEventEveryNEventsModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index);
    void viewProperties(int index);
    void viewEvents(int index);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setEventToCheckForPayload();
    void setEventToSendPayload();
    void setNumberOfEventsBeforeSend();
    void setMinimumNumberOfEventsBeforeSend();
    void setRandomizeNumberOfEvents();
    void eventTableElementSelected(int index, const QString &name);
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void setEventToCheckForId(int index, const QString & name);
    void setEventToSendId(int index, const QString & name);
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events);
    void variableRenamed(const QString & name, int index);
    void eventRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList headerLabels;
    BSEventEveryNEventsModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QLineEdit *eventToCheckForPayload;
    QLineEdit *eventToSendPayload;
    SpinBox *numberOfEventsBeforeSend;
    SpinBox *minimumNumberOfEventsBeforeSend;
    CheckBox *randomizeNumberOfEvents;
};

#endif // BSEVENTEVERYNEVENTSMODIFIERUI_H
