/***
 * Demonstrike Core
 */

#include "../Common.h"
#ifndef _WIN32
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "unix/ini.h"
#endif
#include "IniFiles.h"

CIniFile *mainIni;

CIniFile::CIniFile(char* szFileName) : _error(0)
{
	memset(m_szFileName, 0x00, 255);
	memcpy(m_szFileName, szFileName, strlen(szFileName));
#ifndef _WIN32
    _error = ini_parse(m_szFileName, ValueHandler, this);
#endif
}

void CIniFile::Reload()
{
#ifndef _WIN32
    std::map<std::string, std::string> _oldValues;
    for(std::map<std::string, std::string>::iterator itr = _values.begin(); itr != _values.end(); itr++)
        _oldValues[itr->first] = itr->second;

    _error = ini_parse(m_szFileName, ValueHandler, this);
    if(_error)
    {
        _values.clear();
        for(std::map<std::string, std::string>::iterator itr = _oldValues.begin(); itr != _oldValues.end(); itr++)
            _values[itr->first] = itr->second;
    }
#endif
}

void CIniFile::Unload()
{
    delete [] (char*)m_szFileName;
#ifndef _WIN32
    _values.clear();
#endif
}

int CIniFile::ParseError()
{
    return _error;
}

#ifndef _WIN32
int CIniFile::ValueHandler(void* user, const char* section, const char* name,
                            const char* value)
{
    CIniFile* reader = (CIniFile*)user;
    std::string key = MakeKey(section, name);
    if (reader->_values[key].size() > 0)
        reader->_values[key] += "\n";
    reader->_values[key] += value;
    return 1;
}
#endif

/**************************
 * Read
 */
int CIniFile::ReadInteger(char* szSection, char* szKey, int iDefaultValue)
{
    int iResult = iDefaultValue;
#ifdef _WIN32
	iResult = GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_szFileName);
#else
    const char *valstr = ReadString(szSection, szKey, "").c_str();
    char *end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(valstr, &end, 0);
    if(end > valstr)
        iResult = n;
#endif
	return iResult;
}

float CIniFile::ReadFloat(char* szSection, char* szKey, float fltDefaultValue)
{
	float fltResult = fltDefaultValue;
#ifdef _WIN32
	char szResult[255];
	char szDefault[255];
	sprintf(szDefault, "%f", fltDefaultValue);
	GetPrivateProfileString(szSection,  szKey, szDefault, szResult, 255, m_szFileName);
	fltResult = (float)atof(szResult);
#else
    const char *valstr = ReadString(szSection, szKey, "").c_str();
    char *end;
    double n = std::strtod(valstr, &end);
    if(end > valstr)
        fltResult = (float)n;
#endif
	return fltResult;
}

bool CIniFile::ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue)
{
	bool bolResult = bolDefaultValue;
#ifdef _WIN32
	char szResult[255];
	char szDefault[255];
	sprintf(szDefault, "%s", bolDefaultValue? "True" : "False");
	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
	bolResult = (strcmp(szResult, "True") == 0 || strcmp(szResult, "true") == 0) ? true : false;
#else
    std::string valstr = ReadString(szSection, szKey, "");
    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        bolResult = true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        bolResult = false;
#endif
	return bolResult;
}

#ifndef _WIN32
std::string MakeKey(std::string section, std::string name)
{
    std::string key = section + "." + name;
    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}
#endif

std::string CIniFile::ReadString(char* szSection, char* szKey, const char* szDefaultValue)
{
#ifdef _WIN32
	char* szResult = new char[255];
	memset(szResult, 0x00, 255);
	GetPrivateProfileString(szSection,  szKey, szDefaultValue, szResult, 255, m_szFileName);
	return szResult;
#else
    std::string key = MakeKey(szSection, szKey);
    return (_values.count(key) ? _values[key] : szDefaultValue);
#endif
}
