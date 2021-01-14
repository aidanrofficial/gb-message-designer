#include "MessagePanel.h"
#include "iostream"

BEGIN_EVENT_TABLE(MessagePanel, wxPanel)
   EVT_GRID_RANGE_SELECT(MessagePanel::OnSelect)
END_EVENT_TABLE()

MessagePanel::MessagePanel(wxWindow* parent, wxWindowID id, int rows, wxMenu* editMenu, bool* edited) : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
	this->rows = rows;
	this->editMenu = editMenu;
	this->edited = edited;

	characters = new wxStaticText*[1];

	this->SetBackgroundColour(*wxBLACK);

	grid = new wxGrid(this, 10000, wxDefaultPosition, wxDefaultSize);
	grid->CreateGrid(rows, 18);
	grid->DisableDragRowSize();
	grid->DisableDragColSize();
	grid->SetDefaultColSize(30, true);
	grid->SetDefaultRowSize(30, true);
	grid->SetRowLabelSize(30);
	grid->SetDefaultCellBackgroundColour(*wxBLACK);
	grid->SetDefaultCellTextColour(*wxWHITE);
	grid->EnableEditing(false);
	grid->SetCellHighlightColour(*wxRED);
	grid->SetTabBehaviour(grid->Tab_Stop);

	for(int i = 0; i < 18; i++)
	{
		grid->SetColLabelValue(i, wxString::Format("%i", i + 1));
	}

	wxBoxSizer* gridBoxSizer = new wxBoxSizer(wxVERTICAL);
	gridBoxSizer->Add(grid, 1, wxEXPAND);

	this->SetSizer(gridBoxSizer);

	message = new wxString("");
	message->resize(577);

	grid->Bind(wxEVT_CHAR, &MessagePanel::OnKeyDown, this);
	grid->Bind(wxEVT_CHAR_HOOK, &MessagePanel::OnSpecialKeyDown, this);

	grid->SetGridCursor(0, 0);
}

MessagePanel::~MessagePanel()
{
}

void MessagePanel::AddChar(char character)
{
	int cursorCol = grid->GetGridCursorCol();
	int cursorRow = grid->GetGridCursorRow();
	int charPos = 18 * cursorRow + cursorCol;

	grid->SetCellValue(cursorRow, cursorCol, character);

	message->replace(charPos, 1, character);

	if(cursorCol == 17 && cursorRow < this->rows - 1)
	{
		grid->SetGridCursor(cursorRow, 0);
		grid->MoveCursorDown(false);
	}
	else
	{
		grid->MoveCursorRight(false);
	}

	*edited = true;
}

void MessagePanel::RemoveChar()
{
	int cursorCol = grid->GetGridCursorCol();
	int cursorRow = grid->GetGridCursorRow();
	int charPos = 18 * cursorRow + cursorCol;

	grid->SetCellValue(cursorRow, cursorCol, ' ');

	if(cursorCol == 0 && cursorRow > 0)
	{
		grid->SetGridCursor(cursorRow, 17);
		grid->MoveCursorUp(false);
	}
	else
	{
		grid->MoveCursorLeft(false);
	}

	*edited = true;
}

void MessagePanel::OnKeyDown(wxKeyEvent& event)
{
	int keyCode = event.GetKeyCode();

	if(rows > 0)
	{
		if(keyCode != KEY_BACKSPACE && keyCode != KEY_LEFT && keyCode != KEY_RIGHT && keyCode != KEY_UP && keyCode != KEY_DOWN && keyCode != KEY_SHIFT && keyCode != KEY_CONTROL_Z && keyCode != KEY_CONTROL_Y && wxGetKeyState(WXK_CONTROL) == false)
		{
			AddCharCommand* command = new AddCharCommand(this, keyCode);
			commandManager.DoCommand(command);
		}

		if(keyCode == KEY_BACKSPACE)
		{
			RemoveCharCommand* command = new RemoveCharCommand(this);
			commandManager.DoCommand(command);
		}

		if(keyCode == KEY_CONTROL_Z)
		{
			commandManager.Undo();
		}

		if(keyCode == KEY_CONTROL_Y)
		{
			commandManager.Redo();
		}

		if(commandManager.CanUndo())
		{
			editMenu->Enable(wxID_UNDO, true);
		}
		else
		{
			editMenu->Enable(wxID_UNDO, false);
		}

		if(commandManager.CanRedo())
		{
			editMenu->Enable(wxID_REDO, true);
		}
		else
		{
			editMenu->Enable(wxID_REDO, false);
		}
	}

	event.Skip();
}

void MessagePanel::OnSpecialKeyDown(wxKeyEvent& event)
{
	int keyCode = event.GetKeyCode();

	if(rows > 0)
	{
		if(keyCode == KEY_SPACE)
		{
			AddCharCommand* command = new AddCharCommand(this, ' ');
			commandManager.DoCommand(command);

			return;
		}

		if(keyCode == KEY_ENTER)
		{
			int cursorRow = grid->GetGridCursorRow();
			grid->SetGridCursor(cursorRow, 0);
			grid->MoveCursorDown(false);

			return;
		}
	}

	event.Skip();
}

void MessagePanel::OnSelect(wxGridRangeSelectEvent& event)
{
	wxGridCellCoordsArray selTop = grid->GetSelectionBlockTopLeft();

	for(int i = 0; i < (int)selTop.Count(); i++)
	{
		grid->ClearSelection();
	}

	event.Skip();
}

void MessagePanel::ResizeMessage(int newRows)
{
	int currentRows = grid->GetNumberRows();

	if(currentRows < newRows)
	{
		grid->AppendRows(newRows - currentRows);
	}
	else if(currentRows > newRows)
	{
		grid->DeleteRows(newRows, currentRows - newRows);
	}

	message->resize(newRows * 18);
	message->resize(576);
}

void MessagePanel::DisplayMessage()
{
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < 18; j++)
		{
			grid->SetCellValue(i, j, message->ToStdString()[(18 * i) + j]);
		}
	}
}
