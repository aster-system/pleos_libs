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
// This file contains some features to understand geometry in the PLEOS mathematics file.
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

#ifndef PLEOS_MATHEMATICS_GEOMETRY
#define PLEOS_MATHEMATICS_GEOMETRY

// Include SCLS Graphic Benoit
#include "../../../scls-graphic-benoit/scls_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // The "Vector" class
    //
    //******************

    class Vector {
        // Class representating a mathematical vector (more mathematically advanced and geenralised than scls::Vector_3D)
    public:

        // Vector constructor
        Vector(std::string name):a_name(name){};
        Vector(std::string name, scls::Formula x, scls::Formula y):Vector(name){a_coordinates.push_back(std::make_shared<scls::Formula>(x));a_coordinates.push_back(std::make_shared<scls::Formula>(y));};

        // Returns the introduction of the vector
        inline std::string introduction() const {return std::string("Nous avons le vecteur ") + name() + std::string(" tel que ") + name() + std::string("(") + x()->to_std_string() + std::string(";") + y()->to_std_string() + std::string(").");};
        // Returns the norm of the vector (and the redaction if needed)
        scls::Formula norm(std::string* redaction) const;
        inline scls::Formula norm() const {return norm(0);};

        // Returns the possible known coordinates
        inline void set_x(scls::Formula formula) {if(a_coordinates.size() <= 0){a_coordinates.push_back(std::make_shared<scls::Formula>());}(*a_coordinates[0].get())=formula;};
        inline void set_y(scls::Formula formula) {while(a_coordinates.size() <= 1){a_coordinates.push_back(std::make_shared<scls::Formula>());}(*a_coordinates[1].get())=formula;};
        inline scls::Formula* x() const {if(a_coordinates.size() <= 0){return 0;} return a_coordinates[0].get();};
        inline scls::Formula* y() const {if(a_coordinates.size() <= 1){return 0;} return a_coordinates[1].get();};
        inline scls::Formula* z() const {if(a_coordinates.size() <= 2){return 0;} return a_coordinates[2].get();};
        inline scls::Formula* w() const {if(a_coordinates.size() <= 3){return 0;} return a_coordinates[3].get();};

        // Getters and setters
        inline scls::GUI_Text* connected_object()const{return a_connected_object.lock().get();};
        inline std::string name() const {return a_name;};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_name(std::string new_name){a_name = new_name;if(connected_object() != 0){connected_object()->set_text(std::string("Vecteur ") + a_name);}};

    private:
        // Connected object to this vector
        std::weak_ptr<scls::GUI_Text> a_connected_object = std::weak_ptr<scls::GUI_Text>();
        // Coordinates of the vector
        std::vector<std::shared_ptr<scls::Formula>> a_coordinates;
        // Name of the vector
        std::string a_name;
    };
}

#endif // PLEOS_MATHEMATICS_GEOMETRY
