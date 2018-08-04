#ifndef GETWORLDFROMMODELMODIFIERUI_H
#define GETWORLDFROMMODELMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbGetWorldFromModelModifier;
class LineEdit;
class QuadVariableWidget;
class CheckBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class DoubleSpinBox;
class QuadVariableWidget;

class GetWorldFromModelModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    GetWorldFromModelModifierUI();
    virtual ~GetWorldFromModelModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setTranslationOut();
    void setRotationOut();
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
    hkbGetWorldFromModelModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *translationOut;
    QuadVariableWidget *rotationOut;
};

#endif // GETWORLDFROMMODELMODIFIERUI_H
