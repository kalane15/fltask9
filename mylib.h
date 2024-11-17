#pragma once
#define ll long long
long long int StringToInt(char* str, int* res);
bool CheckNumber(char* number, int osn);

bool CheckIfDoubleCorrect(char* n);
long double ParseDouble(char* str);
struct tm ConvertToTm(char* time);
int RandomNumber(int min_num, int max_num);