/***************************************************************
 * Name:	  TyroApp.cpp
 * Purpose:   Code for Application Class
 * Author:	Timothy J Warren (tim@timshomepage.net)
 * Created:   2015-03-30
 * Copyright: Timothy J Warren (https://timshomepage.net)
 * License:
 **************************************************************/

#include "common.h"
#include "TyroApp.h"
#include "widgets/MainFrame.h"

IMPLEMENT_APP(TyroApp);

bool TyroApp::OnInit()
{
	this->SetAppName(APP_NAME);
	this->SetVendorName(APP_VENDOR);
	
	MainFrame* frame = new MainFrame(0L, "Tyro");

	SetTopWindow(frame);
	
	frame->Layout();
	frame->CenterOnScreen();
	frame->Show(true);

	return true;
}

int TyroApp::OnExit()
{
	return close(true);
}
