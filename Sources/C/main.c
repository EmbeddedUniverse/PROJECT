/*
 * main.c
 */

#include <stdio.h>

#include "test.h"
#include "run.h"

main(void){
    unsigned short command = 0;

    while(1){
        printf("\n----------------------------------------------\n");
        printf("\t1 - Run program | 2 - Test | 3 - Exit\n");
        printf("----------------------------------------------\n");
        printf("\n\nCommand : ");
        scanf("%d", &command);

        if(command == 1)
            run();
        else if(command == 2)
            test();
        else if(command == 3)
            return 0;
        else
            printf("**************INVALID REQUEST**************\n");
    }
}
