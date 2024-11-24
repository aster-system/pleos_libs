//******************
//
// pleos_mathematics_functions.h
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
// This file contains the handling of functions in the PLEOS mathematics file.
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

#ifndef PLEOS_MATHEMATICS_FUNCTIONS
#define PLEOS_MATHEMATICS_FUNCTIONS

// Include SCLS Graphic Benoit
#include "../../../scls-graphic-benoit/scls_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Possible studied type
    enum Studied_Type {ST_Sequence};
    // Datas about a studied function
    struct Function_Studied {
        // Formula of the function
        scls::Formula function_formula;
        // Name of the function
        std::string function_name = "";
        // Number of the function
        int function_number = 0;
        // Name of the unknown in the function
        std::string function_unknown = "x";
        // Type of the studied function
        Studied_Type type = Studied_Type::ST_Sequence;
    };

    // Returns the image of a function
    scls::Formula function_image(Function_Studied current_function, scls::Formula needed_value, std::string& redaction);
    // Returns the limit of a function / polymonial in + infinity
    scls::Limit function_limit(Function_Studied current_function, scls::Limit needed_limit, std::string& redaction);
    // Returns the set of roots of a function
    scls::Set_Number function_roots(Function_Studied current_function, std::string& redaction);
    // Returns the set of a positive function
    scls::Set_Number function_sign(Function_Studied current_function, std::string& redaction);

    //******************
    //
    // Sequences handling
    //
    //******************

    // Returns the interval of an sequence
    scls::Interval sequence_variation(Function_Studied current_function, std::string& redaction);
}

#endif // PLEOS_MATHEMATICS_FUNCTIONS
