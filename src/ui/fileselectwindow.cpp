#include "fileselectwindow.h"

#include <QLabel>
#include <QFileSystemModel>
#include <QLayout>
#include <QFormLayout>
#include <QTreeView>
#include <QPushButton>
#include <QLineEdit>
#include <QSettings>
#include <QCloseEvent>

FileSelectWindow::FileSelectWindow(QString windowName){
    setWindowTitle(windowName);
    topLyt = new QGridLayout(this);
    fileInfoLyt = new QFormLayout(this);
    filenameLE = new QLineEdit;
    fileInfoLyt->addRow(new QLabel("File name: "), filenameLE);
    fileSysFSM = new QFileSystemModel(this);
    fileSysFSM->setRootPath("C:/Users/Wayne/Desktop");
    dirViewTV = new QTreeView(this);
    dirViewTV->setModel(fileSysFSM);
    dirViewTV->setRootIndex(fileSysFSM->setRootPath("C:/Users/Wayne/Desktop"));
    fileSysFSM->setNameFilters(QStringList("*.hkx"));
    fileSysFSM->setNameFilterDisables(false);
    openPB = new QPushButton("Open");
    cancelPB = new QPushButton("Cancel");
    readSettings();
    topLyt->addWidget(dirViewTV, 0, 0, 8, 10);
    topLyt->addLayout(fileInfoLyt, 8, 0, 1, 10);
    topLyt->addWidget(openPB, 9, 6, 1, 2);
    topLyt->addWidget(cancelPB, 9, 8, 1, 2);
    connect(openPB, SIGNAL(pressed()), this, SLOT(select()));
    connect(cancelPB, SIGNAL(pressed()), this, SLOT(cancel()));
    hide();
}

FileSelectWindow::~FileSelectWindow(){
    //
}

QSize FileSelectWindow::sizeHint() const{
    return QSize(800, 800);
}

QString FileSelectWindow::getSelectedFilename(){
    return filenameLE->text();
}

void FileSelectWindow::select(){
    filenameLE->setText(fileSysFSM->fileName(dirViewTV->currentIndex()));
    emit selectFile(fileSysFSM->filePath(dirViewTV->currentIndex()));
    hide();
}

void FileSelectWindow::cancel(){
    hide();
}

void FileSelectWindow::readSettings()
{
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("File Select Window");
    QPoint pos = settings.value("pos", QPoint(500, 500)).toPoint();
    QSize size = settings.value("size", QSize(900, 900)).toSize();
    resize(size);
    move(pos);
    settings.endGroup();
}

void FileSelectWindow::writeSettings()
{
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("File Select Window");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
}

void FileSelectWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}
