#ifndef MAP
#define MAP

#include <vector>
#include <set>
#include "../core/buildin/buildin.h"

class CObj;

struct SGrid
{
    std::set<CObj *> objs;
};

class CMap
{
public:
    CMap(int h, int w);
    ~CMap();

    void enter(CObj *obj);
    void leave(CObj *obj);

    void move(CObj *obj, SPoint pos);

    int getScreenIndex(SPoint pos);
    std::vector<int> getSuroundGrids(int index);

    std::vector<int> screenObjs(int index);

    void update(int delta);

    void broadcast(void *msg, std::vector<int> uids);
    void leaveScreen(CObj *obj, const std::vector<int> &oldGrids, const std::vector<int> &newGrids);
    void enterScreen(CObj *obj, const std::vector<int> &oldGrids, const std::vector<int> &newGrids);

private:
    std::vector<SPoint> findPath(const SPoint &start, const SPoint &end);

    int height;
    int width;

    int heighGridNum;
    int widthGirdNum;

    int gridWidth;
    int gridHeight;

    std::vector<SGrid> gridData;
    std::set<CObj *> needMove;
    std::set<CObj *> needBroadcast;
    std::set<CObj *> allObjs;
};

#endif