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
        std::shared_ptr<scls::Balise_Style_Datas> current_balise = std::make_shared<scls::Balise_Style_Datas>();
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
        current_balise.get()->style.get()->set_border_width(1);
        current_balise.get()->style.get()->set_margin_bottom(16);
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
        current_balise.get()->style.get()->set_margin_bottom(16);
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

	// Returns a formula value
    scls::__Formula_Base::Formula Text_Environment::value_formula(std::string base)const{scls::__Formula_Base formula = scls::string_to_formula(base);return formula.replace_unknowns(a_unknowns.get());}
    // Returns a number value
    scls::Fraction Text_Environment::value_number(std::string base)const{scls::__Formula_Base formula = scls::string_to_formula(base);return formula.value(a_unknowns.get()).real();}
    // Returns a Point_2D value
    scls::Point_2D Text_Environment::value_point_2d(std::string base)const{
        // Format the text
        while(base.size() > 0 && base.at(0) == '('){base = base.substr(1, base.size() - 1);}
        while(base.size() > 0 && base.at(base.size() - 1) == ')'){base = base.substr(0, base.size() - 1);}

        // Get the point
        base = scls::replace(base, std::string(";"), std::string(","));
        std::vector<std::string> cutted = scls::cut_string(base, std::string(","));
        if(cutted.size() != 2) {scls::print(std::string("PLEOS Text Environment"), std::string("Can't get a point 2D from \"") + base + std::string("\"."));return scls::Point_2D(0, 0);}
        return scls::Point_2D(value_number(cutted.at(0)), value_number(cutted.at(1)));
    };

    // Handle unknowns
    // Creates a unknown
    scls::__Formula_Base::Unknown* Text_Environment::create_unknown(std::string name){return a_unknowns.get()->create_unknown(name);};
    std::shared_ptr<scls::__Formula_Base::Unknown> Text_Environment::create_unknown_shared_ptr(std::string name){return a_unknowns.get()->create_unknown_shared_ptr(name);};
    // Returns a value by its name
    scls::Fraction Text_Environment::value_by_name(std::string name)const{scls::__Formula_Base::Unknown*unknow=unknown_by_name(name);if(unknow==0){return 0;}return (unknow->value.get()->value(0).real());};
    // Returns a unknown by its name
    scls::__Formula_Base::Unknown* Text_Environment::unknown_by_name(std::string name)const{return a_unknowns.get()->unknown_by_name(name);};
    std::shared_ptr<scls::__Formula_Base::Unknown> Text_Environment::unknown_shared_ptr_by_name(std::string name)const{return a_unknowns.get()->unknown_shared_ptr_by_name(name);};

    //*********
    // Definition system
    //*********

    void Text_Environment::Definition::add_content(std::string content){if(a_contents.size() <= 0){a_contents.push_back(content);}else{a_contents[0]=content;}};
    std::string Text_Environment::Definition::content(bool capitalise_first_letter)const{
        std::string to_return = std::string();
        if(a_contents.size()>0){to_return = a_contents.at(0);}
        if(capitalise_first_letter&&std::isalpha(to_return.at(0))){to_return=scls::capitalise_letter(to_return,0);}
        return to_return;
    };

    // Returns a definition by its name
    Text_Environment::Definition* Text_Environment::definition_by_name(std::string definition_name){return definition_by_name_shared_ptr(definition_name).get();}
    std::shared_ptr<Text_Environment::Definition> Text_Environment::definition_by_name_shared_ptr(std::string definition_name){for(int i = 0;i<static_cast<int>(a_definitions.size());i++){if(a_definitions.at(i).get()->name() == definition_name){return a_definitions.at(i);}}return std::shared_ptr<Text_Environment::Definition>();};

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
                    // Add a content for the definition
                    std::string needed_content = current_text.get()->sub_texts().at(i).get()->text();
                    created_definition.get()->add_content(needed_content);
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
