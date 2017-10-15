#include "src/ui/mainwindow.h"
#include <QApplication>

#include <QColor>
#include <QPushButton>
#include "src/animData/skyrimanimdata.h"
#include "src/animSetData/skyrimanimsetdata.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //SkyrimAnimData *data = new SkyrimAnimData();
    //SkyrimAnimSetData *data = new SkyrimAnimSetData();
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
    (qFatal(QString::number(linecount)))*/

    return a.exec();
}
