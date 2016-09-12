#include "hkdataui.h"

#include "src/ui/behaviorgraphui/behaviorgraphicons.h"
#include "src/ui/genericdatawidgets.h"
#include "src/hkxclasses/generators/hkbgenerator.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/hkxclasses/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/generators/hkbmodifiergenerator.h"
#include "src/ui/objectdataui/generators/bsistatetagginggeneratorui.h"
#include "src/ui/objectdataui/generators/modifiergeneratorui.h"
#include "src/ui/objectdataui/generators/manualselectorgeneratorui.h"
#include "src/ui/objectdataui/generators/statemachineui.h"

#include <QPushButton>
#include <QMessageBox>
#include <QStackedLayout>

HkDataUI::HkDataUI(const QString &title/*, const QString &button1Name, const QString &button2Name*/)
    : behaviorView(NULL),
      verLyt(new QVBoxLayout),
      //horLyt(new QHBoxLayout),
      //button1(new QPushButton(button1Name)),
      //button2(new QPushButton(button2Name)),
      stack(new QStackedLayout),
      loadedData(NULL),
      noDataL(new QLabel("No Data Selected!")),
      iSTGUI(new BSiStateTaggingGeneratorUI),
      modGenUI(new ModifierGeneratorUI),
      manSelGenUI(new ManualSelectorGeneratorUI),
      stateMachineUI(new StateMachineUI)
{
    setTitle(title);
    stack->addWidget(noDataL);
    stack->addWidget(iSTGUI);
    stack->addWidget(modGenUI);
    stack->addWidget(manSelGenUI);
    stack->addWidget(stateMachineUI);
    //setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //horLyt->addWidget(button1);
    //horLyt->addWidget(button2);
    //verLyt->addLayout(horLyt, 1);
    verLyt->addLayout(stack, 5);
    //button1->setMaximumSize(QSize(200, 50));
    //button2->setMaximumSize(QSize(200, 50));
    setLayout(verLyt);
}

void HkDataUI::changeCurrentDataWidget(GeneratorIcon * icon){
    qulonglong sig;
    if (icon && icon->data.constData()){
        sig = icon->data.constData()->getSignature();
        loadedData = icon->data.data();
        switch (sig) {
        case BS_I_STATE_TAGGING_GENERATOR:
            //if (loadedData != iSTGUI->bsData){
                iSTGUI->loadData(loadedData);
            //}
            stack->setCurrentIndex(BS_I_STATE_TAG_GEN);
            break;
        case HKB_MODIFIER_GENERATOR:
            //if (loadedData != modGenUI->bsData){
                modGenUI->loadData(loadedData);
            //}
            stack->setCurrentIndex(MOD_GEN);
            break;
        case HKB_MANUAL_SELECTOR_GENERATOR:
            //if (loadedData != modGenUI->bsData){
                manSelGenUI->loadData(loadedData);
            //}
            stack->setCurrentIndex(MSG);
            break;
        case HKB_STATE_MACHINE:
            //if (loadedData != modGenUI->bsData){
                stateMachineUI->loadData(loadedData);
            //}
            stack->setCurrentIndex(SM);
            break;
        default:
            stack->setCurrentIndex(NO_DATA_SELECTED);
            break;
        }
    }else{
        stack->setCurrentIndex(NO_DATA_SELECTED);
        loadedData = NULL;
    }
}

void HkDataUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
    iSTGUI->behaviorView = view;
    modGenUI->behaviorView = view;
    manSelGenUI->behaviorView = view;
    stateMachineUI->behaviorView = view;
}
