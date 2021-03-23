#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "reference.h"

typedef QSet<Node *> PointSet;
typedef QList<Position> PositionList;
typedef QMap<QString, int> PointScoreMap;

bool static checkPoint(Position p)
{
    return (p.col >= 0 && p.col < N) && (p.row >= 0 && p.row < N);
}

int static h(Position p1, Position p2)
{
    return 10 * (qAbs(p1.col - p2.col) + qAbs(p1.row - p2.row));
}

Node static *minScore(const PointSet &list, const PointScoreMap &fm, QTextStream *logRepoter)
{
    //    *logRepoter << "Find min f-value point. Open set size: " << list.size() << '\n';
    int fValue = 0;
    Node *result = nullptr;
    for (Node *p : list) {
        int pValue = fm.value(p->position.toString());

        *logRepoter << "    Check: " << p->position.toString() << " Score: " << pValue << '\n';

        if (!pValue) {
            *logRepoter << "  Get min f-value point: " << p->position.toString() << '\n';
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
    *logRepoter << "  Get min f-value point: " << result->position.toString() << '\n';
    return result;
}

bool static checkLegalPoint(Node *node, int status, const PointSet &closeSet)
{
    bool canMove = (status != 1);
    bool inWorld = checkPoint(node->position);
    bool notInCloseSet = !closeSet.contains(node);
    return canMove && inWorld && notInCloseSet;
}

PointSet static neighbour(
    Node *p, int world[N][N], const PointSet &closeList, PointScoreMap *gm, QTextStream *logRepoter)
{
    *logRepoter << "  Ergodic all neighbour: " << p->position.toString() << "\n";

    int row = p->position.row;
    int col = p->position.col;

    PointSet result;

    int value = gm->value(p->position.toString());

    *logRepoter << "    Legal point: ";
    for (int r = -1; r <= 1; r++)
        for (int c = -1; c <= 1; c++) {
            if (!r && !c)
                continue;

            Position pTemp = Position(row + r, col + c);
            Node *pt = new Node(pTemp, p);

            if (checkLegalPoint(pt, pTemp.worldValue(world), closeList)) {
                *logRepoter << pTemp.toString() << ' ';

                int pValue = (qAbs(r) + qAbs(c) == 2) ? 14 : 10;
                gm->insert(pTemp.toString(), value + pValue);
                result << pt;
            }
        }
    *logRepoter << '\n';
    return result;
}

bool static aStar(
    Position start, Position end, int world[N][N], PositionList *result, QTextStream *logRepoter)
{
    PointSet openSet;
    PointSet closeSet;

    PointScoreMap fScoreMap;
    PointScoreMap hScoreMap;
    PointScoreMap gScoreMap;

    Node *s = new Node(start, nullptr);

    qDebug() << start.toString();

    openSet << s;

    gScoreMap.insert(start.toString(), 0);
    hScoreMap.insert(start.toString(), 0);
    fScoreMap.insert(start.toString(), 0);

    while (!openSet.empty()) {
        // 从OpenSet中取出F值最小的一个放入CloseSet
        Node *tempPoint = minScore(openSet, fScoreMap, logRepoter);
        openSet.remove(tempPoint);
        closeSet << tempPoint;

        // 如果tempPoint是终点，则得出结果
        if (tempPoint->position == end) {
            result->clear();
            while (tempPoint->parent->position != start) {
                tempPoint = tempPoint->parent;
                result->push_front(tempPoint->position);
            }
            return true;
        }

        *logRepoter << "Remove " << tempPoint->position.toString() << " Put into Close set\n";
        *logRepoter << "OpenSet size: " << openSet.size() << '\n';
        *logRepoter << "CloseSet size: " << closeSet.size() << '\n';

        // 计算tempPoint的所有邻居，然后遍历，（该列表经过处理后没有冗余项）
        // neighbour函数返回的List中每一项的父节点都是tempPoint
        PointSet tempPointList = neighbour(tempPoint, world, closeSet, &gScoreMap, logRepoter);
        for (Node *p : tempPointList) {
            if (openSet.contains(p))
                continue;

            int tempH = h(p->position, end);
            QString positionString = p->position.toString();
            hScoreMap.insert(positionString, tempH);
            fScoreMap.insert(positionString, tempH + gScoreMap.value(positionString));

            openSet << p;
        }
    }

    return false;
}

#endif // PATHFINDING_H
