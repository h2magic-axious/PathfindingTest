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
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void selfUpdate();
};
#endif // MAINWINDOW_H
