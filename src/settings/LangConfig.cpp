#include "LangConfig.h"
#include <config/languages_json.h>

/**
 * Constructor
 */ 
LangConfig::LangConfig()
{
	this->LoadJson(languages_json);
	this->language = "";
	
	// "cache" reverse map of languages to their keys
	Json::Value langList = this->GetRoot();
	Json::Value::iterator it;
	
	// Special case for non-recognized language
	reverseMap["Plain Text"] = "";
	
	for (it = langList.begin(); it != langList.end(); ++it)
	{
		Json::Value langObj = *it;
		reverseMap[langObj.get("name", Json::Value()).asString()] = it.key().asString();
	}
}

/**
 * Destructor
 */ 
LangConfig::~LangConfig()
{
	wxLogDebug("LangConfig Destructor Called.");
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
	Json::Value langList = this->GetRoot();
	Json::Value::iterator it;

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
				return this->language;
			}
			
			// Go to the next pattern for this language
			file_pattern = file_pattern.AfterFirst(';');
		}
	}

	this->SetLang("");
	return this->language;
}

/**
 * Get the list of keywords for the selected language
 * 
 * @param string lang
 * @return Json::Value
 */
Json::Value LangConfig::GetKeywordList(string lang)
{
	if (lang == "none") lang = this->language;
	
	return this->GetRoot()
		.get(lang, Json::Value())
		.get("keywords", Json::Value());
}

/**
 * Get the lexer theme map for the current language
 * 
 * @param string lang
 * @return Json::Value
 */
Json::Value LangConfig::GetLexerMap(string lang)
{
	if (lang == "none") lang = this->language;
	
	return this->GetRoot()
		.get(lang, Json::Value())
		.get("lexer_map", Json::Value());
}

/**
 * Set the selected language key
 * 
 * @param string lang
 * @return void
 */ 
void LangConfig::SetLang(const string &lang)
{
	this->language = lang;
}

/**
 * Get the current language key
 */
string LangConfig::GetLang()
{
	return this->language;
}

/**
 * Get the name attribute of the currently selected language
 * 
 * @return string
 */
string LangConfig::GetCurrentLangName()
{
	return this->GetRoot()
		.get(this->language, Json::Value())
		.get("name", Json::Value())
		.asString();
}

/**
 * Get the "key" of the language based on its name attribute
 * 
 * @param string name
 * @return string
 */
string LangConfig::GetLangByName(const string &name)
{
	int count = this->reverseMap.count(name);
	return (count > 0) ? this->reverseMap[name] : "";
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
