/**
 * Preference Panes
 */
#pragma once

#include "src/widgets/widget.h"

class PrefFrame {
public:
	PrefFrame();
	~PrefFrame();
	void Show(wxWindow *parent);
	
protected:
	wxPreferencesEditor *pref_window = nullptr;
};

