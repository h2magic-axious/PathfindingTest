#ifndef PATHFINDING_H
#define PATHFINDING_H

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

    QString toString() { return QString("[%1,%2]").arg(point.y()).arg(point.x()); }

    bool operator==(const Node &other) { return point == other.point; }
};

typedef QSet<Node *> PointSet;
typedef QMap<QString, int> PointScoreMap;

bool static checkPoint(QPoint p)
{
    int x = p.x();
    int y = p.y();
    bool bx = x >= 0 && x < N;
    bool by = y >= 0 && y < N;
    return bx && by;
}

bool static pointInSet(QPoint p, const PointSet &set)
{
    for (Node *tempPoint : set)
        if (tempPoint->point == p)
            return true;

    return false;
}

int static h(QPoint p1, QPoint p2)
{
    int dx = qAbs(p1.x() - p2.x());
    int dy = qAbs(p1.y() - p2.y());
    return 10 * (dx + dy);
}

Node static *minScore(const PointSet &list, const PointScoreMap &fm, QTextStream *logRepoter)
{
    //    *logRepoter << "Find min f-value point. Open set size: " << list.size() << '\n';
    int fValue = 0;
    Node *result = nullptr;
    for (Node *p : list) {
        int pValue = fm.value(p->toString());

        *logRepoter << "    Check: " << p->toString() << " Score: " << pValue << '\n';

        if (!pValue) {
            *logRepoter << "  Get min f-value point: " << p->toString() << '\n';
            return p;
        }

        if (!fValue) {
            fValue = pValue;
            result = p;
            continue;
        }

        if (pValue < fValue) {
            fValue = pValue;
            result = p;
        }
    }
    *logRepoter << "  Get min f-value point: " << result->toString() << '\n';
    return result;
}

bool static checkLegalPoint(const QPoint &point, int status, const PointSet &closeSet)
{
    bool canMove = (status != 1);
    bool inWorld = checkPoint(point);
    bool notInCloseSet = !pointInSet(point, closeSet);
    return canMove && inWorld && notInCloseSet;
}

PointSet static neighbour(
    Node *p, int world[N][N], const PointSet &closeList, PointScoreMap *gm, QTextStream *logRepoter)
{
    *logRepoter << "  Ergodic all neighbour: " << p->toString() << "\n";
    int x = p->point.x();
    int y = p->point.y();

    PointSet result;

    int value = gm->value(p->toString());

    *logRepoter << "    Legal point: ";
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++) {
            if (!i && !j)
                continue;

            QPoint pTemp = QPoint(y + j, x + i);
            QString stringPoint = QString("[%1,%2]").arg(pTemp.x()).arg(pTemp.y());

            if (checkLegalPoint(pTemp, world[pTemp.x()][pTemp.y()], closeList)) {
                *logRepoter << stringPoint << ' ';
                int pValue = (qAbs(i) + qAbs(j) == 2) ? 14 : 10;
                Node *pt = new Node(pTemp, p);
                gm->insert(pt->toString(), value + pValue);
                result << pt;
            }
        }
    *logRepoter << '\n';
    return result;
}

bool static aStar(
    QPoint start, QPoint end, int world[N][N], QList<QPoint> *result, QTextStream *logRepoter)
{
    PointSet openSet;
    PointSet closeSet;

    PointScoreMap fScoreMap;
    PointScoreMap hScoreMap;
    PointScoreMap gScoreMap;

    Node *s = new Node(start, nullptr);

    openSet << s;

    gScoreMap.insert(s->toString(), 0);
    hScoreMap.insert(s->toString(), 0);
    fScoreMap.insert(s->toString(), 0);

    while (!openSet.empty()) {
        // 从OpenSet中取出F值最小的一个放入CloseSet
        Node *tempPoint = minScore(openSet, fScoreMap, logRepoter);

        // 如果tempPoint是终点，则得出结果
        if (tempPoint->point == end) {
            result->clear();
            while (tempPoint->parent->point != start) {
                tempPoint = tempPoint->parent;
                result->push_front(tempPoint->point);
            }
            return true;
        }

        *logRepoter << "Remove " << tempPoint->toString() << " Put into Close set\n";

        openSet.remove(tempPoint);
        closeSet << tempPoint;

        // 计算tempPoint的所有邻居，然后遍历，（该列表经过处理后没有冗余项）
        // neighbour函数返回的List中每一项的父节点都是tempPoint
        PointSet tempPointList = neighbour(tempPoint, world, closeSet, &gScoreMap, logRepoter);
        for (Node *p : tempPointList) {
            if (openSet.contains(p))
                continue;

            int tempH = h(p->point, end);
            hScoreMap.insert(p->toString(), tempH);
            fScoreMap.insert(p->toString(), tempH + gScoreMap.value(p->toString()));
            openSet << p;
        }
    }

    return false;
}

#endif // PATHFINDING_H
