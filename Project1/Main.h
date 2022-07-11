#pragma once

#include <wx/wx.h>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <wx/textdlg.h>
#include <wx/file.h>
#include <algorithm>
#include <unordered_set>
#include <filesystem>
#include <stdio.h>
#include "SimpleSerial.cpp"

class Main : public wxFrame
{
public:
	Main();

private:
	std::vector<int> vec;
	std::unordered_set<int> res;
	std::unordered_map<int, int> mp;

	wxStaticText* text = nullptr;
	wxStaticText* text2 = nullptr;
	SimpleSerial* serial = nullptr;
	wxButton* button = nullptr;
	wxButton* button2 = nullptr;
	wxButton* button3 = nullptr;
	wxButton* button4 = nullptr;
	wxTextCtrl* input = nullptr;

	std::atomic<bool> flag;
	std::atomic<bool> flag2;
	
	void begin(wxCommandEvent& evt);
	void handle();
	void mark(wxCommandEvent& evt);
	void end(wxCommandEvent& evt);

	void handle_result(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();
};

