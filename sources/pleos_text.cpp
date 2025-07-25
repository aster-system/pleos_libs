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

	// Creates and returns a graphic from an std::string
	std::shared_ptr<Graphic> graphic_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style, int& graphic_width_in_pixel, int& graphic_height_in_pixel) {std::shared_ptr<Graphic> to_return = Graphic::new_graphic();to_return.get()->graphic_from_xml(xml, needed_style, graphic_width_in_pixel, graphic_height_in_pixel);return to_return;}
	std::shared_ptr<scls::__Image_Base> graphic_image_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style){
	    int graphic_width_in_pixel = 200;int graphic_height_in_pixel = 200;
	    std::shared_ptr<scls::__Image_Base> to_return = graphic_from_xml(xml, needed_style, graphic_width_in_pixel, graphic_height_in_pixel).get()->to_image(graphic_width_in_pixel, graphic_height_in_pixel);
        return to_return;
	}

	// Creates and returns a linked-list from an std::string
	std::shared_ptr<Linked_List> linked_list_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style) {
	    std::shared_ptr<Linked_List> to_return;

	    // Handle a lot of balises
	    std::shared_ptr<Linked_List> current_list;
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
                if(to_return.get() == 0) {to_return = std::make_shared<Linked_List>();to_return.get()->set_image(content);current_list = to_return;}
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
	std::shared_ptr<Table> table_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style) {
	    std::shared_ptr<Table> to_return = std::make_shared<Table>();
	    scls::Text_Image_Generator tig;

	    // Handle the attributes
	    std::vector<scls::XML_Attribute>& attributes = xml.get()->xml_balise_attributes();
	    for(int i = 0;i<static_cast<int>(attributes.size());i++) {
            if(attributes[i].name == std::string("font_size")){needed_style.set_font_size(std::stoi(attributes[i].value));}
            else if(attributes[i].name == std::string("minimum_case_width")){to_return.get()->set_minimum_case_width(std::stoi(attributes[i].value));}
            else if(attributes[i].name == std::string("title")){to_return.get()->set_title(attributes[i].value);}
	    }

	    // Handle a lot of balises
        for(int i = 0;i<static_cast<int>(xml->sub_texts().size());i++){
            std::string current_balise_name = xml->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = xml->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "case" || current_balise_name == "case_plus"){
                scls::Color background_color = scls::Color(255, 255, 255);std::string content = std::string();
                scls::Text_Style case_style = needed_style;case_style.set_border_width(0);
                int height = 1;int width = 1;bool right_border = true;int x = 0;int y = 0;
                for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                    if(!scls::text_style_from_xml_attribute(&attributes[i], &case_style)) {
                        if(attributes[i].name == std::string("background_color")){background_color = scls::Color::from_std_string(attributes[i].value);}
                        else if(attributes[i].name == std::string("content")){content = attributes[i].value;}
                        else if(attributes[i].name == std::string("height")){height = std::stoi(attributes[i].value);}
                        else if(attributes[i].name == std::string("right_border")){if(attributes[i].value==std::string("0")||attributes[i].value==std::string("false")||attributes[i].value==std::string("no")){right_border=false;}}
                        else if(attributes[i].name == std::string("width")){width = std::stoi(attributes[i].value);}
                        else if(attributes[i].name == std::string("x")){x = std::stoi(attributes[i].value);}
                        else if(attributes[i].name == std::string("y")){y = std::stoi(attributes[i].value);}
                    }
                }

                // Create the result
                if(current_balise_name == "case_plus"){
                    content = xml->sub_texts()[i].get()->text();
                    if(case_style.max_width == -1){case_style.max_width = to_return.get()->column_width(x, width);}
                    (*to_return.get()->case_at(x, y)->image.get()) = tig.image_shared_ptr<Text>(content, case_style);
                }
                else{(*to_return.get()->case_at(x, y)->image.get()) = tig.image_shared_ptr(content, case_style);}
                to_return.get()->case_at(x, y)->right_border = right_border;
                to_return.get()->case_at(x, y)->set_background_color(background_color);
                to_return.get()->merge_cases(x, y, width, height);
            }
        }

        // Return the result
        return to_return;
	}

	// Add a node to the graph
	void __graph_add_node_late_balises(int needed_node, std::shared_ptr<scls::__XML_Text_Base> current_text, scls::Text_Style needed_style, Graph<std::string>* graph){
	    // Handle the other balises
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string balise_content = current_text->sub_texts()[i].get()->xml_balise();
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "link"){
                // Link the node
                int needed_id = 0;std::string needed_ponderation = std::string();
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    std::string attribute_name = attributes[j].name;
                    std::string attribute_value = attributes[j].value;
                    if(attribute_name == "id") {needed_id = std::stoi(attribute_value);}
                    else if(attribute_name == "value") {needed_ponderation = attribute_value;}
                }

                // Add the link
                if(needed_id == needed_node){scls::print(std::string("PLEOS Graph"), std::string("Can't link the ") + std::to_string(needed_id) + std::string(" with himself."));}
                else {
                    graph->link_nodes(needed_node, needed_id);
                    graph->set_link_ponderation(needed_node, needed_id, needed_ponderation, needed_style);
                }
            }
        }
	}
	void __graph_add_node(std::shared_ptr<scls::__XML_Text_Base> current_text, scls::Text_Style needed_style, Graph<std::string>* graph) {
        // Handle the attributes
        std::string to_add = std::string();
        scls::Fraction x = 0; scls::Fraction y = 0;
        for(int i = 0;i<static_cast<int>(current_text.get()->xml_attributes().size());i++) {
            if(current_text.get()->xml_attributes()[i].name == std::string("name")){to_add = current_text.get()->xml_attributes()[i].value;}
            else if(current_text.get()->xml_attributes()[i].name == std::string("x")){x = scls::Fraction::from_std_string(current_text.get()->xml_attributes()[i].value);}
            else if(current_text.get()->xml_attributes()[i].name == std::string("y")){y = scls::Fraction::from_std_string(current_text.get()->xml_attributes()[i].value);}
        }

        // Handle a lot of balises
        // Handle the content
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string balise_content = current_text->sub_texts()[i].get()->xml_balise();
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            //std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "content"){to_add = current_text->sub_texts()[i].get()->text();}
        }

        // Creates the node
        //int needed_node =
        graph->add_node(to_add, x, y);
	}
	void __graph_add_datas(std::shared_ptr<scls::__XML_Text_Base> current_text, scls::Text_Style needed_style, Graph<std::string>* graph){
        // Handle a lot of balises
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            //std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "node"){__graph_add_node(current_text->sub_texts()[i], needed_style, graph);}
        }

        // Handle a lot of late balises
       int current_node = 0;
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            if(current_balise_name == "node"){__graph_add_node_late_balises(current_node, current_text->sub_texts()[i], needed_style, graph);current_node++;}
        }
	}
	// Add datas to a tree
	void __tree_stern_brocot(int depht, std::shared_ptr<std::vector<int>> numerator_ptr, std::shared_ptr<std::vector<int>> denominator_ptr, std::shared_ptr<std::vector<Tree<std::string>*>> trees){
	    // Create the first nodes
	    std::shared_ptr<std::vector<int>> new_denominator = std::make_shared<std::vector<int>>();
	    std::shared_ptr<std::vector<int>> new_numerator = std::make_shared<std::vector<int>>();
	    std::shared_ptr<std::vector<Tree<std::string>*>> new_trees = std::make_shared<std::vector<Tree<std::string>*>>();
	    // First fraction first node
	    int current_denominator = denominator_ptr->at(0) + 1;new_denominator.get()->push_back(current_denominator);
	    int current_numerator = numerator_ptr->at(0);new_numerator.get()->push_back(current_numerator);
	    new_trees.get()->push_back(trees.get()->at(0)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
	    new_denominator.get()->push_back(denominator_ptr->at(0));new_numerator.get()->push_back(numerator_ptr->at(0));
	    if(trees.get()->size() == 1) {
            // First fraction last node
            current_denominator = denominator_ptr->at(0);new_denominator.get()->push_back(current_denominator);
            current_numerator = numerator_ptr->at(0) + 1;new_numerator.get()->push_back(current_numerator);
            new_trees.get()->push_back(trees.get()->at(0)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
	    }
	    else {
            // First fraction last node
            current_denominator = denominator_ptr->at(0) + denominator_ptr->at(1);new_denominator.get()->push_back(current_denominator);
            current_numerator = numerator_ptr->at(0) + numerator_ptr->at(1);new_numerator.get()->push_back(current_numerator);
            new_trees.get()->push_back(trees.get()->at(0)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
            new_denominator.get()->push_back(denominator_ptr->at(1));new_numerator.get()->push_back(numerator_ptr->at(1));

            // Create each nodes
            for(int i = 1;i<static_cast<int>(trees.get()->size()) - 1;i++) {
                // Left
                current_denominator = denominator_ptr->at(i * 2 - 1) + denominator_ptr->at(i * 2);new_denominator.get()->push_back(current_denominator);
                current_numerator = numerator_ptr->at(i * 2 - 1) + numerator_ptr->at(i * 2);new_numerator.get()->push_back(current_numerator);
                new_trees.get()->push_back(trees.get()->at(i)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
                new_denominator.get()->push_back(denominator_ptr->at(i * 2));new_numerator.get()->push_back(numerator_ptr->at(i * 2));
                // Right
                current_denominator = denominator_ptr->at(i * 2) + denominator_ptr->at(i * 2 + 1);new_denominator.get()->push_back(current_denominator);
                current_numerator = numerator_ptr->at(i * 2) + numerator_ptr->at(i * 2 + 1);new_numerator.get()->push_back(current_numerator);
                new_trees.get()->push_back(trees.get()->at(i)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
                new_denominator.get()->push_back(denominator_ptr->at(i * 2 + 1));new_numerator.get()->push_back(numerator_ptr->at(i * 2 + 1));
            }

            // Create the last fraction first node
            current_denominator = denominator_ptr->at(denominator_ptr.get()->size() - 2) + denominator_ptr->at(denominator_ptr.get()->size() - 1);new_denominator.get()->push_back(current_denominator);
            current_numerator = numerator_ptr->at(numerator_ptr.get()->size() - 2) + numerator_ptr->at(numerator_ptr.get()->size() - 1);new_numerator.get()->push_back(current_numerator);
            new_trees.get()->push_back(trees.get()->at(trees.get()->size() - 1)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
            new_denominator.get()->push_back(denominator_ptr->at(denominator_ptr.get()->size() - 1));new_numerator.get()->push_back(numerator_ptr.get()->at(numerator_ptr.get()->size() - 1));
            // Create the last fraction last node
            current_denominator = denominator_ptr->at(denominator_ptr.get()->size() - 1);new_denominator.get()->push_back(current_denominator);
            current_numerator = numerator_ptr->at(numerator_ptr.get()->size() - 1) + 1;new_numerator.get()->push_back(current_numerator);
            new_trees.get()->push_back(trees.get()->at(trees.get()->size() - 1)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
	    }

	    // Recursion
        if(depht > 0) {__tree_stern_brocot(depht - 1, new_numerator, new_denominator, new_trees);}
	}
	void __tree_stern_brocot(Tree<std::string>* tree, int depht){
	    std::shared_ptr<std::vector<int>> denominator = std::make_shared<std::vector<int>>(1, 1);std::shared_ptr<std::vector<int>> numerator = std::make_shared<std::vector<int>>(1, 1);
	    __tree_stern_brocot(depht, numerator, denominator, std::make_shared<std::vector<Tree<std::string>*>>(1, tree));
    };
	void __tree_add_datas(std::shared_ptr<scls::__XML_Text_Base> current_text, scls::Text_Style needed_style, Tree<std::string>* tree){
        // Handle the attributes
        std::string to_add = std::string();std::string to_load = std::string();
        for(int i = 0;i<static_cast<int>(current_text.get()->xml_attributes().size());i++) {
            if(current_text.get()->xml_attributes()[i].name == std::string("load")){to_load = current_text.get()->xml_attributes()[i].value;}
            else if(current_text.get()->xml_attributes()[i].name == std::string("name")){to_add = current_text.get()->xml_attributes()[i].value;}
        }

        // Loads a precise tree
        if(to_load.size() >= 12 && to_load.substr(0, 12) == std::string("stern_brocot")) {
            // Get the needed value
            to_load = to_load.substr(12, to_load.size() - 12);
            while(to_load.size() > 0 && to_load[0] == '('){to_load = to_load.substr(1, to_load.size() - 1);}
            while(to_load.size() > 0 && to_load[to_load.size() - 1] == ')'){to_load = to_load.substr(0, to_load.size() - 1);}
            int depht = 1;if(to_load.size() > 0){depht = scls::string_to_double(to_load);};
            if(depht < 0){depht = 1;}

            // Create the tree
            if(to_add == std::string()){to_add = std::string("0");}
            __tree_stern_brocot(tree, depht - 1);
        }

        if(to_add == std::string() && current_text->sub_texts().size() == 1){__tree_add_datas(current_text->sub_texts()[0], needed_style, tree);}
        else {
            // Set the name
            tree->root()->set_style(needed_style);
            tree->set_image(to_add);

            // Handle a lot of balises
            for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
                std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
                //std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
                if(current_balise_name == "tree" || current_balise_name == "trees"){__tree_add_datas(current_text->sub_texts()[i], needed_style, tree->add_node(std::string()));}
                else if(current_balise_name == "node"){__graph_add_node(current_text->sub_texts()[i], needed_style, tree->graph());}
            }
        }
	}
	// Creates and returns a graph / tree from an std::string
	std::shared_ptr<Graph<std::string>> graph_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style){std::shared_ptr<Graph<std::string>> graph = std::make_shared<Graph<std::string>>();__graph_add_datas(xml, needed_style, graph.get());return graph;}
	std::shared_ptr<Tree<std::string>> tree_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style){std::shared_ptr<Tree<std::string>> tree = std::make_shared<Tree<std::string>>();__tree_add_datas(xml, needed_style, tree.get());return tree;}

	// Generate a word
	bool is_special_pleos_balise(std::string name){return name == std::string("definition") || name == std::string("graph") || name == std::string("graphic") || name == std::string("linked_list") || name == std::string("table") || name == std::string("tree");}
	std::shared_ptr<scls::__Image_Base> generate_text_image(std::shared_ptr<scls::__XML_Text_Base> current_text, std::shared_ptr<scls::Text_Style> needed_style, std::shared_ptr<Text_Environment> possible_environment){return generate_text_image(current_text, needed_style, std::shared_ptr<scls::__XML_Text_Base>(), possible_environment);}
	std::shared_ptr<scls::__Image_Base> generate_text_image(std::shared_ptr<scls::__XML_Text_Base> current_text, std::shared_ptr<scls::Text_Style> needed_style, std::shared_ptr<scls::__XML_Text_Base> parent_text, std::shared_ptr<Text_Environment> possible_environment){
	    std::string balise_content = current_text.get()->xml_balise();
        std::string current_balise_name = current_text.get()->xml_balise_name();
        std::shared_ptr<scls::__Image_Base> to_return;

        // Handle style balises
        needed_style = needed_style.get()->new_child();
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string balise_content = current_text->sub_texts()[i].get()->xml_balise();
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            //std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "border") {scls::border_from_xml(current_text->sub_texts()[i], needed_style.get());}
            else if(current_balise_name == "padding") {scls::padding_from_xml(current_text->sub_texts()[i], needed_style.get());}
        }

        // Create the image
        if(current_balise_name == "graph") {to_return = graph_from_xml(current_text, *needed_style.get()).get()->to_image();}
        else if(current_balise_name == "graphic") {to_return = graphic_image_from_xml(current_text, *needed_style.get());}
        else if(current_balise_name == "linked_list"){to_return = linked_list_from_xml(current_text, *needed_style.get()).get()->to_image(needed_style);}
        else if(current_balise_name == "table") {to_return = table_from_xml(current_text, *needed_style.get()).get()->to_image();}
        else if(current_balise_name == "tree") {to_return = tree_from_xml(current_text, *needed_style.get()).get()->to_image();}
        // Format text
        else if(current_balise_name == "definition") {
            // Handle the attributes
            std::vector<scls::XML_Attribute>& attributes = current_text.get()->xml_balise_attributes();
            std::string definition_name = std::string();
            for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                if(attributes[i].name == std::string("name")){definition_name = attributes[i].value;}
            }

            // Get the definition
            Text_Environment::Definition* needed_definition = possible_environment.get()->definition_by_name(definition_name);
            if(needed_definition == 0) {scls::print("PLEOS Text", std::string("Unexisting definition for \"") + definition_name + std::string("\"."));}
            else{
                // Check capitalisation
                bool capitalise = true;
                scls::__XML_Text_Base* balise = current_text.get()->first_balise_at_left();
                if(balise->only_text()){std::string needed_text = balise->text();if(!(needed_text.at(needed_text.size() - 1) == '.' || (needed_text.at(needed_text.size() - 2) == '.'))){capitalise=false;}}
                current_text.get()->set_text(needed_definition->content(capitalise));
            }
        }

        return to_return;
	}
    void __Text_Line::generate_word(std::shared_ptr<scls::__XML_Text_Base> current_text, unsigned int& current_position_in_plain_text, std::shared_ptr<scls::Text_Style> needed_style, std::shared_ptr<scls::Text_Image_Word>& word_to_add) {
        std::string balise_content = current_text.get()->xml_balise();
        std::string current_balise_name = current_text.get()->xml_balise_name();
        if(is_special_pleos_balise(current_balise_name)) {
            // Get the image
            std::shared_ptr<scls::__Image_Base> src_img = generate_text_image(current_text, needed_style, std::shared_ptr<Text_Environment>());

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

    // Loads the needed balises
    void Text::load_balises(std::shared_ptr<scls::_Balise_Style_Container> defined_balises) {load_balises_pleos(defined_balises);}

    //*********
    // GUI Handling
    //*********

    // Creates a text block from a block of text
    std::shared_ptr<scls::__GUI_Text_Metadatas::__GUI_Text_Block> GUI_Text::__create_text_block_object(scls::Text_Image_Block* block_to_apply) {
        if(block_to_apply->datas()->content.get()->xml_balise_name() == "graphic"){return scls::__GUI_Text_Metadatas::__create_text_block_object<GUI_Text::__GUI_Text_Block_Graphic, Graphic_Object>();}
        return __GUI_Text_Metadatas::__create_text_block_object(block_to_apply);
    };

    // Updates the texture of the block
    void GUI_Text::__GUI_Text_Block_Graphic::update_texture(scls::Text_Image_Block* block_to_apply, scls::Image_Generation_Type generation_type){
        int height=0;int width=0;
        graphic()->set_style(*block_to_apply->global_style());
        graphic()->graphic_from_xml(block_to_apply->datas()->content, *block_to_apply->global_style(), width, height);

        // Update the size
        graphic_object()->set_height_in_pixel(height);
        graphic_object()->set_width_in_pixel(width);

        // Update the texture
        graphic_object()->update_texture();
    };
}
