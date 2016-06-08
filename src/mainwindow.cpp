#include "mainwindow.h"
#include "fileselectwindow.h"
#include "hkxfile.h"
#include "generators.h"

#include <QBoxLayout>
#include <QMenuBar>
#include <QFile>
#include <QSettings>
#include <QGroupBox>
#include <QMessageBox>

/**
 * GeneratorIcon
 */

GeneratorIcon::GeneratorIcon(const HkObjectExpSharedPtr & d, const QString & s)
    :data(d),\
      name(s)
{
    //
}
void GeneratorIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::red);
    painter->setBrush(Qt::green);
    painter->drawRoundedRect(boundingRect(), 4, 4);
    painter->setPen(Qt::black);
    painter->drawText(boundingRect(), name);
}

/**
 * BehaviorGraphView
 */

int getLastIndex(const QList <HkObjectExpSharedPtr> & list){
    return list.size() - 1;
}

bool BehaviorGraphView::drawBehaviorGraph(){
    QList <HkObjectExpSharedPtr> objects;
    QList <GeneratorIcon *> parentIcons;
    int result;
    int numitems = 0;
    objects.append(behavior->getRootObject());
    while (!objects.isEmpty()){
        numitems = behaviorGS->items().size();
        qulonglong sig = objects.last()->getSignature();
        switch (sig){
        case HKB_STATE_MACHINE:
        {
            hkbStateMachine *ptr = static_cast<hkbStateMachine *>(objects.last().data());
            result = helperFunction(ptr, objects, parentIcons);
            if (result == 0){
                for (int i = ptr->states.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbStateMachineStateInfo *>(ptr->states.at(i).data())->generator);
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        /*case HKB_STATE_MACHINE_STATE_INFO:
        {
            if (parentIcons.isEmpty()){
                return false;
            }
            if (objects.isEmpty()){
                return false;
            }
            hkbStateMachineStateInfo *ptr = static_cast<hkbStateMachineStateInfo *>(objects.last().data());
            GeneratorIcon *icon = positionIcon(objects.last(), ptr, parentIcons.last());
            if (static_cast<hkbStateMachine *>(parentIcons.last()->data.data())->states.size() < 2 ||\
                    static_cast<hkbStateMachine *>(parentIcons.last()->data.data())->states.last() == objects.last())
            {
                parentIcons.removeLast();
            }
            objects.removeLast();
            objects.append(ptr->generator);
            parentIcons.append(icon);
            break;
        }*/
        case BS_SYNCHRONIZED_CLIP_GENERATOR:
        {
            BSSynchronizedClipGenerator *ptr = static_cast<BSSynchronizedClipGenerator *>(objects.last().data());
            result = helperFunction(ptr, objects, parentIcons);
            if (result == 0){
                objects.append(ptr->pClipGenerator);
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_I_STATE_TAGGING_GENERATOR:
        {
            BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(objects.last().data());
            result = helperFunction(ptr, objects, parentIcons);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_BONE_SWITCH_GENERATOR:
        {
            BSBoneSwitchGenerator *ptr = static_cast<BSBoneSwitchGenerator *>(objects.last().data());
            result = helperFunction(ptr, objects, parentIcons);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
                for (int i = ptr->ChildrenA.size() - 1; i >= 0; i--){
                    objects.append(static_cast<BSBoneSwitchGeneratorBoneData *>(ptr->ChildrenA.at(i).data())->pGenerator);
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *ptr = static_cast<hkbManualSelectorGenerator *>(objects.last().data());
            result = helperFunction(ptr, objects, parentIcons);
            if (result == 0){
                for (int i = ptr->generators.size() - 1; i >= 0; i--){
                    objects.append(ptr->generators.at(i));
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_BLENDER_GENERATOR:
        {
            hkbBlenderGenerator *ptr = static_cast<hkbBlenderGenerator *>(objects.last().data());
            result = helperFunction(ptr, objects, parentIcons);
            if (result == 0){
                for (int i = ptr->children.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbBlenderGeneratorChild *>(ptr->children.at(i).data())->generator);
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_MODIFIER_GENERATOR:
        {
            hkbModifierGenerator *ptr = static_cast<hkbModifierGenerator *>(objects.last().data());
            result = helperFunction(ptr, objects, parentIcons);
            if (result == 0){
                //objects.append(ptr->modifier);
                objects.append(ptr->generator);
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        {
            BSCyclicBlendTransitionGenerator *ptr = static_cast<BSCyclicBlendTransitionGenerator *>(objects.last().data());
            result = helperFunction(ptr, objects, parentIcons);
            if (result == 0){
                objects.append(ptr->pBlenderGenerator);
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_BEHAVIOR_REFERENCE_GENERATOR:
        {
            hkbBehaviorReferenceGenerator *ptr = static_cast<hkbBehaviorReferenceGenerator *>(objects.last().data());
            if (helperFunction(ptr, objects, parentIcons) == -1){
                return false;
            }
            break;
        }
        case HKB_CLIP_GENERATOR:
        {
            hkbClipGenerator *ptr = static_cast<hkbClipGenerator *>(objects.last().data());
            if (helperFunction(ptr, objects, parentIcons) == -1){
                return false;
            }
            break;
        }
        case HKB_BEHAVIOR_GRAPH:
        {
            hkbBehaviorGraph *ptr = static_cast<hkbBehaviorGraph *>(objects.last().data());
            GeneratorIcon *icon = new GeneratorIcon(objects.last(), ptr->name);
            objects.removeLast();
            behaviorGS->addItem(icon);
            objects.append(ptr->rootGenerator);
            parentIcons.append(icon);
            break;
        }
        case HK_ROOT_LEVEL_CONTAINER:
        {
            hkRootLevelContainer *ptr = static_cast<hkRootLevelContainer *>(objects.last().data());
            objects.removeLast();
            for (int i = ptr->namedVariants.size() - 1; i >= 0; i--){
                objects.append(ptr->namedVariants.at(i).variant);
            }
            break;
        }
        default:
            break;
        }
    }
    return true;
}

/**
 * MainWindow
 */

MainWindow::MainWindow(){
    topLyt = new QVBoxLayout(this);
    topMB = new QMenuBar(this);
    openA = new QAction("Open Project", this);
    openA->setStatusTip("Open a hkx project file!");
    openA->setShortcut(QKeySequence::Open);
    openM =new QMenu("File");
    openM->addAction(openA);
    topMB->addMenu(openM);
    topLyt->addWidget(topMB);
    dirViewFSW = new FileSelectWindow("Select a hkx file to open!");
    hkxFile = NULL;
    iconGBLyt = new QVBoxLayout;
    behaviorGraphViewGB = new QGroupBox("Behavior Graph");
    //BehaviorGraphViewGB->setLayout(iconGBLyt);
    readSettings();
    setLayout(topLyt);
    connect(openA, SIGNAL(triggered(bool)), this, SLOT(openDirView()));
    connect(dirViewFSW, SIGNAL(selectFile(QString)), this, SLOT(openHkxfile(QString)));
}

MainWindow::~MainWindow(){
    //
}

void MainWindow::openDirView(){
    dirViewFSW->show();
}

void MainWindow::openHkxfile(QString name){
    hkxFile = new BehaviorFile(name);
    behaviorGraphViewIV = new BehaviorGraphView(hkxFile);
    if (!behaviorGraphViewIV->drawBehaviorGraph()){
        //
    }
    iconGBLyt->addWidget(behaviorGraphViewIV);
    behaviorGraphViewGB->setLayout(iconGBLyt);
    topLyt->addWidget(behaviorGraphViewGB);
}

void MainWindow::readSettings()
{
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("Main Window");
    QPoint pos = settings.value("pos", QPoint(500, 500)).toPoint();
    QSize size = settings.value("size", QSize(900, 900)).toSize();
    resize(size);
    move(pos);
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("Main Window");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
}

bool MainWindow::exitProgram()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"), "Quit?", QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok){
        return true;
    }else if (ret == QMessageBox::Cancel){
        return false;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (exitProgram()) {
        writeSettings();
        event->accept();
    }else {
        event->ignore();
    }
}
