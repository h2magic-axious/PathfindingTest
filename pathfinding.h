#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <QMap>
#include <QPoint>
#include <QVector>

#include "reference.h"
class Node
{
public:
    QPoint point;
    Node *parent;

    Node()
    {
        point = QPoint(-1, -1);
        parent = nullptr;
    }
    Node(QPoint pt, Node *pr) : point(pt), parent(pr) {}

    ~Node() = default;

    QString toString() { return QString("%1,%2").arg(point.x()).arg(point.y()); }
};

typedef QVector<Node> PointList;
typedef QMap<QString, int> PointScoreMap;

bool static checkPoint(QPoint p)
{
    int x = p.x();
    int y = p.y();
    bool bx = x >= 0 && x < N;
    bool by = y >= 0 && y < N;
    return bx && by;
}

bool static pointInSet(QPoint p, const PointList &set)
{
    for (Node tempPoint : set)
        if (tempPoint.point == p)
            return true;

    return false;
}

int static h(QPoint p1, QPoint p2)
{
    int dx = qAbs(p1.x() - p2.x());
    int dy = qAbs(p1.y() - p2.y());
    return 10 * (dx + dy);
}

Node static minScore(const PointList &list, const PointScoreMap &fm)
{
    Node temp = list[0];
    for (int i = 1; i < list.size(); i++) {
        Node p = list[i];
        if (fm.value(temp.toString()) > fm.value(p.toString()))
            temp = p;
    }
    return temp;
}

PointList static neighbour(Node p,
                           int world[N][N],
                           const PointList &openList,
                           const PointList &closeList,
                           PointScoreMap *gm)
{
    int x = p.point.x();
    int y = p.point.y();

    PointList result;

    int value = gm->value(p.toString());

    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++) {
            QPoint pTemp = QPoint(x + i, y + j);

            if (pTemp == p.point)
                continue;
            if (world[pTemp.x()][pTemp.y()] == CAN_NOT_MOVE)
                continue;
            if (pointInSet(pTemp, openList))
                continue;
            if (pointInSet(pTemp, closeList))
                continue;
            if (!checkPoint(pTemp))
                continue;

            Node pt(pTemp, &p);
            int dx = qAbs(i) + qAbs(j);
            gm->insert(pt.toString(), value + (dx == 2 ? 14 : 10));
            result.push_back(Node(pTemp, &p));
        }
    return result;
}

bool static aStar(QPoint start, QPoint end, int world[N][N])
{
    PointList openSet;
    PointList closeSet;

    PointScoreMap fScoreMap;
    PointScoreMap hScoreMap;
    PointScoreMap gScoreMap;

    Node s(start, nullptr);

    openSet.push_back(s);
    closeSet.push_back(s);

    gScoreMap.insert(s.toString(), 0);
    hScoreMap.insert(s.toString(), 0);
    fScoreMap.insert(s.toString(), 0);

    Node tempPoint = s;
    QVector<QPoint> result;

    while (!openSet.empty()) {
        // 从OpenSet中取出F值最小的一个放入CloseSet
        tempPoint = minScore(openSet, fScoreMap);
        qDebug() << tempPoint.toString();

        // 如果tempPoint是终点，则得出结果
        if (tempPoint.point == end) {
            // 从终点开始，依此取得父节点，并添加在resut的头部
            while (tempPoint.parent->point != start) {
                result.push_front(tempPoint.parent->point);
                tempPoint = *tempPoint.parent;
            }
            // 在图形界面中显示
            for (QPoint p : result) {
                world[p.x()][p.y()] = ROUTE;
            }
            return true;
        }

        openSet.pop_back();
        closeSet.push_back(tempPoint);

        // 计算tempPoint的所有邻居，然后遍历，（该列表经过处理后没有冗余项）
        // neighbour函数返回的List中每一项的父节点都是tempPoint
        PointList tempPointList = neighbour(tempPoint, world, openSet, closeSet, &gScoreMap);
        for (Node p : tempPointList) {
            int tempH = h(p.point, end);
            hScoreMap.insert(p.toString(), tempH);
            fScoreMap.insert(p.toString(), tempH + gScoreMap.value(p.toString()));
            openSet.push_back(p);
        }
    }

    return false;
}

#endif // PATHFINDING_H
