#include "mainwindow.h"
#include "fileselectwindow.h"
#include "hkxfile.h"

#include <QBoxLayout>
#include <QMenuBar>
#include <QFile>
#include <QSettings>
#include <QGroupBox>
#include <QMessageBox>

/**
 * GeneratorIcon
 */

void GeneratorIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
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

bool BehaviorGraphView::drawBehaviorGraph(const HkObjectExpSharedPtr & obj, GeneratorIcon * parentIcon){
    if (!obj.data()){
        return false;
    }
    int y = 5;
    int *l = &y;
    /*if (behaviorGS->items().size() > 1000){
        return false;
    }*/
    qulonglong sig = obj->getSignature();
    switch (sig){
    case HKB_STATE_MACHINE:
    {
        hkbStateMachine *ptr = static_cast<hkbStateMachine *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        for (int i = 0; i < ptr->states.size(); i++){
            if (!drawBehaviorGraph(ptr->states.at(i), icon)){
                return false;
            }
        }
        break;
    }
    case HKB_STATE_MACHINE_STATE_INFO:
    {
        hkbStateMachineStateInfo *ptr = static_cast<hkbStateMachineStateInfo *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        if (!drawBehaviorGraph(ptr->generator, icon)){
            return false;
        }
        break;
    }
    case BS_SYNCHRONIZED_CLIP_GENERATOR:
    {
        BSSynchronizedClipGenerator *ptr = static_cast<BSSynchronizedClipGenerator *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        if (!drawBehaviorGraph(ptr->pClipGenerator, icon)){
            return false;
        }
        break;
    }
    case BS_I_STATE_TAGGING_GENERATOR:
    {
        BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        if (!drawBehaviorGraph(ptr->pDefaultGenerator, icon)){
            return false;
        }
        break;
    }
    case BS_BONE_SWITCH_GENERATOR:
    {
        BSBoneSwitchGenerator *ptr = static_cast<BSBoneSwitchGenerator *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        if (!drawBehaviorGraph(ptr->pDefaultGenerator, icon)){
            return false;
        }
        for (int i = 0; i < ptr->ChildrenA.size(); i++){
            if (!drawBehaviorGraph(static_cast<BSBoneSwitchGeneratorBoneData *>(ptr->ChildrenA.at(i).data())->pGenerator, icon)){
                return false;
            }
        }
        break;
    }
    case HKB_MANUAL_SELECTOR_GENERATOR:
    {
        hkbManualSelectorGenerator *ptr = static_cast<hkbManualSelectorGenerator *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        for (int i = 0; i < ptr->generators.size(); i++){
            if (!drawBehaviorGraph(ptr->generators.at(i), icon)){
                return false;
            }
        }
        break;
    }
    case HKB_BLENDER_GENERATOR:
    {
        hkbBlenderGenerator *ptr = static_cast<hkbBlenderGenerator *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        for (int i = 0; i < ptr->children.size(); i++){
            if (!drawBehaviorGraph(static_cast<hkbBlenderGeneratorChild *>(ptr->children.at(i).data())->generator, icon)){
                return false;
            }
        }
        break;
    }
    case HKB_MODIFIER_GENERATOR:
    {
        hkbModifierGenerator *ptr = static_cast<hkbModifierGenerator *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        //if (!drawBehaviorGraph(ptr->modifier, icon)){return false;}
        if (!drawBehaviorGraph(ptr->generator, icon)){
            return false;
        }
        break;
    }
    case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
    {
        BSCyclicBlendTransitionGenerator *ptr = static_cast<BSCyclicBlendTransitionGenerator *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        if (!drawBehaviorGraph(ptr->pBlenderGenerator, icon)){
            return false;
        }
        break;
    }
    case HKB_BEHAVIOR_REFERENCE_GENERATOR:
    {
        hkbBehaviorReferenceGenerator *ptr = static_cast<hkbBehaviorReferenceGenerator *>(obj.data());
        positionIcon(obj, ptr, parentIcon);
        break;
    }
    case HKB_CLIP_GENERATOR:
    {
        hkbClipGenerator *ptr = static_cast<hkbClipGenerator *>(obj.data());
        positionIcon(obj, ptr, parentIcon);
        break;
    }
    case HKB_BEHAVIOR_GRAPH:
    {
        hkbBehaviorGraph *ptr = static_cast<hkbBehaviorGraph *>(obj.data());
        GeneratorIcon *icon = new GeneratorIcon(obj, ptr->name);
        behaviorGS->addItem(icon);
        if (!drawBehaviorGraph(ptr->rootGenerator, icon)){
            return false;
        }
        break;
    }
    case HK_ROOT_LEVEL_CONTAINER:
    {
        hkRootLevelContainer *ptr = static_cast<hkRootLevelContainer *>(obj.data());
        for (int i = 0; i < ptr->namedVariants.size(); i++){
            //Unsafe???
            if (!drawBehaviorGraph(ptr->namedVariants.at(i).variant)){
                return false;
            }
        }
        break;
    }
    default:
        break;
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
    behaviorGraphViewIV->drawBehaviorGraph(hkxFile->getRootObject());
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
