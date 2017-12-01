#pragma once
#include <map>
#include <vector>
#include "stdafx.h"

#include<strsafe.h>
typedef const char * TKEY;
/*
 * Singletone container for string resources for run-time use
 */
class strings
{
private:
	const int BUFSZ = 1023;
	int _nextid = 432510; //random value for initializer
	static strings *_instance;
	strings();
	strings(const strings&);
	strings& operator=(const strings&);


	std::map<TKEY, LPWSTR> m;
	std::vector<LPWSTR> _strings;
	HINSTANCE hInstance;

	void insertIntoMap(TKEY key, LPWSTR val);
public:
	static strings* instance()
	{
		if (!_instance) _instance = new strings();
		return _instance;
	}

	void init(HINSTANCE hInstance);

	LPCWSTR get(TKEY key);
	

	void loadString(UINT uID, TKEY key);
	void loadStrings(const std::vector<UINT>& stringIDs, const std::vector<TKEY>& keys);
	void loadStrings(const std::vector<std::pair<TKEY, UINT>> keyValuePairs);

	template<typename ...Args>
	void formatAndSave(const TKEY fmtStringKey, const TKEY resKey, Args&& ...);

	template<typename ...Args>
	LPWSTR format(const TKEY fmtStringKey, Args&& ...);

	~strings();
};

template <typename ... Args>
void strings::formatAndSave(const TKEY fmtStringKey, const TKEY resKey, Args&&... args)
{
	_strings.push_back(new WCHAR[BUFSZ]); //Allocate memory for new string

	::StringCbPrintf(_strings.back(), BUFSZ, this->get(fmtStringKey), args...);
	this->insertIntoMap(resKey, _strings.back());
}

template <typename ... Args>
LPWSTR strings::format(const TKEY fmtStringKey, Args&&... args)
{
	_strings.push_back(new WCHAR[BUFSZ]); //Allocate memory for new string
	::StringCbPrintf(_strings.back(), BUFSZ, this->get(fmtStringKey), args...);
	return _strings.back();
}

