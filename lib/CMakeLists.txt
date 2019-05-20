set(source_libs calculator.cpp)
set(source_headers calculator.h)

add_library(calc STATIC ${source_libs} ${source_headers})
# Создает библиотеку с именем calc
