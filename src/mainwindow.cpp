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

void BehaviorGraphView::drawBehaviorGraph(const HkObjectExpSharedPtr & obj, GeneratorIcon * parentIcon){
    if (!obj.data()){
        return;
    }
    qulonglong sig = obj->getSignature();
    switch (sig){
    case HKB_STATE_MACHINE:
    {
        hkbStateMachine *ptr = static_cast<hkbStateMachine *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        for (int i = 0; i < ptr->states.size(); i++){
            drawBehaviorGraph(ptr->states.at(i), icon);
        }
        break;
    }
    case HKB_STATE_MACHINE_STATE_INFO:
    {
        hkbStateMachineStateInfo *ptr = static_cast<hkbStateMachineStateInfo *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        drawBehaviorGraph(ptr->generator, icon);
        break;
    }
    case HKB_MANUAL_SELECTOR_GENERATOR:
    {
        hkbManualSelectorGenerator *ptr = static_cast<hkbManualSelectorGenerator *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        for (int i = 0; i < ptr->generators.size(); i++){
            drawBehaviorGraph(ptr->generators.at(i), icon);
        }
        break;
    }
    case HKB_BLENDER_GENERATOR_CHILD:
    {
        hkbBlenderGeneratorChild *ptr = static_cast<hkbBlenderGeneratorChild *>(obj.data());
        if (!parentIcon){
            return;
        }
        if (behaviorGS->items().isEmpty()){
            return;
        }
        GeneratorIcon *icon = new GeneratorIcon(obj, "");
        //icon->setParentItem(parentIcon);
        GeneratorIcon *lastIcon = reinterpret_cast<GeneratorIcon *>(behaviorGS->items(Qt::AscendingOrder).last());
        behaviorGS->addItem(icon);
        icon->setPos(parentIcon->pos().x() + 1.5*parentIcon->boundingRect().width(), lastIcon->pos().y() + 2*lastIcon->boundingRect().height());
        drawBehaviorGraph(ptr->generator, icon);
        break;
    }
    case HKB_BLENDER_GENERATOR:
    {
        hkbBlenderGenerator *ptr = static_cast<hkbBlenderGenerator *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        for (int i = 0; i < ptr->children.size(); i++){
            drawBehaviorGraph(ptr->children.at(i), icon);
        }
        break;
    }
    case HKB_MODIFIER_GENERATOR:
    {
        hkbModifierGenerator *ptr = static_cast<hkbModifierGenerator *>(obj.data());
        GeneratorIcon *icon = positionIcon(obj, ptr, parentIcon);
        //drawBehaviorGraph(ptr->modifier, icon);
        drawBehaviorGraph(ptr->generator, icon);
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
        drawBehaviorGraph(ptr->rootGenerator, icon);
        break;
    }
    case HK_ROOT_LEVEL_CONTAINER:
    {
        hkRootLevelContainer *ptr = static_cast<hkRootLevelContainer *>(obj.data());
        for (int i = 0; i < ptr->namedVariants.size(); i++){
            //Unsafe???
            drawBehaviorGraph(ptr->namedVariants.at(i).variant);
        }
        break;
    }
    default:
        break;
    }
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
