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

class GeneratorIcon: public QGraphicsItem
{
    friend class BehaviorGraphView;
public:
    GeneratorIcon(const HkObjectExpSharedPtr & d, const QString & s){data = d;name = s;}
    QRectF boundingRect() const{return QRectF(0, 0, 200, 50);}
    QString getName() const{return name;}
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
private:
    QString name;
    HkObjectExpSharedPtr data;
};

class BehaviorGraphView: public QGraphicsView
{
    Q_OBJECT
public:
    BehaviorGraphView(BehaviorFile * file): behavior(file), behaviorGS(new QGraphicsScene){setScene(behaviorGS);}
    bool drawBehaviorGraph();
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE{
        if (event->button() == Qt::LeftButton)  this->setDragMode(QGraphicsView::ScrollHandDrag);
        QGraphicsView::mousePressEvent(event);
    }
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE{
        if (event->button() == Qt::LeftButton) this->setDragMode(QGraphicsView::NoDrag);
        QGraphicsView::mouseReleaseEvent(event);
    }
private:
    //enum {OBJECT_CHILDREN_TRAVERSED = -5};
    BehaviorFile *behavior;
    QGraphicsScene *behaviorGS;
    QList <GeneratorIcon *> icons;
private:
    template<typename T>
    void helperFunction(const T & ptr, QList<HkObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons){
        GeneratorIcon *icon = positionIcon(objects.last(), ptr, parentIcons.last());
        if (parentIcons.last()->data->getSignature() == HKB_MANUAL_SELECTOR_GENERATOR){
            if (static_cast<hkbManualSelectorGenerator *>(parentIcons.last()->data.data())->generators.size() < 2 ||\
                    static_cast<hkbManualSelectorGenerator *>(parentIcons.last()->data.data())->generators.last() == objects.last())
            {
                parentIcons.removeLast();
                //objects.removeLast();
            }
            objects.removeLast();
        }else if (parentIcons.last()->data->getSignature() == HKB_BLENDER_GENERATOR){
            if (static_cast<hkbBlenderGenerator *>(parentIcons.last()->data.data())->children.size() < 2 ||\
                    static_cast<hkbBlenderGeneratorChild *>(static_cast<hkbBlenderGenerator *>(parentIcons.last()->data.data())->children.last().data())->generator == objects.last())
            {
                parentIcons.removeLast();
                //objects.removeLast();
            }
            objects.removeLast();
        }else if (parentIcons.last()->data->getSignature() == HKB_STATE_MACHINE){
            if (static_cast<hkbStateMachine *>(parentIcons.last()->data.data())->states.size() < 2 ||\
                    static_cast<hkbStateMachineStateInfo *>(static_cast<hkbStateMachine *>(parentIcons.last()->data.data())->states.last().data())->generator == objects.last())
            {
                parentIcons.removeLast();
                //objects.removeLast();
            }
            objects.removeLast();
        }else{
            parentIcons.removeLast();
            objects.removeLast();
        }
        parentIcons.append(icon);
    }

    template<typename T>
    GeneratorIcon * positionIcon(const HkObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon){
        if (!parentIcon){
            return NULL;
        }
        if (!type){
            return NULL;
        }
        if (behaviorGS->items(Qt::AscendingOrder).isEmpty()){
            return NULL;
        }
        GeneratorIcon *icon = new GeneratorIcon(obj, type->name);
        //icon->setFlag(QGraphicsItem::ItemIsMovable);
        //icon->setParentItem(parentIcon);
        GeneratorIcon *lastIcon = dynamic_cast<GeneratorIcon *>(behaviorGS->items(Qt::AscendingOrder).last());
        /*if (icons.isEmpty()){
            return NULL;
        }*/
        //GeneratorIcon *lastIcon = icons.last();
        if (!lastIcon){
            return NULL;
        }
        behaviorGS->addLine(parentIcon->pos().x() + 1.0*parentIcon->boundingRect().width(), parentIcon->pos().y() + 1.0*parentIcon->boundingRect().height(),\
                            parentIcon->pos().x() + 1.5*parentIcon->boundingRect().width(), lastIcon->pos().y() + 2*lastIcon->boundingRect().height());
        behaviorGS->addItem(icon);
        //icons.append(icon);
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
