#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <menu.h>
#include <stdlib.h>           // added for exit() function

#include "modules/debugmenu/launcher.h" //added for program launcher
#ifdef CONFIG_DEBUG_USER1
#include "modules/debugmenu/user/user1.h"
#endif
#ifdef CONFIG_DEBUG_USER2
#include "modules/debugmenu/user/user2.h"
#endif
#ifdef CONFIG_DEBUG_USER3
#include "modules/debugmenu/user/user3.h"
#endif
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

    char *choices[10] = {};
    int choice;
    int selected=0;
    int choicearrsize = -1;
    #ifdef CONFIG_DEBUG_FILEBROWSER
    	choices[0] = "File browser";
    	choicearrsize++;
    #endif
    #ifdef CONFIG_DEBUG_USER1
    	choices[choicearrsize+1] = user1_menuitem_name;
    	choicearrsize++;
    #endif
    #ifdef CONFIG_DEBUG_USER2
    	choices[choicearrsize+1] = user2_menuitem_name;
    	choicearrsize++;
    #endif
    #ifdef CONFIG_DEBUG_USER3
    	choices[choicearrsize+1] = user3_menuitem_name;
    	choicearrsize++;
    #endif
    choices[choicearrsize+1] = "Bail out";
    choicearrsize++;
    while(1)//Refreshing the menu constantly
    {
        for(i = 0; i < choicearrsize+1; i++) {
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
            if(selected > choicearrsize) selected = choicearrsize;
            break;
        default:
            break;
        }

        if(choice==10) break;
    }
    endwin();
    launcher(choices[selected]);
    
    return 0;
}


