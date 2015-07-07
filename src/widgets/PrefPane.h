/**
 * Preference Panes
 */

#ifndef TYRO_PREF_PANE_H
#define	TYRO_PREF_PANE_H

#include "src/widgets/widget.h"

class PrefPane {
public:
	PrefPane();
	~PrefPane();
	void Show(wxWindow *parent);
	
protected:
	wxPreferencesEditor *pref_window = nullptr;
};

#endif	/* TYRO_PREF_PANE_H */

