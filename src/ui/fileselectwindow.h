#ifndef FILESELECTWINDOW_H
#define FILESELECTWINDOW_H

#include <QWidget>

class QGridLayout;
class QFormLayout;
class QTreeView;
class QFileSystemModel;
class QLineEdit;
class QPushButton;

class FileSelectWindow: public QWidget
{
    Q_OBJECT

public:
    FileSelectWindow(QString windowName);
    virtual ~FileSelectWindow();
    QString getSelectedFilename();
private:
    QGridLayout *topLyt;
    QFormLayout *fileInfoLyt;
    QTreeView *dirViewTV;
    QFileSystemModel *fileSysFSM;
    QLineEdit *filenameLE;
    QPushButton *openPB;
    QPushButton *cancelPB;
signals:
    void selectFile(QString path);
private slots:
    void select();
    void cancel();
};

#endif // FILESELECTWINDOW_H
