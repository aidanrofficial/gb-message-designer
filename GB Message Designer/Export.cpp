#include "Export.h"

using namespace std;

#define wxID_BROWSE 7001

BEGIN_EVENT_TABLE(Export, wxFrame)
	EVT_BUTTON(wxID_OK, Export::Confirm)
	EVT_BUTTON(wxID_APPLY, Export::Apply)
	EVT_BUTTON(wxID_CANCEL, Export::Cancel)
	EVT_BUTTON(wxID_BROWSE, Export::Browse)
END_EVENT_TABLE()

Export::Export(wxWindow* parent, std::string* exportPath, int rows, string message) : wxFrame(parent, wxID_ANY, "Export Options", wxPoint(200, 200), wxSize(400, 150), wxWANTS_CHARS | wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
	this->exportPath = exportPath;
	this->rows = rows;
	this->message = message;

	wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
	wxPanel* rowsPanel = new wxPanel(mainPanel, wxID_ANY, wxPoint(10, 10), wxSize(365, 50), wxBORDER_RAISED);

	wxButton* okButton = new wxButton(mainPanel, wxID_OK, "OK", wxPoint(140, 75), wxSize(75, 30), wxBORDER_RAISED);
	wxButton* applyButton = new wxButton(mainPanel, wxID_APPLY, "Apply", wxPoint(220, 75), wxSize(75, 30), wxBORDER_RAISED);
	wxButton* cancelButton = new wxButton(mainPanel, wxID_CANCEL, "Cancel", wxPoint(300, 75), wxSize(75, 30), wxBORDER_RAISED);
	wxButton* browseButton = new wxButton(rowsPanel, wxID_BROWSE, "Browse", wxPoint(280, 10), wxSize(60, 25), wxBORDER_RAISED);

	wxStaticText* sizeText = new wxStaticText(mainPanel, wxID_ANY, "File");
	sizeText->SetPosition(wxPoint(20, 5));

	fileText = new wxTextCtrl(rowsPanel, wxID_ANY, *this->exportPath, wxPoint(40, 10), wxSize(225, 25), wxTE_LEFT | wxTE_CHARWRAP);
}

void Export::Browse(wxCommandEvent &evt)
{
	wxFileDialog fileDialog(this, "Export Message File", "", ".c", "*.c Files (*.c)|*c", wxFD_SAVE);
	
	if(fileDialog.ShowModal() == wxID_OK)
	{
		fileText->SetLabel(fileDialog.GetPath());
	}

	evt.Skip();
}

void Export::Confirm(wxCommandEvent &evt)
{
	*exportPath = fileText->GetValue().ToStdString();

	GBMSG::GBMSG gbmsgManager;

	string fileName;
	string tempPath = fileText->GetValue().ToStdString();

	for(int i = tempPath.length() - 3; i > 0; i--)
	{
		if(tempPath[i] == '\\')
		{
			break;
		}

		fileName += tempPath[i];
	}

	reverse(fileName.begin(), fileName.end());

	gbmsgManager.Export(*exportPath, rows, fileName, message);

	this->Close();
}

void Export::Apply(wxCommandEvent &evt)
{
	*exportPath = fileText->GetValue().ToStdString();
}


void Export::Cancel(wxCommandEvent &evt)
{
	this->Close();
}