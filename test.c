#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void parse_response(const char *data, int* time1, int* time2, int* time3);

int main()
{
    int a = 0;
    int b = 0;
    int c = 0;
    const char *data = "123/456/789";
    parse_response(data, &a, &b, &c);
    printf("%d\t%d\t%d",a,b, c);
}

void parse_response(const char *data, int* time1, int* time2, int* time3){
    char *str2, *saveptr2;
    char *del = "/";
    char *data2 = strdup(data);
    int i = 0;
    for (str2 = data2; ; str2 = NULL) {
        char *subtoken = strtok_r(str2, del, &saveptr2);
        if (subtoken == NULL)
            break;
        int n=atoi(subtoken);

        switch (i) {
            case 1 :
            *time2 = n;
            break;

            case 0 :
            *time1 = n;
            break;

            case 2:
            *time3 = n;
            break;
        }

        i++;
    }
}