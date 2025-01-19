//******************
//
// pleos_mathematics_geometry.h
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
// This file contains the source code of pleos_mathematics_geometry.cpp.
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

// Include pleos_mathematics_probabilities.h
#include "../../pleos_mathematics/pleos_mathematics_geometry.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    //******************
    //
    // The "Vector" class
    //
    //******************

    // Returns the norm of the vector (and the redaction if needed)
    scls::Formula Vector::norm(std::string* redaction) const {
        // Start the redaction
        if(redaction != 0) {
            (*redaction) += std::string("Nous cherchons la norme du vecteur ") + name() + std::string(". ");
        }

        // Do the calculation
        scls::Formula to_return;
        for(int i = 0;i<static_cast<int>(a_coordinates.size());i++) {to_return += (*a_coordinates[i].get()) * (*a_coordinates[i].get());}

        // Returns the needed result
        if(redaction != 0) {
            if(to_return.is_simple_monomonial()) {
                double value = to_return.to_polymonial().known_monomonial().factor().real().to_double();
                value = std::sqrt(value);
                (*redaction) += std::string("La norme du vecteur ") + name() + std::string(" est donc ") + scls::format_number_to_text(value) + std::string(".");
            }
        }
        return to_return;
    }
}
