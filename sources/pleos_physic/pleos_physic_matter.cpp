//******************
//
// pleos_physic_core.h
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
// This file contains some datas about using the matter in physic.
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

// Include pleos_physic_matter.h
#include "../../pleos_physic/pleos_physic_matter.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    //******************
    //
    // The Bohr_Model class
    //
    //*****************

    // Distance of the electron to the nucleus
    std::shared_ptr<Bohr_Model::Electron> Bohr_Model::electron_datas(int n) {
        std::shared_ptr<Bohr_Model::Electron> needed_electron = std::make_shared<Bohr_Model::Electron>();

        // Get the angle of the electron
        if(n == 1) {needed_electron.get()->angle = scls::Fraction(180);}
        else if(n > 1 && n <= 9) {needed_electron.get()->angle = scls::Fraction(floor(360.0 / (electrons_by_layer(layer(n)))) * (n - 2)); }
        else if(n > 9 && n <= 17) {needed_electron.get()->angle = scls::Fraction(floor(360.0 / (electrons_by_layer(layer(n)))) * (n - 10)); }

        // Get the distance of the electron
        needed_electron.get()->distance = scls::Fraction(1, 2) * layer(n);

        // Returns the result
        return needed_electron;
    }
    // Returns the number of electrons in a layer
    int Bohr_Model::electrons_by_layer(int l){
        int final_electron = a_electron_number;
        int current_line = 0;
        while(current_line < l && final_electron > electrons_max_by_layer(current_line)){
            final_electron -= electrons_max_by_layer(current_line);
            current_line++;
        }
        if(final_electron > electrons_max_by_layer(current_line)){final_electron = electrons_max_by_layer(current_line);}
        return final_electron;
    }
}
