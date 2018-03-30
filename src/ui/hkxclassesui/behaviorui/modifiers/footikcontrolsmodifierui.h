#ifndef FOOTIKCONTROLSMODIFIERUI_H
#define FOOTIKCONTROLSMODIFIERUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbFootIkControlsModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class SpinBox;
class hkbVariableBindingSet;
class ExpressionDataArrayUI;
class QGroupBox;
class LegUI;
class QuadVariableWidget;

class FootIkControlsModifierUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    FootIkControlsModifierUI();
    virtual ~FootIkControlsModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setonOffGain();
    void setgroundAscendingGain();
    void setgroundDescendingGain();
    void setfootPlantedGain();
    void setfootRaisedGain();
    void setfootUnlockGain();
    void setworldFromModelFeedbackGain();
    void seterrorUpDownBias();
    void setalignWorldFromModelGain();
    void sethipOrientationGain();
    void setmaxKneeAngleDifference();
    void setankleOrientationGain();
    void seterrorOutTranslation();
    void setalignWithGroundRotation();
    void viewSelectedChild(int row, int column);
    void variableTableElementSelected(int index, const QString &name);
    void returnToWidget();
private:
    void connectSignals();
    void disconnectSignals();
    void setBindingVariable(int index, const QString & name);
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2);
    void addLeg();
    void removeLeg(int index);
    void loadDynamicTableRows();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events);
    void variableRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        CHILD_WIDGET = 1
    };
    static QStringList headerLabels;
    hkbFootIkControlsModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    QGroupBox *groupBox;
    LegUI *legUI;
    LineEdit *name;
    CheckBox *enable;
    DoubleSpinBox *onOffGain;
    DoubleSpinBox *groundAscendingGain;
    DoubleSpinBox *groundDescendingGain;
    DoubleSpinBox *footPlantedGain;
    DoubleSpinBox *footRaisedGain;
    DoubleSpinBox *footUnlockGain;
    DoubleSpinBox *worldFromModelFeedbackGain;
    DoubleSpinBox *errorUpDownBias;
    DoubleSpinBox *alignWorldFromModelGain;
    DoubleSpinBox *hipOrientationGain;
    DoubleSpinBox *maxKneeAngleDifference;
    DoubleSpinBox *ankleOrientationGain;
    QuadVariableWidget *errorOutTranslation;
    QuadVariableWidget *alignWithGroundRotation;
};

#endif // FOOTIKCONTROLSMODIFIERUI_H
