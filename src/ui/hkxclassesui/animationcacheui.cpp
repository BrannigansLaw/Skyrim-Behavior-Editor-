#include "animationcacheui.h"

#include <QVBoxLayout>
#include <QFormLayout>

#include "src/utility.h"
#include "src/filetypes/projectfile.h"
#include "src/ui/genericdatawidgets.h"
#include "src/animSetData/animcachevariable.h"
#include "src/animSetData/animcacheclipinfo.h"
#include "src/animSetData/animcacheanimationinfo.h"
#include "src/animSetData/animcacheanimsetdata.h"

CacheVariableWidget::CacheVariableWidget()
    : bsData(nullptr),
      lyt(new QVBoxLayout),
      nameLyt(new QFormLayout),
      minValueLyt(new QFormLayout),
      maxValueLyt(new QFormLayout),
      name(new LineEdit),
      minValue(new SpinBox),
      maxValue(new SpinBox),
      returnButton(new QPushButton("Return"))
{
    setTitle("Behavior Variable Cache Data");
    setLayout(lyt);
    lyt->addWidget(returnButton);
    lyt->addLayout(nameLyt);
    lyt->addLayout(minValueLyt);
    lyt->addLayout(maxValueLyt);
    nameLyt->setHorizontalSpacing(2);
    minValueLyt->setHorizontalSpacing(2);
    maxValueLyt->setHorizontalSpacing(2);
    nameLyt->addRow("Variable Name: ", name);
    minValueLyt->addRow("Minimum Value: ", minValue);
    maxValueLyt->addRow("Maximum Value: ", maxValue);
    connectSignals();
}

void CacheVariableWidget::loadData(AnimCacheVariable *data){
    if (data){
        bsData = data;
        disconnectSignals();
        name->setText(bsData->name);
        minValue->setValue(bsData->minValue);
        maxValue->setValue(bsData->maxValue);
        connectSignals();
    }else{
        FATAL_RUNTIME_ERROR("CacheVariableWidget::loadData(): The data is nullptr!!");
    }
}

void CacheVariableWidget::setName(const QString & newname){
    if (bsData){
        bsData->name = newname;
    }else{
        FATAL_RUNTIME_ERROR("CacheVariableWidget::setName(): The data is nullptr!!");
    }
}

void CacheVariableWidget::setMinimumValue(int newvalue){
    if (bsData){
        bsData->minValue = newvalue;
    }else{
        FATAL_RUNTIME_ERROR("CacheVariableWidget::setMinimumValue(): The data is nullptr!!");
    }
}

void CacheVariableWidget::setMaximumValue(int newvalue){
    if (bsData){
        bsData->maxValue = newvalue;
    }else{
        FATAL_RUNTIME_ERROR("CacheVariableWidget::setMaximumValue(): The data is nullptr!!");
    }
}

void CacheVariableWidget::connectSignals(){
    connect(name, SIGNAL(textChanged(QString)), this, SLOT(setName(QString)), Qt::UniqueConnection);
    connect(minValue, SIGNAL(valueChanged(int)), this, SLOT(setMinimumValue(int)), Qt::UniqueConnection);
    connect(maxValue, SIGNAL(valueChanged(int)), this, SLOT(setMaximumValue(int)), Qt::UniqueConnection);
    connect(returnButton, SIGNAL(released()), this, SIGNAL(returnToWidget()), Qt::UniqueConnection);
}

void CacheVariableWidget::disconnectSignals(){
    disconnect(name, SIGNAL(textChanged(QString)), this, SLOT(setName(QString)));
    disconnect(minValue, SIGNAL(valueChanged(int)), this, SLOT(setMinimumValue(int)));
    disconnect(maxValue, SIGNAL(valueChanged(int)), this, SLOT(setMaximumValue(int)));
    disconnect(returnButton, SIGNAL(released()), this, SIGNAL(returnToWidget()));
}

////////////////////////////////////////////////////////////////////////////////////////////////


CacheClipWidget::CacheClipWidget()
    : bsData(nullptr),
      lyt(new QVBoxLayout),
      eventLyt(new QFormLayout),
      unknownLyt(new QFormLayout),
      eventName(new LineEdit),
      unknown(new SpinBox),
      buttonLyt(new QHBoxLayout),
      addClipButton(new QPushButton("Add Clip Generator")),
      removeClipButton(new QPushButton("Remove Selected Clip Generator")),
      clipGenerators(new TableWidget),
      returnButton(new QPushButton("Return"))
{
    setTitle("Clip Generator Cache Data");
    setLayout(lyt);
    lyt->addWidget(returnButton);
    lyt->addLayout(eventLyt);
    lyt->addLayout(unknownLyt);
    lyt->addLayout(buttonLyt);
    lyt->addWidget(clipGenerators);
    eventLyt->setHorizontalSpacing(2);
    unknownLyt->setHorizontalSpacing(2);
    QStringList list = {"Clip Generator Names"};
    clipGenerators->setColumnCount(list.size());
    clipGenerators->setHorizontalHeaderLabels(list);
    clipGenerators->setEditTriggers(QAbstractItemView::DoubleClicked);
    clipGenerators->setToolTip("Double Click To Edit");
    eventLyt->addRow("Event Name: ", eventName);
    unknownLyt->addRow("Unknown Variable: ", unknown);
    buttonLyt->addWidget(addClipButton);
    buttonLyt->addWidget(removeClipButton);
    connectSignals();
}

void CacheClipWidget::connectSignals(){
    connect(eventName, SIGNAL(textChanged(QString)), this, SLOT(setEventName(QString)), Qt::UniqueConnection);
    connect(unknown, SIGNAL(valueChanged(int)), this, SLOT(setUnknown(int)), Qt::UniqueConnection);
    connect(addClipButton, SIGNAL(released()), this, SLOT(addClipGenerator()), Qt::UniqueConnection);
    connect(removeClipButton, SIGNAL(released()), this, SLOT(removeClipGenerator()), Qt::UniqueConnection);
    connect(clipGenerators, SIGNAL(cellChanged(int,int)), this, SLOT(setClipGeneratorAt(int,int)), Qt::UniqueConnection);
    connect(returnButton, SIGNAL(released()), this, SIGNAL(returnToWidget()), Qt::UniqueConnection);
}

void CacheClipWidget::disconnectSignals(){
    disconnect(eventName, SIGNAL(textChanged(QString)), this, SLOT(setEventName(QString)));
    disconnect(unknown, SIGNAL(valueChanged(int)), this, SLOT(setUnknown(int)));
    disconnect(addClipButton, SIGNAL(released()), this, SLOT(addClipGenerator()));
    disconnect(removeClipButton, SIGNAL(released()), this, SLOT(removeClipGenerator()));
    disconnect(clipGenerators, SIGNAL(cellChanged(int,int)), this, SLOT(setClipGeneratorAt(int,int)));
    disconnect(returnButton, SIGNAL(released()), this, SIGNAL(returnToWidget()));
}

void CacheClipWidget::loadData(AnimCacheClipInfo *data){
    if (data){
        bsData = data;
        disconnectSignals();
        eventName->setText(bsData->eventName);
        unknown->setValue(bsData->unknown);
        clipGenerators->setRowCount(bsData->clipGenerators.size());
        for (int i = 0; i < bsData->clipGenerators.size(); i++){
            if (clipGenerators->item(i, 0)){
                clipGenerators->item(i, 0)->setText(bsData->clipGenerators.at(i));
            }else{
                clipGenerators->setItem(i, 0, new TableWidgetItem(bsData->clipGenerators.at(i)));
            }
        }
        connectSignals();
    }else{
        FATAL_RUNTIME_ERROR("CacheClipWidget::loadData(): The data is nullptr!!");
    }
}

void CacheClipWidget::setEventName(const QString & name){
    if (bsData){
        bsData->eventName = name;
    }else{
        FATAL_RUNTIME_ERROR("CacheClipWidget::setEventName(): The data is nullptr!!");
    }
}

void CacheClipWidget::setUnknown(int newvalue){
    if (bsData){
        bsData->unknown = newvalue;
    }else{
        FATAL_RUNTIME_ERROR("CacheClipWidget::setUnknown(): The data is nullptr!!");
    }
}

void CacheClipWidget::addClipGenerator(){
    if (bsData){
        int index = clipGenerators->rowCount();
        bsData->clipGenerators.append("NEWCLIP");
        clipGenerators->setRowCount(index + 1);
        if (clipGenerators->item(index, 0)){
            clipGenerators->item(index, 0)->setText("NEWCLIP");
        }else{
            clipGenerators->setItem(index, 0, new TableWidgetItem("NEWCLIP"));
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheClipWidget::addClipGenerator(): The data is nullptr!!");
    }
}

void CacheClipWidget::removeClipGenerator(){
    if (bsData){
        int index = clipGenerators->currentRow();
        if (index > -1 && index < bsData->clipGenerators.size()){
            bsData->clipGenerators.removeAt(index);
            clipGenerators->removeRow(index);
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheClipWidget::removeClipGenerator(): The data is nullptr!!");
    }
}

void CacheClipWidget::setClipGeneratorAt(int row, int column){
    if (bsData){
        if (row > -1 && row < bsData->clipGenerators.size()){
            bsData->clipGenerators[row] = clipGenerators->item(row, column)->text();
        }else{
            FATAL_RUNTIME_ERROR("CacheClipWidget::setClipGeneratorAt(): Mismatch between data and UI!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheClipWidget::setClipGeneratorAt(): The data is nullptr!!");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////


CacheWidget::CacheWidget()
    : bsData(nullptr),
      projectData(nullptr),
      lyt(new QVBoxLayout),
      eventButtonLyt(new QHBoxLayout),
      addEventButton(new QPushButton("Add Cache Event")),
      removeEventButton(new QPushButton("Remove Selected Cache Event")),
      cacheEvents(new TableWidget),
      variableButtonLyt(new QHBoxLayout),
      addVariableButton(new QPushButton("Add Cache Behavior Variable")),
      removeVariableButton(new QPushButton("Remove Selected Cache Behavior Variable")),
      behaviorVariables(new TableWidget),
      clipButtonLyt(new QHBoxLayout),
      addClipButton(new QPushButton("Add Cache Clip Generator Name")),
      removeClipButton(new QPushButton("Remove Selected Cache Clip Generator Name")),
      clipGenerators(new TableWidget),
      animationButtonLyt(new QHBoxLayout),
      addAnimationButton(new QPushButton("Add Cache Animation Name")),
      removeAnimationButton(new QPushButton("Remove Selected Cache Animation Name")),
      animations(new TableWidget),
      variableUI(new CacheVariableWidget),
      clipUI(new CacheClipWidget),
      returnButton(new QPushButton("Return"))
{
    //setTitle("Cache Data");
    QWidget *wid = new QWidget();
    wid->setLayout(lyt);
    lyt->addWidget(returnButton);
    lyt->addLayout(eventButtonLyt);
    lyt->addWidget(cacheEvents);
    eventButtonLyt->addWidget(addEventButton);
    eventButtonLyt->addWidget(removeEventButton);
    lyt->addLayout(variableButtonLyt);
    lyt->addWidget(behaviorVariables);
    variableButtonLyt->addWidget(addVariableButton);
    variableButtonLyt->addWidget(removeVariableButton);
    lyt->addLayout(clipButtonLyt);
    lyt->addWidget(clipGenerators);
    clipButtonLyt->addWidget(addClipButton);
    clipButtonLyt->addWidget(removeClipButton);
    lyt->addLayout(animationButtonLyt);
    lyt->addWidget(animations);
    animationButtonLyt->addWidget(addAnimationButton);
    animationButtonLyt->addWidget(removeAnimationButton);
    QStringList list = {"Event Names"};
    cacheEvents->setColumnCount(list.size());
    cacheEvents->setHorizontalHeaderLabels(list);
    cacheEvents->setEditTriggers(QAbstractItemView::DoubleClicked);
    cacheEvents->setToolTip("Double Click To Edit");
    list = {"AnimCacheVariable", "Value"};
    behaviorVariables->setColumnCount(list.size());
    behaviorVariables->setHorizontalHeaderLabels(list);
    behaviorVariables->setEditTriggers(QAbstractItemView::NoEditTriggers);
    list = {"AnimCacheClipInfo", "Value"};
    clipGenerators->setColumnCount(list.size());
    clipGenerators->setHorizontalHeaderLabels(list);
    clipGenerators->setEditTriggers(QAbstractItemView::NoEditTriggers);
    list = {"Animation Names"};
    animations->setColumnCount(list.size());
    animations->setHorizontalHeaderLabels(list);
    animations->setEditTriggers(QAbstractItemView::DoubleClicked);
    animations->setToolTip("Double Click To Edit");
    eventButtonLyt->setSpacing(2);
    variableButtonLyt->setSpacing(2);
    clipButtonLyt->setSpacing(2);
    animationButtonLyt->setSpacing(2);
    addWidget(wid);
    addWidget(variableUI);
    addWidget(clipUI);
    connectSignals();
}

void CacheWidget::connectSignals(){
    connect(cacheEvents, SIGNAL(cellChanged(int,int)), this, SLOT(setCacheEventNameAt(int,int)), Qt::UniqueConnection);
    connect(addEventButton, SIGNAL(released()), this, SLOT(addCacheEvent()), Qt::UniqueConnection);
    connect(removeEventButton, SIGNAL(released()), this, SLOT(removeCacheEvent()), Qt::UniqueConnection);
    connect(addVariableButton, SIGNAL(released()), this, SLOT(addBehaviorVariable()), Qt::UniqueConnection);
    connect(removeVariableButton, SIGNAL(released()), this, SLOT(removeBehaviorVariable()), Qt::UniqueConnection);
    connect(behaviorVariables, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewBehaviorVariable(int,int)), Qt::UniqueConnection);
    connect(addClipButton, SIGNAL(released()), this, SLOT(addClipGenerator()), Qt::UniqueConnection);
    connect(removeClipButton, SIGNAL(released()), this, SLOT(removeClipGenerator()), Qt::UniqueConnection);
    connect(clipGenerators, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewClipGenerator(int,int)), Qt::UniqueConnection);
    connect(addAnimationButton, SIGNAL(released()), this, SLOT(addAnimation()), Qt::UniqueConnection);
    connect(removeAnimationButton, SIGNAL(released()), this, SLOT(removeAnimation()), Qt::UniqueConnection);
    connect(animations, SIGNAL(cellChanged(int,int)), this, SLOT(setAnimationNameAt(int,int)), Qt::UniqueConnection);
    connect(variableUI, SIGNAL(returnToWidget()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(clipUI, SIGNAL(returnToWidget()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(returnButton, SIGNAL(released()), this, SIGNAL(returnToMainWidget()), Qt::UniqueConnection);
}

void CacheWidget::disconnectSignals(){
    disconnect(cacheEvents, SIGNAL(cellChanged(int,int)), this, SLOT(setCacheEventNameAt(int,int)));
    disconnect(addEventButton, SIGNAL(released()), this, SLOT(addCacheEvent()));
    disconnect(removeEventButton, SIGNAL(released()), this, SLOT(removeCacheEvent()));
    disconnect(addVariableButton, SIGNAL(released()), this, SLOT(addBehaviorVariable()));
    disconnect(removeVariableButton, SIGNAL(released()), this, SLOT(removeBehaviorVariable()));
    disconnect(behaviorVariables, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewBehaviorVariable(int,int)));
    disconnect(addClipButton, SIGNAL(released()), this, SLOT(addClipGenerator()));
    disconnect(removeClipButton, SIGNAL(released()), this, SLOT(removeClipGenerator()));
    disconnect(clipGenerators, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewClipGenerator(int,int)));
    disconnect(addAnimationButton, SIGNAL(released()), this, SLOT(addAnimation()));
    disconnect(removeAnimationButton, SIGNAL(released()), this, SLOT(removeAnimation()));
    disconnect(animations, SIGNAL(cellChanged(int,int)), this, SLOT(setAnimationNameAt(int,int)));
    disconnect(variableUI, SIGNAL(returnToWidget()), this, SLOT(returnToWidget()));
    disconnect(clipUI, SIGNAL(returnToWidget()), this, SLOT(returnToWidget()));
    disconnect(returnButton, SIGNAL(released()), this, SIGNAL(returnToMainWidget()));
}

void CacheWidget::loadData(AnimCacheAnimSetData *data, ProjectFile *project){
    if (data && project){
        bsData = data;
        projectData = project;
        disconnectSignals();
        cacheEvents->setRowCount(bsData->cacheEvents.size());
        for (int i = 0; i < bsData->cacheEvents.size(); i++){
            if (cacheEvents->item(i, 0)){
                cacheEvents->item(i, 0)->setText(bsData->cacheEvents.at(i));
            }else{
                cacheEvents->setItem(i, 0, new TableWidgetItem(bsData->cacheEvents.at(i)));
            }
        }
        behaviorVariables->setRowCount(bsData->behaviorVariables.size());
        for (int i = 0; i < bsData->behaviorVariables.size(); i++){
            if (behaviorVariables->item(i, 0)){
                behaviorVariables->item(i, 0)->setText(bsData->behaviorVariables.at(i).name);
            }else{
                behaviorVariables->setItem(i, 0, new TableWidgetItem(bsData->behaviorVariables.at(i).name));
                behaviorVariables->setItem(i, 1, new TableWidgetItem("Double Click To Edit"));
            }
        }
        clipGenerators->setRowCount(bsData->clipGenerators.size());
        for (int i = 0; i < bsData->clipGenerators.size(); i++){
            if (clipGenerators->item(i, 0)){
                clipGenerators->item(i, 0)->setText(bsData->clipGenerators.at(i).eventName);
            }else{
                clipGenerators->setItem(i, 0, new TableWidgetItem(bsData->clipGenerators.at(i).eventName));
                clipGenerators->setItem(i, 1, new TableWidgetItem("Double Click To Edit"));
            }
        }
        animations->setRowCount(bsData->animations.size());
        for (int i = 0; i < bsData->animations.size(); i++){
            if (animations->item(i, 0)){
                animations->item(i, 0)->setText(projectData->findAnimationNameFromEncryptedData(bsData->animations.at(i).crcAnimationName));
            }else{
                animations->setItem(i, 0, new TableWidgetItem(projectData->findAnimationNameFromEncryptedData(bsData->animations.at(i).crcAnimationName)));
            }
        }
        connectSignals();
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::loadData(): The data is nullptr!!");
    }
}

void CacheWidget::setCacheEventNameAt(int row, int column){
    if (bsData){
        if (row > -1 && row < bsData->cacheEvents.size()){
            bsData->cacheEvents[row] = cacheEvents->item(row, column)->text();
        }else{
            FATAL_RUNTIME_ERROR("CacheWidget::setEventNameAt(): Mismatch between data and UI!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::setEventNameAt(): The data is nullptr!!");
    }
}

void CacheWidget::addCacheEvent(){
    if (bsData){
        int index = cacheEvents->rowCount();
        bsData->cacheEvents.append("NEWEVENT");
        cacheEvents->setRowCount(index + 1);
        if (cacheEvents->item(index, 0)){
            cacheEvents->item(index, 0)->setText("NEWEVENT");
        }else{
            cacheEvents->setItem(index, 0, new TableWidgetItem("NEWEVENT"));
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::addCacheEvent(): The data is nullptr!!");
    }
}

void CacheWidget::removeCacheEvent(){
    if (bsData){
        int index = cacheEvents->currentRow();
        if (index > -1 && index < bsData->cacheEvents.size()){
            bsData->cacheEvents.removeAt(index);
            cacheEvents->removeRow(index);
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::removeCacheEvent(): The data is nullptr!!");
    }
}

void CacheWidget::addBehaviorVariable(){
    if (bsData){
        int index = behaviorVariables->rowCount();
        bsData->behaviorVariables.append(AnimCacheVariable("NEWVARIABLE"));
        behaviorVariables->setRowCount(index + 1);
        if (behaviorVariables->item(index, 0)){
            behaviorVariables->item(index, 0)->setText("NEWVARIABLE");
        }else{
            behaviorVariables->setItem(index, 0, new TableWidgetItem("NEWVARIABLE"));
            behaviorVariables->setItem(index, 1, new TableWidgetItem("Double Click To Edit"));
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::addBehaviorVariable(): The data is nullptr!!");
    }
}

void CacheWidget::removeBehaviorVariable(){
    if (bsData){
        int index = behaviorVariables->currentRow();
        if (index > -1 && index < bsData->behaviorVariables.size()){
            bsData->behaviorVariables.removeAt(index);
            behaviorVariables->removeRow(index);
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::removeBehaviorVariable(): The data is nullptr!!");
    }
}

void CacheWidget::addClipGenerator(){
    if (bsData){
        int index = clipGenerators->rowCount();
        bsData->clipGenerators.append(AnimCacheClipInfo("NEWCLIPGENERATOR"));
        clipGenerators->setRowCount(index + 1);
        if (clipGenerators->item(index, 0)){
            clipGenerators->item(index, 0)->setText("NEWCLIPGENERATOR");
        }else{
            clipGenerators->setItem(index, 0, new TableWidgetItem("NEWCLIPGENERATOR"));
            clipGenerators->setItem(index, 1, new TableWidgetItem("Double Click To Edit"));
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::addClipGenerator(): The data is nullptr!!");
    }
}

void CacheWidget::removeClipGenerator(){
    if (bsData){
        int index = clipGenerators->currentRow();
        if (index > -1 && index < bsData->clipGenerators.size()){
            bsData->clipGenerators.removeAt(index);
            clipGenerators->removeRow(index);
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::removeClipGenerator(): The data is nullptr!!");
    }
}

void CacheWidget::setAnimationNameAt(int row, int column){
    if (bsData && projectData){
        if (row > -1 && row < bsData->animations.size()){
            bsData->animations[row].setAnimationData(projectData->projectAnimationsPath, animations->item(row, column)->text(), true);
        }else{
            FATAL_RUNTIME_ERROR("CacheWidget::setAnimationNameAt(): Mismatch between data and UI!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::setAnimationNameAt(): The data is nullptr!!");
    }
}

void CacheWidget::addAnimation(){
    if (bsData && projectData){
        int index = animations->rowCount();
        bsData->animations.append(AnimCacheAnimationInfo(projectData->projectAnimationsPath, "NEWANIMATION", true));
        animations->setRowCount(index + 1);
        if (animations->item(index, 0)){
            animations->item(index, 0)->setText(projectData->findAnimationNameFromEncryptedData(bsData->animations.back().crcAnimationName));
        }else{
            animations->setItem(index, 0, new TableWidgetItem(projectData->findAnimationNameFromEncryptedData(bsData->animations.back().crcAnimationName)));
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::addCacheEvent(): The data is nullptr!!");
    }
}

void CacheWidget::removeAnimation(){
    if (bsData){
        int index = animations->currentRow();
        if (index > -1 && index < bsData->animations.size()){
            bsData->animations.removeAt(index);
            animations->removeRow(index);
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::removeCacheEvent(): The data is nullptr!!");
    }
}

void CacheWidget::viewBehaviorVariable(int row, int column){
    if (bsData){
        if (column == 1){
            //int index = behaviorVariables->currentRow();
            if (row > -1 && row < bsData->behaviorVariables.size()){
                variableUI->loadData(&bsData->behaviorVariables[row]);
                setCurrentIndex(VARIABLE_WIDGET);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::viewBehaviorVariable(): The data is nullptr!!");
    }
}

void CacheWidget::viewClipGenerator(int row, int column){
    if (bsData){
        if (column == 1){
            //int index = clipGenerators->currentRow();
            if (row > -1 && row < bsData->clipGenerators.size()){
                clipUI->loadData(&bsData->clipGenerators[row]);
                setCurrentIndex(CLIP_WIDGET);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("CacheWidget::viewBehaviorVariable(): The data is nullptr!!");
    }
}

void CacheWidget::returnToWidget(){
    loadData(bsData, projectData);
    setCurrentIndex(MAIN_WIDGET);
}

///////////////////////////////////////////////////////////////////////////////////////////////////



AnimationCacheUI::AnimationCacheUI()
    : bsData(nullptr),
      projectData(nullptr),
      lyt(new QVBoxLayout),
      groupBox(new QGroupBox("Animation Cache Files")),
      cacheFiles(new TableWidget),
      cacheUI(new CacheWidget),
      cacheButtonLyt(new QHBoxLayout),
      addCacheButton(new QPushButton("Add Cache File")),
      removeCacheButton(new QPushButton("Remove Selected Cache File"))
{
    lyt->addWidget(cacheFiles);
    groupBox->setLayout(lyt);
    QStringList list = {"Filename", "Value"};
    cacheFiles->setColumnCount(list.size());
    cacheFiles->setHorizontalHeaderLabels(list);
    cacheFiles->setEditTriggers(QAbstractItemView::NoEditTriggers);
    lyt->addLayout(cacheButtonLyt);
    lyt->addWidget(cacheFiles);
    cacheButtonLyt->addWidget(addCacheButton);
    cacheButtonLyt->addWidget(removeCacheButton);
    cacheButtonLyt->setSpacing(2);
    addWidget(groupBox);
    addWidget(cacheUI);
    connectSignals();
}

void AnimationCacheUI::connectSignals(){
    connect(cacheUI, SIGNAL(returnToMainWidget()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(addCacheButton, SIGNAL(released()), this, SLOT(addCacheFile()), Qt::UniqueConnection);
    connect(removeCacheButton, SIGNAL(released()), this, SLOT(removeCacheFile()), Qt::UniqueConnection);
    connect(cacheFiles, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedFile(int,int)), Qt::UniqueConnection);
}

void AnimationCacheUI::disconnectSignals(){
    disconnect(cacheUI, SIGNAL(returnToMainWidget()), this, SLOT(returnToWidget()));
    disconnect(addCacheButton, SIGNAL(released()), this, SLOT(addCacheFile()));
    disconnect(removeCacheButton, SIGNAL(released()), this, SLOT(removeCacheFile()));
    disconnect(cacheFiles, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedFile(int,int)));
}

void AnimationCacheUI::loadData(ProjectFile *project){
    if (project){
        projectData = project;
        bsData = projectData->getProjectCacheData();
        disconnectSignals();
        cacheFiles->setRowCount(bsData->fileNames.size());
        for (int i = 0; i < bsData->fileNames.size(); i++){
            if (cacheFiles->item(i, 0)){
                cacheFiles->item(i, 0)->setText(bsData->fileNames.at(i));
            }else{
                cacheFiles->setItem(i, 0, new TableWidgetItem(bsData->fileNames.at(i)));
                cacheFiles->setItem(i, 1, new TableWidgetItem("Double Click To Edit"));
            }
        }
        connectSignals();
    }else{
        FATAL_RUNTIME_ERROR("AnimationCacheUI::loadData(): The data is nullptr!!");
    }
}

void AnimationCacheUI::viewSelectedFile(int row, int column){
    if (bsData){
        if (column == 1){
            //int index = behaviorVariables->currentRow();
            if (row > -1 && row < bsData->animSetData.size()){
                cacheUI->loadData(bsData->animSetData[row], projectData);
                setCurrentIndex(CACHE_WIDGET);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("AnimationCacheUI::viewSelectedFile(): The data is nullptr!!");
    }
}

void AnimationCacheUI::addCacheFile(){
    if (bsData){
        int index = cacheFiles->rowCount();
        bsData->animSetData.append(new AnimCacheAnimSetData(QStringList("FullBody.txt")));
        bsData->fileNames.append(projectData->getProjectName()+"ProjectData\\"+projectData->getProjectName()+".txt");
        cacheFiles->setRowCount(index + 1);
        if (cacheFiles->item(index, 0)){
            cacheFiles->item(index, 0)->setText(bsData->fileNames.last());
        }else{
            cacheFiles->setItem(index, 0, new TableWidgetItem(bsData->fileNames.last()));
            cacheFiles->setItem(index, 0, new TableWidgetItem("Double Click To Edit"));
        }
    }else{
        FATAL_RUNTIME_ERROR("AnimationCacheUI::addCacheFile(): The data is nullptr!!");
    }
}

void AnimationCacheUI::removeCacheFile(){
    if (bsData){
        int index = cacheFiles->currentRow();
        if (index > -1 && index < bsData->animSetData.size()){
            bsData->animSetData.removeAt(index);
            cacheFiles->removeRow(index);
        }
    }else{
        FATAL_RUNTIME_ERROR("AnimationCacheUI::removeCacheFile(): The data is nullptr!!");
    }
}

void AnimationCacheUI::returnToWidget(){
    loadData(projectData);  //Inefficient...
    setCurrentIndex(MAIN_WIDGET);
}
