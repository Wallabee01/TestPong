#include "stdafx.h"
#include "sError.h"


void sError(const std::string& errorStr)
{
	printf("%s\n", errorStr.c_str());
	system("pause");
	exit(1);
}
