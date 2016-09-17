#include "src/ui/mainwindow.h"
#include "src/ui/fileselectwindow.h"
#include "src/filetypes/hkxfile.h"
#include "src/hkxclasses/generators/hkbgenerator.h"
#include "behaviorgraphui.h"
#include "src/hkxclasses/hkxobject.h"
#include "behaviorgraphicons.h"
#include "src/ui/hkdataui.h"

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

GeneratorIcon::GeneratorIcon(const HkxObjectExpSharedPtr & d, const QString & s, GeneratorIcon * par)
    : data(d),
      name(s),
      isExpanded(true),
      parent(par),
      linkToParent(NULL)
{
    if (data->getType() == HkxObject::TYPE_GENERATOR){
        static_cast<hkbGenerator *>(data.data())->icons.append(this);
    }
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
        qreal lastY = 0;
        getLastIconY(parent, lastY);
        int index = parent->children.indexOf(this);
        if (index == -1){
            parent->children.append(this);
        }else{
            parent->children.replace(index, this);
        }
        linkToParent = new QGraphicsLineItem(parent->pos().x() + 1.0*parent->boundingRect().width(), parent->pos().y() + 1.0*parent->boundingRect().height(),
                                             parent->pos().x() + 1.5*parent->boundingRect().width(), lastY + 2*boundingRect().height());
        setPos(parent->pos().x() + 1.5*parent->boundingRect().width(), lastY + 2*boundingRect().height());
    }
}

void GeneratorIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
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
    painter->drawText(textRec, static_cast<hkbGenerator *>(data.data())->getName());
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
    setSelected(event);
}

void GeneratorIcon::setSelected(QGraphicsSceneMouseEvent *event){
    hkbGenerator *ptr = static_cast<hkbGenerator *>(data.data());
    BehaviorGraphView *view = NULL;
    GeneratorIcon *firstSceneIcon = NULL;
    if (scene() && !scene()->items(Qt::AscendingOrder).isEmpty() && !scene()->views().isEmpty()){
        view = static_cast<BehaviorGraphView *>(scene()->views().first());
        firstSceneIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first());
        if (view->selectedIcon != this){
            if (view->selectedIcon){
                view->selectedIcon->rGrad.setColorAt(1.0, Qt::black);
                view->selectedIcon->textPen.setColor(Qt::white);
            }
            view->selectedIcon = this;
            view->ui->changeCurrentDataWidget(this);
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
        if (event->button() == Qt::RightButton){
            view->popUpMenuRequested(view->mapFromScene(event->scenePos()), this);
        }
    }else{
        view->expandBranch(firstSceneIcon);
        view->repositionIcons(firstSceneIcon, true);
    }
    scene()->update();
}

void GeneratorIcon::unselect(){
    BehaviorGraphView *view = NULL;
    GeneratorIcon *firstSceneIcon = NULL;
    if (scene() && !scene()->items(Qt::AscendingOrder).isEmpty() && !scene()->views().isEmpty()){
        view = static_cast<BehaviorGraphView *>(scene()->views().first());
        firstSceneIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first());
        if (view->selectedIcon == this){
            if (view->selectedIcon){
                view->selectedIcon->rGrad.setColorAt(1.0, Qt::black);
                view->selectedIcon->textPen.setColor(Qt::white);
                view->selectedIcon = NULL;
                view->ui->changeCurrentDataWidget(NULL);
            }
        }
        rGrad.setColorAt(1.0, Qt::green);
        textPen.setColor(Qt::blue);
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
