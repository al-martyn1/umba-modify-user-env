#pragma once

#include "marty_cpp/marty_enum.h"

#include <exception>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>




//#!Command
enum class Command : std::uint32_t
{
    invalid      = (std::uint32_t)(-1) /*!<  */,
    unknown      = (std::uint32_t)(-1) /*!<  */,
    none         = 0x0000 /*!<  */,
    addVar       = 0x0001 /*!<  */,
    updateVar    = 0x0002 /*!<  */,
    setVar       = 0x0003 /*!<  */,
    pathAdd      = 0x0004 /*!<  */,
    pathRemove   = 0x0005 /*!<  */,
    listEnv      = 0x0006 /*!<  */,
    open         = 0x0007 /*!<  */,
    regOpen      = 0x0008 /*!<  */,
    openReg      = 0x0008 /*!<  */,
    makeAlias    = 0x0009 /*!<  */

}; // enum 
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(Command)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( Command, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::listEnv      , "ListEnv"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::setVar       , "SetVar"     );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::invalid      , "Invalid"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::none         , "None"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::pathAdd      , "PathAdd"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::addVar       , "AddVar"     );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::updateVar    , "UpdateVar"  );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::pathRemove   , "PathRemove" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::open         , "Open"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::regOpen      , "RegOpen"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::makeAlias    , "MakeAlias"  );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( Command, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( Command, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::listEnv      , "list-env"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::listEnv      , "list_env"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::listEnv      , "listenv"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::setVar       , "set-var"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::setVar       , "set_var"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::setVar       , "setvar"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::invalid      , "invalid"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::invalid      , "unknown"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::none         , "none"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathAdd      , "path-add"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathAdd      , "path_add"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathAdd      , "pathadd"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::addVar       , "add-var"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::addVar       , "add_var"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::addVar       , "addvar"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::updateVar    , "update-var"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::updateVar    , "update_var"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::updateVar    , "updatevar"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathRemove   , "path-remove" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathRemove   , "path_remove" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathRemove   , "pathremove"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::open         , "open"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::regOpen      , "open_reg"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::regOpen      , "reg-open"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::regOpen      , "open-reg"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::regOpen      , "reg_open"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::regOpen      , "openreg"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::regOpen      , "regopen"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::makeAlias    , "make-alias"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::makeAlias    , "make_alias"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::makeAlias    , "makealias"   );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( Command, std::map, 1 )


//#!PrintStyle
enum class PrintStyle : std::uint32_t
{
    invalid     = (std::uint32_t)(-1) /*!<  */,
    unknown     = (std::uint32_t)(-1) /*!<  */,
    normal      = 0x0000 /*!<  */,
    regular     = 0x0000 /*!<  */,
    pathList    = 0x0001 /*!<  */,
    list        = 0x0001 /*!<  */,
    commaList   = 0x0002 /*!<  */

}; // enum 
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(PrintStyle)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( PrintStyle, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrintStyle::commaList   , "CommaList" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrintStyle::invalid     , "Invalid"   );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrintStyle::pathList    , "PathList"  );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( PrintStyle::normal      , "Normal"    );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( PrintStyle, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( PrintStyle, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrintStyle::commaList   , "comma-list" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrintStyle::commaList   , "comma_list" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrintStyle::commaList   , "commalist"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrintStyle::invalid     , "invalid"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrintStyle::invalid     , "unknown"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrintStyle::pathList    , "path-list"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrintStyle::pathList    , "path_list"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrintStyle::pathList    , "pathlist"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrintStyle::pathList    , "list"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrintStyle::normal      , "normal"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( PrintStyle::normal      , "regular"    );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( PrintStyle, std::map, 1 )


