#include "main.h"
#include <stdio.h>
void write_expression(double *answer);
char *read_ln(void);

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


#define FLOW_CAPTURE(out_stream, ... ) \
    std::streambuf * old_stream = std::cout.rdbuf();\
    std::ostringstream out_stream;\
    std::cout.rdbuf( out_stream.rdbuf() );\
    __VA_ARGS__\
    std::cout.rdbuf( old_stream );

TEST(test_error_out, random_symbol){
    FLOW_CAPTURE(str_cout,
        std::string str="  s ";  
        calculator(str);
    );
    // std::cout<< str_cout.str()<<"\n";
    ASSERT_TRUE(str_cout.str()==SYNTAX_ERROR) << "not a true!";
}

TEST(test_error_out, test_sqrt_by_minus){
    FLOW_CAPTURE(str_cout,
        std::string str="1+sqrt(-2)";  
        calculator(str);
    );
    ASSERT_TRUE(str_cout.str()==SQRT_MINUS) << "not a true!";
}

TEST(test_error_out, test_unalanced_backets){
    FLOW_CAPTURE(str_cout,
        std::string str="sqrt(25";  
        calculator(str);
    );
    ASSERT_TRUE(str_cout.str()==UNBALANCED_BACKETS) << "not a true!";
}

TEST(test_error_out, test_not_example){
    FLOW_CAPTURE(str_cout,
        std::string str="";  
        calculator(str);
    );
    ASSERT_TRUE(str_cout.str()==NOT_EXPRESSION) << "not a true!";
}

TEST(test_error_out, test_div_by_zero){
    FLOW_CAPTURE(str_cout,
        std::string str="12+2*3*(123/(25-25))";  
        calculator(str);
    );
    ASSERT_TRUE(str_cout.str()==DIV_BY_ZERO) << "not a true!";
}

TEST(correct_input_parsing, valid_expression_1)
{
    std::string testing_expression="1+2*3^2";
    int answer=calculator(testing_expression);
    ASSERT_TRUE(answer==19) << "not a true!";
}
TEST(correct_input_parsing, valid_expression_2)
{
    std::string testing_expression="35+mod(-30+sqrt(625))";
    int answer=calculator(testing_expression);
    ASSERT_TRUE(answer==40) << "not a true!";
}
TEST(correct_input_parsing, valid_expression_3)
{
    std::string testing_expression="1+2+sqrt(2+mod(-79))";
    int answer=calculator(testing_expression);
    ASSERT_TRUE(answer==12) << "not a true!";
}
TEST(correct_input_parsing, valid_expression_4)
{
    std::string testing_expression="2^2^2+3%2";
    int answer=calculator(testing_expression);
    ASSERT_TRUE(answer==17) << "not a true!";
}


/*
int main(void)
{

    double answer=0;
        char command;
    char delimitr;
    for(;;){
        std::cout<<"---------\nw/-write expression\ns/-save answer\nr/-reciple answer\nl/-Saved list\ne/-exit\n";
        command=getchar();
        if(command!='\n')
        {
            delimitr=getchar();
            if(delimitr!='\n')
                for (;getchar()!='\n';);
        }
        if(delimitr=='/')
            switch(command) {
                case 'w':
                    write_expression(&answer);
                    break;
                case 's':
                    memory_write(answer);
                    std::cout<<"Answer saved->"<<answer<<"\n";
                    break;
                case 'l':
                    answers_list();
                    break;
                case 'r':
                    reciple(&answer);
                    break;
                case 'e':
                    return 0;
                }
        }
    return 0;
}
*/

void write_expression(double *answer)
{
    std::string read_expression;
    std::cout<<"write expression: ";
    std::cin>>read_expression;
    getchar();
    *answer=calculator(read_expression);
    std::cout<<"Answer: " << *answer << '\n';
}

char *read_ln(void)
{
    int i=1;
    char *str=NULL;
    str=new char;
    if (str!=NULL);
    while((str[i-1]=getchar())!='\n')
    {
        i++;
        if (str!=NULL);
            str=(char*)realloc(str,i);
    }
    if (str!=NULL);
        str[i-1]='\0';
    return str;
}

