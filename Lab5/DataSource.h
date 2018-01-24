#pragma once
#include <fstream>
#include "shape.h"
class DataSource
{
protected:
	HWND hWnd;
	std::wstring _fname;
public:
	DataSource(HWND);
	virtual ~DataSource();

	virtual bool OpenFile(const wchar_t*) = 0;
	virtual bool OpenFileDlg() = 0;
	virtual void CloseFile() = 0;
};

class DataWriter : DataSource {
	std::wofstream out;
public:
	DataWriter(HWND hWnd) : DataSource(hWnd) { }
	bool OpenFile(const wchar_t*) override;
	bool OpenFileDlg() override;
	void CloseFile() override;

	void WriteNext(Shape*);
};

class DataReader : DataSource {
	std::wifstream in;
public:
	DataReader(HWND hWnd) : DataSource(hWnd) { }
	bool OpenFile(const wchar_t*) override;
	bool OpenFileDlg() override;
	void CloseFile() override;
	
	Shape* ReadNext();
};