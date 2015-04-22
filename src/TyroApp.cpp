/***************************************************************
 * Name:	  TyroApp.cpp
 * Purpose:   Code for Application Class
 * Author:	Timothy J Warren (tim@timshomepage.net)
 * Created:   2015-03-30
 * Copyright: Timothy J Warren (https://timshomepage.net)
 * License:
 **************************************************************/

#include "wx_common.h"

#include <wx/app.h>
#include <wx/debug.h>

class TyroApp : public wxApp
{
	friend class MainFrame;
public:
	virtual bool OnInit();
	virtual int OnExit();
private:
};

//**************************************************************

#include "widgets/MainFrame.h"

IMPLEMENT_APP(TyroApp);

bool TyroApp::OnInit()
{
	this->SetAppName(APP_NAME);
	this->SetVendorName(APP_VENDOR);
	
	MainFrame* frame = new MainFrame(0L, APP_NAME);

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
