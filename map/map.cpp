#include "map.h"
#include "obj.h"
#include <algorithm>
#include <stdio.h>
#include <iostream>

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec)
{
    for (auto i : vec)
    {
        os << i << " ";
    }
    return os;
}

CMap::CMap(int h, int w) : height(h), width(w)
{
    // 每个格子10像素，将地图划分为多个格子
    gridWidth = 10;
    gridHeight = 10;

    widthGirdNum = width / gridWidth;
    if (width % gridWidth)
        widthGirdNum++;
    heighGridNum = height / gridHeight;
    if (height % gridHeight)
        heighGridNum++;

    // 初始化格子
    for (int i = 0; i < widthGirdNum; i++)
    {
        for (int j = 0; j < heighGridNum; j++)
        {
            SGrid grid;
            gridData.push_back(grid);

            std::cout << (i * widthGirdNum + j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl
              << std::endl;
}

CMap::~CMap()
{
}

void CMap::enter(CObj *obj)
{
    int index = getScreenIndex(obj->getPos());
    gridData[index].objs.insert(obj);
    allObjs.insert(obj);

    std::vector<int> grids = getSuroundGrids(index);
    enterScreen(obj, {}, grids);
}

void CMap::leave(CObj *obj)
{
    int index = getScreenIndex(obj->getPos());
    gridData[index].objs.erase(obj);
    allObjs.erase(obj);

    std::vector<int> grids = getSuroundGrids(index);
    leaveScreen(obj, grids, {});
}

void CMap::move(CObj *obj, SPoint pos)
{
    obj->setPath(findPath(obj->getPos(), pos));
}

int CMap::getScreenIndex(SPoint pos)
{
    int x = pos.x / gridWidth;
    if (pos.x % gridWidth)
        x++;
    if (x > widthGirdNum)
    {
        std::cout << "错误位置" << std::endl;
        return 0;
    }

    int y = pos.y / gridHeight;
    if (pos.y % gridHeight)
        y++;
    if (y > heighGridNum)
    {
        std::cout << "错误位置" << std::endl;
        return 0;
    }

    return x + y * widthGirdNum;
}

std::vector<int> CMap::getSuroundGrids(int index)
{
    using std::max;
    using std::min;
    std::vector<int> grids;

    int x = index % widthGirdNum;
    int y = index / widthGirdNum;

    // 上三格
    if (y - 1 >= 0)
    {
        if (x - 1 >= 0)
            grids.push_back(x - 1 + (y - 1) * widthGirdNum);

        grids.push_back(x + (y - 1) * widthGirdNum);

        if (x + 1 < widthGirdNum)
            grids.push_back(x + 1 + (y - 1) * widthGirdNum);
    }

    // 中间三格
    if (x - 1 >= 0)
        grids.push_back(x - 1 + y * widthGirdNum);

    grids.push_back(index);

    if (x + 1 < widthGirdNum)
        grids.push_back(x + 1 + y * widthGirdNum);

    // 下三格
    if (y + 1 < widthGirdNum)
    {
        if (x - 1 >= 0)
            grids.push_back(x - 1 + (y + 1) * widthGirdNum);

        grids.push_back(x + (y + 1) * widthGirdNum);

        if (x + 1 < widthGirdNum)
            grids.push_back(x + 1 + (y + 1) * widthGirdNum);
    }

    return grids;
}

std::vector<int> CMap::screenObjs(int index)
{
    using namespace std;
    std::vector<int> objs;

    std::vector<int> grids = getSuroundGrids(index);
    for (auto g : grids)
    {
        for (auto o : gridData[g].objs)
        {
            objs.push_back(o->getUid());
        }
    }

    return objs;
}

void CMap::broadcast(void *msg, std::vector<int> uids)
{
}

std::vector<SPoint> CMap::findPath(const SPoint &start, const SPoint &end)
{
    return std::vector<SPoint>({start, end});
}

void CMap::update(int delta)
{
    for (auto obj : allObjs)
    {
        if (obj->move(delta))
        {
            // 判断是否移动到另一个格子
            int oldIndex = getScreenIndex(obj->getOldPos());
            int newIndex = getScreenIndex(obj->getPos());
            if (oldIndex != newIndex)
            {
                gridData[oldIndex].objs.erase(obj);

                std::vector<int> oldGrids = screenObjs(oldIndex);
                std::vector<int> newGrids = screenObjs(newIndex);
                leaveScreen(obj, oldGrids, newGrids);
                enterScreen(obj, oldGrids, newGrids);

                gridData[newIndex].objs.insert(obj);
            }

            needMove.insert(obj);
        }
    }

    // 同步前端
    for (auto obj : needMove)
    {
        // 获取单位视野内格子
        int index = getScreenIndex(obj->getPos());
        auto objs = screenObjs(index);
        printf("obj %d 同步前端 需要同步的对象:", obj->getUid());
        std::cout << objs << std::endl;
    }
    needMove.clear();
}

// 向前端广播该单位进入屏幕(p.s. 新旧格子集合是有序的)
void CMap::enterScreen(CObj *obj, const std::vector<int> &oldGrids, const std::vector<int> &newGrids)
{
    // 找到新旧格子中的差集
    std::vector<int> needBroadcast;
    std::set_difference(newGrids.begin(), newGrids.end(), oldGrids.begin(), oldGrids.end(), std::back_inserter(needBroadcast));

    // 广播 todo
    printf("obj %d 进入屏幕 需要同步的玩家:", obj->getUid());
    std::cout << needBroadcast << std::endl;
}

// 向前端广播该单位离开屏幕(p.s. 新旧格子集合是有序的)
void CMap::leaveScreen(CObj *obj, const std::vector<int> &oldGrids, const std::vector<int> &newGrids)
{
    // 找到新旧格子中的差集
    std::vector<int> needBroadcast;
    std::set_difference(oldGrids.begin(), oldGrids.end(), newGrids.begin(), newGrids.end(), std::back_inserter(needBroadcast));

    // 广播 todo
    printf("obj %d 离开屏幕 需要同步的玩家:", obj->getUid());
    std::cout << needBroadcast << std::endl;
}