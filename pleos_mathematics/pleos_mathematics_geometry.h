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
#include "../pleos_graphic_base.h"

// ID of each types of geometrical objects
#define PLEOS_MATHEMATICS_GEOMETRY_VECTOR 0
#define PLEOS_MATHEMATICS_GEOMETRY_POINT 10
#define PLEOS_MATHEMATICS_GEOMETRY_FORM 100

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // The "Point_2D" (or "Vector_2D") class
    //
    //******************

    enum Vector_Type{VT_Arrow, VT_Point, VT_Vector};
    class Point_2D : public __Graphic_Object_Base {
        // Class representating a mathematical point 2D
    public:

        // Point_2D constructor
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::__Point_2D_Formula point);
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::Point_2D point);
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, scls::Point_2D point);
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y);
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, scls::Formula x, scls::Formula y);
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name);
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base);

        // Returns a vector 3D from a point
        static std::shared_ptr<Point_2D> from_point(std::shared_ptr<__Graphic_Base> graphic_base, scls::model_maker::Point* needed_point){return std::make_shared<Point_2D>(graphic_base, std::string(), scls::Fraction(needed_point->x() * 1000000.0, 1000000), scls::Fraction(needed_point->z() * 1000000.0, 1000000));};

        // Returns the needed XML text to generate this object
        virtual std::string to_displayed_text();
        virtual std::string to_xml_text();
        virtual std::string to_xml_text_object_name();

        // Handle the vector
        void set_x_end(scls::Fraction new_x_end) {attached_transform()->set_scale_x(new_x_end);};
        void set_y_end(scls::Fraction new_y_end) {attached_transform()->set_scale_y(new_y_end);};
        scls::Fraction x_end() {return attached_transform()->x() - attached_transform()->scale_x();};
        scls::Fraction y_end() {return attached_transform()->y() - attached_transform()->scale_y();};

        // Getters and setters
        inline scls::Color color()const{return a_color;};
        inline void set_color(scls::Color new_color){a_color=new_color;};
        inline void set_type(Vector_Type new_type){a_type=new_type;};
        inline Vector_Type type() const {return a_type;};
    private:

        // Color of the point
        scls::Color a_color = scls::Color(255, 0, 0);
        // Type of the point
        Vector_Type a_type = Vector_Type::VT_Vector;
    };

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
        Form_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name):__Graphic_Object_Base(graphic_base, name){};

        // Draws the circle on an image
        virtual void draw_on_image(std::shared_ptr<scls::__Image_Base> image);

        // Adds an exclusion point to the form
        void add_exclusion_point(std::shared_ptr<Point_2D> point);
        // Adds a point to the form
        void add_point_external(std::shared_ptr<__Graphic_Object_Base> point);
        void add_point(std::shared_ptr<__Graphic_Object_Base> point);
        // Returns if the form contains a point or not
        bool contains_point(__Graphic_Object_Base* tested_point);
        // Creates a new point to the form
        std::shared_ptr<Point_2D> new_point(scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y);
        std::shared_ptr<Point_2D> new_point(scls::Point_2D point);

        // Returns a parameter by its name
        virtual std::string parameter(std::string parameter_name);
        // Sets a parameter by its name
        virtual void set_parameter(std::string parameter_name, std::string parameter_value, std::string parameter_value_name, double proportion);

        // Returns a list of the points triangulated
        void triangulate_points_external();
        std::vector<std::shared_ptr<Point_2D>> triangulated_points_external();

        // Returns the introduction of the form 2D
        std::string introduction() const;
        // Returns the name of the type of the form
        std::string type_name(bool capitalise_first_letter) const;
        std::string type_name() const;

        // Getters and setters
        inline scls::Color border_color()const{return a_border_color;};
        inline int border_radius()const{return a_border_radius;};
        inline __Graphic_Object_Base* cloned_from() const {return a_cloned_from.lock().get();};
        inline scls::Color color()const{return a_color;};
        inline std::vector<std::shared_ptr<__Graphic_Object_Base>>& exclusion_points(){return a_exclusion_points;};
        inline Link& last_link(){return a_points_link[a_points_link.size() - 1];};
        inline scls::model_maker::Face::__Triangulation_Datas* last_triangulation()const{return a_last_triangulation.get();};
        inline Link& link(int position){return a_points_link[position];};
        inline std::vector<std::shared_ptr<__Graphic_Object_Base>>& points(){return a_points;};
        inline void set_border_color(scls::Color new_border_color){a_border_color=new_border_color;};
        inline void set_border_radius(int new_border_radius){a_border_radius=new_border_radius;};inline void set_border_width(int new_border_width){a_border_radius=new_border_width;};
        inline void set_cloned_from(std::weak_ptr<__Graphic_Object_Base> new_cloned_from){a_cloned_from = new_cloned_from;};
        inline void set_color(scls::Color new_color){a_color=new_color;};
        inline void set_link_drawing_proportion(int link, double new_proportion){if(link >=static_cast<int>(a_points_link.size())){return;}a_points_link[link].drawing_proportion = new_proportion;};
        inline void set_links_drawing_proportion(double new_proportion){for(int i = 0;i<static_cast<int>(a_points_link.size());i++){a_points_link[i].drawing_proportion = new_proportion;}};
        inline void set_triangulate_while_drawing(bool new_triangulate_while_drawing){a_triangulate_while_drawing = new_triangulate_while_drawing;};

        // Returns this object to an XML text
        virtual std::string to_displayed_text();
        virtual std::string to_xml_text();
        virtual std::string to_xml_text_object_name();
        std::string to_xml_text_border_radius();

        //******************
        // Hierarchy functions
        //******************

        // Clones the form
        void clone(std::shared_ptr<Form_2D> needed_clone);

        // Function called when a new children is added
        //virtual void when_new_children(__Graphic_Object_Base* new_child){if(!contains_point(new_child)){add_point(new_child->this_object_shared_ptr());}};

        // Physic
        virtual int collision_height(){if(a_points.size() == 2){return 1 + std::ceil(std::max(a_points.at(0).get()->absolute_y(), a_points.at(1).get()->absolute_y()) - std::min(a_points.at(0).get()->absolute_y(), a_points.at(1).get()->absolute_y()));}return __Graphic_Object_Base::collision_height();};
        virtual int collision_width(){if(a_points.size() == 2){return 1 + std::ceil(std::max(a_points.at(0).get()->absolute_x(), a_points.at(1).get()->absolute_x()) - std::min(a_points.at(0).get()->absolute_x(), a_points.at(1).get()->absolute_x()));}return __Graphic_Object_Base::collision_width();};
        virtual int collision_x_start(){if(a_points.size() == 2){return std::floor(std::min(a_points.at(0).get()->absolute_x(), a_points.at(1).get()->absolute_x()));}return __Graphic_Object_Base::collision_x_start();};
        virtual int collision_y_start(){if(a_points.size() == 2){return std::floor(std::min(a_points.at(0).get()->absolute_y(), a_points.at(1).get()->absolute_y()));}return __Graphic_Object_Base::collision_y_start();};
    private:
        // Color of the border of the form
        scls::Color a_border_color = scls::Color(255, 0, 0);
        // Radius of the border
        int a_border_radius = 6;
        // Color of the inner form
        scls::Color a_color = scls::Color(0, 255, 0);

        // Exclusions points in the circle
        std::vector<std::shared_ptr<__Graphic_Object_Base>> a_exclusion_points;
        // Last triangulation of the form
        std::shared_ptr<scls::model_maker::Face::__Triangulation_Datas> a_last_triangulation;
        // Points in the form
        std::vector<std::shared_ptr<__Graphic_Object_Base>> a_points;
        // Links for the point
        std::vector<Link> a_points_link;
        // If the form must be triangualted at each drawing or not
        bool a_triangulate_while_drawing = true;

        // From where the object was clone
        std::weak_ptr<__Graphic_Object_Base> a_cloned_from;
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
        Circle(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y, scls::__Formula_Base::Formula radius_x, scls::__Formula_Base::Formula radius_y);

        // Draws the circle on an image
        virtual void draw_on_image(std::shared_ptr<scls::__Image_Base> image);

        // Returns the radius of the circle
        virtual scls::Fraction radius_x(){return attached_transform()->scale_x() / 2;};
        virtual scls::__Formula_Base::Formula radius_x_formula(){return attached_transform()->scale_x_formula() / 2;};
        virtual scls::Fraction radius_y(){return attached_transform()->scale_y() / 2;};
        virtual scls::__Formula_Base::Formula radius_y_formula(){return attached_transform()->scale_y_formula() / 2;};

        // Returns a parameter by its name
        virtual std::string parameter(std::string parameter);
        // Sets a parameter by its name
        virtual void set_parameter(std::string parameter_name, std::string parameter_value, std::string parameter_value_start, double proportion);

        // Returns the needed XML text to generate this object
        virtual std::string to_displayed_text();
        virtual std::string to_xml_text();
        std::string to_xml_text_angle_end();
        std::string to_xml_text_angle_start();
        std::string to_xml_text_border_radius();
        std::string to_xml_text_radius();
        std::string to_xml_text_radius_x();
        std::string to_xml_text_radius_y();
        virtual std::string to_xml_text_object_name();

        // Getters and setters
        inline scls::Formula angle_end() const {return a_angle_end;};
        inline scls::Formula angle_start() const {return a_angle_start;};
        inline scls::Color border_color() const {return a_border_color;};
        inline int border_radius() const {return a_border_radius;};
        inline scls::Point_2D center() const {return attached_transform()->absolute_position();};
        inline scls::Color color() const {return a_color;};
        inline void set_angle_end(scls::Formula new_angle_end){a_angle_end = new_angle_end;};
        inline void set_angle_start(scls::Formula new_angle_start){a_angle_start = new_angle_start;};
        inline void set_border_color(scls::Color new_border_color) {a_border_color = new_border_color;};
        inline void set_border_radius(int new_border_radius) {a_border_radius = new_border_radius;};
        inline void set_center(scls::Point_2D new_center){attached_transform()->set_position(new_center);};
        inline void set_color(scls::Color new_color) {a_color = new_color;};
        inline void set_radius(scls::__Formula_Base::Formula new_radius){set_radius_x(new_radius);set_radius_y(new_radius);}
        inline void set_radius_x(scls::__Formula_Base::Formula new_radius_x){attached_transform()->set_scale_x(new_radius_x * 2);}
        inline void set_radius_y(scls::__Formula_Base::Formula new_radius_y){attached_transform()->set_scale_y(new_radius_y * 2);}
    private:
        // Angle to start / end the drawing
        scls::Formula a_angle_end = 360;scls::Formula a_angle_start = 0;
        // Color of the border of the circle
        scls::Color a_border_color = scls::Color(0, 0, 0);
        // Radius of the border
        int a_border_radius = 5;
        // Color of the circle
        scls::Color a_color = scls::Color(255, 255, 255);
    };
}

#endif // PLEOS_MATHEMATICS_GEOMETRY
