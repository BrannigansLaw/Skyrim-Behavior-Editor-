#include "modifiergeneratorui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/generators/hkbmodifiergenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

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
    if (!bsData){
        return;
    }
    HkxObject *temp = bsData->modifier.data();
    if (!temp){
        return;
    }
    if (behaviorView){
        HkxObject *ptr = bsData->getParentFile()->getModifierDataAt(index - 1);
        if (index < 1 || !ptr || ptr == bsData || !behaviorView->reconnectBranch(temp, ptr, behaviorView->getSelectedItem())){
            QMessageBox msg;
            msg.setWindowTitle("Skyrim Behavior Tool");
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
            msg.exec();
            modifier->silence();
            modifier->setSelectedItem(modifier->getLastIndex());
            modifier->setLastIndex(modifier->currentIndex());
            modifier->reconnect();
            return;
        }
    }
    bsData->getParentFile()->setModifierData(bsData->modifier, index - 1);
    bsData->evaulateDataValidity();
    bsData->getParentFile()->removeData();
    modifier->silence();
    modifier->setStringList(bsData->getParentFile()->getModifierNames());
    modifier->setSelectedItem(bsData->getParentFile()->getIndexOfModifier(bsData->modifier) + 1);
    modifier->reconnect();
}

void ModifierGeneratorUI::setGenerator(int index){
    if (!bsData){
        return;
    }
    HkxObject *temp = bsData->generator.data();
    if (!temp){
        //return;
    }
    if (behaviorView){
        HkxObject *ptr = bsData->getParentFile()->getGeneratorDataAt(index - 1);
        if (index < 1 || !ptr || ptr == bsData || !behaviorView->reconnectBranch(temp, ptr, behaviorView->getSelectedItem())){
            QMessageBox msg;
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
            msg.exec();
            generator->silence();
            generator->setSelectedItem(generator->getLastIndex());
            generator->setLastIndex(generator->currentIndex());
            generator->reconnect();
            return;
        }
    }
    bsData->getParentFile()->setGeneratorData(bsData->generator, index - 1);
    bsData->evaulateDataValidity();
    bsData->getParentFile()->removeData();
    generator->silence();
    generator->setStringList(bsData->getParentFile()->getGeneratorNames());
    generator->setSelectedItem(bsData->getParentFile()->getIndexOfGenerator(bsData->generator) + 1);
    generator->reconnect();
}

void ModifierGeneratorUI::loadData(HkxObject *data){
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
