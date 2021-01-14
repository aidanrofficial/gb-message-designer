#ifndef _COMMAND_H_
#define _COMMAND_H_
#pragma once
#include <list>

using namespace std;

namespace CMAN
{

class Command
{
public:
	virtual bool Execute() = 0;
	virtual bool Unexecute() = 0;
	virtual ~Command() { }

};

template <class T = Command>
class CommandManager
{
private:
	typedef list<T*> CommandList;
	typename CommandList undoList;
	typename CommandList redoList;
	int undoLevel;
	int cleanCount;

public:
	CommandManager(int UndoLevel = 100) : undoLevel(UndoLevel), cleanCount(0){ }
	~CommandManager(){Clear();}

public:
	bool CanUndo() const
	{
		return ((int)undoList.size() > 0);
	}

	bool CanRedo() const
	{
		return ((int)redoList.size() > 0);
	}

	int getUndoLevel() const
	{
		return undoLevel;
	}

	void setUndoLevel(int newValue)
	{
		undoLevel = newValue;
	}

	bool IsDirty() const
	{
		return (cleanCount != 0);
	}

	T* getLastUndoCommand() const
	{
		return undoList.back();
	}

	T* getLastRedoCommand() const
	{
		return redoList.back();
	}

public:
	void DoCommand(T* command)
	{
		ClearRedoList();
		if(command->Execute())
		{
			AddUndo(command);
		}
	}

	void Undo()
	{
		if(CanUndo())
		{
			cleanCount--;
			
			T* command = undoList.back();
			undoList.pop_back();

			if(command->Unexecute())
			{
				AddRedo(command);
			}
			else
			{
				delete command;
			}
		}
	}

	void Redo()
	{
		if(CanRedo())
		{
			cleanCount++;
			T* command = redoList.back();
			redoList.pop_back();

			if(command->Execute())
			{
				AddUndo(command);
			}
			else
			{
				delete command;
			}
		}
	}

	void Clear()
	{
		ClearUndoList();
		ClearRedoList();
	}

	void SetClean()
	{
		cleanCount = 0;
	}

	void AddUndo(T* command)
	{
		if((int)undoList.size() >= undoLevel)
		{
			delete undoList.front();
			undoList.pop_front();
		}

		undoList.push_back(command);

		if(cleanCount < 0 && redoList.size() > 0)
		{
			cleanCount = undoList.size() + redoList.size() + 1;
		}
		else
		{
			cleanCount++;
		}
	}

	void AddRedo(T* command)
	{
		redoList.push_back(command);
	}

	void ClearUndoList()
	{
		ClearCommandList(&undoList);
	}

	void ClearRedoList()
	{
		ClearCommandList(&redoList);
	}

	void ClearCommandList(CommandList* list)
	{
		typename CommandList::iterator it;

		for (it = list->begin(); it != list->end(); it++)
		{
			delete *it;
		}

		list->erase(list->begin(), list->end());
	}
};

}

#endif