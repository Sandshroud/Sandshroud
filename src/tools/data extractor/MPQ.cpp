/*
 * Thetruecrow
 * Citric
 * CactusEMU
 */

#include "headers.h"
#include "MPQ.h"

void LoadLocaleMPQFiles(int const locale)
{
    char filename[512];

    //Locale-xxXX.MPQ
    sprintf_s(filename,"%s/Data/%s/locale-%s.MPQ", input_path, langs[locale], langs[locale]);
    printf("Loading %s\n", filename);
    if(!SFileOpenArchive(filename, 0, MPQ_OPEN_READ_ONLY, &localeMPQ[0]))
    {
        printf("%i\n", GetLastError());
        assert(false && "\nLoad of MPQ failed");
    }

    for(int i = 0; i < PATCH_REV_COUNT; ++i)
    {
        char ext[7] = "";
        sprintf_s(ext, "-%i", patchRev[i]);

        sprintf_s(filename,"%s/Data/wow-update%s.MPQ", input_path, ext);
        if(!SFileOpenPatchArchive(localeMPQ[0], filename, langs[locale], MPQ_OPEN_READ_ONLY))
        {
            printf("%i\n", GetLastError());
            assert(false && "Load of MPQ patch failed");
        }
    }
    if(!SFileIsPatchedArchive(localeMPQ[0]))
        assert(false && "An error occured");

    //Others
    for(int i = 0; i < PATCH_REV_COUNT; ++i)
    {
        char ext[7] = "";
        sprintf_s(ext, "-%i", patchRev[i]);

        sprintf_s(filename,"%s/Data/wow-update%s.MPQ", input_path, ext);
        printf("Loading %s\n", filename);
        if(!SFileOpenArchive(filename, 0, MPQ_OPEN_READ_ONLY, &localeMPQ[i+1]))
        {
            printf("%i\n", GetLastError());
            assert(false && "Load of MPQ failed");
        }

        for(int j = i; j < PATCH_REV_COUNT; ++j)
        {
            char ext[7] = "";
            sprintf_s(ext, "-%i", patchRev[j]);

            sprintf_s(filename,"%s/Data/wow-update%s.MPQ", input_path, ext);
            if(!SFileOpenPatchArchive(localeMPQ[i+1], filename, langs[locale], MPQ_OPEN_READ_ONLY))
            {
                printf("%i\n", GetLastError());
                assert(false && "Load of MPQ patch failed");
            }
        }
        if(!SFileIsPatchedArchive(localeMPQ[i+1]))
            assert(false && "An error occured");
    }
}

void LoadMapMPQFiles()
{
    char filename[512];

    //Locale-xxXX.MPQ
    sprintf_s(filename,"%s/Data/world.MPQ", input_path);
    printf("Loading %s\n", filename);
    if(!SFileOpenArchive(filename, 0, MPQ_OPEN_READ_ONLY, &DataMPQ))
    {
        printf("%i\n", GetLastError());
        assert(false && "\nLoad of MPQ failed");
    }

    for(int j = 0; j < 3; j++)
    {
        sprintf_s(filename, "%s/Data/expansion%u.MPQ", input_path, j+1);
        printf("Loading %s\n", filename);
        if(!SFileOpenPatchArchive(DataMPQ, filename, "", MPQ_OPEN_READ_ONLY))
        {
            printf("%i\n", GetLastError());
            assert(false && "\nLoad of MPQ failed");
        }
    }

    for(int i = 0; i < PATCH_REV_COUNT; ++i)
    {
        char ext[7] = "";
        sprintf_s(ext, "-%i", patchRev[i]);

        sprintf_s(filename,"%s/Data/wow-update%s.MPQ", input_path, ext);
        printf("    -%i\n", patchRev[i]);
        if(!SFileOpenPatchArchive(DataMPQ, filename, "base", MPQ_OPEN_READ_ONLY))
        {
            printf("%i\n", GetLastError());
            assert(false && "Load of MPQ patch failed");
        }
    }
}

int ExtractFileToHardDrive(HANDLE &MPQ_handle, const char * szArchivedFile, const char * szFileName)
{
    HANDLE hFile  = NULL;           // Archived file handle
    TFileStream* handle = NULL;     // Disk file handle
    int nError = ERROR_SUCCESS;     // Result value
    DWORD dwBytesRead;              // Our returned read bytes

    if(nError == ERROR_SUCCESS)
    {
        if(!SFileOpenFileEx(MPQ_handle, szArchivedFile, SFILE_OPEN_FROM_MPQ, &hFile))
            nError = GetLastError();
    }

    // Create the target file
    if(nError == ERROR_SUCCESS)
    {
        handle = FileStream_CreateFile(szFileName, STREAM_FLAG_WRITE_SHARE);
        if(handle == NULL)
            nError = GetLastError();
    }

    // Read the file from the archive
    if(nError == ERROR_SUCCESS)
    {
        // Get the size of the full patched file
        DWORD dwFileSize = SFileGetFileSize(hFile, NULL);
        if(dwFileSize != 0)
        {
            // Allocate space for the full file
            BYTE * pbFullFile = new BYTE[dwFileSize];
            if(!SFileReadFile(hFile, pbFullFile, dwFileSize, &dwBytesRead, NULL))
            {
                nError = GetLastError();
                printf("Failed to read full patched file data \"%s\"\n", szFileName);
                assert(false);
            }
            FileStream_Write(handle, NULL, pbFullFile, dwFileSize);
            delete [] pbFullFile;
        }
    }

    // Cleanup and exit
    if(handle != NULL)
        FileStream_Close(handle);
    if(hFile != NULL)
        SFileCloseFile(hFile);

    return nError;
}

char* ExtractFileToMemory(HANDLE &MPQ_handle, const char * szArchivedFile, int & size )
{
    HANDLE hFile  = NULL;           // Archived file handle
    int nError = ERROR_SUCCESS;     // Result value
    DWORD dwBytesRead;              // Our returned read bytes
    char * pbFullFile = NULL;

    if(nError == ERROR_SUCCESS)
    {
        if(!SFileOpenFileEx(MPQ_handle, szArchivedFile, SFILE_OPEN_FROM_MPQ, &hFile))
            nError = GetLastError();
    }

    // Read the file from the archive
    if(nError == ERROR_SUCCESS)
    {
        // Get the size of the full patched file
        size = SFileGetFileSize(hFile, NULL);
        if(size != 0)
        {
            // Allocate space for the full file
            pbFullFile = new char[size];
            if(!SFileReadFile(hFile, pbFullFile, size, &dwBytesRead, NULL))
            {
                nError = GetLastError();
                printf("Failed to read full patched file data \"%s\"\n", szArchivedFile);
                assert(false);
            }
        }
    }

    // Cleanup and exit
    if(nError == ERROR_SUCCESS && hFile != NULL)
        SFileCloseFile(hFile);

    return pbFullFile;
}