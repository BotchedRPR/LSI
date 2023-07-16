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

    initscr(); //Inizjalizacja całości ncurses, kolory itp
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

    //init_pair(1, COLOR_BLUE, COLOR_BLACK); //wybór kolorów

    getmaxyx(stdscr, hei, len); //pobranie rozmiarów terminalu
    move(hei/2, (len-strlen(greeting))/2); //przesuwamy kursor na środek (tak aby się ładnie wydrukowało)
    //attron(COLOR_PAIR(1)); //Aktywujemy wybrane kolory
    printw(greeting); //Drukujemy powitanie
    //attroff(COLOR_PAIR(1));//Dezaktywujemy kolory
    refresh();//Odswiezamy (inaczej się nie wyswietli)
    WINDOW * menuwin=newwin(7, len-12, hei-9, 6); //Definiujemy i tworzymy 'okno'
    box(menuwin, 0, 0);
    refresh();//ponownie odświeżamy aby okno się pojawiło
    wrefresh(menuwin);//odświeżamy samo okno
    keypad(menuwin, TRUE);//umozliwiamy dzialanie klawiatury w oknie

    char *choices[] = {
       	"Test 1",
        "Test 2",
        "Test 3",
        "Test 4",
        "Bail out",
    };
    int choice;
    int selected=0;

    while(1)//cala ta petla sluzy ciaglemu tworzeniu menu z podswietleniem wybranego elementu
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
            if(selected < 0) selected = 0;//zabezpieczenie przed wyjsciem "poza" menu
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


