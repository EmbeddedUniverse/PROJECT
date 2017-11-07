/*
 * test.c
 *
 *  
 *      
 */


#include "test.h"
#include "unitTest.h"
#include "integrationTest.h"

void (*unitTestSelected[]) (void) = {
   //Add unit tests below
   templateTest,
   autocorrelateTest,
   correlateTest
};

char* unitTestName[][30] = {
   //Add unit test's names below
                        {"templateTest"},
                        {"autocorrelateTest"},
                        {"correlateTest"}
};

void (*integrationTestSelected[]) (void) = {
   //Add integration tests below
   templateTest
};

char* integrationTestName[][30] = {
   //Add integration test's names below
                                 {"templateTest"}
};

void test(){

    unsigned short command = 0;

    printf("\n----------------------------------------------\n");
    printf("\tTESTING\n");

    while(1){
        printf("----------------------------------------------\n");
        printf("\t1 - Specific test\n");
        printf("\t2 - All unit test\n");
        printf("\t3 - All integration test\n");
        printf("\t4 - Test all\n");
        printf("\t5 - Go back\n");
        printf("----------------------------------------------\n");
        printf("\n->Command : ");
        scanf("%d", &command);

        if(command == 1)
            specificTest();
        else if(command == 2)
            testSelection(ALL_UNIT_TESTS);
        else if(command == 3)
            testSelection(ALL_INTEGRATION_TESTS);
        else if(command == 4)
            testSelection(ALL_TESTS);
        else if(command == 5)
            return;
        else{
            printf("\n*******************************************\n");
            printf("**************INVALID REQUEST**************");
            printf("\n*******************************************\n\n");
        }
    }
}

void specificTest(){

    unsigned short command, nbTests, i;
    command = 0;

    printf("\n----------------------------------------------\n");
    printf("\t SPECIFIC TEST\n");

    while(1){
        printf("----------------------------------------------\n");
        printf("\t1 - Unit test\n");
        printf("\t2 - Integration test\n");
        printf("\t3 - Go back\n");
        printf("----------------------------------------------\n");
        printf("\n->Command : ");
        scanf("%d", &command);

        if(command == 1){
            printf("\n----------------------------------------------\n");
            printf("\t UNIT TEST\n");
            printf("----------------------------------------------\n");
            nbTests = sizeof(unitTestSelected)/sizeof(void (*) (void));
            for(i = 0 ; i < nbTests ; i++)
                printf("\t%d - %s\n", i, unitTestName[i][0]);
            printf("----------------------------------------------\n");
            printf("\n->Command : ");
            scanf("%d", &command);
            callSpecificUnitTest(command);
        }
        else if(command == 2){
            printf("\n----------------------------------------------\n");
            printf("\t INTEGRATION TEST\n");
            printf("----------------------------------------------\n");
            nbTests = sizeof(integrationTestSelected)/sizeof(void (*) (void));
            for(i = 0 ; i < nbTests ; i++)
                printf("\t%d - %s\n", i, integrationTestName[i][0]);
            printf("----------------------------------------------\n");
            printf("\n->Command : ");
            scanf("%d", &command);
            callSpecificIntegrationTest(command);
        }
        else if(command == 3)
            return ;
        else{
            printf("\n*******************************************\n");
            printf("**************INVALID REQUEST**************");
            printf("\n*******************************************\n\n");
        }
    }
}

bool callSpecificUnitTest(unsigned short command){
    if(command <= sizeof(unitTestSelected)/sizeof(void (*) (void))){
        do
            (*unitTestSelected[command])();
        while(askIfTestAgain());
        return OK;
    }
    else{
        printf("\n*******************************************\n");
        printf("**************INVALID REQUEST**************");
        printf("\n*******************************************\n\n");
    }
    return ERROR;
}

bool callSpecificIntegrationTest(unsigned short command){
    if(command <= sizeof(integrationTestSelected)/sizeof(void (*) (void))){
        do
            (*integrationTestSelected[command])();
        while(askIfTestAgain());
        return OK;
    }
    else{
        printf("\n*******************************************\n");
        printf("**************INVALID REQUEST**************");
        printf("\n*******************************************\n\n");
    }
    return ERROR;
}

bool testSelection(testType type){

    unsigned short nbTests, nTest;

    do {
        if(type == ALL_UNIT_TESTS){
            printf("----------------------------------------------\n");
            printf("\t ALL UNIT TESTS\n");
            nbTests = sizeof(unitTestSelected)/sizeof(void (*) (void));
            for(nTest = 0; nTest<nbTests; nTest++){
                (*unitTestSelected[nTest])();
            }
        }
        else if(type == ALL_INTEGRATION_TESTS){
            printf("----------------------------------------------\n");
            printf("\t ALL INTEGRATION TESTS\n");
            nbTests = sizeof(integrationTestSelected)/sizeof(void (*) (void));
            for(nTest = 0; nTest<nbTests; nTest++){
                (*integrationTestSelected[nTest])();
            }
        }
        else if(type == ALL_TESTS){
            printf("----------------------------------------------\n");
            printf("\t ALL TESTS\n");
            nbTests = sizeof(unitTestSelected)/sizeof(void (*) (void));
            for(nTest = 0; nTest<nbTests; nTest++){
                (*unitTestSelected[nTest])();
            }

            nbTests = sizeof(integrationTestSelected)/sizeof(void (*) (void));
            for(nTest = 0; nTest<nbTests; nTest++){
                (*integrationTestSelected[nTest])();
            }
        }
        else
            return ERROR;
    } while (askIfTestAgain());

    return OK;
}

void printTestResult(char title[], char  author[], char  reviser[], char description[], int ID, char result[]){
    printf("\n----------------------------------------------\n");
    printf("TEST : %s\n", title);
    printf("AUTHOR : %s\n", author);
    printf("REVISER : %s\n", reviser);
    printf("DESC : %s\n", description);
    printf("ID : %d\n", ID);
    printf("RESULT : %s\n", result);
    printf("----------------------------------------------\n");
}

bool askIfTestAgain(){
    char command = 0;
    bool validAnswer = false;
    do{
        printf("\n\n||||||||||||||||||||||||||||||||||||||||||||||||||\n");
        printf("\t TEST AGAIN ?  Yes = 1 OR No = 0 ?\n");
        printf("||||||||||||||||||||||||||||||||||||||||||||||||||\n");
        printf("\n->Command : ");
        scanf("%d", &command);
        printf("\n");
        if(command == 0 || command == 1)
            validAnswer = OK;
    }while(!validAnswer);

    return command;
}

