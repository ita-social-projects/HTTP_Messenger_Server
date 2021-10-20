#pragma once
#include "../HandlerRequest.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <filesystem>



#define SERVICE_NAME  "My Sample Service"   

template <class T>
void logFile(T text);

namespace fs = std::filesystem;

#define DFILEPATH "D:\\programming\\Soft\\SoftProject\\WindowsServiceApplication\\out.txt"

class ServiceStarter
{
public:
	ServiceStarter() = delete;
	~ServiceStarter() = default;
private:
	static HANDLE g_ServiceStopEvent;
	static SERVICE_STATUS_HANDLE g_StatusHandle;
	static SERVICE_STATUS        g_ServiceStatus ;
	static bool RunningServer;

public:
	static VOID WINAPI ServiceStarter::ServiceMain(DWORD argc, LPTSTR* argv);
	static VOID WINAPI ServiceStarter::ServiceCtrlHandler(DWORD CtrlCode);
	static DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);
};
extern std::ofstream dFile;

