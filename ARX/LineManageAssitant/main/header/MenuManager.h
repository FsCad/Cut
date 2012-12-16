#pragma once

#include "MenuLMAMain.h"

class MenuManager
{
public:

	static void CreateMenu(void* appId);

	static MenuManager* instance();

	static void unRegister();

private:

	MenuManager(const void* appId);

	~MenuManager(void);

	static MenuManager* gMenuManager;

	const void* mAppId;

	MenuLMAMain* mpMainMenu;

	/**
	* ע��ϵͳ�˵�
	**/
	void RegisterMenu();

	/**
	* ע��ϵͳ�˵�
	**/
	void UnRegisterMenu();
};

