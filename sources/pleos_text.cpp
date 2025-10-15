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
        std::shared_ptr<Graphic> needed_graphic = graphic_from_xml(xml, needed_style, graphic_width_in_pixel, graphic_height_in_pixel);
        std::shared_ptr<scls::__Image_Base> to_return = needed_graphic.get()->to_image(graphic_width_in_pixel, graphic_height_in_pixel);
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

	// Add a node to the graph
	void __graph_add_node_late_balises(int needed_node, std::shared_ptr<scls::__XML_Text_Base> current_text, scls::Text_Style needed_style, Graph<std::string>* graph){
	    // Handle possible attributes
	    std::vector<scls::XML_Attribute>& attributes = current_text.get()->xml_balise_attributes();
	    for(int i = 0;i<static_cast<int>(attributes.size());i++) {
            if(attributes.at(i).name == std::string("link") || attributes.at(i).name == std::string("links")){
                std::vector<std::string> cutted = scls::cut_string(attributes.at(i).value, std::string(";"));
                for(int j = 0;j<static_cast<int>(cutted.size());j++) {
                    int needed_id = std::stoi(cutted.at(j));std::string needed_ponderation = std::string();
                    if(needed_id == needed_node){scls::print(std::string("PLEOS Graph"), std::string("Can't link the ") + std::to_string(needed_id) + std::string(" with himself."));}
                    else {
                        graph->link_nodes(needed_node, needed_id);
                        graph->set_link_ponderation(needed_node, needed_id, needed_ponderation, needed_style);
                    }
                }
            }
	    }

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
        if(current_text.get()->only_text()){to_add = current_text.get()->text();}
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
	bool is_special_pleos_balise(std::string name){return name == std::string("definition") || name == std::string("graph") || name == std::string("graphic") || name == std::string("linked_list") || name == std::string("poly") || name == std::string("table") || name == std::string("theorem") || name == std::string("tree");}
	std::shared_ptr<scls::__Image_Base> generate_text_image(std::shared_ptr<scls::__XML_Text_Base> current_text, scls::Text_Style needed_style, std::shared_ptr<Text_Environment> possible_environment){return generate_text_image(current_text, needed_style, std::shared_ptr<scls::__XML_Text_Base>(), possible_environment);}
	std::shared_ptr<scls::__Image_Base> generate_text_image(std::shared_ptr<scls::__XML_Text_Base> current_text, scls::Text_Style needed_style, std::shared_ptr<scls::__XML_Text_Base> parent_text, std::shared_ptr<Text_Environment> possible_environment){
	    std::string balise_content = current_text.get()->xml_balise();
        std::string current_balise_name = current_text.get()->xml_balise_name();
        std::shared_ptr<scls::__Image_Base> to_return;

        // Handle style balises
        needed_style = needed_style.new_child();
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string balise_content = current_text->sub_texts()[i].get()->xml_balise();
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            //std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "border") {scls::border_from_xml(current_text->sub_texts()[i], needed_style);}
            else if(current_balise_name == "padding") {scls::padding_from_xml(current_text->sub_texts()[i], needed_style);}
        }

        // Create the image
        if(current_balise_name == "graph") {to_return = graph_from_xml(current_text, needed_style).get()->to_image();}
        else if(current_balise_name == "graphic") {to_return = graphic_image_from_xml(current_text, needed_style);}
        else if(current_balise_name == "linked_list"){std::shared_ptr<Linked_List> l = linked_list_from_xml(current_text, needed_style);if(l == 0){scls::print("Warning", std::string("PLEOS"), std::string("Can't create a linked list."));}else{to_return = l.get()->to_image(needed_style);}}
        else if(current_balise_name == "table") {to_return = table_from_xml<Table>(current_text, needed_style).get()->to_image().image_shared_ptr();}
        else if(current_balise_name == "tree") {to_return = tree_from_xml(current_text, needed_style).get()->to_image();}
        // Format text
        else if(current_balise_name == "definition" || current_balise_name == "theorem") {
            // Handle the attributes
            std::vector<scls::XML_Attribute>& attributes = current_text.get()->xml_balise_attributes();
            std::string definition_name = std::string();std::string definition_content_name = std::string();
            for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                if(attributes[i].name == std::string("name")){definition_name = attributes[i].value;}
            }

            // Get the definition
            std::vector<std::string> cutted = scls::cut_string(definition_name, std::string("-"));
            if(cutted.size() > 0){definition_name = cutted.at(0);if(cutted.size() > 1){definition_content_name = cutted.at(1);}}
            Text_Environment::Definition* needed_definition = possible_environment.get()->definition_by_name(definition_name);
            if(needed_definition == 0) {scls::print("PLEOS Text", std::string("Unexisting definition for \"") + definition_name + std::string("\"."));}
            else{
                // Check capitalisation
                bool capitalise = true;
                scls::__XML_Text_Base* balise = current_text.get()->first_balise_at_left();
                if(balise != 0 && balise->only_text()){
                    std::string needed_text = balise->text();
                    if(needed_text.size() > 2 && !(needed_text.at(needed_text.size() - 1) == '.' || (needed_text.at(needed_text.size() - 2) == '.'))){
                        capitalise=false;
                    }
                }

                // Add the good object
                std::string needed_content = std::string();
                if(current_balise_name == "definition") {needed_content = needed_definition->content(definition_content_name, capitalise);if(needed_content == std::string()){scls::print("PLEOS Text Environment", std::string("This text environment has no definitions loaded for \"") + definition_name + std::string("\"."));}}
                else if(current_balise_name == "theorem") {needed_content = needed_definition->theorem(definition_content_name, capitalise);if(needed_content == std::string()){scls::print("PLEOS Text Environment", std::string("This text environment has no theorems loaded for \"") + definition_name + std::string("\"."));}}
                current_text.get()->set_xml_balise_name("span");current_text.get()->set_text(needed_content);
                utf_8_symbol_xml(current_text, true); // A RE REFLECHIR
            }
        }
        else if(current_balise_name == std::string("poly")) {
            // Handle the attributes
            std::vector<scls::XML_Attribute>& attributes = current_text.get()->xml_balise_attributes();
            std::string coefficients = std::string();std::string definition_content_name = std::string();
            for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                if(attributes[i].name == std::string("m")){coefficients = attributes[i].value;}
            }

            // Get the good value
            std::vector<std::string> cutted = scls::cut_string(coefficients, std::string(";"));
            std::string result = std::string();
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                std::vector<std::string> parts = scls::cut_string(cutted.at(i), ":");
                std::string current_coefficient = std::string("1");
                if(parts.size() == 1){current_coefficient = parts.at(0);}

                // Asserts
                bool full_coefficient_digit = true;
                for(int j = 0;j<static_cast<int>(current_coefficient.size());j++){if(!(std::isdigit(current_coefficient.at(j)) || current_coefficient.at(j) == '.')){full_coefficient_digit=false;break;}}
                if(current_coefficient == std::string() || (full_coefficient_digit && std::stoi(parts.at(0)) == 0)){continue;}
                else if(current_coefficient == std::string("1")){current_coefficient = std::string();}

                // Update the result
                if(result != std::string()){result += std::string("<mo>+</mo>");}
                if(current_coefficient != std::string()){result += std::string("<mi>") + current_coefficient + std::string("</mi>");}
            }

            // Set the result
            current_text.get()->set_xml_balise_name(std::string("math"));
            current_text.get()->set_text(result);
            std::cout << "G " << current_text.get()->full_text() << std::endl;
        }

        return to_return;
	}
    std::shared_ptr<scls::Text_Image_Block> Text::__generate_block(std::shared_ptr<scls::Block_Datas> block_datas) {
        // Get the datas about the block
        std::shared_ptr<scls::__XML_Text_Base> current_text = block_datas.get()->content;
        scls::Text_Style needed_style = block_datas.get()->global_style;

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
            if(global_style().max_width() > 0 && width > global_style().max_width()) {
                width = global_style().max_width();
                height = static_cast<int>(static_cast<double>(width) * (static_cast<double>(src_img.get()->height()) / static_cast<double>(src_img.get()->width())));
            }

            // Generate the image
            std::shared_ptr<scls::Text_Image_Block> to_return = scls::Text_Image_Block::__generate_block(block_datas, src_img);
            return to_return;
        }

        // Normal block
        return scls::Text_Image_Block::__generate_block(block_datas);
    }

    // Loads the needed balises
    void Text::load_balises(std::shared_ptr<scls::_Balise_Style_Container> defined_balises) {load_balises_pleos(defined_balises);}

    // Set the value of an std::string case
    std::shared_ptr<scls::__Image_Base> Table::case_image_from_text(std::string value, scls::Text_Style needed_style, scls::Text_Image_Generator* tig){return tig->image_shared_ptr<Text>(value, needed_style);}

    //*********
    // GUI Handling
    //*********

    // Creates a text block from a block of text
    std::shared_ptr<scls::__GUI_Text_Metadatas::__GUI_Text_Block> GUI_Text::__create_text_block_object(scls::Text_Image_Block* block_to_apply) {
        if(block_to_apply->datas()->content.get()->xml_balise_name() == "graphic"){return scls::__GUI_Text_Metadatas::__create_text_block_object<GUI_Text::__GUI_Text_Block_Graphic, Graphic_Object>();}
        return __GUI_Text_Metadatas::__create_text_block_object(block_to_apply);
    };

    // Updates the texture of the block
    void GUI_Text::__GUI_Text_Block_Graphic::update_texture(std::shared_ptr<scls::Text_Image_Block> block_to_apply, scls::Image_Generation_Type generation_type){
        int height=0;int width=0;
        graphic()->set_style(block_to_apply.get()->global_style());
        graphic()->graphic_from_xml(block_to_apply.get()->datas()->content, block_to_apply.get()->global_style(), width, height);

        // Update the size
        graphic_object()->set_height_in_pixel(height);
        graphic_object()->set_width_in_pixel(width);

        // Update the texture
        graphic_object()->update_texture();
    };

    // Returns the image of a text
    scls::Image string_to_image(std::string content){scls::Text_Style style;return string_to_image(content, style);}
    scls::Image string_to_image(std::string content, scls::Text_Style style){
        // Format content
        content = scls::format_for_xml(content);

        std::shared_ptr<scls::_Balise_Style_Container> balises = std::make_shared<scls::_Balise_Style_Container>();
        load_balises_pleos(balises);
        pleos::Text t = pleos::Text(balises, content, style);
        std::shared_ptr<scls::__Image_Base> to_return = t.image_shared_pointer();
        return to_return;
    }
}
