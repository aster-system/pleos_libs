//******************
//
// pleos_it_data_structure.h
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
// This file contains the handling of data structures in the PLEOS informatics and technology IT file.
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

#ifndef PLEOS_IT_DATA_STRUCTURE
#define PLEOS_IT_DATA_STRUCTURE

// Include SCLS Graphic Benoit
#include "../../../scls-graphic-benoit/scls_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    //******************
    //
    // Stack structures
    //
    //******************

    template <typename E>
    class List {
        // Class representating a list
    public:

        // List constructor
        List(){};

        // Returns a case of the list
        std::shared_ptr<scls::Image> case_image(int element, scls::Text_Image_Generator& generator, scls::Text_Style& current_style) {
            // Create the text
            std::shared_ptr<scls::Image> to_add = generator.image_shared_ptr(a_elements[element].to_std_string(), current_style);

            // Create the final image
            return to_add;
        };
        // Returns the list in an image
        std::shared_ptr<scls::Image> image() {
            // Get each needed images for elements
            scls::Text_Style current_style; current_style.font_size = 70;
            std::vector<std::shared_ptr<scls::Image>> elements_image;
            scls::Text_Image_Generator generator;
            int max_height = 0;
            int total_width = a_border_width;
            for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                std::shared_ptr<scls::Image> to_add = case_image(i, generator, current_style);
                total_width += to_add.get()->width(); elements_image.push_back(to_add);
                if(max_height < to_add.get()->height()){max_height = to_add.get()->height() + a_inline_border_width;}
            } max_height += a_border_width * 2;
            max_height = 72; total_width = 300;

            // Create the final image
            int current_x = a_border_width;
            std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(total_width, max_height, scls::Color(255, 255, 255, 255));
            // Draw the border
            to_return.get()->fill_rect(0, 0, to_return.get()->width(), a_border_width, scls::Color(0, 0, 0));
            //to_return.get()->fill_rect(0, 0, a_border_width, to_return.get()->height(), scls::Color(0, 0, 0));
            to_return.get()->fill_rect(0, to_return.get()->height() - a_border_width, to_return.get()->width(), a_border_width, scls::Color(0, 0, 0));
            // Draw the elements
            for(int i = 0;i<static_cast<int>(elements_image.size());i++) {
                std::shared_ptr<scls::Image> current_image = elements_image[i];
                // Paste the image
                int current_y = to_return.get()->height() / 2.0 - current_image.get()->height() / 2.0;
                to_return.get()->paste(current_image.get(), current_x, current_y);
                current_x += current_image.get()->width();
                to_return.get()->fill_rect(current_x, 0, a_inline_border_width, to_return.get()->height(), scls::Color(0, 0, 0));
                current_x += a_inline_border_width;
            } return to_return;
        };

        // Getters and setters
        inline std::vector<E>& elements() {return a_elements;};

    protected:
        // Apparence elements
        // Width of the border
        int a_border_width = 4;
        // Width of the innder border
        int a_inline_border_width = 2;

    private:
        // Elements in the list
        std::vector<E> a_elements;
    };

    template <typename E>
    class Queue : public List<E> {
        // Class representating a queue
    public:

        // Queue constructor
        Queue():List<E>(){};

        // Adds an element
        inline void add(E element){List<E>::elements().insert(List<E>::elements().begin(), element);};
        // Removes an element
        inline E pop() {E to_return = List<E>::elements()[List<E>::elements().size() - 1];List<E>::elements().erase(List<E>::elements().end() - 1);return to_return;}
    };

    template <typename E>
    class Stack : public List<E> {
        // Class representating a stack
    public:

        // Stack constructor
        Stack():List<E>(){};

        // Returns the stack in an image
        std::shared_ptr<scls::Image> image() {
            // Get each needed images for elements
            int border_width = 4; int inline_border_width = 2;
            scls::Text_Style current_style; current_style.font_size = 70;
            std::vector<std::shared_ptr<scls::Image>> elements_image;
            scls::Text_Image_Generator generator;
            int max_width = 0;
            int total_height = border_width;
            for(int i = 0;i<static_cast<int>(List<E>::elements().size());i++) {
                std::shared_ptr<scls::Image> to_add = List<E>::case_image(i, generator, current_style);
                total_height += to_add.get()->height(); elements_image.push_back(to_add);
                if(max_width < to_add.get()->width()){max_width = to_add.get()->width() + inline_border_width;}
            } max_width += border_width * 2;
            max_width = 72; total_height = 300;

            // Create the final image
            int current_y = total_height - border_width;
            std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(max_width, total_height, scls::Color(255, 255, 255, 255));
            // Draw the border
            to_return.get()->fill_rect(0, 0, border_width, to_return.get()->height(), scls::Color(0, 0, 0));
            to_return.get()->fill_rect(to_return.get()->width() - border_width, 0, border_width, to_return.get()->height(), scls::Color(0, 0, 0));
            to_return.get()->fill_rect(0, to_return.get()->height() - border_width, to_return.get()->width(), border_width, scls::Color(0, 0, 0));
            // Draw the elements
            for(int i = 0;i<static_cast<int>(elements_image.size());i++) {
                std::shared_ptr<scls::Image> current_image = elements_image[i];
                // Paste the image
                int current_x = to_return.get()->width() / 2.0 - current_image.get()->width() / 2.0;
                current_y -= current_image.get()->height();
                to_return.get()->paste(current_image.get(), current_x, current_y);
                current_y -= inline_border_width;
                to_return.get()->fill_rect(0, current_y, to_return.get()->width(), inline_border_width, scls::Color(0, 0, 0));
            } return to_return;
        };
        // Adds an element
        inline void pile_up(E element){List<E>::elements().push_back(element);};
        // Removes an element
        inline E unstack() {E to_return = List<E>::elements()[List<E>::elements().size() - 1];List<E>::elements().erase(List<E>::elements().end() - 1);return to_return;}
    };

    //******************
    //
    // Graph structures
    //
    //******************

    // Current node style
    extern scls::Text_Style __current_node_style;

    template <typename E> class Graph {
        // Class representating a graph
    public:
        // Node in a graph
        class Node {
        public:
            // Links for a node in a graph
            enum Link_Type{LT_Middle, LT_Angle, LT_Bottom, LT_Left, LT_Right, LT_Top, LT_X, LT_Y};
            struct Link{Link_Type type = Link_Type::LT_Middle;std::weak_ptr<Node> target;};

            // Node constructor
            Node(E new_value, scls::Fraction x, scls::Fraction y, int id):a_id(id),a_x(x),a_y(y){set_value(new_value);};
            Node(){};

            // Links this node with another node
            inline void link(std::shared_ptr<Node> node){a_links.push_back(Link());a_links[a_links.size()-1].target=node;};

            // Returns the value into an image
            template <typename X = E> std::enable_if<!std::is_base_of<X,std::string>::value,std::shared_ptr<scls::Image>>::type image_value() {std::shared_ptr<scls::Image> to_return = scls::to_image(a_value.get());return to_return;};
            template <typename X = E> std::enable_if<std::is_base_of<X,std::string>::value,std::shared_ptr<scls::Image>>::type image_value() {style().set_alignment_horizontal(scls::H_Center);std::shared_ptr<scls::Image> to_return = scls::to_image(a_value.get(), style());return to_return;};

            // Getters and setters
            inline int id() const {return a_id;};
            inline std::vector<Link>& links() {return a_links;};
            inline scls::Text_Style& style() {return a_style;};
            inline void set_style(scls::Text_Style new_style){a_style = new_style;};
            inline void set_value(E new_value){a_value = std::make_shared<E>(new_value);};
            inline void set_x(scls::Fraction new_x){a_x = new_x;};
            inline void set_y(scls::Fraction new_y){a_y = new_y;};
            inline E* value() const {return a_value.get();};
            inline scls::Fraction x() const {return a_x;};
            inline scls::Fraction y() const {return a_y;};
        private:
            // ID of the node
            int a_id = 0;
            // Links to others nodes
            std::vector<Link> a_links;
            // Style of the node
            scls::Text_Style a_style;
            // Value in the node
            std::shared_ptr<E> a_value;
            // X value of the node
            scls::Fraction a_x;
            // Y value of the node
            scls::Fraction a_y;
        }; typedef Node::Link Link;typedef Node::Link_Type Link_Type;

        // Graph constructor
        Graph(){};

        // Adds a node in the graph
        inline int add_node(E value, scls::Fraction x, scls::Fraction y){a_nodes.push_back(std::make_shared<Node>(value, x, y, a_nodes.size()));return a_nodes[a_nodes.size()-1].get()->id();};
        inline int add_node(E value){return add_node(value, 0, 0);};
        // Links two nodes in the graph
        inline bool link_nodes(int id_1, int id_2){if(id_1!=id_2&&id_1<a_nodes.size()&&id_2<a_nodes.size()){a_nodes[id_1].get()->link(a_nodes[id_2]);return true;}return false;};

        // Getters and setters
        inline Node* node(int id)const{if(id > a_nodes.size()){return 0;}return a_nodes.at(id).get();};
        inline std::vector<std::shared_ptr<Node>>& nodes() {return a_nodes;};

        // Returns the tree in an image
        std::shared_ptr<scls::Image> image(std::shared_ptr<scls::Text_Style> needed_style) {
            std::shared_ptr<scls::Image> to_return;
            if(a_nodes.size() > 0) {
                // Get the needed children images
                int max_height = 0;
                scls::Fraction max_x = 0;  scls::Fraction min_x = 0; bool min_x_used = false; bool max_x_used = false;
                scls::Fraction max_y = 0;  scls::Fraction min_y = 0; bool min_y_used = false; bool max_y_used = false;
                int pixel_in_value = 100;
                std::vector<std::shared_ptr<scls::Image>> images;
                for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                    std::shared_ptr<scls::Image> current_image = a_nodes[i].get()->image_value();
                    images.push_back(current_image);
                    // Get the datas about the image
                    // Datas for width
                    scls::Fraction current_min_x = a_nodes[i].get()->x() * pixel_in_value - scls::Fraction(current_image.get()->width(), 2);
                    scls::Fraction current_max_x = a_nodes[i].get()->x() * pixel_in_value + scls::Fraction(current_image.get()->width(), 2);
                    if(current_min_x < min_x || !min_x_used){min_x = current_min_x;min_x_used=true;}
                    if(current_max_x > max_x || !max_x_used){max_x = current_max_x;max_x_used=true;}
                    // Datas for height
                    scls::Fraction current_min_y = a_nodes[i].get()->y() * pixel_in_value - scls::Fraction(current_image.get()->height(), 2);
                    scls::Fraction current_max_y = a_nodes[i].get()->y() * pixel_in_value + scls::Fraction(current_image.get()->height(), 2);
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
                    std::shared_ptr<scls::Image> current_image = images[i];
                    // Datas for width
                    scls::Fraction current_x = (a_nodes[i].get()->x() * pixel_in_value - min_x);
                    current_x -= scls::Fraction(current_image.get()->width(), 2);
                    needed_x.push_back(current_x);
                    // Datas for height
                    scls::Fraction current_y = total_height - (a_nodes[i].get()->y() * pixel_in_value - min_y);
                    current_y -= scls::Fraction(current_image.get()->height(), 2);
                    needed_y.push_back(current_y);
                }

                // Prepare the creation of the image
                int node_to_node_separation = 20;
                int root_to_node_separation = 30;
                int x_offset = 0;int y_offset = 0;int height_to_add = 0;int width_to_add = 0;
                if(needed_style.get() != 0){
                    x_offset = needed_style.get()->border_left_width + needed_style.get()->padding_left();y_offset = needed_style.get()->border_top_width + needed_style.get()->padding_top();
                    height_to_add = y_offset + needed_style.get()->border_right_width + needed_style.get()->padding_right();width_to_add = x_offset + needed_style.get()->border_bottom_width + needed_style.get()->padding_bottom();
                }
                // Draw the final image
                to_return = std::make_shared<scls::Image>(total_width.to_double() + width_to_add, total_height.to_double() + height_to_add, scls::Color(255, 255, 255));
                if(needed_style.get() != 0) {
                    // Draw the border
                    to_return.get()->draw_border(needed_style.get()->border_left_width, needed_style.get()->border_top_width, needed_style.get()->border_bottom_width, needed_style.get()->border_right_width, needed_style.get()->border_color);
                }

                // Draw the links
                for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                    std::vector<Link>& links = a_nodes[i].get()->links();
                    for(int j = 0;j<static_cast<int>(links.size());j++) {
                        std::shared_ptr<Node> current_node = links[j].target.lock();
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
                            if(link_type == Link_Type::LT_Middle) {
                                // Start at the middle
                                current_x_end = needed_x[current_id] + scls::Fraction(images[current_id].get()->width(), 2);
                                current_x_start = needed_x[i] + scls::Fraction(images[i].get()->width(), 2);
                                current_y_end = needed_y[current_id] + scls::Fraction(images[current_id].get()->height(), 2);
                                current_y_start = needed_y[i] + scls::Fraction(images[i].get()->height(), 2);
                            }
                            else if(link_type == Link_Type::LT_Bottom) {
                                // Bottom link
                                current_x_end = needed_x[current_id] + scls::Fraction(images[current_id].get()->width(), 2);
                                current_y_end = needed_y[current_id] + scls::Fraction(images[current_id].get()->height());
                                current_x_start = needed_x[i] + scls::Fraction(images[i].get()->width(), 2);
                                current_y_start = needed_y[i];
                            }
                            else if(link_type == Link_Type::LT_Left) {
                                // Left link
                                current_x_end = needed_x[current_id] + scls::Fraction(images[i].get()->width());
                                current_y_end = needed_y[current_id] + scls::Fraction(images[i].get()->height(), 2);
                                current_x_start = needed_x[i];
                                current_y_start = needed_y[i] + scls::Fraction(images[i].get()->height(), 2);
                            }
                            else if(link_type == Link_Type::LT_Right) {
                                // Right link
                                current_x_end = needed_x[current_id] - scls::Fraction(images[current_id].get()->width());
                                current_y_end = needed_y[current_id] + scls::Fraction(images[i].get()->height(), 2);
                                current_x_start = needed_x[i] + scls::Fraction(images[i].get()->width());
                                current_y_start = needed_y[i] + scls::Fraction(images[i].get()->height(), 2);
                            }
                            else {
                                // Top link
                                current_x_end = needed_x[current_id] + scls::Fraction(images[current_id].get()->width(), 2);
                                current_y_end = needed_y[current_id];
                                current_x_start = needed_x[i] + scls::Fraction(images[i].get()->width(), 2);
                                current_y_start = needed_y[i] + scls::Fraction(images[current_id].get()->height());
                            }//*/

                            // Draw the link
                            to_return.get()->draw_line(current_x_start.to_double() + x_offset, current_y_start.to_double() + y_offset, current_x_end.to_double() + x_offset, current_y_end.to_double() + y_offset, scls::Color(0, 0, 0), 2);
                        }
                    }
                } //*/

                // Paste the children
                for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                    std::shared_ptr<scls::Image> current_image = images[i];
                    to_return.get()->paste(current_image.get(), needed_x[i].to_double() + x_offset, needed_y[i].to_double() + y_offset);
                }
            }
            return to_return;
        };
        inline std::shared_ptr<scls::Image> image(){return image(std::shared_ptr<scls::Text_Style>());};

    private:
        // Nodes in the tree
        std::vector<std::shared_ptr<Node>> a_nodes;
    };

    //******************
    // Linked list
    //******************

    template <typename E> class Linked_List {
        // Class representating a linked-list (in a graph environment)
    public:
        // Linked_List constructor
        Linked_List(std::shared_ptr<Graph<E>> graph, int root_id):a_graph(graph),a_root_id(root_id){};
        Linked_List():a_root_id(a_graph.get()->add_node(E())){};

        // Add a child in the graph
        std::shared_ptr<Linked_List> add_child(E value){int current_id = a_graph.get()->add_node(value);a_child = std::make_shared<Linked_List>(a_graph, current_id);a_child.get()->a_parent=a_this_object;a_child.get()->a_this_object=a_child;a_graph.get()->link_nodes(a_root_id,current_id);place_child_directly();return a_child;}
        // Place the child in the graph
        void place_child() {root()->set_x(a_root_x);root()->set_y(a_root_y);if(a_child.get() != 0){a_child.get()->place_child();}};
        void __place_child_directly(Linked_List* needed_child) {scls::Fraction x = 1;scls::Fraction y = 0;needed_child->a_root_x=a_root_x + x;needed_child->a_root_y=a_root_y + y;a_child.get()->place_child();};
        void place_child_directly() {if(a_child.get() == 0){return;}else if(ignore_for_placement()){if(parent()!=0){parent()->__place_child_directly(a_child.get());}return;}__place_child_directly(a_child.get());};

        // Return the image of the graph attached to the linked-list
        inline std::shared_ptr<scls::Image> to_image(std::shared_ptr<scls::Text_Style> needed_style){place_child();return a_graph.get()->image(needed_style);};
        inline std::shared_ptr<scls::Image> to_image(){return to_image(std::shared_ptr<scls::Text_Style>());};

        // Getters and setters
        inline std::shared_ptr<Linked_List> child() const {return a_child;};
        inline bool ignore_for_placement() const{return a_ignore_for_placement;};
        inline auto& nodes() {return a_graph.get()->nodes();};
        inline Linked_List<E>* parent() const{return a_parent.lock().get();};
        inline auto* root() {return nodes()[a_root_id].get();};
        inline int root_id() const {return a_root_id;};
        inline void set_ignore_for_placement(bool new_ignore_for_placement){a_ignore_for_placement = new_ignore_for_placement;};
        inline void set_value(E new_value){root()->set_value(new_value);};
        inline void set_y(scls::Fraction new_y){a_root_y = new_y;};

    private:

        // Children of this list
        std::shared_ptr<Linked_List> a_child;
        // Attached graph for this tree
        std::shared_ptr<Graph<E>> a_graph = std::make_shared<Graph<E>>();
        // Ignore the element for the place
        bool a_ignore_for_placement = false;
        // Parent of this list
        std::weak_ptr<Linked_List> a_parent;
        // Shared ptr to this object
        std::weak_ptr<Linked_List> a_this_object;

        // Datas for the root
        // ID of the roots for this tree
        int a_root_id = 0;
        // X value of the root
        scls::Fraction a_root_x = 0;
        // Y value of the root
        scls::Fraction a_root_y = 0;
    };

    //******************
    // Tree
    //******************

    template <typename E> class Tree {
        // Class representating a tree
    public:
        // Tree constructor
        Tree(std::shared_ptr<Graph<E>> graph, int root_id):a_graph(graph),a_root_id(root_id){};
        Tree():a_root_id(a_graph.get()->add_node(E())){};

        // Return the image of the graph attached to the tree
        inline std::shared_ptr<scls::Image> to_image(){place_nodes();return a_graph.get()->image();};

        // Adds a node in the tree
        inline Tree* add_node(E value){int current_id = a_graph.get()->add_node(value);a_children.push_back(Tree(a_graph, current_id));Tree& current = a_children[a_children.size() - 1];a_graph.get()->link_nodes(a_root_id,current_id);return &current;}
        inline Tree* add_node(int sub_id, E value){Tree* needed_child = child(sub_id);if(needed_child==0){return 0;}return needed_child->add_node(value);};
        inline Tree* add_node(int sub_id_1, int sub_id_2, E value){Tree* needed_child = child(sub_id_1);if(needed_child==0){return 0;}return needed_child->add_node(sub_id_2, value);};
        // Place the nodes in the tree
        void place_nodes() {
            // Place the root
            a_graph.get()->node(a_root_id)->set_x(a_root_x);
            a_graph.get()->node(a_root_id)->set_y(a_root_y);

            // Place the children
            scls::Fraction needed_width = width();
            scls::Fraction start_x = a_root_x - needed_width / 2;
            scls::Fraction current_x = start_x;
            for(int i = 0;i<static_cast<int>(a_children.size());i++) {
                current_x += a_children[i].width() / 2;
                a_children[i].a_root_x = current_x;
                a_children[i].a_root_y = a_root_y - 1;
                a_children[i].place_nodes();
                current_x += a_children[i].width() / 2;
            }
        };
        // Returns a sub-tree in the tree
        //inline Tree<E>* sub_tree(int n){while(static_cast<int>(a_nodes.size()) <= n){a_nodes.push_back(std::make_shared<Tree>());}return a_nodes[n].get();};
        // Returns the total width of the object
        inline scls::Fraction width(){scls::Fraction to_return = 0;for(int i = 0;i<static_cast<int>(a_children.size());i++){to_return+=a_children[i].width();}if(to_return<=1){return 1;}return to_return;};

        // Getters and setters
        inline Tree* child(int id){for(int i = 0;i<static_cast<int>(a_children.size());i++){if(a_children[i].root_id()==id){return &a_children[i];}}return 0;};
        inline Graph<E>* graph() const {return a_graph.get();};
        inline auto& nodes() {return a_graph.get()->nodes();};
        inline auto* root() {return nodes()[a_root_id].get();};
        inline int root_id() const {return a_root_id;};
        inline void set_value(E new_value){nodes()[a_root_id].get()->set_value(new_value);};
        inline E* value() const {return a_graph.get()->node(a_root_id)->value();};

    private:
        // Children of this tree
        std::vector<Tree> a_children = std::vector<Tree>();
        // Attached graph for this tree
        std::shared_ptr<Graph<E>> a_graph = std::make_shared<Graph<E>>();

        // Datas for the root
        // ID of the roots for this tree
        int a_root_id = 0;
        // X value of the root
        scls::Fraction a_root_x = 0;
        // Y value of the root
        scls::Fraction a_root_y = 0;
    };
}

#endif // PLEOS_IT_DATA_STRUCTURE
