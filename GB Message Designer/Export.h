#pragma once
#include "wx/wx.h"
#include "wx/wxprec.h"
#include "gbmsg.h"
#include <fstream>

class Export : public wxFrame
{
public:
	wxTextCtrl* fileText = nullptr;

public:
	Export(wxWindow* parent, std::string* exportPath, int rows, string message);
	void Browse(wxCommandEvent& evt);
	void Confirm(wxCommandEvent& evt);
	void Apply(wxCommandEvent& evt);
	void Cancel(wxCommandEvent& evt);

	string* exportPath = nullptr;
	int rows;
	string fileName = "";
	string message = "";

	wxDECLARE_EVENT_TABLE();
};
