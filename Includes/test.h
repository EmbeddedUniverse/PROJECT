/*
 * Holds the tests.
 * test.h
 *
 */
 
#ifndef INCLUDES_TEST_H_
#define INCLUDES_TEST_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define PASS "PASS"
#define FAIL "FAIL"

#define ERROR 0
#define OK 1

typedef struct {
   char  title[50];
   char  author[50];
   char  reviser[100];
   char  description[100];
   int   ID;
   char  result[5];
} nTest;

typedef enum {
    ALL_UNIT_TESTS = 0,
    ALL_INTEGRATION_TESTS = 1,
    ALL_TESTS = 2
} testType;

void test();
void specificTest();
bool callSpecificUnitTest(unsigned short command);
bool callSpecificIntegrationTest(unsigned short command);
bool testSelection(testType type);
void printTestResult(char title[], char  author[], char  reviser[], char description[], int ID, char result[]);
bool askIfTestAgain();

#endif /* INCLUDES_TEST_H_ */
