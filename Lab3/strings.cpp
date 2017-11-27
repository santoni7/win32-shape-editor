#include "stdafx.h"
//#include "strings.h"
//
//strings* strings::_instance = 0;
//strings::strings()
//{
//
//}
//
//void strings::init(HINSTANCE hInstance)
//{
//	this->hInstance = hInstance;
//}
//
//strings::~strings()
//{
//	
//}
//
//
//void strings::insertIntoMap(const TKEY key, const LPWSTR val)
//{
//	this->m.insert_or_assign(key, val);
//}
//
//LPCWSTR strings::get(const TKEY key)
//{
//	return this->m.at(key);
//}
//
//void strings::loadString(UINT uID, TKEY key)
//{
//	_strings.push_back(new WCHAR[BUFSZ]); //Allocate memory
//	::LoadStringW(hInstance, uID, _strings.back(), BUFSZ); //Read string from resourse
//	this->insertIntoMap(key, _strings.back());
//}
//void strings::loadStrings(const std::vector<UINT>& stringIDs, const std::vector<TKEY>& keys)
//{
//	auto k_iter = keys.cbegin();
//	for (auto iter = stringIDs.cbegin(); iter != stringIDs.cend() && k_iter != keys.cend(); ++iter, ++k_iter)
//		loadString(*iter, *k_iter);
//}
//
