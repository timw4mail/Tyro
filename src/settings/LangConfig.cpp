/**
 * Lexer configuration object
 * 
 * @extends TyroConfig
 */

#include "LangConfig.h"
#include <config/languages_json.h>

LangConfig::LangConfig()
{
	this->LoadJson(languages_json);
	this->lang = "";
}

LangConfig::~LangConfig() {}

/**
 * Determine the format of the current file by
 * matching its extension against the patterns
 * in the configuration files
 *
 * @return string
 */
string LangConfig::GetLangByFile(wxFileName &fileName)
{
	JsonValue langList = this->GetRoot();
	JsonValue::iterator it;

	wxString curr_file = fileName.GetFullName();

	// Loop through each language to find a matching file pattern
	for (it = langList.begin(); it != langList.end(); ++it)
	{
		string lang = it.key().asString();

		// Parse the file pattern
		wxString file_pattern((*it)["file_pattern"].asString());

		file_pattern.Lower();

		while ( ! file_pattern.empty())
		{
			wxString cur = file_pattern.BeforeFirst(';');
			if (
				(cur == curr_file) ||
				(cur == (curr_file.BeforeLast('.') + ".*")) ||
				(cur == ("*." + curr_file.AfterLast('.')))
			)
			{
				this->SetLang(lang);
				return this->lang;
			}
			
			// Go to the next pattern for this language
			file_pattern = file_pattern.AfterFirst(';');
		}
	}

	this->SetLang("");
	return this->lang;
}

/**
 * Get the list of keywords for the selected language
 * 
 * @param string lang
 * @return JsonValue
 */
JsonValue LangConfig::GetKeywordList(string lang)
{
	if (lang == "none") lang = this->lang;
	
	return this->GetRoot()
		.get(lang, JsonValue())
		.get("keywords", JsonValue());
}

/**
 * Get the lexer theme map for the current language
 * 
 * @param string lang
 * @return JsonValue
 */
JsonValue LangConfig::GetLexerMap(string lang)
{
	if (lang == "none") lang = this->lang;
	
	return this->GetRoot()
		.get(lang, JsonValue())
		.get("lexer_map", JsonValue());
}

void LangConfig::SetLang(string lang)
{
	this->lang = lang;
}

string LangConfig::GetLang()
{
	return this->lang;
}