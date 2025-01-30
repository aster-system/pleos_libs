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

#ifndef PLEOS_PHYSIC_MATTER
#define PLEOS_PHYSIC_MATTER

// Include SCLS Graphic Benoit
#include "../pleos_mathematics/pleos_mathematics_functions.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    //******************
    //
    // The Bohr_Model class
    //
    //*****************

    class Bohr_Model {
        // Class representating a Bohr model
    public:

        // Struct representating a single electron
        struct Electron {
            // Offset of the angle for the rotation
            scls::Fraction angle = 0;
            // Attached circle of the electron
            std::shared_ptr<Circle> circle;
            // Distance to the nucleus
            scls::Fraction distance = 0;
        };
        // Struct representating a single photon
        struct Photon {
            // Attached circle of the photon
            std::shared_ptr<Circle> circle;
            // Time of the creation of the photo
            long long creation_time = 0;
            // Position of the end
            Vector end_position;
            // Position of the start
            Vector start_position;
        };

        // Bohr_Model constructor
        Bohr_Model(int electron_number):a_electron_number(electron_number){a_atom_radius = layer() * scls::Fraction(1, 2);};
        Bohr_Model():Bohr_Model(0){}

        // Returns the number of electrons in a layer
        int electrons_by_layer(int l);
        static int electrons_max_by_layer(int l){if(l == 0){return 2;}else if(l == 1){return 8;}else if(l == 2){return 8;}};
        // Number of layers in the model
        static int layer(int needed_electron_number) {if(needed_electron_number <= 1){return 0;}else if(needed_electron_number <= 9){return 1;}else if(needed_electron_number <= 17){return 2;}return 4;};
        inline int layer() const {return layer(a_electron_number);};

        // Datas about an electron
        // Distance of the electron to the nucleus
        std::shared_ptr<Electron> electron_datas(int n);

        // Datas about a photon
        // Distance of the photon to the nucleus
        std::shared_ptr<Photon> photon_datas(int n){return std::make_shared<Photon>();};

        // Getters and setters
        inline int electron_number() const {return a_electron_number;};

    private:
        // Width of the atom (in arbitrary units)
        scls::Fraction a_atom_radius = scls::Fraction(1, 2);
        // Number of electrons in the model
        int a_electron_number = 0;
    };
}

#endif // PLEOS_PHYSIC_MATTER
