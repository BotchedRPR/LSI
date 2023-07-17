#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <menu.h>
#include <stdlib.h>           // added for exit() function

void fail(char *msg) {
    endwin();
    puts(msg);
    exit(EXIT_FAILURE);
}

int show()
{
    int i;
    char greeting[]="LSI - Debug menu component";
    int hei, len; 

    initscr(); //ncurses init
    raw();
    noecho();
    keypad(stdscr, TRUE);

    /* Test to see if terminal has colors */
    if (has_colors() == false) {
        fail("Colors unavailable\n");
    }

    if (start_color() != OK) {
        fail("Unable to start colors\n");
    }

    getmaxyx(stdscr, hei, len); //Get terminal dimensions
    move(hei/2, (len-strlen(greeting))/2); //Moving cursor
    //attron(COLOR_PAIR(1)); //Colors
    printw(greeting); //Print out LSI - Debug menu component
    //attroff(COLOR_PAIR(1));//No more colors
    refresh();//Refreshing
    WINDOW * menuwin=newwin(7, len-12, hei-9, 6); //Creating window
    box(menuwin, 0, 0);
    refresh();//Refreshing so that the window shows up
    wrefresh(menuwin);//Refreshing just the window
    keypad(menuwin, TRUE);//Enabling keyboard in menu

    char *choices[] = {
       	"Test 1",
        "Test 2",
        "Test 3",
        "Test 4",
        "Bail out",
    };
    int choice;
    int selected=0;

    while(1)//Refreshing the menu constantly
    {
        for(i = 0; i < 5; i++) {
            if(i == selected)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i+1, 1, choices[i]);
            if (i == selected)
                wattroff(menuwin, A_REVERSE);
        }

        choice = wgetch(menuwin);
        switch(choice)
        {
        case KEY_UP:
            selected--;
            if(selected < 0) selected = 0;//Make sure the cursor is in menu bounds
            break;
        case KEY_DOWN:
            selected++;
            if(selected > 4) selected = 4;
            break;
        default:
            break;
        }

        if(choice==10) break;
    }

    printw("\nWybrano:%s", choices[selected]);
    refresh();

    /* Wait for user to press enter to exit */
    getch();

    /* Need to cleanup before exit */
    endwin();

    return 0;
}


