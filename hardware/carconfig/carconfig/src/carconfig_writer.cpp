#include <string.h>
#include <fstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "carconfig_writer.h"
#include "carconfig_file.h"

CarConfigFileWriter::CarConfigFileWriter(std::string fileposition) { fPos = fileposition; }

int CarConfigFileWriter::commitToSharedMemory()
{
    int fd, i, x = 0;
    char *addr;

    fd = shm_open(shm_file_name.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd == -1)
    {
        (void)fprintf(stderr, "shm_open: %s\n", strerror(errno));
        return (1);
    }

    if (ftruncate(fd, bufferSize()) == -1)
    {
        (void)fprintf(stderr, "ftruncate : %s\n", strerror(errno));
        return (1);
    }

    addr = (char *)mmap(0, bufferSize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (addr == MAP_FAILED)
    {
        (void)fprintf(stderr, "mmap failed:%s\n", strerror(errno));
        return (1);
    }

    CarconfigFile ccFile;
    ccFile.open(fPos);
    ccValue ccRes;

    for (i = 1; i <= cc_no_of_variables; i++)
    {
        ccRes = ccFile.getValue(i);
        if (ccRes.status == ccStatus::GOOD)
        {
            addr[x] = ccRes.raw;
        }
        else if (ccRes.status == ccStatus::INVALID)
        {
            addr[x] = ccRes.subs;
        }
        else
        {
            addr[x] = 0x00;
        }
        addr[x + 1] = ccRes.raw;
        x += sizeof(carConfigObject);
    }
    return 0;
}
