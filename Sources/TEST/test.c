/*
 * test.c
 *
 *  
 *      
 */

#include "../../Includes/test.h"

#define S_PASS "PASS"
#define S_FAIL "FAIL"

//WARNING:  -Never change the values of the enum values!
//          -Only add.
//          -NUMBER_UNIT_TESTS must stay last without specific value.
enum unitTest { MATHEMATIC = 0,
                NUMBER_UNIT_TESTS };

//WARNING:  -Never change the values of the enum values!
//          -Only add.
//          -NUMBER_UNIT_TESTS must stay last without specific value.
//enum integrationTest {  ... };

bool unitTest(){
    bool result = false;

    FILE *f = fopen("unit_test_results.txt", "w");
    if (f == NULL){
        printf("ERROR OPENING FILE : unit_test_results.txt \n");
        exit(1);
    }

    int i;

    for(i = 0; i < NUMBER_UNIT_TESTS; i++){
        switch(i){
            case MATHEMATIC:
                result = unitTestMathematic();
                break;
        }
        fprintf(f, "%s\n", (result ? S_PASS : S_FAIL));
    }

    return false;
}

bool integrationTest(){
    //TODO: Must be coded in the same way as the function unitTest(), replace the line below.
    return false;
}
