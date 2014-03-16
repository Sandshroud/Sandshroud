/*
 * Thetruecrow
 * Citric
 * CactusEMU
 */

#ifndef MPQ_H
#define MPQ_H

#include "headers.h"

void LoadLocaleMPQFiles(int const locale);
void LoadMapMPQFiles();
bool FileExists( const char* FileName );

int ExtractFileToHardDrive(HANDLE &MPQ_handle, const char * szArchivedFile, const char * szFileName);
char* ExtractFileToMemory(HANDLE &MPQ_handle, const char * szArchivedFile, int & size );

#endif