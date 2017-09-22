/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
 \*===========================================================================*/

#include "carconfig_file.h"
#include "carconfig_base.h"
#include <sstream>
#include <istream>
#include <stdexcept>
#include <iostream>
#include <string>

CarconfigFile::CarconfigFile()
{
    filePos_ = 0;
    ccFile_ = std::unique_ptr<std::fstream>(new std::fstream);
    fileBuf_ = std::unique_ptr<std::stringstream>(new std::stringstream);
    if (ccFile_ == NULL)
    {
        throw std::runtime_error("Failed to allocate ifstream");
    }
}

CarconfigFile::~CarconfigFile() { close(); }

void CarconfigFile::close()
{
    if (fileupdated_ == true)
    {
        ccFile_->close();
        ccFile_->open(filePath_, std::ios::out | std::ios::trunc);
        ccFile_->write(fileBuf_->str().c_str(), fileBuf_->str().length());
    }
    ccFile_->close();
}

void CarconfigFile::open(const std::string path)
{
    filePath_ = path;
    ccFile_->open(filePath_, std::ios::out | std::ios::in);
    if (!ccFile_->is_open())
    {
        throw std::runtime_error("Could not open carconfig file: " + path);
    }
    *fileBuf_ << ccFile_->rdbuf();
}

bool CarconfigFile::setValue(uint32_t id, ccValue value)
{
    std::string line;
    std::stringstream tmpStrBuf;

    fileupdated_ = true;

    fileBuf_->clear();
    fileBuf_->seekg(0);
    tmpStrBuf << fileBuf_->rdbuf();
    fileBuf_->str("");

    uint32_t i = 0;
    while (std::getline(tmpStrBuf, line))
    {
        if (i == id - 1)
        {
            if (value.status == ccStatus::GOOD)
            {
                *fileBuf_ << std::to_string(value.raw) << "," << std::to_string(value.subs) << ",0" << std::endl;
            }
            else
            {
                *fileBuf_ << std::to_string(value.raw) << "," << std::to_string(value.subs) << ",1" << std::endl;
            }
        }
        else
        {
            *fileBuf_ << line << std::endl;
        }
        i++;
    }
    fileBuf_->clear();
    fileBuf_->seekg(0);
    filePos_ = 0;
    return true;
}

ccValue CarconfigFile::getValue(int id)
{
    std::string line;
    ccValue value;

    if ((id < 1) || (id > cc_no_of_variables))
    {
        throw std::runtime_error("carconfig parameter " + std::to_string(id) + " out of range");
    }
    if (id < filePos_)
    {
        fileBuf_->seekg(0);
        filePos_ = 0;
    }
    if (id > filePos_ + 1)
    {
        while (std::getline(*fileBuf_, line))
        {
            filePos_++;
            if (id == filePos_ + 1)
            {
                break;
            }
        }
    }
    std::string cell;
    std::getline(*fileBuf_, line);

    std::stringstream lineStream(line);
    filePos_++;

    std::getline(lineStream, cell, ',');
    if (!validate(std::stoi(cell), value.raw))
    {
        throw std::runtime_error("Out of range 'Raw'' value carconfig parameter " + std::to_string(filePos_));
    }

    std::getline(lineStream, cell, ',');
    if (!validate(std::stoi(cell), value.subs))
    {
        throw std::runtime_error("Out of range 'Substitute' value carconfig parameter " + std::to_string(filePos_));
    }

    unsigned char status;
    std::getline(lineStream, cell, ',');
    if (!validate(std::stoi(cell), status))
    {
        throw std::runtime_error("Out of range 'Status' value carconfig parameter " + std::to_string(filePos_));
    }
    else
    {
        if (status == 0)
        {
            value.status = ccStatus::GOOD;
        }
        else if (status == 1)
        {
            value.status = ccStatus::INVALID;
        }
        else
        {
            throw std::runtime_error("Undefined 'Status' flag for carconfig parameter " + std::to_string(filePos_));
        }
    }
    return value;
}

bool CarconfigFile::validate(int rawValue, unsigned char &vValue)
{
    if ((rawValue >= 0) && (rawValue <= 255))
    {
        vValue = (char)rawValue;
        return true;
    }
    else
    {
        return false;
    }
}
