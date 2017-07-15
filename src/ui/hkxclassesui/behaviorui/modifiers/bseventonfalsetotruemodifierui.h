#ifndef BSEVENTONFALSETOTRUEMODIFIERUI_H
#define BSEVENTONFALSETOTRUEMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class BSEventOnFalseToTrueModifier;
class LineEdit;
class QLineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class SpinBox;

class BSEventOnFalseToTrueModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSEventOnFalseToTrueModifierUI();
    virtual ~BSEventOnFalseToTrueModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index);
    void viewProperties(int index);
    void viewEvents(int index);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setEnableEvent1();
    void setVariableToTest1();
    void setEventToSend1Payload();
    void setEnableEvent2();
    void setVariableToTest2();
    void setEventToSend2Payload();
    void setEnableEvent3();
    void setVariableToTest3();
    void setEventToSend3Payload();
    void eventTableElementSelected(int index, const QString &name);
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void setEventToSend1Id(int index, const QString & name);
    void setEventToSend2Id(int index, const QString & name);
    void setEventToSend3Id(int index, const QString & name);
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events);
    void variableRenamed(const QString & name, int index);
    void eventRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList headerLabels;
    BSEventOnFalseToTrueModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    CheckBox *bEnableEvent1;
    CheckBox *bVariableToTest1;
    QLineEdit *eventToSend1Payload;
    CheckBox *bEnableEvent2;
    CheckBox *bVariableToTest2;
    QLineEdit *eventToSend2Payload;
    CheckBox *bEnableEvent3;
    CheckBox *bVariableToTest3;
    QLineEdit *eventToSend3Payload;
};

#endif // BSEVENTONFALSETOTRUEMODIFIERUI_H
