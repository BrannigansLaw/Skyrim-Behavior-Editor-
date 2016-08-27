#include "mainwindow.h"
#include "fileselectwindow.h"
#include "src/filetypes/hkxfile.h"
#include "src/hkxclasses/generators/hkbgenerator.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/ui/objectdataui/hkobjectui.h"

#include <QtWidgets>

MainWindow::MainWindow()
    :
      topLyt(new QGridLayout(this)),
      topMB(new QMenuBar(this)),
      debugLog(new QPlainTextEdit(this)),
      openA(new QAction("Open Project", this)),
      openM(new QMenu("File", this)),
      behaviorGraphViewIV(NULL),
      dirViewFSW(new FileSelectWindow("Select a hkx file to open!")),
      hkxFile(NULL),
      iconGBLyt(new QVBoxLayout(this)),
      behaviorGraphViewGB(new QGroupBox("Behavior Graph")),
      //objectDataGB(new QGroupBox("Object Data")),
      objectDataWid(new HkDataUI("Object Data")),
      eventsGB(new QGroupBox("Events")),
      variablesGB(new QGroupBox("Variables")),
      logGB(new QGroupBox("Debug Log")),
      logGBLyt(new QVBoxLayout(this)),
      drawGraph(true),
      progressD(/*new QProgressDialog(this)*/NULL)
{
    logGB->setMinimumHeight(300);
    logGB->setMinimumWidth(300);
    openA->setStatusTip("Open a hkx project file!");
    openA->setShortcut(QKeySequence::Open);
    openM->addAction(openA);
    topMB->setMaximumHeight(50);
    topMB->addMenu(openM);
    logGBLyt->addWidget(debugLog);
    logGB->setLayout(logGBLyt);
    topLyt->addWidget(topMB, 0, 0, 1, 10);
    topLyt->addWidget(behaviorGraphViewGB, 1, 0, 6, 6);
    //topLyt->addWidget(objectDataGB, 1, 6, 6, 4);
    topLyt->addWidget(objectDataWid, 1, 6, 6, 4);
    topLyt->addWidget(eventsGB, 7, 0, 3, 3);
    topLyt->addWidget(variablesGB, 7, 3, 3, 3);
    topLyt->addWidget(logGB, 7, 6, 3, 4);
    //progressD->hide();
    readSettings();
    setLayout(topLyt);
    connect(openA, SIGNAL(triggered(bool)), this, SLOT(openDirView()));
    connect(dirViewFSW, SIGNAL(selectFile(QString)), this, SLOT(openHkxfile(QString)));
}

MainWindow::~MainWindow(){
    //
}

void MainWindow::setProgressData(const QString & message, int max, int min, int value){
    if (progressD){
        progressD->setWindowTitle("Opening "+dirViewFSW->getSelectedFilename());
        progressD->setLabelText(message);
        progressD->setMaximum(max);
        progressD->setMinimum(min);
        progressD->setValue(value);
        progressD->resize(geometry().width()*0.15, geometry().height()*0.05);
        progressD->open();
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
}

void MainWindow::setProgressData(const QString & message, int value){
    if (progressD){
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        progressD->setLabelText(message);
        progressD->setValue(value);
        //progressD->resize(geometry().width()*0.15, geometry().height()*0.15);
        //progressD->open();
    }
}

void MainWindow::writeToLog(const QString &message, bool isError){
    QString str;
    QString temp = "\n#########################################";
    if (!isError){
        str = "WARNING!!!-->";
    }else{
        str = "ERROR!!!-->";
    }
    debugLog->appendPlainText(temp+"\n"+str+message+temp);
    QTextCursor tempTC(debugLog->document());
    tempTC.movePosition(QTextCursor::End);
    debugLog->setTextCursor(tempTC);
}

void MainWindow::openDirView(){
    dirViewFSW->show();
}

void MainWindow::openHkxfile(QString name){
    QTime t;
    t.start();
    if (progressD){
        delete progressD;
    }
    progressD = new QProgressDialog(this);
    progressD->setWindowModality(Qt::WindowModal);
    setProgressData("Opening file...", 0, 100, 0);
    drawGraph = true;
    if (!hkxFile){
        setProgressData("Beginning XML parse...", 5);
        hkxFile = new BehaviorFile(this, name);
        if (!drawGraph){
            delete hkxFile;
            hkxFile = NULL;
            return;
        }
        behaviorGraphViewIV = new BehaviorGraphView(objectDataWid, hkxFile);
        behaviorGraphViewGB->setTitle(name);
        setProgressData("Drawing Behavior Graph...", 60);
        if (!behaviorGraphViewIV->drawBehaviorGraph()){
            writeToLog("MainWindow: drawBehaviorGraph() failed!\nThe behavior graph was drawn incorrectly!", true);
        }
    }else{
        setProgressData("Deleting previously loaded Behavior Graph...", 0);
        delete hkxFile;
        delete behaviorGraphViewIV;
        behaviorGraphViewGB->setTitle(name);
        setProgressData("Beginning XML parse...", 5);
        hkxFile = new BehaviorFile(this, name);
        if (!drawGraph){
            delete hkxFile;
            hkxFile = NULL;
            return;
        }
        behaviorGraphViewIV = new BehaviorGraphView(objectDataWid, hkxFile);
        setProgressData("Drawing Behavior Graph...", 90);
        if (!behaviorGraphViewIV->drawBehaviorGraph()){
            writeToLog("MainWindow: drawBehaviorGraph() failed!\nThe behavior graph was drawn incorrectly!", true);
        }
    }
    iconGBLyt->addWidget(behaviorGraphViewIV);
    behaviorGraphViewGB->setLayout(iconGBLyt);
    progressD->setValue(100);
    progressD->done(0);
    progressD->close();
    debugLog->appendPlainText("\n-------------------------\nTime taken to open file \""+dirViewFSW->getSelectedFilename()+"\" is approximately "+QString::number(t.elapsed()/1000)+" seconds\n-------------------------\n");
}

void MainWindow::readSettings()
{
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("Main Window");
    QPoint pos = settings.value("pos", QPoint(500, 500)).toPoint();
    QSize size = settings.value("size", QSize(900, 900)).toSize();
    resize(size);
    move(pos);
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("Main Window");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
}

bool MainWindow::exitProgram()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"), "Quit?", QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok){
        return true;
    }else if (ret == QMessageBox::Cancel){
        return false;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (exitProgram()) {
        writeSettings();
        event->accept();
    }else {
        event->ignore();
    }
}
