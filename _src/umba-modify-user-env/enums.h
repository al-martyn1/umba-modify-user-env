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
    makeAlias    = 0x0007 /*!<  */

}; // enum 
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(Command)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( Command, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::listEnv      , "ListEnv"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::pathRemove   , "PathRemove" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::pathAdd      , "PathAdd"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::setVar       , "SetVar"     );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::addVar       , "AddVar"     );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::none         , "None"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::makeAlias    , "MakeAlias"  );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::updateVar    , "UpdateVar"  );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::invalid      , "Invalid"    );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( Command, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( Command, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::listEnv      , "list-env"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::listEnv      , "listenv"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::listEnv      , "list_env"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathRemove   , "path-remove" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathRemove   , "path_remove" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathRemove   , "pathremove"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathAdd      , "path-add"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathAdd      , "path_add"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::pathAdd      , "pathadd"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::setVar       , "set-var"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::setVar       , "set_var"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::setVar       , "setvar"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::addVar       , "add-var"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::addVar       , "add_var"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::addVar       , "addvar"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::none         , "none"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::makeAlias    , "make-alias"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::makeAlias    , "make_alias"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::makeAlias    , "makealias"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::updateVar    , "update-var"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::updateVar    , "update_var"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::updateVar    , "updatevar"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::invalid      , "unknown"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::invalid      , "invalid"     );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( Command, std::map, 1 )


