#include "behaviorgraphicon.h"
#include "src/ui/treegraphicsscene.h"
#include "src/ui/dataiconmanager.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "src/utility.h"
#include "src/hkxclasses/hkxobject.h"

#define ITEM_WIDTH 400
#define ITEM_HEIGHT 50

QRectF BehaviorGraphIcon::button = QRectF(360, 0, 40, 50);
QLineF BehaviorGraphIcon::vert = QLineF(380, 5, 380, 45);
QLineF BehaviorGraphIcon::horiz = QLineF(365, 25, 395, 25);
QRectF BehaviorGraphIcon::textRec = QRectF(120, 10, 240, 40);
//QRadialGradient BehaviorGraphIcon::rGrad = QRadialGradient();
QFont BehaviorGraphIcon::font = QFont("Helvetica [Cronyx]", 9);
//QPen BehaviorGraphIcon::textPen = QPen(Qt::white);
//QPen BehaviorGraphIcon::pen = QPen(QBrush(Qt::black), 2);
QBrush BehaviorGraphIcon::brush = QBrush(Qt::green);
QBrush BehaviorGraphIcon::buttonBrush = QBrush(Qt::gray);
//QPolygonF BehaviorGraphIcon::polygon = QPolygonF();
QRectF BehaviorGraphIcon::ellipse = QRectF(40, 5, 40, 40);
QRectF BehaviorGraphIcon::square = QRectF(40, 5, 40, 40);
//QPolygonF BehaviorGraphIcon::arrowHead = QPolygonF();

BehaviorGraphIcon::BehaviorGraphIcon(TreeGraphicsItem *parent, DataIconManager *obj, int indexToInsert, Qt::GlobalColor color)
    : TreeGraphicsItem(parent, obj, indexToInsert, color)
{
    arrowHead << QPointF(365, 25) << QPointF(380, 5) << QPointF(395, 25);
    polygon << QPointF(boundingRect().topLeft().x() + boundingRect().width() * 0.1, boundingRect().topLeft().y() + boundingRect().height() * 0.1)
            << QPointF(boundingRect().topLeft().x() + boundingRect().width() * 0.1, boundingRect().topLeft().y() + boundingRect().height() * 0.9)
            << QPointF(boundingRect().topLeft().x() + boundingRect().width() * 0.2, boundingRect().topLeft().y() + boundingRect().height() * 0.5)
            << QPointF(boundingRect().topLeft().x() + boundingRect().width() * 0.1, boundingRect().topLeft().y() + boundingRect().height() * 0.1);
    rGrad.setCenter(boundingRect().topLeft());
    rGrad.setCenterRadius(boundingRect().width());
    rGrad.setColorAt(0.0, Qt::white);
    if (itemData->getType() == HkxObject::TYPE_MODIFIER){
        rGrad.setColorAt(1.0, Qt::magenta);
    }else{
        HkxSignature sig = itemData->getSignature();
        switch (sig){
        case HKB_BLENDER_GENERATOR:
            path.addRect(square);
            rGrad.setColorAt(1.0, Qt::darkRed);
            break;
        case HKB_POSE_MATCHING_GENERATOR:
            path.addRect(square);
            rGrad.setColorAt(1.0, Qt::darkRed);
            break;
        case BS_BONE_SWITCH_GENERATOR:
            path.addRect(square);
            rGrad.setColorAt(1.0, Qt::darkRed);
            break;
        case HKB_STATE_MACHINE:
            path.addPolygon(polygon);
            rGrad.setColorAt(1.0, Qt::cyan);
            break;
        case HKB_CLIP_GENERATOR:
            path.addEllipse(ellipse);
            rGrad.setColorAt(1.0, Qt::yellow);
            break;
        case HKB_BEHAVIOR_REFERENCE_GENERATOR:
            path.addEllipse(ellipse);
            rGrad.setColorAt(1.0, Qt::darkYellow);
            break;
        case BS_SYNCHRONIZED_CLIP_GENERATOR:
            path.addEllipse(ellipse);
            rGrad.setColorAt(1.0, Qt::yellow);
            break;
        default:
            path.addPolygon(polygon);
            rGrad.setColorAt(1.0, Qt::gray);
        }
    }
}

QRectF BehaviorGraphIcon::boundingRect() const{
    return QRectF(0,0,ITEM_WIDTH,ITEM_HEIGHT);
}

void BehaviorGraphIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setFont(font);
    painter->setPen(pen);
    painter->setBrush(rGrad);
    painter->drawRoundedRect(boundingRect(), 4, 4);
    painter->setBrush(brush);
    painter->drawPath(path);
    painter->setPen(textPen);
    painter->drawText(textRec, itemData->getName());
    painter->setPen(pen);
    painter->setBrush(buttonBrush);
    if (itemData->hasChildren()){
        painter->drawRect(button);
        if (itemData->icons.isEmpty()){
            painter->drawLine(horiz);
            if (!isExpanded){
                painter->drawLine(vert);
            }
        }else if (itemData->icons.first() == this){
            if (!childItems().isEmpty()){
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

void BehaviorGraphIcon::unselect(){
    if (itemData->getType() == HkxObject::TYPE_MODIFIER){
        rGrad.setColorAt(1.0, Qt::magenta);
    }else{
        HkxSignature sig = itemData->getSignature();
        switch (sig){
        case HKB_BLENDER_GENERATOR:
            rGrad.setColorAt(1.0, Qt::darkRed);
            break;
        case HKB_POSE_MATCHING_GENERATOR:
            rGrad.setColorAt(1.0, Qt::darkRed);
            break;
        case BS_BONE_SWITCH_GENERATOR:
            rGrad.setColorAt(1.0, Qt::darkRed);
            break;
        case HKB_STATE_MACHINE:
            rGrad.setColorAt(1.0, Qt::cyan);
            break;
        case HKB_CLIP_GENERATOR:
            rGrad.setColorAt(1.0, Qt::yellow);
            break;
        case HKB_BEHAVIOR_REFERENCE_GENERATOR:
            rGrad.setColorAt(1.0, Qt::darkYellow);
            break;
        case BS_SYNCHRONIZED_CLIP_GENERATOR:
            rGrad.setColorAt(1.0, Qt::yellow);
            break;
        default:
            rGrad.setColorAt(1.0, Qt::gray);
        }
    }
    scene()->update(QRectF(scenePos(), scenePos() + QPointF(ITEM_WIDTH, ITEM_HEIGHT)));
}
