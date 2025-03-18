//******************
//
// pleos_text.cpp
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
// This file contains the source code of "pleos_text.h".
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

// Include "pleos_text.cpp"
#include "../pleos_text.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //*********
	//
	// PLEOS Text handler
	//
	//*********

	// Creates and returns a linked-list from an std::string
	std::shared_ptr<Linked_List<std::string>> linked_list_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style) {
	    std::shared_ptr<Linked_List<std::string>> to_return;

	    // Handle a lot of balises
	    std::shared_ptr<Linked_List<std::string>> current_list;
        for(int i = 0;i<static_cast<int>(xml->sub_texts().size());i++){
            std::string current_balise_name = xml->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = xml->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "case" || current_balise_name == "element"){
                std::string content = std::string();
                bool ignore_for_placement = false;
                scls::Fraction x = 0;bool x_used = false;
                scls::Fraction y = 0;bool y_used = false;
                for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                    if(attributes[i].name == std::string("content")){content = attributes[i].value;}
                    else if(attributes[i].name == std::string("ignore_for_placement")){ignore_for_placement=true;}
                    else if(attributes[i].name == std::string("x")){x = scls::Fraction::from_std_string(attributes[i].value);x_used=true;}
                    else if(attributes[i].name == std::string("y")){y = scls::Fraction::from_std_string(attributes[i].value);y_used=true;}
                }
                if(to_return.get() == 0) {to_return = std::make_shared<Linked_List<std::string>>();to_return.get()->set_value(content);current_list = to_return;}
                else{current_list = current_list.get()->add_child(content);}
                // Configurate the node
                current_list.get()->set_ignore_for_placement(ignore_for_placement);
                if(x_used){current_list.get()->root()->set_x(x);}
                if(y_used){current_list.get()->set_y(y);}
            }
        }

        // Return the result
        return to_return;
	}

	// Creates and returns a table from an std::string
	std::shared_ptr<Table> table_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style) {
	    std::shared_ptr<Table> to_return = std::make_shared<Table>();
	    scls::Text_Image_Generator tig;

	    // Handle a lot of balises
        for(int i = 0;i<static_cast<int>(xml->sub_texts().size());i++){
            std::string current_balise_name = xml->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = xml->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "case"){
                std::string content = std::string();int x = 0;int y = 0;
                for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                    if(attributes[i].name == std::string("content")){content = attributes[i].value;}
                    else if(attributes[i].name == std::string("x")){x = std::stoi(attributes[i].value);}
                    else if(attributes[i].name == std::string("y")){y = std::stoi(attributes[i].value);}
                }
                to_return.get()->case_at(x, y)->image = tig.image_shared_ptr(content, needed_style);
            }
        }

        // Return the result
        return to_return;
	}

	//Add a node to the graph
	void __graph_add_node(std::shared_ptr<scls::XML_Text> current_text, scls::Text_Style needed_style, Graph<std::string>* graph) {
        // Handle the attributes
        std::string to_add = std::string();
        scls::Fraction x = 0; scls::Fraction y = 0;
        for(int i = 0;i<static_cast<int>(current_text.get()->xml_attributes().size());i++) {
            if(current_text.get()->xml_attributes()[i].name == std::string("name")){to_add = current_text.get()->xml_attributes()[i].value;}
            else if(current_text.get()->xml_attributes()[i].name == std::string("x")){x = scls::Fraction::from_std_string(current_text.get()->xml_attributes()[i].value);}
            else if(current_text.get()->xml_attributes()[i].name == std::string("y")){y = scls::Fraction::from_std_string(current_text.get()->xml_attributes()[i].value);}
        }
        int needed_node = graph->add_node(to_add, x, y);

        // Handle a lot of balises
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string balise_content = current_text->sub_texts()[i].get()->xml_balise();
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "link"){
                // Link the node
                int needed_id = 0;
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    std::string attribute_name = attributes[j].name;
                    std::string attribute_value = attributes[j].value;
                    if(attribute_name == "id") {needed_id = std::stoi(attribute_value);}
                }

                // Add the link
                if(needed_id == needed_node){scls::print(std::string("PLEOS Graph"), std::string("Can't link the ") + std::to_string(needed_id) + std::string(" with himself."));}
                else {graph->link_nodes(needed_node, needed_id);}
            }
        }
	}
	// Add datas to a tree
	void __tree_add_datas(std::shared_ptr<scls::XML_Text> current_text, scls::Text_Style needed_style, Tree<std::string>* tree){
        // Handle the attributes
        std::string to_add = std::string();
        for(int i = 0;i<static_cast<int>(current_text.get()->xml_attributes().size());i++) {
            if(current_text.get()->xml_attributes()[i].name == std::string("name")){to_add = current_text.get()->xml_attributes()[i].value;}
        }

        if(to_add == std::string() && current_text->sub_texts().size() == 1){__tree_add_datas(current_text->sub_texts()[0], needed_style, tree);}
        else {
            // Set the name
            tree->root()->set_style(needed_style);
            tree->set_value(to_add);

            // Handle a lot of balises
            for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
                std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
                std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
                if(current_balise_name == "tree" || current_balise_name == "trees"){
                    // Add the node
                    __tree_add_datas(current_text->sub_texts()[i], needed_style, tree->add_node(std::string()));
                }
                else if(current_balise_name == "node"){
                    // Add the node
                    __graph_add_node(current_text->sub_texts()[i], needed_style, tree->graph());
                }
            }
        }
	}
	// Creates and returns a tree from an std::string
	std::shared_ptr<Tree<std::string>> tree_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style){std::shared_ptr<Tree<std::string>> tree = std::make_shared<Tree<std::string>>();__tree_add_datas(xml, needed_style, tree.get());return tree;};

	// Generate a word
	std::shared_ptr<scls::Image> generate_text_image(std::shared_ptr<scls::XML_Text> current_text, std::shared_ptr<scls::Text_Style> needed_style){
	    std::string balise_content = current_text.get()->xml_balise();
        std::string current_balise_name = current_text.get()->xml_balise_name();
        std::shared_ptr<scls::Image> to_return;
        if(current_balise_name == "graphic") {
            // Generate a graphic
            Graphic graphic = Graphic();

            // Get the datas about the graphic
            scls::Fraction graphic_height = 10;scls::Fraction graphic_width = 10;
            // Handle a lot of balises
            for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++) {
                std::string balise_content = current_text->sub_texts()[i].get()->xml_balise();
                std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
                std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
                if(current_balise_name == "background_color") {graphic.set_background_color(scls::Color::from_xml(current_text->sub_texts()[i]));}
                else if(current_balise_name == "base") {
                    // Get the datas about the base of the graphic
                    for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                        if(attributes[j].name == "height") {graphic_height = scls::Fraction::from_std_string(attributes[j].value).to_double();}
                        else if(attributes[j].name == "width") {graphic_width = scls::Fraction::from_std_string(attributes[j].value).to_double();}
                    }
                }
                else if(current_balise_name == "form") {
                    // Get the datas about a vector of the graphic
                    std::string needed_name = std::string();std::string needed_points = std::string();
                    for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                        if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                        else if(attributes[j].name == "points") {needed_points = attributes[j].value;}
                    }
                    // Add the form
                    graphic.set_form_points(graphic.new_form(needed_name), needed_points);
                }
                else if(current_balise_name == "point" || current_balise_name == "vec") {
                    // Get the datas about a vector of the graphic
                    std::string needed_name = std::string();scls::Fraction needed_x = 0;scls::Fraction needed_y = 0;
                    for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                        if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                        else if(attributes[j].name == "x") {
                            // X of the vector
                            needed_x = scls::Fraction::from_std_string(attributes[j].value).to_double();
                        }
                        else if(attributes[j].name == "y") {
                            // Y of the vector
                            needed_y = scls::Fraction::from_std_string(attributes[j].value).to_double();
                        }
                    }
                    // Add the vector
                    if(current_balise_name == "point"){graphic.add_point(Vector(needed_name, needed_x, needed_y));}
                    else if(current_balise_name == "vec"){graphic.add_vector(Vector(needed_name, needed_x, needed_y));}
                }
            }
            // Set the datas
            graphic.set_scale(graphic_width.to_double(), graphic_height.to_double());

            // Get the image
            to_return = graphic.to_image(200, 200);
            to_return.get()->draw_border(1, 1, 1, 1, scls::Color(0, 0, 0));
        }
        else if(current_balise_name == "linked_list"){to_return = linked_list_from_xml(current_text, *needed_style.get()).get()->to_image();}
        else if(current_balise_name == "table") {to_return = table_from_xml(current_text, *needed_style.get()).get()->to_image();}
        else if(current_balise_name == "tree") {to_return = tree_from_xml(current_text, *needed_style.get()).get()->to_image();}

        return to_return;
	}
    void __Text_Line::generate_word(std::shared_ptr<scls::XML_Text> current_text, unsigned int& current_position_in_plain_text, std::shared_ptr<scls::Text_Style> needed_style, std::shared_ptr<scls::Text_Image_Word>& word_to_add) {
        std::string balise_content = current_text.get()->xml_balise();
        std::string current_balise_name = current_text.get()->xml_balise_name();
        if(current_balise_name == "graphic" || current_balise_name == "linked_list" || current_balise_name == "table" || current_balise_name == "tree") {
            // Get the image
            std::shared_ptr<scls::Image> src_img = generate_text_image(current_text, needed_style);

            // Change the image
            int height = -1; int width = -1;
            if(height == -1 && width == -1) {height = src_img.get()->height();width = src_img.get()->width();}
            else if(height != -1 && width == -1) {
                width = static_cast<int>(static_cast<double>(height) * (static_cast<double>(src_img.get()->width()) / static_cast<double>(src_img.get()->height())));
            } else if(height == -1 && width != -1) {
                height = static_cast<int>(static_cast<double>(width) * (static_cast<double>(src_img.get()->height()) / static_cast<double>(src_img.get()->width())));
            }
            if(global_style()->max_width > 0 && width > global_style()->max_width) {
                width = global_style()->max_width;
                height = static_cast<int>(static_cast<double>(width) * (static_cast<double>(src_img.get()->height()) / static_cast<double>(src_img.get()->width())));
            }
            __generate_image(word_to_add, src_img, current_position_in_plain_text, a_current_width, height, width);
        }
        else{scls::Text_Image_Line::generate_word(current_text, current_position_in_plain_text, needed_style, word_to_add);}
    }
}
