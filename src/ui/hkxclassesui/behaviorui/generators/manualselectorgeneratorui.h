#ifndef MANUALSELECTORGENERATORUI_H
#define MANUALSELECTORGENERATORUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class BehaviorGraphView;
class GenericTableWidget;
class hkbManualSelectorGenerator;
class TableWidget;
class SpinBox;
class LineEdit;
class ComboBox;
class QGridLayout;
class hkbVariableBindingSet;

class ManualSelectorGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    ManualSelectorGeneratorUI();
    virtual ~ManualSelectorGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
    void viewVariables(int index);
    void viewGenerators(int index);
    void viewProperties(int index);
private slots:
    void setName();
    void setSelectedGeneratorIndex();
    void setCurrentGeneratorIndex();
    void setBindingVariable(int index, const QString & name);
    void setGenerator(int index, const QString & name);
    void viewSelectedChild(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path);
    void variableRenamed(const QString & name, int index);
    void generatorRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void removeGenerator(int index);
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString &tip1, const QString &tip2);
    void connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties);
    void addGenerator();
    void loadDynamicTableRows();
private:
    enum Generator_Type {
        STATE_MACHINE = 0,
        MANUAL_SELECTOR_GENERATOR = 1,
        BLENDER_GENERATOR = 2,
        I_STATE_TAGGING_GENERATOR = 3,
        BONE_SWITCH_GENERATOR = 4,
        CYCLIC_BLEND_TRANSITION_GENERATOR = 5,
        SYNCHRONIZED_CLIP_GENERATOR = 6,
        MODIFIER_GENERATOR = 7,
        OFFSET_ANIMATION_GENERATOR = 8,
        POSE_MATCHING_GENERATOR = 9,
        CLIP_GENERATOR = 10,
        BEHAVIOR_REFERENCE_GENERATOR = 11
    };
    static QStringList types;
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    hkbManualSelectorGenerator *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    SpinBox *selectedGeneratorIndex;
    SpinBox *currentGeneratorIndex;
    ComboBox *typeSelectorCB;
};

#endif // MANUALSELECTORGENERATORUI_H
