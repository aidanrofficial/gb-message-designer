#include "Properties.h"

using namespace std;

BEGIN_EVENT_TABLE(Properties, wxFrame)
	EVT_BUTTON(wxID_OK, Properties::Confirm)
	EVT_BUTTON(wxID_CANCEL, Properties::Cancel)
END_EVENT_TABLE()

Properties::Properties(wxWindow* parent, MessagePanel* messagePanel, bool* edited) : wxFrame(parent, wxID_ANY, "Message Properties", wxPoint(200, 200), wxSize(300, 150), wxWANTS_CHARS | wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
	this->messagePanel = messagePanel;
	currentRows = this->messagePanel->rows;
	this->edited = edited;

	wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
	wxPanel* rowsPanel = new wxPanel(mainPanel, wxID_ANY, wxPoint(10, 10), wxSize(265, 50), wxBORDER_RAISED);

	wxButton* okButton = new wxButton(mainPanel, wxID_OK, "OK", wxPoint(120, 75), wxSize(75, 30), wxBORDER_RAISED);
	wxButton* cancelButton = new wxButton(mainPanel, wxID_CANCEL, "Cancel", wxPoint(200, 75), wxSize(75, 30), wxBORDER_RAISED);

	wxStaticText* sizeText = new wxStaticText(mainPanel, wxID_ANY, "Size");
	sizeText->SetPosition(wxPoint(20, 5));

	rowsValidator = new wxTextValidator(wxFILTER_NUMERIC);
	rowsText = new wxTextCtrl(rowsPanel, wxID_ANY, to_string(currentRows), wxPoint(40, 10), wxSize(40, 25), wxTE_LEFT, *rowsValidator);
}

void Properties::Confirm(wxCommandEvent &evt)
{
	int enteredRows = stoi(rowsText->GetValue().ToStdString());

	if(enteredRows > 32)
	{
		wxMessageBox(wxString::Format("The number of rows must be less than 32."));
	}
	else
	{
		messagePanel->rows = enteredRows;
		messagePanel->ResizeMessage(enteredRows);

		*edited = true;

		this->Close();
	}

	evt.Skip();
}

void Properties::Cancel(wxCommandEvent &evt)
{
	this->Close();
}