#include "MenuLMAMain.h"

#include "Resource.h"
#include <acdocman.h>
#include <acutads.h>

#include "StdAfx.h"

extern HINSTANCE _hdllInstance = NULL;//全局变量

MenuLMAMain::MenuLMAMain(void)
{
	acutPrintf(L"开始构造主菜单。");
	acDocManagerPtr()->pushResourceHandle(_hdllInstance);
    
	this->mpMenu = new CMenu();
	this->mpMenu->LoadMenu(IDR_MENU1);// ID_LMA_MAIN_MENU 是要调入的菜单 

    acDocManager->popResourceHandle();
	acutPrintf(L"主菜单构造完成。");
}

MenuLMAMain::~MenuLMAMain(void)
{
	if (mpMenu) 
        delete mpMenu;
}

void* MenuLMAMain::getMenuContext(const AcRxClass *, const AcDbObjectIdArray&)
{
	acutPrintf(L"得到子菜单");
	m_tempHMenu = mpMenu->GetSubMenu(0)->GetSafeHmenu();   
	return &m_tempHMenu;
}

void MenuLMAMain::onCommand(Adesk::UInt32 cmdIndex)
{
    acDocManager->pushResourceHandle(_hdllInstance);

    CString strMenuTitle,strPrompt;
    mpMenu->GetMenuString(cmdIndex,strMenuTitle,MF_BYCOMMAND);

	strPrompt.Format(L"\n您已经选取了菜单:%s\n",strMenuTitle);
	acutPrintf(strPrompt); // 我们的例子显示哪一个菜单项被选择

    acedPostCommandPrompt();
	acDocManager->pushResourceHandle(_hdllInstance); // 切换当前使用的资源
}

void MenuLMAMain::OnUpdateMenu()
{
	//mpMenu->EnableMenuItem(ID_LINE_CONF,MF_GRAYED); // 使菜单变灰
	//mpMenu->EnableMenuItem(ID_LINE_INPUT,MF_ENABLED); // 使能菜单项
	//mpMenu->CheckMenuItem(ID_GENERATE_CUT, MF_BYCOMMAND|MF_CHECKED); // 复选菜单项 
}

