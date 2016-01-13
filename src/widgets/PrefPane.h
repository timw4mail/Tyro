/**
 * Preference Panes
 */
#pragma once

#include "src/widgets/widget.h"

class PrefPane {
public:
	PrefPane();
	~PrefPane();
	void Show(wxWindow *parent);
	
protected:
	wxPreferencesEditor *pref_window = nullptr;
};

