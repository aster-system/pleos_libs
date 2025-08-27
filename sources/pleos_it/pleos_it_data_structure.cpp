//******************
//
// pleos_it_data_structure.cpp
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
// This file contains the source code of pleos_it_data_structure.h.
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

// Include pleos_it_data_structure.h
#include "../../pleos_it/pleos_it_data_structure.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Current node style
    scls::Text_Style __current_node_style = scls::Text_Style();

    //******************
    // __Graph_Base node
    //******************

    // __Node_Base constructor
    __Graph_Base::__Node_Base::__Node_Base(std::shared_ptr<scls::__Image_Base> new_image, scls::Fraction x, scls::Fraction y, int id):a_id(id),a_image(new_image),a_x(x),a_y(y){};
    __Graph_Base::__Node_Base::__Node_Base(){};

    // Links this node with another node
    __Graph_Base::__Node_Base::Link* __Graph_Base::__Node_Base::link(std::shared_ptr<__Node_Base> node){Link* temp = link_by_object(node);if(link_by_object(node) == temp){a_links.push_back(Link());a_links[a_links.size()-1].target=node;return &a_links[a_links.size()-1];}return temp;};
    // Returns a link by the linked object
    __Graph_Base::__Node_Base::Link* __Graph_Base::__Node_Base::link_by_object(__Node_Base* node){for(int i = 0;i<static_cast<int>(a_links.size());i++){if(a_links.at(i).target.lock().get() == node){return &a_links[i];}}return 0;};
    __Graph_Base::__Node_Base::Link* __Graph_Base::__Node_Base::link_by_object(std::shared_ptr<__Node_Base> node){return link_by_object(node.get());};

    // Returns a node to an std::string
    std::string __Graph_Base::__Node_Base::to_xml_text(std::string node_object_name, std::string node_content){
        std::string links = std::string();
        for(int i = 0;i<static_cast<int>(a_links.size());i++){links += std::string("<link id=") + std::to_string(a_links.at(i).target.lock().get()->id()) + std::string(">");}
        node_content = std::string("<content>") + node_content + std::string("</content>");
        return std::string("<") + node_object_name + to_xml_text_x() + to_xml_text_y() + std::string(">") + node_content + links + std::string("</node>");
    }
    std::string __Graph_Base::__Node_Base::to_xml_text_x(){if(x() == 0){return std::string();}return std::string(" x=") + x().to_std_string(0);}
    std::string __Graph_Base::__Node_Base::to_xml_text_y(){if(y() == 0){return std::string();}return std::string(" y=") + y().to_std_string(0);}

    //******************
    // Graph
    //******************

    // Adds a node in the graph
    int __Graph_Base::add_node(std::shared_ptr<scls::__Image_Base> image, scls::Fraction x, scls::Fraction y){std::shared_ptr<__Node_Base>new_node=__create_node(image,x,y);a_nodes.push_back(new_node);return new_node.get()->id();};
    int __Graph_Base::add_node(std::shared_ptr<scls::__Image_Base> image){return add_node(image, 0, 0);};
    int __Graph_Base::add_node(){return add_node(std::shared_ptr<scls::__Image_Base>(), 0, 0);};
    // Links two nodes in the graph
    bool __Graph_Base::link_nodes(int id_1, int id_2){if(id_1!=id_2&&id_1<static_cast<int>(a_nodes.size())&&id_2<static_cast<int>(a_nodes.size())){a_nodes[id_1].get()->link(a_nodes[id_2]);return true;}return false;};
    // Sets the ponderation of a link node
    void __Graph_Base::set_link_ponderation(int id_1, int id_2, std::shared_ptr<scls::__Image_Base> needed_ponderation){if(id_1!=id_2&&id_1<static_cast<int>(a_nodes.size())&&id_2<static_cast<int>(a_nodes.size())){Link* needed_link = a_nodes[id_1].get()->link(a_nodes[id_2]);needed_link->ponderation = needed_ponderation;}};
    void __Graph_Base::set_link_ponderation(int id_1, int id_2, std::string needed_ponderation, scls::Text_Style style, scls::Text_Image_Generator* tig){set_link_ponderation(id_1, id_2, tig->image_shared_ptr(needed_ponderation, style));};
    void __Graph_Base::set_link_ponderation(int id_1, int id_2, std::string needed_ponderation, scls::Text_Style style){set_link_ponderation(id_1, id_2, scls::to_image(&needed_ponderation, style));};

    // Returns the tree in an image
    std::shared_ptr<scls::__Image_Base> __Graph_Base::to_image(){return to_image(scls::Text_Style());};
    std::shared_ptr<scls::__Image_Base> __Graph_Base::to_image(scls::Text_Style needed_style) {
        std::shared_ptr<scls::__Image_Base> to_return;
        if(static_cast<int>(a_nodes.size()) > 0) {
            // Get the needed children images
            scls::Fraction max_x = 0;  scls::Fraction min_x = 0; bool min_x_used = false; bool max_x_used = false;
            scls::Fraction max_y = 0;  scls::Fraction min_y = 0; bool min_y_used = false; bool max_y_used = false;
            int pixel_in_value = 100;
            std::vector<std::shared_ptr<scls::__Image_Base>> images;
            for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                std::shared_ptr<scls::__Image_Base> current_image = a_nodes[i].get()->image_shared_ptr();
                images.push_back(current_image);int image_height = 0;int image_width = 0;
                if(current_image.get() != 0){image_height = current_image.get()->height();image_width = current_image.get()->width();}

                // Get the datas about the image
                // Datas for width
                scls::Fraction current_min_x = a_nodes[i].get()->x() * pixel_in_value - scls::Fraction(image_width, 2);
                scls::Fraction current_max_x = a_nodes[i].get()->x() * pixel_in_value + scls::Fraction(image_width, 2);
                if(current_min_x < min_x || !min_x_used){min_x = current_min_x;min_x_used=true;}
                if(current_max_x > max_x || !max_x_used){max_x = current_max_x;max_x_used=true;}
                // Datas for height
                scls::Fraction current_min_y = a_nodes[i].get()->y() * pixel_in_value - scls::Fraction(image_height, 2);
                scls::Fraction current_max_y = a_nodes[i].get()->y() * pixel_in_value + scls::Fraction(image_height, 2);
                if(current_min_y < min_y || !min_y_used){min_y = current_min_y;min_y_used=true;}
                if(current_max_y > max_y || !max_y_used){max_y = current_max_y;max_y_used=true;}
            }

            // Get all the positions
            std::vector<scls::Fraction> needed_x;
            std::vector<scls::Fraction> needed_y;
            scls::Fraction total_height = max_y - min_y;
            scls::Fraction total_width = max_x - min_x;
            for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                // Get the needed value
                std::shared_ptr<scls::__Image_Base> current_image = images[i];
                int image_height = 0;int image_width = 0;
                if(current_image.get() != 0){image_height = current_image.get()->height();image_width = current_image.get()->width();}

                // Datas for width
                scls::Fraction current_x = (a_nodes[i].get()->x() * pixel_in_value - min_x);
                current_x -= scls::Fraction(image_width, 2);
                needed_x.push_back(current_x);
                // Datas for height
                scls::Fraction current_y = total_height - (a_nodes[i].get()->y() * pixel_in_value - min_y);
                current_y -= scls::Fraction(image_height, 2);
                needed_y.push_back(current_y);
            }

            // Prepare the creation of the image
            int x_offset = 0;int y_offset = 0;int height_to_add = 0;int width_to_add = 0;
            x_offset = needed_style.border_left_width() + needed_style.padding_left();y_offset = needed_style.border_top_width() + needed_style.padding_top();
            height_to_add = y_offset + needed_style.border_right_width() + needed_style.padding_right();width_to_add = x_offset + needed_style.border_bottom_width() + needed_style.padding_bottom();

            // Draw the final image
            to_return = std::make_shared<scls::__Image_Base>(total_width.to_double() + width_to_add, total_height.to_double() + height_to_add, scls::Color(255, 255, 255));
            // Draw the border
            to_return.get()->draw_border(needed_style.border_left_width(), needed_style.border_top_width(), needed_style.border_bottom_width(), needed_style.border_right_width(), needed_style.border_color());

            // Draw the links
            for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                std::vector<Link>& links = a_nodes[i].get()->links();
                for(int j = 0;j<static_cast<int>(links.size());j++) {
                    std::shared_ptr<__Node_Base> current_node = links[j].target.lock();
                    int current_id = current_node.get()->id();
                    if(current_id > a_nodes[i].get()->id() || true) {
                        // Get the good link type
                        Link_Type link_type = links[j].type;

                        // Calculate with the angle
                        if(link_type == Link_Type::LT_Angle) {
                            // Get the good angle
                            scls::Fraction x_diff = needed_x[current_id] - needed_x[i];
                            scls::Fraction y_diff = needed_y[current_id] - needed_y[i];
                            double angle = scls::vector_2d_angle(x_diff.to_double(), y_diff.to_double()) * (180.0 / SCLS_PI);
                            while(angle < 0){angle += 360;}
                            if(angle > 225 && angle <= 315){link_type = Link_Type::LT_Bottom;}
                            else if(angle > 135 && angle <= 225){link_type = Link_Type::LT_Left;}
                            else if(angle <= 45 || angle >= 315){link_type = Link_Type::LT_Right;}
                            else {link_type = Link_Type::LT_Top;}
                        }

                        // Get the good values
                        scls::Fraction current_x_end;scls::Fraction current_x_start;
                        scls::Fraction current_y_end;scls::Fraction current_y_start;
                        int image_height_current = 0;int image_width_current = 0;
                        if(images.at(current_id).get() != 0){image_height_current = images.at(current_id).get()->height();image_width_current = images.at(current_id).get()->width();}
                        int image_height_i = 0;int image_width_i = 0;
                        if(images.at(i).get() != 0){image_height_i = images.at(i).get()->height();image_width_i = images.at(i).get()->width();}
                        if(link_type == Link_Type::LT_Middle) {
                            // Start at the middle
                            current_x_end = needed_x[current_id] + scls::Fraction(image_width_current, 2);
                            current_x_start = needed_x[i] + scls::Fraction(image_width_i, 2);
                            current_y_end = needed_y[current_id] + scls::Fraction(image_height_current, 2);
                            current_y_start = needed_y[i] + scls::Fraction(image_height_i, 2);
                        }
                        else if(link_type == Link_Type::LT_Bottom) {
                            // Bottom link
                            current_x_end = needed_x[current_id] + scls::Fraction(image_width_current, 2);
                            current_y_end = needed_y[current_id] + scls::Fraction(image_height_current);
                            current_x_start = needed_x[i] + scls::Fraction(image_width_i, 2);
                            current_y_start = needed_y[i];
                        }
                        else if(link_type == Link_Type::LT_Left) {
                            // Left link
                            current_x_end = needed_x[current_id] + scls::Fraction(images[i].get()->width());
                            current_y_end = needed_y[current_id] + scls::Fraction(images[i].get()->height(), 2);
                            current_x_start = needed_x[i];
                            current_y_start = needed_y[i] + scls::Fraction(image_height_i, 2);
                        }
                        else if(link_type == Link_Type::LT_Right) {
                            // Right link
                            current_x_end = needed_x[current_id] - scls::Fraction(image_width_current);
                            current_y_end = needed_y[current_id] + scls::Fraction(image_height_i, 2);
                            current_x_start = needed_x[i] + scls::Fraction(image_width_i);
                            current_y_start = needed_y[i] + scls::Fraction(image_height_i, 2);
                        }
                        else {
                            // Top link
                            current_x_end = needed_x[current_id] + scls::Fraction(image_width_current, 2);
                            current_y_end = needed_y[current_id];
                            current_x_start = needed_x[i] + scls::Fraction(image_width_i, 2);
                            current_y_start = needed_y[i] + scls::Fraction(image_height_current);
                        }//*/

                        // Paste the ponderation
                        if(links[j].ponderation.get() != 0) {
                            scls::Fraction x_middle = (current_x_start + current_x_end) / 2;
                            scls::Fraction y_middle = (current_y_start + current_y_end) / 2;

                            // Put the image at the center
                            x_middle -= links[j].ponderation.get()->width() / 2;
                            y_middle -= links[j].ponderation.get()->height() / 2;

                            to_return.get()->paste(links[j].ponderation.get(), x_middle.to_double() + x_offset, y_middle.to_double() + y_offset);
                        }

                        // Draw the link
                        to_return.get()->draw_line(current_x_start.to_double() + x_offset, current_y_start.to_double() + y_offset, current_x_end.to_double() + x_offset, current_y_end.to_double() + y_offset, scls::Color(0, 0, 0), 2);
                    }
                }
            } //*/

            // Paste the children
            for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                std::shared_ptr<scls::__Image_Base> current_image = images[i];
                if(current_image.get() == 0){continue;}
                to_return.get()->paste(current_image.get(), needed_x[i].to_double() + x_offset, needed_y[i].to_double() + y_offset);
            }
        }
        return to_return;
    }

    // Getters and setters
    __Graph_Base::__Node_Base* __Graph_Base::node(int id)const{if(id > static_cast<int>(a_nodes.size())){return 0;}return a_nodes.at(id).get();};
    std::vector<std::shared_ptr<__Graph_Base::__Node_Base>>& __Graph_Base::nodes() {return a_nodes;};

    //******************
    // Linked list
    //******************

    // Linked_List constructor
    Linked_List::Linked_List(std::shared_ptr<__Graph_Base> graph, int root_id):a_graph(graph),a_root_id(root_id){};
    Linked_List::Linked_List():a_root_id(a_graph.get()->add_node()){};

    // Add a child in the graph
    std::shared_ptr<Linked_List> Linked_List::add_child(std::shared_ptr<scls::__Image_Base> image){int current_id = a_graph.get()->add_node(image);a_child = std::make_shared<Linked_List>(a_graph, current_id);a_child.get()->a_parent=a_this_object;a_child.get()->a_this_object=a_child;a_graph.get()->link_nodes(a_root_id,current_id);place_child_directly();return a_child;}
    std::shared_ptr<Linked_List> Linked_List::add_child(std::string value){return add_child(scls::to_image(&value));}
    // Place the child in the graph
    void Linked_List::place_child() {root()->set_x(a_root_x);root()->set_y(a_root_y);if(a_child.get() != 0){a_child.get()->place_child();}};
    void Linked_List::__place_child_directly(Linked_List* needed_child) {scls::Fraction x = 1;scls::Fraction y = 0;needed_child->a_root_x=a_root_x + x;needed_child->a_root_y=a_root_y + y;a_child.get()->place_child();};
    void Linked_List::place_child_directly() {if(a_child.get() == 0){return;}else if(ignore_for_placement()){if(parent()!=0){parent()->__place_child_directly(a_child.get());}return;}__place_child_directly(a_child.get());};

    // Return the image of the graph attached to the linked-list
    std::shared_ptr<scls::__Image_Base> Linked_List::to_image(scls::Text_Style needed_style){place_child();return a_graph.get()->to_image(needed_style);};
    std::shared_ptr<scls::__Image_Base> Linked_List::to_image(){return to_image(scls::Text_Style());};

    //******************
    // Tree
    //******************
}
