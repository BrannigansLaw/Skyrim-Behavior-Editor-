#ifndef HkxObjectUI_H
#define HkxObjectUI_H

#include <QGroupBox>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QPushButton>

class HkxObject;
class BehaviorGraphView;
class GeneratorIcon;
class PointerWidget;
class StringWidget;
class IntWidget;
class BSiStateTaggingGeneratorUI;
class ModifierGeneratorUI;
class BSiStateTaggingGenerator;
class hkbModifierGenerator;
class QLabel;

#include "src/hkxclasses/hkxobject.h"

class HkDataUI: public QGroupBox
{
    Q_OBJECT
public:
    HkDataUI(const QString & title, const QString & button1Name = "Back", const QString & button2Name = "Forward");
    virtual ~HkDataUI(){}
    void setBehaviorView(BehaviorGraphView *view);
public slots:
    void changeCurrentDataWidget(GeneratorIcon *icon);
private:
    HkDataUI& operator=(const HkDataUI&);
    HkDataUI(const HkDataUI &);
private:
    enum {NO_DATA_SELECTED = 0, BS_I_STATE_TAG_GEN = 1, MOD_GEN = 2};
    BehaviorGraphView *behaviorView;
    QVBoxLayout *verLyt;
    QHBoxLayout *horLyt;
    QPushButton *button1;
    QPushButton *button2;
    QStackedLayout *stack;
    HkxObject *loadedData;
    //QList <HkxObject *> viewedData;
    //All HkxObjectData UI's??
    QLabel *noDataL;
    BSiStateTaggingGeneratorUI *iSTGUI;
    ModifierGeneratorUI *modGenUI;
};

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

#endif // HkxObjectUI_H
