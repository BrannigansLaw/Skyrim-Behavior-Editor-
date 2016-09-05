#ifndef BSISTATETAGGINGGENERATORUI_H
#define BSISTATETAGGINGGENERATORUI_H

#include <QGroupBox>

class BehaviorGraphView;
class PointerWidget;
class StringWidget;
class IntWidget;
class BSiStateTaggingGenerator;
class QLabel;
class QVBoxLayout;
class HkxObject;

class BSiStateTaggingGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSiStateTaggingGeneratorUI();
    virtual ~BSiStateTaggingGeneratorUI(){}
    void loadData(HkxObject *data);
private slots:
    void setName();
    void setDefaultGenerator(int index);
    void setIStateToSetAs();
    void setIPriority();
private:
    BehaviorGraphView *behaviorView;
    BSiStateTaggingGenerator *bsData;
    QVBoxLayout *lyt;
    StringWidget *name;
    PointerWidget *pDefaultGenerator;
    IntWidget *iStateToSetAs;
    IntWidget *iPriority;
};

#endif // BSISTATETAGGINGGENERATORUI_H
