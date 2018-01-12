#ifndef ANIMATIONCACHEUI_H
#define ANIMATIONCACHEUI_H

#include <QStackedWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>

#include "src/utility.h"
#include "src/filetypes/projectfile.h"
#include "src/ui/genericdatawidgets.h"
#include "src/animSetData/animcachevariable.h"
#include "src/animSetData/animcacheclipinfo.h"
#include "src/animSetData/animcacheanimationinfo.h"
#include "src/animSetData/animcacheanimsetdata.h"

class AnimCacheProjectData;
class AnimCacheVariable;

class CacheVariableWidget: QGroupBox
{
    Q_OBJECT
public:
    CacheVariableWidget()
        : lyt(new QVBoxLayout),
          nameLyt(new QFormLayout),
          minValueLyt(new QFormLayout),
          maxValueLyt(new QFormLayout),
          name(new LineEdit),
          minValue(new SpinBox),
          maxValue(new SpinBox)
    {
        setTitle("Behavior Variable Cache Data");
        setLayout(lyt);
        lyt->addLayout(nameLyt);
        lyt->addLayout(minValueLyt);
        lyt->addLayout(maxValueLyt);
        nameLyt->addRow("Variable Name: ", name);
        minValueLyt->addRow("Minimum Value: ", minValue);
        maxValueLyt->addRow("Maximum Value: ", maxValue);
        connect(name, SIGNAL(textChanged(QString)), this, SLOT(setName(QString)), Qt::UniqueConnection);
        connect(minValue, SIGNAL(valueChanged(int)), this, SLOT(setMinimumValue(int)), Qt::UniqueConnection);
        connect(maxValue, SIGNAL(valueChanged(int)), this, SLOT(setMaximumValue(int)), Qt::UniqueConnection);
    }
    void loadData(AnimCacheVariable *data){
        if (data){
            bsData = data;
            name->setText(bsData->name);
            minValue->setValue(bsData->minValue);
            maxValue->setValue(bsData->maxValue);
        }else{
            (qFatal("CacheVariableWidget::loadData(): The data is NULL!!"));
        }
    }
private slots:
    void setName(const QString & newname){
        if (bsData){
            bsData->name = newname;
        }else{
            (qFatal("CacheVariableWidget::setName(): The data is NULL!!"));
        }
    }
    void setMinimumValue(int newvalue){
        if (bsData){
            bsData->minValue = newvalue;
        }else{
            (qFatal("CacheVariableWidget::setMinimumValue(): The data is NULL!!"));
        }
    }
    void setMaximumValue(int newvalue){
        if (bsData){
            bsData->minValue = newvalue;
        }else{
            (qFatal("CacheVariableWidget::setMaximumValue(): The data is NULL!!"));
        }
    }
private:
    AnimCacheVariable *bsData;
    QVBoxLayout *lyt;
    QFormLayout *nameLyt;
    QFormLayout *minValueLyt;
    QFormLayout *maxValueLyt;
    LineEdit *name;
    SpinBox *minValue;
    SpinBox *maxValue;
};

class CacheClipWidget: QGroupBox
{
    Q_OBJECT
    friend class CacheWidget;
public:
    CacheClipWidget()
        : lyt(new QVBoxLayout),
          eventLyt(new QFormLayout),
          unknownLyt(new QFormLayout),
          eventName(new LineEdit),
          unknown(new SpinBox),
          buttonLyt(new QHBoxLayout),
          addClipButton(new QPushButton("Add Clip Generator")),
          removeClipButton(new QPushButton("Remove Selected Clip Generator")),
          clipGenerators(new TableWidget)
    {
        setTitle("Clip Generator Cache Data");
        setLayout(lyt);
        lyt->addLayout(eventLyt);
        lyt->addLayout(unknownLyt);
        lyt->addLayout(buttonLyt);
        lyt->addWidget(clipGenerators);
        QStringList list = {"Clip Generator Names"};
        clipGenerators->setColumnCount(list.size());
        clipGenerators->setHorizontalHeaderLabels(list);
        clipGenerators->setEditTriggers(QAbstractItemView::DoubleClicked);
        eventLyt->addRow("Event Name: ", eventName);
        unknownLyt->addRow("Unknown Variable: ", unknown);
        buttonLyt->addWidget(addClipButton);
        buttonLyt->addWidget(removeClipButton);
        connect(eventName, SIGNAL(textChanged(QString)), this, SLOT(setEventName(QString)), Qt::UniqueConnection);
        connect(unknown, SIGNAL(valueChanged(int)), this, SLOT(setUnknown(int)), Qt::UniqueConnection);
        connect(addClipButton, SIGNAL(released()), this, SLOT(addClipGenerator()), Qt::UniqueConnection);
        connect(removeClipButton, SIGNAL(released()), this, SLOT(removeClipGenerator()), Qt::UniqueConnection);
        connect(clipGenerators, SIGNAL(cellChanged(int,int)), this, SLOT(setClipGeneratorAt(int,int)), Qt::UniqueConnection);
    }
private slots:
    void loadData(AnimCacheClipInfo *data){
        if (data){
            bsData = data;
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
        }else{
            (qFatal("CacheVariableWidget::loadData(): The data is NULL!!"));
        }
    }
    void setEventName(const QString & name){
        if (bsData){
            bsData->eventName = name;
        }else{
            (qFatal("CacheClipWidget::setEventName(): The data is NULL!!"));
        }
    }
    void setUnknown(int newvalue){
        if (bsData){
            bsData->unknown = newvalue;
        }else{
            (qFatal("CacheClipWidget::setUnknown(): The data is NULL!!"));
        }
    }
    void addClipGenerator(){
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
            (qFatal("CacheClipWidget::addClipGenerator(): The data is NULL!!"));
        }
    }
    void removeClipGenerator(){
        if (bsData){
            int index = clipGenerators->currentRow();
            if (index > -1 && index < bsData->clipGenerators.size()){
                bsData->clipGenerators.removeAt(index);
                clipGenerators->removeRow(index);
            }
        }else{
            (qFatal("CacheClipWidget::removeClipGenerator(): The data is NULL!!"));
        }
    }
    void setClipGeneratorAt(int row, int column){
        if (bsData){
            if (row > -1 && row < bsData->clipGenerators.size()){
                bsData->clipGenerators[row] = clipGenerators->item(row, column)->text();
            }else{
                (qFatal("CacheClipWidget::setClipGeneratorAt(): Mismatch between data and UI!!"));
            }
        }else{
            (qFatal("CacheClipWidget::setClipGeneratorAt(): The data is NULL!!"));
        }
    }
private:
    AnimCacheClipInfo *bsData;
    QVBoxLayout *lyt;
    QFormLayout *eventLyt;
    QFormLayout *unknownLyt;
    LineEdit *eventName;
    SpinBox *unknown;
    QHBoxLayout *buttonLyt;
    QPushButton *addClipButton;
    QPushButton *removeClipButton;
    TableWidget *clipGenerators;
};

class CacheWidget: QStackedWidget
{
    Q_OBJECT
public:
    CacheWidget()
        : lyt(new QVBoxLayout),
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
          animations(new TableWidget)
    {
        //setTitle("Cache Data");
        setLayout(lyt);
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
    }
private slots:
    void loadData(AnimCacheAnimSetData *data, ProjectFile *project){
        if (data && project){
            bsData = data;
            projectData = project;
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
                }
            }
            clipGenerators->setRowCount(bsData->clipGenerators.size());
            for (int i = 0; i < bsData->clipGenerators.size(); i++){
                if (clipGenerators->item(i, 0)){
                    clipGenerators->item(i, 0)->setText(bsData->clipGenerators.at(i).eventName);
                }else{
                    clipGenerators->setItem(i, 0, new TableWidgetItem(bsData->clipGenerators.at(i).eventName));
                }
            }
            animations->setRowCount(bsData->animations.size());
            for (int i = 0; i < bsData->animations.size(); i++){
                if (animations->item(i, 0)){
                    animations->item(i, 0)->setText(bsData->animations.at(i).crcAnimationName); //Get animation from data here...
                }else{
                    animations->setItem(i, 0, new TableWidgetItem(bsData->animations.at(i).crcAnimationName));
                }
            }
        }else{
            (qFatal("CacheWidget::loadData(): The data is NULL!!"));
        }
    }
    void setCacheEventNameAt(int row, int column){
        if (bsData){
            if (row > -1 && row < bsData->cacheEvents.size()){
                bsData->cacheEvents[row] = cacheEvents->item(row, column)->text();
            }else{
                (qFatal("CacheWidget::setEventNameAt(): Mismatch between data and UI!!"));
            }
        }else{
            (qFatal("CacheWidget::setEventNameAt(): The data is NULL!!"));
        }
    }
    void addCacheEvent(){
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
            (qFatal("CacheWidget::addCacheEvent(): The data is NULL!!"));
        }
    }
    void removeCacheEvent(){
        if (bsData){
            int index = cacheEvents->currentRow();
            if (index > -1 && index < bsData->cacheEvents.size()){
                bsData->cacheEvents.removeAt(index);
                cacheEvents->removeRow(index);
            }
        }else{
            (qFatal("CacheWidget::removeCacheEvent(): The data is NULL!!"));
        }
    }
    void addBehaviorVariable(){
        if (bsData){
            int index = behaviorVariables->rowCount();
            bsData->behaviorVariables.append(AnimCacheVariable("NEWVARIABLE"));
            behaviorVariables->setRowCount(index + 1);
            if (behaviorVariables->item(index, 0)){
                behaviorVariables->item(index, 0)->setText("NEWVARIABLE");
            }else{
                behaviorVariables->setItem(index, 0, new TableWidgetItem("NEWVARIABLE"));
            }
        }else{
            (qFatal("CacheWidget::addBehaviorVariable(): The data is NULL!!"));
        }
    }
    void removeBehaviorVariable(){
        if (bsData){
            int index = behaviorVariables->currentRow();
            if (index > -1 && index < bsData->behaviorVariables.size()){
                bsData->behaviorVariables.removeAt(index);
                behaviorVariables->removeRow(index);
            }
        }else{
            (qFatal("CacheWidget::removeBehaviorVariable(): The data is NULL!!"));
        }
    }
    void addClipGenerator(){
        if (bsData){
            int index = clipGenerators->rowCount();
            bsData->clipGenerators.append(AnimCacheClipInfo("NEWCLIPGENERATOR"));
            clipGenerators->setRowCount(index + 1);
            if (clipGenerators->item(index, 0)){
                clipGenerators->item(index, 0)->setText("NEWCLIPGENERATOR");
            }else{
                clipGenerators->setItem(index, 0, new TableWidgetItem("NEWCLIPGENERATOR"));
            }
        }else{
            (qFatal("CacheWidget::addClipGenerator(): The data is NULL!!"));
        }
    }
    void removeClipGenerator(){
        if (bsData){
            int index = clipGenerators->currentRow();
            if (index > -1 && index < bsData->clipGenerators.size()){
                bsData->clipGenerators.removeAt(index);
                clipGenerators->removeRow(index);
            }
        }else{
            (qFatal("CacheWidget::removeClipGenerator(): The data is NULL!!"));
        }
    }
    void setAnimationNameAt(int row, int column){
        if (bsData && projectData){
            if (row > -1 && row < bsData->animations.size()){
                bsData->animations[row].setAnimationData(projectData->projectPath, animations->item(row, column)->text(), true);
            }else{
                (qFatal("CacheWidget::setAnimationNameAt(): Mismatch between data and UI!!"));
            }
        }else{
            (qFatal("CacheWidget::setAnimationNameAt(): The data is NULL!!"));
        }
    }
    void addAnimation(){
        if (bsData && projectData){
            int index = animations->rowCount();
            bsData->animations.append(AnimCacheAnimationInfo(projectData->projectPath, "NEWANIMATION"));
            animations->setRowCount(index + 1);
            if (animations->item(index, 0)){
                animations->item(index, 0)->setText("NEWANIMATION");
            }else{
                animations->setItem(index, 0, new TableWidgetItem("NEWANIMATION"));
            }
        }else{
            (qFatal("CacheWidget::addCacheEvent(): The data is NULL!!"));
        }
    }
    void removeAnimation(){
        if (bsData){
            int index = animations->currentRow();
            if (index > -1 && index < bsData->animations.size()){
                bsData->animations.removeAt(index);
                animations->removeRow(index);
            }
        }else{
            (qFatal("CacheWidget::removeCacheEvent(): The data is NULL!!"));
        }
    }
    void viewBehaviorVariable(int row, int column){
        if (bsData && column == 1){
            //int index = behaviorVariables->currentRow();
            if (row > -1 && row < bsData->behaviorVariables.size()){
                variableUI->loadData(&bsData->behaviorVariables[row]);
                setCurrentIndex(VARIABLE_WIDGET);
            }
        }else{
            (qFatal("CacheWidget::viewBehaviorVariable(): The data is NULL!!"));
        }
    }
    void viewClipGenerator(int row, int column){
        if (bsData && column == 1){
            //int index = clipGenerators->currentRow();
            if (row > -1 && row < bsData->clipGenerators.size()){
                clipUI->loadData(&bsData->clipGenerators[row]);
                setCurrentIndex(CLIP_WIDGET);
            }
        }else{
            (qFatal("CacheWidget::viewBehaviorVariable(): The data is NULL!!"));
        }
    }
    void returnToWidget(){
        setCurrentIndex(MAIN_WIDGET);
    }
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        VARIABLE_WIDGET,
        CLIP_WIDGET
    };
private:
    AnimCacheAnimSetData *bsData;
    ProjectFile *projectData;
    QVBoxLayout *lyt;
    QHBoxLayout *eventButtonLyt;
    QPushButton *addEventButton;
    QPushButton *removeEventButton;
    TableWidget *cacheEvents;
    QHBoxLayout *variableButtonLyt;
    QPushButton *addVariableButton;
    QPushButton *removeVariableButton;
    TableWidget *behaviorVariables;
    QHBoxLayout *clipButtonLyt;
    QPushButton *addClipButton;
    QPushButton *removeClipButton;
    TableWidget *clipGenerators;
    QHBoxLayout *animationButtonLyt;
    QPushButton *addAnimationButton;
    QPushButton *removeAnimationButton;
    TableWidget *animations;
    CacheVariableWidget *variableUI;
    CacheClipWidget *clipUI;
};

class AnimationCacheUI: public QStackedWidget
{
    Q_OBJECT
public:
    AnimationCacheUI();
    void loadData(AnimCacheProjectData *animSetData);
private slots:
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        CACHE_WIDGET
    };
    void connectSignals();
    void disconnectSignals();
    void viewSelectedChild(int row, int column);
    void addCacheFile();
    void addCacheEvents();
    void addCacheVariable();
    void addCacheClipData();
    void addCacheAnimation();
    void loadDynamicTableRows();
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString &tip1, const QString &tip2);
    void loadTableValue(int row, const QString & value);
    void removeCacheFile(int index);
    void removeCacheEvents(int index);
    void removeCacheVariable(int index);
    void removeCacheClipData(int index);
    void removeCacheAnimation(int index);
private:
    static QStringList types;
    static QStringList headerLabels;
    AnimCacheProjectData *bsData;
    QGroupBox *groupBox;
    TableWidget *cacheFiles;
    CacheWidget *cacheUI;
    TableWidget *animationCacheFiles;
};

#endif // ANIMATIONCACHEUI_H
