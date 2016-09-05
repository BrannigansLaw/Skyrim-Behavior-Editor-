#ifndef MODIFIERGENERATORUI_H
#define MODIFIERGENERATORUI_H

#include <QGroupBox>

class HkxObject;
class BehaviorGraphView;
class PointerWidget;
class StringWidget;
class IntWidget;
class hkbModifierGenerator;
class QVBoxLayout;

class ModifierGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    ModifierGeneratorUI();
    virtual ~ModifierGeneratorUI(){}
    void loadData(HkxObject *data);
private slots:
    void setName();
    void setModifier(int index);
    void setGenerator(int index);
private:
    BehaviorGraphView *behaviorView;
    hkbModifierGenerator *bsData;
    QVBoxLayout *lyt;
    StringWidget *name;
    PointerWidget *modifier;
    PointerWidget *generator;
};

#endif // MODIFIERGENERATORUI_H
