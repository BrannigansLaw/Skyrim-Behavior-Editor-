#ifndef BEHAVIORGRAPHUI_H
#define BEHAVIORGRAPHUI_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>

#include "hkobject.h"

class FileSelectWindow;
class QVBoxLayout;
class QMenuBar;
class QMenu;
class QFile;
class QGraphicsScene;
class QGroupBox;
class QHBoxLayout;
class BehaviorFile;
class hkbManualSelectorGenerator;
class hkbBlenderGenerator;
class hkbBlenderGeneratorChild;
class hkbStateMachine;
class hkbStateMachineStateInfo;
class BSBoneSwitchGenerator;
class BSBoneSwitchGeneratorBoneData;
class hkbGenerator;

class GeneratorIcon: public QGraphicsItem
{
    friend class BehaviorGraphView;
public:
    GeneratorIcon(const HkObjectExpSharedPtr & d, const QString & s, GeneratorIcon *par = NULL);
    QRectF boundingRect() const{
        return boundingRectangle;
    }
    QString getName() const{
        return name;
    }
    void setLinkCoordinates(const QLineF & line){
        if (linkToParent){
            linkToParent->setLine(line);
        }
    }
    void errorHighlight(){
        if (pen.color() == Qt::black){
            pen.setColor(Qt::red);
        }else{
            pen.setColor(Qt::black);
        }
        if (scene()){
            scene()->update();
        }
    }
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
private:
    static void updateStaticMembers();
    bool getLastIconY(GeneratorIcon *parent, qreal &lastIconY);
    void updatePosition();
private:
    static QRectF boundingRectangle;
    static QRectF button;
    static QRectF textRec;
    static QFont font;
    QPen pen;
    static QBrush brush;
    static QBrush buttonBrush;
    static QLineF vert;
    static QLineF horiz;
    static QRectF ellipse;
    static QPolygonF polygon;
    static QRectF square;
    static QPolygonF arrowHead;
    QRadialGradient rGrad;
    QPen textPen;
    QPainterPath path;
    QString name;
    HkObjectExpSharedPtr data;
    bool isExpanded;
    GeneratorIcon *parent;
    QList <GeneratorIcon *> children;
    QGraphicsLineItem *linkToParent;
};

class BehaviorGraphView: public QGraphicsView
{
    Q_OBJECT
    friend class GeneratorIcon;
public:
    BehaviorGraphView(BehaviorFile * file);
    bool drawBehaviorGraph();
    QSize sizeHint() const Q_DECL_OVERRIDE{
        return QSize(500, 400);
    }
protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private slots:
    void wrapStateMachine();
    void removeSelectedObject();
    void removeSelectedObjectBranch();
private:
    BehaviorFile *behavior;
    QGraphicsScene *behaviorGS;
    GeneratorIcon *selectedIcon;
    QMenu *contextMenu;
    QMenu *wrapGeneratorMenu;
    QAction *wrapSMAct;
    QAction *wrapMSGAct;
    QAction *wrapMGAct;
    QAction *wrapBSISTGAct;
    QAction *wrapBSSCGAct;
    QAction *wrapBSOAGAct;
    QAction *wrapBSCBTGAct;
    QAction *wrapPMGAct;
    QMenu *wrapBlenderMenu;
    QAction *wrapBGAct;
    QAction *wrapBSBSGAct;
    QAction *removeObjAct;
    QAction *removeObjBranchAct;
    const qreal minScale;
    const qreal maxScale;
    const qreal initScale;
    const qreal iconFocusScale;
    qreal currentScale;
    const qreal scaleUpFactor;
    const qreal scaleDownFactor;
private:
    bool drawBranch(GeneratorIcon * rootIcon);
    void removeChildIcons(GeneratorIcon *parent, int startIndex = 0);
    void repositionIcons(GeneratorIcon * icon);
    void popUpMenuRequested(const QPoint &pos, const HkObjectExpSharedPtr & obj);
    void expandBranch(GeneratorIcon * icon, bool expandAll = false);
    void contractBranch(GeneratorIcon * icon, bool contractAll = false);
    HkObject * getFirstChild(const HkObjectExpSharedPtr &obj);
    int manageIcons();
    bool positionIcon(GeneratorIcon * icon);

    template<typename T>
    int initializeIconsForNewBranch(const T & ptr, QList<HkObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount){
        if (parentIcons.isEmpty()){
            return -1;
        }
        if (objects.isEmpty()){
            return -1;
        }
        if (objectChildCount.isEmpty()){
            return -1;
        }
        bool isBranchTip = true;
        GeneratorIcon *icon = addIconToGraph(objects.last(), ptr, parentIcons.last());
        if (!icon){
            return -1;
        }
        if (ptr->getSignature() != HKB_CLIP_GENERATOR && ptr->getSignature() != HKB_BEHAVIOR_REFERENCE_GENERATOR){
            isBranchTip = false;
        }
        objects.removeLast();
        objectChildCount.last()--;
        if (objectChildCount.last() < 1){
            parentIcons.removeLast();
            objectChildCount.removeLast();
        }
        if (!isBranchTip){
            objectChildCount.append(0);
            parentIcons.append(icon);
        }
        ptr->icons.append(icon);
        return 0;
    }

    template<typename T>
    int initializeIcons(const T & ptr, QList<HkObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount){
        if (parentIcons.isEmpty()){
            return -1;
        }
        if (objects.isEmpty()){
            return -1;
        }
        if (objectChildCount.isEmpty()){
            return -1;
        }
        bool isBranchTip = true;
        for (int i = 0; i < parentIcons.last()->children.size(); i++){
            if (parentIcons.last()->children.at(i)->data == objects.last()){
                objects.removeLast();
                objectChildCount.last()--;
                if (objectChildCount.last() < 1){
                    parentIcons.removeLast();
                    objectChildCount.removeLast();
                }
                return 1;
            }
        }
        GeneratorIcon *icon = addIconToGraph(objects.last(), ptr, parentIcons.last());
        if (!icon){
            return -1;
        }
        if (ptr->getSignature() != HKB_CLIP_GENERATOR && ptr->getSignature() != HKB_BEHAVIOR_REFERENCE_GENERATOR){
            isBranchTip = false;
            if (!ptr->icons.isEmpty()){
                ptr->icons.append(icon);
                objects.removeLast();
                objectChildCount.last()--;
                if (objectChildCount.last() < 1){
                    parentIcons.removeLast();
                    objectChildCount.removeLast();
                }
                return 1;
            }
        }
        objects.removeLast();
        objectChildCount.last()--;
        if (objectChildCount.last() < 1){
            parentIcons.removeLast();
            objectChildCount.removeLast();
        }
        if (!isBranchTip){
            objectChildCount.append(0);
            parentIcons.append(icon);
        }
        ptr->icons.append(icon);
        return 0;
    }

    template<typename T>
    GeneratorIcon * addIconToGraph(const HkObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon){
        if (!parentIcon){
            return NULL;
        }
        if (!type){
            return NULL;
        }
        /*if (behaviorGS->items(Qt::AscendingOrder).isEmpty()){
            return NULL;
        }*/
        GeneratorIcon *icon = new GeneratorIcon(obj, type->name, parentIcon);
        //icon->setFlag(QGraphicsItem::ItemIsMovable);
        qreal lastY = 0;
        icon->getLastIconY(parentIcon, lastY);
        icon->setLinkCoordinates(QLineF(parentIcon->pos().x() + 1.0*parentIcon->boundingRect().width(), parentIcon->pos().y() + 1.0*parentIcon->boundingRect().height(),\
                                        parentIcon->pos().x() + 1.5*parentIcon->boundingRect().width(), lastY + 2*icon->boundingRect().height()));
        /*GeneratorIcon *lastIcon = dynamic_cast<GeneratorIcon *>(behaviorGS->items(Qt::AscendingOrder).last());
        if (!lastIcon){
            return NULL;
        }
        icon->setLinkCoordinates(QLineF(parentIcon->pos().x() + 1.0*parentIcon->boundingRect().width(), parentIcon->pos().y() + 1.0*parentIcon->boundingRect().height(),\
                                        parentIcon->pos().x() + 1.5*parentIcon->boundingRect().width(), lastIcon->pos().y() + 2*lastIcon->boundingRect().height()));*/
        behaviorGS->addItem(icon->linkToParent);
        behaviorGS->addItem(icon);
        icon->setPos(parentIcon->pos().x() + 1.5*parentIcon->boundingRect().width(), lastY + 2*icon->boundingRect().height());
        return icon;
    }

    template<typename T>
    GeneratorIcon * initalizeInjectedIcon(const HkObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon){
        if (!parentIcon){
            return NULL;
        }
        GeneratorIcon *icon = new GeneratorIcon(obj, type->name, parentIcon);
        QLineF line(icon->parent->pos().x() + 1.0*icon->parent->boundingRect().width(), icon->parent->pos().y() + 1.0*icon->parent->boundingRect().height(),\
                             icon->parent->pos().x() + 1.5*icon->parent->boundingRect().width(), icon->parent->boundingRect().height());
        if (icon->linkToParent){
            icon->linkToParent->setLine(line);
        }else {
            icon->linkToParent = new QGraphicsLineItem(line);
        }
        for (int j = 0; j < selectedIcon->parent->children.size(); j++){
            if (selectedIcon->parent->children.at(j) == selectedIcon){
                selectedIcon->parent->children.replace(j, icon);
                //Constructor for GeneratorIcon appends icon to parent so we need to remove it...
                if (selectedIcon->parent->children.size() - 1 != j){
                    selectedIcon->parent->children.removeLast();
                }else{
                    //Problem???
                }
                break;
            }
        }
        //type->icons.append(icon);
        selectedIcon->parent = icon;
        icon->children.append(selectedIcon);
        //Order is important??
        behaviorGS->addItem(icon->linkToParent);
        behaviorGS->addItem(icon);
        return icon;
    }
};

#endif // BEHAVIORGRAPHUI_H
