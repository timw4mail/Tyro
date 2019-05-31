#include "src/widgets/PrefPane.h"
#include "src/widgets/MainFrame.h"

extern wxConfigBase *Glob_config;

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
		sizer->Add(this->showLineNumbers, wxSizerFlags().Border());
		sizer->Add(this->showIndentGuides, wxSizerFlags().Border());
		sizer->Add(this->showCodeFolding, wxSizerFlags().Border());

		this->SetSizerAndFit(sizer);

		// Change settings on selection, rather than on apply button
		// On supported platforms
		if (wxPreferencesEditor::ShouldApplyChangesImmediately())
		{
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
		wxDELETE(this->showLineNumbers);
		wxDELETE(this->showIndentGuides);
		wxDELETE(this->showCodeFolding);
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

		wxCommandEvent evt = wxCommandEvent();
		this->frame->OnPrefsChanged(evt);

		Glob_config->Flush();

		return true;
	}

private:
	MainFrame *frame;
	wxCheckBox *showLineNumbers = nullptr;
	wxCheckBox *showIndentGuides = nullptr;
	wxCheckBox *showCodeFolding = nullptr;
};

class FontPrefPanePage : public wxPanel{
public:
	FontPrefPanePage(wxWindow *parent): wxPanel(parent)
	{
		this->frame = (MainFrame *) parent;

		wxSizer *sizer = new wxBoxSizer(wxVERTICAL);

		wxFont globalFont = wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT);
		Glob_config->Read("global_font", &globalFont);

		this->fontPicker = new wxFontPickerCtrl(
			this,
			myID_PREFS_FONT,
			globalFont,
			wxDefaultPosition,
			wxDefaultSize,
			wxFNTP_FONTDESC_AS_LABEL
		);
		this->fontPicker->SetLabelText("Editor Font");
		wxSizer *fontSizer = new wxBoxSizer(wxHORIZONTAL);
		fontSizer->AddSpacer(50);
		fontSizer->Add(this->fontPicker, wxSizerFlags().Border());
		fontSizer->AddSpacer(50);
		sizer->Add(fontSizer, wxSizerFlags().Border());

		this->SetSizerAndFit(sizer);

		// Change settings on selection, rather than on apply button
		// On supported platforms
		if (wxPreferencesEditor::ShouldApplyChangesImmediately())
		{
			this->fontPicker->Bind(wxEVT_FONTPICKER_CHANGED, [=] (wxFontPickerEvent &event) {
				Glob_config->Write("global_font", event.GetFont());
				this->frame->OnPrefsChanged(event);
				Glob_config->Flush();
			}, myID_PREFS_FONT);
		}
	}

	~FontPrefPanePage()
	{
		wxDELETE(this->fontPicker);
	}

	/**
	 * Apply current settings to the pref window
	 *
	 * @return bool
	 */
	virtual bool TransferDataToWindow()
	{
		wxFont globalFont = wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT);
		Glob_config->Read("global_font", &globalFont);

		this->fontPicker->SetSelectedFont(globalFont);

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
		Glob_config->Write("global_font", this->fontPicker->GetSelectedFont());

		wxCommandEvent evt = wxCommandEvent();
		this->frame->OnPrefsChanged(evt);

		Glob_config->Flush();

		return true;
	}

private:
	MainFrame *frame;
	wxFontPickerCtrl *fontPicker = nullptr;
};

/*
 * Creates the "General" pane on the pref window
 * */
class GeneralPrefPane: public wxStockPreferencesPage {
public:
	GeneralPrefPane() : wxStockPreferencesPage(Kind_General) {}
	virtual wxWindow *CreateWindow(wxWindow *parent)
	{
		return new GeneralPrefPanePage(parent);
	}
};

class FontPrefPane: public wxStockPreferencesPage {
public:
	FontPrefPane() : wxStockPreferencesPage(Kind_Advanced) {}
	virtual wxWindow *CreateWindow(wxWindow *parent)
	{
		return new FontPrefPanePage(parent);
	}
};

// -----------------------------------------------------------------------------
// ! Implementation of PrefPane Class
// -----------------------------------------------------------------------------

PrefPane::PrefPane()
{
	this->pref_window = new wxPreferencesEditor();
	this->pref_window->AddPage(new GeneralPrefPane());
	this->pref_window->AddPage(new FontPrefPane());
}

PrefPane::~PrefPane()
{
	//delete this->pref_window;
}

void PrefPane::Show(wxWindow *parent)
{
	this->pref_window->Show(parent);
}
