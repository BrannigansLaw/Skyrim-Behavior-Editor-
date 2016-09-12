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
class QTableWidget;
class QTableWidgetItem;
class QSpinBox;
class QLineEdit;
class QComboBox;
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
private slots:
    void setName();
    void addNewGenerator();
    void addExistingGenerator(int index);
    void removeGenerator();
    void setSelectedGeneratorIndex(int value);
    void setSelectedGeneratorIndexBoundVariable(int index);
    void setCurrentGeneratorIndex(int value);
    void setCurrentGeneratorIndexBoundVariable(int index);
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

    static QStringList list;
    static QStringList headerLabels1;
    static QStringList headerLabels2;
    BehaviorGraphView *behaviorView;
    hkbManualSelectorGenerator *bsData;
    QVBoxLayout *lyt;
    QTableWidget *table;
    QLineEdit *name;
    QTableWidget *generators;
    QPushButton *addObjectPB;
    QPushButton *removeObjectPB;
    QComboBox *typeSelectorCB;
    QSignalMapper *signalMapper;
    QHBoxLayout *buttonLyt;
    QSpinBox *selectedGeneratorIndex;
    QComboBox *selectedGeneratorIndexBind;
    QSpinBox *currentGeneratorIndex;
    QComboBox *currentGeneratorIndexBind;
};

#endif // MANUALSELECTORGENERATORUI_H
