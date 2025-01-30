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
        Vector():Vector(""){};

        // Returns a copy of this vector
        Vector vector_copy() const {Vector to_return(a_name);for(int i = 0;i<static_cast<int>(a_coordinates.size()) && i < static_cast<int>(a_coordinates.size());i++){to_return.a_coordinates.push_back(std::make_shared<scls::Formula>(a_coordinates[i].get()->formula_copy()));}return to_return;};

        // Returns the mesured angle between to vector
        scls::Formula angle(Vector* needed_vector, std::string* redaction);
        inline scls::Formula angle(Vector* needed_vector) {return angle(needed_vector, 0);};
        // Returns the introduction of the vector
        inline std::string introduction() const {return std::string("Nous avons le vecteur ") + name() + std::string(" tel que ") + name() + std::string("(") + x()->to_std_string() + std::string(";") + y()->to_std_string() + std::string(").");};
        // Returns the norm of the vector (and the redaction if needed)
        scls::Formula norm(std::string* redaction);
        inline scls::Formula norm() {return norm(0);};

        // Returns the possible known coordinates
        inline void set_x(scls::Formula formula) {if(a_coordinates.size() <= 0){a_coordinates.push_back(std::make_shared<scls::Formula>());}(*a_coordinates[0].get())=formula;};
        inline void set_y(scls::Formula formula) {while(a_coordinates.size() <= 1){a_coordinates.push_back(std::make_shared<scls::Formula>());}(*a_coordinates[1].get())=formula;};
        inline scls::Formula* x() const {if(a_coordinates.size() <= 0){return 0;} return a_coordinates[0].get();};
        inline scls::Formula* y() const {if(a_coordinates.size() <= 1){return 0;} return a_coordinates[1].get();};
        inline scls::Formula* z() const {if(a_coordinates.size() <= 2){return 0;} return a_coordinates[2].get();};
        inline scls::Formula* w() const {if(a_coordinates.size() <= 3){return 0;} return a_coordinates[3].get();};

        // Operators methods
        // With Fractions
        void __multiply(scls::Fraction value){for(int i = 0;i<static_cast<int>(a_coordinates.size());i++){a_coordinates[i].get()->__multiply(value);}};
        // With Vector
        void __add(Vector value){for(int i = 0;i<static_cast<int>(a_coordinates.size()) && i < static_cast<int>(value.a_coordinates.size());i++){a_coordinates[i].get()->__add(value.a_coordinates[i].get());}};
        void __substract(Vector value){for(int i = 0;i<static_cast<int>(a_coordinates.size()) && i < static_cast<int>(value.a_coordinates.size());i++){a_coordinates[i].get()->__substract(value.a_coordinates[i].get());}};

        // Operators
        // With Fractions
        Vector operator*=(scls::Fraction value){__multiply(value);return *this;};
        Vector operator*(scls::Fraction value) const {Vector new_value(vector_copy());new_value.__multiply(value);return new_value;};
        // With Vector
        Vector& operator+=(Vector value){__add(value);return *this;};
        Vector operator+(Vector value) const {Vector new_value(vector_copy());new_value.__add(value);return new_value;};
        Vector& operator-=(Vector value){__substract(value);return *this;};
        Vector operator-(Vector value) const {Vector new_value(vector_copy());new_value.__substract(value);return new_value;};

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

        // Last norm of the vector
        scls::Formula a_last_norm; bool a_last_norm_calculated = false;
    };

    //******************
    //
    // The "Circle" class
    //
    //******************

    class Circle {
        // Class representating a geometrical circle
    public:
        // Circle constructor
        Circle(std::string name, Vector center, scls::Formula radius):a_center(center),a_name(name),a_radius(radius){};

       // Returns the radius of the circle
        virtual scls::Formula radius(){return a_radius;};

        // Getters and setters
        inline scls::Color border_color() const {return a_border_color;};
        inline int border_radius() const {return a_border_radius;};
        inline Vector center() const {return a_center;};
        inline scls::Color color() const {return a_color;};
        inline std::string name() const {return a_name;};
        inline void set_border_color(scls::Color new_border_color) {a_border_color = new_border_color;};
        inline void set_border_radius(int new_border_radius) {a_border_radius = new_border_radius;};
        inline void set_center(Vector new_center){a_center = new_center;};
        inline void set_color(scls::Color new_color) {a_color = new_color;};
    private:
        // Color of the border of the circle
        scls::Color a_border_color;
        // Radius of the border
        int a_border_radius = 0;
        // Center of the circle
        Vector a_center;
        // Color of the circle
        scls::Color a_color;
        // Name of the circle
        std::string a_name = std::string();
        // Radius of the circle
        scls::Formula a_radius;
    };
}

#endif // PLEOS_MATHEMATICS_GEOMETRY
