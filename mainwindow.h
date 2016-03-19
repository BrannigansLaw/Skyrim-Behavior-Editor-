#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>

class FileSelectWindow;
class QVBoxLayout;
class QMenuBar;
class QMenu;
class QFile;
class QGraphicsScene;
class QGroupBox;
class QHBoxLayout;
class BehaviorFile;

class IconView: public QGraphicsView
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE{
        if (event->button() == Qt::LeftButton)  this->setDragMode(QGraphicsView::ScrollHandDrag);
        QGraphicsView::mousePressEvent(event);
    }
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE{
        if (event->button() == Qt::LeftButton) this->setDragMode(QGraphicsView::NoDrag);
        QGraphicsView::mouseReleaseEvent(event);
    }
};

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow();
private:
    QVBoxLayout *topLyt;
    QHBoxLayout *mainLyt;
    QMenuBar *topMB;
    QAction *openA;
    QMenu *openM;
    FileSelectWindow *dirViewFSW;
    BehaviorFile *hkxFile;
    QGraphicsScene *iconViewerGS;
    IconView *iconViewIV;
    QGroupBox *iconViewGB;
    QGroupBox *objectDataGB;
private slots:
    void openDirView();
    void openHkxfile(QString name);
private:
    void drawIcons();
    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H
