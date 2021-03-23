#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QRect>
#include <QTime>

#include "pathfinding.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int world[N][N];

    int w = SQUARE / N;
    int status = 0;

    QFile *logFile = new QFile("C:\\Users\\H2MAGIC\\Desktop\\log.txt");
    QTextStream *logReporter;

    QPushButton *clearButton = new QPushButton("全部清空", this);
    QPushButton *runButton = new QPushButton("开始寻路", this);
    QLabel *statusLabel = new QLabel("当前状态: ", this);
    QLabel *currentStatusLabel = new QLabel("设置障碍", this);

    Position start;
    Position end;
    PositionList route;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool positionToRowCol(int x, int y, int *row, int *col);
    bool positionInWorld(int x, int y);
    QRect rowColToIndex(int r, int c);
    bool checkWorld();

    bool pathFinding(Position start, Position end, int world[N][N], PositionList *result);

    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void selfUpdate();
};
#endif // MAINWINDOW_H
