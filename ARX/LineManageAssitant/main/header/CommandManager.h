#pragma once

#include <map>
#include <string>

#include <accmd.h>

using namespace std;

#define MAIN_MENU_NAME L"���߸���ϵͳ"

#define MAIN_MENU_LINE_CONFIG L"���Ͷ���"

#define MAIN_MENU_LINE_INPUT L"λ��¼��"

#define MAIN_MENU_LINE_CUT L"������ͼ"

#define MAIN_MENU_LINE_CUT_BACK L"�ָ��Ӵ�"

#define MAIN_MENU_LINE_TEST L"���ܲ���"

class CommandManager
{
public:

	static CommandManager* gCmdManager;

	static CommandManager* instance();

	static void Release();

	void RegisterCommand() const;

	void UnRegisterCommand() const;

	static void ShowConfigDialog();

	static void ShowAddLineDialog();

	static void GenerateCut();

	static void GenerateCutBack();

	static void TestFunction();

	static const WCHAR* CMD_GROUP;

	static const WCHAR* CMD_LINE_CONFIG; 
	static const WCHAR* CMD_LINE_INPUT;
	static const WCHAR* CMD_LIEN_CUT;
	static const WCHAR* CMD_LINE_CUT_BACK;
	static const WCHAR* CMD_LINE_TEST;

private:

	CommandManager(void);
	~CommandManager(void);

	//list to contains the commands supported
	map<wstring,AcRxFunctionPtr> mSupportCommands;
};
