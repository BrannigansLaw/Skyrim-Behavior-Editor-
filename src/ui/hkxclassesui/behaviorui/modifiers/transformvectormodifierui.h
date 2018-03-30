#ifndef TRANSFORMVECTORMODIFIERUI_H
#define TRANSFORMVECTORMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbTransformVectorModifier;
class LineEdit;
class QuadVariableWidget;
class CheckBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class DoubleSpinBox;

class TransformVectorModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    TransformVectorModifierUI();
    virtual ~TransformVectorModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setRotation();
    void setTranslation();
    void setVectorIn();
    void setVectorOut();
    void setRotateOnly();
    void setInverse();
    void setComputeOnActivate();
    void setComputeOnModify();
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
    hkbTransformVectorModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *rotation;
    QuadVariableWidget *translation;
    QuadVariableWidget *vectorIn;
    QuadVariableWidget *vectorOut;
    CheckBox *rotateOnly;
    CheckBox *inverse;
    CheckBox *computeOnActivate;
    CheckBox *computeOnModify;
};

#endif // TRANSFORMVECTORMODIFIERUI_H
