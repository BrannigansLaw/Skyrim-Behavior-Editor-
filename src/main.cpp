#include "src/ui/mainwindow.h"
#include <QApplication>

#include <QColor>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.setStyleSheet("QWidget {background: blue;font-weight: bold; color: red}QComboBox {background: yellow}");
    w.setWindowTitle("Skyrim Behavior Tool");
    w.show();

    /*ulong linecount = 0;
    QDirIterator dirIt("C:/Users/Wayne/Documents/Skyrim-Behavior-Editor-", QDirIterator::Subdirectories);
    while (dirIt.hasNext()){
        dirIt.next();
        if (QFileInfo(dirIt.filePath()).isFile()){
            if (QFileInfo(dirIt.filePath()).suffix() == "cpp" || QFileInfo(dirIt.filePath()).suffix() == "h"){
                QFile file(dirIt.filePath());
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                    return 0;
                }
                while (!file.atEnd()) {
                    file.readLine();
                    linecount++;
                }
            }
        }
    }
    CRITICAL_ERROR_MESSAGE(QString(QString::number(linecount)))*/

    /*int numFiles = 100000;
    QProgressDialog progress("Copying files...", "Abort Copy", 0, numFiles, 0);
    progress.setWindowModality(Qt::WindowModal);
    for (int i = 0; i < numFiles; i++) {
        progress.setValue(i);
        if (progress.wasCanceled())
            break;
    }
    progress.setValue(numFiles);*/
    /*QProgressDialog dialog("Opening project...", "", 0, 100);
    dialog.setMinimumDuration(0);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setLabelText("Character data loaded sucessfully!!!");
    dialog.setValue(40);
    dialog.setLabelText("Loading skeleton data...");
    dialog.setValue(50);
    dialog.setLabelText("Project loaded sucessfully!!!");
    dialog.setValue(dialog.maximum());*/
    return a.exec();
}
