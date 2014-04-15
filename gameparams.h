#ifndef GAMEPARAMS_H
#define GAMEPARAMS_H

#include <vector>
using namespace std;

class GameParams{
public:
    GameParams();
    virtual ~GameParams();
    int colorCount();
    int boardSize();
    int level();    // from 0 to 4
    void setLevel(const int& level);    // from 0 to 4

private:
    const vector<int> boardSizes = { 12, 10, 8, 8, 8 };
    const vector<int> boardColorCounts = { 5, 5, 5, 6, 7 };
    int m_colorCount;
    int m_size;
    int m_level;
};

#endif // GAMEPARAMS_H
