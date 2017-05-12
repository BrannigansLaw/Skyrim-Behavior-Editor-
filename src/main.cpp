#include "src/ui/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
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

    return a.exec();
}
