#include "skyrimanimationdataui.h"

#include "src/ui/genericdatawidgets.h"
#include "src/ui/animationrotationui.h"
#include "src/ui/animationtranslationui.h"
#include "src/ui/mainwindow.h"

#include "src/animData/skyrimanimationmotiondata.h"

#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 4

#define NAME_ROW 0
#define DURATION_ROW 1
#define ADD_TRANSLATION_ROW 2
#define INITIAL_ROTATION_ROW 3

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList SkyrimAnimationDataUI::headerLabels = {
    "Name",
    "Type",
    "",
    "Value"
};

SkyrimAnimationDataUI::SkyrimAnimationDataUI()
    : bsData(NULL),
      rotationsButtonRow(INITIAL_ROTATION_ROW),
      groupBox(new QGroupBox("AnimationData")),
      topLyt(new QGridLayout),
      rotationUI(new AnimationRotationUI()),
      translationUI(new AnimationTranslationUI()),
      table(new TableWidget(QColor(Qt::white))),
      returnPB(new QPushButton("Return")),
      name(new LineEdit),
      duration(new DoubleSpinBox)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(DURATION_ROW, NAME_COLUMN, new TableWidgetItem("duration"));
    table->setItem(DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DURATION_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(DURATION_ROW, VALUE_COLUMN, duration);
    table->setItem(ADD_TRANSLATION_ROW, NAME_COLUMN, new TableWidgetItem("Add Translation", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a Translation"));
    table->setItem(ADD_TRANSLATION_ROW, TYPE_COLUMN, new TableWidgetItem("Translation", Qt::AlignCenter, QColor(Qt::gray)));
    table->setItem(ADD_TRANSLATION_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Translation", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    table->setItem(ADD_TRANSLATION_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Translation", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    table->setItem(INITIAL_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("Add Rotation", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a Rotation"));
    table->setItem(INITIAL_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("Rotation", Qt::AlignCenter, QColor(Qt::gray)));
    table->setItem(INITIAL_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Rotation", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    table->setItem(INITIAL_ROTATION_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Rotation", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(translationUI);
    addWidget(rotationUI);
    connectSignals();
}

void SkyrimAnimationDataUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(duration, SIGNAL(editingFinished()), this, SLOT(setDuration()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(translationUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(rotationUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
}

void SkyrimAnimationDataUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(duration, SIGNAL(editingFinished()), this, SLOT(setDuration()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(translationUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(rotationUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
}

void SkyrimAnimationDataUI::loadData(SkyrimAnimationMotionData *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    if (data){
        bsData = data;
        duration->setValue(bsData->duration);
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::loadData(): The data is NULL or an incorrect type!!"));
    }
    connectSignals();
}

void SkyrimAnimationDataUI::setName()
{
    //
}

void SkyrimAnimationDataUI::loadDynamicTableRows(){
    if (bsData){
        int temp = ADD_TRANSLATION_ROW + bsData->translations.size() + 1 - rotationsButtonRow;
        if (temp > 0){
            for (int i = 0; i < temp; i++){
                table->insertRow(rotationsButtonRow);
                rotationsButtonRow++;
            }
        }else if (temp < 0){
            for (int i = temp; i < 0; i++){
                table->removeRow(rotationsButtonRow - 1);
                rotationsButtonRow--;
            }
        }
        rotationsButtonRow = ADD_TRANSLATION_ROW + bsData->translations.size() + 1;
        for (int i = INITIAL_ROTATION_ROW, j = 0; i < rotationsButtonRow, j < bsData->translations.size(); i++, j++){
            setRowItems(i, "Translation "+QString::number(j), "hkVector3", "Remove", "Edit", "Double click to remove this translation", "Double click to edit this translation");
        }
        table->setRowCount(rotationsButtonRow + bsData->rotations.size() + 1);
        for (int i = rotationsButtonRow + 1, j = 0; i < table->rowCount(), j < bsData->rotations.size(); i++, j++){
            setRowItems(i, "Rotation "+QString::number(j), "hkQuaternion", "Remove", "Edit", "Double click to remove this quaternion", "Double click to edit this quaternion");
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::loadDynamicTableRows(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
    if (table->item(row, NAME_COLUMN)){
        table->item(row, NAME_COLUMN)->setText(name);
    }else{
        table->setItem(row, NAME_COLUMN, new TableWidgetItem(name));
    }
    if (table->item(row, TYPE_COLUMN)){
        table->item(row, TYPE_COLUMN)->setText(classname);
    }else{
        table->setItem(row, TYPE_COLUMN, new TableWidgetItem(classname, Qt::AlignCenter));
    }
    if (table->item(row, BINDING_COLUMN)){
        table->item(row, BINDING_COLUMN)->setText(bind);
    }else{
        table->setItem(row, BINDING_COLUMN, new TableWidgetItem(bind, Qt::AlignCenter, QColor(Qt::red), QBrush(Qt::black), tip1));
    }
    if (table->item(row, VALUE_COLUMN)){
        table->item(row, VALUE_COLUMN)->setText(value);
    }else{
        table->setItem(row, VALUE_COLUMN, new TableWidgetItem(value, Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), tip2));
    }
}

void SkyrimAnimationDataUI::setDuration(){
    if (bsData){
        if (bsData->duration != duration->value()){
            bsData->duration = duration->value();
            //bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::setduration(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::addTranslation(){
    if (bsData){
        bsData->addTranslation();
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::addEnterEvent(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::removeTranslation(int index){
    if (bsData){
        bsData->removeTranslation(index);
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::removeEnterEvent(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::addRotation(){
    if (bsData){
        bsData->addRotation();
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::addExitEvent(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::removeRotation(int index){
    if (bsData){
        bsData->removeRotation(index);
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::removeExitEvent(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::viewSelectedChild(int row, int column){
    int result;
    if (bsData){
        if (row == ADD_TRANSLATION_ROW && column == NAME_COLUMN){
            addTranslation();
        }else if (row == rotationsButtonRow && column == NAME_COLUMN){
            addRotation();
        }else if (row > ADD_TRANSLATION_ROW && row < rotationsButtonRow){
            result = row - ADD_TRANSLATION_ROW - 1;
            if (bsData->translations.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    translationUI->loadData(&bsData->translations[result], bsData->duration);
                    setCurrentIndex(TRANSLATION_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the enter translation \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeTranslation(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }else if (row > rotationsButtonRow && row < table->rowCount()){
            result = row - BASE_NUMBER_OF_ROWS - bsData->translations.size();
            if (result < bsData->rotations.size() && result >= 0){
                if (column == VALUE_COLUMN){
                    rotationUI->loadData(&bsData->rotations[result], bsData->duration);
                    setCurrentIndex(ROTATION_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the rotation \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeRotation(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}
