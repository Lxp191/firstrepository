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
    // ÿ������10���أ�����ͼ����Ϊ�������
    gridWidth = 10;
    gridHeight = 10;

    widthGirdNum = width / gridWidth;
    if (width % gridWidth)
        widthGirdNum++;
    heighGridNum = height / gridHeight;
    if (height % gridHeight)
        heighGridNum++;

    // ��ʼ������
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
        std::cout << "����λ��" << std::endl;
        return 0;
    }

    int y = pos.y / gridHeight;
    if (pos.y % gridHeight)
        y++;
    if (y > heighGridNum)
    {
        std::cout << "����λ��" << std::endl;
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

    // ������
    if (y - 1 >= 0)
    {
        if (x - 1 >= 0)
            grids.push_back(x - 1 + (y - 1) * widthGirdNum);

        grids.push_back(x + (y - 1) * widthGirdNum);

        if (x + 1 < widthGirdNum)
            grids.push_back(x + 1 + (y - 1) * widthGirdNum);
    }

    // �м�����
    if (x - 1 >= 0)
        grids.push_back(x - 1 + y * widthGirdNum);

    grids.push_back(index);

    if (x + 1 < widthGirdNum)
        grids.push_back(x + 1 + y * widthGirdNum);

    // ������
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
            // �ж��Ƿ��ƶ�����һ������
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

    // ͬ��ǰ��
    for (auto obj : needMove)
    {
        // ��ȡ��λ��Ұ�ڸ���
        int index = getScreenIndex(obj->getPos());
        auto objs = screenObjs(index);
        printf("obj %d ͬ��ǰ�� ��Ҫͬ���Ķ���:", obj->getUid());
        std::cout << objs << std::endl;
    }
    needMove.clear();
}

// ��ǰ�˹㲥�õ�λ������Ļ(p.s. �¾ɸ��Ӽ����������)
void CMap::enterScreen(CObj *obj, const std::vector<int> &oldGrids, const std::vector<int> &newGrids)
{
    // �ҵ��¾ɸ����еĲ
    std::vector<int> needBroadcast;
    std::set_difference(newGrids.begin(), newGrids.end(), oldGrids.begin(), oldGrids.end(), std::back_inserter(needBroadcast));

    // �㲥 todo
    printf("obj %d ������Ļ ��Ҫͬ�������:", obj->getUid());
    std::cout << needBroadcast << std::endl;
}

// ��ǰ�˹㲥�õ�λ�뿪��Ļ(p.s. �¾ɸ��Ӽ����������)
void CMap::leaveScreen(CObj *obj, const std::vector<int> &oldGrids, const std::vector<int> &newGrids)
{
    // �ҵ��¾ɸ����еĲ
    std::vector<int> needBroadcast;
    std::set_difference(oldGrids.begin(), oldGrids.end(), newGrids.begin(), newGrids.end(), std::back_inserter(needBroadcast));

    // �㲥 todo
    printf("obj %d �뿪��Ļ ��Ҫͬ�������:", obj->getUid());
    std::cout << needBroadcast << std::endl;
}