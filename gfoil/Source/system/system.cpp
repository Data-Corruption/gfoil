#include "System.h"

#include <string>
#include <vector>

#include <mutex>
#include <sstream>
#include <fstream>

#include <filesystem>
#include <iostream>
#include <ctime>

#include <windows.h>

#include "Win64/CDialogEventHandler.h"

#include "../convert.h"

std::string system::log::log_path;
system::log::levels system::log::current_logging_level;

std::string system::get_time() {
	std::string result = "%F,%T";
	std::time_t time = std::time(0);
	std::tm bt{};

#ifdef _MSC_VER
	localtime_s(&bt, &time);
#else
	static std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
	bt = *std::localtime(&time);
#endif

	char buf[64];
	return { buf, std::strftime(buf, sizeof(buf), result.c_str(), &bt) };
}

// ----==== Error Throwing ====----

void system::throw_error(const std::string& e) {
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	std::cerr << system::get_time() << ",ERROR: " << e << std::endl << "Press any key to exit: ";
	std::string i;
	getline(std::cin, i);
	exit(EXIT_FAILURE);
}
void system::throw_error(const std::exception& e) {
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	std::cerr << system::get_time() << ",ERROR: " << e.what() << "\n\n" << "Press any key to exit: ";
	std::string i;
	getline(std::cin, i);
	exit(EXIT_FAILURE);
}
void system::throw_error(const std::ifstream::failure& e) {
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	std::cerr << system::get_time() << ",ERROR: " << e.what() << "\n\n" << e.code() << "\n\n" << "Press any key to exit: ";
	std::string i;
	getline(std::cin, i);
	exit(EXIT_FAILURE);
}

// ----==== Files ====----

void system::files::write(const std::string& data, const std::string& path) {
	std::ofstream file;
	file.open(path);
	file << data;
}

void system::files::append(const std::string& data, const std::string& path) {
	std::ofstream file;
	file.open(path, std::ios_base::app);
	file << data;
}

std::string system::files::read(const std::string& path) {
	std::string result = "";
	read(result, path);
	return result;
}

void system::files::read(std::string& data, const std::string& path) {
	if (!exists(path))
		system::throw_error("Attempting to read file that does not exist: " + path);
	try {
		std::stringstream stream;
		std::ifstream file;
		file.open(path);
		stream << file.rdbuf();
		file.close();
		data = stream.str();
	}
	catch (std::ifstream::failure e) {
		system::throw_error(e);
	}
}

void system::files::list_contents(std::vector<std::string>& files, std::vector<std::string>& folders, std::string path) {

	files.clear();
	files.shrink_to_fit();

	folders.clear();
	folders.shrink_to_fit();

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			files.emplace_back(entry.path().string());
		} else if (entry.is_directory()) {
			folders.emplace_back(entry.path().string());
		}
	}
}

bool system::files::exists(const std::string& path) {
	return std::filesystem::exists(path);
}

// ----==== Logging ====----

void system::log::set_level(levels level) {
	current_logging_level = level;
}

void system::log::info(const std::string& msg) {
	if (current_logging_level <= levels::INFO)
		system::files::append(system::get_time() + ",INFO: " + msg + "\n", log_path);
}

void system::log::warn(const std::string& msg) {
	if (current_logging_level <= levels::WARN)
		system::files::append(system::get_time() + ",WARN: " + msg + "\n", log_path);
}

void system::log::error(const std::string& msg) {
	if (current_logging_level <= levels::I_NEED_AN_ADULT) {
		system::files::append(system::get_time() + ",ERROR: " + msg + "\n", log_path);
		system::throw_error("See log file for error");
	}
}

// ----==== OS Dialog ====----

std::string system::open_dialog(dialog_types type) {
	// create dialog object
	IFileDialog* pfd = NULL;
	HRESULT hr = 1;
	if ((type == system::dialog_types::OPEN_FOLDER) || (type == system::dialog_types::OPEN_FILE)) {
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	}
	else if (type == system::dialog_types::SAVE_AS) {
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	}
	if (!SUCCEEDED(hr))
		system::log::error("Failed to create dialog object, type: " + std::to_string(int(type)));

	// create event handling object
	IFileDialogEvents* pfde = NULL;
	hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
	if (!SUCCEEDED(hr))
		system::log::error("Failed to create event handling object for dialog object, type: " + std::to_string(int(type)));

	// connect the event handler.
	DWORD dwCookie;
	hr = pfd->Advise(pfde, &dwCookie);
	if (!SUCCEEDED(hr))
		system::log::error("Failed to connect event handler to dialog object, type: " + std::to_string(int(type)));

	// Set the options on the dialog.
	DWORD dwFlags;
	hr = pfd->GetOptions(&dwFlags);
	if (!SUCCEEDED(hr))
		system::log::error("Failed to get options for dialog object, type: " + std::to_string(int(type)));
	if ((type == system::dialog_types::OPEN_FILE) || (type == system::dialog_types::SAVE_AS)) {
		hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
	}
	else if (type == system::dialog_types::OPEN_FOLDER) {
		hr = pfd->SetOptions(dwFlags | FOS_PICKFOLDERS);
	}
	if (!SUCCEEDED(hr))
		system::log::error("Failed to set options for dialog object, type: " + std::to_string(int(type)));

	if ((type == system::dialog_types::OPEN_FILE) || (type == system::dialog_types::SAVE_AS)) {
		// set valid file types
		LPCWSTR valid_text_files = L"Valid Text File Types";
		COMDLG_FILTERSPEC rgSpec[] = {
			{ L"All Files",  L"*.*" },
		};
		hr = pfd->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);
		if (!SUCCEEDED(hr))
			system::log::error("Failed to set valid file types for dialog object, type: " + std::to_string(int(type)));
		hr = pfd->SetFileTypeIndex(0);
		if (!SUCCEEDED(hr))
			system::log::error("Failed to set index to valid file types for dialog object, type: " + std::to_string(int(type)));

		// set default extension 
		hr = pfd->SetDefaultExtension(L"sse");
		if (!SUCCEEDED(hr))
			system::log::error("Failed to set default extension for dialog object, type: " + std::to_string(int(type)));
	}

	// show the dialog
	hr = pfd->Show(NULL);
	if (!SUCCEEDED(hr)) {
		// failed / canceled
		pfd->Unadvise(dwCookie);
		pfde->Release();
		pfd->Release();
		return "";
	}

	// get result
	IShellItem* psiResult;
	hr = pfd->GetResult(&psiResult);
	if (!SUCCEEDED(hr))
		system::log::error("Failed to retreive results from open dialog object");
	PWSTR pszFilePath = NULL;
	hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
	if (!SUCCEEDED(hr))
		system::log::error("Failed to get display name from results from open dialog object");
	psiResult->Release();

	pfd->Unadvise(dwCookie);
	pfde->Release();
	pfd->Release();

	if (pszFilePath == 0)
		return "";

	return convert::utf16_to_utf8(std::wstring(pszFilePath));
}