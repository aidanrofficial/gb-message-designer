#include "Main.h"

using namespace std;

#define wxID_EXPORT 6001

wxBEGIN_EVENT_TABLE(Main, wxFrame)
EVT_CLOSE(Main::OnCloseWindow)
EVT_MENU(wxID_OPEN, Main::OnMenuOpen)
EVT_MENU(wxID_SAVE, Main::OnMenuSave)
EVT_MENU(wxID_SAVEAS, Main::OnMenuSaveAs)
EVT_MENU(wxID_PROPERTIES, Main::OnMenuProperties)
EVT_MENU(wxID_EXPORT, Main::OnMenuExport)
EVT_MENU(wxID_CLOSE, Main::OnMenuClose)
EVT_MENU(wxID_UNDO, Main::OnMenuUndo)
EVT_MENU(wxID_REDO, Main::OnMenuRedo)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "GB Message Designer", wxPoint(200, 200), wxSize(603, 350), wxWANTS_CHARS | wxDEFAULT_FRAME_STYLE)
{
	this->SetIcon(wxIcon(wxT("Game_Boy.ico"), wxBITMAP_TYPE_ICO));

	menuBar = new wxMenuBar();
	fileMenu = new wxMenu();
	editMenu = new wxMenu();

	fileMenu->Append(wxID_OPEN, "Open");
	fileMenu->Append(wxID_SAVE, "Save");
	fileMenu->Append(wxID_SAVEAS, "Save As");

	fileMenu->AppendSeparator();

	fileMenu->Append(wxID_PROPERTIES, "Message Properties");

	fileMenu->AppendSeparator();

	fileMenu->Append(wxID_EXPORT, "Export");

	fileMenu->AppendSeparator();

	fileMenu->Append(wxID_CLOSE, "Close");

	editMenu->Append(wxID_UNDO, "Undo");
	editMenu->Append(wxID_REDO, "Redo");

	menuBar->Append(fileMenu, "File");
	menuBar->Append(editMenu, "Edit");
	SetMenuBar(menuBar);

	editMenu->Enable(wxID_UNDO, false);
	editMenu->Enable(wxID_REDO, false);

	messagePanel = new MessagePanel(this, wxID_ANY, 0, editMenu, &this->edited);
}

Main::~Main()
{
}

bool Main::SaveFile()
{
	wxFileDialog fileDialog(this, "Save Message File", "", ".gbmsg", "*.gbmsg Files (*.gbmsg)|*gbmsg", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	ofstream file;

	if(filePath == "")
	{
		if(fileDialog.ShowModal() == wxID_OK)
		{
			GBMSG::GBMSG gbmsgManager;

			string tempPath;
			tempPath = fileDialog.GetPath();

			string tempMessage;
			tempMessage = messagePanel->message->ToStdString();

			if(gbmsgManager.Save(tempPath, messagePanel->rows, exportPath, tempMessage))
			{
				messagePanel->message->ToStdString() = tempMessage;

				this->SetTitle(wxString::Format("GB Message Designer - %s", GetNameFromPath(tempPath)));

				filePath = tempPath;

				edited = false;

				return true;
			}
			else
			{
				wxMessageBox(wxString::Format("Could not save: %s", tempPath));

				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		GBMSG::GBMSG gbmsgManager;

		string tempMessage;
		tempMessage = messagePanel->message->ToStdString();

		if(gbmsgManager.Save(filePath, messagePanel->rows, exportPath, tempMessage))
		{
			messagePanel->message->ToStdString() = tempMessage;

			this->SetTitle(wxString::Format("GB Message Designer - %s", GetNameFromPath(filePath)));

			edited = false;

			return true;
		}
		else
		{
			wxMessageBox(wxString::Format("Could not save: %s", filePath));

			return false;
		}
	}
}

void Main::SaveFileAs()
{
	wxFileDialog fileDialog(this, "Save Message File As", "", ".gbmsg", "*.gbmsg Files (*.gbmsg)|*gbmsg", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	ofstream file;

	if(fileDialog.ShowModal() == wxID_OK)
	{
		string tempPath;
		tempPath = fileDialog.GetPath();

		GBMSG::GBMSG gbmsgManager;

		string tempMessage;
		tempMessage = messagePanel->message->ToStdString();
		
		if(gbmsgManager.Save(tempPath, messagePanel->rows, exportPath, tempMessage))
		{
			wxMessageBox(wxString::Format("Saved: %s", tempPath));

			this->SetTitle(wxString::Format("GB Message Designer - %s", GetNameFromPath(tempPath)));

			filePath = tempPath;

			edited = false;
		}
		else
		{
			wxMessageBox(wxString::Format("Could not save: %s", tempPath));
		}
	}
}

void Main::OpenFile()
{
	wxFileDialog fileDialog(this, "Open Message File", "", "", "*.gbmsg Files (*.gbmsg)|*gbmsg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	ifstream file;

	if(fileDialog.ShowModal() == wxID_OK)
	{
		string tempPath;
		tempPath = fileDialog.GetPath();
		
		GBMSG::GBMSG gbmsgManager;

		int tempRows;
		string tempVersion;
		string tempMessage;
		string tempExportPath;

		if(gbmsgManager.Load(tempPath, tempRows, tempVersion, tempExportPath, tempMessage))
		{
			messagePanel->ResizeMessage(tempRows);
			messagePanel->rows = tempRows;

			filePath = tempPath;
			exportPath = tempExportPath;
			version = tempVersion;

			messagePanel->message->Clear();
			messagePanel->message->append(tempMessage);

			messagePanel->DisplayMessage();

			this->SetTitle(wxString::Format("GB Message Designer - %s", GetNameFromPath(tempPath)));

			edited = false;
		}
		else
		{
			wxMessageBox(wxString::Format("Could not open: %s", filePath));
		}
	}
}

void Main::ExportFile()
{
	
}

void Main::Undo()
{
	messagePanel->commandManager.Undo();

	if(!messagePanel->commandManager.CanUndo())
	{
		editMenu->Enable(wxID_UNDO, false);
	}
}

void Main::Redo()
{
	messagePanel->commandManager.Redo();

	if(!messagePanel->commandManager.CanRedo())
	{
		editMenu->Enable(wxID_REDO, false);
	}
}

string Main::GetNameFromPath(string path)
{
	string fileName;

	for(int i = path.length() - 1; i > 0; i--)
	{
		if(path[i] == '\\')
		{
			break;
		}

		fileName += path[i];
	}

	reverse(fileName.begin(), fileName.end());

	return fileName;
}

void Main::OnMenuOpen(wxCommandEvent &evt)
{
	OpenFile();
	evt.Skip();
}

void Main::OnMenuSave(wxCommandEvent &evt)
{
	SaveFile();
	evt.Skip();
}

void Main::OnMenuSaveAs(wxCommandEvent &evt)
{
	SaveFileAs();
	evt.Skip();
}

void Main::OnMenuProperties(wxCommandEvent &evt)
{
	propertiesFrame = new Properties(this, messagePanel, &edited);
	propertiesFrame->Show(true);
	evt.Skip();
}

void Main::OnMenuExport(wxCommandEvent & evt)
{
	exportFrame = new Export(this, &exportPath, messagePanel->rows, messagePanel->message->ToStdString());
	exportFrame->Show(true);
	evt.Skip();
}

void Main::OnMenuClose(wxCommandEvent &evt)
{
	Close();
	evt.Skip();
}

void Main::OnMenuUndo(wxCommandEvent &evt)
{
	Undo();
	evt.Skip();
}

void Main::OnMenuRedo(wxCommandEvent &evt)
{
	Redo();
	evt.Skip();
}

void Main::OnCloseWindow(wxCloseEvent &evt)
{
	if(edited == true)
	{
		wxMessageDialog* saveMessage = new wxMessageDialog(nullptr, "Save before closing?", "Confirm", wxYES_NO | wxCANCEL | wxYES_DEFAULT, wxDefaultPosition);
		int buttonPressed = saveMessage->ShowModal();

		if(buttonPressed == wxID_YES)
		{
			if(SaveFile())
			{
				evt.Skip();
			}
		}
		else if(buttonPressed == wxID_NO)
		{
			evt.Skip();
		}
		else if(buttonPressed == wxID_CANCEL)
		{
		
		}
	}
	else
	{
		evt.Skip();
	}
}