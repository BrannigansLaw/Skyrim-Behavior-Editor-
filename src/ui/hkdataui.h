#ifndef HKDATAUI_H
#define HKDATAUI_H

#include <QGroupBox>

class BehaviorGraphView;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QStackedLayout;
class HkxObject;
class QLabel;
class BSiStateTaggingGeneratorUI;
class ModifierGeneratorUI;
class ManualSelectorGeneratorUI;
class StateMachineUI;
class GeneratorIcon;

class HkDataUI: public QGroupBox
{
    Q_OBJECT
public:
    HkDataUI(const QString & title/*, const QString & button1Name = "Back", const QString & button2Name = "Forward"*/);
    virtual ~HkDataUI(){}
    void setBehaviorView(BehaviorGraphView *view);
public slots:
    void changeCurrentDataWidget(GeneratorIcon *icon);
private:
    HkDataUI& operator=(const HkDataUI&);
    HkDataUI(const HkDataUI &);
private:
    enum {NO_DATA_SELECTED = 0, BS_I_STATE_TAG_GEN = 1, MOD_GEN = 2, MSG = 3, SM = 4};
    BehaviorGraphView *behaviorView;
    QVBoxLayout *verLyt;
    //QHBoxLayout *horLyt;
    //QPushButton *button1;
    //QPushButton *button2;
    QStackedLayout *stack;
    HkxObject *loadedData;
    //QList <HkxObject *> viewedData;
    //All HkxObjectData UI's??
    QLabel *noDataL;
    BSiStateTaggingGeneratorUI *iSTGUI;
    ModifierGeneratorUI *modGenUI;
    ManualSelectorGeneratorUI *manSelGenUI;
    StateMachineUI *stateMachineUI;
};

#endif // HKDATAUI_H
