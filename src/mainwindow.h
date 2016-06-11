#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    QRectF boundingRect() const{return boundingRectangle;}
    QString getName() const{return name;}
    void setLinkCoordinates(const QLineF & line){if (linkToParent){linkToParent->setLine(line);}}
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
private:
    static void updateStaticMembers();
    void expandBranch(GeneratorIcon * icon, bool expandAll = false);
    void contractBranch(GeneratorIcon * icon, bool contractAll = false);
    bool getLastIconY(GeneratorIcon *parent, qreal &lastIconY);
    void updatePosition();
private:
    static QRectF boundingRectangle;
    static QRectF button;
    static QRectF textRec;
    static QFont font;
    //static QRadialGradient rGrad;
    //static QPen textPen;
    static QPen pen;
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
    void repositionIcons(GeneratorIcon * icon);
protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private:
    BehaviorFile *behavior;
    QGraphicsScene *behaviorGS;
    GeneratorIcon *selectedIcon;
private:
    int manageIcons();
    bool positionIcon(GeneratorIcon * icon);
    template<typename T>
    int initializeIcons(const T & ptr, QList<HkObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons){
        if (parentIcons.isEmpty()){
            return -1;
        }
        if (objects.isEmpty()){
            return -1;
        }
        GeneratorIcon *icon = addIconToGraph(objects.last(), ptr, parentIcons.last());
        if (!icon){
            return -1;
        }
        if (parentIcons.last()->data->getSignature() == HKB_MANUAL_SELECTOR_GENERATOR){
            if (static_cast<hkbManualSelectorGenerator *>(parentIcons.last()->data.data())->generators.size() < 2 ||\
                    static_cast<hkbManualSelectorGenerator *>(parentIcons.last()->data.data())->generators.last() == objects.last())
            {
                if (parentIcons.isEmpty()){
                    return -1;
                }
                parentIcons.removeLast();
            }
        }else if (parentIcons.last()->data->getSignature() == HKB_BLENDER_GENERATOR){
            if (static_cast<hkbBlenderGenerator *>(parentIcons.last()->data.data())->children.size() < 2 ||\
                    static_cast<hkbBlenderGeneratorChild *>(static_cast<hkbBlenderGenerator *>(parentIcons.last()->data.data())->children.last().data())->generator == objects.last())
            {
                if (parentIcons.isEmpty()){
                    return -1;
                }
                parentIcons.removeLast();
            }
        }else if (parentIcons.last()->data->getSignature() == HKB_POSE_MATCHING_GENERATOR){
            if (static_cast<hkbPoseMatchingGenerator *>(parentIcons.last()->data.data())->children.size() < 2 ||\
                    static_cast<hkbBlenderGeneratorChild *>(static_cast<hkbPoseMatchingGenerator *>(parentIcons.last()->data.data())->children.last().data())->generator == objects.last())
            {
                if (parentIcons.isEmpty()){
                    return -1;
                }
                parentIcons.removeLast();
            }
        }else if (parentIcons.last()->data->getSignature() == BS_BONE_SWITCH_GENERATOR){
            if (static_cast<BSBoneSwitchGenerator *>(parentIcons.last()->data.data())->pDefaultGenerator == objects.last())
            {
                if (parentIcons.isEmpty()){
                    return -1;
                }
                parentIcons.removeLast();
            }
        }else if (parentIcons.last()->data->getSignature() == HKB_STATE_MACHINE){
            if (static_cast<hkbStateMachine *>(parentIcons.last()->data.data())->states.size() < 2 ||\
                    static_cast<hkbStateMachineStateInfo *>(static_cast<hkbStateMachine *>(parentIcons.last()->data.data())->states.last().data())->generator == objects.last())
            {
                if (parentIcons.isEmpty()){
                    return -1;
                }
                parentIcons.removeLast();
            }
        }else{
            if (parentIcons.isEmpty()){
                return -1;
            }
            parentIcons.removeLast();
        }
        if (objects.isEmpty()){
            return -1;
        }
        if (ptr->getSignature() != HKB_CLIP_GENERATOR && ptr->getSignature() != HKB_BEHAVIOR_REFERENCE_GENERATOR){
            if (!ptr->icons.isEmpty()){
                ptr->icons.append(icon);
                objects.removeLast();
                return 1;
            }
            parentIcons.append(icon);
        }
        ptr->icons.append(icon);
        objects.removeLast();
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
        if (behaviorGS->items(Qt::AscendingOrder).isEmpty()){
            return NULL;
        }
        GeneratorIcon *icon = new GeneratorIcon(obj, type->name, parentIcon);
        //icon->setFlag(QGraphicsItem::ItemIsMovable);
        GeneratorIcon *lastIcon = dynamic_cast<GeneratorIcon *>(behaviorGS->items(Qt::AscendingOrder).last());
        if (!lastIcon){
            return NULL;
        }
        icon->setLinkCoordinates(QLineF(parentIcon->pos().x() + 1.0*parentIcon->boundingRect().width(), parentIcon->pos().y() + 1.0*parentIcon->boundingRect().height(),\
                                        parentIcon->pos().x() + 1.5*parentIcon->boundingRect().width(), lastIcon->pos().y() + 2*lastIcon->boundingRect().height()));
        behaviorGS->addItem(icon->linkToParent);
        behaviorGS->addItem(icon);
        icon->setPos(parentIcon->pos().x() + 1.5*parentIcon->boundingRect().width(), lastIcon->pos().y() + 2*lastIcon->boundingRect().height());
        return icon;
    }
};

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    QVBoxLayout *topLyt;
    QHBoxLayout *mainLyt;
    QMenuBar *topMB;
    QAction *openA;
    QMenu *openM;
    FileSelectWindow *dirViewFSW;
    BehaviorFile *hkxFile;
    BehaviorGraphView *behaviorGraphViewIV;
    QGroupBox *behaviorGraphViewGB;
    QVBoxLayout *iconGBLyt;
    QGroupBox *objectDataGB;
private slots:
    void openDirView();
    void openHkxfile(QString name);
private:
    bool exitProgram();
    //void drawIcons();
    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H
