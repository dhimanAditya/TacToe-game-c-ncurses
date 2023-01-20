#include <ncurses.h>
#include <stdlib.h>


WINDOW  * titleBar,
        * gameWin,
        * infoBar,
        * statusBar;


void askForGame();


int main()
{
    initscr();
    raw();
    noecho();
    curs_set(0);


    start_color();

    init_color(COLOR_BLACK, 400, 400, 400);

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLUE, COLOR_YELLOW);
    init_pair(3, COLOR_RED, COLOR_WHITE);
    init_pair(4, COLOR_YELLOW, COLOR_BLUE);
    

    titleBar = newwin(3, COLS, 0, 0);
    gameWin = newwin(5, 11, (LINES-5)/2, (COLS-11)/2);
    statusBar = newwin(1, COLS, LINES-1, 0);
    infoBar = newwin(3, COLS, LINES-6, 0);


    keypad(stdscr, true);
    keypad(gameWin, true);


    bkgd(COLOR_PAIR(1));
    wbkgd(titleBar, COLOR_PAIR(2));
    wbkgd(gameWin, COLOR_PAIR(3));
    wbkgd(infoBar, COLOR_PAIR(1));
    wbkgd(statusBar, COLOR_PAIR(4));


    refresh();


    wattron(titleBar, A_BOLD);
    mvwprintw(titleBar, 1, (COLS-15)/2, "o - Tac&Toe - x");
    wattroff(titleBar, A_BOLD);

    wrefresh(titleBar);
    
    wattron(infoBar, COLOR_PAIR(3));
    mvwprintw(infoBar, 0, (COLS-27)/2, "$ ./Welcome");
    wattroff(infoBar, COLOR_PAIR(3));
    mvwprintw(infoBar, 1, (COLS-27)/2, "> Program  :  Tac&Toe Game");
    mvwprintw(infoBar, 2, (COLS-27)/2, "> Creator  :  Aditya Dhiman");
    
    wrefresh(infoBar);


    askForGame();


    return 0;
}


void renderGrid(char matrix[3][3])
{
    mvwprintw(gameWin, 0, 0, " %c | %c | %c ", matrix[0][0], matrix[0][1], matrix[0][2]);
    mvwprintw(gameWin, 1, 0, "———————————");
    mvwprintw(gameWin, 2, 0, " %c | %c | %c ", matrix[1][0], matrix[1][1], matrix[1][2]);
    mvwprintw(gameWin, 3, 0, "———————————");
    mvwprintw(gameWin, 4, 0, " %c | %c | %c ", matrix[2][0], matrix[2][1], matrix[2][2]);

    wrefresh(gameWin);
}


void newGame()
{
    char matrix[3][3] = {
        { ' ', ' ', ' ' },
        { ' ', ' ', ' ' },
        { ' ', ' ', ' ' }
    };

    int play, turn, y, x;
    play = turn = y = x = 1;
    
    char text[27] = "Game Over ! DRAW";


    for (int i = 0; (i < 9) && play;)
    {
        mvwprintw(statusBar, 0, (COLS-15)/2, "Player '%c' Turn", turn ? 'o': 'x');
        wrefresh(statusBar);
        
        int s;
        do
        {
            renderGrid(matrix);

            switch(s)
            {
                case KEY_LEFT:
                    x -= (x > 0) ? 1: 0;
                    break;
                case KEY_RIGHT:
                    x += (x < 2) ? 1: 0;
                    break;
                case KEY_UP:
                    y -= (y > 0) ? 1: 0;
                    break;
                case KEY_DOWN:
                    y += (y < 2) ? 1: 0;
                    break;
                case KEY_END:
                    play = 0;
                    strcpy(text, "Game Terminated");
            }

            mvwchgat(gameWin, 2*y, 4*x, 3, A_NORMAL, 4, NULL);
        }
        while (play && (s = wgetch(gameWin)) != '\n');

        if (matrix[y][x] != ' ' || !play) continue;

        matrix[y][x] = turn ? 'o': 'x';

        renderGrid(matrix);


        if (matrix[0][x] == matrix[1][x] && matrix[1][x] == matrix[2][x] && matrix[2][x] == matrix[0][x] || matrix[y][0] == matrix[y][1] && matrix[y][1] == matrix[y][2] && matrix[y][2] == matrix[y][0] || y == x && matrix[0][0] == matrix[1][1] && matrix[1][1] == matrix[2][2] && matrix[2][2] == matrix[0][0] || (y + x) == 2 && matrix[0][2] == matrix[1][1] && matrix[1][1] == matrix[2][0] && matrix[2][0] == matrix[0][2])
        {
            play = 0;
            sprintf(text, "Game Over ! Player '%c' Win", (turn ? 'o' : 'x'));
        }

        turn = !turn;
        i++;
    }
    
    
    werase(infoBar);
    wattron(infoBar, COLOR_PAIR(3));
    mvwprintw(infoBar, 2, (COLS-strlen(text)-4)/2, " > %s ", text);
    wattroff(infoBar, COLOR_PAIR(3));
    wrefresh(infoBar);
    
    
    askForGame();
}


void askForGame()
{
    mvwprintw(statusBar, 0, (COLS-28)/2, "new game [Enter] or exit [End]");
    wrefresh(statusBar);
    

    while (1)
    {
        int ch = getch();

        if (ch == '\n')
            break;
        else if (ch == KEY_END) {
            endwin();
            exit(0);
        }
    }
    

    werase(statusBar);


    werase(infoBar);
    
    mvwprintw(infoBar, 0, (COLS-42)/2, "[Arrow keys]  :  To move the pointer");
    mvwprintw(infoBar, 1, (COLS-42)/2, "[Enter]       :  To select the pointed grid");
    mvwprintw(infoBar, 2, (COLS-42)/2, "[End]         :  To terminate the game");
    wrefresh(infoBar);


    newGame();
}