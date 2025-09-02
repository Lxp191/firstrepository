#include "obj.h"
#include <math.h>

void CObj::setPath(std::vector<SPoint> &&path)
{
    this->path = path;
    moving = 1;
    pathIndex = 0;
}

bool CObj::move(int delta)
{
    // 非移动状态直接返回
    if (!moving)
    {
        return false;
    }

    oldPos = pos;

    // 移动中判断当前位置
    auto &nextPos = path[pathIndex + 1];

    // 计算上一帧位置到下一个目标位置的距离和需要的时间
    int distance = sqrt((nextPos.x - pos.x) * (nextPos.x - pos.x) + (nextPos.y - pos.y) * (nextPos.y - pos.y));
    int moveTime = distance / speed;

    if (moveTime > delta)
    {
        // 移动到下一帧位置
        pos.x += (nextPos.x - pos.x) * delta / moveTime;
        pos.y += (nextPos.y - pos.y) * delta / moveTime;
    }
    else
    {
        // 移动到下一个目标位置
        pos = nextPos;
        pathIndex++;

        if (pathIndex + 1 == path.size())
        {
            path.clear();
            moving = 0;
            pathIndex = 0;
        }
        else
        {
            move(delta - moveTime);
        }
    }

    return true;
}