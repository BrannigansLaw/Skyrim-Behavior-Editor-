#ifndef HKOBJECTUI_H
#define HKOBJECTUI_H

#include <QGroupBox>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QPushButton>

class HkObject;
class BehaviorGraphView;
class GeneratorIcon;
class PointerWidget;
class StringWidget;
class IntWidget;
class BSiStateTaggingGeneratorUI;

#include "hkobject.h"

class HkDataUI: public QGroupBox
{
    Q_OBJECT
public:
    HkDataUI(const QString & title, const QString & button1Name = "Back", const QString & button2Name = "Forward");
    virtual ~HkDataUI(){}
    void setBehaviorView(BehaviorGraphView *view);
private slots:
    void changeCurrentDataWidget(GeneratorIcon *icon);
private:
    HkDataUI& operator=(const HkDataUI&);
    HkDataUI(const HkDataUI &);
private:
    BehaviorGraphView *behaviorView;
    QVBoxLayout *verLyt;
    QHBoxLayout *horLyt;
    QPushButton *button1;
    QPushButton *button2;
    QStackedLayout *stack;
    QList <HkObject *> viewedData;
    //All hkObjectData UI's??
    BSiStateTaggingGeneratorUI *iSTGUI;
};

class BSiStateTaggingGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSiStateTaggingGeneratorUI();
    virtual ~BSiStateTaggingGeneratorUI(){}
    void loadData(const HkObjectExpSharedPtr & data);
private slots:
    void setDefaultGenerator(int index);
private:
    BehaviorGraphView *behaviorView;
    HkObjectExpSharedPtr bsData;
    QVBoxLayout *lyt;
    StringWidget *name;
    PointerWidget *pDefaultGenerator;
    IntWidget *iStateToSetAs;
    IntWidget *iPriority;
};

#endif // HKOBJECTUI_H
