//******************
//
// pleos_graphic_base.cpp
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
// This file contains the source code of pleos_graphic_base.h.
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

// Include pleos_graphic_base.h
#include "../pleos_graphic_base.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // The base of all the next class
    //
    //******************

    // Base of the graphic
    int Plane_Base::graphic_x_to_pixel_x(double x, int needed_width){return std::round((x - a_middle_x) * a_pixel_by_case_x + (static_cast<double>(needed_width) / 2.0)) ;};
    int Plane_Base::graphic_x_to_pixel_x(double x, std::shared_ptr<scls::__Image_Base> needed_image){return graphic_x_to_pixel_x(x, needed_image.get()->width());};
    int Plane_Base::graphic_y_to_pixel_y(double y, int needed_height){return (std::round((y - a_middle_y) * a_pixel_by_case_y) + (needed_height / 2.0)) - a_y_offset;};
    int Plane_Base::graphic_y_to_pixel_y(double y, std::shared_ptr<scls::__Image_Base> needed_image){return graphic_y_to_pixel_y(y, needed_image.get()->height());}
    scls::Fraction Plane_Base::pixel_x_to_graphic_x(int x, int image_width){return scls::Fraction::from_double(a_middle_x) + ((scls::Fraction(x) - scls::Fraction(image_width, 2)) / scls::Fraction::from_double(a_pixel_by_case_x));}
    scls::Fraction Plane_Base::pixel_x_to_graphic_x(int x, std::shared_ptr<scls::__Image_Base> needed_image){return pixel_x_to_graphic_x(x, needed_image.get()->width());}
    scls::Fraction Plane_Base::pixel_y_to_graphic_y(int y, int needed_height){return scls::Fraction::from_double(a_middle_y) + ((scls::Fraction(y) - scls::Fraction(needed_height, 2)) / scls::Fraction::from_double(a_pixel_by_case_y));}
    scls::Fraction Plane_Base::pixel_y_to_graphic_y(int y, std::shared_ptr<scls::__Image_Base> needed_image){return pixel_y_to_graphic_y(y, needed_image.get()->height());}

    // Clone actions
    std::shared_ptr<__Graphic_Object_Base::Action> __Graphic_Object_Base::Action_Accelerate::clone(){std::shared_ptr<__Graphic_Object_Base::Action_Accelerate> new_action = std::make_shared<__Graphic_Object_Base::Action_Accelerate>();new_action.get()->duration = duration;new_action.get()->direct_pass_at_end = direct_pass_at_end;new_action.get()->x = x;new_action.get()->y = y;return new_action;}
    std::shared_ptr<__Graphic_Object_Base::Action> __Graphic_Object_Base::Action_Delete::clone(){std::shared_ptr<__Graphic_Object_Base::Action_Delete> new_action = std::make_shared<__Graphic_Object_Base::Action_Delete>();new_action.get()->duration = duration;new_action.get()->direct_pass_at_end = direct_pass_at_end;new_action.get()->to_delete = to_delete;return new_action;}
    std::shared_ptr<__Graphic_Object_Base::Action> __Graphic_Object_Base::Action_Function_Call::clone(){std::shared_ptr<__Graphic_Object_Base::Action_Function_Call> new_action = std::make_shared<__Graphic_Object_Base::Action_Function_Call>();new_action.get()->duration = duration;new_action.get()->direct_pass_at_end = direct_pass_at_end;new_action.get()->function_name = function_name;return new_action;}
    std::shared_ptr<__Graphic_Object_Base::Action> __Graphic_Object_Base::Action_Move::clone(){std::shared_ptr<__Graphic_Object_Base::Action_Move> new_action = std::make_shared<__Graphic_Object_Base::Action_Move>();new_action.get()->duration = duration;new_action.get()->direct_pass_at_end = direct_pass_at_end;new_action.get()->speed = speed;new_action.get()->x_end = x_end;new_action.get()->y_end = y_end;return new_action;}
    std::shared_ptr<__Graphic_Object_Base::Action> __Graphic_Object_Base::Action_Rotate::clone(){std::shared_ptr<__Graphic_Object_Base::Action_Rotate> new_action = std::make_shared<__Graphic_Object_Base::Action_Rotate>();new_action.get()->duration = duration;new_action.get()->direct_pass_at_end = direct_pass_at_end;new_action.get()->rotation_end = rotation_end;new_action.get()->speed = speed;return new_action;}
    std::shared_ptr<__Graphic_Object_Base::Action> __Graphic_Object_Base::Action_Set_Parameter::clone(){std::shared_ptr<__Graphic_Object_Base::Action_Set_Parameter> new_action = std::make_shared<__Graphic_Object_Base::Action_Set_Parameter>();new_action.get()->duration = duration;new_action.get()->direct_pass_at_end = direct_pass_at_end;new_action.get()->parameter_name = parameter_name;new_action.get()->parameter_value = parameter_value;return new_action;}
    std::shared_ptr<__Graphic_Object_Base::Action> __Graphic_Object_Base::Action_Stop::clone(){std::shared_ptr<__Graphic_Object_Base::Action_Stop> new_action = std::make_shared<__Graphic_Object_Base::Action_Stop>(duration);return new_action;}
    std::shared_ptr<__Graphic_Object_Base::Action> __Graphic_Object_Base::Action_Wait::clone(){std::shared_ptr<__Graphic_Object_Base::Action_Wait> new_action = std::make_shared<__Graphic_Object_Base::Action_Wait>();new_action.get()->duration = duration;new_action.get()->direct_pass_at_end = direct_pass_at_end;return new_action;}
    std::shared_ptr<__Graphic_Object_Base::Action> __Graphic_Object_Base::Action_Wait_Until::clone(){std::shared_ptr<__Graphic_Object_Base::Action_Wait_Until> new_action = std::make_shared<__Graphic_Object_Base::Action_Wait_Until>();new_action.get()->duration = duration;new_action.get()->direct_pass_at_end = direct_pass_at_end;return new_action;}
    // Clone a structure
    std::shared_ptr<__Graphic_Object_Base::Action> __Graphic_Object_Base::Action_Structure::clone(){return clone_as_structure();}
    std::shared_ptr<__Graphic_Object_Base::Action_Structure> __Graphic_Object_Base::Action_Structure::clone_as_structure() {std::shared_ptr<__Graphic_Object_Base::Action_Structure> new_action = std::make_shared<__Graphic_Object_Base::Action_Structure>(type);clone_content(new_action);return new_action;}
    void __Graphic_Object_Base::Action_Structure::clone_content(std::shared_ptr<Action_Structure> target){for(int i = 0;i<static_cast<int>(a_actions.size());i++){target.get()->add_action(a_actions.at(i).get()->clone());}}
    // Complex structures
    std::shared_ptr<__Graphic_Object_Base::Action_Function> __Graphic_Object_Base::Action_Function::clone_as_function() {std::shared_ptr<__Graphic_Object_Base::Action_Function> new_action = std::make_shared<__Graphic_Object_Base::Action_Function>();new_action.get()->function_name = function_name;clone_content(new_action);return new_action;}
    std::shared_ptr<__Graphic_Object_Base::Action_Structure> __Graphic_Object_Base::Action_Function::clone_as_structure() {std::shared_ptr<__Graphic_Object_Base::Action_Function> new_action = std::make_shared<__Graphic_Object_Base::Action_Function>();new_action.get()->function_name = function_name;clone_content(new_action);return new_action;}
    std::shared_ptr<__Graphic_Object_Base::Action_Structure> __Graphic_Object_Base::Action_Loop::clone_as_structure() {std::shared_ptr<__Graphic_Object_Base::Action_Loop> new_action = std::make_shared<__Graphic_Object_Base::Action_Loop>();new_action.get()->a_repetition = a_repetition;clone_content(new_action);return new_action;}

    // Returns the action to a XML text
    // Action
    std::string __Graphic_Object_Base::Action::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) + to_xml_text_time() + std::string(">");}
    std::string __Graphic_Object_Base::Action::to_xml_text_name(){return std::string("action");}
    std::string __Graphic_Object_Base::Action::to_xml_text_object(std::string object_name){if(object_name == std::string()){return std::string();}return std::string(" object=\"") + object_name + std::string("\"");}
    std::string __Graphic_Object_Base::Action::to_xml_text_time() const{if(duration == 0){return std::string();}return std::string(" time=") + scls::Fraction::from_double(duration).to_std_string(0);}
    // Action accelerate
    std::string __Graphic_Object_Base::Action_Accelerate::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) + to_xml_text_x() + to_xml_text_y() +  std::string(">");}
    std::string __Graphic_Object_Base::Action_Accelerate::to_xml_text_name(){return std::string("action_accelerate");}
    std::string __Graphic_Object_Base::Action_Accelerate::to_xml_text_x(){return std::string(" x=") + scls::Fraction::from_double(x).to_std_string(0);}
    std::string __Graphic_Object_Base::Action_Accelerate::to_xml_text_y(){return std::string(" y=") + scls::Fraction::from_double(y).to_std_string(0);}
    // Action delete
    std::string __Graphic_Object_Base::Action_Delete::to_xml_text_name(){return std::string("action_delete");}
    // Action function
    std::string __Graphic_Object_Base::Action_Function::to_xml_text_name(){return std::string("action_function");}
    std::string __Graphic_Object_Base::Action_Function_Call::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) + to_xml_text_function_name() +  std::string(">");}
    std::string __Graphic_Object_Base::Action_Function_Call::to_xml_text_name(){return std::string("action_function_call");}
    std::string __Graphic_Object_Base::Action_Function_Call::to_xml_text_function_name(){return std::string(" function=") + function_name;}
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

    // __Graphic_Object_Base constructor
    int __current_id = 0;
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
    void __Graphic_Object_Base::Action_Structure::clear_actions(){a_actions.clear();a_current_action = 0;};
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

    // Returns a color adapted with the needed opacity
    scls::Color __Graphic_Object_Base::color_with_absolute_opacity(scls::Color needed_color)const{needed_color.set_alpha(static_cast<double>(needed_color.alpha()) * absolute_opacity());return needed_color;};
    scls::Color __Graphic_Object_Base::color_with_opacity(scls::Color needed_color)const{needed_color.set_alpha(static_cast<double>(needed_color.alpha()) * a_opacity);return needed_color;};

    // Draws the object on an image
    void __Graphic_Object_Base::draw_on_image(std::shared_ptr<scls::__Image_Base>){};

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
        scls::GUI_Text_Input* x_text = x_shared_ptr.get();
        x_text->set_border_width_in_pixel(1);
        x_text->attach_right_of_object_in_parent(x_title, 10);x_text->attach_bottom_of_object_in_parent(name_shared_ptr, 10);
        x_text->set_height_in_pixel(40);x_text->set_width_in_scale(scls::Fraction(1, 4));
        x_text->set_text(std::to_string(x()));

        // Y of the object
        std::shared_ptr<scls::GUI_Text> y_title_shared_ptr = *gui_object->new_object<scls::GUI_Text>(gui_object->name() + std::string("-y_title"));
        scls::GUI_Text* y_title = y_title_shared_ptr.get();
        y_title->attach_right_of_object_in_parent(x_shared_ptr, 10);y_title->attach_bottom_of_object_in_parent(name_title_shared_ptr, 10);
        y_title->set_height_in_pixel(40);y_title->set_width_in_scale(scls::Fraction(1, 10));
        y_title->set_text(std::string("Y :"));
        std::shared_ptr<scls::GUI_Text_Input> y_shared_ptr = *gui_object->new_object<scls::GUI_Text_Input>(gui_object->name() + std::string("-y"));
        scls::GUI_Text_Input* y_text = y_shared_ptr.get();
        y_text->set_border_width_in_pixel(1);
        y_text->attach_right_of_object_in_parent(y_title, 10);y_text->attach_bottom_of_object_in_parent(name_shared_ptr, 10);
        y_text->set_height_in_pixel(40);y_text->set_width_in_scale(scls::Fraction(1, 4));
        y_text->set_text(std::to_string(y()));
    }

    // Loads the tags
    void __Graphic_Object_Base::load_tags(std::string new_tags){a_tags = scls::cut_string(new_tags, std::string(";"));}

    // Remvoes a tag to the object
    void __Graphic_Object_Base::remove_tag(std::string needed_tag){for(int i = 0;i<static_cast<int>(a_tags.size());i++){if(a_tags.at(i) == needed_tag){a_tags.erase(a_tags.begin() + i);break;}}}

    // Annoying functions to draw the image
    int __Graphic_Object_Base::graphic_x_to_pixel_x(double x){return std::ceil((x - graphic_base()->a_middle_x) * pixel_by_case_x() + (static_cast<double>(graphic_base()->a_width_in_pixel) / 2.0));};
    int __Graphic_Object_Base::graphic_x_to_pixel_x(scls::Fraction x){return std::ceil(((x - scls::Fraction::from_double(graphic_base()->a_middle_x)) * pixel_by_case_x() + scls::Fraction(graphic_base()->a_width_in_pixel, 2)).to_double());};
    int __Graphic_Object_Base::graphic_y_to_pixel_y(double y){return (std::ceil((y - graphic_base()->a_middle_y) * pixel_by_case_y()) + (graphic_base()->a_height_in_pixel / 2.0)) - graphic_base()->a_y_offset;};
    int __Graphic_Object_Base::graphic_y_to_pixel_y(scls::Fraction y){return std::ceil(((y - scls::Fraction::from_double(graphic_base()->a_middle_y)) * pixel_by_case_y() + scls::Fraction(graphic_base()->a_height_in_pixel, 2)).to_double()) - graphic_base()->a_y_offset;};
    int __Graphic_Object_Base::graphic_y_to_pixel_y_inversed(double y){return graphic_base()->a_height_in_pixel - graphic_y_to_pixel_y(y);};
    int __Graphic_Object_Base::graphic_y_to_pixel_y_inversed(scls::Fraction y){return graphic_base()->a_height_in_pixel - graphic_y_to_pixel_y(y);};
    double __Graphic_Object_Base::pixel_by_case_x() const {return graphic_base()->a_pixel_by_case_x;};
    double __Graphic_Object_Base::pixel_by_case_y() const {return graphic_base()->a_pixel_by_case_y;};
    scls::Fraction __Graphic_Object_Base::pixel_x_to_graphic_x(int x){return scls::Fraction::from_double(graphic_base()->a_middle_x) + ((scls::Fraction(x) - scls::Fraction(graphic_base()->a_width_in_pixel, 2)) / scls::Fraction::from_double(pixel_by_case_x()));}
    scls::Fraction __Graphic_Object_Base::pixel_y_to_graphic_y(int y){return scls::Fraction::from_double(graphic_base()->a_middle_y) + ((scls::Fraction(graphic_base()->a_height_in_pixel, 2) - scls::Fraction(y)) / scls::Fraction::from_double(pixel_by_case_y()));}

    // Returns a parameter by its name
    std::string __Graphic_Object_Base::parameter(std::string parameter_name) {
        if(parameter_name == std::string("opacity")){return scls::Fraction::from_double(opacity()).to_std_string(0);}
        return std::string();
    }
    // Sets a parameter by its name
    void __Graphic_Object_Base::set_parameter(std::string parameter_name, std::string parameter_value){return set_parameter(parameter_name, parameter_value, std::string(""), 1);}
    void __Graphic_Object_Base::set_parameter(std::string parameter_name, std::string parameter_value, std::string parameter_value_start, double proportion) {
        if(parameter_name == std::string("opacity")){
            double current = scls::string_to_double(parameter_value);
            if(proportion != 1){double start = scls::Fraction::from_std_string(parameter_value_start).to_double();set_opacity(start + (current - start) * proportion);}
            else{set_opacity(current);}
        }
        else if(parameter_name == std::string("height")){double current = scls::string_to_double(parameter_value);set_height(current);}
        else if(parameter_name == std::string("width")){double current = scls::string_to_double(parameter_value);set_width(current);}
        else if(parameter_name == std::string("x")){double current = scls::string_to_double(parameter_value);set_x(current);}
        else if(parameter_name == std::string("y")){double current = scls::string_to_double(parameter_value);set_y(current);}
    }

    // Sets the parent of the object
    void __Graphic_Object_Base::__delete_children(__Graphic_Object_Base* object_to_delete){for(int i = 0;i<static_cast<int>(a_children.size());i++){if(a_children.at(i).get()==object_to_delete){a_children.erase(a_children.begin() + i);break;}}}
    void __Graphic_Object_Base::set_parent(__Graphic_Object_Base* new_parent){if(new_parent==0){set_parent(std::weak_ptr<__Graphic_Object_Base>());}else{set_parent(new_parent->this_object_shared_ptr());}}
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
        else{a_parent.reset();attached_transform()->set_parent(std::shared_ptr<scls::Transform_Object_2D>());}
    }

    // Returns the needed XML text to generate this object
    std::string __Graphic_Object_Base::to_displayed_text(){return std::string("objet");}
    std::string __Graphic_Object_Base::to_xml_text(){return std::string("<") + to_xml_text_base() + std::string(">");}
    std::string __Graphic_Object_Base::to_xml_text_base(){return to_xml_text_object_name() + to_xml_text_name() + to_xml_text_parent() + to_xml_text_x() + to_xml_text_y() + to_xml_text_width() + to_xml_text_height() + to_xml_text_opacity() + to_xml_text_tags();}
    std::string __Graphic_Object_Base::to_xml_text_color(std::string attribute_name, scls::Color color){return std::string(" ") + attribute_name + std::string("=(") + std::to_string(color.red()) + std::string(",") + std::to_string(color.green()) + std::string(",") + std::to_string(color.blue()) + std::string(",") + std::to_string(color.alpha()) + std::string(")");};
    std::string __Graphic_Object_Base::to_xml_text_height(std::string attribute_name){if(height() == 1){return std::string();}return std::string(" ") + attribute_name + std::string("=") + std::to_string(height());}
    std::string __Graphic_Object_Base::to_xml_text_height(){return to_xml_text_height(std::string("height"));}
    std::string __Graphic_Object_Base::to_xml_text_name(){if(a_name == std::string()){return std::string();}return std::string(" name=\"") + a_name + std::string("\"");}
    std::string __Graphic_Object_Base::to_xml_text_object_name(){return std::string("object");}
    std::string __Graphic_Object_Base::to_xml_text_opacity(){if(opacity() == 1){return std::string();}return std::string(" opacity=") + scls::Fraction::from_double(opacity()).to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_parent() {if(parent() == 0){return std::string();}return std::string(" parent=\"") + parent()->name() + std::string("\"");}
    std::string __Graphic_Object_Base::to_xml_text_rotation(){if(rotation() == 0){return std::string();}return std::string(" rotation=\"") + rotation().to_std_string(0) + std::string("\"");}
    std::string __Graphic_Object_Base::to_xml_text_tags(){if(static_cast<int>(a_tags.size())==0){return std::string();}std::string to_return=std::string();for(int i=0;i<static_cast<int>(a_tags.size());i++){to_return+=a_tags.at(i);if(i<static_cast<int>(a_tags.size())-1){to_return+=std::string(";");}}return std::string(" tags=\"") + to_return + std::string("\"");}
    std::string __Graphic_Object_Base::to_xml_text_x(){if(x() == 0){return std::string();}return std::string(" x=") + scls::remove_space(x_formula().to_std_string(0));}
    std::string __Graphic_Object_Base::to_xml_text_y(){if(y() == 0){return std::string();}return std::string(" y=") + scls::remove_space(y_formula().to_std_string(0));}
    std::string __Graphic_Object_Base::to_xml_text_width(std::string attribute_name){if(width() == 1){return std::string();}return std::string(" ") + attribute_name + std::string("=") + width_formula().to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_width(){return to_xml_text_width(std::string("width"));}

    // Updates the actions of the object
    bool __Graphic_Object_Base::update_action(double used_delta_time, __Graphic_Object_Base::Action* action, int& deleted_objects) {
        if(action->type == ACTION_STOP){
            double proportion = 1;if(action->duration != 0){proportion = action->passed_time / action->duration;}
            if(proportion >= 1){set_velocity(velocity() * 0);return true;}
            else{set_velocity(velocity() * 0.995);}
        }
        else if(action->type == ACTION_WAIT){
            // Wait action
            __Graphic_Object_Base::Action_Wait* l_a = reinterpret_cast<__Graphic_Object_Base::Action_Wait*>(action);
            if(l_a->passed_time >= l_a->duration){return true;}
        }
        else if(action->type == ACTION_WAIT_UNTIL){
            // Wait until action
            __Graphic_Object_Base::Action_Wait_Until* l_a = reinterpret_cast<__Graphic_Object_Base::Action_Wait_Until*>(action);
            if(graphic_base()->a_time.to_double() >= l_a->duration){return true;}
        }

        return false;
    }

    // Function called when the object should be delete
    void __Graphic_Object_Base::when_should_delete(){for(int i = 0;i<static_cast<int>(a_children.size());i++){a_children.at(i).get()->set_should_delete(true);}}
}
