#include <stdlib.h>
#include <signal.h>
#include <ncurses.h>
#include <cstdlib>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "tetromino.h"

#define W_X	20	// BoardPane --> 가로(2 * 10)
#define H_Y	20	// BoardPane --> 세로(2 * 10)	

using namespace std;

void checkRows(WINDOW* win);
void reDraw(WINDOW* win);

int g_masterGrid[W_X][H_Y] = {8};
int g_lines;				// Delete Total Line

class Pane {
public:
  int width_, height_;
  int x_, y_;
  WINDOW *win_;

public:
  Pane(int x, int y, int w, int h) :x_(x), y_(y), width_(w), height_(h){
    win_ = newwin(h, w, y, x);
  }
  virtual ~Pane(){
    delwin(win_);
  }
  virtual void draw(){
    box(win_, 0, 0);
    wrefresh(win_);
  }
};


class InfoPane : public Pane {
public:
  InfoPane(int x, int y, int w, int h) : Pane(x,y,w,h){}

  void draw(){

    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    wattron(win_, COLOR_PAIR(2));
    mvwprintw(win_, 0,0,"Your level : 0");
    mvwprintw(win_, 1,0,"Full lines : ");
    mvwprintw(win_, 3,0,"SCORE      : ");

    wattroff(win_, COLOR_PAIR(2));
    wrefresh(win_);
  }
};


class HelpPane : public Pane {

public:

  HelpPane(int x, int y, int w, int h) : Pane(x,y,w,h){}

  void draw(){

    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW,COLOR_BLACK);
    wattron(win_, COLOR_PAIR(6));
    box(win_,0,0);
    wattroff(win_,COLOR_PAIR(6));

    wattron(win_, COLOR_PAIR(2));
    mvwprintw(win_, 1,5, "H E L P");
    mvwprintw(win_, 3,2, "KEY_LEFT :Left");
    mvwprintw(win_, 4,2, "KEY_RIGHT:Right");
    mvwprintw(win_, 5,2, "KEY_UP   :Rotate");
    mvwprintw(win_, 6,2, "KEY_DOWN :Down");
    mvwprintw(win_, 7,2, "KEY_SPACE:DROP");
    wattroff(win_, COLOR_PAIR(2));
    wrefresh(win_);
  }
};

class NextPane : public Pane {

public:
  NextPane(int x, int y, int w, int h) : Pane(x,y,w,h){}

  void draw(){

    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    wattron(win_, COLOR_PAIR(2));

    mvwprintw(win_, 0,5, "NEXT :");
    wattroff(win_, COLOR_PAIR(2));
    wrefresh(win_);
  }
};


class BoardPane : public Pane {

public:

  BoardPane(int x, int y, int w, int h) : Pane(x,y,w,h){}

  void draw(){

    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    wattron(win_, COLOR_PAIR(4));
    mvwvline(win_, 1, 0, ACS_LTEE, H_Y+1);
    mvwvline(win_, 1, W_X+1, ACS_RTEE, H_Y+1);
    mvwhline(win_, H_Y+1, 0, ACS_BTEE, W_X+2);
    wattroff(win_, COLOR_PAIR(4));

    wattron(win_,COLOR_PAIR(5));
    mvwprintw(win_, 22,5, "PLAY TETRIS !");
    wattroff(win_,COLOR_PAIR(5));
    wrefresh(win_);
  }

};

class StatPane : public Pane {

public:
  StatPane(int x, int y, int w, int h) : Pane(x,y,w,h){}
  void draw(){

    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_BLACK);

    wattron(win_, COLOR_PAIR(7));
    mvwprintw(win_, 0, 4, "STATISTICS");
    wattroff(win_, COLOR_PAIR(7));

    for(int i = 1; i < 8; i++)
    {	
        wattron(win_,COLOR_PAIR(i));
        mvwprintw(win_,i*2,15,"---");
        wattroff(win_,COLOR_PAIR(i));
        wrefresh(win_);
    }	

    wattron(win_,COLOR_PAIR(7));
    mvwhline(win_, height_ - 6, 0, ACS_S7, width_);
    mvwprintw(win_,19,5,"SUM   :");
    wattroff(win_,COLOR_PAIR(7));
    wrefresh(win_);

///////////////////////////////////////////////////
 
    WINDOW *win1;
    init_pair(10,COLOR_YELLOW,COLOR_YELLOW);

    win1= newwin(2,4,3,55);
    wbkgd(win1,COLOR_PAIR(10));
    wattron(win1,COLOR_PAIR(10));
    wrefresh(win1);
    wattroff(win1,COLOR_PAIR(10));
 


 
/*
    win6= newwin(2,4,14,63);


    WINDOW *win1;
    init_pair(7, COLOR_MAGENTA,COLOR_MAGENTA);
    win1= newwin(2,2,3,55);
    wbkgd(win1,COLOR_PAIR(7));
    wattron(win1,COLOR_PAIR(7));
    wrefresh(win1);
    win1= newwin(1,4,3,57);
    wbkgd(win1,COLOR_PAIR(7));
    wattron(win1,COLOR_PAIR(7));
    wrefresh(win1);

    WINDOW *win2;
    init_pair(9,COLOR_RED,COLOR_RED);
    win2= newwin(1,8,5,59);
    wbkgd(win2,COLOR_PAIR(9));
    wattron(win2,COLOR_PAIR(9));
    wrefresh(win2);

    WINDOW *win3;
    init_pair(10,COLOR_YELLOW,COLOR_YELLOW);
    win3= newwin(1,6,7,55);
    wbkgd(win3,COLOR_PAIR(10));
    wattron(win3,COLOR_PAIR(10));
    wrefresh(win3);

    win3=newwin(1,2,7,57);
    wbkgd(win3,COLOR_PAIR(10));
    wattron(win3,COLOR_PAIR(10));
    wrefresh(win3);

    WINDOW *win4;
    init_pair(11,COLOR_GREEN,COLOR_GREEN);
    win4= newwin(1,4,9,61);
    wbkgd(win4,COLOR_PAIR(11));
    wattron(win4,COLOR_PAIR(11));
    wrefresh(win4);

    win4=newwin(1,4,10,59);
    wbkgd(win4,COLOR_PAIR(11));
    wattron(win4,COLOR_PAIR(11));
    wrefresh(win4);

    WINDOW *win5;
    init_pair(14,COLOR_CYAN,COLOR_CYAN);
    win5= newwin(1,4,12,55);
    wbkgd(win5,COLOR_PAIR(14));
    wattron(win5,COLOR_PAIR(14));
    wrefresh(win5);
    win5=newwin(1,4,13,57);
    wbkgd(win5,COLOR_PAIR(14));
    wattron(win5,COLOR_PAIR(14));
    wrefresh(win5);

     WINDOW *win7;
    init_pair(16,COLOR_WHITE,COLOR_WHITE);
    win7= newwin(1,4,16,55);
    wbkgd(win7,COLOR_PAIR(16));
    wattron(win7,COLOR_PAIR(16));
    wrefresh(win7);
    win7=newwin(2,2,16,59);
    wbkgd(win7,COLOR_PAIR(16));
    wattron(win7,COLOR_PAIR(16));
    wrefresh(win7);
*/
  }
};

//////////////// Program Main ////////////////

int main()
{
    int nKeyValue;	        // 사용자로 부터 수신한 문자 값 
    int i,j,k;                  // Loop용 변수선언 
    int nShapeID;               // 테트로미노 구분자
    bool bAlive;
 
    int x, y;                   // 테트로미노 가로,세로 좌표 값
    int nBlCreatCount[7] = {0,0,0,0,0,0,0}; // Block별 Count
    int nTotBlCount;            // Total Block Count				  

    Tetromino* curBlock;        // Current Block
    Tetromino* nextBlock;       // Next Block 

    initscr();
    start_color();
    cbreak();
    refresh();

    Pane *infoPane_, *helpPane_, *nextPane_, *boardPane_, *statPane_;

    infoPane_ = new InfoPane(1,1,23,5);
    helpPane_ = new HelpPane(1,6,23,10);
    nextPane_ = new NextPane(1,17,23,6);
    boardPane_= new BoardPane(25,0,W_X+2,H_Y+6); 
    statPane_ = new StatPane(55,1,20,24);

    infoPane_->draw();
    helpPane_->draw();
    nextPane_->draw();
    boardPane_->draw();
    statPane_->draw();

    keypad(stdscr,TRUE);

    for (i = 0; i < W_X; i++)
        for (j = 0; j < H_Y; j++)
            g_masterGrid[i][j] = 8;
 
 //   reDraw(boardPane_->win_);

    srand (time(NULL) );          //uses the time to create randomness

    nShapeID = rand() % 7 + 1;       

    nBlCreatCount[nShapeID-1] = nBlCreatCount[nShapeID-1]+1;
    for(i=1; i<8; i++)
        mvwprintw(statPane_->win_, 2*i,19,"%.1d", nBlCreatCount[i-1]);
 
    mvwprintw(statPane_->win_, 19,16,"1");
    wrefresh(statPane_->win_);

    nKeyValue = 0;

    mvwprintw(infoPane_->win_, 1, 13, "%.1d", g_lines);
    wrefresh(infoPane_->win_);

    curBlock = new Tetromino(boardPane_->win_, nShapeID, 1, 9);
    curBlock->passMaster(g_masterGrid);

    bAlive = true; 
          
    while (nKeyValue != 'q' && bAlive == true) // Match loop
    {
        bool set = false;
        x = 9;
        y = 1;

        werase(nextPane_->win_); // clears the preview window
        wborder(nextPane_->win_, 0, 0, 0, 0, 0, 0, 0, 0);
        wrefresh(nextPane_->win_);
        wrefresh(boardPane_->win_);

        nShapeID = rand() % 7 + 1;
        Tetromino* nextBlock = new Tetromino(nextPane_->win_, nShapeID, 2, 3);
        // 생성하고자 하는 블록영역에 이미 기존 블록영역이 존재하면 bAlive의 값은 false가 된다.
        bAlive = curBlock->spawn(boardPane_->win_, y, x);
        mvwprintw(infoPane_->win_, 1, 13, "%.1d", g_lines);
        mvwprintw(infoPane_->win_, 3, 13, "%.1d", g_lines*30);
        wrefresh(infoPane_->win_);

        // Block loop
        while (nKeyValue != 'q' && set == false) 
   	{
            switch(nKeyValue)
            {
                case KEY_UP:
	             curBlock->tryRotate(boardPane_->win_, y, x);
                     break;
    	        case KEY_LEFT:
                     if(curBlock->tryLeft(boardPane_->win_, y, x))
                     x -= 2;
	             break;  
                case KEY_RIGHT:
                     if(curBlock->tryRight(boardPane_->win_, y, x))
                     x += 2;
		     break;  
                case KEY_DOWN:
	             if (y < H_Y) 			// 현 세로 방향 좌표값이 최대 세로 값보다 작으면...
                     {    
                         if (!curBlock->tryDown(boardPane_->win_, y, x)) 
                         {
                             set = true;
                    	     for (i = 0; i < 4; i++) 
			     {
	                         for (j = 0; j < 4; j++) 
			         {
                            	     if (curBlock->m_block[curBlock->m_rotate][i][j] == 1) 
				     {
                                         g_masterGrid[(y-1)+i][(x-1) + (j*2)] = curBlock->m_blockID;
                                         g_masterGrid[(y-1)+i][(x-1) + (j*2)+1] = curBlock->m_blockID;
                            	     }
                                 }
                    	     }
                  	     checkRows(infoPane_->win_);
                    	     reDraw(boardPane_->win_);
                         } 
                         else 
		         {
                      	      y++;
                         }
		     }    

 //             mvwprintw(infoPane_->win_, 4, 3, "[Y = %.1d][SET = %d]", y, set);
 //             wrefresh(infoPane_->win_);
     		     break;
 		case 0x20:					// Key Space 
		     while (curBlock->tryDown(boardPane_->win_, y, x))  y++;
		     set = true;
		     for (i = 0; i < 4; i++)
                     {
                         for (j = 0; j < 4; j++) 
                         {
                             if(curBlock->m_block[curBlock->m_rotate][i][j] == 1)
                             {
                                g_masterGrid[(y-1) + i][(x-1) + (j*2)] = curBlock->m_blockID;
                                g_masterGrid[(y-1) + i][(x-1) + (j*2) + 1] = curBlock->m_blockID;
                            }
                        }
                    }
		    checkRows(boardPane_->win_);
                    reDraw(boardPane_->win_);
		    break;
	            
                default:
		    break;                  
            }// SWITCH

            infoPane_->draw();
            helpPane_->draw();
            nextPane_->draw();
            boardPane_->draw();
            statPane_->draw();

            nKeyValue=getch();
        }
 
	nTotBlCount =0;
        nBlCreatCount[nShapeID-1] = nBlCreatCount[nShapeID-1]+1;
        for(i=1; i<8; i++)
	{	
            mvwprintw(statPane_->win_, 2*i,19,"%.1d", nBlCreatCount[i-1]);
            nTotBlCount += nBlCreatCount[i-1];
	}	
        mvwprintw(statPane_->win_, 19,16,"%.1d", nTotBlCount);
        wrefresh(statPane_->win_);

        curBlock = nextBlock;
        curBlock->passMaster(g_masterGrid);
 
   } //while root-1


    delete infoPane_;
    delete helpPane_;
    delete nextPane_;
    delete boardPane_;
    delete statPane_;

    endwin();

  return 0;
}


void checkRows(WINDOW* win) 
{
    for (int i = 0; i < W_X; i++)
    {
        bool hole = false;
        for (int j = 0; j < H_Y; j++) 
        {
            if (g_masterGrid[i][j] == 8) hole = true;
        }
        if (hole == false) 
        {
            for (int l = i; l > 0; l--)
            {
                for (int k = 0; k < H_Y; k++)
                {
                    g_masterGrid[l][k] = g_masterGrid[l - 1][k];
                }
            }
            g_lines++;
            mvwprintw(win, 1, 13, "%.1d", g_lines);
            mvwprintw(win, 3, 13, "%.1d", g_lines*30);
            wrefresh(win);

        }
    }
}

void reDraw(WINDOW* win) 
{
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_BLACK);

    for (int i = 0; i < W_X; i ++) 
    {
        for (int j = 0; j < H_Y; j++) 
	{
            wattron(win, COLOR_PAIR(g_masterGrid[i][j]));
            wmove(win, i + 1, j + 1);
            waddch(win, ' '|A_REVERSE);
            int colorz = g_masterGrid[i][j];
            wattroff(win, COLOR_PAIR(g_masterGrid[i][j]));
            wrefresh(win);
        }
    }
}


