#include "widget.h"

extern wxConfigBase *Glob_config;

class GeneralPrefPanePage : public wxPanel {
public:
	GeneralPrefPanePage(wxWindow *parent)
	: wxPanel(parent)
	{
		this->frame = (MainFrame *) parent;
		
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
			showLineNumbers->Bind(wxEVT_CHECKBOX, &GeneralPrefPanePage::ToggleShowLineNumbers, this, myID_PREFS_LINE_NUMBERS);
			showIndentGuides->Bind(wxEVT_CHECKBOX, &GeneralPrefPanePage::ToggleShowIndentGuides, this, myID_PREFS_IDENT_GUIDES);
			showCodeFolding->Bind(wxEVT_CHECKBOX, &GeneralPrefPanePage::ToggleShowCodeFolding, this, myID_PREFS_CODE_FOLDING);
		}
	}
	
	~GeneralPrefPanePage()
	{
		delete showLineNumbers;
		delete showIndentGuides;
		delete showCodeFolding;
	}
	
	/**
	 * Apply current settings to the pref window
	 * 
     * @return bool
     */
	virtual bool TransferDataToWindow()
	{
		showLineNumbers->SetValue(Glob_config->ReadBool("show_line_numbers", true));
		showIndentGuides->SetValue(Glob_config->ReadBool("show_indent_guides", false));
		showCodeFolding->SetValue(Glob_config->ReadBool("show_code_folding", false));
		
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
		Glob_config->Write("show_line_numbers", showLineNumbers->IsChecked());
		Glob_config->Write("show_indent_guides", showIndentGuides->IsChecked());
		Glob_config->Write("show_code_folding", showCodeFolding->IsChecked());
		
		wxCommandEvent evt = wxCommandEvent();
		this->frame->OnPrefsChanged(evt);
		
		Glob_config->Flush();
		
		return true;
	}
	
private:
	MainFrame *frame;
	wxCheckBox *showLineNumbers;
	wxCheckBox *showIndentGuides;
	wxCheckBox *showCodeFolding;
	
	void ToggleShowLineNumbers(wxCommandEvent &event)
	{
		
		Glob_config->Write("show_line_numbers", event.IsChecked());
		this->frame->OnPrefsChanged(event);
		Glob_config->Flush();
	}
	
	void ToggleShowIndentGuides(wxCommandEvent &event)
	{
		Glob_config->Write("show_indent_guides", event.IsChecked());
		this->frame->OnPrefsChanged(event);
		Glob_config->Flush();
	}
	
	void ToggleShowCodeFolding(wxCommandEvent &event)
	{
		Glob_config->Write("show_code_folding", event.IsChecked());
		this->frame->OnPrefsChanged(event);
		Glob_config->Flush();
	}
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
	this->pref_window->AddPage(new GeneralPrefPane());
}

PrefPane::~PrefPane()
{
	delete this->pref_window;
}

void PrefPane::Show(wxWindow *parent)
{
	this->pref_window->Show(parent);
}




