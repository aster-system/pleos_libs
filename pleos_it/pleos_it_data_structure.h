//******************
//
// pleos_mathematics_functions.h
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
    // Tree structures
    //
    //******************

    template <typename E>
    class Tree {
        // Class representating a tree
    public:
        // Tree constructor
        Tree(){};

        // Returns a sub-tree in the tree
        inline Tree<E>* sub_tree(int n){while(static_cast<int>(a_nodes.size()) <= n){a_nodes.push_back(std::make_shared<Tree>());}return a_nodes[n].get();};

        // Getters and setters
        inline std::vector<std::shared_ptr<Tree>>& nodes() {return a_nodes;};
        inline void set_value(E new_value){a_value = std::make_shared<E>(new_value);};
        inline E* value() const {return a_value.get();};

        // Returns the tree in an image
        std::shared_ptr<scls::Image> image() {
            std::shared_ptr<scls::Image> to_return;
            if(a_nodes.size() <= 0) {
                // The tree has no children
                to_return = image_value();
            }
            else {
                // Get the needed children images
                int max_height = 0;
                int total_width = 0;
                std::vector<std::shared_ptr<scls::Image>> children_images;
                for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                    std::shared_ptr<scls::Image> current_image = a_nodes[i].get()->image();
                    children_images.push_back(current_image);
                    // Get the datas about the image
                    if(current_image.get()->height() > max_height){max_height = current_image.get()->height();}
                    total_width += current_image.get()->width();
                }
                // Get the value image
                std::shared_ptr<scls::Image> value_image = image_value();
                if(value_image.get()->width() > total_width){total_width = value_image.get()->width();}

                // Draw the final image
                int node_to__node_separation = 20;
                int root_to_node_separation = 30;
                int total_height = max_height + value_image.get()->height() + root_to_node_separation;
                total_width = total_width + (children_images.size() - 1) * node_to__node_separation;
                to_return = std::make_shared<scls::Image>(total_width, total_height, scls::Color(255, 255, 255));

                // Draw the links
                int base_x = to_return.get()->width() / 2;
                int base_y = value_image.get()->height();
                int current_x = 0;
                int current_y = base_y + root_to_node_separation;
                scls::Color link_color = scls::Color(0, 0, 0); int link_width = 2;
                for(int i = 0;i<static_cast<int>(children_images.size());i++) {
                    std::shared_ptr<scls::Image> current_image = children_images[i];
                    to_return.get()->draw_line(base_x - link_width / 2, base_y, current_x + current_image.get()->width() / 2, current_y, link_color, link_width);
                    current_x += current_image.get()->width() + node_to__node_separation;
                }

                // Paste the value
                to_return.get()->paste(value_image.get(), to_return.get()->width() / 2 - value_image.get()->width() / 2, 0);
                // Paste the children
                current_x = 0;
                current_y = value_image.get()->height() + root_to_node_separation;
                for(int i = 0;i<static_cast<int>(children_images.size());i++) {
                    std::shared_ptr<scls::Image> current_image = children_images[i];
                    to_return.get()->paste(current_image.get(), current_x, current_y);
                    current_x += current_image.get()->width() + node_to__node_separation;
                }
            } return to_return;
        };
        // Returns the value into an image
        template <typename X = E> std::enable_if<!std::is_base_of<X,std::string>::value,std::shared_ptr<scls::Image>>::type image_value() {std::shared_ptr<scls::Image> to_return = scls::to_image(a_value.get());return to_return;};
        template <typename X = E> std::enable_if<std::is_base_of<X,std::string>::value,std::shared_ptr<scls::Image>>::type image_value() {scls::Text_Style style;style.alignment_horizontal=scls::H_Center;std::shared_ptr<scls::Image> to_return = scls::to_image(a_value.get(), style);return to_return;};

    private:
        // Nodes in the tree
        std::vector<std::shared_ptr<Tree>> a_nodes;
        // Value of the root of the tree
        std::shared_ptr<E> a_value;
    };

    //******************
    //
    // Graph structures
    //
    //******************

    template <typename E>
    class Graph {
        // Class representating a graph
    public:
        // Node in a graph
        class Node {
        public:
            // Links for a node in a graph
            struct Link{std::weak_ptr<Node> target;};

            // Node constructor
            Node(E new_value, scls::Fraction x, scls::Fraction y):a_x(x),a_y(y){set_value(new_value);};
            Node(){};

            // Returns the value into an image
            template <typename X = E> std::enable_if<!std::is_base_of<X,std::string>::value,std::shared_ptr<scls::Image>>::type image_value() {std::shared_ptr<scls::Image> to_return = scls::to_image(a_value.get());return to_return;};
            template <typename X = E> std::enable_if<std::is_base_of<X,std::string>::value,std::shared_ptr<scls::Image>>::type image_value() {scls::Text_Style style;style.alignment_horizontal=scls::H_Center;std::shared_ptr<scls::Image> to_return = scls::to_image(a_value.get(), style);return to_return;};

            // Getters and setters
            inline void set_value(E new_value){a_value = std::make_shared<E>(new_value);};
            inline E* value() const {return a_value.get();};
            inline scls::Fraction x() const {return a_x;};
            inline scls::Fraction y() const {return a_y;};
        private:
            // Links to others nodes
            std::vector<Link> a_links;
            // Value in the node
            std::shared_ptr<E> a_value;
            // X value of the node
            scls::Fraction a_x;
            // Y value of the node
            scls::Fraction a_y;
        };

        // Graph constructor
        Graph(){};

        // Adds a node in the graph
        inline void add_node(E value, scls::Fraction x, scls::Fraction y){a_nodes.push_back(std::make_shared<Node>(value, x, y));};
        inline void add_node(E value){add_node(value, 0, 0);};

        // Getters and setters
        inline std::vector<std::shared_ptr<Node>>& nodes() {return a_nodes;};

        // Returns the tree in an image
        std::shared_ptr<scls::Image> image() {
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

                // Draw the final image
                int node_to_node_separation = 20;
                int root_to_node_separation = 30;
                scls::Fraction total_height = max_y - min_y;
                scls::Fraction total_width = max_x - min_x;
                to_return = std::make_shared<scls::Image>(total_width.to_double(), total_height.to_double(), scls::Color(255, 255, 255));

                /*// Draw the links
                int base_x = to_return.get()->width() / 2;
                int base_y = 0;
                int current_x = 0;
                int current_y = base_y + root_to_node_separation;
                scls::Color link_color = scls::Color(0, 0, 0); int link_width = 2;
                for(int i = 0;i<static_cast<int>(images.size());i++) {
                    std::shared_ptr<scls::Image> current_image = images[i];
                    to_return.get()->draw_line(base_x - link_width / 2, base_y, current_x + current_image.get()->width() / 2, current_y, link_color, link_width);
                    current_x += current_image.get()->width() + node_to__node_separation;
                } //*/

                // Paste the children
                for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                    std::shared_ptr<scls::Image> current_image = images[i];
                    scls::Fraction current_x = (a_nodes[i].get()->x() * pixel_in_value - min_x);
                    current_x -= scls::Fraction(current_image.get()->width(), 2);
                    scls::Fraction current_y = total_height - (a_nodes[i].get()->y() * pixel_in_value - min_y);
                    current_y -= scls::Fraction(current_image.get()->height(), 2);
                    to_return.get()->paste(current_image.get(), current_x.to_double(), current_y.to_double());
                }
            }
            return to_return;
        };

    private:
        // Nodes in the tree
        std::vector<std::shared_ptr<Node>> a_nodes;
    };
}

#endif // PLEOS_IT_DATA_STRUCTURE
