#pragma once


#include "umba/umba.h"
//
#include "umba/string_plus.h"
#include "umba/filesys.h"
//
#include "enums.h"

//
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <umba/the.h>


struct CommandInfo
{
    Command       cmd     = Command::none;
    std::string   name ;
    std::string   value;
    bool          expandSz    = false;
    bool          replaceSz   = false;
    bool          pathPrepend = false;

};

struct AppConfig
{
    std::vector<CommandInfo>                      commands    ;
    CommandInfo                                   curCommand  ;

    std::unordered_map<std::string, PrintStyle>   printStyles ; // names of vars of semicolon separated lists       --print, --print-style
    std::unordered_set<std::string>               restrictVars; // names of vars which restricted from modification --restrict=VAR

    /*
        Добавляем команду в очередь только при получении новой команды,
        или после окончания обработки аргументов командной строки, если текущая команда не Command::none.

        При добавлении команды всё обнуляем в текущей, ставим cmd = Command::none
     */


    bool startNewCommand(Command c)
    {
        // Проверку валидности команд сделаем перед обработкой
        bool expandSz    = curCommand.expandSz   ;
        bool replaceSz   = curCommand.replaceSz  ;
        bool pathPrepend = curCommand.pathPrepend;
        commands.emplace_back(curCommand);
        curCommand = CommandInfo{};
        curCommand.cmd = c;
        curCommand.expandSz    = expandSz   ;
        curCommand.replaceSz   = replaceSz  ;
        curCommand.pathPrepend = pathPrepend;
        return true;
    }

    bool setExpandSz(bool bExpand)
    {
        curCommand.expandSz = bExpand;
        return true;
    }

    bool setReplaceSz(bool bReplace)
    {
        curCommand.replaceSz = bReplace;
        return true;
    }

    bool setPathPrepend(bool bPrepend)
    {
        curCommand.pathPrepend = bPrepend;
        return true;
    }

    bool addRestrictVar(const std::string &var)
    {
#if defined(WIN32) || defined(_WIN32)
        restrictVars.insert(umba::string::toupper_copy(var));
#else
        restrictVars.insert(var);
#endif
        return true;
    }

    bool isVarRestricted(const std::string &var) const
    {
#if defined(WIN32) || defined(_WIN32)
        auto it = restrictVars.find(umba::string::toupper_copy(var));
#else
        auto it = restrictVars.find(var);
#endif
        return it!=restrictVars.end();
    }

    PrintStyle getPrintStyle(const std::string &var) const
    {
#if defined(WIN32) || defined(_WIN32)
        auto it = printStyles.find(umba::string::toupper_copy(var));
#else
        auto it = printStyles.find(var);
#endif
        if (it==printStyles.end())
            return PrintStyle::normal;
        return it->second;
    }

    bool setPrintStyle(const std::string &styleStr, const std::vector<std::string> &names)
    {
        auto style = enum_deserialize(styleStr, PrintStyle::unknown);
        if (style==PrintStyle::unknown)
            return false;

        for(auto &&name : names)
        {
            if (name.empty())
                continue;

#if defined(WIN32) || defined(_WIN32)
            printStyles[umba::string::toupper_copy(name)] = style;
#else
            printStyles[name] = style;
#endif
        }

        return true;
    }

    bool setPrintStyle(const std::string &style, const std::string &namesStr)
    {
        std::vector<std::string> namesVec = umba::string_plus::split(namesStr, ',', false);
        return setPrintStyle(style, namesVec);
    }

    bool setPrintStyle(const std::string &styleAndNames)
    {
        auto pos = styleAndNames.find(':');
        if (pos== styleAndNames.npos)
            return false;
        return setPrintStyle(std::string(styleAndNames, 0, pos), std::string(styleAndNames, pos+1));
    }

    bool startNewCommand(Command c, const std::string &nameValue)
    {
        if (!startNewCommand(c))
            return false;

        return setNameValue(nameValue);
    }

    bool isCommandSet() const
    {
        return curCommand.cmd!=Command::none;
    }

    bool isNameSet() const
    {
        return !curCommand.name.empty();
    }

    bool isValueSet() const
    {
        return !curCommand.value.empty();
    }

    bool finalizeCommands()
    {
        if (!isCommandSet())
        {
            return true; // curCommand.path.empty();
        }

        commands.emplace_back(curCommand);
        curCommand = CommandInfo{};
        return true;
    }

    bool setNameValue(const std::string &name, const std::string &value)
    {
        if (isNameSet() || isValueSet())
            return false;

        curCommand.name  = name ;
        curCommand.value = value;
    
        return !curCommand.name.empty();
    }

    bool setNameValue(const std::string &nameValue)
    {
        auto pos = nameValue.find(':');
        if (pos==nameValue.npos)
            return setNameValue(nameValue, std::string());
        return setNameValue(std::string(nameValue, 0, pos), std::string(nameValue, pos+1));
    }

    bool setValue(const std::string &value)
    {
        if (isValueSet())
            return false;

        curCommand.value = value;
    
        return true;
    }



}; // struct AppConfig



