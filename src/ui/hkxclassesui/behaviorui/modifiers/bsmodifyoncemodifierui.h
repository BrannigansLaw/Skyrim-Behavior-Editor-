#ifndef BSMODIFYONCEMODIFIERUI_H
#define BSMODIFYONCEMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class BehaviorGraphView;
class BSModifyOnceModifier;
class QGridLayout;
class TableWidget;
class LineEdit;
class GenericTableWidget;
class CheckBox;
class hkbVariableBindingSet;

class BSModifyOnceModifierUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSModifyOnceModifierUI();
    virtual ~BSModifyOnceModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void modifierNameChanged(const QString & newName, int index);
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewModifiers(int index, const QString & typeallowed, const QStringList &typesdisallowed);
private slots:
    void setName();
    void setModifier(int index, const QString & name);
    void setBindingVariable(int index, const QString & name);
    void viewSelected(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    void variableRenamed(const QString & name, int index);
    void modifierRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path);
    void connectToTables(GenericTableWidget *modifiers, GenericTableWidget *variables, GenericTableWidget *properties);
    void setBehaviorView(BehaviorGraphView *view);
private:
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    BSModifyOnceModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
};

#endif // BSMODIFYONCEMODIFIERUI_H
