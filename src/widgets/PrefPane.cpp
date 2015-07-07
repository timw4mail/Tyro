#include "src/widgets/PrefPane.h"
#include "src/widgets/MainFrame.h"

extern wxConfigBase *Glob_config;

#ifndef TRAVIS
class GeneralPrefPanePage : public wxPanel {
public:
	GeneralPrefPanePage(wxWindow *parent)
	: wxPanel(parent)
	{
		this->frame = (MainFrame *) parent;
		
		this->showLineNumbers = new wxCheckBox(this, myID_PREFS_LINE_NUMBERS, "Show line numbers");
		this->showIndentGuides = new wxCheckBox(this, myID_PREFS_IDENT_GUIDES, "Show indent guides");
		this->showCodeFolding = new wxCheckBox(this, myID_PREFS_CODE_FOLDING, "Show code folding");
		
		wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		
		// Check that version of OS X is less than 10.10 for wxWidgets < 3.0.3
		// Otherwise the font control will segfault
		if( ! HAS_FONT_BUG())
		{
			this->fontPicker = new wxFontPickerCtrl(
				this, 
				myID_PREFS_FONT,
				wxNullFont,
				wxDefaultPosition,
				wxDefaultSize,
				wxFNTP_USE_TEXTCTRL ^ wxFNTP_USEFONT_FOR_LABEL
			);
			wxSizer *fontSizer = new wxBoxSizer(wxHORIZONTAL);
			fontSizer->Add(this->fontPicker, wxSizerFlags().Border());
			sizer->Add(fontSizer, wxSizerFlags().Border());
		}
		
		sizer->Add(this->showLineNumbers, wxSizerFlags().Border());
		sizer->Add(this->showIndentGuides, wxSizerFlags().Border());
		sizer->Add(this->showCodeFolding, wxSizerFlags().Border());
		
		this->SetSizerAndFit(sizer);
		
		// Change settings on selection, rather than on apply button
		// On supported platforms
		if (wxPreferencesEditor::ShouldApplyChangesImmediately())
		{
			if ( ! HAS_FONT_BUG())
			{
				this->fontPicker->Bind(wxEVT_FONTPICKER_CHANGED, [=] (wxFontPickerEvent &event) {
					Glob_config->Write("global_font", event.GetFont());
					this->frame->OnPrefsChanged(event);
					Glob_config->Flush();
				}, myID_PREFS_FONT);
			}
			
			this->showLineNumbers->Bind(wxEVT_CHECKBOX, [=] (wxCommandEvent &event) {
				Glob_config->Write("show_line_numbers", event.IsChecked());
				this->frame->OnPrefsChanged(event);
				Glob_config->Flush();
			}, myID_PREFS_LINE_NUMBERS);
			
			this->showIndentGuides->Bind(wxEVT_CHECKBOX, [=] (wxCommandEvent &event) {
				Glob_config->Write("show_indent_guides", event.IsChecked());
				this->frame->OnPrefsChanged(event);
				Glob_config->Flush();
			}, myID_PREFS_IDENT_GUIDES);
			
			this->showCodeFolding->Bind(wxEVT_CHECKBOX, [=] (wxCommandEvent &event) {
				Glob_config->Write("show_code_folding", event.IsChecked());
				this->frame->OnPrefsChanged(event);
				Glob_config->Flush();
			}, myID_PREFS_CODE_FOLDING);
		}
	}
	
	~GeneralPrefPanePage()
	{
	}
	
	/**
	 * Apply current settings to the pref window
	 * 
     * @return bool
     */
	virtual bool TransferDataToWindow()
	{
		this->showLineNumbers->SetValue(Glob_config->ReadBool("show_line_numbers", true));
		this->showIndentGuides->SetValue(Glob_config->ReadBool("show_indent_guides", false));
		this->showCodeFolding->SetValue(Glob_config->ReadBool("show_code_folding", false));
		
		if ( ! HAS_FONT_BUG())
		{
			wxFont globalFont;
			Glob_config->Read("global_font", &globalFont);
			
			this->fontPicker->SetSelectedFont(globalFont);
		}
		
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
		Glob_config->Write("show_line_numbers", this->showLineNumbers->IsChecked());
		Glob_config->Write("show_indent_guides", this->showIndentGuides->IsChecked());
		Glob_config->Write("show_code_folding", this->showCodeFolding->IsChecked());
		
		if ( ! HAS_FONT_BUG())
		{
			Glob_config->Write("global_font", this->fontPicker->GetSelectedFont());
		}
		
		wxCommandEvent evt = wxCommandEvent();
		this->frame->OnPrefsChanged(evt);
		
		Glob_config->Flush();
		
		return true;
	}
	
private:
	MainFrame *frame;
	wxFontPickerCtrl *fontPicker = nullptr;
	wxCheckBox *showLineNumbers = nullptr;
	wxCheckBox *showIndentGuides = nullptr;
	wxCheckBox *showCodeFolding = nullptr;
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
	//delete this->pref_window;
}

void PrefPane::Show(wxWindow *parent)
{
	this->pref_window->Show(parent);
}

#endif // ifndef TRAVIS

