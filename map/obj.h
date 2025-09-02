#pragma once

#include <vector>

#include "../core/buildin/buildin.h"

class CObj
{
public:
    CObj(int uid, int speed, SPoint pos) : uid(uid), speed(speed), pos(pos),
                                           moving(0), pathIndex(0), totalMoveTime(0),
                                           path(std::vector<SPoint>({pos})) {}
    bool move(int delta);

    const int getUid() const { return uid; };

    void setPath(std::vector<SPoint> &&path);

    const SPoint &getOldPos() { return oldPos; };
    const SPoint &getPos() { return pos; };

private:
    int uid;

    int speed;
    SPoint oldPos;
    SPoint pos;

    int moving;
    int pathIndex;
    int totalMoveTime;
    std::vector<SPoint> path;
};