#include "widget.h"

class GeneralPrefPanePage : public wxPanel {
public:
	GeneralPrefPanePage(wxWindow *parent)
	: wxPanel(parent)
	{
		showLineNumbers = new wxCheckBox(this, myID_PREFS_LINE_NUMBERS, "Show line numbers");
		showIndentGuides = new wxCheckBox(this, myID_PREFS_IDENT_GUIDES, "Show indent guides");
		showCodeFolding = new wxCheckBox(this, myID_PREFS_CODE_FOLDING, "Show code folding");
		
		wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		
		sizer->Add(showLineNumbers, wxSizerFlags().Border());
		sizer->Add(showIndentGuides, wxSizerFlags().Border());
		sizer->Add(showCodeFolding, wxSizerFlags().Border());
		
		this->SetSizerAndFit(sizer);
		
		// Change settings on selection, rather than on apply button
		// On supported platforms
		if (wxPreferencesEditor::ShouldApplyChangesImmediately())
		{
			// @TODO add event handlers
		}
	}
	
	/**
	 * Apply current settings to the pref window
	 * 
     * @return bool
     */
	virtual bool TransferDataToWindow()
	{
		showLineNumbers->SetValue(true);
		return true;
	}
	
	/**
	 * Called on platforms with modal preferences dialog to save 
	 * and apply the changes
	 * 
     * @return bool 
     */
	virtual bool TransferDataFromWindow()
	{
		return true;
	}
	
	~GeneralPrefPanePage()
	{
		
	}
private:
	wxCheckBox *showLineNumbers;
	wxCheckBox *showIndentGuides;
	wxCheckBox *showCodeFolding;
};

class GeneralPrefPane: public wxStockPreferencesPage {
public:
	GeneralPrefPane() : wxStockPreferencesPage(Kind_General) {}
	virtual wxWindow *CreateWindow(wxWindow *parent)
	{
		return new GeneralPrefPanePage(parent);
	}
};

// -----------------------------------------------------------------------------
// ! Implementation of PrefPane Class
// -----------------------------------------------------------------------------

PrefPane::PrefPane()
{
	this->pref_window = new wxPreferencesEditor();
	this->setupGeneral();
}

PrefPane::~PrefPane()
{
	delete this->pref_window;
}

void PrefPane::Show(wxWindow *parent)
{
	this->pref_window->Show(parent);
}

void PrefPane::setupGeneral()
{
	//this->pref_window.reset(new wxPreferencesEditor);
	this->pref_window->AddPage(new GeneralPrefPane());
}



