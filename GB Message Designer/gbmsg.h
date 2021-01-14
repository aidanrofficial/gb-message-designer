#ifndef _FILE_H_
#define _FILE_H_
#pragma once
#include <string>
#include <fstream>

using namespace std;

struct GBMSGStruct
{
	char headerID[16];
	char version[6];
	char exportPath[120];
	int rows;
	char message[577];
};

namespace GBMSG
{

class GBMSG
{
private:
	const string headerID = "gameboymessage#";
	const string currentVersion = "1.0.0";

public:
	bool Load(string path, int& rows, string& version, string& exportPath, string& message)
	{
		ifstream inputFile(path, ios::binary);

		GBMSGStruct gbmsgStruct;

		if(path.find(".gbmsg") == string::npos)
		{
			return false;
		}

		inputFile.read((char *) &gbmsgStruct, sizeof(gbmsgStruct));

		if(gbmsgStruct.headerID != headerID)
		{
			return false;
		}

		version = gbmsgStruct.version;
		exportPath = gbmsgStruct.exportPath;

		rows = gbmsgStruct.rows;

		message.resize(577);

		for(int i = 0; i < 577; i++)
		{
			message[i] = gbmsgStruct.message[i];
		}

		message.resize(576);

		inputFile.close();

		return true;
	}
	 
	bool Save(string path, int rows, string exportPath, string message)
	{
		ofstream outputFile(path, ios::binary);

		GBMSGStruct gbmsgStruct;
		
		string tempMessage = message;
		string tempExportPath = exportPath;

		if(path.find(".gbmsg") == string::npos)
		{
			return false;
		}
		
		for(int i = 0; i < 16; i++)
		{
			gbmsgStruct.headerID[i] = headerID[i];
		}

		for(int i = 0; i < 6; i++)
		{
			gbmsgStruct.version[i] = currentVersion[i];
		}

		tempExportPath.resize(120);

		for(int i = 0; i < 120; i++)
		{
			gbmsgStruct.exportPath[i] = tempExportPath[i];
		}

		gbmsgStruct.rows = rows;

		tempMessage.resize(577);

		for(int i = 0; i < 577; i++)
		{
			gbmsgStruct.message[i] = tempMessage[i];
		}

		outputFile.write((char *) &gbmsgStruct, sizeof(gbmsgStruct));

		outputFile.close();

		return true;
	}

	bool Export(string path, int rows, string fileName, string message)
	{
		ofstream outputFile(path);

		outputFile << "const unsigned char " << fileName << "[] =" << endl;
		outputFile << "{";

		for(int i = 0; i < rows; i++)
		{
			outputFile << endl << "  ";

			for(int j = 0; j < 18; j++)
			{
				outputFile << CharLookup(message[18 * i + j]);
				
				if(i < rows - 1 || j < 17)
				{
					outputFile << ",";
				}
			}
		}

		outputFile << endl << "};";

		return true;
	}

	string CurrentVersion()
	{
		return currentVersion;
	}

	string CharLookup(char character)
	{
		switch(character)
		{
			case '\0':
				return "0xCC";
			case '%':
				return "0xFC";
			case '~':
				return "0xB4";
			case '[':
				return "0xB5";
			case ']':
				return "0xB6";
			case '"':
				return "0xB7";
			case ':':
				return "0xB8";
			case ',':
				return "0xB9";
			case '.':
				return "0xF9";
			case '\'':
				return "0xFA";
			case '-':
				return "0xFB";
			case '!':
				return "0xFE";
			case '?':
				return "0xFF";
			case '0':
				return "0xD5";
			case '1':
				return "0xD6";
			case '2':
				return "0xD7";
			case '3':
				return "0xD8";
			case '4':
				return "0xD9";
			case '5':
				return "0xDA";
			case '6':
				return "0xDB";
			case '7':
				return "0xDC";
			case '8':
				return "0xDD";
			case '9':
				return "0xDE";
			case 'A':
				return "0xDF";
			case 'B':
				return "0xE0";
			case 'C':
				return "0xE1";
			case 'D':
				return "0xE2";
			case 'E':
				return "0xE3";
			case 'F':
				return "0xE4";
			case 'G':
				return "0xE5";
			case 'H':
				return "0xE6";
			case 'I':
				return "0xE7";
			case 'J':
				return "0xE8";
			case 'K':
				return "0xE9";
			case 'L':
				return "0xEA";
			case 'M':
				return "0xEB";
			case 'N':
				return "0xEC";
			case 'O':
				return "0xED";
			case 'P':
				return "0xEE";
			case 'Q':
				return "0xEF";
			case 'R':
				return "0xF1";
			case 'S':
				return "0xF2";
			case 'T':
				return "0xF3";
			case 'U':
				return "0xF4";
			case 'V':
				return "0xF5";
			case 'W':
				return "0xF6";
			case 'X':
				return "0xF7";
			case 'Y':
				return "0xF8";
			case 'Z':
				return "0xF9";
			case 'a':
				return "0xBA";
			case 'b':
				return "0xBB";
			case 'c':
				return "0xBC";
			case 'd':
				return "0xBD";
			case 'e':
				return "0xBE";
			case 'f':
				return "0xBF";
			case 'g':
				return "0xC0";
			case 'h':
				return "0xC1";
			case 'i':
				return "0xC2";
			case 'j':
				return "0xC3";
			case 'k':
				return "0xC4";
			case 'l':
				return "0xC5";
			case 'm':
				return "0xC6";
			case 'n':
				return "0xC7";
			case 'o':
				return "0xC8";
			case 'p':
				return "0xC9";
			case 'q':
				return "0xCA";
			case 'r':
				return "0xCB";
			case 's':
				return "0xCD";
			case 't':
				return "0xCE";
			case 'u':
				return "0xCF";
			case 'v':
				return "0xD0";
			case 'w':
				return "0xD1";
			case 'x':
				return "0xD2";
			case 'y':
				return "0xD3";
			case 'z':
				return "0xD4";
			default:
				return "0xCC";
		}
	}
};

}

#endif