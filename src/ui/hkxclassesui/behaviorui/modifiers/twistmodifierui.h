#ifndef TWISTMODIFIERUI_H
#define TWISTMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbTwistModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;

class TwistModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    TwistModifierUI();
    virtual ~TwistModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setAxisOfRotation();
    void setTwistAngle();
    void setStartBoneIndex(int index);
    void setEndBoneIndex(int index);
    void setSetAngleMethod(int index);
    void setRotationAxisCoordinates(int index);
    void setIsAdditive();
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
    hkbTwistModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *axisOfRotation;
    DoubleSpinBox *twistAngle;
    ComboBox *startBoneIndex;
    ComboBox *endBoneIndex;
    ComboBox *setAngleMethod;
    ComboBox *rotationAxisCoordinates;
    CheckBox *isAdditive;
};

#endif // TWISTMODIFIERUI_H
