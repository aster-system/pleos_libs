//******************
//
// pleos_graphic_base.h
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
// This file contains the basic object of the PLEOS graphic system.
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

#ifndef PLEOS_GRAPHIC_BASE
#define PLEOS_GRAPHIC_BASE

// Include SCLS Graphic Benoit
#include "pleos_environment.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // The base of all the next class
    //
    //******************

    // Base of the graphic
    struct Plane_Base {
        // Datas about the plane
        int graphic_x_to_pixel_x(double x, int needed_width);
        int graphic_x_to_pixel_x(double x, std::shared_ptr<scls::__Image_Base> needed_image);
        int graphic_y_to_pixel_y(double y, int needed_height);
        int graphic_y_to_pixel_y(double y, std::shared_ptr<scls::__Image_Base> needed_image);
        scls::Fraction pixel_x_to_graphic_x(int x, int needed_width);
        scls::Fraction pixel_x_to_graphic_x(int x, std::shared_ptr<scls::__Image_Base> needed_image);
        scls::Fraction pixel_y_to_graphic_y(int y, int needed_height);
        scls::Fraction pixel_y_to_graphic_y(int y, std::shared_ptr<scls::__Image_Base> needed_image);

        // Calculated datas
        double a_pixel_by_case_x = 100;
        double a_pixel_by_case_y = 100;

        // Coordinates of the base
        double a_middle_x = 0;
        double a_middle_y = 0;
        int a_y_offset = 0;

        // Size of the base
        double a_height = -1;double a_width = -1;
        int a_height_in_pixel = -1;int a_width_in_pixel = -1;
        bool a_height_used = false;bool a_width_used = false;
    };
    struct __Graphic_Base : public Plane_Base {
        int a_function_number = 0;

        // If the eco-mode is activated or not
        bool a_eco_mode = false;
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
            #define ACTION_CONTAINER -1
            #define ACTION_DELETE 0
            #define ACTION_LOOP 1
            #define ACTION_MOVE 2
            #define ACTION_ROTATE 3
            #define ACTION_SET_PARAMETER 4
            #define ACTION_STOP 5
            #define ACTION_STRUCTURE 6
            #define ACTION_WAIT 7
            #define ACTION_WAIT_UNTIL 8

            // Action constructor
            Action(short action_type):type(action_type){};

            // Soft-resets the ction
            virtual void soft_reset(){};

            // Returns the action to a XML text
            virtual std::string to_xml_text(std::string object_name);
            virtual std::string to_xml_text_name();
            std::string to_xml_text_object(std::string object_name);
            std::string to_xml_text_time() const;

            // Duration of the action
            double duration = 0;
            // Passed time at executing this action
            double passed_time = 0;
            // If the object should be save in XML
            bool save_to_xml_text = false;
            // Step of the action
            unsigned short step = 0;
            // Type of the action
            const short type = -1;
        };

        // Possible actions
        // Delete action
        struct Action_Delete : public Action {
            #define ACTION_DELETE_OBJECT 0
            #define ACTION_DELETE_PHYSIC 1

            // Action_Delete constructor
            Action_Delete():Action(ACTION_DELETE){};
            Action_Delete(char object):Action_Delete(){to_delete = object;};

            // Returns the action to a XML text
            virtual std::string to_xml_text_name();

            // To delete
            char to_delete = ACTION_DELETE_OBJECT;
        };
        // Move action
        struct Action_Move : public Action {
            // Action_Move constructor
            Action_Move():Action(ACTION_MOVE){};
            Action_Move(double needed_x, double needed_y, double needed_speed):Action_Move(){speed=needed_speed;x_end=needed_x;y_end=needed_y;};

            // Returns the action to a XML text
            virtual std::string to_xml_text(std::string object_name);
            virtual std::string to_xml_text_name();
            std::string to_xml_text_speed();
            std::string to_xml_text_x();
            std::string to_xml_text_y();

            // Coordinates to go
            double speed = 1.0;double x_end = 0;double y_end = 0;
            inline scls::Point_2D position_end(){return scls::Point_2D(x_end, y_end);};
        };
        // Rotate action
        struct Action_Rotate : public Action {
            // Action_Rotate constructor
            Action_Rotate():Action(ACTION_ROTATE){};
            Action_Rotate(double needed_rotation, double needed_speed):Action_Rotate(){rotation_end=needed_rotation;speed=needed_speed;};

            // Soft-resets the ction
            virtual void soft_reset(){Action::soft_reset();__rotated=0;};

            // Returns the action to a XML text
            virtual std::string to_xml_text(std::string object_name);
            virtual std::string to_xml_text_name();
            std::string to_xml_text_rotation();

            // Coordinates to go
            double speed = 1.0;
            double __rotated = 0;double rotation_end = 0;double rotation_start = 0;
        };
        // Set parameter action
        struct Action_Set_Parameter : public Action {
            // Action_Set_Parameter constructor
            Action_Set_Parameter():Action(ACTION_SET_PARAMETER){};
            Action_Set_Parameter(std::string needed_parameter_name, std::string needed_parameter_value, double needed_duration):Action_Set_Parameter(){parameter_name = needed_parameter_name;parameter_value = needed_parameter_value;duration = needed_duration;};

            // Returns the action to a XML text
            virtual std::string to_xml_text(std::string object_name);
            virtual std::string to_xml_text_name();
            std::string to_xml_text_parameter() const;
            std::string to_xml_text_value() const;

            // Parameter datas
            std::string parameter_name = std::string();
            std::string parameter_value = std::string();
            // Start parameter
            std::string parameter_start = std::string();
        };
        // Stop action
        struct Action_Stop : public Action {
            // Action_Stop constructor
            Action_Stop(double needed_duration):Action(ACTION_STOP){duration = needed_duration;};

            // Returns the action to a XML text
            virtual std::string to_xml_text_name();
        };
        // Wait action
        struct Action_Wait : public Action {
            // Action_Wait constructor
            Action_Wait():Action(ACTION_WAIT){};
            Action_Wait(double time):Action_Wait(){duration = time;};

            // Returns the action to a XML text
            virtual std::string to_xml_text(std::string object_name);
            virtual std::string to_xml_text_name();
            std::string to_xml_text_duration();
        };
        struct Action_Wait_Until : public Action {
            // Action_Wait_Until constructor
            Action_Wait_Until():Action(ACTION_WAIT_UNTIL){};
            Action_Wait_Until(double time):Action_Wait_Until(){duration = time;};

            // Returns the action to a XML text
            virtual std::string to_xml_text(std::string object_name);
            virtual std::string to_xml_text_name();
            std::string to_xml_text_duration();
        };

        // Structure action
        struct Action_Structure : public Action {
            // Action structure

            // Action_Structure constructor
            Action_Structure(short action_type):Action(action_type){};

            // Adds an action
            std::shared_ptr<Action> add_action(std::shared_ptr<Action> needed_action);
            // Clears the actions
            void clear_actions();
            // Deletes the last action
            void delete_last_action();

            // Go to the first / next action
            void go_to_first_action();
            void go_to_next_action();
            // If the current action is the end action
            bool is_end_action() const;
            // Returns a last action
            Action* last_action() const;
            short last_action_type() const;
            // Returns the next action
            Action* next_action() const;
            short next_action_type() const;

            // Returns the action to a XML text
            std::string to_xml_text_content();
            virtual std::string to_xml_text(std::string object_name);

            // Getters and setters
            inline std::vector<std::shared_ptr<Action>>& actions(){return a_actions;};
            inline int current_action() const {return a_current_action;};
        private:
            // Needed actions
            std::vector<std::shared_ptr<Action>> a_actions;
            // Current action
            int a_current_action = 0;
        };

        // Loop action
        struct Action_Loop : public Action_Structure {
            // Action loop

            // Action_Loop constructor
            Action_Loop():Action_Structure(ACTION_LOOP){};

            // Returns the action to a XML text
            virtual std::string to_xml_text_name();

            // Getters and setters
            inline int repetition() const {return a_repetition;};
            inline void set_repetition(int new_repetition){a_repetition = new_repetition;};
        private:
            // Needed repetition
            int a_repetition = 1;
        };

        // Container of actions
        struct Action_Container : public Action_Structure {
            // Action container

            // Action_Container constructor
            Action_Container():Action_Structure(ACTION_CONTAINER){};

            // Adds a delete action
            void add_action_delete(){std::shared_ptr<Action_Delete> action = std::make_shared<Action_Delete>();actions().push_back(action);};
            void add_action_delete_physic(){std::shared_ptr<Action_Delete> action = std::make_shared<Action_Delete>();action.get()->to_delete=ACTION_DELETE_PHYSIC;actions().push_back(action);};
            // Adds a move action
            std::shared_ptr<Action_Move> add_action_move(double x_end, double y_end, double needed_speed){std::shared_ptr<Action_Move> action = std::make_shared<Action_Move>();action.get()->x_end=x_end;action.get()->y_end=y_end;action.get()->speed = needed_speed;actions().push_back(action);return action;};
            std::shared_ptr<Action_Move> add_action_move(double x_end, double y_end){std::shared_ptr<Action_Move> action = std::make_shared<Action_Move>();action.get()->x_end=x_end;action.get()->y_end=y_end;actions().push_back(action);return action;};
            std::shared_ptr<Action_Move> add_action_move(scls::Point_2D position, double needed_speed){return add_action_move(position.x(), position.y(), needed_speed);};
            std::shared_ptr<Action_Move> add_action_move(scls::Point_2D position){return add_action_move(position.x(), position.y());};
            // Adds a set_parameter action
            void add_action_set_parameter(std::string parameter_name, std::string parameter_value, double duration){std::shared_ptr<Action_Set_Parameter> action = std::make_shared<Action_Set_Parameter>();action.get()->duration=duration;action.get()->parameter_name=parameter_name;action.get()->parameter_value=parameter_value;actions().push_back(action);};
            void add_action_set_parameter(std::string parameter_name, double parameter_value, double duration){add_action_set_parameter(parameter_name, std::to_string(parameter_value), duration);};
            void add_action_set_parameter(std::string parameter_name, double parameter_value){add_action_set_parameter(parameter_name, std::to_string(parameter_value), 0);};
            void add_action_set_parameter(std::string parameter_name, std::string parameter_value){add_action_set_parameter(parameter_name, parameter_value, 0);};
            // Adds a stop action
            void add_action_stop(){std::shared_ptr<Action_Stop> action = std::make_shared<Action_Stop>(0);actions().push_back(action);};
            // Adds a structure action
            void add_action_structure(std::shared_ptr<Action_Structure> loop){actions().push_back(loop);};
            // Adds a wait action
            void add_action_wait(double time_in_second){std::shared_ptr<Action_Wait> action = std::make_shared<Action_Wait>();action.get()->duration=time_in_second;actions().push_back(action);};

            // Returns a last action
            Action_Delete* last_action_delete() const;
            Action_Loop* last_action_loop() const;
            Action_Move* last_action_move() const;
            Action_Set_Parameter* last_action_set_parameter() const;
            Action_Stop* last_action_stop() const;
            Action_Wait* last_action_wait() const;
        };

        // Clears the actions
        void clear_actions();
        // Deletes the last action
        void delete_last_action();

        // Returns a last action
        Action* last_action() const;
        Action_Delete* last_action_delete() const;
        Action_Loop* last_action_loop() const;
        Action_Move* last_action_move() const;
        Action_Set_Parameter* last_action_set_parameter() const;
        Action_Stop* last_action_stop() const;
        short last_action_type() const;
        Action_Wait* last_action_wait() const;

        // Returns the next action
        Action* next_action() const;
        short next_action_type() const;

        // Getters and setters
        inline Action_Container* actions() {return a_actions.get();};
        inline std::vector<std::shared_ptr<Action>>& actions_list(){return a_actions.get()->actions();};

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
                // Collision constructor
                Collision(std::weak_ptr<Graphic_Collision> collision_parent):a_collision_parent(collision_parent){}

                // This object
                Graphic_Collision* collision_parent() const {return a_collision_parent.lock().get();};
                std::weak_ptr<Graphic_Collision> collision_parent_weak_ptr() const {return a_collision_parent;};
                __Graphic_Object_Base* object() const {return collision_parent()->attached_object();};

                // If the collision happens or not
                bool happens = false;

                // Other collided object
                Collision* other_collision(){return a_other_collision.lock().get();};
                std::weak_ptr<__Graphic_Object_Base> __other_object;
                template <typename T = __Graphic_Object_Base> T* other_object() const {return reinterpret_cast<T*>(__other_object.lock().get());};
                void set_other_collision(std::weak_ptr<Collision> new_other_collision){a_other_collision = new_other_collision;};

                // Acceleration generated by the force
                scls::Point_2D acceleration;
                // Type of the collision
                Graphic_Collision_Type type = Graphic_Collision_Type::GCT_Rect;

            private:
                // Parent collision of this object
                std::weak_ptr<Graphic_Collision> a_collision_parent;

                // Other collision object
                std::weak_ptr<Collision> a_other_collision;
            };

            // Datas for a circle collision
            struct Collision_Circle : public Collision {
                // Collision_Circle constructor
                Collision_Circle(std::weak_ptr<Graphic_Collision> collision_parent):Collision(collision_parent){type = Graphic_Collision_Type::GCT_Circle;};

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

                // Collision_Rect_Rect constructor
                Collision_Rect_Rect(std::weak_ptr<Graphic_Collision> collision_parent):Collision(collision_parent){type = Graphic_Collision_Type::GCT_Rect;};

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
            scls::Fraction absolute_height() const;
            scls::Point_2D absolute_scale() const;
            scls::Fraction absolute_width() const;
            double absolute_x() const;
            double absolute_x_1() const;
            double absolute_x_2() const;
            double absolute_y() const;
            double absolute_y_1() const;
            double absolute_y_2() const;
            __Graphic_Object_Base* attached_object()const;
            std::weak_ptr<__Graphic_Object_Base> attached_object_weak_ptr()const;
            scls::Transform_Object_2D* attached_transform()const;
            double direct_x_1() const;
            double direct_x_2() const;
            double direct_y_1() const;
            double direct_y_2() const;
            double max_absolute_x() const;
            double max_absolute_y() const;
            double min_absolute_x() const;
            double min_absolute_y() const;
            double max_absolute_x_next() const;
            double max_absolute_y_next() const;
            double min_absolute_x_next() const;
            double min_absolute_y_next() const;
            scls::Point_2D position_next() const;
            void set_type(Graphic_Collision_Type new_type);
            void set_x_1(scls::Fraction new_x_1);
            void set_x_2(scls::Fraction new_x_2);
            void set_y_1(scls::Fraction new_y_1);
            void set_y_2(scls::Fraction new_y_2);
            Graphic_Collision_Type type()const;
            double x_1() const;
            double x_2() const;
            double y_1() const;
            double y_2() const;

        private:
            // Attached object
            std::weak_ptr<__Graphic_Object_Base> a_attached_object;
            std::weak_ptr<scls::Transform_Object_2D> a_attached_transform;
            // Type of the collision
            Graphic_Collision_Type a_type = Graphic_Collision_Type::GCT_Rect;

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
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::Point_2D position);
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name);
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, scls::Point_2D position);
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base);

        // Adds a tag to the object
        void add_tag(std::string new_tag);
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

        // Returns a parameter by its name
        virtual std::string parameter(std::string parameter);
        // Sets a parameter by its name
        void set_parameter(std::string parameter_name, std::string parameter_value);
        virtual void set_parameter(std::string parameter_name, std::string parameter_value, std::string parameter_value_start, double proportion);

        // Sets the parent of the object
        void __delete_children(__Graphic_Object_Base* object_to_delete);
        void set_parent(std::weak_ptr<__Graphic_Object_Base> new_parent);
        void set_parent(__Graphic_Object_Base* new_parent);

        // Soft reset the object
        virtual void soft_reset(){a_collisions_this_frame.clear();a_transform.get()->soft_reset();};

        // Returns the needed XML text to generate this object
        virtual std::string to_displayed_text();
        virtual std::string to_xml_text();
        virtual std::string to_xml_text_base();
        virtual std::string to_xml_text_object_name();
        std::string to_xml_text_color(std::string attribute_name, scls::Color color);
        std::string to_xml_text_height(std::string attribute_name);
        std::string to_xml_text_height();
        std::string to_xml_text_name();
        std::string to_xml_text_opacity();
        std::string to_xml_text_parent();
        std::string to_xml_text_rotation();
        std::string to_xml_text_tags();
        std::string to_xml_text_x();
        std::string to_xml_text_y();
        std::string to_xml_text_width(std::string attribute_name);
        std::string to_xml_text_width();

        // Returns a color adapted with the needed opacity
        scls::Color color_with_absolute_opacity(scls::Color needed_color)const;
        scls::Color color_with_opacity(scls::Color needed_color)const;

        // Returns the distance between this object and a point
        inline double distance(scls::Point_2D needed_position) const {return absolute_position().distance(needed_position);};

        // Handles the settings of the object
        virtual void load_object_settings(scls::GUI_Object* gui_object);

        // Moves the object
        inline void move_x(scls::Fraction movement){attached_transform()->move_x(movement);};
        inline void move_y(scls::Fraction movement){attached_transform()->move_y(movement);};

        // Formula datas
        inline scls::__Formula height_formula() const {return scls::__Formula(scls::Complex(a_transform.get()->scale_y()));};
        inline scls::__Formula width_formula() const {return scls::__Formula(scls::Complex(a_transform.get()->scale_x()));};
        inline scls::__Formula y_formula() const {return scls::__Formula(scls::Complex(a_transform.get()->y()));};
        inline scls::__Formula x_formula() const {return scls::__Formula(scls::Complex(a_transform.get()->x()));};

        // Returns the point in a point 3D
        inline scls::Point_3D to_point_3d_absolute() {return scls::Point_3D(absolute_x(), 0, absolute_y());};

        // Getters and setters
        inline double absolute_height() const {return a_transform.get()->absolute_scale_y();};
        inline double absolute_opacity() const {if(parent() == 0){return opacity();}return parent()->absolute_opacity() * opacity();};
        inline scls::Point_2D absolute_position() const {return a_transform.get()->absolute_position();};
        inline double absolute_rotation() const {return a_transform.get()->absolute_rotation();};
        inline double absolute_width() const {return a_transform.get()->absolute_scale_x();};
        inline double absolute_x() const {return a_transform.get()->absolute_x();};
        inline double absolute_y() const {return a_transform.get()->absolute_y();};
        inline scls::Transform_Object_2D* attached_transform() const {return a_transform.get();};
        inline scls::Transform_Object_2D* attached_transform_parent() const {return a_transform.get()->parent();};
        inline std::shared_ptr<scls::Transform_Object_2D> attached_transform_shared_ptr() const {return a_transform;};
        inline std::vector<std::shared_ptr<__Graphic_Object_Base>>& children() {return a_children;};
        inline scls::GUI_Text* connected_object()const{return a_connected_object.lock().get();};
        inline std::shared_ptr<scls::GUI_Text> connected_object_shared_ptr()const{return a_connected_object.lock();};
        inline double deadline() const {return a_deadline;};
        inline double drawing_proportion() const {return a_drawing_proportion;};
        inline __Graphic_Base* graphic_base() const {return a_graphic_base.lock().get();};
        inline std::shared_ptr<__Graphic_Base> graphic_base_shared_ptr() const {return a_graphic_base.lock();};
        inline double height() const {return a_transform.get()->scale_y();};
        inline int id() const {return a_id;};
        inline int index() const {return a_index;};
        inline double live_time() const {return a_live_time;};
        inline double max_absolute_x() const {return a_transform.get()->max_absolute_x();};
        inline double max_absolute_x_next() const {return a_transform.get()->max_absolute_x_next();};
        inline double max_absolute_y() const {return a_transform.get()->max_absolute_y();};
        inline double max_absolute_y_next() const {return a_transform.get()->max_absolute_y_next();};
        inline double min_absolute_x() const {return a_transform.get()->min_absolute_x();};
        inline double min_absolute_x_next() const {return a_transform.get()->min_absolute_x_next();};
        inline double min_absolute_y() const {return a_transform.get()->min_absolute_y();};
        inline double min_absolute_y_next() const {return a_transform.get()->min_absolute_y_next();};
        inline double max_x() const {return a_transform.get()->max_x();};
        inline double max_y() const {return a_transform.get()->max_y();};
        inline double min_x() const {return a_transform.get()->min_x();};
        inline double min_y() const {return a_transform.get()->min_y();};
        inline std::string name() const {return a_name;};
        inline std::string object_name() const {return a_object_name;};
        inline double opacity() const {return a_opacity;};
        inline __Graphic_Object_Base* parent() const {return a_parent.lock().get();};
        inline scls::Point_2D position() const {return a_transform.get()->position();};
        inline void rotate(scls::Fraction needed_rotation) const {a_transform.get()->rotate(needed_rotation);}
        inline scls::Fraction rotation() const {return a_transform.get()->rotation();}
        inline bool save_to_xml_text() const {return a_save_to_xml_text;};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_deadline(double new_dead_line){a_deadline = new_dead_line;if(should_delete()){when_should_delete();}};
        inline void set_drawing_proportion(double new_drawing_proportion) {a_drawing_proportion = new_drawing_proportion;};
        inline void set_height(double new_height){a_transform.get()->set_scale_y(new_height);};
        inline void set_index(int new_index){a_index=new_index;};
        inline void set_live_time(double new_live_time){a_live_time = new_live_time;};
        inline void set_object_name(std::string new_object_name){a_object_name = new_object_name;};
        inline void set_opacity(double new_opacity){a_opacity = new_opacity;};
        inline void set_position(scls::Point_2D_Formula new_position){attached_transform()->set_position(new_position);};
        inline void set_name(std::string new_name) {a_name = new_name;};
        inline void set_rotation(double new_rotation){a_transform.get()->set_rotation(scls::Fraction::from_double(new_rotation));};
        inline void set_rotation(scls::__Fraction_Base new_rotation){a_transform.get()->set_rotation(new_rotation);};
        inline void set_rotation(scls::Fraction new_rotation){a_transform.get()->set_rotation(new_rotation);};
        inline void set_save_to_xml_text(bool new_save_to_xml_text) {a_save_to_xml_text = new_save_to_xml_text;};
        inline void set_should_delete(bool new_should_delete){a_should_delete = new_should_delete;if(should_delete()){when_should_delete();}};
        inline void set_this_object(std::weak_ptr<__Graphic_Object_Base> new_this_object){a_this_object=new_this_object;};
        inline void set_unknowns(std::shared_ptr<scls::__Formula_Base::Unknowns_Container> new_unknowns){a_unknowns = new_unknowns;a_transform.get()->set_unknowns(new_unknowns);};
        inline void set_velocity(scls::Point_2D new_velocity){a_transform.get()->set_velocity(new_velocity);};
        inline bool should_delete() const {return a_should_delete || (a_deadline != -1 && a_live_time >= a_deadline);};
        inline void set_width(double new_width){a_transform.get()->set_scale_x(new_width);};
        inline void set_x(double new_x){a_transform.get()->set_x(new_x);};
        inline void set_y(double new_y){a_transform.get()->set_y(new_y);};
        inline std::vector<std::string>& tags() {return a_tags;};
        inline std::shared_ptr<__Graphic_Object_Base> this_object_shared_ptr() const {return a_this_object.lock();};
        inline scls::Point_2D velocity() const {return a_transform.get()->velocity();};
        inline double velocity_x() const {return a_transform.get()->velocity_x();};
        inline double velocity_y() const {return a_transform.get()->velocity_y();};
        inline double width() const {return a_transform.get()->scale_x();};
        inline double x() const {return a_transform.get()->x();};
        inline double y() const {return a_transform.get()->y();};

        //******************
        // Hierarchy functions
        //******************

        // Function called after the creation
        virtual void after_creation(){};
        // Function called after the XML load
        virtual void after_xml_loading(){};
        // Convert a collision to a collision circle
        static Graphic_Collision::Collision_Circle* collision_circle(Graphic_Collision::Collision* collision){if(collision == 0 || collision->type != Graphic_Collision_Type::GCT_Circle){return 0;} return reinterpret_cast<Graphic_Collision::Collision_Circle*>(collision);};
        // Draws the object on an image
        virtual void draw_on_image(std::shared_ptr<scls::__Image_Base>){};

        // Applies a tranformation to the object
        virtual void apply_transformation(double pourcentage){};
        // If the object is transformable or not
        virtual int transformable() const {return 0;};

        // Updates the object
        virtual int update(double used_delta_time){return 1;};
        // Updates the actions of the object
        virtual bool update_action(double used_delta_time, Action* action, int& deleted_objects);

        // Function called during an interaction
        virtual void interaction(__Graphic_Object_Base* sender, std::string current_interaction){};
        // Function called when a new children is added
        virtual void when_new_children(__Graphic_Object_Base* new_child){};
        // Function called when the object should be delete
        virtual void when_should_delete();

        // Physic
        virtual int collision_height(){return std::ceil(max_absolute_y_next()) - std::floor(min_absolute_y_next());};
        virtual int collision_width(){return std::ceil(max_absolute_x_next()) - std::floor(min_absolute_x_next());};
        virtual int collision_x_start(){return std::floor(min_absolute_x_next());};
        virtual int collision_y_start(){return std::floor(min_absolute_y_next());};
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
        std::shared_ptr<Action_Container> a_actions = std::make_shared<Action_Container>();

        //******************
        // Main attributes
        //******************

        // Connected object to this object
        std::weak_ptr<scls::GUI_Text> a_connected_object = std::weak_ptr<scls::GUI_Text>();
        // Graphic base
        std::weak_ptr<__Graphic_Base> a_graphic_base;
        // Transformation in the object
        std::shared_ptr<scls::Transform_Object_2D> a_transform = std::make_shared<scls::Transform_Object_2D>();

        // Children of the object
        std::vector<std::shared_ptr<__Graphic_Object_Base>> a_children;
        // Deadline of the object
        double a_deadline = -1;
        // Drawing proportion of the object
        double a_drawing_proportion = 1;
        // ID (unique) of the object
        int a_id = -1;
        // Index of the object
        int a_index = 0;
        // Time since the object was created
        double a_live_time = 0;
        // Name of the object
        std::string a_name = std::string();
        // Used object name for this object
        std::string a_object_name;
        // Opacity of the object
        double a_opacity = 1.0;
        // Parent of the object
        std::weak_ptr<__Graphic_Object_Base> a_parent;
        // If the object should be save in XML
        bool a_save_to_xml_text = true;
        // If the object should be delete
        bool a_should_delete = false;
        // Tags of the object
        std::vector<std::string> a_tags;
        // This object
        std::weak_ptr<__Graphic_Object_Base> a_this_object;
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
}

#endif // PLEOS_GRAPHIC_BASE
