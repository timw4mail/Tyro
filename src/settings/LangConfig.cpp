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
	
	// "cache" reverse map of languages to their keys
	JsonValue langList = this->GetRoot();
	JsonValue::iterator it;
	
	for (it = langList.begin(); it != langList.end(); ++it)
	{
		JsonValue langObj = *it;
		reverseMap[langObj.get("name", JsonValue()).asString()] = it.key().asString();
	}
}

LangConfig::~LangConfig()
{
	wxLogDebug("Called LangConfig Destructor");
}

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

/**
 * Get the current language key
 */
string LangConfig::GetLang()
{
	return this->lang;
}

/**
 * Get the name attribute of the currently selected language
 * 
 * @return string
 */
string LangConfig::GetCurrentLangName()
{
	return this->GetRoot()
		.get(this->lang, JsonValue())
		.get("name", JsonValue())
		.asString();
}

/**
 * Gets the list of languages available
 * 
 * @return StringMap
 */
StringMap LangConfig::GetLangList()
{
	StringMap revList = this->reverseMap;
	StringMap::iterator it;

	StringMap outputList;

	for (it = revList.begin(); it != revList.end(); ++it)
	{
		outputList[it->second] = it->first;
	}

	return outputList;
}