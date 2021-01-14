#pragma once
#include "wx/wx.h"
#include "wx/grid.h"
#include "command.h"

#define KEY_BACKSPACE 8
#define KEY_SHIFT 306
#define KEY_ENTER 13
#define KEY_LEFT 314
#define KEY_UP 315
#define KEY_RIGHT 316
#define KEY_DOWN 317
#define KEY_SPACE 32
#define KEY_Z 122
#define KEY_CONTROL_Z 26
#define KEY_CONTROL_Y 25

class MessagePanel : public wxPanel
{
public:
	MessagePanel(wxWindow* parent, wxWindowID id, int rows, wxMenu* editMenu, bool* edited);
	~MessagePanel();

	void AddChar(char character);
	void RemoveChar();

	void OnKeyDown(wxKeyEvent& event);
	void OnSpecialKeyDown(wxKeyEvent& event);
	void OnSelect(wxGridRangeSelectEvent& event);

	void ResizeMessage(int newRows);
	void DisplayMessage();

public:
	wxWindow* parent = nullptr;
	wxGrid* grid = nullptr;

	wxMenu* editMenu = nullptr;

	wxString* message = nullptr;
	wxStaticText** characters;
	int rows = 8;
	bool* edited = nullptr;

	CMAN::CommandManager<CMAN::Command> commandManager;

	wxDECLARE_EVENT_TABLE();

};

class AddCharCommand : public CMAN::Command
{
private:
	MessagePanel* messagePanel;
	char charToAdd;
	char savedChar;
	wxPoint cursorPos;

public:
	AddCharCommand(MessagePanel* panel, char character)
	{
		this->messagePanel = panel;
		this->charToAdd = character;
	}

	virtual ~AddCharCommand()
	{
	}

	virtual bool Execute()
	{
		cursorPos.x = messagePanel->grid->GetGridCursorCol();
		cursorPos.y = messagePanel->grid->GetGridCursorRow();
		savedChar = messagePanel->grid->GetCellValue(cursorPos.y, cursorPos.x).ToStdString()[0];

		messagePanel->AddChar(charToAdd);
		return true;
	}

	virtual bool Unexecute()
	{
		messagePanel->grid->SetGridCursor(cursorPos.y, cursorPos.x);

		messagePanel->grid->SetCellValue(cursorPos.y, cursorPos.x, savedChar);
		
		return true;
	}
};

class RemoveCharCommand : public CMAN::Command
{
private:
	MessagePanel* messagePanel;
	char savedChar;
	wxPoint cursorPos;

public:
	RemoveCharCommand(MessagePanel* panel)
	{
		this->messagePanel = panel;
	}

	virtual ~RemoveCharCommand()
	{
	}

	virtual bool Execute()
	{
		cursorPos.x = messagePanel->grid->GetGridCursorCol();
		cursorPos.y = messagePanel->grid->GetGridCursorRow();
		savedChar = messagePanel->grid->GetCellValue(cursorPos.y, cursorPos.x).ToStdString()[0];

		messagePanel->RemoveChar();
		return true;
	}

	virtual bool Unexecute()
	{
		messagePanel->grid->SetGridCursor(cursorPos.y, cursorPos.x);

		messagePanel->grid->SetCellValue(cursorPos.y, cursorPos.x, savedChar);
		
		return true;
	}
};