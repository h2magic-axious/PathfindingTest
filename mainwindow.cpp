#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QString("Pathfinding Test"));
    setGeometry(300, 300, SQUARE + 2 * DX, SQUARE + DX + START_Y);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            world[i][j] = CAN_MOVE;
    }

    clearButton->move(0, 0);
    setHinderButton->move(100, 0);
    setStartButton->move(200, 0);
    setEndButton->move(300, 0);
    runButton->move(400, 0);
    statusLabel->move(500, 0);

    currentStatusLabel->setStyleSheet("color:blue;");
    currentStatusLabel->move(600, 0);

    connect(clearButton, &QPushButton::clicked, this, [=]() {
        status = HINDER;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                world[i][j] = CAN_MOVE;
        selfUpdate();
    });
    connect(setHinderButton, &QPushButton::clicked, this, [=]() {
        status = HINDER;
        selfUpdate();
    });
    connect(setStartButton, &QPushButton::clicked, this, [=]() {
        status = START_POINT;
        selfUpdate();
    });
    connect(setEndButton, &QPushButton::clicked, this, [=]() {
        status = END_POINT;
        selfUpdate();
    });
}

MainWindow::~MainWindow()
{
}

bool MainWindow::posToRC(int x, int y, int *resultX, int *resultY)
{
    if (!posInWorld(x, y))
        return false;

    *resultX = (y - START_Y) / w;
    *resultY = (x - START_X) / w;
    return true;
}

bool MainWindow::posInWorld(int x, int y)
{
    bool bx = x > START_X && x < START_X + SQUARE;
    bool by = y > START_Y && y < START_Y + SQUARE;
    return bx && by;
}

QRect MainWindow::RCToPos(int r, int c)
{
    return QRect(START_X + c * w, START_Y + r * w, w, w);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter;
    painter->begin(this);

    int worldTemp = 0;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            QRect tempRect = RCToPos(i, j);
            worldTemp = world[i][j];

            if (worldTemp == CAN_MOVE)
                painter->setBrush(Qt::white);
            if (worldTemp == CAN_NOT_MOVE)
                painter->setBrush(Qt::black);
            if (worldTemp == START_POS)
                painter->setBrush(Qt::red);
            if (worldTemp == END_POS)
                painter->setBrush(Qt::blue);

            painter->drawRect(tempRect);
            //painter->drawText(tempRect, QString("%1,%2").arg(i).arg(j));
        }

    painter->end();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    Qt::MouseButtons b = event->buttons();
    int posX = event->x();
    int posY = event->y();
    int tempX = -1;
    int tempY = -1;
    if (!posToRC(posX, posY, &tempX, &tempY))
        return;
    //qDebug() << QString("%1,%2").arg(posX).arg(posY) << ','
    //<< QString("%1,%2").arg(tempX).arg(tempY);
    if (status == START_POINT)
        world[tempX][tempY] = START_POS;
    if (status == END_POINT)
        world[tempX][tempY] = END_POS;

    selfUpdate();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    Qt::MouseButtons b = event->buttons();
    int posX = event->x();
    int posY = event->y();
    int tempX = -1;
    int tempY = -1;
    if (!posToRC(posX, posY, &tempX, &tempY))
        return;

    if (status == HINDER)
        world[tempX][tempY] = CAN_NOT_MOVE;

    selfUpdate();
}

void MainWindow::selfUpdate()
{
    switch (status) {
    case HINDER:
        currentStatusLabel->setText("设置障碍");
        break;
    case START_POINT:
        currentStatusLabel->setText("设置起点");
        break;
    case END_POINT:
        currentStatusLabel->setText("设置终点");
        break;
    case RUN:
        currentStatusLabel->setText("开始寻路");
        break;
    }
    update();
}
