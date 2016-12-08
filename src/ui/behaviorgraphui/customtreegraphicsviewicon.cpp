#include "src/ui/mainwindow.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/hkxclasses/behavior/modifiers/hkbmodifier.h"
#include "src/ui/customtreegraphicsview.h"
#include "src/ui/behaviorgraphui/behaviorgraphview.h"
#include "src/hkxclasses/hkxobject.h"
#include "customtreegraphicsviewicon.h"
#include "src/ui//hkxclassesui/hkdataui.h"

#include <QBoxLayout>
#include <QMenuBar>
#include <QFile>
#include <QSettings>
#include <QGroupBox>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QSize>

#define BOUND_RECT_LEN 200
#define BOUND_RECT_HEIGHT 40
#define X_BOUNDARY BOUND_RECT_LEN*0.1
#define Y_BOUNDARY BOUND_RECT_HEIGHT*0.1
#define BUTTON_RECT_LEN 40
#define BUTTON_RECT_HEIGHT BOUND_RECT_HEIGHT
#define TEXT_RECT_LEN BOUND_RECT_LEN*0.8
#define TEXT_RECT_HEIGHT 20

#define ICON_SPACING_X 1.2
#define ICON_SPACING_Y 1.0
#define ICON_SPACING 0.5

QRectF CustomTreeGraphicsViewIcon::boundingRectangle = QRectF(0, 0, 400, 50);
QRectF CustomTreeGraphicsViewIcon::button = QRectF(360, 0, 40, 50);
QLineF CustomTreeGraphicsViewIcon::vert = QLineF(380, 5, 380, 45);
QLineF CustomTreeGraphicsViewIcon::horiz = QLineF(365, 25, 395, 25);
QRectF CustomTreeGraphicsViewIcon::textRec = QRectF(120, 10, 240, 40);
//QRadialGradient CustomTreeGraphicsViewIcon::rGrad = QRadialGradient();
QFont CustomTreeGraphicsViewIcon::font = QFont("Helvetica [Cronyx]", 9);
//QPen CustomTreeGraphicsViewIcon::textPen = QPen(Qt::white);
//QPen CustomTreeGraphicsViewIcon::pen = QPen(QBrush(Qt::black), 2);
QBrush CustomTreeGraphicsViewIcon::brush = QBrush(Qt::green);
QBrush CustomTreeGraphicsViewIcon::buttonBrush = QBrush(Qt::gray);
QPolygonF CustomTreeGraphicsViewIcon::polygon = QPolygonF();
QRectF CustomTreeGraphicsViewIcon::ellipse = QRectF(40, 5, 40, 40);
QRectF CustomTreeGraphicsViewIcon::square = QRectF(40, 5, 40, 40);
QPolygonF CustomTreeGraphicsViewIcon::arrowHead = QPolygonF();

void CustomTreeGraphicsViewIcon::updateStaticMembers(){
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

CustomTreeGraphicsViewIcon::CustomTreeGraphicsViewIcon(DataIconManager *d, const QString & s, CustomTreeGraphicsViewIcon * par)
    : data(HkxObjectExpSharedPtr(d)),
      name(s),
      isExpanded(true),
      parent(par),
      linkToParent(NULL)
{
    if (data->getType() == HkxObject::TYPE_GENERATOR){
        static_cast<DataIconManager *>(data.data())->icons.append(this);
        rGrad.setColorAt(0.0, Qt::white);
        rGrad.setColorAt(1.0, Qt::black);
    }else if (data->getType() == HkxObject::TYPE_MODIFIER){
        static_cast<hkbModifier *>(data.data())->icons.append(this);
        rGrad.setColorAt(0.0, Qt::white);
        rGrad.setColorAt(1.0, Qt::blue);
    }
    pen = QPen(QBrush(Qt::black), 2);
    textPen.setColor(Qt::white);
    rGrad.setCenter(boundingRect().topLeft());
    rGrad.setCenterRadius(boundingRect().width());
    if (data->getSignature() == HKB_BLENDER_GENERATOR || data->getSignature() == BS_BONE_SWITCH_GENERATOR || data->getSignature() == HKB_POSE_MATCHING_GENERATOR){
        path.addRect(square);
    }else if (data->getSignature() == HKB_CLIP_GENERATOR){
        path.addEllipse(ellipse);
    }else{
        path.addPolygon(polygon);
    }
    if (parent){
        qreal lastY = 0;
        getLastIconY(parent, lastY);
        if (!parent->children.contains(this)){
            int index = static_cast<DataIconManager *>(parent->data.data())->getIndexToInsertIcon(data.data());
            if (index > -1 && index < parent->children.size()){
                parent->children.insert(index, this);
            }else{
                parent->children.append(this);
            }
        }
        /*int index = parent->children.indexOf(this);
        if (index == -1){
            parent->children.append(this);
        }*/
        linkToParent = new QGraphicsLineItem(parent->pos().x() + parent->boundingRect().width(), parent->pos().y() + 1.0*parent->boundingRect().height(),
                                             parent->pos().x() + ICON_SPACING_X*parent->boundingRect().width(), lastY + ICON_SPACING*boundingRect().height());
        setPos(parent->pos().x() + ICON_SPACING_X*parent->boundingRect().width(), lastY + ICON_SPACING*boundingRect().height());
    }
}

QRectF CustomTreeGraphicsViewIcon::boundingRect() const{
    return boundingRectangle;
}

QString CustomTreeGraphicsViewIcon::getName() const{
    return name;
}

void CustomTreeGraphicsViewIcon::unhighlight(){
    rGrad.setColorAt(1.0, Qt::black);
    textPen.setColor(Qt::white);
    if (scene()){
        scene()->update();
    }
}

bool CustomTreeGraphicsViewIcon::hasChildIcon(CustomTreeGraphicsViewIcon *child){
    if (children.contains(child)){
        return true;
    }
    return false;
}

CustomTreeGraphicsViewIcon* CustomTreeGraphicsViewIcon::getChildIcon(HkxObject *child){
    for (int i = 0; i < children.size(); i++){
        if (children.at(i)->data.data() == child){
            return children.at(i);
        }
    }
    return NULL;
}

void CustomTreeGraphicsViewIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    if (data.constData()->isDataValid()){
        pen.setColor(Qt::black);
    }else{
        pen.setColor(Qt::red);
    }
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setFont(font);
    painter->setPen(pen);
    painter->setBrush(rGrad);
    painter->drawRoundedRect(boundingRect(), 4, 4);
    painter->setBrush(brush);
    painter->drawPath(path);
    painter->setPen(textPen);
    painter->drawText(textRec, static_cast<DataIconManager *>(data.data())->getName());
    painter->setPen(pen);
    painter->setBrush(buttonBrush);
    if (static_cast<DataIconManager *>(data.data())->hasChildren()){
        DataIconManager *ptr = static_cast<DataIconManager *>(data.data());
        painter->drawRect(button);
        if (ptr->icons.isEmpty()){
            painter->drawLine(horiz);
            if (!isExpanded){
                painter->drawLine(vert);
            }
        }else if (ptr->icons.first() == this){
            if (children.size() > 0){
                painter->drawLine(horiz);
                if (!isExpanded){
                    painter->drawLine(vert);
                }
            }
        }else{
            painter->drawPolygon(arrowHead);
            painter->drawLine(vert);
        }
    }
}

void CustomTreeGraphicsViewIcon::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setSelected(event);
}

void CustomTreeGraphicsViewIcon::setSelected(QGraphicsSceneMouseEvent *event){
    DataIconManager *ptr = static_cast<DataIconManager *>(data.data());
    CustomTreeGraphicsView *view = NULL;
    CustomTreeGraphicsViewIcon *firstSceneIcon = NULL;
    if (scene() && !scene()->items(Qt::AscendingOrder).isEmpty() && !scene()->views().isEmpty()){
        view = static_cast<CustomTreeGraphicsView *>(scene()->views().first());
        firstSceneIcon = static_cast<CustomTreeGraphicsViewIcon *>(scene()->items(Qt::AscendingOrder).first());
        if (view->selectedIcon != this){
            if (view->selectedIcon){
                if (view->selectedIcon->data->getType() == HkxObject::TYPE_GENERATOR){
                    view->selectedIcon->rGrad.setColorAt(0.0, Qt::white);
                    view->selectedIcon->rGrad.setColorAt(1.0, Qt::black);
                }else if (view->selectedIcon->data->getType() == HkxObject::TYPE_MODIFIER){
                    view->selectedIcon->rGrad.setColorAt(0.0, Qt::white);
                    view->selectedIcon->rGrad.setColorAt(1.0, Qt::blue);
                }
                view->selectedIcon->textPen.setColor(Qt::white);
            }
            view->selectedIcon = this;
            static_cast<BehaviorGraphView *>(view)->ui->changeCurrentDataWidget(this);
        }
        rGrad.setColorAt(1.0, Qt::green);
        textPen.setColor(Qt::blue);
    }
    if (event && (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)){
        if (button.contains(event->lastPos())){
            if (ptr->icons.isEmpty()){
                return;
            }
            if (ptr->icons.first() != this){
                if (!ptr->icons.first()->isVisible()){
                    view->expandBranch(firstSceneIcon, true);
                }
                view->repositionIcons(firstSceneIcon);
                view->scale(view->iconFocusScale, view->iconFocusScale);
                view->currentScale = view->iconFocusScale;
                view->centerOn(ptr->icons.first());
                view->selectedIcon = ptr->icons.first();
                ptr->icons.first()->rGrad.setColorAt(1.0, Qt::green);
                ptr->icons.first()->textPen.setColor(Qt::blue);
                rGrad.setColorAt(1.0, Qt::black);
                textPen.setColor(Qt::white);
            }else{
                if (isExpanded){
                    view->contractBranch(this);
                    isExpanded = false;
                }else{
                    isExpanded = true;
                    view->expandBranch(this);
                }
                view->repositionIcons(firstSceneIcon);
            }
        }
    }else{
        view->expandBranch(firstSceneIcon);
        view->repositionIcons(firstSceneIcon, true);
    }
    scene()->update();
}

void CustomTreeGraphicsViewIcon::unselect(){
    CustomTreeGraphicsView *view = NULL;
    CustomTreeGraphicsViewIcon *firstSceneIcon = NULL;
    if (scene() && !scene()->items(Qt::AscendingOrder).isEmpty() && !scene()->views().isEmpty()){
        view = static_cast<CustomTreeGraphicsView *>(scene()->views().first());
        firstSceneIcon = static_cast<CustomTreeGraphicsViewIcon *>(scene()->items(Qt::AscendingOrder).first());
        if (view->selectedIcon == this){
            if (view->selectedIcon){
                if (data->getType() == HkxObject::TYPE_GENERATOR){
                    rGrad.setColorAt(0.0, Qt::white);
                    rGrad.setColorAt(1.0, Qt::black);
                }else if (data->getType() == HkxObject::TYPE_MODIFIER){
                    rGrad.setColorAt(0.0, Qt::white);
                    rGrad.setColorAt(1.0, Qt::blue);
                }
                view->selectedIcon = NULL;
                static_cast<BehaviorGraphView *>(view)->ui->changeCurrentDataWidget(NULL);
            }
        }
        rGrad.setColorAt(1.0, Qt::green);
        textPen.setColor(Qt::blue);
    }
    scene()->update();
}

bool CustomTreeGraphicsViewIcon::getLastIconY(CustomTreeGraphicsViewIcon *parent, qreal &lastIconY){
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

void CustomTreeGraphicsViewIcon::updatePosition(bool updateNonVisable){
    if (!parent || (!isVisible() && !updateNonVisable)){
        return;
    }
    qreal iconY = 0;
    getLastIconY(parent, iconY);
    qreal x = parent->pos().x() + ICON_SPACING_X*(parent->boundingRect().width());
    qreal yt = iconY + 1.0*parent->boundingRect().height();
    setPos(x, yt);
    QLineF line(parent->pos().x() + parent->boundingRect().width(), parent->pos().y() + 1.0*parent->boundingRect().height(),\
                 parent->pos().x() + ICON_SPACING_X*parent->boundingRect().width(), yt + ICON_SPACING_Y*parent->boundingRect().height());
    if (linkToParent){
        linkToParent->setLine(line);
    }else {
        linkToParent = new QGraphicsLineItem(line);
    }
}

void CustomTreeGraphicsViewIcon::setLinkCoordinates(const QLineF & line){
    if (linkToParent){
        linkToParent->setLine(line);
    }
}

bool CustomTreeGraphicsViewIcon::isGranfatherParadox(CustomTreeGraphicsViewIcon *child) const{
    CustomTreeGraphicsViewIcon *parentIcon = parent;
    while (parentIcon){
        if (parentIcon->data == child->data){
            return true;
        }
        parentIcon = parentIcon->parent;
    }
    return false;
}

bool CustomTreeGraphicsViewIcon::isGranfatherParadox(DataIconManager *child) const{
    if (child == data.data()){
        return true;
    }
    CustomTreeGraphicsViewIcon *parentIcon = parent;
    while (parentIcon){
        if (parentIcon->data.data() == child){
            return true;
        }
        parentIcon = parentIcon->parent;
    }
    return false;
}

bool CustomTreeGraphicsViewIcon::isDescendant(CustomTreeGraphicsViewIcon *icon){
    if (icon){
        for (int i = 0; i < children.size(); i++){
            if (children.at(i) == icon){
                return true;
            }else{
                if (children.at(i)->isDescendant(icon)){
                    return true;
                }
            }
        }
    }
    return false;
}

void CustomTreeGraphicsViewIcon::errorHighlight(){
    if (pen.color() == Qt::black){
        pen.setColor(Qt::red);
    }else{
        pen.setColor(Qt::black);
    }
    if (scene()){
        scene()->update();
    }
}

CustomTreeGraphicsViewIcon* CustomTreeGraphicsViewIcon::setParent(CustomTreeGraphicsViewIcon *newParent){//If data is the same replace icon???
    if (!newParent){
        return NULL;
    }
    CustomTreeGraphicsViewIcon *oldParent = parent;
    CustomTreeGraphicsViewIcon *newIcon = NULL;
    CustomTreeGraphicsViewIcon *temp = NULL;
    int i;
    if (parent){
        i = parent->children.indexOf(this);
        if (children.isEmpty() && i > -1 && i < parent->children.size()){
            newIcon = new CustomTreeGraphicsViewIcon(static_cast<DataIconManager *>(data.data()), static_cast<DataIconManager *>(data.data())->getName(), parent);
            parent->children.move(parent->children.size() - 1, i);
            if (scene()){
                scene()->addItem(newIcon);
                scene()->addItem(newIcon->linkToParent);
            }
        }
        parent->children.removeAll(this);
    }
    temp = newParent->getChildIcon(this->data.data());
    if (!temp){
        newParent->children.append(this);
    }else{
        newParent->children.replace(newParent->children.indexOf(temp), this);
    }
    parent = newParent;
    QLineF line(parent->pos().x() + parent->boundingRect().width(), parent->pos().y() + 1.0*parent->boundingRect().height(),\
                parent->pos().x() + ICON_SPACING_X*parent->boundingRect().width(), 1.0*parent->boundingRect().height());
    if (linkToParent){
        linkToParent->setLine(line);
    }else{
        linkToParent = new QGraphicsLineItem(line);
    }
    return oldParent;
}

CustomTreeGraphicsViewIcon* CustomTreeGraphicsViewIcon::setParentReturnDuplicate(CustomTreeGraphicsViewIcon *newParent){//If data is the same replace icon???
    if (!newParent){
        return NULL;
    }
    //CustomTreeGraphicsViewIcon *oldParent = parent;
    CustomTreeGraphicsViewIcon *newIcon = NULL;
    CustomTreeGraphicsViewIcon *temp = NULL;
    int i;
    if (parent){
        i = parent->children.indexOf(this);
        if (i > -1 && i < parent->children.size()){
            newIcon = new CustomTreeGraphicsViewIcon(static_cast<DataIconManager *>(data.data()), static_cast<DataIconManager *>(data.data())->getName(), parent);
            parent->children.move(parent->children.size() - 1, i);
            if (scene()){
                scene()->addItem(newIcon);
                scene()->addItem(newIcon->linkToParent);
            }
        }
        parent->children.removeAll(this);
    }
    temp = newParent->getChildIcon(this->data.data());
    if (!temp){
        newParent->children.append(this);
    }else{
        newParent->children.replace(newParent->children.indexOf(temp), this);
    }
    parent = newParent;
    QLineF line(parent->pos().x() + parent->boundingRect().width(), parent->pos().y() + 1.0*parent->boundingRect().height(),\
                parent->pos().x() + ICON_SPACING_X*parent->boundingRect().width(), ICON_SPACING_Y*parent->boundingRect().height());
    if (linkToParent){
        linkToParent->setLine(line);
    }else{
        linkToParent = new QGraphicsLineItem(line);
    }
    return newIcon;
}

void CustomTreeGraphicsViewIcon::replaceChild(CustomTreeGraphicsViewIcon *childToReplace, CustomTreeGraphicsViewIcon *replacementChild){
    if (!childToReplace || !replacementChild){
        return;
    }
    for (int i = 0; i < children.size(); i++){
        if (children.at(i) == childToReplace){
            children.replace(i, replacementChild);
            replacementChild->setParent(this);
            childToReplace->setParent(replacementChild);
        }
    }
}

CustomTreeGraphicsViewIcon::~CustomTreeGraphicsViewIcon(){
    //
}
