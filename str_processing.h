#ifndef _STRING_PROCESSING_H_
#define _STRING_PROCESSING_H_
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
int Is_Email_Valid(char *email);
int Is_Password_Valid(char *pass);
void Generate_Random_String(char *str, int length);
#endif