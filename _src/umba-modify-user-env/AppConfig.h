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

};

struct AppConfig
{
    std::vector<CommandInfo>       commands  ;
    CommandInfo                    curCommand;

    /*
        Добавляем команду в очередь только при получении новой команды,
        или после окончания обработки аргументов командной строки, если текущая команда не Command::none.

        При добавлении команды всё обнуляем в текущей, ставим cmd = Command::none
     */


    bool startNewCommand(Command c)
    {
        // Проверку валидности команд сделаем перед обработкой
        bool recurse = curCommand.recurse;
        commands.emplace_back(curCommand);
        curCommand = CommandInfo{};
        curCommand.cmd = c;
        curCommand.recurse = recurse;
        return true;
    }

    bool startNewCommand(Command c, const std::string &nameValue)
    {
        if (!startNewCommand())
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



