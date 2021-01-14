#ifndef _MAIN_H_
#define _MAIN_H_
#pragma once
#include "wx/wx.h"
#include "MessagePanel.h"
#include "Properties.h"
#include "Export.h"
#include "gbmsg.h"
#include <fstream>

using namespace std;

class Main : public wxFrame
{
public:
	Main();
	~Main();

	bool SaveFile();
	void SaveFileAs();
	void OpenFile();
	void ExportFile();

	void Undo();
	void Redo();

	string GetNameFromPath(string path);

	void OnMenuOpen(wxCommandEvent& evt);
	void OnMenuSave(wxCommandEvent &evt);
	void OnMenuSaveAs(wxCommandEvent& evt);
	void OnMenuProperties(wxCommandEvent& evt);
	void OnMenuExport(wxCommandEvent& evt);
	void OnMenuClose(wxCommandEvent& evt);

	void OnMenuUndo(wxCommandEvent& evt);
	void OnMenuRedo(wxCommandEvent& evt);

	void OnCloseWindow(wxCloseEvent& evt);

public:
	wxMenuBar* menuBar = nullptr;
	wxMenu* fileMenu = nullptr;
	wxMenu* editMenu = nullptr;

	MessagePanel* messagePanel = nullptr;
	Properties* propertiesFrame = nullptr;
	Export* exportFrame = nullptr;

	string filePath = "";
	string exportPath = "";
	string version = "";

	bool edited = false;

	wxDECLARE_EVENT_TABLE();
};

#endif