#ifndef GAMEPARAMS_H
#define GAMEPARAMS_H

#include <vector>
using namespace std;

class GameParams{
public:
    GameParams();
    virtual ~GameParams();
    int colorCount() const;
    int boardSize() const;
    int moves() const;
    int level() const;    // from 0 to 4
    void setLevel(const int level);    // from 0 to 4

private:
    const vector<int> boardSizes = {4, 12, 10, 8, 8, 8 }; //il primo 'e per test
    const vector<int> boardColorCounts = {4, 5, 5, 5, 6, 7};
    const vector<int> numMoves = {30, 30, 20, 10, 10, 10};
    int m_colorCount;
    int m_size;
    int m_level;
    int m_moves;
};

#endif // GAMEPARAMS_H
