#include "src/widgets/PrefPane.h"
#include "src/widgets/MainFrame.h"

extern wxConfigBase *Glob_config;

// As much as I dislike this global reference
// it seems to be the safest option for calling
// a method from the Main frame. Even attempting
// to get the Main frame from the GetParent() method
// is more prone to segfaulting.
extern MainFrame *Glob_main_frame;

class GeneralPrefPanePage : public wxPanel {
public:
	explicit GeneralPrefPanePage(
		wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr
	) : wxPanel(parent, winid, pos, size, style, name)
	{
		auto BASE_MARGIN = 30;

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

		this->showLineNumbers = new wxCheckBox(this, myID_PREFS_LINE_NUMBERS, "Show line numbers");
		this->showIndentGuides = new wxCheckBox(this, myID_PREFS_IDENT_GUIDES, "Show indent guides");
		this->showCodeFolding = new wxCheckBox(this, myID_PREFS_CODE_FOLDING, "Show code folding");

		wxSizer *hSizer = new wxBoxSizer(wxHORIZONTAL);
		hSizer->AddSpacer(BASE_MARGIN);

		wxSizer *vSizer = new wxBoxSizer(wxVERTICAL);
		vSizer->AddSpacer(BASE_MARGIN);
		vSizer->Add(this->fontPicker, wxSizerFlags().Border());
		vSizer->AddSpacer(10);
		vSizer->Add(this->showLineNumbers, wxSizerFlags().Border());
		vSizer->Add(this->showIndentGuides, wxSizerFlags().Border());
		vSizer->Add(this->showCodeFolding, wxSizerFlags().Border());
		vSizer->AddSpacer(BASE_MARGIN);

		hSizer->Add(vSizer);
		hSizer->AddSpacer(BASE_MARGIN);

		this->SetSizerAndFit(hSizer);

		// Change settings on selection, rather than on apply button
		// On supported platforms
		if (wxPreferencesEditor::ShouldApplyChangesImmediately())
		{
			this->showLineNumbers->Bind(wxEVT_CHECKBOX, [=] (wxCommandEvent &event) {
				Glob_config->Write("show_line_numbers", event.IsChecked());
				Glob_main_frame->OnPrefsChanged(event);
				Glob_config->Flush();
			}, myID_PREFS_LINE_NUMBERS);

			this->showIndentGuides->Bind(wxEVT_CHECKBOX, [=] (wxCommandEvent &event) {
				Glob_config->Write("show_indent_guides", event.IsChecked());
				Glob_main_frame->OnPrefsChanged(event);
				Glob_config->Flush();
			}, myID_PREFS_IDENT_GUIDES);

			this->showCodeFolding->Bind(wxEVT_CHECKBOX, [=] (wxCommandEvent &event) {
				Glob_config->Write("show_code_folding", event.IsChecked());
				Glob_main_frame->OnPrefsChanged(event);
				Glob_config->Flush();
			}, myID_PREFS_CODE_FOLDING);
		}
	}

	~GeneralPrefPanePage() override
	{
		wxLogDebug("GeneralPrefPanePage Destructor Called.");
		wxDELETE(this->showLineNumbers);
		wxDELETE(this->showIndentGuides);
		wxDELETE(this->showCodeFolding);
		wxDELETE(this->fontPicker);
	}

	/**
	 * Apply current settings to the pref window
	 *
	 * @return bool
	 */
	bool TransferDataToWindow() override
	{
		this->showLineNumbers->SetValue(Glob_config->ReadBool("show_line_numbers", true));
		this->showIndentGuides->SetValue(Glob_config->ReadBool("show_indent_guides", false));
		this->showCodeFolding->SetValue(Glob_config->ReadBool("show_code_folding", false));

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
	bool TransferDataFromWindow() override
	{
		Glob_config->Write("show_line_numbers", this->showLineNumbers->IsChecked());
		Glob_config->Write("show_indent_guides", this->showIndentGuides->IsChecked());
		Glob_config->Write("show_code_folding", this->showCodeFolding->IsChecked());
		Glob_config->Write("global_font", this->fontPicker->GetSelectedFont());

		auto evt = wxCommandEvent();
		Glob_main_frame->OnPrefsChanged(evt);

		Glob_config->Flush();

		return true;
	}

private:
	wxCheckBox *showLineNumbers = nullptr;
	wxCheckBox *showIndentGuides = nullptr;
	wxCheckBox *showCodeFolding = nullptr;
	wxFontPickerCtrl *fontPicker = nullptr;
};

/*
 * Creates the "General" pane on the pref window
 * */
class GeneralPrefPane: public wxStockPreferencesPage {
public:
	GeneralPrefPane() : wxStockPreferencesPage(Kind_General) {}
	wxWindow *CreateWindow(wxWindow *parent) override
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
	wxLogDebug("PrefPane Destructor Called.");
	delete this->pref_window;
}

void PrefPane::Show(wxWindow *parent)
{
	this->pref_window->Show(parent);
}
