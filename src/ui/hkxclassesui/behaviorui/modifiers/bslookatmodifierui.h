#ifndef BSLOOKATMODIFIERUI_H
#define BSLOOKATMODIFIERUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class TableWidget;
class QGridLayout;
class SpinBox;
class LineEdit;
class ComboBox;
class BSLookAtModifier;
class BSBoneUI;
class CheckBox;
class QGroupBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class CheckButtonCombo;
class DoubleSpinBox;
class QuadVariableWidget;

class BSLookAtModifierUI final: public QStackedWidget
{
    Q_OBJECT
public:
    BSLookAtModifierUI();
    BSLookAtModifierUI& operator=(const BSLookAtModifierUI&) = delete;
    BSLookAtModifierUI(const BSLookAtModifierUI &) = delete;
    ~BSLookAtModifierUI() = default;
public:
    void loadData(HkxObject *data);
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events);
    void variableRenamed(const QString & name, int index);
    void eventRenamed(const QString & name, int index);
signals:
    void modifierNameChanged(const QString & newName, int index);
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
private slots:
    void setName(const QString &newname);
    void setEnable();
    void setLookAtTarget();
    void setLimitAngleDegrees();
    void setLimitAngleThresholdDegrees();
    void setContinueLookOutsideOfLimit();
    void setOnGain();
    void setOffGain();
    void setUseBoneGains();
    void setTargetLocation();
    void setTargetOutsideLimits();
    void setTargetOutOfLimitEventId(int index, const QString & name);
    void setTargetOutOfLimitEventPayload();
    void setLookAtCamera();
    void setLookAtCameraX();
    void setLookAtCameraY();
    void setLookAtCameraZ();
    void viewSelectedChild(int row, int column);
    void variableTableElementSelected(int index, const QString &name);
    void returnToWidget();
private:
    void toggleSignals(bool toggleconnections);
    void addBone();
    void addEyeBone();
    void loadDynamicTableRows();
    void setBindingVariable(int index, const QString & name);
    void removeBone(int index);
    void removeEyeBone(int index);
    void selectTableToView(bool properties, const QString & path);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET,
        BONE_UI_WIDGET,
    };
private:
    static const QStringList headerLabels;
    int eyeBoneButtonRow;
    QStringList boneList;
    BSLookAtModifier *bsData;
    QGroupBox *groupBox;
    BSBoneUI *boneUI;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    CheckBox *lookAtTarget;
    DoubleSpinBox *limitAngleDegrees;
    DoubleSpinBox *limitAngleThresholdDegrees;
    CheckBox *continueLookOutsideOfLimit;
    DoubleSpinBox *onGain;
    DoubleSpinBox *offGain;
    CheckBox *useBoneGains;
    QuadVariableWidget *targetLocation;
    CheckBox *targetOutsideLimits;
    LineEdit *targetOutOfLimitEventPayload;
    CheckBox *lookAtCamera;
    DoubleSpinBox *lookAtCameraX;
    DoubleSpinBox *lookAtCameraY;
    DoubleSpinBox *lookAtCameraZ;
};

#endif // BSLOOKATMODIFIERUI_H
