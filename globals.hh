#pragma once
#include <unordered_map>

class c_globals {
public:
	bool active = true;
	char user_name[255] = "";
	char pass_word[255] = "";
	char user_key[255] = "";
	bool checkbox = false;
	bool show_new_ui = false;
	bool done_logging_in = false;
	bool logging_in = false;
	const char* combo = "One\0Two\0Three\0Four";
	int cur_selec = 0;
	std::string info_text = "Ready";
	bool sucessful_login = false;
	bool needs_to_register = false;
	bool done_registering = false;
	bool done_injecting = false;
	bool is_injecting = false;
	bool auto_login = false;
	std::string autolog_username;
	std::string autolog_password;
	bool needs_to_upgrade = false;
	bool done_upgrading = false;
	bool user_is_alpha = false;

};

inline c_globals globals;