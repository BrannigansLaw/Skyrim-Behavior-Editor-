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
#include <QGraphicsSceneMouseEvent>

/**
 * GeneratorIcon
 */

QRectF GeneratorIcon::boundingRectangle = QRectF(0, 0, 400, 50);
QRectF GeneratorIcon::button = QRectF(360, 0, 40, 50);
QLineF GeneratorIcon::vert = QLineF(380, 5, 380, 45);
QLineF GeneratorIcon::horiz = QLineF(365, 25, 395, 25);
QRectF GeneratorIcon::textRec = QRectF(120, 10, 240, 40);
//QRadialGradient GeneratorIcon::rGrad = QRadialGradient();
QFont GeneratorIcon::font = QFont("Helvetica [Cronyx]", 9);
//QPen GeneratorIcon::textPen = QPen(Qt::white);
QPen GeneratorIcon::pen = QPen(QBrush(Qt::black), 2);
QBrush GeneratorIcon::brush = QBrush(Qt::green);
QBrush GeneratorIcon::buttonBrush = QBrush(Qt::gray);
QPolygonF GeneratorIcon::polygon = QPolygonF();
QRectF GeneratorIcon::ellipse = QRectF(40, 5, 40, 40);
QRectF GeneratorIcon::square = QRectF(40, 5, 40, 40);
QPolygonF GeneratorIcon::arrowHead = QPolygonF();

void GeneratorIcon::updateStaticMembers(){
    arrowHead << QPointF(365, 25) << QPointF(380, 5) << QPointF(395, 25);
    polygon << QPointF(boundingRectangle.topLeft().x() + boundingRectangle.width() * 0.1, boundingRectangle.topLeft().y() + \
                          boundingRectangle.height() * 0.1)\
                << QPointF(boundingRectangle.topLeft().x() + boundingRectangle.width() * 0.1, boundingRectangle.topLeft().y() + \
                          boundingRectangle.height() * 0.9)\
                << QPointF(boundingRectangle.topLeft().x() + boundingRectangle.width() * 0.2, boundingRectangle.topLeft().y() + \
                          boundingRectangle.height() * 0.5)
                << QPointF(boundingRectangle.topLeft().x() + boundingRectangle.width() * 0.1, boundingRectangle.topLeft().y() + \
                          boundingRectangle.height() * 0.1);
    /*rGrad.setCenter(boundingRectangle.topLeft());
    rGrad.setCenterRadius(boundingRectangle.width());
    rGrad.setColorAt(0.0, Qt::white);
    rGrad.setColorAt(1.0, Qt::black);*/
}

GeneratorIcon::GeneratorIcon(const HkObjectExpSharedPtr & d, const QString & s, GeneratorIcon * par)
    : data(d),\
      name(s),\
      isExpanded(true),\
      parent(par),\
      linkToParent(NULL)
{
    textPen.setColor(Qt::white);
    rGrad.setCenter(boundingRect().topLeft());
    rGrad.setCenterRadius(boundingRect().width());
    rGrad.setColorAt(0.0, Qt::white);
    rGrad.setColorAt(1.0, Qt::black);
    if (data->getSignature() == HKB_BLENDER_GENERATOR || \
            data->getSignature() == BS_BONE_SWITCH_GENERATOR || \
            data->getSignature() == HKB_POSE_MATCHING_GENERATOR)
    {
        path.addRect(square);
    }else if (data->getSignature() == HKB_CLIP_GENERATOR)
    {        path.addEllipse(ellipse);
    }else{
        path.addPolygon(polygon);
    }
    if (parent){
        parent->children.append(this);
        linkToParent = new QGraphicsLineItem(
                    parent->pos().x() + 1.0*parent->boundingRect().width(), parent->pos().y() + 1.0*parent->boundingRect().height(),
                    parent->pos().x() + 1.5*parent->boundingRect().width(), pos().y() + 2*boundingRect().height()
                    );
        setPos(parent->pos().x() + 1.5*parent->boundingRect().width(), pos().y() + 2*boundingRect().height());
    }
}

void GeneratorIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setFont(font);
    painter->setPen(pen);
    painter->setBrush(rGrad);
    painter->drawRoundedRect(boundingRect(), 4, 4);
    painter->setBrush(brush);
    painter->drawPath(path);
    painter->setPen(textPen);
    painter->drawText(textRec, name);
    painter->setPen(pen);
    painter->setBrush(buttonBrush);
    if (data->getSignature() != HKB_CLIP_GENERATOR && data->getSignature() != HKB_BEHAVIOR_REFERENCE_GENERATOR){
        hkbGenerator *ptr = static_cast<hkbGenerator *>(data.data());
        painter->drawRect(button);
        if (ptr->icons.isEmpty()){
            painter->drawLine(horiz);
            if (!isExpanded){
                painter->drawLine(vert);
            }
        }else if (ptr->icons.first() == this){
            painter->drawLine(horiz);
            if (!isExpanded){
                painter->drawLine(vert);
            }
        }else{
            painter->drawPolygon(arrowHead);
            painter->drawLine(vert);
        }
    }

}

void GeneratorIcon::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        if (button.contains(event->lastPos())){
            hkbGenerator *ptr = static_cast<hkbGenerator *>(data.data());
            if (!ptr->icons.isEmpty() && ptr->icons.first() != this){
                if (scene() && !scene()->views().isEmpty()){
                    if (!ptr->icons.first()->isVisible() && !scene()->items(Qt::AscendingOrder).isEmpty()){
                        expandBranch(static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first()), true);
                    }
                    scene()->views().first()->centerOn(ptr->icons.first());
                    BehaviorGraphView *view = static_cast<BehaviorGraphView *>(scene()->views().first());
                    GeneratorIcon *icon = static_cast<BehaviorGraphView *>(scene()->views().first())->selectedIcon;
                    view->selectedIcon = ptr->icons.first();
                    if (icon){
                        icon->rGrad.setColorAt(1.0, Qt::black);
                        icon->textPen.setColor(Qt::white);
                    }
                    ptr->icons.first()->rGrad.setColorAt(1.0, Qt::green);
                    ptr->icons.first()->textPen.setColor(Qt::blue);
                    scene()->update();
                }
            }
            if (isExpanded){
                contractBranch(this);
                isExpanded = false;
            }else{
                isExpanded = true;
                expandBranch(this);
            }
            update(boundingRect());
            if (scene() && !scene()->views().isEmpty() && !scene()->items(Qt::AscendingOrder).isEmpty()){
                static_cast<BehaviorGraphView *>(scene()->views().first())->repositionIcons(static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first()));
            }
        }else{
            if (scene() && !scene()->views().isEmpty()){
                BehaviorGraphView *view = static_cast<BehaviorGraphView *>(scene()->views().first());
                GeneratorIcon *icon = static_cast<BehaviorGraphView *>(scene()->views().first())->selectedIcon;
                if (icon != this){
                    view->selectedIcon = this;
                    if (icon){
                        icon->rGrad.setColorAt(1.0, Qt::black);
                        icon->textPen.setColor(Qt::white);
                    }
                    rGrad.setColorAt(1.0, Qt::green);
                    textPen.setColor(Qt::blue);
                    scene()->update();
                }
            }
        }
    }
}

bool GeneratorIcon::getLastIconY(GeneratorIcon *parent, qreal &lastIconY){
    if (!parent){
        return false;
    }
    if (parent != this && parent->isVisible()){
        QPointF temp = parent->pos();
        lastIconY = temp.y() + parent->boundingRect().height();
        for (qint16 i = 0; i < parent->children.size(); i++){
            if (getLastIconY(parent->children.at(i), lastIconY)){
                i = parent->children.size();
            }
        }
    }else{
        return true;
    }
    return false;
}

void GeneratorIcon::updatePosition(){
    if (!parent || !isVisible()){
        return;
    }
    qreal iconY = 0;
    getLastIconY(parent, iconY);
    qreal x = parent->pos().x() + 1.5*(parent->boundingRect().width());
    qreal yt = iconY + parent->boundingRect().height();
    setPos(x , yt);
    QLineF line(parent->pos().x() + 1.0*parent->boundingRect().width(), parent->pos().y() + 1.0*parent->boundingRect().height(),\
                 parent->pos().x() + 1.5*parent->boundingRect().width(), yt + parent->boundingRect().height());
    if (linkToParent){
        linkToParent->setLine(line);
    } else {
        linkToParent = new QGraphicsLineItem(line);
    }
}

void GeneratorIcon::contractBranch(GeneratorIcon * icon, bool contractAll){
    if (!icon){
        return;
    }
    for (int i = 0; i < icon->children.size(); i++){
        icon->children.at(i)->setVisible(false);
        if (icon->children.at(i)->linkToParent){
            icon->children.at(i)->linkToParent->setVisible(false);
        }
        if (contractAll){
            icon->children.at(i)->isExpanded = false;
            contractBranch(icon->children.at(i), true);
        }else{
            contractBranch(icon->children.at(i));
        }
    }
}

void GeneratorIcon::expandBranch(GeneratorIcon * icon, bool expandAll){
    if (!icon){
        return;
    }
    for (int i = 0; i < icon->children.size(); i++){
        icon->children.at(i)->setVisible(true);
        if (icon->children.at(i)->linkToParent){
            icon->children.at(i)->linkToParent->setVisible(true);
        }
        if (expandAll || (icon->children.at(i)->isExpanded)){
            expandBranch(icon->children.at(i));
        }
    }
}

/**
 * BehaviorGraphView
 */

BehaviorGraphView::BehaviorGraphView(BehaviorFile * file)
    : behavior(file),\
      behaviorGS(new QGraphicsScene),\
      selectedIcon(NULL)
{
    setScene(behaviorGS);
    GeneratorIcon::updateStaticMembers();
}

void BehaviorGraphView::repositionIcons(GeneratorIcon *icon){
    for (int i = 0; i < icon->children.size(); i++){
        icon->children.at(i)->updatePosition();
        repositionIcons(icon->children.at(i));
    }
}

void BehaviorGraphView::wheelEvent(QWheelEvent *event){
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    if (event->delta() > 0){
        scale(1.2, 1.2);
    }else{
        scale(0.8, 0.8);
    }
}

void BehaviorGraphView::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        this->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    QGraphicsView::mousePressEvent(event);
}

void BehaviorGraphView::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        this->setDragMode(QGraphicsView::NoDrag);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

bool BehaviorGraphView::drawBehaviorGraph(){
    QList <HkObjectExpSharedPtr> objects;
    QList <GeneratorIcon *> parentIcons;
    int result;
    objects.append(behavior->getRootObject());
    while (!objects.isEmpty()){
        qulonglong sig = objects.last()->getSignature();
        switch (sig){
        case HKB_STATE_MACHINE:
        {
            hkbStateMachine *ptr = static_cast<hkbStateMachine *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons);
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
            result = initializeIcons(ptr, objects, parentIcons);
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
            result = initializeIcons(ptr, objects, parentIcons);
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
            result = initializeIcons(ptr, objects, parentIcons);
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
            result = initializeIcons(ptr, objects, parentIcons);
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
            result = initializeIcons(ptr, objects, parentIcons);
            if (result == 0){
                for (int i = ptr->children.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbBlenderGeneratorChild *>(ptr->children.at(i).data())->generator);
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_POSE_MATCHING_GENERATOR:
        {
            hkbPoseMatchingGenerator *ptr = static_cast<hkbPoseMatchingGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons);
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
            result = initializeIcons(ptr, objects, parentIcons);
            if (result == 0){
                //objects.append(ptr->modifier);
                objects.append(ptr->generator);
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_OFFSET_ANIMATION_GENERATOR:
        {
            BSOffsetAnimationGenerator *ptr = static_cast<BSOffsetAnimationGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
                objects.append(ptr->pOffsetClipGenerator);
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        {
            BSCyclicBlendTransitionGenerator *ptr = static_cast<BSCyclicBlendTransitionGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons);
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
            if (initializeIcons(ptr, objects, parentIcons) == -1){
                return false;
            }
            break;
        }
        case HKB_CLIP_GENERATOR:
        {
            hkbClipGenerator *ptr = static_cast<hkbClipGenerator *>(objects.last().data());
            if (initializeIcons(ptr, objects, parentIcons) == -1){
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
    if (!hkxFile){
        hkxFile = new BehaviorFile(name);
        behaviorGraphViewIV = new BehaviorGraphView(hkxFile);
        if (!behaviorGraphViewIV->drawBehaviorGraph()){
            //
        }
    }else{
        delete hkxFile;
        delete behaviorGraphViewIV;
        hkxFile = new BehaviorFile(name);
        behaviorGraphViewIV = new BehaviorGraphView(hkxFile);
        if (!behaviorGraphViewIV->drawBehaviorGraph()){
            //
        }
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
