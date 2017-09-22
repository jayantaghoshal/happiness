#ifndef CARCONFIG_WRITER_H
#define CARCONFIG_WRITER_H

class CarConfigFileWriter
{
public:
    CarConfigFileWriter(std::string f) {}
    int commitToSharedMemory() { return 1; }
};

#endif
