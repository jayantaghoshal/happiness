#ifndef CARCONFIG_FILE_H
#define CARCONFIG_FILE_H

#include <string>
#include <fstream>
#include <sstream>
#include <memory>

/*
 * The carconfig backend file is based on the CSV "standard"
 * The file should have 1778 parameters according to the
 * following format:
 *  Raw Value,Substitute Value,Status flag
 * Example:
 *  255,255,0
 *
 * Each parameter can have a value of 0 to 255.
 * */

enum class ccStatus
{
    GOOD = 0,
    INVALID = 1
};

typedef struct
{
    unsigned char raw;
    unsigned char subs;
    ccStatus status;
} ccValue;

class CarconfigFile
{
   public:
    CarconfigFile();
    ~CarconfigFile();
    void open(const std::string path);
    void close();
    ccValue getValue(int id);
    bool setValue(uint32_t id, ccValue value);

   private:
    std::unique_ptr<std::fstream> ccFile_;
    std::unique_ptr<std::stringstream> fileBuf_;
    std::string filePath_;
    int filePos_;
    bool fileupdated_ = false;
    bool validate(int rawValue, unsigned char &vValue);
};

#endif  // CARCONFIG_FILE_H
