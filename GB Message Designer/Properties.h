#pragma once
#include "wx/wx.h"
#include "wx/wxprec.h"
#include "MessagePanel.h"

class Properties : public wxFrame
{
public:
	wxTextValidator* rowsValidator = nullptr;
	wxTextCtrl* rowsText = nullptr;
	int currentRows = 0;
	bool* edited;

public:
	Properties(wxWindow* parent, MessagePanel* messagePanel, bool* editable);
	void Confirm(wxCommandEvent& evt);
	void Cancel(wxCommandEvent& evt);

	MessagePanel* messagePanel = nullptr;

	wxDECLARE_EVENT_TABLE();
};

