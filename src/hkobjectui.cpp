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
      loadedData(NULL),
      noDataL(new QLabel("No Data Selected!")),
      iSTGUI(new BSiStateTaggingGeneratorUI),
      modGenUI(new ModifierGeneratorUI)
{
    setTitle(title);
    stack->addWidget(noDataL);
    stack->addWidget(iSTGUI);
    stack->addWidget(modGenUI);
    horLyt->addWidget(button1);
    horLyt->addWidget(button2);
    verLyt->addLayout(horLyt, 1);
    verLyt->addLayout(stack, 5);
    button1->setMaximumSize(QSize(200, 50));
    button2->setMaximumSize(QSize(200, 50));
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
    connect(name, SIGNAL(editingDone()), this, SLOT(setName()));
    connect(pDefaultGenerator, SIGNAL(editingDone(int)), this, SLOT(setDefaultGenerator(int)));
    connect(iStateToSetAs, SIGNAL(editingDone()), this, SLOT(setIStateToSetAs()));
    connect(iPriority, SIGNAL(editingDone()), this, SLOT(setIPriority()));
}

void BSiStateTaggingGeneratorUI::setName(){
    if (bsData){
        bsData->name = name->getText();
    }
}

void BSiStateTaggingGeneratorUI::setDefaultGenerator(int index){
    if (index < 1){
        QMessageBox msg;
        msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a dead end!!!");
        msg.exec();
        pDefaultGenerator->silence();
        pDefaultGenerator->setSelectedItem(pDefaultGenerator->getLastIndex());
        pDefaultGenerator->reconnect();
        return;
    }
    if (!bsData){
        return;
    }
    HkObject *temp = bsData->pDefaultGenerator.data();
    if (!temp){
        return;
    }
    if (behaviorView){
        HkObjectExpSharedPtr ptr = *(bsData->getParentFile()->getGeneratorDataAt(index -1));
        if (!behaviorView->reconnectBranch(temp, ptr.data(), behaviorView->getSelectedItem())){
            QMessageBox msg;
            msg.setWindowTitle("Skyrim Behavior Tool");
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch!!!");
            msg.exec();
            return;
        }
    }
    bsData->getParentFile()->setGeneratorData(bsData->pDefaultGenerator, index - 1);
    bsData->getParentFile()->removeData();
    pDefaultGenerator->silence();
    pDefaultGenerator->setStringList(bsData->getParentFile()->getGeneratorNames());
    pDefaultGenerator->setSelectedItem(bsData->getParentFile()->getIndexOfGenerator(bsData->pDefaultGenerator) + 1);
    pDefaultGenerator->reconnect();
}

void BSiStateTaggingGeneratorUI::loadData(HkObject *data){
    if (data && data->getSignature() == BS_I_STATE_TAGGING_GENERATOR){
        bsData = static_cast<BSiStateTaggingGenerator *>(data);
        name->setText(bsData->name);
        pDefaultGenerator->silence();
        pDefaultGenerator->setStringList(bsData->getParentFile()->getGeneratorNames());
        int index = bsData->getParentFile()->getIndexOfGenerator(bsData->pDefaultGenerator) + 1;
        pDefaultGenerator->setLastIndex(index);
        pDefaultGenerator->setSelectedItem(index);
        pDefaultGenerator->reconnect();
        iStateToSetAs->setValue(bsData->iStateToSetAs);
        iStateToSetAs->setBoundVariableList(bsData->getParentFile()->getVariableNames());
        iPriority->setValue(bsData->iPriority);
        iPriority->setBoundVariableList(bsData->getParentFile()->getVariableNames());
    }
}

void BSiStateTaggingGeneratorUI::setIStateToSetAs(){
    if (bsData){
        bsData->iStateToSetAs = iStateToSetAs->getValue();
    }
}

void BSiStateTaggingGeneratorUI::setIPriority(){
    if (bsData){
        bsData->iPriority = iPriority->getValue();
    }
}
/*
 * ModifierGeneratorUI
 */

ModifierGeneratorUI::ModifierGeneratorUI()
    : behaviorView(NULL),
      lyt(new QVBoxLayout),
      name(new StringWidget("Name")),
      modifier(new PointerWidget("modifier", "hkbModifier")),
      generator(new PointerWidget("generator", "hkbGenerator"))
{
    setTitle("hkbModifierGenerator");
    lyt->addWidget(name);
    lyt->addWidget(modifier);
    lyt->addWidget(generator);
    setLayout(lyt);
    connect(modifier, SIGNAL(editingDone(int)), this, SLOT(setModifier(int)));
    connect(generator, SIGNAL(editingDone(int)), this, SLOT(setGenerator(int)));
}

void ModifierGeneratorUI::setName(){
    if (bsData){
        bsData->name = name->getText();
    }
}

void ModifierGeneratorUI::setModifier(int index){
    if (index < 1){
        QMessageBox msg;
        msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a dead end!!!");
        msg.exec();
        modifier->silence();
        modifier->setSelectedItem(modifier->getLastIndex());
        modifier->reconnect();
        return;
    }
    if (!bsData){
        return;
    }
    HkObject *temp = bsData->modifier.data();
    if (!temp){
        return;
    }
    if (behaviorView){
        HkObjectExpSharedPtr ptr = *(bsData->getParentFile()->getModifierDataAt(index -1));
        if (!behaviorView->reconnectBranch(temp, ptr.data(), behaviorView->getSelectedItem())){
            QMessageBox msg;
            msg.setWindowTitle("Skyrim Behavior Tool");
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch!!!");
            msg.exec();
            return;
        }
    }
    bsData->getParentFile()->setModifierData(bsData->modifier, index - 1);
    bsData->getParentFile()->removeData();
    modifier->silence();
    modifier->setStringList(bsData->getParentFile()->getModifierNames());
    modifier->setSelectedItem(bsData->getParentFile()->getIndexOfModifier(bsData->modifier) + 1);
    modifier->reconnect();
}

void ModifierGeneratorUI::setGenerator(int index){
    if (index < 1){
        QMessageBox msg;
        msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a dead end!!!");
        msg.exec();
        generator->silence();
        generator->setSelectedItem(generator->getLastIndex());
        generator->reconnect();
        return;
    }
    if (!bsData){
        return;
    }
    HkObject *temp = bsData->generator.data();
    if (!temp){
        //return;
    }
    if (behaviorView){
        HkObjectExpSharedPtr ptr = *(bsData->getParentFile()->getGeneratorDataAt(index -1));
        if (!behaviorView->reconnectBranch(temp, ptr.data(), behaviorView->getSelectedItem())){
            QMessageBox msg;
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch!!!");
            msg.exec();
            return;
        }
    }
    bsData->getParentFile()->setGeneratorData(bsData->generator, index - 1);
    bsData->getParentFile()->removeData();
    generator->silence();
    generator->setStringList(bsData->getParentFile()->getGeneratorNames());
    generator->setSelectedItem(bsData->getParentFile()->getIndexOfGenerator(bsData->generator) + 1);
    generator->reconnect();
}

void ModifierGeneratorUI::loadData(HkObject *data){
    if (data && data->getSignature() == HKB_MODIFIER_GENERATOR){
        bsData = static_cast<hkbModifierGenerator *>(data);
        name->setText(bsData->name);
        modifier->silence();
        modifier->setStringList(bsData->getParentFile()->getModifierNames());
        int index = bsData->getParentFile()->getIndexOfModifier(bsData->modifier) + 1;
        modifier->setLastIndex(index);
        modifier->setSelectedItem(index);
        modifier->reconnect();
        generator->silence();
        generator->setStringList(bsData->getParentFile()->getGeneratorNames());
        index = bsData->getParentFile()->getIndexOfGenerator(bsData->generator) + 1;
        generator->setLastIndex(index);
        generator->setSelectedItem(index);
        generator->reconnect();
    }
}
