#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <ctype.h>

int SFilter(const struct dirent *_N)
{
    return 1;
}

int FeintCompiled()
{
    FILE *fp = fopen("feintc", "rb");

    if (!fp) {return 0;}

    fclose(fp);

    return 1;
}

int main(int argc, char** argv)
{
    if (FeintCompiled() == 0) {printf("Error: Feint has to be compiled before using this tool!\n"); return 1;}

    if (argc < 2) {printf("Error: No directory with test files specified!\n"); return 1;}

    struct dirent **FList;

    int _n = 1;
    int _r = 0;

    int _t = 0;
    int _p = 0;
    int _f = 0;

    _r = scandir(argv[1], &FList, SFilter, alphasort);

    if (_r == -1) {printf("Error while scanning directory: [%s]!", argv[1]); return 1;}

    while (_r--)
    {
        char *_TCmd  = calloc(1, sizeof(char));
        char *_FName = FList[_n]->d_name;

        if (FList[_n] == NULL) {break;}

        _TCmd = realloc(_TCmd, (strlen(_TCmd) + strlen("./feintc ") + strlen(argv[1]) + strlen("/") + strlen(_FName) + strlen(" 0 > /dev/null") + 1));

        strcat(_TCmd, "./feintc ");
        strcat(_TCmd, argv[1]);
        strcat(_TCmd, "/");
        strcat(_TCmd, _FName);
        strcat(_TCmd, " 0 > /dev/null");

        if (strcmp(_FName, ".") != 0 && strcmp(_FName, "..") != 0)
        {
            int _R = system(_TCmd);

            if (_R == 0)
            {
                printf("[%d]: Test \033[0;32m passed \033[0m   \033[0;36m'%s'\033[0m!\n", _t, _FName);

                _p++;
            }
            else
            {
                printf("[%d]: Test \033[0;31m failed \033[0m   \033[0;36m'%s'\033[0m!\n", _t, _FName);

                _f++;
            }
            
            _t++;
        }

        free(FList[_n]);

        _n++;
    }

    printf("\nTest results: [%d]\033[0;36m Total\033[0m| [%d]\033[0;32m Passed\033[0m | [%d]\033[0;31m Failed\033[0m!\n", _t, _p, _f);

    free(FList);

    return 0;
}
