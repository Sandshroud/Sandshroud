#ifndef MPQ_H
#define MPQ_H

#include "loadlib/loadlib.h"
#include "libmpq/mpq.h"
#include <string.h>
#include <ctype.h>
#include <vector>
#include <iostream>
#include <deque>

using namespace std;

class MPQArchive
{

public:
    mpq_archive_s *mpq_a;

    MPQArchive(const char* filename);
    void close();

    void GetFileListTo(vector<string>& filelist) {
        uint32_t filenum;
        if(libmpq__file_number(mpq_a, "(listfile)", &filenum)) return;
        libmpq__off_t libSize;
        libmpq__file_unpacked_size(mpq_a, filenum, &libSize);
        // Limit size to positive value and less than signed int max
        if(libSize < 0 || libSize > 0x7FFFFFFF)
            return;

        size_t buffSize = 0x7FFFFFFF&libSize;
        char *buffer = new char[buffSize];

        libmpq__file_read(mpq_a, filenum, (unsigned char*)buffer, buffSize, &libSize);

        char seps[] = "\n";
        char *token = NULL, *nextToken = NULL;

        token = strtok_s(buffer, seps, &nextToken);
        uint32 counter = 0;
        while ((token != NULL) && (counter < buffSize))
        {
            //cout << token << endl;
            token[strlen(token) - 1] = 0;
            string s = token;
            filelist.push_back(s);
            counter += strlen(token) + 2;
            token = strtok_s(NULL, seps, &nextToken);
        }

        delete[] buffer;
    }
};
typedef std::deque<MPQArchive*> ArchiveSet;

class MPQFile
{
    //MPQHANDLE handle;
    bool eof;
    char *buffer;
    size_t pointer, size;

    // disable copying
    MPQFile(const MPQFile& /*f*/) {}
    void operator=(const MPQFile& /*f*/) {}

public:
    const char* getfilename() { return f_name; };
    MPQFile(const char* filename);    // filenames are not case sensitive
    ~MPQFile() { close(); }
    size_t read(void* dest, size_t bytes);
    size_t getSize() { return size; }
    size_t getPos() { return pointer; }
    char* getBuffer() { return buffer; }
    char* getPointer() { return buffer + pointer; }
    bool isEof() { return eof; }
    void seek(int offset);
    void seekRelative(int offset);
    void close();

private:
    const char* f_name;
};

inline void flipcc(char *fcc)
{
    char t;
    t=fcc[0];
    fcc[0]=fcc[3];
    fcc[3]=t;
    t=fcc[1];
    fcc[1]=fcc[2];
    fcc[2]=t;
}

#endif
