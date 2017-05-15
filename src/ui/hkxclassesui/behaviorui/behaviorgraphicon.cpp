#include "behaviorgraphicon.h"
#include "src/ui/treegraphicsscene.h"
#include "src/ui/dataiconmanager.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "src/utility.h"
#include "src/hkxclasses/hkxobject.h"

#define ITEM_WIDTH 500
#define ITEM_HEIGHT 50

#define OUTLINE_PEN_WIDTH 5
#define TEXT_PEN_WIDTH 2

QRectF BehaviorGraphIcon::button = QRectF(ITEM_WIDTH*0.9, 0, ITEM_WIDTH*0.1, ITEM_HEIGHT);
QLineF BehaviorGraphIcon::vert = QLineF(ITEM_WIDTH*0.95, ITEM_HEIGHT*0.1, ITEM_WIDTH*0.95, ITEM_HEIGHT*0.8);
QLineF BehaviorGraphIcon::horiz = QLineF(ITEM_WIDTH*0.91, ITEM_HEIGHT*0.5, ITEM_WIDTH*0.99, ITEM_HEIGHT*0.5);
QRectF BehaviorGraphIcon::textRec = QRectF(ITEM_WIDTH*0.25, ITEM_HEIGHT*0.2, ITEM_WIDTH*0.75, ITEM_HEIGHT*0.8);
QFont BehaviorGraphIcon::font = QFont("Helvetica [Cronyx]", 9);
QBrush BehaviorGraphIcon::brush = QBrush(Qt::green);
QBrush BehaviorGraphIcon::buttonBrush = QBrush(Qt::gray);
QRectF BehaviorGraphIcon::ellipse = QRectF(ITEM_WIDTH*0.1, ITEM_HEIGHT*0.1, ITEM_WIDTH*0.1, ITEM_HEIGHT*0.8);
QRectF BehaviorGraphIcon::square = QRectF(ITEM_WIDTH*0.1, ITEM_HEIGHT*0.1, ITEM_WIDTH*0.1, ITEM_HEIGHT*0.8);

BehaviorGraphIcon::BehaviorGraphIcon(TreeGraphicsItem *parent, DataIconManager *obj, int indexToInsert, Qt::GlobalColor color)
    : TreeGraphicsItem(parent, obj, indexToInsert, color)
{
    arrowHead << QPointF(boundingRect().width()*0.9125, boundingRect().height()*0.5)
              << QPointF(boundingRect().width()*0.95, boundingRect().height()*0.1)
              << QPointF(boundingRect().width()*0.9875, boundingRect().height()*0.5);
    polygon << QPointF(boundingRect().width() * 0.1, boundingRect().height() * 0.1)
            << QPointF(boundingRect().width() * 0.1, boundingRect().height() * 0.9)
            << QPointF(boundingRect().width() * 0.2, boundingRect().height() * 0.5)
            << QPointF(boundingRect().width() * 0.1, boundingRect().height() * 0.1);
    rGrad.setCenter(boundingRect().topLeft());
    rGrad.setCenterRadius(boundingRect().width());
    rGrad.setColorAt(0.0, Qt::white);
    pen.setWidth(OUTLINE_PEN_WIDTH);
    textPen.setWidth(TEXT_PEN_WIDTH);
    if (itemData->getType() == HkxObject::TYPE_MODIFIER){
        path.addEllipse(ellipse);
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

void BehaviorGraphIcon::setIconSelected(){
    rGrad.setColorAt(1.0, Qt::green);
    scene()->update(QRectF(scenePos(), scenePos() + QPointF(boundingRect().width(), boundingRect().height())));
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
    painter->setBrush(buttonBrush);
    if (itemData->hasChildren()){
        painter->drawRect(button);
        if (itemData->icons.isEmpty()){
            CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphIcon::paint(): Icon data has no icons!!!"));
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
    scene()->update(QRectF(scenePos(), scenePos() + QPointF(boundingRect().width(), boundingRect().height())));
}
