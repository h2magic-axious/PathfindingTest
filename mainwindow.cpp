#include "mainwindow.h"

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
    runButton->move(100, 0);
    statusLabel->move(200, 0);

    currentStatusLabel->setStyleSheet("color:blue;");
    currentStatusLabel->move(600, 0);

    connect(clearButton, &QPushButton::clicked, this, [=]() {
        status = HINDER;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                world[i][j] = CAN_MOVE;
        selfUpdate();
    });
    connect(runButton, &QPushButton::clicked, this, [=]() {
        status = RUN;
        if (!checkWorld()) {
            QMessageBox::critical(this, "错误", "请只设置一个起点和一个终点");
            return;
        }

        if (pathFinding(start, end, world, &route)) {
            for (Position p : route)
                world[p.row][p.col] = ROUTE;
        } else {
            QMessageBox::information(this, "未找到", "未找到任何路径");
            return;
        }
        status = HINDER;
        selfUpdate();
    });
}

MainWindow::~MainWindow()
{
    
}

bool MainWindow::positionToRowCol(int x, int y, int *row, int *col)
{
    if (!positionInWorld(x, y))
        return false;

    *col = (x - START_X) / w;
    *row = (y - START_Y) / w;
    return true;
}

bool MainWindow::positionInWorld(int x, int y)
{
    bool bx = x > START_X && x < START_X + SQUARE;
    bool by = y > START_Y && y < START_Y + SQUARE;
    return bx && by;
}

QRect MainWindow::rowColToIndex(int r, int c)
{
    return QRect(START_X + c * w, START_Y + r * w, w, w);
}

bool MainWindow::checkWorld()
{
    int sum = 0;
    int temp = 0;
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++) {
            temp = world[row][col];
            if (temp == CAN_NOT_MOVE || temp == CAN_MOVE || temp == ROUTE)
                continue;
            sum += world[row][col];
        }
    return sum == 5;
}

bool MainWindow::pathFinding(Position start, Position end, int world[N][N], PositionList *result)
{
    // coding
    // all route mark with GREEN, look like: world[i][j] = ROUTE;
    // if find successfully, return true.
    return aStar(start, end, world, result);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter;
    painter->begin(this);

    int worldTemp = 0;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            QRect tempRect = rowColToIndex(i, j);
            worldTemp = world[i][j];

            if (worldTemp == CAN_MOVE)
                painter->setBrush(Qt::white);
            if (worldTemp == CAN_NOT_MOVE)
                painter->setBrush(Qt::black);
            if (worldTemp == START_POS)
                painter->setBrush(Qt::red);
            if (worldTemp == END_POS)
                painter->setBrush(Qt::blue);
            if (worldTemp == ROUTE)
                painter->setBrush(Qt::green);

            painter->drawRect(tempRect);
            //painter->drawText(tempRect, QString("%1,%2").arg(i).arg(j));
        }

    painter->end();
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Qt::MouseButtons b = event->buttons();
    int posX = event->x();
    int posY = event->y();
    int row = -1;
    int col = -1;
    if (!positionToRowCol(posX, posY, &row, &col))
        return;

    if (b == Qt::LeftButton) {
        world[row][col] = START_POS;
        start = Position(row, col);
    }
    if (b == Qt::RightButton) {
        world[row][col] = END_POS;
        end = Position(row, col);
    }

    selfUpdate();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int posX = event->x();
    int posY = event->y();
    int col = -1;
    int row = -1;
    if (!positionToRowCol(posX, posY, &row, &col))
        return;

    if (status == HINDER)
        world[row][col] = CAN_NOT_MOVE;

    selfUpdate();
}

void MainWindow::selfUpdate()
{
    switch (status) {
    case HINDER:
        currentStatusLabel->setText("设置障碍");
        break;
    case RUN:
        currentStatusLabel->setText("开始寻路");
        break;
    }
    update();
}
