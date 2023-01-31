#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket;

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slot_ready_read);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    socket->connectToHost("127.0.0.1", 9999);

    color = QColor(255,0,0);

    ui->textEdit->setText("ФОНАРЬ");
    ui->textEdit->setAlignment(Qt::AlignCenter);

}

MainWindow::~MainWindow()
{
    if (socket->state() == QTcpSocket::ConnectedState)
    {
        socket->disconnectFromHost();
    }
    delete ui;
}

void MainWindow::slot_ready_read()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    if (in.status() == QDataStream::Ok)
    {
        quint16 nextBlockSize = 0;
        while(true)
        {
            if (nextBlockSize == 0)
            {
                if (socket->bytesAvailable() < 2)
                {
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                break;
            }

            in >> Data;
            if (!Data.isEmpty() && nextBlockSize > 6)
            {
                char *bytes_arr = Data.data();
                quint8  type = bytes_arr[0];
                switch (type)
                {
                case 0x12:
                    on_flishlight();
                    break;
                case 0x13:
                    off_flishlight();
                    break;
                case 0x20:
                    if (Data.size() >= (sizeof(quint8) + sizeof(quint16)))
                    {
                        quint16 len  = static_cast<quint16>(bytes_arr[1] << 8) | (bytes_arr[2]);
                        if (Data.size() >= (sizeof(quint8) + sizeof(quint16) + len))
                        {
                            color = QColor(static_cast<int>(bytes_arr[3]),
                                           static_cast<int>(bytes_arr[4]),
                                           static_cast<int>(bytes_arr[5]));

                            set_color_flishlight(color);
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}

void MainWindow::on_flishlight()
{
    set_color_flishlight(color);
}
void MainWindow::off_flishlight()
{
    set_color_flishlight();
}
void MainWindow::set_color_flishlight(QColor color)
{
    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Base, color);
    ui->textEdit->setPalette(p);
}
