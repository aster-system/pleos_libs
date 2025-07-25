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
#include "../pleos_environment.h"

// ID of each types of geometrical objects
#define PLEOS_MATHEMATICS_GEOMETRY_VECTOR 0
#define PLEOS_MATHEMATICS_GEOMETRY_POINT 10
#define PLEOS_MATHEMATICS_GEOMETRY_FORM 100

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // The base of all the next class
    //
    //******************

    // Base of the graphic
    struct __Graphic_Base {
        int a_function_number = 0;

        scls::__Formula_Base::Formula a_height = -1;scls::__Formula_Base::Formula a_width = -1;
        int a_height_in_pixel = -1;int a_width_in_pixel = -1;
        bool a_height_used = false;bool a_width_used = false;

        scls::Fraction a_middle_x =  0;
        scls::Fraction a_middle_y = 0;
        double a_pixel_by_case_x = 100;
        double a_pixel_by_case_y = 100;

        // Environment in the graphic
        std::shared_ptr<pleos::Text_Environment> a_environment;
        // Time in the graphic
        scls::Fraction a_time = 0;
    };

    class __Graphic_Object_Base {
        // Class representating a base object to display in a graphic
    public:

        //******************
        // Action handling
        //******************

        // Action than the robot can do
        struct Action {
            #define ACTION_MOVE 0

            // Action constructor
            Action(short action_type):type(action_type){};

            // Step of the action
            unsigned short step = 0;
            // Type of the action
            const short type = -1;
        };

        // Possible actions
        // Move action
        struct Action_Move : public Action {
            // Action_Move constructor
            Action_Move():Action(ACTION_MOVE){};

            // Coordinates to go
            scls::Fraction x_end = 0;scls::Fraction y_end = 0;
            inline scls::Point_2D position_end(){return scls::Point_2D(x_end, y_end);};
        };

        // Adds a move action
        void add_action_move(scls::Fraction x_end, scls::Fraction y_end){std::shared_ptr<Action_Move> action = std::make_shared<Action_Move>();action.get()->x_end=x_end;action.get()->y_end=y_end;a_actions.push_back(action);};
        void add_action_move(scls::Point_2D position){add_action_move(position.x(), position.y());};

        // Clears the actions
        void clear_actions(){a_actions.clear();};
        // Deletes the last action
        void delete_last_action(){a_actions.erase(a_actions.begin());};
        // Returns a last action
        Action* last_action() const {if(a_actions.size() <= 0){return 0;}return a_actions.at(0).get();};
        Action_Move* last_action_move() const {if(last_action() == 0 || last_action()->type != ACTION_MOVE){return 0;}return reinterpret_cast<Action_Move*>(last_action());};

        // Getters and setters
        inline std::vector<std::shared_ptr<Action>>& actions() {return a_actions;};

        //******************
        // Collision handling
        //******************

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

        // Getters and setters
        inline std::vector<Graphic_Collision::Collision*>& collisions_this_frame() {return a_collisions_this_frame;};

        //******************
        // The base of graphic objects
        //******************

        // __Graphic_Object_Base constructor
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, __Graphic_Object_Base* parent, scls::__Point_2D_Formula position);
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, __Graphic_Object_Base* parent, scls::Point_2D position);
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, __Graphic_Object_Base* parent);
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::__Point_2D_Formula position);
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::Point_2D position);
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name);
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, scls::Point_2D position);
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base);

        // Returns if the object contains a specific tag
        bool contains_tag(std::string tag);
        // Loads the tags
        void load_tags(std::string new_tags);

        // Returns an introduction of the object
        std::string introduction(scls::Textual_Math_Settings* settings) const;

        // Annoying functions to draw the image
        int graphic_x_to_pixel_x(double x);
        int graphic_x_to_pixel_x(scls::Fraction x);
        int graphic_y_to_pixel_y(double y);
        int graphic_y_to_pixel_y(scls::Fraction y);
        int graphic_y_to_pixel_y_inversed(double y);
        int graphic_y_to_pixel_y_inversed(scls::Fraction y);
        double pixel_by_case_x() const;
        double pixel_by_case_y() const;
        scls::Fraction pixel_x_to_graphic_x(int x);
        scls::Fraction pixel_y_to_graphic_y(int y);

        // Soft reset the object
        virtual void soft_reset(){a_collisions_this_frame.clear();a_transform.get()->soft_reset();};

        // Returns the needed XML text to generate this object
        virtual std::string to_xml_text();
        virtual std::string to_xml_text_base();
        virtual std::string to_xml_text_object_name();
        std::string to_xml_text_color(std::string attribute_name, scls::Color color);
        std::string to_xml_text_height(std::string attribute_name);
        std::string to_xml_text_height();
        std::string to_xml_text_name();
        std::string to_xml_text_rotation();
        std::string to_xml_text_tags();
        std::string to_xml_text_x();
        std::string to_xml_text_y();
        std::string to_xml_text_width(std::string attribute_name);
        std::string to_xml_text_width();

        // Returns a color adapted with the needed opacity
        scls::Color color_with_opacity(scls::Color needed_color)const{needed_color.set_alpha(static_cast<double>(needed_color.alpha()) * a_opacity);return needed_color;};

        // Moves the object
        inline void move_x(scls::Fraction movement){attached_transform()->move_x(movement);};
        inline void move_y(scls::Fraction movement){attached_transform()->move_y(movement);};

        // Returns the point in a point 3D
        inline scls::Point_3D to_point_3d_absolute() {return scls::Point_3D(absolute_x().to_double(), 0, absolute_y().to_double());};

        // Getters and setters
        inline scls::Fraction absolute_rotation() const {return a_transform.get()->absolute_rotation();};
        inline scls::Fraction absolute_x() const {return a_transform.get()->absolute_x();};
        inline scls::Fraction absolute_y() const {return a_transform.get()->absolute_y();};
        inline scls::Transform_Object_2D* attached_transform() const {return a_transform.get();};
        inline std::shared_ptr<scls::Transform_Object_2D> attached_transform_shared_ptr() const {return a_transform;};
        inline scls::GUI_Text* connected_object()const{return a_connected_object.lock().get();};
        inline std::shared_ptr<scls::GUI_Text> connected_object_shared_ptr()const{return a_connected_object.lock();};
        inline double drawing_proportion() const {return a_drawing_proportion;};
        inline __Graphic_Base* graphic_base() const {return a_graphic_base.lock().get();};
        inline std::shared_ptr<__Graphic_Base> graphic_base_shared_ptr() const {return a_graphic_base.lock();};
        inline scls::Fraction height() const {return a_transform.get()->scale_y();};
        inline int id() const {return a_id;};
        inline int index() const {return a_index;};
        inline scls::Fraction max_x() const {return a_transform.get()->max_x();};
        inline scls::Fraction max_y() const {return a_transform.get()->max_y();};
        inline scls::Fraction min_x() const {return a_transform.get()->min_x();};
        inline scls::Fraction min_y() const {return a_transform.get()->min_y();};
        inline std::string name() const {return a_name;};
        inline double opacity() const {return a_opacity;};
        inline scls::Point_2D position() const {return a_transform.get()->position();};
        inline void rotate(scls::Fraction needed_rotation) const {a_transform.get()->rotate(needed_rotation);}
        inline scls::Fraction rotation() const {return a_transform.get()->rotation();}
        inline scls::__Formula_Base::Formula rotation_formula() const {return a_transform.get()->rotation_formula();};
        inline bool save_to_xml_text() const {return a_save_to_xml_text;};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_drawing_proportion(double new_drawing_proportion) {a_drawing_proportion = new_drawing_proportion;};
        inline void set_height(scls::Fraction new_height){a_transform.get()->set_scale_y(new_height);};
        inline void set_index(int new_index){a_index=new_index;};
        inline void set_opacity(double new_opacity){a_opacity = new_opacity;};
        inline void set_position(scls::Point_2D new_position){attached_transform()->set_position(new_position);};
        inline void set_name(std::string new_name) {a_name = new_name;};
        inline void set_rotation(scls::__Fraction_Base new_rotation){a_transform.get()->set_rotation(new_rotation);};
        inline void set_rotation(scls::Fraction new_rotation){a_transform.get()->set_rotation(new_rotation);};
        inline void set_rotation(scls::__Formula_Base::Formula new_rotation){a_transform.get()->set_rotation(new_rotation);};
        inline void set_save_to_xml_text(bool new_save_to_xml_text) {a_save_to_xml_text = new_save_to_xml_text;};
        inline void set_should_delete(bool new_should_delete){a_should_delete = new_should_delete;};
        inline void set_unknowns(std::shared_ptr<scls::__Formula_Base::Unknowns_Container> new_unknowns){a_unknowns = new_unknowns;};
        inline bool should_delete() const {return a_should_delete;};
        inline void set_width(scls::Fraction new_width){a_transform.get()->set_scale_x(new_width);};
        inline void set_x(int new_x){a_transform.get()->set_x(new_x);};
        inline void set_x(scls::Fraction new_x){a_transform.get()->set_x(new_x);};
        inline void set_x(scls::__Fraction_Base new_x){a_transform.get()->set_x(new_x);};
        inline void set_x(scls::__Formula_Base::Formula new_x){a_transform.get()->set_x(new_x);};
        inline void set_y(int new_y){a_transform.get()->set_y(new_y);};
        inline void set_y(scls::Fraction new_y){a_transform.get()->set_y(new_y);};
        inline void set_y(scls::__Fraction_Base new_y){a_transform.get()->set_y(new_y);};
        inline void set_y(scls::__Formula_Base::Formula new_y){a_transform.get()->set_y(new_y);};
        inline std::vector<std::string>& tags() {return a_tags;};
        inline scls::Fraction width() const {return a_transform.get()->scale_x();};
        inline scls::Fraction x() const {return a_transform.get()->x();};
        inline scls::__Formula_Base::Formula x_formula() const {return a_transform.get()->x_formula();};
        inline scls::Fraction y() const {return a_transform.get()->y();};
        inline scls::__Formula_Base::Formula y_formula() const {return a_transform.get()->y_formula();};

        //******************
        // Hierarchy functions
        //******************

        // Convert a collision to a collision circle
        static Graphic_Collision::Collision_Circle* collision_circle(Graphic_Collision::Collision* collision){if(collision == 0 || collision->type != Graphic_Collision_Type::GCT_Circle){return 0;} return reinterpret_cast<Graphic_Collision::Collision_Circle*>(collision);};
        // Draws the object on an image
        virtual void draw_on_image(std::shared_ptr<scls::__Image_Base>){};

        // Function called when a collision occurs
        virtual void when_collision(Graphic_Collision::Collision* collision){a_collisions_this_frame.push_back(collision);};

    protected:

        // Returns the used unknowns
        inline scls::__Formula_Base::Unknowns_Container* unknowns() const {return a_unknowns.get();};

    private:
        //******************
        // Action handling
        //******************

        // Actions to do
        std::vector<std::shared_ptr<Action>> a_actions;

        //******************
        // Main attributes
        //******************

        // Connected object to this object
        std::weak_ptr<scls::GUI_Text> a_connected_object = std::weak_ptr<scls::GUI_Text>();
        // Graphic base
        std::weak_ptr<__Graphic_Base> a_graphic_base;
        // Transformation in the object
        std::shared_ptr<scls::Transform_Object_2D> a_transform = std::make_shared<scls::Transform_Object_2D>();

        // Drawing proportion of the object
        double a_drawing_proportion = 1;
        // ID (unique) of the object
        int a_id = -1;
        // Index of the object
        int a_index = 0;
        // Name of the object
        std::string a_name = std::string();
        // Opacity of the object
        double a_opacity = 1.0;
        // If the object should be save in XML
        bool a_save_to_xml_text = true;
        // If the object should be delete
        bool a_should_delete = false;
        // Tags of the object
        std::vector<std::string> a_tags;
        // Unknowns in the object
        std::shared_ptr<scls::__Formula_Base::Unknowns_Container> a_unknowns;

        // Collisions this frame
        std::vector<Graphic_Collision::Collision*> a_collisions_this_frame = std::vector<Graphic_Collision::Collision*>();
    };
    typedef __Graphic_Object_Base::Graphic_Collision Graphic_Collision;
    typedef __Graphic_Object_Base::Graphic_Collision::Collision Collision;
    typedef __Graphic_Object_Base::Graphic_Collision_Type Graphic_Collision_Type;
    typedef __Graphic_Object_Base::Graphic_Collision::Collision_Circle Collision_Circle;
    typedef __Graphic_Object_Base::Graphic_Collision::Collision_Rect_Rect Collision_Rect_Rect;

    //******************
    //
    // The "Point_2D" (or "Vector_2D") class
    //
    //******************

    /*enum Vector_Type{VT_Arrow, VT_Point, VT_Vector};
    class Point_2D : public __Graphic_Object_Base {
        // Class representating a mathematical vector (more mathematically advanced and geenralised than scls::Vector_3D)
    public:

        // Point_2D constructor
        Point_2D(std::string name, scls::Formula x, scls::Formula y):__Graphic_Object_Base(name){};
        Point_2D(scls::Point_2D point):__Graphic_Object_Base(std::string(), point){};
        Point_2D(scls::Formula x, scls::Formula y):Point_2D(std::string(), x, y){};
        Vector(std::string name):Point_2D(name, 0, 0){};
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
    //*/

    enum Vector_Type{VT_Arrow, VT_Point, VT_Vector};
    class Point_2D : public __Graphic_Object_Base {
        // Class representating a mathematical point 2D
    public:

        // Point_2D constructor
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::__Point_2D_Formula point):__Graphic_Object_Base(graphic_base, name, point){};
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::Point_2D point):__Graphic_Object_Base(graphic_base, name, point){};
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, scls::Point_2D point):Point_2D(graphic_base, std::string(), point){};
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::Formula x, scls::Formula y):Point_2D(graphic_base, name, scls::__Point_2D_Formula(x, y)){};
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, scls::Formula x, scls::Formula y):Point_2D(graphic_base, std::string(), x, y){};
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name):Point_2D(graphic_base, name, 0, 0){};
        Point_2D(std::weak_ptr<__Graphic_Base> graphic_base):Point_2D(graphic_base, std::string(), scls::Point_2D(0, 0)){};

        // Returns a vector 3D from a point
        static std::shared_ptr<Point_2D> from_point(std::shared_ptr<__Graphic_Base> graphic_base, scls::model_maker::Point* needed_point){return std::make_shared<Point_2D>(graphic_base, std::string(), scls::Fraction(needed_point->x() * 1000000.0, 1000000), scls::Fraction(needed_point->z() * 1000000.0, 1000000));};

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
        Form_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, __Graphic_Object_Base* parent):__Graphic_Object_Base(graphic_base, name, parent){};
        Form_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name):__Graphic_Object_Base(graphic_base, name){};

        // Draws the circle on an image
        virtual void draw_on_image(std::shared_ptr<scls::__Image_Base> image);

        // Adds an exclusion point to the form
        inline void add_exclusion_point(std::shared_ptr<Point_2D> point){a_exclusion_points.push_back(point);};
        // Adds a point to the form
        inline void add_point_external(std::shared_ptr<__Graphic_Object_Base> point){a_points.push_back(point);a_points_link.push_back(Link());};
        inline void add_point(std::shared_ptr<Point_2D> point){point.get()->attached_transform()->set_parent(attached_transform());add_point_external(point);};
        // Creates a new point to the form
        std::shared_ptr<Point_2D> new_point(scls::Fraction x, scls::Fraction y);

        // Returns a list of the points triangulated
        std::vector<std::shared_ptr<Point_2D>> triangulated_points();

        // Returns the introduction of the form 2D
        std::string introduction() const;
        // Returns the name of the type of the form
        std::string type_name(bool capitalise_first_letter) const;
        std::string type_name() const;

        // Getters and setters
        inline scls::Color border_color()const{return a_border_color;};
        inline int border_radius()const{return a_border_radius;};
        inline scls::Color color()const{return a_color;};
        inline std::vector<std::shared_ptr<__Graphic_Object_Base>>& exclusion_points(){return a_exclusion_points;};
        inline Link& last_link(){return a_points_link[a_points_link.size() - 1];};
        inline Link& link(int position){return a_points_link[position];};
        inline std::vector<std::shared_ptr<__Graphic_Object_Base>>& points(){return a_points;};
        inline void set_border_color(scls::Color new_border_color){a_border_color=new_border_color;};
        inline void set_border_radius(int new_border_radius){a_border_radius=new_border_radius;};inline void set_border_width(int new_border_width){a_border_radius=new_border_width;};
        inline void set_color(scls::Color new_color){a_color=new_color;};
        inline void set_link_drawing_proportion(int link, double new_proportion){if(link >=static_cast<int>(a_points_link.size())){return;}a_points_link[link].drawing_proportion = new_proportion;};
        inline void set_links_drawing_proportion(double new_proportion){for(int i = 0;i<static_cast<int>(a_points_link.size());i++){a_points_link[i].drawing_proportion = new_proportion;}};

        // Returns this object to an XML text
        virtual std::string to_xml_text();
        virtual std::string to_xml_text_object_name();
    private:
        // Color of the border of the form
        scls::Color a_border_color = scls::Color(255, 0, 0);
        // Radius of the border
        int a_border_radius = 6;
        // Color of the inner form
        scls::Color a_color = scls::Color(0, 255, 0);

        // Exclusions points in the circle
        std::vector<std::shared_ptr<__Graphic_Object_Base>> a_exclusion_points;
        // Points in the form
        std::vector<std::shared_ptr<__Graphic_Object_Base>> a_points;
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
        Circle(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, __Graphic_Object_Base* parent, scls::Point_2D center, scls::__Formula_Base radius_x, scls::__Formula_Base radius_y);
        Circle(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::Point_2D center, scls::__Formula_Base radius_x, scls::__Formula_Base radius_y);
        Circle(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, __Graphic_Object_Base* parent, scls::Point_2D center, scls::__Formula_Base radius);
        Circle(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::Point_2D center, scls::__Formula_Base radius);

        // Draws the circle on an image
        virtual void draw_on_image(std::shared_ptr<scls::__Image_Base> image);

        // Returns the radius of the circle
        virtual scls::Fraction radius_x(){return attached_transform()->absolute_scale_x() / 2;};
        virtual std::shared_ptr<scls::__Formula_Base> radius_x_formula_shared_ptr(){return ((*attached_transform()->scale_x_formula_shared_ptr().get()) / 2).formula_copy();};
        virtual scls::Fraction radius_y(){return attached_transform()->absolute_scale_y() / 2;};
        virtual std::shared_ptr<scls::__Formula_Base> radius_y_formula_shared_ptr(){return ((*attached_transform()->scale_y_formula_shared_ptr().get()) / 2).formula_copy();};

        // Returns the needed XML text to generate this object
        virtual std::string to_xml_text();
        std::string to_xml_text_angle_end();
        std::string to_xml_text_angle_start();
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
        inline void set_radius_x(scls::__Formula_Base new_radius_x){attached_transform()->set_scale_x((new_radius_x * 2).formula_copy());}
        inline void set_radius_y(scls::__Formula_Base new_radius_y){attached_transform()->set_scale_y((new_radius_y * 2).formula_copy());}
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
