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
    QSize sizeHint() const Q_DECL_OVERRIDE;
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
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
    void readSettings();
    void writeSettings();
};

#endif // FILESELECTWINDOW_H
