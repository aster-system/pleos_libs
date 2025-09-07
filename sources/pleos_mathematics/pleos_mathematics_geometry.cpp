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
// This file contains the source code of pleos_mathematics_geometry.cpp.
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

// Include pleos_mathematics_probabilities.h
#include "../../pleos_mathematics/pleos_mathematics_geometry.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // The base of all the next class
    //
    //******************

    // Returns the action to a XML text
    // Action
    std::string __Graphic_Object_Base::Action::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) +  std::string(">");}
    std::string __Graphic_Object_Base::Action::to_xml_text_name(){return std::string("action");}
    std::string __Graphic_Object_Base::Action::to_xml_text_object(std::string object_name){if(object_name == std::string()){return std::string();}return std::string(" object=\"") + object_name + std::string("\"");}
    std::string __Graphic_Object_Base::Action::to_xml_text_time() const{if(duration == 0){return std::string();}return std::string(" time=") + scls::Fraction::from_double(duration).to_std_string(0);}
    // Action delete
    std::string __Graphic_Object_Base::Action_Delete::to_xml_text_name(){return std::string("action_delete");}
    // Action loop
    std::string __Graphic_Object_Base::Action_Loop::to_xml_text_name(){return std::string("action_loop");}
    // Action move
    std::string __Graphic_Object_Base::Action_Move::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) + to_xml_text_x() + to_xml_text_y() + to_xml_text_speed() +  std::string(">");}
    std::string __Graphic_Object_Base::Action_Move::to_xml_text_name(){return std::string("action_move");}
    std::string __Graphic_Object_Base::Action_Move::to_xml_text_speed(){return std::string(" speed=") + scls::Fraction::from_double(speed).to_std_string(0);}
    std::string __Graphic_Object_Base::Action_Move::to_xml_text_x(){return std::string(" x=") + scls::Fraction::from_double(x_end).to_std_string(0);}
    std::string __Graphic_Object_Base::Action_Move::to_xml_text_y(){return std::string(" y=") + scls::Fraction::from_double(y_end).to_std_string(0);}
    // Action rotate
    std::string __Graphic_Object_Base::Action_Rotate::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) + to_xml_text_rotation() + std::string(">");}
    std::string __Graphic_Object_Base::Action_Rotate::to_xml_text_name(){return std::string("action_rotate");}
    std::string __Graphic_Object_Base::Action_Rotate::to_xml_text_rotation(){return std::string(" rotation=") + scls::Fraction::from_double(rotation_end).to_std_string(0);}
    // Action set parameter
    std::string __Graphic_Object_Base::Action_Set_Parameter::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) + to_xml_text_parameter() + to_xml_text_value() + to_xml_text_time() + std::string(">");}
    std::string __Graphic_Object_Base::Action_Set_Parameter::to_xml_text_name(){return std::string("action_set_parameter");}
    std::string __Graphic_Object_Base::Action_Set_Parameter::to_xml_text_parameter()const{return std::string(" parameter=") + parameter_name;}
    std::string __Graphic_Object_Base::Action_Set_Parameter::to_xml_text_value()const{return std::string(" value=") + parameter_value;}
    // Action stop
    std::string __Graphic_Object_Base::Action_Stop::to_xml_text_name(){return std::string("action_stop");}
    // Action structure
    std::string __Graphic_Object_Base::Action_Structure::to_xml_text_content(){std::string content = std::string();for(int i = 0;i<static_cast<int>(a_actions.size());i++){content += a_actions.at(i).get()->to_xml_text(std::string());if(i<static_cast<int>(a_actions.size())-1){content+=std::string("\n");}}return content;}
    std::string __Graphic_Object_Base::Action_Structure::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) + std::string(">\n") + to_xml_text_content() + std::string("\n</") + to_xml_text_name() + std::string(">");}
    // Action wait
    std::string __Graphic_Object_Base::Action_Wait::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) + to_xml_text_duration() +  std::string(">");}
    std::string __Graphic_Object_Base::Action_Wait::to_xml_text_duration(){return std::string(" duration=") + scls::Fraction::from_double(duration).to_std_string(0);}
    std::string __Graphic_Object_Base::Action_Wait::to_xml_text_name(){return std::string("action_wait");}
    std::string __Graphic_Object_Base::Action_Wait_Until::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) + to_xml_text_duration() +  std::string(">");}
    std::string __Graphic_Object_Base::Action_Wait_Until::to_xml_text_duration(){return std::string(" duration=") + scls::Fraction::from_double(duration).to_std_string(0);}
    std::string __Graphic_Object_Base::Action_Wait_Until::to_xml_text_name(){return std::string("action_wait_until");}

    // Getters and setters
    scls::Fraction __Graphic_Object_Base::Graphic_Collision::absolute_height() const {return attached_transform()->absolute_scale_y();};
    scls::Point_2D __Graphic_Object_Base::Graphic_Collision::absolute_scale() const {return attached_transform()->absolute_scale();};
    scls::Fraction __Graphic_Object_Base::Graphic_Collision::absolute_width() const {return attached_transform()->absolute_scale_x();};
    double __Graphic_Object_Base::Graphic_Collision::absolute_x() const {return attached_transform()->absolute_x();};
    double __Graphic_Object_Base::Graphic_Collision::absolute_x_1() const{return a_x_1.to_double();};
    double __Graphic_Object_Base::Graphic_Collision::absolute_x_2() const{return a_x_2.to_double();};
    double __Graphic_Object_Base::Graphic_Collision::absolute_y() const {return attached_transform()->absolute_y();};
    double __Graphic_Object_Base::Graphic_Collision::absolute_y_1() const{return a_y_1.to_double();};
    double __Graphic_Object_Base::Graphic_Collision::absolute_y_2() const{return a_y_2.to_double();};
    __Graphic_Object_Base* __Graphic_Object_Base::Graphic_Collision::attached_object()const{return a_attached_object.lock().get();};
    std::weak_ptr<__Graphic_Object_Base> __Graphic_Object_Base::Graphic_Collision::attached_object_weak_ptr()const{return a_attached_object;};
    scls::Transform_Object_2D* __Graphic_Object_Base::Graphic_Collision::attached_transform()const{return a_attached_transform.lock().get();};
    double __Graphic_Object_Base::Graphic_Collision::direct_x_1() const {return a_x_1.to_double();};
    double __Graphic_Object_Base::Graphic_Collision::direct_x_2() const {return a_x_2.to_double();};
    double __Graphic_Object_Base::Graphic_Collision::direct_y_1() const {return a_y_1.to_double();};
    double __Graphic_Object_Base::Graphic_Collision::direct_y_2() const {return a_y_2.to_double();};
    double __Graphic_Object_Base::Graphic_Collision::max_absolute_x() const {return attached_transform()->max_absolute_x();};
    double __Graphic_Object_Base::Graphic_Collision::max_absolute_x_next() const {return attached_transform()->max_absolute_x_next();};
    double __Graphic_Object_Base::Graphic_Collision::max_absolute_y() const {return attached_transform()->max_absolute_y();};
    double __Graphic_Object_Base::Graphic_Collision::max_absolute_y_next() const {return attached_transform()->max_absolute_y_next();};
    double __Graphic_Object_Base::Graphic_Collision::min_absolute_x() const {return attached_transform()->min_absolute_x();};
    double __Graphic_Object_Base::Graphic_Collision::min_absolute_x_next() const {return attached_transform()->min_absolute_x_next();};
    double __Graphic_Object_Base::Graphic_Collision::min_absolute_y() const {return attached_transform()->min_absolute_y();};
    double __Graphic_Object_Base::Graphic_Collision::min_absolute_y_next() const {return attached_transform()->min_absolute_y_next();};
    scls::Point_2D __Graphic_Object_Base::Graphic_Collision::position_next() const {return attached_transform()->position_next();};
    void __Graphic_Object_Base::Graphic_Collision::set_type(Graphic_Collision_Type new_type){a_type = new_type;};
    void __Graphic_Object_Base::Graphic_Collision::set_x_1(scls::Fraction new_x_1){a_x_1 = new_x_1;};
    void __Graphic_Object_Base::Graphic_Collision::set_x_2(scls::Fraction new_x_2){a_x_2 = new_x_2;};
    void __Graphic_Object_Base::Graphic_Collision::set_y_1(scls::Fraction new_y_1){a_y_1 = new_y_1;};
    void __Graphic_Object_Base::Graphic_Collision::set_y_2(scls::Fraction new_y_2){a_y_2 = new_y_2;};
    Graphic_Collision_Type __Graphic_Object_Base::Graphic_Collision::type()const{return a_type;};
    double __Graphic_Object_Base::Graphic_Collision::x_1() const {return attached_transform()->x() + a_x_1.to_double();};
    double __Graphic_Object_Base::Graphic_Collision::x_2() const {return attached_transform()->x() + a_x_2.to_double();};
    double __Graphic_Object_Base::Graphic_Collision::y_1() const {return attached_transform()->y() + a_y_1.to_double();};
    double __Graphic_Object_Base::Graphic_Collision::y_2() const {return attached_transform()->y() + a_y_2.to_double();};

    // __Graphic_Object_Base constructor
    int __current_id = 0;
    __Graphic_Object_Base::__Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::__Point_2D_Formula position):a_graphic_base(graphic_base),a_transform(std::make_shared<scls::Transform_Object_2D>(position)),a_id(__current_id),a_name(name){a_id = __current_id;__current_id++;a_transform.get()->set_this_object(a_transform);};
    __Graphic_Object_Base::__Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::Point_2D position):a_graphic_base(graphic_base),a_transform(std::make_shared<scls::Transform_Object_2D>(position)),a_id(__current_id),a_name(name){a_id = __current_id;__current_id++;a_transform.get()->set_this_object(a_transform);};
    __Graphic_Object_Base::__Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, std::string name):__Graphic_Object_Base(graphic_base,name,scls::Point_2D(0, 0)){};
    __Graphic_Object_Base::__Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base, scls::Point_2D position):__Graphic_Object_Base(graphic_base, std::string(), position){};
    __Graphic_Object_Base::__Graphic_Object_Base(std::weak_ptr<__Graphic_Base> graphic_base):__Graphic_Object_Base(graphic_base, std::string(), scls::Point_2D(0, 0)){};

    // Adds a tag to the object
    void __Graphic_Object_Base::add_tag(std::string new_tag){if(!contains_tag(new_tag)){a_tags.push_back(new_tag);}}

    // Returns if the object contains a specific tag
    bool __Graphic_Object_Base::contains_tag(std::string tag){for(int i=0;i<static_cast<int>(a_tags.size());i++){if(a_tags.at(i)==tag){return true;}}return false;}

    // Adds an action
    std::shared_ptr<__Graphic_Object_Base::Action> __Graphic_Object_Base::Action_Structure::add_action(std::shared_ptr<__Graphic_Object_Base::Action> needed_action){a_actions.push_back(needed_action);return needed_action;}
    // Clears the actions
    void __Graphic_Object_Base::Action_Structure::clear_actions(){a_actions.clear();};
    void __Graphic_Object_Base::clear_actions(){a_actions.get()->clear_actions();};
    // Deletes the last action
    void __Graphic_Object_Base::Action_Structure::delete_last_action(){if(a_actions.size() > 0){a_actions.erase(a_actions.begin());}};
    void __Graphic_Object_Base::delete_last_action(){a_actions.get()->delete_last_action();};
    // Go to the next action
    void __Graphic_Object_Base::Action_Structure::go_to_first_action(){a_current_action=0;__Graphic_Object_Base::Action* action = next_action();if(action != 0){action->soft_reset();}}
    void __Graphic_Object_Base::Action_Structure::go_to_next_action(){a_current_action++;__Graphic_Object_Base::Action* action = next_action();if(action != 0){action->soft_reset();}}
    // If the current action is the end action
    bool __Graphic_Object_Base::Action_Structure::is_end_action() const{return a_current_action >= static_cast<int>(a_actions.size());}
    // Returns a last action
    __Graphic_Object_Base::Action* __Graphic_Object_Base::last_action() const {return a_actions.get()->last_action();};
    __Graphic_Object_Base::Action* __Graphic_Object_Base::Action_Structure::last_action() const {if(static_cast<int>(a_actions.size()) <= 0){return 0;}return a_actions.at(0).get();};
    __Graphic_Object_Base::Action_Delete* __Graphic_Object_Base::Action_Container::last_action_delete() const{if(last_action() == 0 || last_action()->type != ACTION_DELETE){return 0;}return reinterpret_cast<__Graphic_Object_Base::Action_Delete*>(last_action());};
    __Graphic_Object_Base::Action_Delete* __Graphic_Object_Base::last_action_delete() const{return a_actions.get()->last_action_delete();};
    __Graphic_Object_Base::Action_Loop* __Graphic_Object_Base::Action_Container::last_action_loop() const{if(last_action() == 0 || last_action()->type != ACTION_LOOP){return 0;}return reinterpret_cast<__Graphic_Object_Base::Action_Loop*>(last_action());}
    __Graphic_Object_Base::Action_Loop* __Graphic_Object_Base::last_action_loop() const{return a_actions.get()->last_action_loop();}
    __Graphic_Object_Base::Action_Move* __Graphic_Object_Base::Action_Container::last_action_move() const {if(last_action() == 0 || last_action()->type != ACTION_MOVE){return 0;}return reinterpret_cast<__Graphic_Object_Base::Action_Move*>(last_action());};
    __Graphic_Object_Base::Action_Move* __Graphic_Object_Base::last_action_move() const {return a_actions.get()->last_action_move();};
    __Graphic_Object_Base::Action_Set_Parameter* __Graphic_Object_Base::Action_Container::last_action_set_parameter() const {if(last_action() == 0 || last_action()->type != ACTION_SET_PARAMETER){return 0;}return reinterpret_cast<__Graphic_Object_Base::Action_Set_Parameter*>(last_action());};
    __Graphic_Object_Base::Action_Set_Parameter* __Graphic_Object_Base::last_action_set_parameter() const {return a_actions.get()->last_action_set_parameter();};
    __Graphic_Object_Base::Action_Stop* __Graphic_Object_Base::Action_Container::last_action_stop() const {if(last_action() == 0 || last_action()->type != ACTION_STOP){return 0;}return reinterpret_cast<__Graphic_Object_Base::Action_Stop*>(last_action());};
    __Graphic_Object_Base::Action_Stop* __Graphic_Object_Base::last_action_stop() const {return a_actions.get()->last_action_stop();};
    short __Graphic_Object_Base::Action_Structure::last_action_type() const {return last_action()->type;};
    short __Graphic_Object_Base::last_action_type() const {return a_actions.get()->last_action_type();};
    __Graphic_Object_Base::Action_Wait* __Graphic_Object_Base::Action_Container::last_action_wait() const {if(last_action() == 0 || last_action()->type != ACTION_WAIT){return 0;}return reinterpret_cast<__Graphic_Object_Base::Action_Wait*>(last_action());};
    __Graphic_Object_Base::Action_Wait* __Graphic_Object_Base::last_action_wait() const {return a_actions.get()->last_action_wait();};
    // Returns the next action
    __Graphic_Object_Base::Action* __Graphic_Object_Base::Action_Structure::next_action() const{if(static_cast<int>(a_actions.size()) <= a_current_action){return 0;}return a_actions.at(a_current_action).get();}
    __Graphic_Object_Base::Action* __Graphic_Object_Base::next_action() const{return a_actions.get()->next_action();}
    short __Graphic_Object_Base::Action_Structure::next_action_type() const{__Graphic_Object_Base::Action* action = next_action();if(action==0){return 0;}return action->type;}
    short __Graphic_Object_Base::next_action_type() const{return a_actions.get()->next_action_type();}

    // Returns an introduction of the object
    std::string __Graphic_Object_Base::introduction(scls::Textual_Math_Settings* settings) const {return std::string("Nous avons l'object \"") + name() + std::string("\".");};

    // Handles the settings of the object
    void __Graphic_Object_Base::load_object_settings(scls::GUI_Object* gui_object) {
        gui_object->delete_children();

        // Name of the object
        std::shared_ptr<scls::GUI_Text> name_title_shared_ptr = *gui_object->new_object<scls::GUI_Text>(gui_object->name() + std::string("-name_title"));
        scls::GUI_Text* name_title = name_title_shared_ptr.get();
        name_title->attach_left_in_parent(10);name_title->attach_top_in_parent(10);
        name_title->set_height_in_pixel(40);name_title->set_width_in_scale(scls::Fraction(1, 10));
        name_title->set_text(std::string("Nom :"));
        std::shared_ptr<scls::GUI_Text_Input> name_shared_ptr = *gui_object->new_object<scls::GUI_Text_Input>(gui_object->name() + std::string("-name"));
        scls::GUI_Text_Input* name_object = name_shared_ptr.get();
        name_object->set_border_width_in_pixel(1);
        name_object->attach_right_of_object_in_parent(name_title, 10);name_object->attach_top_in_parent(10);
        name_object->set_height_in_pixel(40);name_object->set_width_in_scale(scls::Fraction(1, 4));
        name_object->set_text(name());

        // X of the object
        std::shared_ptr<scls::GUI_Text> x_title_shared_ptr = *gui_object->new_object<scls::GUI_Text>(gui_object->name() + std::string("-x_title"));
        scls::GUI_Text* x_title = x_title_shared_ptr.get();
        x_title->attach_left_in_parent(10);x_title->attach_bottom_of_object_in_parent(name_title_shared_ptr, 10);
        x_title->set_height_in_pixel(40);x_title->set_width_in_scale(scls::Fraction(1, 10));
        x_title->set_text(std::string("X :"));
        std::shared_ptr<scls::GUI_Text_Input> x_shared_ptr = *gui_object->new_object<scls::GUI_Text_Input>(gui_object->name() + std::string("-x"));
        scls::GUI_Text_Input* x = x_shared_ptr.get();
        x->set_border_width_in_pixel(1);
        x->attach_right_of_object_in_parent(x_title, 10);x->attach_bottom_of_object_in_parent(name_shared_ptr, 10);
        x->set_height_in_pixel(40);x->set_width_in_scale(scls::Fraction(1, 4));
        x->set_text(x_formula().to_std_string(0));

        // Y of the object
        std::shared_ptr<scls::GUI_Text> y_title_shared_ptr = *gui_object->new_object<scls::GUI_Text>(gui_object->name() + std::string("-y_title"));
        scls::GUI_Text* y_title = y_title_shared_ptr.get();
        y_title->attach_right_of_object_in_parent(x_shared_ptr, 10);y_title->attach_bottom_of_object_in_parent(name_title_shared_ptr, 10);
        y_title->set_height_in_pixel(40);y_title->set_width_in_scale(scls::Fraction(1, 10));
        y_title->set_text(std::string("Y :"));
        std::shared_ptr<scls::GUI_Text_Input> y_shared_ptr = *gui_object->new_object<scls::GUI_Text_Input>(gui_object->name() + std::string("-y"));
        scls::GUI_Text_Input* y = y_shared_ptr.get();
        y->set_border_width_in_pixel(1);
        y->attach_right_of_object_in_parent(y_title, 10);y->attach_bottom_of_object_in_parent(name_shared_ptr, 10);
        y->set_height_in_pixel(40);y->set_width_in_scale(scls::Fraction(1, 4));
        y->set_text(y_formula().to_std_string(0));
    }

    // Loads the tags
    void __Graphic_Object_Base::load_tags(std::string new_tags){a_tags = scls::cut_string(new_tags, std::string(";"));}

    // Annoying functions to draw the image
    int __Graphic_Object_Base::graphic_x_to_pixel_x(double x){return std::ceil((x - graphic_base()->a_middle_x.to_double()) * pixel_by_case_x() + (static_cast<double>(graphic_base()->a_width_in_pixel) / 2.0));};
    int __Graphic_Object_Base::graphic_x_to_pixel_x(scls::Fraction x){return std::ceil(((x - graphic_base()->a_middle_x) * pixel_by_case_x() + scls::Fraction(graphic_base()->a_width_in_pixel, 2)).to_double());};
    int __Graphic_Object_Base::graphic_y_to_pixel_y(double y){return (std::ceil((y - graphic_base()->a_middle_y.to_double()) * pixel_by_case_y()) + (graphic_base()->a_height_in_pixel / 2.0)) - graphic_base()->a_y_offset;};
    int __Graphic_Object_Base::graphic_y_to_pixel_y(scls::Fraction y){return std::ceil(((y - graphic_base()->a_middle_y) * pixel_by_case_y() + scls::Fraction(graphic_base()->a_height_in_pixel, 2)).to_double()) - graphic_base()->a_y_offset;};
    int __Graphic_Object_Base::graphic_y_to_pixel_y_inversed(double y){return graphic_base()->a_height_in_pixel - graphic_y_to_pixel_y(y);};
    int __Graphic_Object_Base::graphic_y_to_pixel_y_inversed(scls::Fraction y){return graphic_base()->a_height_in_pixel - graphic_y_to_pixel_y(y);};
    double __Graphic_Object_Base::pixel_by_case_x() const {return graphic_base()->a_pixel_by_case_x;};
    double __Graphic_Object_Base::pixel_by_case_y() const {return graphic_base()->a_pixel_by_case_y;};
    scls::Fraction __Graphic_Object_Base::pixel_x_to_graphic_x(int x){return graphic_base()->a_middle_x + ((scls::Fraction(x) - scls::Fraction(graphic_base()->a_width_in_pixel, 2)) / scls::Fraction::from_double(pixel_by_case_x()));}
    scls::Fraction __Graphic_Object_Base::pixel_y_to_graphic_y(int y){return graphic_base()->a_middle_y + ((scls::Fraction(graphic_base()->a_height_in_pixel, 2) - scls::Fraction(y)) / scls::Fraction::from_double(pixel_by_case_y()));}

    // Returns a parameter by its name
    std::string __Graphic_Object_Base::parameter(std::string parameter_name) {
        if(parameter_name == std::string("opacity")){return scls::Fraction::from_double(opacity()).to_std_string(0);}
        return std::string();
    }
    // Sets a parameter by its name
    void __Graphic_Object_Base::set_parameter(std::string parameter_name, std::string parameter_value, std::string parameter_value_start, double proportion) {
        if(parameter_name == std::string("opacity")){
            double start = scls::Fraction::from_std_string(parameter_value_start).to_double();
            double current = scls::string_to_double(parameter_value);
            if(proportion != 1){set_opacity(start + (current - start) * proportion);}
            else{set_opacity(current);}
        }
    }

    // Sets the parent of the object
    void __Graphic_Object_Base::__delete_children(__Graphic_Object_Base* object_to_delete){for(int i = 0;i<static_cast<int>(a_children.size());i++){if(a_children.at(i).get()==object_to_delete){a_children.erase(a_children.begin() + i);break;}}}
    void __Graphic_Object_Base::set_parent(__Graphic_Object_Base* new_parent){set_parent(new_parent->this_object_shared_ptr());}
    void __Graphic_Object_Base::set_parent(std::weak_ptr<__Graphic_Object_Base> new_parent) {
        // Reset the old parent
        if(parent() != 0){parent()->__delete_children(this);}

        // Set the new parent
        if(new_parent.lock().get() != 0) {
            a_parent = new_parent;
            attached_transform()->set_parent(new_parent.lock().get()->attached_transform());
            parent()->__delete_children(this);parent()->a_children.push_back(a_this_object.lock());
            new_parent.lock().get()->when_new_children(this);

            // Update the position (TEMP)
            //move_x(-parent()->absolute_x());
            //move_y(-parent()->absolute_y());
        }
    }

    // Returns the needed XML text to generate this object
    std::string __Graphic_Object_Base::to_displayed_text(){return std::string("objet");}
    std::string __Graphic_Object_Base::to_xml_text(){return std::string("<") + to_xml_text_base() + std::string(">");}
    std::string __Graphic_Object_Base::to_xml_text_base(){return to_xml_text_object_name() + to_xml_text_name() + to_xml_text_parent() + to_xml_text_x() + to_xml_text_y() + to_xml_text_width() + to_xml_text_height() + to_xml_text_opacity();}
    std::string __Graphic_Object_Base::to_xml_text_color(std::string attribute_name, scls::Color color){return std::string(" ") + attribute_name + std::string("=(") + std::to_string(color.red()) + std::string(",") + std::to_string(color.green()) + std::string(",") + std::to_string(color.blue()) + std::string(",") + std::to_string(color.alpha()) + std::string(")");};
    std::string __Graphic_Object_Base::to_xml_text_height(std::string attribute_name){if(height() == 1){return std::string();}return std::string(" ") + attribute_name + std::string("=") + height_formula().to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_height(){return to_xml_text_height(std::string("height"));}
    std::string __Graphic_Object_Base::to_xml_text_name(){if(a_name == std::string()){return std::string();}return std::string(" name=\"") + a_name + std::string("\"");}
    std::string __Graphic_Object_Base::to_xml_text_object_name(){return std::string("object");}
    std::string __Graphic_Object_Base::to_xml_text_opacity(){if(opacity() == 1){return std::string();}return std::string(" opacity=") + scls::Fraction::from_double(opacity()).to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_parent() {if(parent() == 0){return std::string();}return std::string(" parent=\"") + parent()->name() + std::string("\"");}
    std::string __Graphic_Object_Base::to_xml_text_rotation(){if(rotation_formula() == 0){return std::string();}return std::string(" rotation=\"") + rotation_formula().to_std_string(0) + std::string("\"");}
    std::string __Graphic_Object_Base::to_xml_text_tags(){if(static_cast<int>(a_tags.size())==0){return std::string();}std::string to_return=std::string();for(int i=0;i<static_cast<int>(a_tags.size());i++){to_return+=a_tags.at(i);if(i<static_cast<int>(a_tags.size())-1){to_return+=std::string(";");}}return std::string(" tags=\"") + to_return + std::string("\"");}
    std::string __Graphic_Object_Base::to_xml_text_x(){if(x() == 0){return std::string();}return std::string(" x=") + scls::remove_space(x_formula().to_std_string(0));}
    std::string __Graphic_Object_Base::to_xml_text_y(){if(y() == 0){return std::string();}return std::string(" y=") + scls::remove_space(y_formula().to_std_string(0));}
    std::string __Graphic_Object_Base::to_xml_text_width(std::string attribute_name){if(width() == 1){return std::string();}return std::string(" ") + attribute_name + std::string("=") + width_formula().to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_width(){return to_xml_text_width(std::string("width"));}

    // Updates the actions of the object
    bool __Graphic_Object_Base::update_action(double used_delta_time, __Graphic_Object_Base::Action* action, int& deleted_objects) {
        if(action->type == ACTION_STOP){set_velocity(velocity() * 0);return true;}
        else if(action->type == ACTION_WAIT){
            // Wait action
            __Graphic_Object_Base::Action_Wait* l_a = reinterpret_cast<__Graphic_Object_Base::Action_Wait*>(action);
            if(l_a->passed_time >= l_a->duration){return true;}
        }
        else if(action->type == ACTION_WAIT_UNTIL){
            // Wait  until action
            __Graphic_Object_Base::Action_Wait_Until* l_a = reinterpret_cast<__Graphic_Object_Base::Action_Wait_Until*>(action);
            if(graphic_base()->a_time.to_double() >= l_a->duration){return true;}
        }

        return false;
    }

    // Function called when the object should be delete
    void __Graphic_Object_Base::when_should_delete(){for(int i = 0;i<static_cast<int>(a_children.size());i++){a_children.at(i).get()->set_should_delete(true);}}

    //******************
    //
    // The "Point_2D" class
    //
    //******************

    // Point_2D constructor
    Point_2D::Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::__Point_2D_Formula point):__Graphic_Object_Base(graphic_base, name, point){};
    Point_2D::Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::Point_2D point):__Graphic_Object_Base(graphic_base, name, point){};
    Point_2D::Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, scls::Point_2D point):Point_2D(graphic_base, std::string(), point){};
    Point_2D::Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y):Point_2D(graphic_base, name, scls::__Point_2D_Formula(x, y)){};
    Point_2D::Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, scls::Formula x, scls::Formula y):Point_2D(graphic_base, std::string(), x, y){};
    Point_2D::Point_2D(std::weak_ptr<__Graphic_Base> graphic_base):Point_2D(graphic_base, std::string(), scls::Point_2D(0, 0)){};

    // Returns the needed XML text to generate this object
    std::string Point_2D::to_displayed_text(){return std::string("point");}
    std::string Point_2D::to_xml_text(){return std::string("<") + to_xml_text_base() + std::string(">");}
    std::string Point_2D::to_xml_text_object_name(){return std::string("point");}

    //******************
    //
    // The "Form_2D" class
    //
    //******************

    // Adds an exclusion point to the form
    void Form_2D::add_exclusion_point(std::shared_ptr<Point_2D> point){a_exclusion_points.push_back(point);};

    // Adds a point to the form
    void Form_2D::add_point_external(std::shared_ptr<__Graphic_Object_Base> point){if(!contains_point(point.get())){a_points.push_back(point);a_points_link.push_back(Link());}};
    void Form_2D::add_point(std::shared_ptr<__Graphic_Object_Base> point){add_point_external(point);point.get()->set_parent(this_object_shared_ptr());};

    // Clones the form
    void Form_2D::clone(std::shared_ptr<Form_2D> needed_clone) {
        // Sets the basic datas
        needed_clone.get()->set_border_color(border_color());
        needed_clone.get()->set_cloned_from(this_object_shared_ptr());
        needed_clone.get()->set_color(color());

        // Clone the points
        for(int i = 0;i<static_cast<int>(points().size());i++){std::shared_ptr<Point_2D> created_point = needed_clone.get()->new_point(points().at(i).get()->absolute_position());}
    }

    // Returns if the form contains a point or not
    bool Form_2D::contains_point(__Graphic_Object_Base* tested_point){for(int i=0;i<static_cast<int>(a_points.size());i++){if(a_points.at(i).get()==tested_point){return true;}}return false;};

    // Draws the circle on an image
    void Form_2D::draw_on_image(std::shared_ptr<scls::__Image_Base> image) {
        // Asserts
        if(points().size() < 2){return;}
        else if(points().size() == 2) {
            // Draw a line
            __Graphic_Object_Base* point_1 = points().at(0).get();
            __Graphic_Object_Base* point_2 = points().at(1).get();
            double last_x = graphic_x_to_pixel_x(point_1->absolute_x());
            double last_y = graphic_y_to_pixel_y_inversed(point_1->absolute_y());
            double needed_x = graphic_x_to_pixel_x(point_2->absolute_x());
            double needed_y = graphic_y_to_pixel_y_inversed(point_2->absolute_y());
            // Apply the proportion
            double needed_proportion = link(0).drawing_proportion;
            double needed_move_x = needed_x - last_x;double needed_move_x_minus = 0;
            double needed_move_y = needed_y - last_y;double needed_move_y_minus = 0;
            if(needed_proportion >= 0) {
                needed_move_x *= needed_proportion;
                needed_move_y *= needed_proportion;
            }
            else {
                needed_move_x_minus = needed_move_x * (1.0 + needed_proportion);
                needed_move_y_minus = needed_move_y * (1.0 + needed_proportion);
            }

            // Apply the proportion
            int line_width = border_radius();
            image.get()->draw_line(last_x + needed_move_x_minus + line_width / 2.0, last_y + needed_move_y_minus, last_x + needed_move_x, last_y + needed_move_y, color_with_opacity(border_color()), line_width);
            return;
        }

        // Triangulate the form
        std::vector<std::shared_ptr<Point_2D>> current_triangulated_points = triangulated_points_external();

        // Draw the inner form
        scls::Color inner_color = color_with_opacity(color());
        if(inner_color.alpha() > 0) {
            for(int i = 0;i<static_cast<int>(current_triangulated_points.size());i+=3) {
                std::shared_ptr<Point_2D> current_point = current_triangulated_points[i];
                double first_x = graphic_x_to_pixel_x(current_point.get()->x());
                double first_y = graphic_y_to_pixel_y_inversed(current_point.get()->y());
                current_point = current_triangulated_points[i + 1];
                double second_x = graphic_x_to_pixel_x(current_point.get()->x());
                double second_y = graphic_y_to_pixel_y_inversed(current_point.get()->y());
                current_point = current_triangulated_points[i + 2];
                double third_x = graphic_x_to_pixel_x(current_point.get()->x());
                double third_y = graphic_y_to_pixel_y_inversed(current_point.get()->y());
                image.get()->fill_triangle(first_x, first_y, second_x, second_y, third_x, third_y, inner_color);
            } current_triangulated_points.clear();
        }

        // Draw the links
        std::shared_ptr<__Graphic_Object_Base> last_point = points()[points().size() - 1];
        double last_x = graphic_x_to_pixel_x(last_point.get()->absolute_x());
        double last_y = graphic_y_to_pixel_y_inversed(last_point.get()->absolute_y());

        // Link each points
        scls::Color current_border_color = color_with_opacity(border_color());
        for(int j = 0;j<static_cast<int>(points().size());j++) {
            std::shared_ptr<__Graphic_Object_Base> current_point = points()[j];
            double needed_x = graphic_x_to_pixel_x(current_point.get()->absolute_x());
            double needed_y = graphic_y_to_pixel_y_inversed(current_point.get()->absolute_y());
            Form_2D::Link current_link;
            if(j <= 0){current_link = last_link();}
            else{current_link = link(j - 1);}

            // Apply the proportion
            double needed_proportion = current_link.drawing_proportion;
            double needed_move_x = needed_x - last_x;double needed_move_x_minus = 0;
            double needed_move_y = needed_y - last_y;double needed_move_y_minus = 0;
            if(needed_proportion >= 0) {
                needed_move_x *= needed_proportion;
                needed_move_y *= needed_proportion;
            }
            else {
                needed_move_x_minus = needed_move_x * (1.0 + needed_proportion);
                needed_move_y_minus = needed_move_y * (1.0 + needed_proportion);
            }

            image.get()->draw_line(last_x + needed_move_x_minus, last_y + needed_move_y_minus, last_x + needed_move_x, last_y + needed_move_y, current_border_color, border_radius());
            last_point = current_point; last_x = needed_x; last_y = needed_y;
        }
    }

    // Returns the introduction of the form 2D
    std::string Form_2D::introduction() const {return std::string("Nous avons le ") + type_name(false) + std::string(" ") + name() + std::string(".");};

    // Creates a new point to the form
    std::shared_ptr<Point_2D> Form_2D::new_point(scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y){
        std::shared_ptr<Point_2D> point = std::make_shared<Point_2D>(graphic_base_shared_ptr(), name() + std::string("_") + std::to_string(a_points.size()), x, y);
        point.get()->set_this_object(point);point.get()->set_parent(this_object_shared_ptr());add_point(point);
        return point;
    }
    std::shared_ptr<Point_2D> Form_2D::new_point(scls::Point_2D point){return new_point(point.x(), point.y());}

    // Returns a parameter by its name
    std::string Form_2D::parameter(std::string parameter_name) {
        if(parameter_name == std::string("color")){return color().to_std_string(0);}
        return __Graphic_Object_Base::parameter(parameter_name);
    }
    // Sets a parameter by its name
    void Form_2D::set_parameter(std::string parameter_name, std::string parameter_value, std::string parameter_value_start, double proportion) {
        if(parameter_name == std::string("color")){
            scls::Color base_color = scls::Color::from_std_string(parameter_value_start);
            scls::Color needed_color = scls::Color::from_std_string(parameter_value);
            if(proportion < 1){needed_color = base_color + (needed_color - base_color) * proportion;}

            set_color(needed_color);
        }
        else if(parameter_name == std::string("proportion")){if(a_points_link.size() > 0){a_points_link[0].drawing_proportion = scls::string_to_double(parameter_value) * proportion;}}
        else{__Graphic_Object_Base::set_parameter(parameter_name, parameter_value, parameter_value_start, proportion);}
    }

    // Returns this object to an XML text
    std::string Form_2D::to_displayed_text(){return std::string("forme");}
    std::string Form_2D::to_xml_text() {
        // Basic datas
        std::string content = std::string();

        // Clone
        if(cloned_from() != 0){
            content += std::string("<form_clone from=") + cloned_from()->name() + to_xml_text_name();
            content += std::string(">");
            return content;
        }

        // Check the common forms
        if(a_points.size() == 2 && object_name() == std::string("line")) {
            // The form is a line

            // Needed coordinates
            scls::__Formula_Base::Formula x_1 = a_points.at(0).get()->absolute_x_formula();
            scls::__Formula_Base::Formula x_2 = a_points.at(1).get()->absolute_x_formula();
            scls::__Formula_Base::Formula y_1 = a_points.at(0).get()->absolute_y_formula();
            scls::__Formula_Base::Formula y_2 = a_points.at(1).get()->absolute_y_formula();

            // Add the form
            double proportion = a_points_link.at(0).drawing_proportion;
            std::string proportion_string = std::string();if(proportion != 1){proportion_string = std::string(" proportion=") + std::to_string(proportion);}
            content += std::string("<line") + to_xml_text_name() + to_xml_text_tags() + to_xml_text_opacity() + to_xml_text_color(std::string("border_color"), border_color()) + std::string(" x_1=") + x_1.to_std_string(0) + std::string(" y_1=") + y_1.to_std_string(0) + std::string(" x_2=") + x_2.to_std_string(0) + std::string(" y_2=") + y_2.to_std_string(0) + to_xml_text_border_radius() + proportion_string + std::string(" physic=1 collision=line>");
            return content;
        }
        else if(a_points.size() == 4 && object_name() == std::string("rect")) {
            // The form is a rect

            // Needed coordinates
            scls::Fraction x_1 = a_points.at(0).get()->absolute_x();
            scls::Fraction x_2 = a_points.at(1).get()->absolute_x();
            scls::Fraction x_3 = a_points.at(2).get()->absolute_x();
            scls::Fraction x_4 = a_points.at(3).get()->absolute_x();
            scls::Fraction y_1 = a_points.at(0).get()->absolute_y();
            scls::Fraction y_2 = a_points.at(1).get()->absolute_y();
            scls::Fraction y_3 = a_points.at(2).get()->absolute_y();
            scls::Fraction y_4 = a_points.at(3).get()->absolute_y();

            // Get the good datas
            scls::__Formula_Base::Formula needed_height = height_formula();
            scls::__Formula_Base::Formula needed_width = width_formula();
            scls::__Formula_Base::Formula needed_x = x_formula() - needed_width / 2;
            scls::__Formula_Base::Formula needed_y = y_formula() - needed_height / 2;

            // Add the form
            content += std::string("<rect") + to_xml_text_name() + to_xml_text_tags() + to_xml_text_opacity() + to_xml_text_color(std::string("border_color"), border_color()) + to_xml_text_color(std::string("color"), color()) + std::string(" x=") + needed_x.to_std_string(0) + std::string(" y=") + needed_y.to_std_string(0) + std::string(" width=") + needed_width.to_std_string(0) + std::string(" height=") + needed_height.to_std_string(0) + to_xml_text_border_radius() + std::string(">");
            return content;
        }
        else if(object_name().size() > 6 && object_name().substr(0, 7) == std::string("polygon")) {
            // Add the form
            std::vector<std::string> cutted = scls::cut_string(object_name(), std::string("_"));
            int number = -1;if(cutted.size() >= 2){number =std::stoi(cutted.at(1));}
            content += std::string("<") + to_xml_text_object_name() + to_xml_text_name() + to_xml_text_tags() + to_xml_text_opacity() + to_xml_text_color(std::string("border_color"), border_color()) + to_xml_text_color(std::string("color"), color()) + to_xml_text_width() + to_xml_text_height() + std::string(" polygon=") + std::to_string(number) + std::string(">");
            return content;
        }

        // Add the points
        std::string point_names = std::string();
        for(int i = 0;i<static_cast<int>(a_points.size());i++){__Graphic_Object_Base* p = a_points.at(i).get();point_names += p->name() + std::string(";");}
        if(point_names.size() > 0 && point_names.at(point_names.size() - 1) == ';'){point_names = point_names.substr(0, point_names.size() - 1);}

        // Add the form
        content += std::string("<") + to_xml_text_object_name() + to_xml_text_name() + to_xml_text_tags() + to_xml_text_opacity() + to_xml_text_color(std::string("border_color"), border_color()) + to_xml_text_color(std::string("color"), color()) + to_xml_text_x() + to_xml_text_y() + to_xml_text_width() + to_xml_text_height() + std::string(" points=") + point_names + std::string(">");
        return content;

    }
    std::string Form_2D::to_xml_text_border_radius(){return std::string(" border_radius=") + std::to_string(border_radius());}
    std::string Form_2D::to_xml_text_object_name(){return std::string("form");}

    // Returns a list of the points triangulated
    void Form_2D::triangulate_points_external() {
        // Assert
        a_last_triangulation.reset();
        if(static_cast<int>(points().size()) < 3){return;}

        // Triangulate the face with the model maker part of SCLS
        scls::model_maker::Face form_to_face;
        for(int i = 0;i<static_cast<int>(points().size());i++) {
            scls::model_maker::Point current_point = points().at(i).get()->to_point_3d_absolute();
            form_to_face.points().push_back(std::make_shared<scls::model_maker::Point>(current_point));
        }
        for(int i = 0;i<static_cast<int>(exclusion_points().size());i++) {
            scls::model_maker::Point current_point = exclusion_points()[i].get()->to_point_3d_absolute();
            form_to_face.exclusion_points().push_back(std::make_shared<scls::model_maker::Point>(current_point));
        }
        a_last_triangulation = form_to_face.triangulate();
    }
    std::vector<std::shared_ptr<Point_2D>> Form_2D::triangulated_points_external() {
        // Triangulate the point
        triangulate_points_external();
        //if(a_last_triangulation.get() == 0){triangulate_points_external();}
        if(a_last_triangulation.get() == 0){return std::vector<std::shared_ptr<Point_2D>>();}

        // Get the needed result
        std::vector<std::shared_ptr<scls::model_maker::Point>>& points_for_rendering = a_last_triangulation.get()->points_for_rendering;
        std::vector<std::shared_ptr<Point_2D>> to_return = std::vector<std::shared_ptr<Point_2D>>(points_for_rendering.size());
        for(int i = 0;i<static_cast<int>(points_for_rendering.size());i++) {
            scls::model_maker::Point* current_point = points_for_rendering.at(i).get();
            to_return[i]=Point_2D::from_point(graphic_base_shared_ptr(), current_point);
        }
        return to_return;
    }

    // Returns the name of the type of the form
    std::string Form_2D::type_name(bool capitalise_first_letter) const {std::string to_return=std::string();if(a_points.size()==3){if(capitalise_first_letter){return std::string("Triangle");}return std::string("triangle");}if(capitalise_first_letter){to_return+=std::string("Forme");}else{to_return+=std::string("forme");}to_return+=std::string(" à ")+std::to_string(a_points.size())+std::string(" points");return to_return;};
    std::string Form_2D::type_name() const {return type_name(true);};

    //******************
    //
    // The "Circle" class
    //
    //******************

    // Circle constructor
    Circle::Circle(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y, scls::__Formula_Base::Formula radius_x, scls::__Formula_Base::Formula radius_y):__Graphic_Object_Base(graphic_base, name, scls::Point_2D_Formula(x, y)){set_radius_x(radius_x);set_radius_y(radius_y);}

    // Draws the circle on an image
    void Circle::draw_on_image(std::shared_ptr<scls::__Image_Base> image){
        scls::Point_2D current_center = center();
        double current_radius_x = radius_x().to_double();current_radius_x = current_radius_x * pixel_by_case_x();
        double current_radius_y = radius_y().to_double();current_radius_y = current_radius_y * pixel_by_case_y();
        double needed_x = graphic_x_to_pixel_x(current_center.x());
        double needed_y = graphic_y_to_pixel_y_inversed(current_center.y());
        image.get()->fill_circle(needed_x, needed_y, current_radius_x, current_radius_y, rotation_formula().value_to_double(unknowns()), angle_start().value_to_double(unknowns()) , angle_end().value_to_double(unknowns()), color(), border_radius(), border_color());
    }

    // Returns the needed XML text to generate this object
    std::string Circle::to_displayed_text(){return std::string("cercle");}
    std::string Circle::to_xml_text_angle_end(){if(angle_end() == 360){return std::string();}return std::string(" angle_end=") + scls::remove_space(angle_end().to_std_string(0));}
    std::string Circle::to_xml_text_angle_start(){if(angle_start() == 0){return std::string();}return std::string(" angle_start=") + scls::remove_space(angle_start().to_std_string(0));}
    std::string Circle::to_xml_text_radius(){if(radius_x() != radius_y()){return std::string();}return std::string(" radius=") + radius_x().to_std_string(0);}
    std::string Circle::to_xml_text_radius_x(){if(radius_x() == 1 || radius_x() == radius_y()){return std::string();}return std::string(" radius_x=") + radius_x().to_std_string(0);}
    std::string Circle::to_xml_text_radius_y(){if(radius_y() == 1 || radius_x() == radius_y()){return std::string();}return std::string(" radius_y=") + radius_y().to_std_string(0);}
    std::string Circle::to_xml_text_object_name(){return std::string("circle");}
    std::string Circle::to_xml_text(){return std::string("<") + to_xml_text_object_name() + to_xml_text_name() + to_xml_text_parent() + to_xml_text_rotation() + to_xml_text_tags() + to_xml_text_color(std::string("border_color"), border_color()) + to_xml_text_color(std::string("color"), color()) + to_xml_text_x() + to_xml_text_y() + to_xml_text_radius() + to_xml_text_radius_x() + to_xml_text_radius_y() + to_xml_text_angle_start() + to_xml_text_angle_end() + std::string(">");}
}
