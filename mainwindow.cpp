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
    setHinderButton->move(100, 0);
    runButton->move(200, 0);
    statusLabel->move(300, 0);

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
    connect(runButton, &QPushButton::clicked, this, [=]() {
        status = RUN;
        if (!checkWorld()) {
            QMessageBox::critical(this, "错误", "请只设置一个起点和一个终点");
            return;
        }

        if (!pathFinding(start, end, world)) {
            QMessageBox::information(this, "未找到", "未找到任何路径");
            return;
        }

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

    *resultX = (x - START_X) / w;
    *resultY = (y - START_Y) / w;
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

bool MainWindow::checkWorld()
{
    int sum = 0;
    int temp = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            temp = world[i][j];
            if (temp == 1 || temp == 0)
                continue;
            sum += world[i][j];
        }
    return sum == 5;
}

bool MainWindow::pathFinding(QPoint start, QPoint end, int world[N][N])
{
    // coding
    // all route mark with GREEN, look like: world[i][j] = ROUTE;
    // if find successfully, return true.

    return aStar(start, end, world);
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
            if (worldTemp == ROUTE)
                painter->setBrush(Qt::green);

            painter->drawRect(tempRect);
            painter->drawText(tempRect, QString("%1,%2").arg(j).arg(i));
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
    if (!posToRC(posX, posY, &col, &row))
        return;

    qDebug() << QString("world[%1][%2] = %2").arg(row).arg(col).arg(world[row][col]);

    if (b == Qt::LeftButton) {
        world[row][col] = START_POS;
        start = QPoint(row, col);
    }
    if (b == Qt::RightButton) {
        world[row][col] = END_POS;
        end = QPoint(row, col);
    }
    selfUpdate();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int posX = event->x();
    int posY = event->y();
    int col = -1;
    int row = -1;
    if (!posToRC(posX, posY, &col, &row))
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
