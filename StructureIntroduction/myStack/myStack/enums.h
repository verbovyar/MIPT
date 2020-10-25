#pragma once

#define DEF_CMD(name, num, arg, code)\
CMD_##name = num,

enum comands {
	#include "comands.h"
};

#undef DEF_CMD