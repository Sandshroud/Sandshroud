/*
 * Thetruecrow
 * Citric
 * CactusEMU
 */

#ifndef MISC_H
#define MISC_H

#include <string>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

bool FileExists( const char* FileName );
void CreateDir( const std::string& Path );
int ReadBuild(int locale);

#endif