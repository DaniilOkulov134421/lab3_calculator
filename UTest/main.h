#include <locale.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <string>

#include "../googletest/googletest/include/gtest/gtest.h"
#include "../lib/calculator.h"

using String = std::string;

static const String
	SYNTAX_ERROR = "SYNTAX_ERROR\n",
	UNBALANCED_BACKETS = "UNBALANCED_BACKETS\n",
	NOT_EXPRESSION = "NOT_EXPRESSION\n",
	DIV_BY_ZERO = "DIV_BY_ZERO\n",
	MEMORY_ALLOC_ERROR = "MEMORY_ALLOC_ERROR\n",
	SQRT_MINUS = "SQRT_MINUS\n";