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
#include "pleos_mathematics_geometry.h"

// Possibles operations at click
#define PLEOS_OPERATION_NOTHING 0
#define PLEOS_OPERATION_VECTOR 100
#define PLEOS_OPERATION_POINT 101
#define PLEOS_OPERATION_FORM 150

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

        // Returns the introduction for this function
        inline std::string introduction() const {return std::string("Nous avons la fonction ") + name() + std::string(" tel que ") + name() + std::string("(") + function_unknown + std::string(") = ") + function_formula.to_std_string() + std::string(" .");};

        // Connected object to this vector
        std::weak_ptr<scls::GUI_Text> a_connected_object = std::weak_ptr<scls::GUI_Text>();
        inline scls::GUI_Text* connected_object()const{return a_connected_object.lock().get();};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_formula(scls::Formula formula){function_formula = formula;};
        // Name of the function
        inline std::string name() const {return function_name;};
        inline void set_name(std::string new_name){function_name = new_name;if(connected_object()!=0){connected_object()->set_text(std::string("Fonction ") + new_name);}};

        // Studied things
        // Definition set of the function
        std::shared_ptr<scls::Set_Number> definition_set;
    };

    // Returns the definition set of a function
    scls::Set_Number function_definition_set(Function_Studied* current_function, std::string* redaction);
    // Returns the derivation of a function
    scls::Formula function_derivation(Function_Studied current_function, std::string& redaction);
    // Returns the image of a function
    scls::__Formula_Base::Formula function_image(Function_Studied current_function, scls::Formula needed_value, std::string& redaction);
    // Returns the limit of a function / polymonial in + infinity
    scls::Limit function_limit(Function_Studied current_function, scls::Limit needed_limit, std::string& redaction);
    // Returns the set of roots of a function
    scls::Set_Number function_roots(Function_Studied* current_function, std::string& redaction);
    // Returns the set of a positive function
    scls::Set_Number function_sign(Function_Studied current_function, std::string& redaction);

    //******************
    //
    // Sequences handling
    //
    //******************

    // Returns the interval of an sequence
    scls::Interval sequence_variation(Function_Studied current_function, std::string& redaction);

    //******************
    //
    // Graphic handling
    //
    //******************

    // Base object in a graphic
    class Graphic_Base_Object{
        // Class representating the base of an object in a graphic
    public:
        // Graphic_Base_Object constructor
        Graphic_Base_Object(){};

        // Getters and setters
        inline scls::Fraction height() const {return a_height;};
        inline scls::Fraction max_x() const {return a_x + a_width / 2;};
        inline scls::Fraction max_y() const {return a_y + a_height / 2;};
        inline scls::Fraction min_x() const {return a_x - a_width / 2;};
        inline scls::Fraction min_y() const {return a_y - a_height / 2;};
        inline bool moved_during_this_frame() const {return a_moved_this_frame;};
        inline void set_height(scls::Fraction new_height){a_height = new_height;a_moved_this_frame=true;};
        inline void set_this_object(std::weak_ptr<Graphic_Base_Object> new_this_object){a_this_object = new_this_object;};
        inline void set_width(scls::Fraction new_width){a_width = new_width;a_moved_this_frame=true;};
        inline void set_x(scls::Fraction new_x){a_x = new_x;a_moved_this_frame=true;};
        inline void set_y(scls::Fraction new_y){a_y = new_y;a_moved_this_frame=true;};
        inline scls::Fraction width() const {return a_width;};
        inline scls::Fraction x() const {return a_x;};
        inline scls::Fraction y() const {return a_y;};

    protected:
        // If the object has been moved during this frame
        bool a_moved_this_frame = true;
        // Position of the object in the graphic
        scls::Fraction a_x;scls::Fraction a_y;
        // Size of the object in the graphic
        scls::Fraction a_height;scls::Fraction a_width;

        // This object
        std::weak_ptr<Graphic_Base_Object> a_this_object;
    };

    class Graphic {
        // Class representating a graphic for one (or a lot) of functions
    public:

        // Needed fragment shader for the function
        static std::string graphic_function_fragment_shader(scls::Formula needed_formula);

        // Base of the graphic
        struct __Graphic_Base {int a_function_number = 0;double a_height = -1;scls::Fraction a_middle_x = 0;scls::Fraction a_middle_y = 0;double a_pixel_by_case_x = 100;double a_pixel_by_case_y = 100;double a_width = -1;};

        //******************
        //
        // The Graphic_Function handling
        //
        //******************

        class Graphic_Function {
            // Class representating a function for a graphic

        public:

            // Graphic_Function constructor
            Graphic_Function(std::shared_ptr<Function_Studied> function_studied);

            // Getters and setters
            inline scls::Set_Number* definition_set(){return a_function_studied.get()->definition_set.get();};
            inline scls::Formula& formula(){return a_function_studied.get()->function_formula;};
            inline scls::Fraction middle_x() const {return a_graphic_base.get()->a_middle_x;};
            inline scls::Fraction middle_y() const {return a_graphic_base.get()->a_middle_y;};
            inline double pixel_by_case_x() const {return a_graphic_base.get()->a_pixel_by_case_x;};
            inline double pixel_by_case_y() const {return a_graphic_base.get()->a_pixel_by_case_y;};
            inline void set_graphic_base(std::shared_ptr<__Graphic_Base> base) {a_graphic_base=base;};

        private:
            // Formula of the function
            std::shared_ptr<Function_Studied> a_function_studied;
            // Datas about the plane
            std::shared_ptr<__Graphic_Base> a_graphic_base;
        };

        // Text in a graphic
        struct Graphic_Text : public __Graphic_Object_Base{
            std::string content;std::shared_ptr<scls::Text_Style> style;scls::Fraction x = 0;scls::Fraction y = 0;

            // Moves an object to a position, from an another position
            inline void move_from_to(Vector pos_1, Vector pos_2, double proportion){Vector new_pos = pos_1 + (pos_2 - pos_1) * proportion;x = new_pos.x()->value(0).real();y = new_pos.y()->value(0).real();};
            // Returns the position of the text
            inline Vector position() const {return Vector("v", x, y);};
        };

        // Graphic constructor
        Graphic(){a_style.get()->set_background_color(scls::Color(0, 0, 0, 0));};

        // Sets the middle of the base
        inline void set_middle(double middle_x, double middle_y){a_graphic_base.get()->a_middle_x = middle_x;a_graphic_base.get()->a_middle_y = middle_y;};
        // Sets the scale of the base
        inline void set_scale(double width, double height){a_graphic_base.get()->a_height = height;a_graphic_base.get()->a_width = width;};

        // Annoying functions to draw the image
        int graphic_x_to_pixel_x(double x, int needed_width);
        int graphic_x_to_pixel_x(double x, std::shared_ptr<scls::Image>& needed_image){return graphic_x_to_pixel_x(x, needed_image.get()->width());};
        int graphic_y_to_pixel_y(double y, int needed_height);
        int graphic_y_to_pixel_y(double y, std::shared_ptr<scls::Image>& needed_image){return graphic_y_to_pixel_y(y, needed_image.get()->height());};
        int graphic_y_to_pixel_y_inversed(double y, int needed_height);
        int graphic_y_to_pixel_y_inversed(double y, std::shared_ptr<scls::Image>& needed_image){return graphic_y_to_pixel_y_inversed(y, needed_image.get()->height());};
        scls::Fraction pixel_x_to_graphic_x(int x, int needed_width);
        scls::Fraction pixel_x_to_graphic_x(int x, std::shared_ptr<scls::Image>& needed_image){return pixel_x_to_graphic_x(x, needed_image.get()->width());};
        scls::Fraction pixel_y_to_graphic_y(int y, int needed_height);
        scls::Fraction pixel_y_to_graphic_y(int y, std::shared_ptr<scls::Image>& needed_image){return pixel_y_to_graphic_y(y, needed_image.get()->height());};
        // Draw the bases of the image
        void image_draw_base(std::shared_ptr<scls::Image> to_return);
        // Draw a function on the image
        void image_draw_function(std::shared_ptr<scls::Image> to_return, std::shared_ptr<Graphic_Function> needed_function, std::vector<scls::Fraction>& screen_pos);

        // Adds a function to the graphic
        void add_function(std::shared_ptr<Function_Studied> function_studied);
        // Resets the object
        inline void reset(){a_forms_2d.clear();a_functions.clear();a_points.clear();a_vectors.clear();};
        // Returns the image of the graphic
        std::shared_ptr<scls::Image> to_image(int width_in_pixel, int height_in_pixel);

        // Handle circles
        // Adds a circle to the graphic
        inline std::shared_ptr<Circle>* add_circle(std::string circle_name, Vector center, scls::Formula radius){a_circles.push_back(std::make_shared<Circle>(circle_name, center, radius));return &a_circles[a_circles.size() - 1];};
        // Removes circle from the graphic
        inline std::shared_ptr<Circle> remove_circle(std::string circle_name){for(int i = 0;i<static_cast<int>(a_circles.size());i++){if(a_circles[i].get()->name()==circle_name){std::shared_ptr<Circle> temp = a_circles[i];a_circles.erase(a_circles.begin() + i);return temp;} }return std::shared_ptr<Circle>();};

        // Handle forms
        // Adds a forms to the graphic
        inline void add_form(std::shared_ptr<Form_2D> needed_formd_2d){a_forms_2d.push_back(needed_formd_2d);};
        // Draws a form on the graphic
        void draw_form(Form_2D* needed_form, std::shared_ptr<scls::Image> to_return);
        // Returns a form by its name
        inline std::shared_ptr<Form_2D> form_shared_ptr(std::string form_name){for(int i = 0;i<static_cast<int>(a_forms_2d.size());i++){if(a_forms_2d[i].get()->name() == form_name){return a_forms_2d[i];}}return std::shared_ptr<Form_2D>();};
        inline Form_2D* form(std::string form_name){return form_shared_ptr(form_name).get();};
        // Creates and returns a form
        inline std::shared_ptr<Form_2D> new_form(std::string name){std::shared_ptr<Form_2D>to_return=std::make_shared<Form_2D>(name);add_form(to_return);return to_return;};
        inline std::shared_ptr<Form_2D> new_form(std::string name, std::string points){std::shared_ptr<Form_2D>needed_form=new_form(name);set_form_points(needed_form.get(), points);return needed_form;};
        // Creates and returns a form from a face
        inline std::shared_ptr<Form_2D> new_form_from_face(std::string name, scls::model_maker::Face* face){
            std::shared_ptr<Form_2D>to_return=new_form(name);

            // Add the points
            for(int i = 0;i<static_cast<int>(face->points().size());i++) {
                std::string current_name = name + std::string("-") + std::to_string(i);
                std::shared_ptr<Vector> current_vector = add_point(current_name, face->points()[i].get());
                to_return.get()->add_point(current_vector);
            }
            // Add the exclusion points
            std::string final_points = std::string();
            for(int i = 0;i<static_cast<int>(face->exclusion_points().size());i++) {
                std::string current_name = name + std::string("-") + std::to_string(i);
                std::shared_ptr<Vector> current_vector = add_point(current_name, face->exclusion_points()[i].get());
                to_return.get()->add_exclusion_point(current_vector);
            }

            return to_return;
        };
        // Sets the points in a form of the graphic
        void set_form_points(Form_2D* needed_form, std::string points){std::vector<std::string> cutted = scls::cut_string(points, ";");for(int i = 0;i<static_cast<int>(cutted.size());i++){needed_form->add_point(point_shared_ptr(cutted[i]));}};
        void set_form_points(std::shared_ptr<Form_2D> needed_form, std::string points){set_form_points(needed_form.get(), points);};

        // Creates and returns a triangle (and its point)
        std::shared_ptr<Form_2D> new_triangle(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2, scls::Fraction x_3, scls::Fraction y_3);

        // Handle lines
        // Draw a line between two points
        void draw_line(Vector* point_1, Vector* point_2, scls::Color color, double width, double proportion, std::shared_ptr<scls::Image> to_return);
        // Creates and returns a line (and its points)
        std::shared_ptr<Form_2D> new_line(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2);

        // Handle points
        // Adds a points to the graphic
        inline void add_point(std::shared_ptr<Vector> needed_point){a_points.push_back(needed_point);};
        inline void add_point(Vector needed_point){add_point(std::make_shared<Vector>(needed_point));};
        inline std::shared_ptr<Vector> add_point(std::string name, scls::model_maker::Point* needed_point){add_point(std::make_shared<Vector>(name, scls::Fraction(needed_point->x() * 100000.0, 100000.0), scls::Fraction(needed_point->z() * 100000.0, 100000.0)));return a_points.at(a_points.size() - 1);};
        // Returns a point by his name
        inline std::shared_ptr<Vector> point_shared_ptr(std::string point_name){for(int i = 0;i<static_cast<int>(a_points.size());i++){if(a_points[i].get()->name() == point_name){return a_points[i];}}return std::shared_ptr<Vector>();};
        inline Vector* point(std::string point_name){return point_shared_ptr(point_name).get();};
        // Creates and returns a new point in the graphic
        inline std::shared_ptr<Vector> new_point(std::string name, scls::Fraction x, scls::Fraction y){std::shared_ptr<Vector>needed=std::make_shared<Vector>(name, x, y);add_point(needed);return needed;};

        // Handle texts
        // Creates and returns a new text in the graphic
        inline Graphic_Text* new_text(std::string content, scls::Fraction x, scls::Fraction y, std::shared_ptr<scls::Text_Style> style){a_texts.push_back(std::make_shared<Graphic_Text>());Graphic_Text* current_text=a_texts[a_texts.size() - 1].get();current_text->content = content;current_text->style = style;current_text->x=x;current_text->y=y;return current_text;};
        inline Graphic_Text* new_text(std::string content, Vector position, std::shared_ptr<scls::Text_Style> style){return new_text(content, position.x()->value(0).real(), position.y()->value(0).real(), style);};

        // Handle vectors
        // Adds a vector to the graphic
        inline void add_vector(Vector needed_vector){a_vectors.push_back(std::make_shared<Vector>(needed_vector));};
        // Draws a vector on the graphic
        void draw_vector(Vector* needed_point, std::shared_ptr<scls::Image> to_return);

        // Getters and setters
        inline bool draw_base() const {return a_draw_base;};
        inline bool draw_sub_bases() const {return a_draw_sub_bases;};
        inline scls::Fraction middle_x() const {return a_graphic_base.get()->a_middle_x;};
        inline void middle_x_add(scls::Fraction value) {a_graphic_base.get()->a_middle_x += value;};
        inline scls::Fraction middle_y() const {return a_graphic_base.get()->a_middle_y;};
        inline void middle_y_add(scls::Fraction value) {a_graphic_base.get()->a_middle_y += value;};
        inline double pixel_by_case_x() const {return a_graphic_base.get()->a_pixel_by_case_x;};
        inline void pixel_by_case_x_add(double value) const {a_graphic_base.get()->a_pixel_by_case_x += value;};
        inline double pixel_by_case_y() const {return a_graphic_base.get()->a_pixel_by_case_y;};
        inline void pixel_by_case_y_add(double value) const {a_graphic_base.get()->a_pixel_by_case_y += value;};
        inline void set_background_color(scls::Color new_background_color){a_style.get()->set_background_color(new_background_color);};
        inline void set_draw_base(bool new_draw_base) {a_draw_base = new_draw_base;};
        inline void set_draw_sub_bases(bool new_draw_sub_bases) {a_draw_sub_bases = new_draw_sub_bases;};
        inline scls::Text_Style* style() const {return a_style.get();};
        inline std::vector<std::shared_ptr<Graphic_Text>>& texts(){return a_texts;};
        inline std::vector<std::shared_ptr<Vector>>& vectors(){return a_vectors;};

        //******************
        //
        // Physic handling
        //
        //******************

        // Collision in a graphic object
        enum Graphic_Collision_Type {GCT_Line, GCT_Rect};
        class Graphic_Collision {
            // Class representating a collision in a graphic object
        public:

            // Datas for a rect collision
            struct Collision_Rect_Rect {
                // Possible side for collision
                #define PLEOS_PHYSIC_RECT_COLLISION_BOTTOM 3
                #define PLEOS_PHYSIC_RECT_COLLISION_LEFT 2
                #define PLEOS_PHYSIC_RECT_COLLISION_RIGHT 4
                #define PLEOS_PHYSIC_RECT_COLLISION_TOP 1

                // Distance between the colliding side
                double distance;
                // If the collision happens or not
                bool happens = false;
                // Sides of the collision
                bool side_bottom = false;
                bool side_left = false;
                bool side_right = false;
                bool side_top = false;
            };

            // Graphic_Collision constructor
            Graphic_Collision(std::weak_ptr<Graphic_Base_Object> attached_object):a_attached_object(attached_object){};

            // Getters and setters
            inline Graphic_Base_Object* attached_object()const{return a_attached_object.lock().get();};
            inline scls::Fraction direct_x_1() const {return a_x_1;};
            inline scls::Fraction direct_x_2() const {return a_x_2;};
            inline scls::Fraction direct_y_1() const {return a_y_1;};
            inline scls::Fraction direct_y_2() const {return a_y_2;};
            inline scls::Fraction max_x() const {return attached_object()->max_x();};
            inline scls::Fraction max_y() const {return attached_object()->max_y();};
            inline scls::Fraction min_x() const {return attached_object()->min_x();};
            inline scls::Fraction min_y() const {return attached_object()->min_y();};
            inline void set_type(Graphic_Collision_Type new_type){a_type = new_type;};
            inline void set_x_1(scls::Fraction new_x_1){a_x_1 = new_x_1;};
            inline void set_x_2(scls::Fraction new_x_2){a_x_2 = new_x_2;};
            inline void set_y_1(scls::Fraction new_y_1){a_y_1 = new_y_1;};
            inline void set_y_2(scls::Fraction new_y_2){a_y_2 = new_y_2;};
            inline Graphic_Collision_Type type()const{return a_type;};
            inline scls::Fraction x_1() const {return attached_object()->x() + a_x_1;};
            inline scls::Fraction x_2() const {return attached_object()->x() + a_x_2;};
            inline scls::Fraction y_1() const {return attached_object()->y() + a_y_1;};
            inline scls::Fraction y_2() const {return attached_object()->y() + a_y_2;};
        private:
            // Attached object
            std::weak_ptr<Graphic_Base_Object> a_attached_object;
            // Type of the collision
            Graphic_Collision_Type a_type = Graphic_Collision_Type::GCT_Rect;

            // Two points (needed for lines)
            scls::Fraction a_x_1 = 0;scls::Fraction a_y_1 = 0;
            scls::Fraction a_x_2 = 0;scls::Fraction a_y_2 = 0;
        };

        // Physic case
        struct Physic_Case{
            // Static objects in the case
            inline void delete_static_object_collision(Graphic_Collision* object){for(int i=0;i<static_cast<int>(static_objects_collisions.size());i++){if(static_objects_collisions[i].lock().get()==object){static_objects_collisions.erase(static_objects_collisions.begin() + i);break;}}};
            std::vector<std::weak_ptr<Graphic_Collision>> static_objects_collisions;
        };

        // Loads 100 X 100 physic map
        void load_physic_map(int middle_loading_x, int middle_loading_y);
        // Returns a physic case by its coordinates
        inline Physic_Case* physic_case(int x, int y){return &a_physic_map[(-a_physic_map_start_x) + x][(-a_physic_map_start_y) + y];};

        // Physic in a graphic object
        class Graphic_Physic {
            // Class representating a physic handler in a graphic object
        public:
            // Graphic_Physic constructor
            Graphic_Physic(std::weak_ptr<Graphic_Base_Object> attached_object):a_attached_object(attached_object){};

            // Add a collision to the graphic object
            void add_collision(){a_collisions.push_back(std::make_shared<Graphic_Collision>(a_attached_object));};
            // Adds a line collision to the graphic object
            void add_collision(scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2){
                a_collisions.push_back(std::make_shared<Graphic_Collision>(a_attached_object));
                std::shared_ptr<Graphic_Collision> collision=a_collisions.at(a_collisions.size()-1);
                collision.get()->set_x_1(x_1);collision.get()->set_y_1(y_1);
                collision.get()->set_x_2(x_2);collision.get()->set_y_2(y_2);
                collision.get()->set_type(Graphic_Collision_Type::GCT_Line);
            };
            // Checks if a collision occurs with an another collision
            void check_collision(Graphic_Collision* collision);

            // Accelerates the object
            inline void accelerate(scls::Point_3D acceleration){a_velocity += acceleration;};
            inline void accelerate_x(double acceleration){a_velocity.move_x(acceleration);};
            // Remove the X / Y velocity
            inline void remove_x_velocity(){a_velocity.set_x(0);};
            inline void remove_y_velocity(){a_velocity.set_y(0);};

            // Adds a value to the movement
            inline void add_next_movement(scls::Point_3D new_next_movement){a_next_movement += new_next_movement;};
            inline void add_next_movement_x(double new_next_movement){a_next_movement.move_x(new_next_movement);};
            inline void add_next_movement_y(double new_next_movement){a_next_movement.move_y(new_next_movement);};
            // Remove the X / Y movement
            inline void remove_x_movement(){a_next_movement.set_x(0);};
            inline void remove_y_movement(){a_next_movement.set_y(0);};

            // Getters and setters
            inline Graphic_Base_Object* attached_object()const{return a_attached_object.lock().get();};
            inline std::vector<std::shared_ptr<Graphic_Collision>>& collisions(){return a_collisions;};
            inline bool is_static() const {return a_static;};
            inline scls::Point_3D next_movement() const {return a_next_movement;};
            inline void set_next_movement(scls::Point_3D new_next_movement){a_next_movement = new_next_movement;};
            inline void set_next_movement(int new_next_movement){set_next_movement(scls::Point_3D(new_next_movement, new_next_movement, new_next_movement));};
            inline void set_static(bool new_static) {a_static = new_static;}
            inline void set_use_gravity(bool new_use_gravity){a_use_gravity = new_use_gravity;};
            inline bool use_gravity() const {return a_use_gravity;};
            inline std::vector<Physic_Case*>& used_physic_case(){return a_used_physic_case;};
            inline scls::Point_3D& velocity() {return a_velocity;};
        private:
            // Attached object
            std::weak_ptr<Graphic_Base_Object> a_attached_object;
            // Collisions in the physic object
            std::vector<std::shared_ptr<Graphic_Collision>> a_collisions;
            // Next movement of the object
            scls::Point_3D a_next_movement;
            // If the object is static or not
            bool a_static = true;
            // If the object use gravity or not
            bool a_use_gravity = true;
            // Used physic cases
            std::vector<Physic_Case*> a_used_physic_case;
            // Velocity of the object
            scls::Point_3D a_velocity;
        };

    private:

        // Things to draw
        bool a_draw_base = true;
        bool a_draw_sub_bases = true;
        // Style of the graphic
        std::shared_ptr<scls::Text_Style> a_style = std::make_shared<scls::Text_Style>();

        // Loaded function
        std::vector<std::shared_ptr<Graphic_Function>> a_functions;
        // Datas about the plane
        std::shared_ptr<__Graphic_Base> a_graphic_base = std::make_shared<__Graphic_Base>();

        // Geometrical objects
        std::vector<std::shared_ptr<Circle>> a_circles;
        // Geometrical forms 2D
        std::vector<std::shared_ptr<Form_2D>> a_forms_2d;
        // Geometrical point
        std::vector<std::shared_ptr<Vector>> a_points;
        // Text
        std::vector<std::shared_ptr<Graphic_Text>> a_texts;
        // Geometrical vectors
        std::vector<std::shared_ptr<Vector>> a_vectors;

        //******************
        //
        // Physic handling
        //
        //******************

        // Physic map
        std::vector<std::vector<Physic_Case>> a_physic_map;
        int a_physic_map_start_x = 0;int a_physic_map_start_y = 0;
    };
    typedef Graphic::Graphic_Collision::Collision_Rect_Rect Collision_Rect_Rect;

    class Graphic_Object : public scls::GUI_Object {
        // Class representating an object containing a graphic
    public:

        class Graphic_GUI_Object : public Graphic_Base_Object {
            // GUI object in a graphic
        public:
            // Graphic_GUI_Object constructor
            Graphic_GUI_Object(std::shared_ptr<scls::GUI_Object>needed_object):Graphic_Base_Object(),a_object(needed_object){needed_object.get()->set_ignore_click(true);needed_object.get()->set_texture_alignment(scls::T_Fill);};

            // Updates the event of the object
            virtual void update_event(){};
            // Soft reset the object
            virtual void soft_reset(){a_moved_this_frame=false;};

            // Move the GUI object
            void move(scls::Point_3D point){physic_object()->add_next_movement(point);};
            void __move(scls::Point_3D point){a_x += scls::Fraction::from_double(point.x());a_y += scls::Fraction::from_double(point.y());};
            // Scale the GUI Object
            void scale(Graphic* graphic, int image_width, int image_height);

            // Sets the physic object
            inline void set_physic_object(bool is_static){a_physic_object=std::make_shared<Graphic::Graphic_Physic>(a_this_object);a_physic_object.get()->set_static(is_static);a_physic_object.get()->set_use_gravity(!is_static);};

            // Getters and setters
            inline scls::Fraction max_x_next() const {return a_x + a_width / 2 + scls::Fraction::from_double(physic_object()->next_movement().x());};
            inline scls::Fraction max_y_next() const {return a_y + a_height / 2 + scls::Fraction::from_double(physic_object()->next_movement().y());};
            inline scls::Fraction min_x_next() const {return a_x - a_width / 2 + scls::Fraction::from_double(physic_object()->next_movement().x());};
            inline scls::Fraction min_y_next() const {return a_y - a_height / 2 + scls::Fraction::from_double(physic_object()->next_movement().y());};
            inline scls::GUI_Object* object() const {return a_object.get();};
            inline Graphic::Graphic_Physic* physic_object() const {return a_physic_object.get();};
            inline scls::_Window_Advanced_Struct* window_struct(){return &a_object.get()->window_struct();};
        private:
            // GUI object
            std::shared_ptr<scls::GUI_Object> a_object;

            //******************
            //
            // Physic handling
            //
            //******************

            // Physic object of this object
            std::shared_ptr<Graphic::Graphic_Physic> a_physic_object;
        };

        //******************
        //
        // Graphic_Object handling
        //
        //******************

        // Graphic constructor
        Graphic_Object(scls::_Window_Advanced_Struct& window, std::string name, std::weak_ptr<scls::GUI_Object> parent);

        // Function called after resizing the window
        virtual void after_resizing(){update_texture();};
        // Renders the object
        virtual void render(glm::vec3 scale_multiplier = glm::vec3(1, 1, 1));
        // Soft reset the object
        virtual void soft_reset(){a_created_vectors_at_click.clear();};
        // Updates the object
        virtual void update_event();
        virtual void update_texture();

        // Adds a function to the graphic
        inline void add_function(std::shared_ptr<Function_Studied> function_studied){a_datas.add_function(function_studied);};
        // Resets the object
        inline void reset(){a_datas.reset();};
        // Returns the image of the graphic
        inline std::shared_ptr<scls::Image> to_image(){return a_datas.to_image(width_in_pixel(), height_in_pixel());};
        inline bool use_image() const {return a_datas.draw_base() && a_datas.draw_sub_bases();};

        // Handle circles
        // Adds a circle to the graphic
        inline std::shared_ptr<Circle>* add_circle(std::string circle_name, Vector center, scls::Formula radius){return a_datas.add_circle(circle_name, center, radius);};
        // Removes circle from the graphic
        inline std::shared_ptr<Circle> remove_circle(std::string circle_name){return a_datas.remove_circle(circle_name);};

        // Handle forms
        // Adds a forms to the graphic
        inline void add_form(std::shared_ptr<Form_2D> needed_formd_2d){a_datas.add_form(needed_formd_2d);};
        // Draws a form on the graphic
        inline void draw_form(Form_2D* needed_form, std::shared_ptr<scls::Image> to_return){a_datas.draw_form(needed_form, to_return);};

        // Handle points
        // Adds a points to the graphic
        inline void add_point(std::shared_ptr<Vector> needed_point){a_datas.add_point(needed_point);};
        inline void add_point(Vector needed_point){a_datas.add_point(needed_point);};

        // Handle vectors
        // Adds a vector to the graphic
        inline void add_vector(Vector needed_vector){a_datas.add_vector(needed_vector);};
        inline void add_vector(scls::Fraction x, scls::Fraction y){a_datas.add_vector(Vector(std::string(), x, y));};
        // Draws a vector on the graphic
        inline void draw_vector(Vector* needed_point, std::shared_ptr<scls::Image> to_return){a_datas.draw_vector(needed_point, to_return);};

        // Handle other object
        // Creates a new GUI object
        template <typename T = Graphic_GUI_Object, typename G = scls::GUI_Object> std::shared_ptr<T> new_other_object(std::string other_name){std::shared_ptr<scls::GUI_Object>to_return=*new_object<G>(other_name);std::shared_ptr<T>object=std::make_shared<T>(to_return);object.get()->set_this_object(object);a_gui_objects.push_back(object);return object;};

        // Annoying functions to draw the image
        inline int graphic_x_to_pixel_x(double x, int needed_width){return a_datas.graphic_x_to_pixel_x(x, needed_width);};
        inline int graphic_x_to_pixel_x(double x, std::shared_ptr<scls::Image>& needed_image){return a_datas.graphic_x_to_pixel_x(x, needed_image);};
        inline int graphic_y_to_pixel_y(double y, int needed_height){return a_datas.graphic_y_to_pixel_y(y, needed_height);};
        inline int graphic_y_to_pixel_y(double y, std::shared_ptr<scls::Image>& needed_image){return a_datas.graphic_y_to_pixel_y(y, needed_image);};
        inline int graphic_y_to_pixel_y_inversed(double y, int needed_height){return a_datas.graphic_y_to_pixel_y_inversed(y, needed_height);};
        inline int graphic_y_to_pixel_y_inversed(double y, std::shared_ptr<scls::Image>& needed_image){return a_datas.graphic_y_to_pixel_y_inversed(y, needed_image);};
        inline scls::Fraction pixel_x_to_graphic_x(int x, int needed_width){return a_datas.pixel_x_to_graphic_x(x, needed_width);};
        inline scls::Fraction pixel_x_to_graphic_x(int x, std::shared_ptr<scls::Image>& needed_image){return a_datas.pixel_x_to_graphic_x(x, needed_image);};
        inline scls::Fraction pixel_y_to_graphic_y(int y, int needed_height){return a_datas.pixel_y_to_graphic_y(y, needed_height);};
        inline scls::Fraction pixel_y_to_graphic_y(int y, std::shared_ptr<scls::Image>& needed_image){return a_datas.pixel_y_to_graphic_y(y, needed_image);};

        // Getters and setters
        inline std::vector<std::shared_ptr<Vector>>& created_vectors_at_click(){return a_created_vectors_at_click;};
        inline Form_2D* current_form_2d() const {return a_current_form_2d.get();};
        inline bool draw_base() const {return a_datas.draw_base();};
        inline bool draw_sub_bases() const {return a_datas.draw_sub_bases();};
        inline Graphic* graphic() {return &a_datas;};
        inline std::vector<std::shared_ptr<Graphic_GUI_Object>>& gui_objects(){return a_gui_objects;};
        inline scls::Fraction middle_x() const {return a_datas.middle_x();};
        inline void middle_x_add(scls::Fraction value) {a_datas.middle_x_add(value);};
        inline scls::Fraction middle_y() const {return a_datas.middle_y();};
        inline void middle_y_add(scls::Fraction value) {a_datas.middle_y_add(value);};
        inline int operation_at_click() const {return a_operation_at_click;};
        inline double pixel_by_case_x() const {return a_datas.pixel_by_case_x();};
        inline void pixel_by_case_x_add(double value) const {a_datas.pixel_by_case_x_add(value);};
        inline double pixel_by_case_y() const {return a_datas.pixel_by_case_y();};
        inline void pixel_by_case_y_add(double value) const {a_datas.pixel_by_case_y_add(value);};
        inline void set_draw_base(bool new_draw_base) {a_datas.set_draw_base(new_draw_base);};
        inline void set_draw_sub_bases(bool new_draw_sub_bases) {a_datas.set_draw_sub_bases(new_draw_sub_bases);};
        inline void set_operation_at_click(int new_operation_at_click) {a_operation_at_click = new_operation_at_click;};
        inline std::vector<std::shared_ptr<Vector>>& vectors(){return a_datas.vectors();};

        //******************
        //
        // Physic handling
        //
        //******************

        // Loads 100 X 100 physic map
        inline void load_physic_map(int middle_loading_x, int middle_loading_y){a_datas.load_physic_map(middle_loading_x, middle_loading_y);};
        // Returns a physic case by its coordinates
        inline Graphic::Physic_Case* physic_case(int x, int y){return a_datas.physic_case(x, y);};
        // Updates the physic
        int update_physic();

    private:

        // Datas about the graphic
        Graphic a_datas;
        // Other object in the graphic
        std::vector<std::shared_ptr<Graphic_GUI_Object>> a_gui_objects;

        // Created objects at click
        std::vector<std::shared_ptr<Vector>> a_created_vectors_at_click;
        // Operation to do at click
        int a_operation_at_click = PLEOS_OPERATION_NOTHING;

        // Current object
        std::shared_ptr<Form_2D> a_current_form_2d;
    };
}

#endif // PLEOS_MATHEMATICS_FUNCTIONS
