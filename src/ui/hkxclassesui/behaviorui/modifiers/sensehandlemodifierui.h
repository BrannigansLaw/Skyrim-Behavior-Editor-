#ifndef SENSEHANDLEMODIFIERUI_H
#define SENSEHANDLEMODIFIERUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbSenseHandleModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class SpinBox;
class hkbVariableBindingSet;
class ExpressionDataArrayUI;
class QGroupBox;
class RangesUI;
class QuadVariableWidget;

class SenseHandleModifierUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    SenseHandleModifierUI();
    virtual ~SenseHandleModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index);
    void viewProperties(int index);
    void viewEvents(int index);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setSensorLocalOffset();
    void setLocalFrameName(const QString & text);
    void setSensorLocalFrameName(const QString & text);
    void setMinDistance();
    void setMaxDistance();
    void setDistanceOut();
    void setCollisionFilterInfo(int index);
    void setSensorRagdollBoneIndex(int index);
    void setSensorAnimationBoneIndex(int index);
    void setSensingMode(int index);
    void setExtrapolateSensorPosition();
    void setKeepFirstSensedHandle();
    void setFoundHandleOut();
    void viewSelectedChild(int row, int column);
    void variableTableElementSelected(int index, const QString &name);
    void returnToWidget();
private:
    void connectSignals();
    void disconnectSignals();
    void setBindingVariable(int index, const QString & name);
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2);
    void addRange();
    void removeRange(int index);
    void eventRenamed(const QString & name, int index);
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
    static QStringList collisionLayers;
    static QStringList headerLabels;
    hkbSenseHandleModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    QGroupBox *groupBox;
    RangesUI *rangeUI;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *sensorLocalOffset;
    ComboBox *localFrameName;
    ComboBox *sensorLocalFrameName;
    DoubleSpinBox *minDistance;
    DoubleSpinBox *maxDistance;
    DoubleSpinBox *distanceOut;
    ComboBox *collisionFilterInfo;
    ComboBox *sensorRagdollBoneIndex;
    ComboBox *sensorAnimationBoneIndex;
    ComboBox *sensingMode;
    CheckBox *extrapolateSensorPosition;
    CheckBox *keepFirstSensedHandle;
    CheckBox *foundHandleOut;
};

#endif // SENSEHANDLEMODIFIERUI_H
