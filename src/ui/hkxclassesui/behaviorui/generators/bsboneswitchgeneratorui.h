#ifndef BSBONESWITCHGENERATORUI_H
#define BSBONESWITCHGENERATORUI_H

#include <QStackedWidget>

#include "src/utility.h"

class TableWidget;
class BSBoneSwitchGenerator;
class QGridLayout;
class HkxObject;
class DoubleSpinBox;
class QCheckBox;
class SpinBox;
class QPushButton;
class QGroupBox;
class BSBoneSwitchGeneratorBoneDataUI;
class LineEdit;
class BehaviorGraphView;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;

class BSBoneSwitchGeneratorUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSBoneSwitchGeneratorUI();
    virtual ~BSBoneSwitchGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
    void viewVariables(int index);
    void viewGenerators(int index);
    void viewProperties(int index);
private slots:
    void setName();
    void viewSelectedChild(int row, int column);
    void returnToWidget(bool reloadData);
    void variableTableElementSelected(int index, const QString &name);
    void generatorTableElementSelected(int index, const QString &name);
private:
    void connectSignals();
    void disconnectSignals();
    void setDefaultGenerator(int index, const QString & name);
    void addChildWithGenerator();
    void removeChild(int index);
    void loadDynamicTableRows();
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString &tip1, const QString &tip2);
    void connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties);
    void variableRenamed(const QString & name, int index);
    void generatorRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        CHILD_WIDGET = 1
    };
    enum Generator_Type {
        STATE_MACHINE,
        MANUAL_SELECTOR_GENERATOR,
        BLENDER_GENERATOR,
        I_STATE_TAGGING_GENERATOR,
        BONE_SWITCH_GENERATOR,
        CYCLIC_BLEND_TRANSITION_GENERATOR,
        SYNCHRONIZED_CLIP_GENERATOR,
        MODIFIER_GENERATOR,
        OFFSET_ANIMATION_GENERATOR,
        POSE_MATCHING_GENERATOR,
        CLIP_GENERATOR,
        BEHAVIOR_REFERENCE_GENERATOR
    };
    static QStringList types;
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    BSBoneSwitchGenerator *bsData;
    QGroupBox *groupBox;
    BSBoneSwitchGeneratorBoneDataUI *childUI;
    QGridLayout *topLyt;
    ComboBox *typeSelectorCB;
    TableWidget *table;
    LineEdit *name;
};

#endif // BSBONESWITCHGENERATORUI_H
