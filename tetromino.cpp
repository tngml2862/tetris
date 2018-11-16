#include "tetromino.h"
#include "shape.h"

using namespace std;

Tetromino::Tetromino(WINDOW* win, int id, int row, int col)
{
    m_rotate = 0;
    m_blockID = id;
    setGridBounds();
    travPrint(win, m_blockID, row, col);
    wrefresh(win);
}

void Tetromino::setGridBounds()
{
    switch(m_blockID)
    {
	case 1:
	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 4; j++)
		     m_block[m_rotate][i][j] = o_block[m_rotate][i][j];
     	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 3; j++)
		     m_bounds[i][j] = o_bounds[i][j];
              break;
	case 2:
    	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 4; j++)
		     m_block[m_rotate][i][j] = l_block[m_rotate][i][j];

	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 3; j++)
		     m_bounds[i][j] = l_bounds[i][j];
              break;
	case 3:
	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 4; j++)
		     m_block[m_rotate][i][j] = j_block[m_rotate][i][j];

	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 3; j++)
		     m_bounds[i][j] = j_bounds[i][j];
              break;
	case 4:
	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 4; j++)
		      m_block[m_rotate][i][j] = s_block[m_rotate][i][j];

	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 3; j++)
		      m_bounds[i][j] = s_bounds[i][j];
              break;
	case 5:
	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 4; j++)
		      m_block[m_rotate][i][j] = z_block[m_rotate][i][j];

	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 3; j++)
		      m_bounds[i][j] = z_bounds[i][j];
              break;
	case 6:
	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 4; j++)
		      m_block[m_rotate][i][j] = t_block[m_rotate][i][j];

	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 3; j++)
		      m_bounds[i][j] = t_bounds[i][j];
              break;
	case 7:
	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 4; j++)
		      m_block[m_rotate][i][j] = i_block[m_rotate][i][j];

	      for (int i = 0; i < 4; i++)
		  for (int j = 0; j < 3; j++)
		      m_bounds[i][j] = i_bounds[i][j];
              break;
     }
}

void Tetromino::passMaster(int master[MAX_X][MAX_Y])
{
    for (int i = 0; i < MAX_X; i++)
    {
        for (int j = 0; j < MAX_Y; j++)
        {
            m_localMaster[i][j] = master[i][j];
        }
    }
}

bool Tetromino::spawn(WINDOW* win, int row, int col)
{
    bool collision = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_block[m_rotate][i][j] == 1)
            {
                if (m_localMaster[(row - 1) + i][(col - 1) + (j * 2)] != 8)
                    collision = true;
            }
        }
    }
    travPrint(win, m_blockID, row, col);
    return !collision;
}

void Tetromino::tryRotate(WINDOW* win, int row, int col)
{
    bool collision = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_block[m_rotate][i][j] == 2)
            {
                int y = (row - 1) + i;
                int x = (col - 1) + (j * 2);
                int tmp = m_localMaster[(row - 1) + i][(col - 1) + (j * 2)];
                if (tmp != 8 || x < 0 || x > 19 || y > 21)
                    collision = true;
            }
        }
    }
    if (collision == false)
        this->moveRotate(win, row, col);
}

void Tetromino::moveRotate(WINDOW* win, int row, int col)
{
    travPrint(win, 8, row, col);
    if (m_rotate < 3)
        this->m_rotate++;
    else
        this->m_rotate = 0;
    setGridBounds();
    travPrint(win, m_blockID, row, col);
}

bool Tetromino::tryDown(WINDOW* win, int row, int col)
{
    bool collision = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_block[m_rotate][i][j] == 1)
            {
                if (m_localMaster[row + i][(col - 1) + (j * 2)] != 8)
                    collision = true;
            }
        }
    }

    if (row > (MAX_Y - m_bounds[m_rotate][1]) || collision == true)
    {
        return false;
    }
    else
    {
        this->moveDown(win, row, col);
        return true;
    }
}

void Tetromino::moveDown(WINDOW* win, int row, int col)
{
    travPrint(win, 8, row++, col);
    travPrint(win, m_blockID, row, col);
}

bool Tetromino::tryRight(WINDOW* win, int row, int col)
{
    bool collision = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_block[m_rotate][i][j] == 1)
            {
                if (m_localMaster[(row - 1) + i][(col - 1) + (j * 2) + 2] != 8)
                    collision = true;
            }
        }
    }
    if ((col + (m_bounds[m_rotate][2] * 2)) >= MAX_X || collision == true)
    {
        return false;
    }

    else
    {
        this->moveRight(win, row, col);
        return true;
    }
}

void Tetromino::moveRight(WINDOW* win, int row, int col)
{
    travPrint(win, 8, row, col);
    col += 2;
    travPrint(win, m_blockID, row, col);
}

bool Tetromino::tryLeft(WINDOW* win, int row, int col)
{
    bool collision = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_block[m_rotate][i][j] == 1)
            {
                if (m_localMaster[(row - 1) + i][(col - 1) + (j * 2) - 1] != 8)
                    collision = true;
            }
        }
    }
    if ((col + (m_bounds[m_rotate][0] * 2)) <= 1 || collision == true)
    {
        return false;
    }
    else
    {
        this->moveLeft(win, row, col);
        return true;
    }
}

void Tetromino::moveLeft(WINDOW* win, int row, int col)
{
    travPrint(win, 8, row, col);
    col -= 2;
    travPrint(win, m_blockID, row, col);
}

void Tetromino::travPrint(WINDOW *win, int color, int row, int col)
{
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_BLACK);

    for (int i = 0; i < 4; i++)
    {
        int tmpcol = col;
        for (int j = 0; j < 4; j++)
        {
            if (m_block[m_rotate][i][j] == 1)
            {
                wattron(win, COLOR_PAIR(color));
                wmove(win, row, tmpcol);
                waddch(win, ' '|A_REVERSE);
                wmove(win, row, tmpcol + 1);
                waddch(win, ' '|A_REVERSE);
                wattroff(win, COLOR_PAIR(color));
                wrefresh(win);
            }
            tmpcol += 2;
        }
        row++;
    }
}

