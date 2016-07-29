#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef enum { false, true } bool;
typedef unsigned int ypinum_t ;

#define STACK_SIZE 30000

ypinum_t stack[STACK_SIZE];
ypinum_t p;
ypinum_t tmp;
bool notmp;

//#define tell() //printf("Cell number: %u\nCell value: %u\npc: %li\nop: %c\n\n", p, stack[p], ftell(file), c)
#define VERSION "0.0.1"

void showversion() {
    fprintf(stderr,
    "ypifajkb %s \nCompiled on %s %s\n", VERSION, __DATE__, __TIME__);
}

void showhelp(char* progname) {
    fprintf(stderr,
"Usage: %s [OPTIONS] [FILE]\n\
    -h   Show this help message and exit\n\
    -v   Show version information and exit\n", progname);
}

void ypierror(char* err) {
    fprintf(stderr, "[Error: %s]\n", err);
    exit(1);
}

void push(ypinum_t x) {
    if (p >= STACK_SIZE)
        ypierror("Stack full");

    stack[++p] = x;
}

bool willoverflow = false;

ypinum_t pop() {
    if (willoverflow)
        ypierror("Stack empty");

    if (p == 0)
        willoverflow = true;

    ypinum_t x = stack[p];
    stack[p--] = 0;
    return x;
}

ypinum_t rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

void execute(FILE* file) {
    char c;

    #define nextc() c = getc(file)//; tell()
    #define prevc() {fseek(file, -2, SEEK_CUR);} c = getc(file)//; tell()

    bool stringmode = false;

    while (!feof(file)) {
        nextc();

        if (stringmode && c != '"') {
            push(c);
            continue;
        }

        else if (isdigit(c)) {
            push(c - '0');
            continue;
        }

        switch (c) {
            case '"':
                //Toggle string mode
                stringmode = !stringmode;
                break;

            /*Math operations*/

            case '+':
                push(pop() + pop());
                break;

            case '-': {
                //Substract
                ypinum_t a,b;
                a = pop();
                b = pop();
                push(b - a);
            } break;

            case '*':
                //Multiply
                push(pop() * pop());
                break;

            case '/': {
                //Divide
                ypinum_t a,b;
                a = pop();
                b = pop();
                push(b / a);
            } break;

            case '%': {
                //Modulo division
                ypinum_t a,b;
                a = pop();
                b = pop();
                push(b % a);
            } break;            

            case '~': {
                //Swap
                ypinum_t a,b;
                a = pop();
                b = pop();
                push(a);
                push(b);
            } break;

            case ':': {
                //Duplicate
                ypinum_t a = pop();
                push(a);
                push(a);
                push(a);
            }

            case ',':
                //Pop and print as character
                putchar(pop());
                break;

            case '.': {
                //Pop and print as number
                printf("%u",pop());
            } break;

            case ';':
                //Push one character from input
                push(getchar());
                break;

            case '=':
                //Pop a and b, push 1 if a == b or 0 otherwise
                push(pop() == pop());
                break;

            case '!':
                //Pop a, push the logical not of a
                push(!pop());
                break;

            case '$':
                //Remove last value
                pop();
                break;

            case '?': {
                //Push random value between 0 and 9
                push(rand_interval(0,9));
            } break;

            case '^':
                //Pop and put value in tmp storage
                tmp = pop();
                notmp = false;

            case 'v':
                //Push value from tmp storage
                if (notmp)
                    ypierror("There's no value stored in tmp");

                push(tmp);
                notmp = true;

            case '[': {
                //Start loop
                int bal = 1;
                if (stack[p] == 0)
                    do {
                        nextc();
                        if (c == '[')
                            bal++;
                        else if (c == ']')
                            bal--;
                        else if (feof(file))
                            ypierror("Unbalanced brackets");
                    } while ( bal != 0 );
            } break;

            case ']': {
                //Close loop
                int bal = 0;
                do {
                    if (c == '[')
                        bal++;
                    else if (c == ']')
                        bal--;
                    else if (ftell(file) == 1)
                        ypierror("Unbalanced brackets");

                    prevc();
                    } while ( bal != 0 );
            } break;
        }
    }
    if (stringmode)
        ypierror("Unbalanced string");
}

int main(int argc, char** argv) {
    FILE* file = NULL;

    if (argc > 1) {
        for (int i=1; i < argc; i++) {
            char* arg = argv[i];

            if (arg[0] == '-' ) {
                switch (arg[1]) {
                    case 'h':
                        showhelp(argv[0]);
                        return 0;

                    case 'v':
                        showversion();
                        return 0;
                        
                    case '\0':
                        file = stdin;
                        continue;

                    default:
                        printf("Invalid option: '%c'\n", arg[1]);
                        showhelp(argv[0]);
                        return 1;
                }
            }
            else
                file = fopen(arg, "r");
        }
    }
    else {
        showhelp(argv[0]);
        return 1;
    }

    if (file == NULL)
        ypierror(strerror(errno));

    srand(time(NULL));
    tmp = 0;
    notpm = true;
    execute(file);
}
