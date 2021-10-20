#include "Service/ServiceStarter.h"
#include "Service/ServiceController.h"


int main(int argc, char* argv[])
{
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
            logFile("Installing");
        }
        else if(!strcmp(UserCommand, INSTALL_COMMAND))
        {  
            ServiceController::InstallService();
        }
        else if (!strcmp(UserCommand, UNINSTALL_COMMAND))
        {  
            ServiceController::UninstallService();
        }
        else if (!strcmp(UserCommand, START_COMMAND))
        {
            ServiceController::StartService_();
        }
        else if (!strcmp(UserCommand, STOP_COMMAND))
        {
            ServiceController::StopService();
        }
        else
        {
            //log bad command
        }
    }
    else
    {
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