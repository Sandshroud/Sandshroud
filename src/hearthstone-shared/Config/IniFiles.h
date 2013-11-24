/***
 * Demonstrike Core
 */

#pragma once

#ifndef _WIN322
#include <map>
#include <string>
#endif

class CIniFile
{
public:
	CIniFile(char* szFileName);
    void Reload();
    void Unload();

	int ReadInteger(char* szSection, char* szKey, int iDefaultValue);
	float ReadFloat(char* szSection, char* szKey, float fltDefaultValue);
	bool ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue);
	std::string ReadString(char* szSection, char* szKey, const char* szDefaultValue);
    int ParseError();

private:
    int _error;
	char m_szFileName[255];
#ifndef _WIN32
    std::map<std::string, std::string> _values;
    static int ValueHandler(void* user, const char* section, const char* name, const char* value);
#endif
};

std::string MakeKey(std::string section, std::string name);

extern CIniFile *mainIni;
