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

class GeneratorIcon: public QGraphicsItem
{
public:
    GeneratorIcon(const HkObjectExpSharedPtr & d, const QString & s){data = d;name = s;}
    QRectF boundingRect() const{return QRectF(0, 0, 200, 50);}
    QString getName() const{return name;}
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    QString name;
    HkObjectExpSharedPtr data;
};

class BehaviorGraphView: public QGraphicsView
{
    Q_OBJECT
public:
    BehaviorGraphView(BehaviorFile * file): behavior(file), behaviorGS(new QGraphicsScene){setScene(behaviorGS);}
    void drawBehaviorGraph(const HkObjectExpSharedPtr & ptr, GeneratorIcon * parentIcon = NULL);
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
    BehaviorFile *behavior;
    QGraphicsScene *behaviorGS;
private:
    template<typename T>
    GeneratorIcon * positionIcon(const HkObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon){
        if (!parentIcon){
            return NULL;
        }
        if (behaviorGS->items().isEmpty()){
            return NULL;
        }
        GeneratorIcon *icon = new GeneratorIcon(obj, type->name);
        icon->setFlag(QGraphicsItem::ItemIsMovable);
        //icon->setParentItem(parentIcon);
        GeneratorIcon *lastIcon = reinterpret_cast<GeneratorIcon *>(behaviorGS->items(Qt::AscendingOrder).last());
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
