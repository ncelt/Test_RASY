#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slot_ready_read();

private:
    Ui::MainWindow *ui = nullptr;
    QTcpSocket *socket = nullptr;

    QByteArray Data;

    QColor color;

    void on_flishlight();
    void off_flishlight();
    void set_color_flishlight(QColor color = QColor(255,255,255));
};
#endif // MAINWINDOW_H
