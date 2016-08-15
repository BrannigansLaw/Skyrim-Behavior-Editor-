#include "mainwindow.h"
#include "fileselectwindow.h"
#include "hkxfile.h"
#include "generators.h"
#include "behaviorgraphui.h"
#include "hkobject.h"
#include "behaviorgraphicons.h"

#include <QBoxLayout>
#include <QMenuBar>
#include <QFile>
#include <QSettings>
#include <QGroupBox>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QSize>

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
//QPen GeneratorIcon::pen = QPen(QBrush(Qt::black), 2);
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
    /*if (data->getType() == HkObject::TYPE_GENERATOR){
        static_cast<hkbGenerator *>(data.data())->icons.append(this);
    }*/
    pen = QPen(QBrush(Qt::black), 2);
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
    if (data.constData()->getIsDataValid()){
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
        if (event->button() == Qt::RightButton){
            view->popUpMenuRequested(view->mapFromScene(event->scenePos()), this);
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

void GeneratorIcon::updatePosition(bool updateNonVisable){
    if (!parent || (!isVisible() && !updateNonVisable)){
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
