#ifndef MODIFIERLISTUI_H
#define MODIFIERLISTUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class BehaviorGraphView;
class GenericTableWidget;
class hkbModifierList;
class TableWidget;
class SpinBox;
class ComboBox;
class LineEdit;
class CheckBox;
class QGridLayout;
class hkbVariableBindingSet;

class ModifierListUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    ModifierListUI();
    virtual ~ModifierListUI(){}
    void loadData(HkxObject *data);
signals:
    void modifierNameChanged(const QString & newName, int index);
    void viewVariables(int index);
    void viewProperties(int index);
    void viewModifiers(int index);
private slots:
    void setName();
    void setEnable();
    void setModifier(int index, const QString & name);
    void setBindingVariable(int index, const QString & name);
    void viewSelectedChild(int row, int column);
    void swapGeneratorIndices(int index1, int index2);
private:
    void connectSignals();
    void disconnectSignals();
    void modifierRenamed(const QString & name, int index);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path);
    void variableRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void removeModifier(int index);
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString &tip1, const QString &tip2);
    void connectToTables(GenericTableWidget *modifiers, GenericTableWidget *variables, GenericTableWidget *properties);
    void addModifier();
    void loadDynamicTableRows();
private:
    static QStringList types;
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    hkbModifierList *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    ComboBox *typeSelectorCB;
};

#endif // MODIFIERLISTUI_H
