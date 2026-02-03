//******************
//
// pleos_command.h
//
//******************
// Presentation :
//
// PLEOS, by Aster System, is a project which aims education.
// By using the power of SCLS and other Aster System's tools, we created  this.
// We want to make education easier for everyone (teachers, as students and pupils).
// The software is made in french, because the main goal is France educational system.
// For more information, see : https://aster-system.github.io/aster-system/projects/pleos.html.
//
// This file contains an API to remotely use PLEOS.
//
//******************
//
// License (GPL V3.0) :
//
// Copyright (C) 2024 by Aster System, Inc. <https://aster-system.github.io/aster-system/>
// This file is part of PLEOS.
// PLEOS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// PLEOS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with PLEOS. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef PLEOS_COMMAND
#define PLEOS_COMMAND

// Include SCLS Graphic Benoit
#include "pleos_text.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    // Possible errors
    constexpr int PLEOS_COMMAND_NOT_ENOUGH_PARAMETERS = -1;
    constexpr int PLEOS_COMMAND_TOO_MUCH_PARAMETERS = -2;
    constexpr int PLEOS_COMMAND_UNKNOWN_COMMAND = -3;

    // Executes a command
    int execute(std::string command, std::string file_output, std::vector<std::string> parameters);
}

#endif // PLEOS_COMMAND
