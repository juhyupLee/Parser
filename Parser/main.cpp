#include <iostream>
#include "Parser.h"

using namespace std;

int main()
{
	Parser parser;
	parser.Parser_LoadFile(L"config.txt");
	
	int serverID = 0;
	char szServerBindIP[255];
	int serverBindPort = 0;
	int WorkerThread = 0;
	int MaxUser = 0;
	char szServerValue[255];

	parser.GetValue_Int(L"ServerID", &serverID);
	//parser.GetValue_Str("ServerBindIP", szServerBindIP, 255);
	//parser.GetValue_Int("ServerBindPort", &serverBindPort);
	//parser.GetValue_Int("WorkerThread", &WorkerThread);
	//parser.GetValue_Int("MaxUser", &MaxUser);
	//parser.GetValue_Group("Server", szServerValue, 255);

	cout << serverID << endl;
	cout << szServerBindIP << endl;
	cout << serverBindPort << endl;
	cout << WorkerThread << endl;
	cout << MaxUser << endl;
	cout << szServerValue << endl;

	//int Level = 0;
	//int Power = 0;
	//int HP = 0;
	//int MP = 0;
	//int EXP = 0;
	//char szPlayerValue[255];

	//parser.GetValue_Int("Level", &Level);
	//parser.GetValue_Int("Power", &Power);
	//parser.GetValue_Int("HP", &HP);
	//parser.GetValue_Int("MP", &MP);
	//parser.GetValue_Int("EXP", &EXP);
	//parser.GetValue_Group("Player", szPlayerValue, 255);

	//cout << Level << endl;
	//cout << Power << endl;
	//cout << HP << endl;
	//cout << MP << endl;
	//cout << EXP << endl;
	//cout << szPlayerValue << endl;

	return 0;
}