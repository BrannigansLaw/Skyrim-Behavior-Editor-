#include "mainwindow.h"
#include "fileselectwindow.h"
#include "hkxfile.h"
#include "generators.h"
#include "behaviorgraphui.h"

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
}

GeneratorIcon::GeneratorIcon(const HkObjectExpSharedPtr & d, const QString & s, GeneratorIcon * par)
    : data(d),
      name(s),
      isExpanded(true),
      parent(par),
      linkToParent(NULL)
{
    textPen.setColor(Qt::white);
    rGrad.setCenter(boundingRect().topLeft());
    rGrad.setCenterRadius(boundingRect().width());
    rGrad.setColorAt(0.0, Qt::white);
    rGrad.setColorAt(1.0, Qt::black);
    if (data->getSignature() == HKB_BLENDER_GENERATOR || data->getSignature() == BS_BONE_SWITCH_GENERATOR || data->getSignature() == HKB_POSE_MATCHING_GENERATOR){
        path.addRect(square);
    }else if (data->getSignature() == HKB_CLIP_GENERATOR){
        path.addEllipse(ellipse);
    }else{
        path.addPolygon(polygon);
    }
    if (parent){
        parent->children.append(this);
        linkToParent = new QGraphicsLineItem(parent->pos().x() + 1.0*parent->boundingRect().width(), parent->pos().y() + 1.0*parent->boundingRect().height(),
                                             parent->pos().x() + 1.5*parent->boundingRect().width(), pos().y() + 2*boundingRect().height());
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
    hkbGenerator *ptr = static_cast<hkbGenerator *>(data.data());
    if (!scene() || scene()->views().isEmpty() || scene()->items(Qt::AscendingOrder).isEmpty()){
        return;
    }
    BehaviorGraphView *view = static_cast<BehaviorGraphView *>(scene()->views().first());
    GeneratorIcon *icon = static_cast<BehaviorGraphView *>(scene()->views().first())->selectedIcon;
    GeneratorIcon *firstSceneIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first());
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton){
        if (icon != this){
            view->selectedIcon = this;
            if (icon){
                icon->rGrad.setColorAt(1.0, Qt::black);
                icon->textPen.setColor(Qt::white);
            }
        }
        rGrad.setColorAt(1.0, Qt::green);
        textPen.setColor(Qt::blue);
        if (button.contains(event->lastPos())){
            if (ptr->icons.isEmpty()){
                return;
            }
            if (ptr->icons.first() != this){
                if (!ptr->icons.first()->isVisible()){
                    expandBranch(firstSceneIcon, true);
                }
                view->centerOn(ptr->icons.first());
                view->selectedIcon = ptr->icons.first();
                ptr->icons.first()->rGrad.setColorAt(1.0, Qt::green);
                ptr->icons.first()->textPen.setColor(Qt::blue);
                rGrad.setColorAt(1.0, Qt::black);
                textPen.setColor(Qt::white);
            }else{
                if (isExpanded){
                    contractBranch(this);
                    isExpanded = false;
                }else{
                    isExpanded = true;
                    expandBranch(this);
                }
            }
            view->repositionIcons(firstSceneIcon);
        }
        if (event->button() == Qt::RightButton){
            view->popUpMenuRequested(view->mapFromScene(event->scenePos()), data);
        }
    }
    scene()->update();
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
    setPos(x, yt);
    QLineF line(parent->pos().x() + 1.0*parent->boundingRect().width(), parent->pos().y() + 1.0*parent->boundingRect().height(),\
                 parent->pos().x() + 1.5*parent->boundingRect().width(), yt + parent->boundingRect().height());
    if (linkToParent){
        linkToParent->setLine(line);
    }else {
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
    icon->isExpanded = true;
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
    : behavior(file),
      behaviorGS(new QGraphicsScene),
      selectedIcon(NULL),
      contextMenu(new QMenu(this)),
      wrapGeneratorMenu(new QMenu("Wrap inside Generator:", contextMenu)),
      wrapSMAct(new QAction("State Machine", wrapGeneratorMenu)),
      wrapMSGAct(new QAction("Manual Selector Generator", wrapGeneratorMenu)),
      wrapMGAct(new QAction("Modifier Generator", wrapGeneratorMenu)),
      wrapBSISTGAct(new QAction("BS iState Tagging Generator", wrapGeneratorMenu)),
      wrapBSSCGAct(new QAction("BS Synchronized Clip Generator", wrapGeneratorMenu)),
      wrapBSOAGAct(new QAction("BS Offset Animation Generator", wrapGeneratorMenu)),
      wrapBSCBTGAct(new QAction("BS Cyclic Blend Transition Generator", wrapGeneratorMenu)),
      wrapPMGAct(new QAction("Pose Matching Generator", wrapGeneratorMenu)),
      wrapBlenderMenu(new QMenu("Wrap inside Blend:", contextMenu)),
      wrapBGAct(new QAction("Blender Generator", wrapBlenderMenu)),
      wrapBSBSGAct(new QAction("BS Bone Switch Generator", wrapBlenderMenu))
{
    setScene(behaviorGS);
    setContextMenuPolicy(Qt::CustomContextMenu);
    GeneratorIcon::updateStaticMembers();
    contextMenu->addMenu(wrapGeneratorMenu);
    wrapGeneratorMenu->addAction(wrapSMAct);
    wrapGeneratorMenu->addAction(wrapMSGAct);
    wrapGeneratorMenu->addAction(wrapMGAct);
    wrapGeneratorMenu->addAction(wrapBSISTGAct);
    wrapGeneratorMenu->addAction(wrapBSSCGAct);
    wrapGeneratorMenu->addAction(wrapBSOAGAct);
    wrapGeneratorMenu->addAction(wrapBSCBTGAct);
    wrapGeneratorMenu->addAction(wrapPMGAct);
    contextMenu->addMenu(wrapBlenderMenu);
    wrapBlenderMenu->addAction(wrapBGAct);
    wrapBlenderMenu->addAction(wrapBSBSGAct);
    connect(wrapSMAct, SIGNAL(triggered()), this, SLOT(wrapStateMachine()));
}

void BehaviorGraphView::wrapStateMachine(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbStateMachine *stateMachine = new hkbStateMachine(behavior);
        behavior->generators.append(HkObjectExpSharedPtr(stateMachine));
        hkbStateMachineStateInfo *state = new hkbStateMachineStateInfo(behavior);
        behavior->generators.append(HkObjectExpSharedPtr(state));
        stateMachine->states.append(behavior->generators.last());
        qulonglong sig = selectedIcon->parent->data->getSignature();
        bool drawIcon = true;
        switch (sig){
        case HKB_STATE_MACHINE:
        {
            const hkbStateMachine *parent = static_cast<const hkbStateMachine *>(selectedIcon->parent->data.constData());
            hkbStateMachineStateInfo *parentChild;
            for (int i = 0; i < parent->states.size(); i++){
                parentChild = static_cast<hkbStateMachineStateInfo *>(parent->states.at(i).data());
                if (parentChild->generator == selectedIcon->data){
                    state->generator = parentChild->generator;
                    parentChild->generator = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(parentChild->generator, stateMachine, selectedIcon->parent));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *parent = static_cast<hkbManualSelectorGenerator *>(selectedIcon->parent->data.data());
            for (int i = 0; i < parent->generators.size(); i++){
                if (parent->generators.at(i) == selectedIcon->data){
                    state->generator = parent->generators.at(i);
                    parent->generators[i] = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(parent->generators.at(i), stateMachine, selectedIcon->parent));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case HKB_BLENDER_GENERATOR:
        {
            const hkbBlenderGenerator *parent = static_cast<const hkbBlenderGenerator *>(selectedIcon->parent->data.constData());
            hkbBlenderGeneratorChild *parentChild;
            for (int i = 0; i < parent->children.size(); i++){
                parentChild = static_cast<hkbBlenderGeneratorChild *>(parent->children.at(i).data());
                if (parentChild->generator == selectedIcon->data){
                    state->generator = parentChild->generator;
                    parentChild->generator = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(parentChild->generator, stateMachine, selectedIcon->parent));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case BS_I_STATE_TAGGING_GENERATOR:
        {
            BSiStateTaggingGenerator *parent = static_cast<BSiStateTaggingGenerator *>(selectedIcon->parent->data.data());
            if (parent->pDefaultGenerator == selectedIcon->data){
                state->generator = parent->pDefaultGenerator;
                parent->pDefaultGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(parent->pDefaultGenerator, stateMachine, selectedIcon->parent));
            }
            break;
        }
        case BS_BONE_SWITCH_GENERATOR:
        {
            BSBoneSwitchGenerator *parent = static_cast<BSBoneSwitchGenerator *>(selectedIcon->parent->data.data());
            BSBoneSwitchGeneratorBoneData *parentChild;
            if (parent->pDefaultGenerator == selectedIcon->data){
                state->generator = parent->pDefaultGenerator;
                parent->pDefaultGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(parent->pDefaultGenerator, stateMachine, selectedIcon->parent));
                drawIcon = false;
            }
            for (int i = 0; i < parent->ChildrenA.size(); i++){
                parentChild = static_cast<BSBoneSwitchGeneratorBoneData *>(parent->ChildrenA.at(i).data());
                if (parentChild->pGenerator == selectedIcon->data){
                    state->generator = parentChild->pGenerator;
                    parentChild->pGenerator = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(parentChild->pGenerator, stateMachine, selectedIcon->parent));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case HKB_MODIFIER_GENERATOR:
        {
            hkbModifierGenerator *parent = static_cast<hkbModifierGenerator *>(selectedIcon->parent->data.data());
            if (parent->generator == selectedIcon->data){
                state->generator = parent->generator;
                parent->generator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(parent->generator, stateMachine, selectedIcon->parent));
            }
            break;
        }
        case BS_OFFSET_ANIMATION_GENERATOR:
        {
            BSOffsetAnimationGenerator *parent = static_cast<BSOffsetAnimationGenerator *>(selectedIcon->parent->data.data());
            if (parent->pDefaultGenerator == selectedIcon->data){
                state->generator = parent->pDefaultGenerator;
                parent->pDefaultGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(parent->pDefaultGenerator, stateMachine, selectedIcon->parent));
            }
            break;
        }
        case HKB_POSE_MATCHING_GENERATOR:
        {
            const hkbPoseMatchingGenerator *parent = static_cast<const hkbPoseMatchingGenerator *>(selectedIcon->parent->data.constData());
            hkbBlenderGeneratorChild *parentChild;
            for (int i = 0; i < parent->children.size(); i++){
                parentChild = static_cast<hkbBlenderGeneratorChild *>(parent->children.at(i).data());
                if (parentChild->generator == selectedIcon->data){
                    state->generator = parentChild->generator;
                    parentChild->generator = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(parentChild->generator, stateMachine, selectedIcon->parent));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case HKB_BEHAVIOR_GRAPH:
        {
            hkbBehaviorGraph *parent = static_cast<hkbBehaviorGraph *>(selectedIcon->parent->data.data());
            if (parent->rootGenerator == selectedIcon->data){
                state->generator = parent->rootGenerator;
                parent->rootGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(parent->rootGenerator, stateMachine, selectedIcon->parent));
            }
            break;
        }
        default:
            delete stateMachine;
            delete state;
            return;
        }
        behavior->generators.append(HkObjectExpSharedPtr(stateMachine));
        behavior->generators.append(HkObjectExpSharedPtr(state));
        if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty()){
            GeneratorIcon *behaviorGraphIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first());
            behaviorGraphIcon->expandBranch(behaviorGraphIcon, true);
            repositionIcons(behaviorGraphIcon);
        }
    }
}

void BehaviorGraphView::popUpMenuRequested(const QPoint &pos, const HkObjectExpSharedPtr & obj){
    qulonglong sig = obj->getSignature();
    if (sig == HKB_BLENDER_GENERATOR){
        wrapBSCBTGAct->setVisible(true);
    }else if (sig == HKB_CLIP_GENERATOR){
        wrapBSSCGAct->setVisible(true);
        wrapBSOAGAct->setVisible(true);
    }else{
        wrapBSCBTGAct->setVisible(false);
        wrapBSSCGAct->setVisible(false);
        wrapBSOAGAct->setVisible(false);
    }
    contextMenu->popup(viewport()->mapToGlobal(pos));
}

void BehaviorGraphView::repositionIcons(GeneratorIcon *icon){
    if (!icon){
        return;
    }
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
    QVector <short> objectChildCount;
    int result;
    objects.append(behavior->getRootObject());
    while (!objects.isEmpty()){
        qulonglong sig = objects.last()->getSignature();
        switch (sig){
        case HKB_STATE_MACHINE:
        {
            hkbStateMachine *ptr = static_cast<hkbStateMachine *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->states.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbStateMachineStateInfo *>(ptr->states.at(i).data())->generator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_SYNCHRONIZED_CLIP_GENERATOR:
        {
            BSSynchronizedClipGenerator *ptr = static_cast<BSSynchronizedClipGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pClipGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_I_STATE_TAGGING_GENERATOR:
        {
            BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_BONE_SWITCH_GENERATOR:
        {
            BSBoneSwitchGenerator *ptr = static_cast<BSBoneSwitchGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
                objectChildCount.last()++;
                for (int i = ptr->ChildrenA.size() - 1; i >= 0; i--){
                    objects.append(static_cast<BSBoneSwitchGeneratorBoneData *>(ptr->ChildrenA.at(i).data())->pGenerator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *ptr = static_cast<hkbManualSelectorGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->generators.size() - 1; i >= 0; i--){
                    objects.append(ptr->generators.at(i));
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_BLENDER_GENERATOR:
        {
            hkbBlenderGenerator *ptr = static_cast<hkbBlenderGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->children.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbBlenderGeneratorChild *>(ptr->children.at(i).data())->generator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_POSE_MATCHING_GENERATOR:
        {
            hkbPoseMatchingGenerator *ptr = static_cast<hkbPoseMatchingGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->children.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbBlenderGeneratorChild *>(ptr->children.at(i).data())->generator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_MODIFIER_GENERATOR:
        {
            hkbModifierGenerator *ptr = static_cast<hkbModifierGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                //objects.append(ptr->modifier);
                //objectChildCount.last()++;
                objects.append(ptr->generator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_OFFSET_ANIMATION_GENERATOR:
        {
            BSOffsetAnimationGenerator *ptr = static_cast<BSOffsetAnimationGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
                objectChildCount.last()++;
                objects.append(ptr->pOffsetClipGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        {
            BSCyclicBlendTransitionGenerator *ptr = static_cast<BSCyclicBlendTransitionGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pBlenderGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_BEHAVIOR_REFERENCE_GENERATOR:
        {
            hkbBehaviorReferenceGenerator *ptr = static_cast<hkbBehaviorReferenceGenerator *>(objects.last().data());
            if (initializeIcons(ptr, objects, parentIcons, objectChildCount) == -1){
                return false;
            }
            break;
        }
        case HKB_CLIP_GENERATOR:
        {
            hkbClipGenerator *ptr = static_cast<hkbClipGenerator *>(objects.last().data());
            if (initializeIcons(ptr, objects, parentIcons, objectChildCount) == -1){
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
            objectChildCount.append(1);
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
