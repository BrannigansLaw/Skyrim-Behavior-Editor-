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
    //Need to null smptr in ui widget when data view changes to ensure proper behavior data removal...
    if (icon && icon->data.constData()){
        sig = icon->data.constData()->getSignature();
        loadedData = icon->data.data();
        switch (sig) {
        case BS_I_STATE_TAGGING_GENERATOR:
            if (icon->data != iSTGUI->bsData){
                iSTGUI->loadData(icon->data);
            }
            stack->setCurrentIndex(BS_I_STATE_TAG_GEN);
            break;
        case HKB_MODIFIER_GENERATOR:
            if (icon->data != modGenUI->bsData){
                modGenUI->loadData(icon->data);
            }
            stack->setCurrentIndex(MOD_GEN);
            break;
        default:
            stack->setCurrentIndex(NO_DATA_SELECTED);
            break;
        }
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
    BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(bsData.data());
    if (ptr){
        ptr->name = name->getText();
    }
}

void BSiStateTaggingGeneratorUI::setDefaultGenerator(int index){
    BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(bsData.data());
    {
        //Done to ensure smptr goes out of scope before removing the data...
        HkObjectExpSharedPtr smptr = ptr->pDefaultGenerator;
        //ptr->getParentFile()->setGeneratorData(ptr->pDefaultGenerator, index - 1);//PREVENT RECURSION!!!
        if (behaviorView){
            if (!behaviorView->reconnectBranch(smptr, *ptr->getParentFile()->getGeneratorDataAt(index -1), behaviorView->getSelectedItem())){
                QMessageBox msg;
                msg.setWindowTitle("Skyrim Behavior Tool");
                msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch!!!");
                msg.exec();
                return;
            }
        }
        ptr->getParentFile()->setGeneratorData(ptr->pDefaultGenerator, index - 1);//PREVENT RECURSION!!!
    }
    ptr->getParentFile()->removeData();
    pDefaultGenerator->silence();
    pDefaultGenerator->setStringList(ptr->getParentFile()->getGeneratorNames());
    pDefaultGenerator->setSelectedItem(ptr->getParentFile()->getIndexOfGenerator(ptr->pDefaultGenerator) + 1);
    pDefaultGenerator->reconnect();
}

void BSiStateTaggingGeneratorUI::loadData(const HkObjectExpSharedPtr & data){
    BSiStateTaggingGenerator *ptr = NULL;
    if (data.constData() && data.constData()->getSignature() == BS_I_STATE_TAGGING_GENERATOR){
        bsData = data;
        ptr = static_cast<BSiStateTaggingGenerator *>(bsData.data());
        name->setText(ptr->name);
        pDefaultGenerator->silence();
        pDefaultGenerator->setStringList(ptr->getParentFile()->getGeneratorNames());
        pDefaultGenerator->setSelectedItem(ptr->getParentFile()->getIndexOfGenerator(ptr->pDefaultGenerator) + 1);
        pDefaultGenerator->reconnect();
        iStateToSetAs->setValue(ptr->iStateToSetAs);
        iStateToSetAs->setBoundVariableList(ptr->getParentFile()->getVariableNames());
        iPriority->setValue(ptr->iPriority);
        iPriority->setBoundVariableList(ptr->getParentFile()->getVariableNames());
    }
}

void BSiStateTaggingGeneratorUI::setIStateToSetAs(){
    BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(bsData.data());
    if (ptr){
        ptr->iStateToSetAs = iStateToSetAs->getValue();
    }
}

void BSiStateTaggingGeneratorUI::setIPriority(){
    BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(bsData.data());
    if (ptr){
        ptr->iPriority = iPriority->getValue();
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
    hkbModifierGenerator *ptr = static_cast<hkbModifierGenerator *>(bsData.data());
    if (ptr){
        ptr->name = name->getText();
    }
}

void ModifierGeneratorUI::setModifier(int index){
    hkbModifierGenerator *ptr = static_cast<hkbModifierGenerator *>(bsData.data());
    {
        //Done to ensure smptr goes out of scope before removing the data...
        HkObjectExpSharedPtr smptr = ptr->modifier;
        if (behaviorView){
            if (!behaviorView->reconnectBranch(smptr, *ptr->getParentFile()->getModifierDataAt(index -1), behaviorView->getSelectedItem())){
                QMessageBox msg;
                msg.setWindowTitle("Skyrim Behavior Tool");
                msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch!!!");
                msg.exec();
                return;
            }
        }
        ptr->getParentFile()->setModifierData(ptr->modifier, index - 1);//PREVENT RECURSION!!!
    }
    ptr->getParentFile()->removeData();
    modifier->silence();
    modifier->setStringList(ptr->getParentFile()->getModifierNames());
    modifier->setSelectedItem(ptr->getParentFile()->getIndexOfModifier(ptr->modifier) + 1);
    modifier->reconnect();
}

void ModifierGeneratorUI::setGenerator(int index){
    hkbModifierGenerator *ptr = static_cast<hkbModifierGenerator *>(bsData.data());
    {
        //Done to ensure smptr goes out of scope before removing the data...
        HkObjectExpSharedPtr smptr = ptr->generator;
        if (behaviorView){
            if (!behaviorView->reconnectBranch(smptr, *ptr->getParentFile()->getGeneratorDataAt(index -1), behaviorView->getSelectedItem())){
                QMessageBox msg;
                msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch!!!");
                msg.exec();
                return;
            }
        }
        ptr->getParentFile()->setGeneratorData(ptr->generator, index - 1);//PREVENT RECURSION!!!
    }
    ptr->getParentFile()->removeData();
    generator->silence();
    generator->setStringList(ptr->getParentFile()->getGeneratorNames());
    generator->setSelectedItem(ptr->getParentFile()->getIndexOfGenerator(ptr->generator) + 1);
    generator->reconnect();
}

void ModifierGeneratorUI::loadData(const HkObjectExpSharedPtr & data){
    hkbModifierGenerator *ptr = NULL;
    if (data.constData() && data.constData()->getSignature() == HKB_MODIFIER_GENERATOR){
        bsData = data;
        ptr = static_cast<hkbModifierGenerator *>(bsData.data());
        name->setText(ptr->name);
        modifier->silence();
        modifier->setStringList(ptr->getParentFile()->getModifierNames());
        modifier->setSelectedItem(ptr->getParentFile()->getIndexOfModifier(ptr->modifier) + 1);
        modifier->reconnect();
        generator->silence();
        generator->setStringList(ptr->getParentFile()->getGeneratorNames());
        generator->setSelectedItem(ptr->getParentFile()->getIndexOfGenerator(ptr->generator) + 1);
        generator->reconnect();
    }
}
