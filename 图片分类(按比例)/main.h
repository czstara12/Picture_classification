#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>
#include <io.h>
#include "CImage.h"
#include <map>
#include <algorithm>

int fenlei(char* str);
int tongji(char* str);
class yuansu
{
public:
	double bili_d;
	std::string bili_s;
	int count;
	bool operator>(yuansu yuansu2)
	{
		if (bili_d > yuansu2.bili_d)
			return true;
		else
			return false;
	}
	yuansu(double cbili_d, std::string cbili_s) :bili_d(cbili_d), bili_s(cbili_s), count(1)
	{

	}
};