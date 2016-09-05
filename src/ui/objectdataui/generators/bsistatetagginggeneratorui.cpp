#include "bsistatetagginggeneratorui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/generators/bsistatetagginggenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

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
    if (!bsData){
        return;
    }
    HkxObject *temp = bsData->pDefaultGenerator.data();
    if (!temp){
        return;
    }
    if (behaviorView){
        HkxObject *ptr = bsData->getParentFile()->getGeneratorDataAt(index - 1);
        if (index < 1 || !ptr || ptr == bsData || !behaviorView->reconnectBranch(temp, ptr, behaviorView->getSelectedItem())){
            QMessageBox msg;
            msg.setWindowTitle("Skyrim Behavior Tool");
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
            msg.exec();
            pDefaultGenerator->silence();
            pDefaultGenerator->setSelectedItem(pDefaultGenerator->getLastIndex());
            pDefaultGenerator->setLastIndex(pDefaultGenerator->currentIndex());
            pDefaultGenerator->reconnect();
            return;
        }
    }
    bsData->getParentFile()->setGeneratorData(bsData->pDefaultGenerator, index - 1);
    bsData->evaulateDataValidity();
    bsData->getParentFile()->removeData();
    pDefaultGenerator->silence();
    pDefaultGenerator->setStringList(bsData->getParentFile()->getGeneratorNames());
    pDefaultGenerator->setSelectedItem(bsData->getParentFile()->getIndexOfGenerator(bsData->pDefaultGenerator) + 1);
    pDefaultGenerator->reconnect();
}

void BSiStateTaggingGeneratorUI::loadData(HkxObject *data){
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
