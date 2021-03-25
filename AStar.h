#ifndef ASTAR_H
#define ASTAR_H

#include "reference.h"

// Map< parentPosititon, childPosition >
typedef QMap<QString, QString> RouteMap;

static Position minScore(PositionList &set, PositionScoreMap &fMap)
{
    int fTemp = -1;
    Position pTemp;

    for (Position p : set) {
        int value = fMap.value(p.toString());

        if (fTemp == -1) {
            fTemp = value;
            pTemp = p;
            continue;
        } else {
            if (value < fTemp) {
                fTemp = value;
                pTemp = p;
            }
        }
    }

    return pTemp;
}

static PositionList neighbourPositions(Position position, int world[N][N])
{
    PositionList result;
    for (int row = -1; row <= 1; row++)
        for (int col = -1; col <= 1; col++) {
            Position tempPosition(position.row - row, position.col - col);
            if (tempPosition.worldValue(world) == CAN_NOT_MOVE)
                continue;
            if (tempPosition == position)
                continue;
            result << tempPosition;
        }
    return result;
}

static bool aStar(Position start, Position end, int world[N][N], PositionList *result)
{
    PositionList closeSet;
    PositionList openSet;
    openSet << start;

    RouteMap cameForm;

    PositionScoreMap gScore;
    PositionScoreMap hScore;
    PositionScoreMap fScore;

    QString stringStart = start.toString();

    gScore.insert(stringStart, 0);
    hScore.insert(stringStart, heuristic(start, end));
    fScore.insert(stringStart, hScore.value(stringStart));

    while (!openSet.empty()) {
        Position x = minScore(openSet, fScore);
        if (x == end) {
            QString currentPosition = end.toString();
            QString stringStartPosition = start.toString();

            QString parentPosition = cameForm.value(currentPosition);
            while (parentPosition != stringStartPosition) {
                result->push_front(Position(parentPosition));
                parentPosition = cameForm.value(parentPosition);
            }

            return true;
        }

        openSet.removeOne(x);
        closeSet << x;

        for (Position y : neighbourPositions(x, world)) {
            if (closeSet.contains(y))
                continue;

            QString stringX = x.toString();
            QString stringY = y.toString();

            int tentativeGScore = gScore[stringX] + distance(x, y);
            bool tentativeIsBetter;

            if (!openSet.contains(y))
                tentativeIsBetter = true;
            else if (tentativeGScore < gScore[stringY])
                tentativeIsBetter = true;
            else
                tentativeIsBetter = false;

            if (tentativeIsBetter) {
                cameForm.insert(stringY, stringX);
                gScore.insert(stringY, tentativeGScore);
                hScore.insert(stringY, heuristic(y, end));
                fScore.insert(stringY, gScore.value(stringY) + hScore.value(stringY));
                openSet << y;
            }
        }
    }

    return false;
}

#endif // ASTAR_H
