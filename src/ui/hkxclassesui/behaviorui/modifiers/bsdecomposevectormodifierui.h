#ifndef BSDECOMPOSEVECTORMODIFIERUI_H
#define BSDECOMPOSEVECTORMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class BSDecomposeVectorModifier;
class LineEdit;
class QuadVariableWidget;
class CheckBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class DoubleSpinBox;

class BSDecomposeVectorModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSDecomposeVectorModifierUI();
    virtual ~BSDecomposeVectorModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setVector();
    void setX();
    void setY();
    void setZ();
    void setW();
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties);
    void variableRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList headerLabels;
    BSDecomposeVectorModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *vector;
    DoubleSpinBox *x;
    DoubleSpinBox *y;
    DoubleSpinBox *z;
    DoubleSpinBox *w;
};

#endif // BSDECOMPOSEVECTORMODIFIERUI_H
