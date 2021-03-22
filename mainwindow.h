#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QPushButton>
#include <QRect>
#include <QTime>

#include "pathfinding.h"
#include "reference.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int world[N][N];

    int w = SQUARE / N;
    int status = 0;

    QPushButton *clearButton = new QPushButton("全部清空", this);
    QPushButton *setHinderButton = new QPushButton("设置障碍", this);
    QPushButton *setStartButton = new QPushButton("设置起点", this);
    QPushButton *setEndButton = new QPushButton("设置终点", this);
    QPushButton *runButton = new QPushButton("开始寻路", this);
    QLabel *statusLabel = new QLabel("当前状态: ", this);
    QLabel *currentStatusLabel = new QLabel("设置障碍", this);

    QPoint start;
    QPoint end;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool posToRC(int x, int y, int *resultX, int *resultY);
    bool posInWorld(int x, int y);
    QRect RCToPos(int r, int c);
    bool checkWorld();

    bool pathFinding(QPoint start, QPoint end, int world[N][N]);

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void selfUpdate();
};
#endif // MAINWINDOW_H
