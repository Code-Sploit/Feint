#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <ctype.h>

typedef struct RESULT_STRUCT {
    char *_M;
    
    int _R:
} TestResult;

typedef struct FS_STRUCT {
    char *_NAME;
} FileName;

int main(int argc, char** argv)
{
    struct dirent *DEP; /* DEP = (Directory Entry Point) */

    TestResult **Results;
    FileName **FileNames;
    DIR *TestDirectory;

    TestDirectory = dopen(argv[1]);

    int _n = 0;

    if (TestDirectory)
    {
        while ((DEP = readdir(TestDirectory)) != NULL)
        {
            FileNames[_n] = calloc(1, sizeof(FileName));

            FileNames[_n]->_NAME = DEP->d_name;
        }
    }

    for (int i = 0; i < _n; i++)
    {
        char *_TCmd  = calloc(1, sizeof(char));
        char *_FName = FileNames[i];

        _TCmd = realloc(_TCmd, (strlen(_TCmd) + strlen("./feint -t ") + strlen(_FName) + strlen("\n") + 1));

        Results[_n] = calloc(1, sizeof(TestResult));

        Results[_n] = system(_TCmd);
    }

    for (int i = 0; i < _n; i++)
    {
        if (Results[_n] == 1) {printf("Error while testing index: '%d' of test array!\n", _n);}
    }

    return 0;
}
