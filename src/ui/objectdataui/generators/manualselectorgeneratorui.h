#ifndef MANUALSELECTORGENERATORUI_H
#define MANUALSELECTORGENERATORUI_H

#include <QGroupBox>

class HkxObject;
class BehaviorGraphView;
class PointerWidget;
class StringWidget;
class IntWidget;
class GenericTableWidget;
class hkbManualSelectorGenerator;
class QVBoxLayout;
class TableWidget;
class QTableWidgetItem;
class SpinBox;
class QLineEdit;
class ComboBox;
class QPushButton;
class QHBoxLayout;
class QSignalMapper;

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
private slots:
    void setName();
    void addNewGenerator();
    void setGeneratorAt(int index);
    void removeGenerator();
    void setSelectedGeneratorIndex();
    void setSelectedGeneratorIndexBind(int index);
    void setCurrentGeneratorIndex();
    void setCurrentGeneratorIndexBind(int index);
private:
    //void addEventToLists(const QString & name);
    //void removeEventFromLists(int index);
    //void renameEventInLists(const QString & newName, int index);
    void addVariableToLists(const QString & name);
    void removeVariableFromLists(int index);
    void renameVariableInLists(const QString & newName, int index);
    void addGeneratorToLists(const QString & name);
    void removeGeneratorFromLists(int index);
    void renameGeneratorInLists(const QString & name, int index);
    //void addModifierToLists(const QString & name);
    //void removeModifierFromLists(int index);
    //void renameModifierInLists(const QString & name, int index);
    void loadComboBoxes();
private:
    enum Type {
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
    static QStringList headerLabels1;
    static QStringList headerLabels2;
    BehaviorGraphView *behaviorView;
    hkbManualSelectorGenerator *bsData;
    QVBoxLayout *lyt;
    TableWidget *table;
    QLineEdit *name;
    TableWidget *generators;
    QPushButton *addObjectPB;
    QPushButton *removeObjectPB;
    ComboBox *typeSelectorCB;
    QSignalMapper *signalMapper;
    QHBoxLayout *buttonLyt;
    SpinBox *selectedGeneratorIndex;
    ComboBox *selectedGeneratorIndexBind;
    SpinBox *currentGeneratorIndex;
    ComboBox *currentGeneratorIndexBind;
};

#endif // MANUALSELECTORGENERATORUI_H
