//******************
//
// pleos_command.cpp
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
// This file contains the source code of pleos_command.h.
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

// Include SCLS Graphic Benoit
#include "../pleos_command.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    // Executes a command
    int execute(std::string command, std::string file_output, std::vector<std::string> parameters) {
        if(command == std::string_view("graph")) {
            // Solve command

            // Asserts
            if(parameters.size() < 1){return PLEOS_COMMAND_NOT_ENOUGH_PARAMETERS;}
            else if(parameters.size() > 1){return PLEOS_COMMAND_TOO_MUCH_PARAMETERS;}

            // Solve the equation
            scls::Graph_Base g = scls::Graph_Base();
            std::string to_solve = parameters.at(0);
            g.load_from_xml(to_solve);

            // Write the result
            g.to_image().get()->save_png(file_output);
            return 1;
        }
        else if(command == std::string_view("solve")) {
            // Solve command

            // Asserts
            if(parameters.size() < 1){return PLEOS_COMMAND_NOT_ENOUGH_PARAMETERS;}
            else if(parameters.size() > 1){return PLEOS_COMMAND_TOO_MUCH_PARAMETERS;}

            // Solve the equation
            std::string redaction = std::string();
            std::string to_solve = parameters.at(0);
            std::shared_ptr<scls::__Formula> f = scls::string_to_formula(to_solve);
            Function_Studied fs = Function_Studied(f);
            solve_equation(&fs, &redaction);

            // Write the result
            scls::Text_Style s;s.set_max_width(600);
            scls::to_image(redaction, s).get()->save_png(file_output);
            return 1;
        }
        else if(command == std::string_view("study")) {
            // Solve command

            // Asserts
            if(parameters.size() < 1){return PLEOS_COMMAND_NOT_ENOUGH_PARAMETERS;}
            else if(parameters.size() > 1){return PLEOS_COMMAND_TOO_MUCH_PARAMETERS;}

            // Solve the equation
            std::string redaction = std::string();
            std::string to_solve = parameters.at(0);
            std::shared_ptr<scls::__Formula> f = scls::string_to_formula(to_solve);
            Function_Studied fs = Function_Studied(f);
            study_function(f.get(), &redaction);

            // Write the result
            scls::Text_Style s;s.set_max_width(600);
            scls::Text_Image_Generator tig;
            pleos::load_balises_pleos(tig.balises_shared_ptr());
            tig.image_shared_ptr<pleos::Text>(redaction, s).get()->save_png(file_output);
            return 1;
        }

        // Unknown command
        return PLEOS_COMMAND_UNKNOWN_COMMAND;
    }
}
