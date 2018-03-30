#ifndef BSISACTIVEMODIFIERUI_H
#define BSISACTIVEMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class BSIsActiveModifier;
class LineEdit;
class QuadVariableWidget;
class CheckBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class DoubleSpinBox;

class BSIsActiveModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSIsActiveModifierUI();
    virtual ~BSIsActiveModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setIsActive0();
    void setInvertActive0();
    void setIsActive1();
    void setInvertActive1();
    void setIsActive2();
    void setInvertActive2();
    void setIsActive3();
    void setInvertActive3();
    void setIsActive4();
    void setInvertActive4();
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties);
    void variableRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList headerLabels;
    BSIsActiveModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    CheckBox *bIsActive0;
    CheckBox *bInvertActive0;
    CheckBox *bIsActive1;
    CheckBox *bInvertActive1;
    CheckBox *bIsActive2;
    CheckBox *bInvertActive2;
    CheckBox *bIsActive3;
    CheckBox *bInvertActive3;
    CheckBox *bIsActive4;
    CheckBox *bInvertActive4;
};

#endif // BSISACTIVEMODIFIERUI_H
