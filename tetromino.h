#include <ncurses.h>

using namespace std;
//#ifndef BLOCK_H
//#define BLOCK_H

#define MAX_X 20	
#define MAX_Y 20       

class Tetromino
{
private:
public:
    int m_block[4][4][4];
    int m_bounds[4][3];
    int m_localMaster[MAX_X][MAX_Y];
    int m_blockID;
    int m_rotate;
    int myColor;

    Tetromino();
    Tetromino(WINDOW* win, int id, int row, int col);
    void passMaster(int master[MAX_X][MAX_Y]);
    bool spawn(WINDOW* win, int row, int col);
    void tryRotate(WINDOW* win, int row, int col);
    void moveRotate(WINDOW* win, int row, int col);

    bool tryDown(WINDOW* win, int row, int col);

    void moveDown(WINDOW* win, int row, int col);
    bool tryRight(WINDOW* win, int row, int col);
    void moveRight(WINDOW* win, int row, int col);
    bool tryLeft(WINDOW* win, int row, int col);
    void moveLeft(WINDOW* win, int row, int col);
    void travPrint(WINDOW* win, int color, int row, int col);

    void setGridBounds();
};
//#endif
