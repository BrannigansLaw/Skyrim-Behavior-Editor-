#ifndef COMBINETRANSFORMSMODIFIERUI_H
#define COMBINETRANSFORMSMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbCombineTransformsModifier;
class LineEdit;
class QuadVariableWidget;
class CheckBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class DoubleSpinBox;

class CombineTransformsModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    CombineTransformsModifierUI();
    virtual ~CombineTransformsModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index);
    void viewProperties(int index);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setTranslationOut();
    void setRotationOut();
    void setLeftTranslation();
    void setLeftRotation();
    void setRightTranslation();
    void setRightRotation();
    void setInvertLeftTransform();
    void setInvertRightTransform();
    void setInvertResult();
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
    hkbCombineTransformsModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *translationOut;
    QuadVariableWidget *rotationOut;
    QuadVariableWidget *leftTranslation;
    QuadVariableWidget *leftRotation;
    QuadVariableWidget *rightTranslation;
    QuadVariableWidget *rightRotation;
    CheckBox *invertLeftTransform;
    CheckBox *invertRightTransform;
    CheckBox *invertResult;
};

#endif // COMBINETRANSFORMSMODIFIERUI_H
