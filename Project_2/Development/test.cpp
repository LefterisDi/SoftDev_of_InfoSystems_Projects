#include <iostream>
#include <string.h>

typedef struct test{
    int i;
} Test;

Test& func(void)
{
    Test* t = new Test;
    t->i = 2;

    std::cout << t << '\n';
    return *t;
}

int main(int argc, char const *argv[]) {
    char str[] = "Hello there; How are you? / I'm good - End";
    char *copy = strdup(str);
    char *delim = ";-/";
    char *res = strtok( str, delim );
    while (res) {
        printf("%c\n", copy[res-str+strlen(res)]);
        printf("%p\n", res);
        printf("%p\n", str);
        std::cout << res << '\n';
        std::cout << str << '\n';
        res = strtok( NULL, delim );
    }
    free(copy);

    return 0;
}
