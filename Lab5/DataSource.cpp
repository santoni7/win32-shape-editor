#include "stdafx.h"
#include <commdlg.h>
#include <codecvt>
#include <vector>
#include <sstream>
#include "DataSource.h"
#include "ShapeFactory.h"
TCHAR* g_formats = 
	_T("TXT-File\0*.TXT\0All files\0*.*\0\0");
std::wstring GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	std::wstring wc(cSize, L'#');
	mbstowcs(&wc[0], c, cSize);

	return wc;
}

DataSource::DataSource(HWND hWnd)
{
	this->hWnd = hWnd;
}

DataSource::~DataSource()
{
}

/* DataWriter: */
bool DataWriter::OpenFile(const wchar_t* fileName) {
	std::wstring fname = fileName;
	this->_fname = fileName;
	out = std::wofstream(fname);
	return TRUE;
}

bool DataWriter::OpenFileDlg() {
	OPENFILENAME ofn;       
	wchar_t szFile[260];    
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = g_formats;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrDefExt = _T("txt");
	ofn.Flags = OFN_PATHMUSTEXIST; 
	if (GetSaveFileName(&ofn) == TRUE)
	{

		WCHAR* st = new WCHAR[255];
		wcscpy(st, ofn.lpstrFile);
		this->_fname = st;
		return OpenFile(ofn.lpstrFile);
	}
	return FALSE;
}

void DataWriter::CloseFile() {
	out.close();
}

void DataWriter::WriteNext(Shape* shape) {
	if (!out.is_open()) return;
	Shape::render_data rd = shape->GetRenderData();
	out << GetWC(shape->SimpleName()) << _T('\t')
		<< shape->p1.x << _T('\t') << shape->p1.y << _T('\t')
		<< shape->p2.x << _T('\t') << shape->p2.y << _T('\t')
		<< rd.fillCol << _T('\t') << rd.outlineCol << _T('\t') << (int)(rd.shouldFill ? 1 : 0) << _T('\n');
}

/* DataReader: */
bool DataReader::OpenFile(const wchar_t* fileName) {
	std::wstring fname = fileName;
	this->_fname = fileName;
	in = std::wifstream(fname);
	return TRUE;
}

bool DataReader::OpenFileDlg() {
	OPENFILENAME ofn;      
	wchar_t szFile[260];      
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = g_formats;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrDefExt = _T("txt");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn) == TRUE)
	{
		WCHAR* st = new WCHAR[255];
		wcscpy(st, ofn.lpstrFile);
		this->_fname = st;
		return OpenFile(ofn.lpstrFile);
	}
	return FALSE;
}

void DataReader::CloseFile() {
	in.close();
}

Shape* DataReader::ReadNext() {
	if (!in.is_open()) return nullptr;
	MPoint p1, p2;
	std::wstring simpleNameW;
	//std::wstring str;
	//do {
	//	std::getline(in, str, _T('\n'));
	//} while (str.empty() && !in.eof());
	//if (str.empty()) return nullptr;

	//std::wistringstream stream(str);
	Shape::render_data rd;
	in >> simpleNameW >> p1.x >> p1.y >> p2.x >> p2.y >> rd.fillCol >> rd.outlineCol >> rd.shouldFill;
	Shape*res =  ShapeFactory::fromWString(simpleNameW, p1, p2);
	if (res) {
		res->SetFillColor(rd.fillCol, rd.shouldFill);
		res->SetOutlineColor(rd.outlineCol);
	}
	return res;
}