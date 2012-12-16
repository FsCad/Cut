
#include "MenuLMAMain.h"

#include "MenuManager.h"
#include "aced.h"

#include "StdAfx.h"

MenuManager* MenuManager::gMenuManager = NULL;

void MenuManager::CreateMenu(void* appId)
{
	if( gMenuManager == NULL )
	{
		gMenuManager = new MenuManager(appId);
	}
}

MenuManager* MenuManager::instance()
{
	assert(gMenuManager);

	return gMenuManager;
}

void MenuManager::unRegister()
{
	if( gMenuManager )
	{
		delete gMenuManager;
	}
}

MenuManager::MenuManager(const void* appId)
	:mAppId(appId)
{
	RegisterMenu();
}

MenuManager::~MenuManager(void)
{
	UnRegisterMenu();
}

void MenuManager::RegisterMenu()
{
	mpMainMenu = new MenuLMAMain();
	acedAddDefaultContextMenu(mpMainMenu,mAppId,L"���߸���ϵͳ");

	acutPrintf(L"�˵����سɹ�\n");
}

void MenuManager::UnRegisterMenu()
{
	if( mpMainMenu )
	{
		acutPrintf(L"����ϵͳ�˵���ʼж��\n");
		acedRemoveDefaultContextMenu(mpMainMenu ); // �Ƴ�Ĭ�������Ĳ˵�
	}

	acutPrintf(L"�˵�ж�سɹ�\n");
}
