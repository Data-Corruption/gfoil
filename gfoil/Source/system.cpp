#include "system.hpp"

#include <string>
#include <vector>

#include <mutex>
#include <sstream>
#include <fstream>

#include <filesystem>
#include <iostream>
#include <chrono>
#include <ctime>

#include "Windows.h"
#include "windows.h"
#include "Win64/CDialogEventHandler.hpp"
#include "convert.hpp"
#include "string_addons.hpp"

std::string gfoil::system::log::log_path = "log.txt";
gfoil::system::log::levels gfoil::system::log::current_logging_level = gfoil::system::log::levels::INFO;

static std::mutex local_time_mutex;

std::string gfoil::system::get_time() {
	std::string result = "%F,%T";
	std::time_t time = std::time(0);
	std::tm bt{};

	std::lock_guard<std::mutex> lock(local_time_mutex);
	localtime_s(&bt, &time);

	char buf[64];
	return { buf, std::strftime(buf, sizeof(buf), result.c_str(), &bt) };
}

// ----==== Error Throwing ====----

void gfoil::system::throw_error(const std::string& e) {
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	std::cerr << gfoil::system::get_time() << ",ERROR: " << e << std::endl << "Press any key to exit: ";
	std::string i;
	getline(std::cin, i);
	exit(EXIT_FAILURE);
}
void gfoil::system::throw_error(const std::exception& e) {
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	std::cerr << gfoil::system::get_time() << ",ERROR: " << e.what() << "\n\n" << "Press any key to exit: ";
	std::string i;
	getline(std::cin, i);
	exit(EXIT_FAILURE);
}
void gfoil::system::throw_error(const std::ifstream::failure& e) {
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	std::cerr << gfoil::system::get_time() << ",ERROR: " << e.what() << "\n\n" << e.code() << "\n\n" << "Press any key to exit: ";
	std::string i;
	getline(std::cin, i);
	exit(EXIT_FAILURE);
}

// ----==== Files ====----

bool gfoil::system::files::exists(const std::string& path) {
	return std::filesystem::exists(path);
}
void gfoil::system::files::write(const std::string& data, const std::string& path) {
	std::ofstream file(path, std::ios::binary);
	file << data;
	gfoil::system::log::info("Write file " + path + " size: " + std::to_string(data.size()));
}
void gfoil::system::files::append(const std::string& data, const std::string& path) {
	std::ofstream file;
	file.open(path, std::ios_base::app);
	file << data;
}
void gfoil::system::files::read(std::string& data, const std::string& path) {
	if (!exists(path))
		gfoil::system::throw_error("Attempting to read file that does not exist: " + path);
	try {
		std::stringstream stream;
		std::ifstream file(path, std::ios::binary);
		stream << file.rdbuf();
		file.close();
		data = stream.str();
		gfoil::system::log::info("Read file " + path + " size: " + std::to_string(data.size()));
	}
	catch (std::ifstream::failure e) {
		gfoil::system::throw_error(e);
	}
}
void gfoil::system::files::list_folder_contents(std::vector<std::string>& files, std::vector<std::string>& folders, std::string path) {

	files.clear();
	folders.clear();

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			files.emplace_back(entry.path().string());
		} else if (entry.is_directory()) {
			folders.emplace_back(entry.path().string());
		}
	}
}
std::string gfoil::system::files::last_write_time(std::string path) {
	auto time_fs = std::filesystem::last_write_time(path);
	auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(time_fs - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
	auto time = std::chrono::system_clock::to_time_t(sctp);
	
	std::tm bt{};

	std::lock_guard<std::mutex> lock(local_time_mutex);
	localtime_s(&bt, &time);

	char buf[64];
	// yy-mm-dd,hh:mm:ss
	std::string time_string = { buf, std::strftime(buf, sizeof(buf), "%F,%T", &bt) };

	std::vector<std::string> staring_point, date_parts, time_parts;
	string_addons::split(staring_point, {','}, time_string);
	string_addons::split(date_parts, { '-' }, staring_point[0]);
	string_addons::split(time_parts, { ':' }, staring_point[1]);

	std::string am_pm = "am";
	int hour = std::stoi(time_parts[0]);
	if (hour > 12) {
		hour -= 12;
		am_pm = "pm";
	}

	return date_parts[1] + "/" + date_parts[2] + "/" + date_parts[0] + " " + std::to_string(hour) + ":" + time_parts[1] + ":" + time_parts[2] + " " + am_pm;
}
void gfoil::system::files::open_external(const std::string& path) {
	std::wstring wpath = convert::to_wstring(path);
	PCWSTR file = wpath.c_str();
	PCIDLIST_ABSOLUTE pidl = ILCreateFromPathW(file);
	if (pidl) {
		SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
	}
}

// ----==== Logging ====----

void gfoil::system::log::set_level(levels level) { current_logging_level = level; }
void gfoil::system::log::info(const std::string& msg) { if (current_logging_level <= levels::INFO) { gfoil::system::files::append(gfoil::system::get_time() + ",INFO: " + msg + "\n", log_path); } }
void gfoil::system::log::warn(const std::string& msg) { if (current_logging_level <= levels::WARN) { gfoil::system::files::append(gfoil::system::get_time() + ",WARN: " + msg + "\n", log_path); } }
void gfoil::system::log::error(const std::string& msg) {
	if (current_logging_level <= levels::ERR) {
		gfoil::system::files::append(gfoil::system::get_time() + ",ERROR: " + msg + "\n", log_path);
		gfoil::system::throw_error("See log file for error");
	}
}

// ----==== OS Dialog ====----

bool gfoil::system::dialog::open(std::string& path, gfoil::system::dialog::types type) {
	// create dialog object
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	IFileDialog* pfd = NULL;
	HRESULT hr = 1;
	if ((type == gfoil::system::dialog::types::OPEN_FOLDER) || (type == gfoil::system::dialog::types::OPEN_FILE)) {
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	}
	else if (type == gfoil::system::dialog::types::SAVE_AS) {
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	}
	if (!SUCCEEDED(hr))
		gfoil::system::log::error("Failed to create dialog object, type: " + std::to_string(int(type)));

	// create event handling object
	IFileDialogEvents* pfde = NULL;
	hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
	if (!SUCCEEDED(hr))
		gfoil::system::log::error("Failed to create event handling object for dialog object, type: " + std::to_string(int(type)));

	// connect the event handler.
	DWORD dwCookie;
	hr = pfd->Advise(pfde, &dwCookie);
	if (!SUCCEEDED(hr))
		gfoil::system::log::error("Failed to connect event handler to dialog object, type: " + std::to_string(int(type)));

	// Set the options on the dialog.
	DWORD dwFlags;
	hr = pfd->GetOptions(&dwFlags);
	if (!SUCCEEDED(hr))
		gfoil::system::log::error("Failed to get options for dialog object, type: " + std::to_string(int(type)));
	if ((type == gfoil::system::dialog::types::OPEN_FILE) || (type == gfoil::system::dialog::types::SAVE_AS)) {
		hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
	}
	else if (type == gfoil::system::dialog::types::OPEN_FOLDER) {
		hr = pfd->SetOptions(dwFlags | FOS_PICKFOLDERS);
	}
	if (!SUCCEEDED(hr))
		gfoil::system::log::error("Failed to set options for dialog object, type: " + std::to_string(int(type)));

	if ((type == gfoil::system::dialog::types::OPEN_FILE) || (type == gfoil::system::dialog::types::SAVE_AS)) {
		// set valid file types
		LPCWSTR valid_text_files = L"Valid Text File Types";
		COMDLG_FILTERSPEC rgSpec[] = {
			{ L"All Files",  L"*.*" },
		};
		hr = pfd->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);
		if (!SUCCEEDED(hr))
			gfoil::system::log::error("Failed to set valid file types for dialog object, type: " + std::to_string(int(type)));
		hr = pfd->SetFileTypeIndex(0);
		if (!SUCCEEDED(hr))
			gfoil::system::log::error("Failed to set index to valid file types for dialog object, type: " + std::to_string(int(type)));

		// set default extension 
		hr = pfd->SetDefaultExtension(L"sse");
		if (!SUCCEEDED(hr))
			gfoil::system::log::error("Failed to set default extension for dialog object, type: " + std::to_string(int(type)));
	}

	// show the dialog
	hr = pfd->Show(NULL);
	if (!SUCCEEDED(hr)) {
		// failed / canceled
		pfd->Unadvise(dwCookie);
		pfde->Release();
		pfd->Release();
		return false;
	}

	// get result
	IShellItem* psiResult;
	hr = pfd->GetResult(&psiResult);
	if (!SUCCEEDED(hr)) {
		gfoil::system::log::warn("Failed to retreive results from open dialog object");
		return false;
	}
	PWSTR pszFilePath = NULL;
	hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
	if (!SUCCEEDED(hr)) {
		gfoil::system::log::warn("Failed to get display name from results from open dialog object");
		return false;
	}
	psiResult->Release();

	pfd->Unadvise(dwCookie);
	pfde->Release();
	pfd->Release();

	if (pszFilePath == 0)
		return false;

	path = convert::utf16_to_utf8(std::wstring(pszFilePath));
	return true;
}