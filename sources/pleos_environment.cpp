//******************
//
// pleos_environment.cpp
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
// This file contains the source code of pleos_environment.h.
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

// Include SCLS Graphic Benoit
#include "../pleos_environment.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //*********
	//
	// The Text_Environment class
	//
	//*********

	// Loads the PLEOS balises
    void load_balises_pleos(std::shared_ptr<scls::_Balise_Style_Container> defined_balises) {
        // Loads basic balises
        defined_balises.get()->_load_built_in_balises();

        std::shared_ptr<scls::Balise_Style_Datas> current_balise;
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("action_loop", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("case_plus", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("content", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("function", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("graph", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.set_border_width(1);
        current_balise.get()->style.set_margin_bottom(16);
        defined_balises.get()->set_defined_balise("graphic", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("histogram", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = false;
        defined_balises.get()->set_defined_balise("link", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("linked_list", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("loop_action", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("new_definition", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("node", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("nodes", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("point_cloud", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("point_cloud_linked", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("repeat", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.set_margin_bottom(16);
        defined_balises.get()->set_defined_balise("table", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("text", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("tree", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("trees", current_balise);
    }

    // __Content constructor
    Text_Environment::Definition::__Content::__Content(std::string needed_content):content(needed_content){}
    Text_Environment::Definition::__Content::__Content(std::string needed_name,std::string needed_content):content(needed_content),name(needed_name){}

    // Returns a color value
    scls::Color Text_Environment::value_color(std::string base)const{
        // Function
        scls::Function_Called_Text called_function = scls::parse_function_call(base);
        if(called_function.name == std::string("random")){return scls::Color(scls::random_int_between_included(0, 255), scls::random_int_between_included(0, 255), scls::random_int_between_included(0, 255));}

        // Set the color
        std::vector<std::string> cutted = scls::Color::from_std_string_parts(base);

        // Get the color
        scls::Color to_return(255, 255, 255);
        if(cutted.size() > 1) {
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                if(i == 0) to_return.set_red(value_double(cutted[i]));
                else if(i == 1) to_return.set_green(value_double(cutted[i]));
                else if(i == 2) to_return.set_blue(value_double(cutted[i]));
                else if(i == 3) to_return.set_alpha(value_double(cutted[i]));
            }
        }
        else if(cutted.size() == 1) {scls::defined_color_by_name(cutted.at(0), to_return);}

        return to_return;
    }
    // Returns a list color value
    std::vector<scls::Color> Text_Environment::value_color_list(std::string base)const{
        std::vector<std::string> cutted = scls::cut_string_out_of_2(base, std::string(","), std::string("("), std::string(")"));
        std::vector<scls::Color> to_return = std::vector<scls::Color>(cutted.size());
        for(int i = 0;i<static_cast<int>(cutted.size());i++){to_return[i] = value_color(cutted.at(i));}
        return to_return;
    }
	// Returns a Point_2D value
    scls::Point_2D_Formula Text_Environment::value_point_2d(std::string base)const{
        // Format the text
        while(base.size() > 0 && base.at(0) == '('){base = base.substr(1, base.size() - 1);}
        while(base.size() > 0 && base.at(base.size() - 1) == ')'){base = base.substr(0, base.size() - 1);}

        // Get the point
        base = scls::replace(base, std::string(";"), std::string(","));
        std::vector<std::string> cutted = scls::cut_string(base, std::string(","));
        if(cutted.size() != 2) {scls::print(std::string("PLEOS Text Environment"), std::string("Can't get a point 2D from \"") + base + std::string("\"."));return scls::Point_2D(0, 0);}
        return scls::Point_2D_Formula(value_number(cutted.at(0)), value_number(cutted.at(1)));
    };

    //*********
    // Definition system
    //*********

    // Adds a content to the definition
    void Text_Environment::Definition::add_content(std::string content){if(a_contents.size() <= 0){a_contents.push_back(std::make_shared<Text_Environment::Definition::__Content>(content));}else{a_contents.at(0).get()->content=content;}};
    void Text_Environment::Definition::add_content(std::string content_name, std::string content){Text_Environment::Definition::__Content* definition = content_full_by_name(content_name);if(definition==0){a_contents.push_back(std::make_shared<Text_Environment::Definition::__Content>(content_name, content));}else{definition->content=content;}}

    // Gets the content of a definition
    std::string Text_Environment::Definition::content(bool capitalise_first_letter)const{
        std::string to_return = std::string();
        if(a_contents.size()>0){to_return = a_contents.at(0).get()->content;if(to_return.size()>0&&capitalise_first_letter&&std::isalpha(to_return.at(0))){to_return=scls::capitalise_letter(to_return,0);}}
        return to_return;
    };
    std::string Text_Environment::Definition::content(std::string content_name, bool capitalise_first_letter)const{
        std::string to_return = std::string();__Content* needed_content = content_full_by_name(content_name);
        if(needed_content!=0){to_return = needed_content->content;if(to_return.size()>0&&capitalise_first_letter&&std::isalpha(to_return.at(0))){to_return=scls::capitalise_letter(to_return,0);}}
        return to_return;
    }
    Text_Environment::Definition::__Content* Text_Environment::Definition::content_full_by_name(std::string content_name) const{return content_full_by_name_shared_ptr(content_name).get();}
    std::shared_ptr<Text_Environment::Definition::__Content> Text_Environment::Definition::content_full_by_name_shared_ptr(std::string content_name) const{for(int i = 0;i<static_cast<int>(a_contents.size());i++){if(a_contents.at(i).get()->name == content_name){return a_contents.at(i);}}return std::shared_ptr<Text_Environment::Definition::__Content>();}

    // Adds a theorem to the definition
    void Text_Environment::Definition::add_theorem(std::string theorem){if(a_theorems.size() <= 0){a_theorems.push_back(std::make_shared<Text_Environment::Definition::__Content>(theorem));}else{a_theorems.at(0).get()->content=theorem;}};
    void Text_Environment::Definition::add_theorem(std::string theorem_name, std::string theorem){Text_Environment::Definition::__Content* definition = theorem_full_by_name(theorem_name);if(definition==0){a_theorems.push_back(std::make_shared<Text_Environment::Definition::__Content>(theorem_name, theorem));}else{definition->content=theorem;}}

    // Gets one theorem of the definition
    std::string Text_Environment::Definition::theorem(bool capitalise_first_letter)const{
        std::string to_return = std::string();
        if(a_theorems.size()>0){to_return = a_theorems.at(0).get()->content;if(to_return.size()>0&&capitalise_first_letter&&std::isalpha(to_return.at(0))){to_return=scls::capitalise_letter(to_return,0);}}
        return to_return;
    }
    std::string Text_Environment::Definition::theorem(std::string theorem_name, bool capitalise_first_letter)const{
        std::string to_return = std::string();__Content* needed_content = theorem_full_by_name(theorem_name);
        if(needed_content!=0){to_return = needed_content->content;if(to_return.size()>0&&capitalise_first_letter&&std::isalpha(to_return.at(0))){to_return=scls::capitalise_letter(to_return,0);}}
        return to_return;
    }
    Text_Environment::Definition::__Content* Text_Environment::Definition::theorem_full_by_name(std::string theorem_name) const{return theorem_full_by_name_shared_ptr(theorem_name).get();}
    std::shared_ptr<Text_Environment::Definition::__Content> Text_Environment::Definition::theorem_full_by_name_shared_ptr(std::string content_name) const{for(int i = 0;i<static_cast<int>(a_theorems.size());i++){if(a_theorems.at(i).get()->name == content_name){return a_theorems.at(i);}}return std::shared_ptr<Text_Environment::Definition::__Content>();}

    // Returns a definition by its name
    Text_Environment::Definition* Text_Environment::definition_by_name(std::string definition_name){return definition_by_name_shared_ptr(definition_name).get();}
    std::shared_ptr<Text_Environment::Definition> Text_Environment::definition_by_name_shared_ptr(std::string definition_name){for(int i = 0;i<static_cast<int>(a_definitions.size());i++){if(a_definitions.at(i).get()->name() == definition_name){return a_definitions.at(i);}}return std::shared_ptr<Text_Environment::Definition>();};

    // Text_Environment constructor
    Text_Environment::Text_Environment():scls::Math_Environment(){};

    // Loads the definitions
    void Text_Environment::__load_definition_from_xml(std::shared_ptr<scls::__XML_Text_Base> current_text) {
        // Handle the attributes
        std::vector<scls::XML_Attribute>& attributes = current_text.get()->xml_balise_attributes();
        std::string definition_name = std::string();
        for(int i = 0;i<static_cast<int>(attributes.size());i++) {
            if(attributes[i].name == std::string("name")){definition_name = attributes[i].value;}
        }

        // Create the definition
        if(definition_name != std::string()) {
            std::shared_ptr<Definition> created_definition = new_definition(definition_name);
            for(int i = 0;i<static_cast<int>(current_text.get()->sub_texts().size());i++) {
                if(current_text.get()->sub_texts().at(i).get()->xml_balise_name() == std::string("content")){
                    // Get the balises
                    std::vector<scls::XML_Attribute>& attributes = current_text.get()->sub_texts().at(i).get()->xml_balise_attributes();
                    std::string name = std::string();std::string type = std::string();
                    for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                        if(attributes[i].name == std::string("name")){name = attributes[i].value;}
                        else if(attributes[i].name == std::string("type")){type = attributes[i].value;}
                    }

                    // Add a content for the definition
                    std::string needed_content = current_text.get()->sub_texts().at(i).get()->text();
                    if(type == std::string("theorem")){created_definition.get()->add_theorem(name, needed_content);}
                    else{created_definition.get()->add_content(name, needed_content);}
                }
            }
        }
    }
    void Text_Environment::load_definitions_from_path(std::string path) {
        // Get the entire text
        std::string total = std::string();
        if(std::filesystem::is_directory(path)){
            std::vector<std::string> all_path = scls::directory_content(path, true);
            for(int i = 0;i<static_cast<int>(all_path.size());i++){total+=scls::read_file(all_path.at(i));}
        }
        else if(std::filesystem::exists(path)){total=scls::read_file(path);}
        else{scls::print(std::string("PLEOS Text Environment"), std::string("The path \"") + path + std::string("\" where you want to load definitions does not exists."));return;}

        // Loads the definitions
        std::shared_ptr<scls::_Balise_Style_Container> balises = std::make_shared<scls::_Balise_Style_Container>();load_balises_pleos(balises);
        std::shared_ptr<scls::__XML_Text_Base> content = scls::xml(balises, total);
        for(int i = 0;i<static_cast<int>(content.get()->sub_texts().size());i++) {
            if(content.get()->sub_texts().at(i).get()->xml_balise_name() == std::string("new_definition")){__load_definition_from_xml(content.get()->sub_texts().at(i));}
        }
    }

    // Creates and returns a new definition
    std::shared_ptr<Text_Environment::Definition> Text_Environment::new_definition(std::string definition_name){std::shared_ptr<Text_Environment::Definition> created_definition = std::make_shared<Text_Environment::Definition>(definition_name);a_definitions.push_back(created_definition);return created_definition;}
}
