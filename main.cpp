#include "Service/ServiceStarter.h"
#include "Service/ServiceController.h"

void InitLogger();

#include <cstdlib>
int main(int argc, char* argv[])
{
    InitLogger();
    if (argc > 1)
    {
        ServiceController::set_ExecutableName(argv[0]);
        ServiceController::set_ServiceName("HTTP_MESSENGER_SERVER");
        
        char* UserCommand = argv[1];
        if (!strcmp(UserCommand, NO_SERVICE_COMMAND))
        {
            HandlerRequest h;
            bool ServerWorking = 1;
            h.AddQueueThread(ServerWorking);
            LOG_DEBUG("Running without Service");
        }
        else if(!strcmp(UserCommand, INSTALL_COMMAND))
        {  
            if (ServiceController::InstallService())
            {
                std::cout << "Service was installed successfully!\n";
                LOG_DEBUG("Service Installed!");
            }
            else
            {
                std::cout << "Service was not installed CHECK LOGS!\n";
                LOG_ERROR("Service Was not Installed!");
            }
        }
        else if (!strcmp(UserCommand, UNINSTALL_COMMAND))
        {  
            if (ServiceController::UninstallService())
            {
                std::cout << "Service was Uninstalled successfully!\n";
                LOG_DEBUG("Service Uninstalled!");
            }
            else
            {
                std::cout << "Service was not Uninstalled CHECK LOGS!\n";
                LOG_DEBUG("Service Uninstalled!");
            }
        }
        else if (!strcmp(UserCommand, START_COMMAND))
        {
            if (ServiceController::StartService_())
            {
                std::cout << "Service was Started successfully!\n";
                LOG_DEBUG("Service Started!");
            }
            else
            {
                std::cout << "Service was not Started CHECK LOGS!\n";
                LOG_DEBUG("Service Started!");
            }
        }
        else if (!strcmp(UserCommand, STOP_COMMAND))
        {
            if (ServiceController::StopService())
            {
                std::cout << "Service was stopped successfully!\n";
                LOG_DEBUG("Service stopped!");
            }
            else
            {
                std::cout << "Service was not stopped CHECK LOGS!\n";
                LOG_DEBUG("Service Stoped!");
            }
        }
        else if (!strcmp(UserCommand, HELP_COMMAND_))
        {
            ServiceController::Help();
        }
        else
        {
            ServiceController::Help();
			LOG_FATAL("Bad Input command!" + std::string(UserCommand));
        }
    }
    else
    {
        LOG_DEBUG("Starting Service");
        SERVICE_TABLE_ENTRY ServiceTable[] =
        {
            {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceStarter::ServiceMain},
            {NULL, NULL}
        };

        if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
        {
            return GetLastError();
        }
    }
}

void InitLogger()
{
    std::string ProgrammDataPath(std::getenv("PROGRAMDATA"));
    std::string FolderName("\\HTTP_Messenger_Server");
    CreateDirectory((ProgrammDataPath + FolderName).c_str(), NULL);
    SET_LOGS_FILE(std::string(ProgrammDataPath) + FolderName + "\\Logs.txt");
	SHOW_LOGS_IN_CONSOLE(true);
}
