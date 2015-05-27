/**
 * Preference Panes
 */

#ifndef TYRO_PREF_PANE_H
#define	TYRO_PREF_PANE_H

class PrefPane {
public:
	PrefPane();
	~PrefPane();
	void Show(wxWindow *parent);
	
protected:
	wxPreferencesEditor *pref_window;
};

#endif	/* TYRO_PREF_PANE_H */

