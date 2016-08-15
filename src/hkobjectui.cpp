#include "hkobjectui.h"
#include "behaviorgraphicons.h"
#include "genericdatawidgets.h"
#include "generators.h"
#include "hkxfile.h"
#include "behaviorgraphui.h"

HkDataUI::HkDataUI(const QString &title, const QString &button1Name, const QString &button2Name)
    : behaviorView(NULL),
      verLyt(new QVBoxLayout),
      horLyt(new QHBoxLayout),
      button1(new QPushButton(button1Name)),
      button2(new QPushButton(button2Name)),
      stack(new QStackedLayout),
      iSTGUI(new BSiStateTaggingGeneratorUI)
{
    setTitle(title);
    horLyt->addWidget(button1);
    horLyt->addWidget(button2);
    verLyt->addLayout(horLyt, 1);
    verLyt->addLayout(stack, 5);
    button1->setMaximumSize(QSize(200, 50));
    button2->setMaximumSize(QSize(200, 50));
    setLayout(verLyt);
}

void HkDataUI::changeCurrentDataWidget(GeneratorIcon * icon){
    if (icon && icon->data.constData() && icon->data.constData()->getSignature() == BS_I_STATE_TAGGING_GENERATOR){
        if (icon->data != iSTGUI->bsData){
            iSTGUI->loadData(icon->data);
        }
        if (stack->count() == 0){
            stack->setCurrentIndex(stack->addWidget(iSTGUI));
        }
    }
}

void HkDataUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
    iSTGUI->behaviorView = view;
}

/*
 * BSiStateTaggingGeneratorUI
 */

BSiStateTaggingGeneratorUI::BSiStateTaggingGeneratorUI()
    : behaviorView(NULL),
      lyt(new QVBoxLayout),
      name(new StringWidget("Name")),
      pDefaultGenerator(new PointerWidget("pDefaultGenerator", "hkbGenerator")),
      iStateToSetAs(new IntWidget("iStateToSetAs")),
      iPriority(new IntWidget("iPriority"))
{
    setTitle("BSiStateTaggingGenerator");
    lyt->addWidget(name);
    lyt->addWidget(pDefaultGenerator);
    lyt->addWidget(iStateToSetAs);
    lyt->addWidget(iPriority);
    setLayout(lyt);
    connect(pDefaultGenerator, SIGNAL(editingDone(int)), this, SLOT(setDefaultGenerator(int)));
}

void BSiStateTaggingGeneratorUI::setDefaultGenerator(int index){
    BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(bsData.data());
    HkObjectExpSharedPtr smptr = ptr->pDefaultGenerator;
    //ptr->getParentFile()->setGeneratorData(ptr->pDefaultGenerator, index - 1);//PREVENT RECURSION!!!
    if (behaviorView){
        behaviorView->reconnectBranch(smptr, *ptr->getParentFile()->getGeneratorDataAt(index -1), behaviorView->getSelectedItem());
    }
    ptr->getParentFile()->setGeneratorData(ptr->pDefaultGenerator, index - 1);//PREVENT RECURSION!!!
}

void BSiStateTaggingGeneratorUI::loadData(const HkObjectExpSharedPtr & data){
    BSiStateTaggingGenerator *ptr = NULL;
    if (data.constData() && data.constData()->getSignature() == BS_I_STATE_TAGGING_GENERATOR){
        bsData = data;
        ptr = static_cast<BSiStateTaggingGenerator *>(bsData.data());
        name->setText(ptr->name);
        pDefaultGenerator->setStringList(ptr->getParentFile()->getGeneratorNames());
        pDefaultGenerator->setSelectedItem(ptr->getParentFile()->getIndexOfGenerator(ptr->pDefaultGenerator) + 1);
        iStateToSetAs->setValue(ptr->iStateToSetAs);
        iStateToSetAs->setBoundVariableList(ptr->getParentFile()->getVariableNames());
        iPriority->setValue(ptr->iPriority);
        iPriority->setBoundVariableList(ptr->getParentFile()->getVariableNames());
    }
}
