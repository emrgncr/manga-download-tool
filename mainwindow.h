#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static QDir defaultDownloadDirectory;
    static QString domainLink;

private slots:
    void on_actionchoose_download_location_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
