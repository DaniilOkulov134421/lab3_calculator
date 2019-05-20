#include <locale.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <string.h>

#include "../googletest/googletest/include/gtest/gtest.h"
#include "../lib/calculator.h"

#define SYNTAX_ERROR "SYNTAX_ERROR\n"
#define UNBALANCED_BACKETS "UNBALANCED_BACKETS\n"
#define NOT_EXPRESSION "NOT_EXPRESSION\n"
#define DIV_BY_ZERO "DIV_BY_ZERO\n"
#define MEMORY_ALLOC_ERROR "MEMORY_ALLOC_ERROR\n"
#define SQRT_MINUS "SQRT_MINUS\n"