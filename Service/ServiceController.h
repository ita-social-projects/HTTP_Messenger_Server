#pragma once
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

constexpr const char* INSTALL_COMMAND = "Install";
constexpr const char* UNINSTALL_COMMAND = "Uninstall";
constexpr const char* START_COMMAND = "Start";
constexpr const char* STOP_COMMAND = "Stop";
constexpr const char* NO_SERVICE_COMMAND = "NoService";



class ServiceController
{
public:
	ServiceController() = delete;
	~ServiceController() = default;
private:
	inline static std::string m_ExePath;
	inline static bool m_OpenSCManager(SC_HANDLE& hSCManager);
	inline static LPCSTR get_LPCSTR_ServiceName();

public:
	inline static std::string m_ExeName;
	inline static std::string m_ServiceName;
	inline static SC_HANDLE m_hSCManager;
	inline static SC_HANDLE m_hService;

	static void set_ExecutableName(const char*);
	static void set_ServiceName(std::string);
	static bool InstallService();
	static bool UninstallService();
	static bool StartService_();
	static bool StopService();
};


