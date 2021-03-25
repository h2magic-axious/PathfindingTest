#ifndef REFERENCE_H
#define REFERENCE_H

#if _MSC_VER >= 1600
#pragma execution_character_set(“utf - 8”)
#endif

#define N 20
#define DX 10

#define START_X 10
#define START_Y 40
#define SQUARE 800

#define HINDER 0
#define RUN 1

#define CAN_MOVE 0
#define CAN_NOT_MOVE 1
#define START_POS 2
#define END_POS 3
#define ROUTE 4

#include <QDebug>
#include <QFile>
#include <QList>
#include <QMap>

#include <QSet>
#include <QString>

class Position
{
public:
    int row;
    int col;

    int d;

    Position()
    {
        row = 0;
        col = 0;
        d = 0;
    }
    Position(int r, int c) : row(r), col(c) { d = row * row + col * col; }
    Position(QString stringPosition)
    {
        QStringList stringList = stringPosition.split(',');
        row = stringList.value(0).toInt();
        col = stringList.value(1).toInt();
        d = row * row + col * col;
    }
    ~Position() = default;

    bool operator<(const Position &other) { return d < other.d; }
    bool operator==(const Position &other) { return (row == other.row) && (col == other.col); }
    bool operator!=(const Position &other) { return (row != other.row) || (col != other.col); }
    QString toString() { return QString("%1,%2").arg(row).arg(col); }

    int worldValue(int world[N][N]) { return world[row][col]; }
};

typedef QList<Position> PositionList;
typedef QMap<QString, int> PositionScoreMap;

class Node
{
public:
    Position position;
    Node *parent;

    Node()
    {
        position = Position();
        parent = nullptr;
    }
    Node(Position pt, Node *pr) : position(pt), parent(pr) {}

    ~Node() = default;

    bool operator==(const Node &other)
    {
        return position == other.position && parent == other.parent;
    }
    bool operator!=(const Node &other)
    {
        return position != other.position || parent != other.parent;
    }
    QString toString()
    {
        if (parent)
            return position.toString() + ";" + parent->position.toString();
        else
            return position.toString();
    }
};

static bool checkPoint(Position p)
{
    return (p.col >= 0 && p.col < N) && (p.row >= 0 && p.row < N);
}

static int heuristic(Position p1, Position p2)
{
    int dx = qAbs(p1.col - p2.col);
    int dy = qAbs(p1.row - p2.row);

    int hStraight = dx + dy;
    int hDiagonal = qMin(dx, dy);

    return 14 * hDiagonal + 10 * (hStraight - 2 * hDiagonal);
}

static int distance(Position p1, Position p2)
{
    int dx = qAbs(p1.col - p2.col);
    int dy = qAbs(p1.row - p2.row);

    if (dx + dy == 1)
        return 10;
    if (dx + dy == 2)
        return 14;

    return 0;
}

#endif // REFERENCE_H
