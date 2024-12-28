/*! \file
    \brief 
 */


// Должна быть первой
#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"
#include "umba/shellapi.h"
#include "umba/program_location.h"
#include "umba/cli_tool_helpers.h"
#include "umba/cmd_line.h"
//

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "umba/debug_helpers.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <exception>
#include <stdexcept>

#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/scanners.h"
#include "umba/relops.h"
#include "umba/debug_helpers.h"
#include "umba/rule_of_five.h"

#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"

#include "encoding/encoding.h"
#include "umba/cli_tool_helpers.h"
#include "umba/time_service.h"
#include "umba/shellapi.h"

//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

bool bOverwrite         = false;

//
#include "log.h"
//
#include "AppConfig.h"

AppConfig appConfig;

#include "ArgParser.h"



int unsafeMain(int argc, char* argv[]);


UMBA_APP_MAIN()
{
    try
    {
        return unsafeMain(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        std::cout << "Unknown error\n";
        return 2;
    }

}



int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    auto argsParser = umba::command_line::makeArgsParser( ArgParser<std::string>()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            //, "" // overrideExeName
                                                            )
                                                        );

    // Force set CLI arguments while running under debugger
    if (umba::isDebuggerPresent())
    {
        // argsParser.args.clear();
        // argsParser.args.push_back("--overwrite");

        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

    } // if (umba::isDebuggerPresent())


    // try
    // {
        // Job completed - may be, --where option found
        if (argsParser.mustExit)
            return 0;

        // if (!argsParser.quet)
        // {
        //     printNameVersion();
        // }

        // LOG_INFO("config") << "-----------------------------------------" << "\n";
        // LOG_INFO("config") << "Processing builtin option files\n";
        if (!argsParser.parseStdBuiltins())
        {
            // LOG_INFO("config") << "Error found in builtin option files\n";
            return 1;
        }
        // LOG_INFO("config") << "-----------------------------------------" << "\n";

        if (argsParser.mustExit)
            return 0;

        // LOG_INFO("config") << "-----------------------------------------" << "\n";
        // LOG_INFO("config") << "Processing command line arguments\n";
        if (!argsParser.parse())
        {
            // LOG_INFO("config") << "Error found while parsing command line arguments\n";
            return 1;
        }
        // LOG_INFO("config") << "-----------------------------------------" << "\n";

        if (argsParser.mustExit)
            return 0;
    // }
    // catch(const std::exception &e)
    // {
    //     LOG_ERR << e.what() << "\n";
    //     return -1;
    // }
    // catch(const std::exception &e)
    // {
    //     LOG_ERR << "command line arguments parsing error" << "\n";
    //     return -1;
    // }

    appConfig.finalizeCommands();


    if (!argsParser.quet  /* && !hasHelpOption */ )
    {
        //printNameVersion();
        //LOG_MSG<<"\n";
        umba::cli_tool_helpers::printNameVersion(umbaLogStreamMsg);
    }


    if (appConfig.commands.empty())
    {
        LOG_ERR<<"No commands taken, nothing to do."<<"\n";
    }


    for(auto c : appConfig.commands)
    {
        if (c.cmd==Command::addVar)
        {

//#if defined(WIN32) || defined(_WIN32)
        }

        else if (c.cmd==Command::updateVar)
        {
        }

        else if (c.cmd==Command::setVar)
        {
        }

        else if (c.cmd==Command::pathAdd)
        {
        }

        else if (c.cmd==Command::pathRemove)
        {
        }

        else if (c.cmd==Command::listEnv)
        {
        }

        else if (c.cmd==Command::makeAlias)
        {
        }


    } // for(auto c : appConfig.commands)


    // https://learn.microsoft.com/ru-ru/windows/win32/procthread/environment-variables
    // https://learn.microsoft.com/en-us/windows/win32/procthread/environment-variables
    // To programmatically add or modify system environment variables, add them to the 
    // HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment registry key, 
    // then broadcast a WM_SETTINGCHANGE message with lParam set to the string "Environment".

    // WM_SETTINGCHANGE - https://stackoverflow.com/questions/48928002/setting-global-environment-variables-programmatically

    // User Variables   - HKEY_CURRENT_USER\Environment
    // System Variables - HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment
    // User path variables (My Documents, AppData, etc) - HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders 

    // https://learn.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regsetvalueexa
    // https://learn.microsoft.com/en-us/windows/win32/sysinfo/registry-element-size-limits
    // https://learn.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regqueryvalueexw
    // https://learn.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-reggetvaluea




    return 0;
}
