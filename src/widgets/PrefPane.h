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
private:
	void setupGeneral();
};

#endif	/* TYRO_PREF_PANE_H */

