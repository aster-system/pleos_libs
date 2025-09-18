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
        int a_y_offset = 0;

        // Coordinates of the base
        double a_middle_x = 0;
        double a_middle_y = 0;
        double a_pixel_by_case_x = 100;
        double a_pixel_by_case_y = 100;

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
            Action_Stop():Action(ACTION_STOP){};

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
            void add_action_move(double x_end, double y_end, double needed_speed){std::shared_ptr<Action_Move> action = std::make_shared<Action_Move>();action.get()->x_end=x_end;action.get()->y_end=y_end;action.get()->speed = needed_speed;actions().push_back(action);};
            void add_action_move(double x_end, double y_end){std::shared_ptr<Action_Move> action = std::make_shared<Action_Move>();action.get()->x_end=x_end;action.get()->y_end=y_end;actions().push_back(action);};
            void add_action_move(scls::Point_2D position, double needed_speed){add_action_move(position.x(), position.y(), needed_speed);};
            void add_action_move(scls::Point_2D position){add_action_move(position.x(), position.y());};
            // Adds a set_parameter action
            void add_action_set_parameter(std::string parameter_name, std::string parameter_value, double duration){std::shared_ptr<Action_Set_Parameter> action = std::make_shared<Action_Set_Parameter>();action.get()->duration=duration;action.get()->parameter_name=parameter_name;action.get()->parameter_value=parameter_value;actions().push_back(action);};
            void add_action_set_parameter(std::string parameter_name, double parameter_value, double duration){add_action_set_parameter(parameter_name, std::to_string(parameter_value), duration);};
            void add_action_set_parameter(std::string parameter_name, double parameter_value){add_action_set_parameter(parameter_name, std::to_string(parameter_value), 0);};
            void add_action_set_parameter(std::string parameter_name, std::string parameter_value){add_action_set_parameter(parameter_name, parameter_value, 0);};
            // Adds a stop action
            void add_action_stop(){std::shared_ptr<Action_Stop> action = std::make_shared<Action_Stop>();actions().push_back(action);};
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
        __Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::__Point_2D_Formula position);
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

        // Returns the point in a point 3D
        inline scls::Point_3D to_point_3d_absolute() {return scls::Point_3D(absolute_x(), 0, absolute_y());};

        // Getters and setters
        inline double absolute_height() const {return a_transform.get()->absolute_scale_y();};
        inline scls::__Formula_Base::Formula absolute_height_formula() const {return a_transform.get()->absolute_scale_y_formula();};
        inline double absolute_opacity() const {if(parent() == 0){return opacity();}return parent()->absolute_opacity() * opacity();};
        inline scls::Point_2D absolute_position() const {return a_transform.get()->absolute_position();};
        inline double absolute_rotation() const {return a_transform.get()->absolute_rotation();};
        inline double absolute_width() const {return a_transform.get()->absolute_scale_x();};
        inline scls::__Formula_Base::Formula absolute_width_formula() const {return a_transform.get()->absolute_scale_x_formula();};
        inline double absolute_x() const {return a_transform.get()->absolute_x();};
        inline scls::__Formula_Base::Formula absolute_x_formula() const {return a_transform.get()->absolute_x_formula();};
        inline double absolute_y() const {return a_transform.get()->absolute_y();};
        inline scls::__Formula_Base::Formula absolute_y_formula() const {return a_transform.get()->absolute_y_formula();};
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
        inline scls::__Formula_Base::Formula height_formula() const {return a_transform.get()->scale_y_formula();};
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
        inline scls::__Formula_Base::Formula rotation_formula() const {return a_transform.get()->rotation_formula();};
        inline bool save_to_xml_text() const {return a_save_to_xml_text;};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_deadline(double new_dead_line){a_deadline = new_dead_line;if(should_delete()){when_should_delete();}};
        inline void set_drawing_proportion(double new_drawing_proportion) {a_drawing_proportion = new_drawing_proportion;};
        inline void set_height(scls::__Formula_Base::Formula new_height){a_transform.get()->set_scale_y(new_height);};
        inline void set_index(int new_index){a_index=new_index;};
        inline void set_live_time(double new_live_time){a_live_time = new_live_time;};
        inline void set_object_name(std::string new_object_name){a_object_name = new_object_name;};
        inline void set_opacity(double new_opacity){a_opacity = new_opacity;};
        inline void set_position(scls::Point_2D_Formula new_position){attached_transform()->set_position(new_position);};
        inline void set_name(std::string new_name) {a_name = new_name;};
        inline void set_rotation(double new_rotation){a_transform.get()->set_rotation(scls::Fraction::from_double(new_rotation));};
        inline void set_rotation(scls::__Fraction_Base new_rotation){a_transform.get()->set_rotation(new_rotation);};
        inline void set_rotation(scls::Fraction new_rotation){a_transform.get()->set_rotation(new_rotation);};
        inline void set_rotation(scls::__Formula_Base::Formula new_rotation){a_transform.get()->set_rotation(new_rotation);};
        inline void set_save_to_xml_text(bool new_save_to_xml_text) {a_save_to_xml_text = new_save_to_xml_text;};
        inline void set_should_delete(bool new_should_delete){a_should_delete = new_should_delete;if(should_delete()){when_should_delete();}};
        inline void set_this_object(std::weak_ptr<__Graphic_Object_Base> new_this_object){a_this_object=new_this_object;};
        inline void set_unknowns(std::shared_ptr<scls::__Formula_Base::Unknowns_Container> new_unknowns){a_unknowns = new_unknowns;a_transform.get()->set_unknowns(new_unknowns);};
        inline void set_velocity(scls::Point_2D new_velocity){a_transform.get()->set_velocity(new_velocity);};
        inline bool should_delete() const {return a_should_delete || (a_deadline != -1 && a_live_time >= a_deadline);};
        inline void set_width(scls::__Formula_Base::Formula new_width){a_transform.get()->set_scale_x(new_width);};
        inline void set_x(scls::__Formula_Base::Formula new_x){a_transform.get()->set_x(new_x);};
        inline void set_y(scls::__Formula_Base::Formula new_y){a_transform.get()->set_y(new_y);};
        inline std::vector<std::string>& tags() {return a_tags;};
        inline std::shared_ptr<__Graphic_Object_Base> this_object_shared_ptr() const {return a_this_object.lock();};
        inline scls::Point_2D velocity() const {return a_transform.get()->velocity();};
        inline double velocity_x() const {return a_transform.get()->velocity_x();};
        inline double velocity_y() const {return a_transform.get()->velocity_y();};
        inline double width() const {return a_transform.get()->scale_x();};
        inline scls::__Formula_Base::Formula width_formula() const {return a_transform.get()->scale_x_formula();};
        inline double x() const {return a_transform.get()->x();};
        inline scls::__Formula_Base::Formula x_formula() const {return a_transform.get()->x_formula();};
        inline double y() const {return a_transform.get()->y();};
        inline scls::__Formula_Base::Formula y_formula() const {return a_transform.get()->y_formula();};

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
        virtual int collision_height(){if(a_points.size() == 2){return std::ceil(std::max(a_points.at(0).get()->absolute_y(), a_points.at(1).get()->absolute_y()) - std::min(a_points.at(0).get()->absolute_y(), a_points.at(1).get()->absolute_y()));}return __Graphic_Object_Base::collision_height();};
        virtual int collision_width(){if(a_points.size() == 2){return std::ceil(std::max(a_points.at(0).get()->absolute_x(), a_points.at(1).get()->absolute_x()) - std::min(a_points.at(0).get()->absolute_x(), a_points.at(1).get()->absolute_x()));}return __Graphic_Object_Base::collision_width();};
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
