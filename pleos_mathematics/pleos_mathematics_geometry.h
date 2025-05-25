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

    enum Vector_Type{VT_Arrow, VT_Point, VT_Vector};
    class Vector {
        // Class representating a mathematical vector (more mathematically advanced and geenralised than scls::Vector_3D)
    public:

        // Vector constructor
        Vector(std::string name):a_name(name){};
        Vector(std::string name, scls::Formula x, scls::Formula y):Vector(name){a_coordinates.push_back(std::make_shared<scls::Formula>(x));a_coordinates.push_back(std::make_shared<scls::Formula>(y));};
        Vector(scls::Formula x, scls::Formula y):Vector(std::string(), x, y){};
        Vector(scls::Point_2D point):Vector(std::string(), point.x(), point.y()){};
        Vector():Vector(""){};

        // Returns a vector 3D from a point
        static std::shared_ptr<Vector> from_point(scls::model_maker::Point* needed_point){return std::make_shared<Vector>(std::string(), scls::Fraction(needed_point->x() * 1000000.0, 1000000), scls::Fraction(needed_point->z() * 1000000.0, 1000000));};
        // Returns this vector in a point 2D / 3D
        inline scls::Point_2D to_point_2d() {if(a_coordinates.size() > 1){return scls::Point_2D(x()->to_polymonial().monomonial().factor().real().to_double(), y()->to_polymonial().monomonial().factor().real().to_double());}else if(a_coordinates.size() > 0){return scls::Point_2D(x()->to_polymonial().monomonial().factor().real().to_double(), 0);}return scls::Point_2D();};
        inline scls::Point_3D to_point_3d() {if(a_coordinates.size() > 2){return scls::Point_3D(x()->to_polymonial().monomonial().factor().real().to_double(), y()->to_polymonial().monomonial().factor().real().to_double(), z()->to_polymonial().monomonial().factor().real().to_double());}else if(a_coordinates.size() > 1){return scls::Point_3D(x()->to_polymonial().monomonial().factor().real().to_double(), 0, y()->to_polymonial().monomonial().factor().real().to_double());}else if(a_coordinates.size() > 0){return scls::Point_3D(x()->to_polymonial().monomonial().factor().real().to_double(), 0,0);}return scls::Point_3D();};
        // Returns a copy of this vector
        Vector vector_copy() const {Vector to_return(a_name);for(int i = 0;i<static_cast<int>(a_coordinates.size()) && i < static_cast<int>(a_coordinates.size());i++){to_return.a_coordinates.push_back(a_coordinates[i].get()->formula_copy());}return to_return;};

        // Returns the mesured angle between to vector
        scls::Formula angle(Vector* needed_vector, std::string* redaction, scls::Textual_Math_Settings* settings);
        inline scls::Formula angle(Vector* needed_vector) {return angle(needed_vector, 0, 0);};
        // Returns the complex number form of the vector (and the redaction if needed)
        scls::Formula complex_number(std::string* redaction, scls::Textual_Math_Settings* settings);
        // Returns the introduction of the vector
        inline std::string introduction(scls::Textual_Math_Settings* settings) const {return std::string("Nous avons le ") + type_name(false) + std::string(" ") + name() + std::string(" tel que ") + name() + std::string("(") + x()->to_std_string(settings) + std::string(";") + y()->to_std_string(settings) + std::string(").");};
        // Returns the norm of the vector (and the redaction if needed)
        scls::Formula norm(std::string* redaction, scls::Textual_Math_Settings* settings);
        inline scls::Formula norm() {return norm(0, 0);};
        // Rotates the vector
        inline void rotate(scls::Fraction angle){scls::Point_3D point = scls::Point_3D(x_to_double(), 0, y_to_double());point.rotate_y(angle.to_double());set_x(scls::Fraction(point.x() * 100000.0, 100000.0));set_y(scls::Fraction(point.z() * 100000.0, 100000.0));};

        // Returns the possible known coordinates
        inline void set_x(scls::Formula formula) {if(a_coordinates.size() <= 0){a_coordinates.push_back(std::make_shared<scls::Formula>());}(*a_coordinates[0].get())=formula;};
        inline void set_x_end(scls::Formula formula) {if(a_coordinates_end.size() <= 0){a_coordinates_end.push_back(std::make_shared<scls::Formula>());}(*a_coordinates_end[0].get())=formula;};
        inline void set_y(scls::Formula formula) {while(a_coordinates.size() <= 1){a_coordinates.push_back(std::make_shared<scls::Formula>());}(*a_coordinates[1].get())=formula;};
        inline void set_y_end(scls::Formula formula) {if(a_coordinates_end.size() <= 1){a_coordinates_end.push_back(std::make_shared<scls::Formula>());}(*a_coordinates_end[1].get())=formula;};
        inline scls::Formula* x() const {if(a_coordinates.size() <= 0){return 0;} return a_coordinates[0].get();};
        inline scls::Formula* x_end() const {if(a_coordinates_end.size() <= 0){return 0;} return a_coordinates_end[0].get();};
        inline double x_end_to_double() const{if(x_end() == 0){return 0;}return x_end()->value(0).real().to_double();};
        inline double x_to_double() const{if(x() == 0){return 0;}return x()->value(0).real().to_double();};
        inline scls::Formula* y() const {if(a_coordinates.size() <= 1){return 0;} return a_coordinates[1].get();};
        inline scls::Formula* y_end() const {if(a_coordinates_end.size() <= 1){return 0;} return a_coordinates_end[1].get();};
        inline double y_end_to_double() const{if(y_end() == 0){return 0;}return y_end()->value(0).real().to_double();};
        inline double y_to_double() const{if(y() == 0){return 0;}return y()->value(0).real().to_double();};
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
        inline scls::Color color() const {return a_color;};
        inline scls::GUI_Text* connected_object()const{return a_connected_object.lock().get();};
        inline std::vector<std::shared_ptr<scls::Formula>>& coordinates(){return a_coordinates;};
        inline std::vector<std::shared_ptr<scls::Formula>>& coordinates_end(){return a_coordinates_end;};
        inline scls::Fraction drawing_proportion() const {return a_drawing_proportion;};
        inline std::string name() const {return a_name;};
        inline void set_color(scls::Color new_color){a_color = new_color;};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_drawing_proportion(scls::Fraction new_drawing_proportion){a_drawing_proportion=new_drawing_proportion;};
        inline void set_name(std::string new_name){a_name = new_name;if(connected_object() != 0){connected_object()->set_text(type_name() + std::string(" ") + a_name);}};
        inline void set_type(Vector_Type new_type){a_type=new_type;};
        inline Vector_Type type() const {return a_type;};
        inline std::string type_name(bool capitalise_first_letter) const {if(capitalise_first_letter){if(a_type == Vector_Type::VT_Point){return std::string("Point");}return std::string("Vecteur");}if(a_type == Vector_Type::VT_Point){return std::string("point");}return std::string("vecteur");};
        inline std::string type_name() const {return type_name(true);};

    private:
        // Connected object to this vector
        std::weak_ptr<scls::GUI_Text> a_connected_object = std::weak_ptr<scls::GUI_Text>();

        // Color of the vector
        scls::Color a_color = scls::Color(0, 0, 0);
        // Coordinates of the vector
        std::vector<std::shared_ptr<scls::Formula>> a_coordinates;
        std::vector<std::shared_ptr<scls::Formula>> a_coordinates_end;
        // Drawing proportion
        scls::Fraction a_drawing_proportion = 1;
        // Name of the vector
        std::string a_name;
        // Type of the vector
        Vector_Type a_type = Vector_Type::VT_Vector;

        // Last norm of the vector
        scls::Formula a_last_norm; bool a_last_norm_calculated = false;
    };

    //******************
    //
    // The base of all the next class
    //
    //******************

    class __Graphic_Object_Base {
        // Class representating a base object to display in a graphic
    public:

        // Collision in a graphic object
        enum Graphic_Collision_Type {GCT_Circle, GCT_Line, GCT_Rect};
        class Graphic_Collision {
            // Class representating a collision in a graphic object
        public:

            // Base of a collision
            struct Collision {
                // If the collision happens or not
                bool happens = false;

                // Other collided object
                std::shared_ptr<Collision> other_collision;
                std::weak_ptr<__Graphic_Object_Base> __other_object;
                template <typename T = __Graphic_Object_Base> T* other_object() const {return reinterpret_cast<T*>(__other_object.lock().get());};

                // Acceleration generated by the force
                scls::Point_2D acceleration;
                // Type of the collision
                Graphic_Collision_Type type = Graphic_Collision_Type::GCT_Rect;
            };

            // Datas for a circle collision
            struct Collision_Circle : public Collision {
                // Collision_Circle constructor
                Collision_Circle(){type = Graphic_Collision_Type::GCT_Circle;};

                // Angle of the collision
                double angle;
            };

            // Datas for a rect collision
            struct Collision_Rect_Rect : public Collision {
                // Possible side for collision
                #define PLEOS_PHYSIC_RECT_COLLISION_BOTTOM 3
                #define PLEOS_PHYSIC_RECT_COLLISION_LEFT 2
                #define PLEOS_PHYSIC_RECT_COLLISION_RIGHT 4
                #define PLEOS_PHYSIC_RECT_COLLISION_TOP 1

                // Distance between the colliding side
                double distance;
                // Sides of the collision
                bool side_bottom = false;
                bool side_left = false;
                bool side_right = false;
                bool side_top = false;
            };

            // Graphic_Collision constructor
            Graphic_Collision(std::weak_ptr<__Graphic_Object_Base> attached_object, std::weak_ptr<scls::Transform_Object_2D> attached_transform):a_attached_object(attached_object),a_attached_transform(attached_transform){};

            // Getters and setters
            inline __Graphic_Object_Base* attached_object()const{return a_attached_object.lock().get();};
            inline std::weak_ptr<__Graphic_Object_Base> attached_object_weak_ptr()const{return a_attached_object;};
            inline scls::Transform_Object_2D* attached_transform()const{return a_attached_transform.lock().get();};
            inline scls::Fraction direct_x_1() const {return a_x_1;};
            inline scls::Fraction direct_x_2() const {return a_x_2;};
            inline scls::Fraction direct_y_1() const {return a_y_1;};
            inline scls::Fraction direct_y_2() const {return a_y_2;};
            inline scls::Fraction height() const {return a_height;};
            inline scls::Fraction max_x() const {return attached_transform()->x() + a_width / 2;};
            inline scls::Fraction max_x_next() const {return attached_transform()->max_x_next();};
            inline scls::Fraction max_y() const {return attached_transform()->y() + a_height / 2;};
            inline scls::Fraction max_y_next() const {return attached_transform()->max_y_next();};
            inline scls::Fraction min_x() const {return attached_transform()->x() - a_width / 2;};
            inline scls::Fraction min_x_next() const {return attached_transform()->min_x_next();};
            inline scls::Fraction min_y() const {return attached_transform()->y() - a_height / 2;};
            inline scls::Fraction min_y_next() const {return attached_transform()->min_y_next();};
            inline scls::Point_2D position_next() const {return attached_transform()->position_next();};
            inline void set_height(scls::Fraction new_height) {a_height = new_height;};
            inline void set_type(Graphic_Collision_Type new_type){a_type = new_type;};
            inline void set_width(scls::Fraction new_width) {a_width = new_width;};
            inline void set_x_1(scls::Fraction new_x_1){a_x_1 = new_x_1;};
            inline void set_x_2(scls::Fraction new_x_2){a_x_2 = new_x_2;};
            inline void set_y_1(scls::Fraction new_y_1){a_y_1 = new_y_1;};
            inline void set_y_2(scls::Fraction new_y_2){a_y_2 = new_y_2;};
            inline Graphic_Collision_Type type()const{return a_type;};
            inline scls::Fraction width() const {return a_width;};
            inline scls::Fraction x_1() const {return attached_transform()->x() + a_x_1;};
            inline scls::Fraction x_2() const {return attached_transform()->x() + a_x_2;};
            inline scls::Fraction y_1() const {return attached_transform()->y() + a_y_1;};
            inline scls::Fraction y_2() const {return attached_transform()->y() + a_y_2;};

        private:
            // Attached object
            std::weak_ptr<__Graphic_Object_Base> a_attached_object;
            std::weak_ptr<scls::Transform_Object_2D> a_attached_transform;
            // Type of the collision
            Graphic_Collision_Type a_type = Graphic_Collision_Type::GCT_Rect;

            // Scale of the collision
            scls::Fraction a_height = 1;scls::Fraction a_width = 1;

            // Two points (needed for lines)
            scls::Fraction a_x_1 = 0;scls::Fraction a_y_1 = 0;
            scls::Fraction a_x_2 = 0;scls::Fraction a_y_2 = 0;
        };

        // __Graphic_Object_Base constructor
        __Graphic_Object_Base(){};
        __Graphic_Object_Base(scls::Point_2D position):a_transform(std::make_shared<scls::Transform_Object_2D>(position)){};

        // Returns a color adapted with the needed opacity
        scls::Color color_with_opacity(scls::Color needed_color)const{needed_color.set_alpha(static_cast<double>(needed_color.alpha()) * a_opacity);return needed_color;};

        // Getters and setters
        inline scls::Transform_Object_2D* attached_transform() const {return a_transform.get();};
        inline std::shared_ptr<scls::Transform_Object_2D> attached_transform_shared_ptr() const {return a_transform;};
        inline double opacity() const {return a_opacity;};
        inline void set_opacity(double new_opacity){a_opacity = new_opacity;};
        inline void set_should_delete(bool new_should_delete){a_should_delete = new_should_delete;};
        inline void set_unknowns(std::shared_ptr<scls::__Formula_Base::Unknowns_Container> new_unknowns){a_unknowns = new_unknowns;};
        inline bool should_delete() const {return a_should_delete;};

        //******************
        // Hierarchy functions
        //******************

        // Convert a collision to a collision circle
        static Graphic_Collision::Collision_Circle* collision_circle(Graphic_Collision::Collision* collision){if(collision == 0 || collision->type != Graphic_Collision_Type::GCT_Circle){return 0;} return reinterpret_cast<Graphic_Collision::Collision_Circle*>(collision);};

        // Function called when a collision occurs
        virtual void when_collision(Graphic_Collision::Collision* collision){};

    protected:

        // Returns the used unknowns
        inline scls::__Formula_Base::Unknowns_Container* unknowns() const {return a_unknowns.get();};

    private:
        // Transformation in the circle
        std::shared_ptr<scls::Transform_Object_2D> a_transform = std::make_shared<scls::Transform_Object_2D>();

        // Opacity of the object
        double a_opacity = 1.0;
        // If the object should be delete
        bool a_should_delete = false;
        // Unknowns in the object
        std::shared_ptr<scls::__Formula_Base::Unknowns_Container> a_unknowns;
    };
    typedef __Graphic_Object_Base::Graphic_Collision Graphic_Collision;
    typedef __Graphic_Object_Base::Graphic_Collision::Collision Collision;
    typedef __Graphic_Object_Base::Graphic_Collision_Type Graphic_Collision_Type;
    typedef __Graphic_Object_Base::Graphic_Collision::Collision_Circle Collision_Circle;
    typedef __Graphic_Object_Base::Graphic_Collision::Collision_Rect_Rect Collision_Rect_Rect;

    //******************
    //
    // The "Form_2D" class
    //
    //******************

    class Form_2D : public __Graphic_Object_Base {
        // Class representating a geometrical form in 2D
    public:
        // Link between the point
        struct Link {double drawing_proportion = 1;};

        // Form_2D constructor
        Form_2D(std::string name):__Graphic_Object_Base(),a_name(name){};

        // Adds an exclusion point to the form
        inline void add_exclusion_point(std::shared_ptr<Vector> point){a_exclusion_points.push_back(point);};
        // Adds a point to the form
        inline void add_point(std::shared_ptr<Vector> point){a_points.push_back(point);a_points_link.push_back(Link());};
        // Moves the points in teh form
        inline void move_x(scls::Fraction movement){for(int i = 0;i<static_cast<int>(a_points.size());i++){(*a_points[i].get()->x()) += movement;}};
        inline void move_y(scls::Fraction movement){for(int i = 0;i<static_cast<int>(a_points.size());i++){(*a_points[i].get()->y()) += movement;}};
        // Creates a new point to the form
        inline std::shared_ptr<Vector> new_point(scls::Fraction x, scls::Fraction y){std::shared_ptr<Vector>point=std::make_shared<Vector>(x, y);a_points.push_back(point);a_points_link.push_back(Link());return point;};
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
        inline std::vector<std::shared_ptr<Vector>>& points(){return a_points;};
        inline void set_border_color(scls::Color new_border_color){a_border_color=new_border_color;};
        inline void set_border_radius(int new_border_radius){a_border_radius=new_border_radius;};inline void set_border_width(int new_border_width){a_border_radius=new_border_width;};
        inline void set_color(scls::Color new_color){a_color=new_color;};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_link_drawing_proportion(int link, double new_proportion){if(link >=static_cast<int>(a_points_link.size())){return;}a_points_link[link].drawing_proportion = new_proportion;};
        inline void set_links_drawing_proportion(double new_proportion){for(int i = 0;i<static_cast<int>(a_points_link.size());i++){a_points_link[i].drawing_proportion = new_proportion;}};
        inline void set_name(std::string new_name){a_name = new_name;if(connected_object() != 0){connected_object()->set_text(std::string("Forme ") + a_name);}};

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

    class Circle : public __Graphic_Object_Base {
        // Class representating a geometrical circle
    public:
        // Circle constructor
        Circle(std::string name, Vector center, scls::__Formula_Base radius):__Graphic_Object_Base(center.to_point_2d()),a_name(name){set_radius(radius);};

        // Returns the radius of the circle
        virtual scls::Fraction radius(){return attached_transform()->scale_x() / 2;};
        virtual std::shared_ptr<scls::__Formula_Base> radius_formula_shared_ptr(){return ((*attached_transform()->scale_x_formula_shared_ptr().get()) / 2).formula_copy();};

        // Getters and setters
        inline scls::Formula angle_end() const {return a_angle_end;};
        inline scls::Formula angle_start() const {return a_angle_start;};
        inline scls::Color border_color() const {return a_border_color;};
        inline int border_radius() const {return a_border_radius;};
        inline Vector center() const {return attached_transform()->position();};
        inline scls::Color color() const {return a_color;};
        inline std::string name() const {return a_name;};
        inline void set_angle_end(scls::Formula new_angle_end){a_angle_end = new_angle_end;};
        inline void set_angle_start(scls::Formula new_angle_start){a_angle_start = new_angle_start;};
        inline void set_border_color(scls::Color new_border_color) {a_border_color = new_border_color;};
        inline void set_border_radius(int new_border_radius) {a_border_radius = new_border_radius;};
        inline void set_center(Vector new_center){attached_transform()->set_position(new_center.to_point_2d());};
        inline void set_color(scls::Color new_color) {a_color = new_color;};
        inline void set_radius(scls::__Formula_Base new_radius){attached_transform()->set_scale_x((new_radius * 2).formula_copy());}
    private:
        // Angle to start / end the drawing
        scls::Formula a_angle_end = 360;scls::Formula a_angle_start = 0;
        // Color of the border of the circle
        scls::Color a_border_color;
        // Radius of the border
        int a_border_radius = 0;
        // Color of the circle
        scls::Color a_color;
        // Name of the circle
        std::string a_name = std::string();
    };
}

#endif // PLEOS_MATHEMATICS_GEOMETRY
