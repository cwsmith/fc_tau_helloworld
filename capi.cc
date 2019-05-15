#include "capi.h"
#include <iostream>

int pass_by_val(int num)
{
	std::cout << num << std::endl;
	return 0;
}


