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

// ID of each types of geometrical objects
#define PLEOS_MATHEMATICS_GEOMETRY_VECTOR 0
#define PLEOS_MATHEMATICS_GEOMETRY_POINT 10
#define PLEOS_MATHEMATICS_GEOMETRY_FORM 100

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // The "Vector" class
    //
    //******************

    enum Vector_Type{VT_Vector, VT_Point};
    class Vector {
        // Class representating a mathematical vector (more mathematically advanced and geenralised than scls::Vector_3D)
    public:

        // Vector constructor
        Vector(std::string name):a_name(name){};
        Vector(std::string name, scls::Formula x, scls::Formula y):Vector(name){a_coordinates.push_back(std::make_shared<scls::Formula>(x));a_coordinates.push_back(std::make_shared<scls::Formula>(y));};
        Vector():Vector(""){};

        // Returns a vector 3D from a point
        static std::shared_ptr<Vector> from_point(scls::model_maker::Point* needed_point){return std::make_shared<Vector>(std::string(), scls::Fraction(needed_point->x() * 1000000.0, 1000000), scls::Fraction(needed_point->z() * 1000000.0, 1000000));};
        // Returns this vector in a point 3D
        inline scls::Point_3D to_point_3d() {if(a_coordinates.size() > 2){return scls::Point_3D(x()->to_polymonial().monomonial().factor().real().to_double(), y()->to_polymonial().monomonial().factor().real().to_double(), z()->to_polymonial().monomonial().factor().real().to_double());}else if(a_coordinates.size() > 1){return scls::Point_3D(x()->to_polymonial().monomonial().factor().real().to_double(), 0, y()->to_polymonial().monomonial().factor().real().to_double());}else if(a_coordinates.size() > 0){return scls::Point_3D(x()->to_polymonial().monomonial().factor().real().to_double(), 0,0);}return scls::Point_3D();};
        // Returns a copy of this vector
        Vector vector_copy() const {Vector to_return(a_name);for(int i = 0;i<static_cast<int>(a_coordinates.size()) && i < static_cast<int>(a_coordinates.size());i++){to_return.a_coordinates.push_back(std::make_shared<scls::Formula>(a_coordinates[i].get()->formula_copy()));}return to_return;};

        // Returns the mesured angle between to vector
        scls::Formula angle(Vector* needed_vector, std::string* redaction);
        inline scls::Formula angle(Vector* needed_vector) {return angle(needed_vector, 0);};
        // Returns the complex number form of the vector (and the redaction if needed)
        scls::Formula complex_number(std::string* redaction);
        // Returns the introduction of the vector
        inline std::string introduction() const {return std::string("Nous avons le ") + type_name(false) + std::string(" ") + name() + std::string(" tel que ") + name() + std::string("(") + x()->to_std_string() + std::string(";") + y()->to_std_string() + std::string(").");};
        // Returns the norm of the vector (and the redaction if needed)
        scls::Formula norm(std::string* redaction);
        inline scls::Formula norm() {return norm(0);};
        // Rotates the vector
        inline void rotate(scls::Fraction angle){scls::Point_3D point = scls::Point_3D(x_to_double(), 0, y_to_double());point.rotate_y(angle.to_double());set_x(scls::Fraction(point.x() * 100000.0, 100000.0));set_y(scls::Fraction(point.z() * 100000.0, 100000.0));};

        // Returns the possible known coordinates
        inline void set_x(scls::Formula formula) {if(a_coordinates.size() <= 0){a_coordinates.push_back(std::make_shared<scls::Formula>());}(*a_coordinates[0].get())=formula;};
        inline void set_y(scls::Formula formula) {while(a_coordinates.size() <= 1){a_coordinates.push_back(std::make_shared<scls::Formula>());}(*a_coordinates[1].get())=formula;};
        inline scls::Formula* x() const {if(a_coordinates.size() <= 0){return 0;} return a_coordinates[0].get();};
        inline double x_to_double() const{return x()->value(0).real().to_double();};
        inline scls::Formula* y() const {if(a_coordinates.size() <= 1){return 0;} return a_coordinates[1].get();};
        inline double y_to_double() const{return y()->value(0).real().to_double();};
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
        inline std::vector<std::shared_ptr<scls::Formula>>& coordinates(){return a_coordinates;};
        inline std::string name() const {return a_name;};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_name(std::string new_name){a_name = new_name;if(connected_object() != 0){connected_object()->set_text(type_name() + std::string(" ") + a_name);}};
        inline void set_type(Vector_Type new_type){a_type=new_type;};
        inline Vector_Type type() const {return a_type;};
        inline std::string type_name(bool capitalise_first_letter) const {if(capitalise_first_letter){if(a_type == Vector_Type::VT_Point){return std::string("Point");}return std::string("Vecteur");}if(a_type == Vector_Type::VT_Point){return std::string("point");}return std::string("vecteur");};
        inline std::string type_name() const {return type_name(true);};

    private:
        // Connected object to this vector
        std::weak_ptr<scls::GUI_Text> a_connected_object = std::weak_ptr<scls::GUI_Text>();
        // Coordinates of the vector
        std::vector<std::shared_ptr<scls::Formula>> a_coordinates;
        // Name of the vector
        std::string a_name;
        // Type of the vector
        Vector_Type a_type = Vector_Type::VT_Vector;

        // Last norm of the vector
        scls::Formula a_last_norm; bool a_last_norm_calculated = false;
    };

    //******************
    //
    // The "Form_2D" class
    //
    //******************

    class Form_2D {
        // Class representating a geometrical form in 2D
    public:
        // Link between the point
        struct Link {double drawing_proportion = 1;};

        // Form_2D constructor
        Form_2D(std::string name):a_name(name){};

        // Adds an exclusion point to the form
        inline void add_exclusion_point(std::shared_ptr<Vector> point){a_exclusion_points.push_back(point);};
        // Adds a point to the form
        inline void add_point(std::shared_ptr<Vector> point){a_points.push_back(point);a_points_link.push_back(Link());};
        // Rotates the form
        inline void rotate(scls::Fraction angle){for(int i = 0;i<static_cast<int>(a_points.size());i++){a_points[i].get()->rotate(angle);}};

        // Returns a list of the points triangulated
        std::vector<std::shared_ptr<Vector>> triangulated_points();

        // Returns the introduction of the form 2D
        inline std::string introduction() const {return std::string("Nous avons le ") + type_name(false) + std::string(" ") + name() + std::string(".");};
        // Returns the name of the type of the form
        inline std::string type_name(bool capitalise_first_letter) const {std::string to_return=std::string();if(a_points.size()==3){if(capitalise_first_letter){return std::string("Triangle");}return std::string("triangle");}if(capitalise_first_letter){to_return+=std::string("Forme");}else{to_return+=std::string("forme");}to_return+=std::string(" à ")+std::to_string(a_points.size())+std::string(" points");return to_return;};
        inline std::string type_name() const {return type_name(true);};

        // Getters and setters
        inline scls::Color border_color()const{return a_border_color;};
        inline int border_radius()const{return a_border_radius;};
        inline scls::Color color()const{return a_color;};
        inline scls::GUI_Text* connected_object()const{return a_connected_object.lock().get();};
        inline std::vector<std::shared_ptr<Vector>>& exclusion_points(){return a_exclusion_points;};
        inline Link& last_link(){return a_points_link[a_points_link.size() - 1];};
        inline Link& link(int position){return a_points_link[position];};
        inline std::string name() const {return a_name;};
        inline double opacity() const {return a_opacity;};
        inline std::vector<std::shared_ptr<Vector>>& points(){return a_points;};
        inline void set_border_color(scls::Color new_border_color){a_border_color=new_border_color;};
        inline void set_border_radius(int new_border_radius){a_border_radius=new_border_radius;};
        inline void set_color(scls::Color new_color){a_color=new_color;};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_link_drawing_proportion(int link, double new_proportion){if(link >= a_points_link.size()){return;}a_points_link[link].drawing_proportion = new_proportion;};
        inline void set_links_drawing_proportion(double new_proportion){for(int i = 0;i<static_cast<int>(a_points_link.size());i++){a_points_link[i].drawing_proportion = new_proportion;}};
        inline void set_name(std::string new_name){a_name = new_name;if(connected_object() != 0){connected_object()->set_text(std::string("Forme ") + a_name);}};
        inline void set_opacity(double new_opacity){a_opacity = new_opacity;};

    private:
        // Color of the border of the form
        scls::Color a_border_color = scls::Color(255, 0, 0);
        // Radius of the border
        int a_border_radius = 6;
        // Color of the inner form
        scls::Color a_color = scls::Color(0, 255, 0);
        // Connected object to this vector
        std::weak_ptr<scls::GUI_Text> a_connected_object = std::weak_ptr<scls::GUI_Text>();
        // Name of the circle
        std::string a_name = std::string();
        // Opacity of the form
        double a_opacity = 1;

        // Exclusions points in the circle
        std::vector<std::shared_ptr<Vector>> a_exclusion_points;
        // Points in the circle
        std::vector<std::shared_ptr<Vector>> a_points;
        // Links for the point
        std::vector<Link> a_points_link;
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
