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
#include "umba/string_plus.h"
#include "umba/utf.h"
//
#if defined(WIN32) || defined(_WIN32)
#include "umba/win32_utils.h"
#endif

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
                                                            , "umba-modify-user-env" // overrideExeName
                                                            )
                                                        );

    // Force set CLI arguments while running under debugger
    if (umba::isDebuggerPresent())
    {
        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        argsParser.args.clear();
        argsParser.args.push_back("--list");

        #if 1
        argsParser.args.push_back("--path-prepend");
        argsParser.args.push_back("--path-add=BLA");
        #endif
          
        #if 0
        // argsParser.args.push_back("--no-expand-sz");
        argsParser.args.push_back("--expand-sz");
        argsParser.args.push_back("--set=TEST2");
        argsParser.args.push_back("test333");
        #endif
          
        #if 0
        argsParser.args.push_back("--add=TEST1:val");
        //argsParser.args.push_back("--add=TEST2");
        argsParser.args.push_back("val");
        //argsParser.args.push_back("--update=TEST3:fff");
        argsParser.args.push_back("--update=TEST4:eee");
        //argsParser.args.push_back("--set=PATH:path;path;path");
        //argsParser.args.push_back("--set=PATH");
        //argsParser.args.push_back("path;path;path");
        #endif

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

#if defined(WIN32) || defined(_WIN32)
    auto openEnvKey = [](HKEY &hkEnv)
    {
        using namespace umba::win32_utils;
        return regOpenKeyEx( HKEY_CURRENT_USER
                           , L"Environment"
                           , 0
                           , KEY_READ | KEY_WRITE
                           , &hkEnv
                           );
    };
#endif

#if defined(WIN32) || defined(_WIN32)
    HKEY hkEnv;
    auto resSt = openEnvKey(hkEnv);
    if (resSt!=ERROR_SUCCESS)
    {
        LOG_ERR<<"Failed to open 'Environment' key: " << umba::shellapi::getErrorMessage((int)resSt) << "\n";
        return 2;
    }
#endif


    int errorCount = 0;
    for(auto c : appConfig.commands)
    {

        if (!c.name.empty() && appConfig.isVarRestricted(c.name))
        {
            LOG_ERR<<"Can't perform actions on restricted environment variable '" << c.name << "'\n";
            ++errorCount;
            continue;
        }
        // if (umba::string::toupper_copy(c.name)=="PATH")
        // {
        //     LOG_ERR<<"Can't perform actions on PATH envicronment variable"<<"\n";
        //     ++errorCount;
        //     continue;
        // }


#if defined(WIN32) || defined(_WIN32)
        using namespace umba::win32_utils;

        if (c.cmd==Command::addVar || c.cmd==Command::updateVar)
        {
            std::wstring value;
            DWORD type;
            if (!regQueryValueEx(hkEnv, umba::fromUtf8(c.name), value, &type, &resSt))
            {
                if (resSt!=ERROR_FILE_NOT_FOUND)
                {
                    LOG_ERR<<"Failed to query value '" << c.name << "': " << umba::shellapi::getErrorMessage((int)resSt) << "\n";
                    ++errorCount;
                }
                else
                {
                    if (c.cmd==Command::updateVar)
                    {
                        LOG_ERR<<"--update-var: can't update variable '" << c.name << "': variable not found." << "\n";
                        ++errorCount;
                    }
                }
            }
            else // var was found
            {
                if (c.cmd==Command::addVar)
                {
                    LOG_ERR<<"--add-var: can't add variable '" << c.name << "': variable already exists." << "\n";
                    ++errorCount;
                }
            }

        } // if (c.cmd==Command::addVar || c.cmd==Command::updateVar)

        else if (c.cmd==Command::pathAdd || c.cmd==Command::pathRemove)
        {
            if (c.value.empty())
            {
                LOG_ERR << "--" << (c.cmd == Command::pathAdd ? "path-add" : "path-remove")
                    //<< ": can't " << (c.cmd==Command::pathAdd?"add":"remove")  << "add variable '" << c.name << "': variable already exists" << "\n";
                    << ": empty value taken.";
                // (c.cmd==Command::pathAdd?"":"") 

                ++errorCount;
            }

            std::wstring pathListStr;
            if (!regQueryValueEx(hkEnv, L"PATH", pathListStr, 0 /* &type */ , &resSt))
            {
                LOG_ERR<<"Failed to query PATH value: " << umba::shellapi::getErrorMessage((int)resSt) << "\n";
                ++errorCount;
                continue;
            }

        }

#endif

    } // for(auto c : appConfig.commands)


    if (errorCount)
    {
        ::RegCloseKey(hkEnv);
        LOG_ERR<<"Some error occurs. Nothing to be done" << "\n";
        return 3;
    }

    auto isPathStringsEqual = [](std::wstring p, const std::wstring &c1, const std::wstring &c2)
    {
        auto c = umba::filename::makeCanonicalForCompare(p);
        return c==c1 || c==c2;
    };
    
            // std::wstring pathListStr;
            // regQueryValueEx(hkEnv, L"PATH", pathListStr, 0 /* &type */ , &resSt);
            //  
            // auto paramPath = fromUtf8(umba::string::toupper_copy(c.value));



    for(auto c : appConfig.commands)
    {
#if defined(WIN32) || defined(_WIN32)
        using namespace umba::win32_utils;
        using umba::fromUtf8;
        using umba::toUtf8;
#endif
// struct CommandInfo
// {
//     Command       cmd     = Command::none;
//     std::string   name ;
//     std::string   value;
//  
// };

        if (c.cmd==Command::addVar || c.cmd==Command::updateVar || c.cmd==Command::setVar)
        {
            // Отсутствие/наличие переменной проверили раньше
            // Тут у нас для a//update/set код не отличается

#if defined(WIN32) || defined(_WIN32)

            bool expandSz = c.expandSz; // По дефоту устанавливаем то, что у нас задано для новых переменных

            if (!c.replaceSz) // У нас задано, что мы не заменяем текущий тип
            {
                DWORD type = REG_SZ;
                if (regQueryValueType(hkEnv, fromUtf8(c.name), type /* , LSTATUS *pStatus=0 */ )) 
                {
                    expandSz = type!=REG_SZ; // если получилось получить тип переменной, задаём expandSz в зависимости от полученного значения
                }
            }

            if (!argsParser.quet)
            {
                if (c.cmd==Command::addVar)         umbaLogStreamMsg << "Add var ";
                else if (c.cmd==Command::updateVar) umbaLogStreamMsg << "Update var ";
                else if (c.cmd==Command::setVar)    umbaLogStreamMsg << "Set var ";

                umbaLogStreamMsg << "'" << c.name << "' " << (c.cmd == Command::addVar ? "with value" : "to") << " '" << c.value << "' as " << (expandSz ? "REG_EXPAND_SZ" : "REG_SZ") << "\n";
            }

            // Ошибку лень проверять, тут ничего не должно странного происходить
            // Да, проверка/установка у нас не транзакционная
            regSetValue(hkEnv, fromUtf8(c.name), fromUtf8(c.value), 0 /* pStatus */, expandSz);

#else

#endif

        }

        else if (c.cmd==Command::pathAdd)
        {

#if defined(WIN32) || defined(_WIN32)

            std::wstring pathListStr;
            regQueryValueEx(hkEnv, L"PATH", pathListStr, 0 /* &type */ , &resSt);

            auto paramPathCanonical = fromUtf8(umba::filename::makeCanonical(c.value));
            //auto paramPathCmp = umba::string::toupper_copy(paramPathCanonical);
            //umba::filename::stripLastPathSep(paramPathCmp);
            umba::filename::stripLastPathSep(paramPathCanonical);

            auto canon1 = umba::filename::makeCanonical(paramPathCanonical);
            //auto canon2 = umba::filename::makeCanonical(paramPathCanonical+L'\\');

            auto cmp1 = umba::filename::makeCanonicalForCompare(paramPathCanonical);
            auto cmp2 = umba::filename::makeCanonicalForCompare(paramPathCanonical+L'\\');

            bool bFound = false;
            std::vector<std::wstring> pathList = umba::filename::splitPathList(pathListStr);
            for(auto &&p : pathList)
            {
                if (isPathStringsEqual(p, cmp1, cmp2))
                {
                    bFound = true;
                    break;
                }
            }

            if (!argsParser.quet)
            {
                umbaLogStreamMsg << "Path add: '" << c.value << "'" << ", canonical value: '" << toUtf8(canon1) << "'";
                if (bFound)
                    umbaLogStreamMsg << ": already in PATH, nothing to add";
                umbaLogStreamMsg << ".\n";
            }

            if (!bFound)
            {
                // Добавляем каноничный вариант, система неправильные слеши может не распарсить
                if (c.pathPrepend)
                    pathList.insert(pathList.begin(), paramPathCanonical); // В начало
                else
                    pathList.emplace_back(paramPathCanonical); // В конец

                pathListStr = umba::filename::mergePathList(pathList);
                regSetValue(hkEnv, L"Path", pathListStr, 0 /* pStatus */, true /* expandSz */ );
            }
            //isPathStringsEqual = [](std::wstring p, const std::wstring &c1, const std::wstring &c2)


            
#else

#endif
        }

        else if (c.cmd==Command::pathRemove)
        {

#if defined(WIN32) || defined(_WIN32)

            std::wstring pathListStr;
            regQueryValueEx(hkEnv, L"PATH", pathListStr, 0 /* &type */ , &resSt);

            auto paramPathCanonical = fromUtf8(umba::filename::makeCanonical(c.value));
            //auto paramPathCmp = umba::string::toupper_copy(paramPathCanonical);
            //umba::filename::stripLastPathSep(paramPathCmp);
            umba::filename::stripLastPathSep(paramPathCanonical);

            auto canon1 = umba::filename::makeCanonical(paramPathCanonical);
            //auto canon2 = umba::filename::makeCanonical(paramPathCanonical+L'\\');

            auto cmp1 = umba::filename::makeCanonicalForCompare(paramPathCanonical);
            auto cmp2 = umba::filename::makeCanonicalForCompare(paramPathCanonical+L'\\');

            std::vector<std::wstring> pathList = umba::filename::splitPathList(pathListStr);
            std::vector<std::size_t>  rmIndices;

            for(std::size_t i=0; i!=pathList.size(); ++i)
            {
                if (isPathStringsEqual(pathList[i], cmp1, cmp2))
                    rmIndices.emplace_back(i);
            }

            // лень геморроится с реверс итераторами
            std::reverse(rmIndices.begin(), rmIndices.end());

            for(auto rmIdx : rmIndices)
            {
                if (!argsParser.quet)
                {
                    umbaLogStreamMsg << "Path remove: '" << c.value << "'" << ", removing entry at index " << rmIdx << "\n";
                }

                pathList.erase(pathList.begin()+std::ptrdiff_t(rmIdx));
            }

            auto pathListStrNew = umba::filename::mergePathList(pathList);
            if (pathListStrNew!=pathListStr)
                regSetValue(hkEnv, L"Path", pathListStrNew, 0 /* pStatus */, true /* expandSz */ );

#else

#endif

        }

        else if (c.cmd==Command::listEnv)
        {
#if defined(WIN32) || defined(_WIN32)

            std::vector< RegValueInfo<std::wstring> > valInfos;

            LSTATUS status = ERROR_SUCCESS;
            regEnumValues(hkEnv, valInfos, &status);
            if (valInfos.empty() && status!=ERROR_SUCCESS)
            {
                LOG_ERR<<"Enumerating values error code: " << umba::shellapi::getErrorMessage((int)status) << "\n";
            }

            if (valInfos.empty())
                umbaLogStreamMsg << "\nThere is no user environment variables at all\n";
            else
                umbaLogStreamMsg << "\nList of user environment variables:\n"; 

            for(auto && vi : valInfos)
            {
                umbaLogStreamMsg << umba::toUtf8(vi.name) << ": ";
                if (vi.type==REG_EXPAND_SZ)
                    umbaLogStreamMsg << "REG_EXPAND_SZ: ";
                else if (vi.type==REG_SZ)
                    umbaLogStreamMsg << "REG_SZ: ";
                else
                {
                    umbaLogStreamMsg << "type incompatible with environment variable type\n";
                    continue;
                }

                std::wstring value;
                
                if (!regQueryValueEx(hkEnv, vi.name, value, 0 /* &type */ , &status))
                {
                    umbaLogStreamMsg << "Something goes wrong\n";
                    LOG_ERR<<"Failed to query value '" << c.name << "': " << umba::shellapi::getErrorMessage((int)resSt) << "\n";
                    continue;
                }

                PrintStyle printStyle = appConfig.getPrintStyle(umba::toUtf8(vi.name));
                if (printStyle!=PrintStyle::list)
                {
                    umbaLogStreamMsg << umba::toUtf8(value) << "\n";
                }
                else
                {
                    // Special formatting for PATH env var
                    umbaLogStreamMsg << "\n";
                    //std::vector<std::wstring> paths = umba::string_plus::split(value, L';', false);
                    std::vector<std::wstring> paths = umba::filename::splitPathList(value);
                    // StringType mergePathList( const std::vector< StringType > &plParts, typename StringType::value_type pathListSep = getNativePathListSep<typename StringType::value_type>())
                    for(auto &&p : paths)
                    {
                        if (!p.empty())
                            umbaLogStreamMsg << "    " << umba::toUtf8(p) << "\n";
                    }
                    //umbaLogStreamMsg << "\n";
                }
                
            } // for(auto && vi : valInfos)

            umbaLogStreamMsg << "\n";

#endif


// template<typename StringType>
// struct RegValueInfo
// {
//     StringType     name;
//     DWORD          type;
//  
// }; // struct RegValueInfo
//  
// //----------------------------------------------------------------------------
// template<typename StringType>
// inline
// void regEnumValues(HKEY hKey, std::vector< RegValueInfo<StringType> > &valInfos)
// {



        }

        else if (c.cmd==Command::openReg)
        {
#if defined(WIN32) || defined(_WIN32)
            HKEY hkRg;
            auto stRg = regOpenKeyEx( HKEY_CURRENT_USER
                                    , L"Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Regedit"
                                    , 0
                                    ,  /* KEY_READ | */  KEY_WRITE
                                    , &hkRg
                                    );
            if (stRg==ERROR_SUCCESS)
            {
                regSetValue(hkRg, L"Lastkey", L"HKEY_CURRENT_USER\\Environment", 0 /* pStatus */, false /* expandSz */ );
                //regSetValue(hkRg, L"Lastkey", L"\\HKCU\\Environment", 0 /* pStatus */, true /* expandSz */ );
                //regSetValue(hkRg, L"Lastkey", L"HKCU\\Environment", 0 /* pStatus */, true /* expandSz */ );
            }

            ::RegCloseKey(hkRg);
            
            umba::shellapi::executeOpen("regedit");

#else

#endif
        }

        else if (c.cmd==Command::open)
        {
#if defined(WIN32) || defined(_WIN32)

#else

#endif
        }

        else if (c.cmd==Command::makeAlias)
        {
#if defined(WIN32) || defined(_WIN32)

            //umba::shellapi::executeOpen("rundll32 sysdm.cpl,EditEnvironmentVariables");
            //rundll32 sysdm.cpl,EditEnvironmentVariables"
            //umba::shellapi::callSystem(const std::string &cmd, const std::vector<std::string> &cmdArgs, std::string *pErrMsg=0, bool allocateConsole=true)
            // umba::shellapi::callSystem("rundll32", std::vector<std::string>{"sysdm.cpl,EditEnvironmentVariables"});

            // cmd /c "start rundll32 sysdm.cpl,EditEnvironmentVariables"
            // umba::shellapi::callSystem("cmd", std::vector<std::string>{"/c", "start rundll32 sysdm.cpl,EditEnvironmentVariables"});

            // umba::shellapi::callSystem("sysdm.cpl", std::vector<std::string>{"EditEnvironmentVariables"});

            // WinExec("c:\windows\system32\control.exe sysdm.cpl,EditEnvironmentVariables", SW_NORMAL);

            // WinExec("%systemroot%\system32\control.exe /name EditEnvironmentVariables", SW_NORMAL);

            // ::ShellExecuteA(0, NULL, "control.exe", "sysdm.cpl,EditEnvironmentVariables", NULL, SW_SHOW);

            // WinExec("%systemroot%\system32\control.exe /name sysdm.cpl,EditEnvironmentVariables", SW_NORMAL);

// https://learn.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-expandenvironmentstringsa
//             DWORD ExpandEnvironmentStringsA(
//   [in]            LPCSTR lpSrc,
//   [out, optional] LPSTR  lpDst,
//   [in]            DWORD  nSize
// );


            // https://winaero.com/open-control-panel-applets-windows-10/   

#else

#endif
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
    // https://learn.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-reggetvaluea   - requires _WIN32_WINNT as 0x0600 or later

    // reg - https://www.techtarget.com/searchwindowsserver/tip/Command-line-options-for-Regeditexe

    // Regedit\LastKey
    // https://superuser.com/questions/115854/open-registry-directly-to-a-given-key

    // HKCU\Software\Microsoft\Windows\CurrentVersion\Applets\Regedit

    // Set WshShell = CreateObject("WScript.Shell")
    // WshShell.RegWrite "HKCU\Software\Microsoft\Windows\CurrentVersion\Applets\Regedit\Lastkey","HKLM\Software\Microsoft\Foo\Bar","REG_SZ"
    // WshShell.Run "regedit", 1,True
    // Set WshShell = Nothing

    // http://www.adp-gmbh.ch/win/misc/environment_variables.html
    // rundll32 sysdm.cpl,EditEnvironmentVariables
    // cmd /c "start rundll32 sysdm.cpl,EditEnvironmentVariables"


    ::RegCloseKey(hkEnv);

    return 0;
}
