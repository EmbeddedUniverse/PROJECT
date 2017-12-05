/*
 * main.c
 */

#include <stdio.h>
#include "test.h"
#include "run.h"
#include "Menu.h"


int clean_stdin()
{
    while (getchar()!='\n');
    return 1;
}


main(void){
    unsigned short command = 0;

    while(1){
        printf("\n------------------------------------------------------\n");
        printf("\t    Embedded Universe's Software");
        printf("\n------------------------------------------------------\n");
        printf("\t1 - Run program | 2 - Test | 3 - Exit");
        printf("\n------------------------------------------------------\n");
        printf("\n\n->Command : ");
        scanf("%d", &command);

        if(command == 1)
            run();
            //runMenu();
        else if(command == 2)
            test();
        else if(command == 3)
            return 0;
        else
            printf("**************INVALID REQUEST**************\n");
    }
}
