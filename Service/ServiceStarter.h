#pragma once
#include "../HandlerRequest.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <filesystem>



#define SERVICE_NAME  "HTTP_MESSENGER_SERVER"   


namespace fs = std::filesystem;

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

