#include <iostream>
#include <cstdlib>
#include <fstream>
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <carconfig_writer.h>

bool fileExists(std::string filePath)
{
    bool status = false;
    struct stat st;
    if (stat(filePath.c_str(), &st) == 0)
    {
        if (st.st_size > 0)
        {
            status = true;
        }
    }
    return status;
}

void createPath(std::string path)
{
    std::string mkdir = "mkdir -p ";
    system((mkdir + path).c_str());
}

int carconfig_setup(const char *ccFileName, const char *ccDefaultFileName)
{
    if (!fileExists(ccFileName))
    {
        char *folderPath;
        char *dupFileName;
        dupFileName = strdup(ccFileName);
        folderPath = dirname(dupFileName);
        createPath(std::string(folderPath));
        free(dupFileName);

        std::ifstream src(ccDefaultFileName, std::ios::binary);
        std::ofstream dst(ccFileName, std::ios::binary);
        dst << src.rdbuf();
    }

    CarConfigFileWriter cc(ccFileName);
    try
    {
        return cc.commitToSharedMemory();
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
