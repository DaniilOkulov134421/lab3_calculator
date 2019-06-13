#include "calculator.h"

static const int
    DELIMITER = 1,
    FUNCTION = 2,
    NUMBER = 3,
    SYNTAX_ERROR = 0,
    UNBALANCED_BACKETS = 1,
    NOT_EXPRESSION = 2,
    DIV_BY_ZERO = 3,
    MEMORY_ALLOC_ERROR = 4,
    SQRT_MINUS = 5;

std::string::iterator expression;
char tok_type;

std::vector <double> memory;
std::vector <char> token={0};

double calculator(char *read);

void addition_subtraction (double *answer);
void mult_div_remainder (double *answer);
void pow (double *answer);
void unary (double *answer);
void bracket_and_num (double *answer);
void functions (double *answer);

void get_token(void);
void clear_token(void);

void memory_write (double answer);
double memory_read (int number);
void answers_list (void);
void reciple (double *answer);

int get_number ();
void serror (int error);
int is_delim (char c);
void read_num (double *answer);
void read_num_of_expression (void);
void copy_token_to_str (char* str);
char token_function (std::string str2);
void read_function (void);

#define EXPRESSION_UNDEFINED (read.empty())
#define TOKEN_EMPTY (token[0]=='\0')
#define SECOND_MEMBER_UNDEFINED (second_member!=second_member)
double calculator(std::string read)
{
    double answer=0;
    expression=read.begin();
    if(EXPRESSION_UNDEFINED)
    {
        serror(NOT_EXPRESSION);
        return 0;
    }
    get_token();
    if(TOKEN_EMPTY)
    {
        serror(NOT_EXPRESSION);
        return 0;
    }
    addition_subtraction(&answer);
    if(!TOKEN_EMPTY)
        serror(SYNTAX_ERROR);
    return answer;
}

#define OP_TOKEN_IS(first,second) ((op_token = token[0]) == first || op_token == second)
void addition_subtraction(double *answer)
{
    try
    {
        register char op_token;
        double second_member;
        mult_div_remainder(answer);
        while(OP_TOKEN_IS('+','-'))
        {
            second_member=NAN;
            get_token();
            mult_div_remainder(&second_member);
            if(SECOND_MEMBER_UNDEFINED)
                throw SYNTAX_ERROR;
            switch(op_token) {
                  case '-':
                    *answer = *answer - second_member;
                    break;
                  case '+':
                    *answer = *answer + second_member;
                    break;
            }
        }
    }
    catch(int error)
    {
        serror(error);
        clear_token();
        return;
    }
}

#undef OP_TOKEN_IS
#define OP_TOKEN_IS(first,second,third) ((op_token = token[0]) == first || op_token == second || op_token == third)
void mult_div_remainder(double *answer)
{
    try
    {
        register char op_token;
        double second_member;
        pow(answer);
        while(OP_TOKEN_IS('*','/','%'))
        {
            second_member=NAN;
            get_token();
            pow(&second_member);
            if(SECOND_MEMBER_UNDEFINED)
            {
                throw SYNTAX_ERROR;
                return;
            }
            switch(op_token) {
              case '*':
                *answer = *answer * second_member;
                break;
              case '/':
                if(second_member == 0.0)
                    throw DIV_BY_ZERO;
                else
                    *answer = *answer / second_member;
                break;
              case '%':
                *answer = (int) *answer % (int) second_member;
                break;
                }
        }
    }
    catch(int error)
    {
        serror(error);
        *answer = 0.0;
    }
}

void pow(double *answer)
{
    try
    {
        double second_member=NAN;
        register int t;
        unary(answer);
        if(token[0] == '^')
        {
            get_token();
            pow(&second_member);
            if(SECOND_MEMBER_UNDEFINED)
                throw SYNTAX_ERROR;
            *answer=pow(*answer,second_member);
        }
    }
    catch(int error)
    {
        serror(SYNTAX_ERROR);
        clear_token();
        return;
    }
}

#define TOKEN(first,second) (token[0]==first || token[0] == '-')
void unary(double *answer)
{
    register char  op_token;
    op_token = 0;
    if(TOKEN('+','-'))
    {
        op_token = token[0];
        get_token();
    }
    bracket_and_num(answer);
    if(op_token == '-') *answer = -(*answer);
}

#undef TOKEN
#define TOKEN(checking) (token[0] == checking)
void bracket_and_num(double *answer)
{
    functions(answer);
    if(TOKEN('('))
    {
        get_token();
        addition_subtraction(answer);
        if(!TOKEN(')'))
        {
            serror(UNBALANCED_BACKETS);
            return;
        }
        get_token();
    }
    else if(tok_type==NUMBER)
        read_num(answer);
}

void functions(double *answer)
{
    try
    {
        double second_member=NAN;
        if(tok_type==FUNCTION)
        {
            if(token_function("sqrt"))
            {
                get_token();

                if(token[0]=='(')
                {
                    bracket_and_num(&second_member);
                    if(SECOND_MEMBER_UNDEFINED)
                        throw SYNTAX_ERROR;
                    else if(second_member<0)
                        throw SQRT_MINUS;
                    *answer=sqrt(second_member);
                }
                else
                    throw SYNTAX_ERROR;
            }
            else if(token_function("mod"))
            {
                get_token();
                if(token[0]=='(')
                 {
                    bracket_and_num(&second_member);
                    if(SECOND_MEMBER_UNDEFINED)
                        throw SYNTAX_ERROR;
                    *answer=second_member;
                    if(*answer<0)
                        *answer=-(*answer);
                }
                else
                    throw SYNTAX_ERROR;
            }
            else if(token_function("MR"))
            {
                get_token();
                if(token[0]=='[')
                {
                    get_token();
                    if(tok_type==NUMBER)
                    {
                        int number=get_number();
                        *answer=memory_read(number);
                    }
                    else
                        throw SYNTAX_ERROR;
                    if(token[0]!=']')
                        serror(SYNTAX_ERROR);
                    get_token();
                }
                else
                    *answer=memory_read(memory.size()-1);
            }
        }
    }
    catch(int error)
    {
        serror(error);
        clear_token();
        *answer=0;
        return;
    }
}

void read_num(double *answer)
{
    try
    {
        if(tok_type == NUMBER)
        {
            char* str= new char[token.size()];
            if(str==NULL)
                throw MEMORY_ALLOC_ERROR;
            copy_token_to_str(str);
            *answer = atof(str);
            delete str;
            get_token();
            if(*answer==INFINITY)
                throw SYNTAX_ERROR;
            return;
        }
        throw SYNTAX_ERROR;
    }
    catch (int error)
    {
        serror(error);
        clear_token();
        *answer=0;
        return;
    }
}

void serror(int error)
{
    static std::string error_list[]= {
         "SYNTAX_ERROR" ,
         "UNBALANCED_BACKETS",
         "NOT_EXPRESSION",
         "DIV_BY_ZERO",
         "MEMORY_ALLOC_ERROR", 
         "SQRT_MINUS"
    };
    std::cout<<error_list[error]<<"\n";
}

void get_token(void)
{
    clear_token();
    if(!*expression)
        return;
    while(isspace(*expression))
        ++expression;
    if(strchr("+-*/%^=()[]", *expression))
    {
        tok_type = DELIMITER;
        token.push_back(*expression++);
    }
    else if(isalpha(*expression))
    {
        read_function();
    }
    else if(isdigit(*expression))
    {
        read_num_of_expression();
    }
    token.push_back('\0');
}
void clear_token(void)
{
    token[0]='\0';
    token.resize(0);
    tok_type = 0;
}



#define MAX_SIZE_LINE_DOUBLE 320

void read_num_of_expression(void)
{
    int counter=0;
    while(!is_delim(*expression))
        {
            if(!isdigit(*expression))
            {
                serror(SYNTAX_ERROR);
                break;
            }
            token.push_back(*expression++);
            counter++;
            if(counter==MAX_SIZE_LINE_DOUBLE)
                break;
        }
        tok_type = NUMBER;
}

#define MAX_SIZE_FUNCTION 4
void read_function(void)
{
    int counter=0;
    while(!is_delim(*expression))
    {
        token.push_back(*expression++);
        counter++;
        if(counter==MAX_SIZE_FUNCTION)
            break;
    }
    tok_type = FUNCTION;
}

int is_delim(char c)
{
    if(strchr(" +-/*%^=()[]", c) || c==9 || c=='\r' || c==0)
        return 1;
    return 0;
}

void answers_list(void)
{
    std::cout<<"Answers list\n";
    for (double n : memory)
        std::cout<<n<<"\n";
}

void memory_write(double answer)
{
    memory.push_back(answer);
}
double memory_read(int number)
{
    int size=memory.size();
    if((number>=0)&&(number<size))
    {
        return memory[number];
    }
    return 0;
}
int get_number(void)
{
    char* second_member=new char [token.size()];
    if (second_member==NULL) return 0;
    
    copy_token_to_str(second_member);
    int number=atoi(second_member);
    get_token();
    delete second_member;
    return number;
}
void reciple(double *answer)
{
    if(*answer!=0)
    {
        *answer=1/(*answer);
    }
    std::cout<<*answer<<"\n";
}
char token_function(std::string str)
{
    int size;
    if(size=token.size()==(str.size()+1))
    {
        for(int num=0;num<size;num++)
        {
            if(token[num]!=str[num])
            {
                return 0;
            }
        }
        return 1;
    }
    else
        return 0;
}
void copy_token_to_str(char* str)
{
    int size=token.size();
    for(int k=0;k<size;k++)
    {
        str[k]=token[k];
    }
}
