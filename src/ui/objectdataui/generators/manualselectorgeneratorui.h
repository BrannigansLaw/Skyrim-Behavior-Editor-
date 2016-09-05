#ifndef MANUALSELECTORGENERATORUI_H
#define MANUALSELECTORGENERATORUI_H

#include <QGroupBox>

class HkxObject;
class BehaviorGraphView;
class PointerWidget;
class StringWidget;
class IntWidget;
class GenericTabelWidget;
class hkbManualSelectorGenerator;
class QVBoxLayout;

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
    void addNewGenerator(int index);
    void addExistingGenerator(int index);
    void removeGenerator(int index);
    void setSelectedGeneratorIndex();
    void setCurrentGeneratorIndex();
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
    BehaviorGraphView *behaviorView;
    hkbManualSelectorGenerator *bsData;
    QVBoxLayout *lyt;
    StringWidget *name;
    GenericTabelWidget *generators;
    IntWidget *selectedGeneratorIndex;
    IntWidget *currentGeneratorIndex;
};

#endif // MANUALSELECTORGENERATORUI_H
