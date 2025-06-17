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
        std::shared_ptr<scls::__Image_Base> case_image(int element, scls::Text_Image_Generator& generator, scls::Text_Style& current_style) {
            // Create the text
            std::shared_ptr<scls::__Image_Base> to_add = generator.image_shared_ptr(a_elements[element].to_std_string(), current_style);

            // Create the final image
            return to_add;
        };
        // Returns the list in an image
        std::shared_ptr<scls::__Image_Base> image() {
            // Get each needed images for elements
            scls::Text_Style current_style; current_style.font_size = 70;
            std::vector<std::shared_ptr<scls::__Image_Base>> elements_image;
            scls::Text_Image_Generator generator;
            int max_height = 0;
            int total_width = a_border_width;
            for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                std::shared_ptr<scls::__Image_Base> to_add = case_image(i, generator, current_style);
                total_width += to_add.get()->width(); elements_image.push_back(to_add);
                if(max_height < to_add.get()->height()){max_height = to_add.get()->height() + a_inline_border_width;}
            } max_height += a_border_width * 2;
            max_height = 72; total_width = 300;

            // Create the final image
            int current_x = a_border_width;
            std::shared_ptr<scls::__Image_Base> to_return = std::make_shared<scls::__Image_Base>(total_width, max_height, scls::Color(255, 255, 255, 255));
            // Draw the border
            to_return.get()->fill_rect(0, 0, to_return.get()->width(), a_border_width, scls::Color(0, 0, 0));
            //to_return.get()->fill_rect(0, 0, a_border_width, to_return.get()->height(), scls::Color(0, 0, 0));
            to_return.get()->fill_rect(0, to_return.get()->height() - a_border_width, to_return.get()->width(), a_border_width, scls::Color(0, 0, 0));
            // Draw the elements
            for(int i = 0;i<static_cast<int>(elements_image.size());i++) {
                std::shared_ptr<scls::__Image_Base> current_image = elements_image[i];
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
        std::shared_ptr<scls::__Image_Base> image() {
            // Get each needed images for elements
            int border_width = 4; int inline_border_width = 2;
            scls::Text_Style current_style; current_style.font_size = 70;
            std::vector<std::shared_ptr<scls::__Image_Base>> elements_image;
            scls::Text_Image_Generator generator;
            int max_width = 0;
            int total_height = border_width;
            for(int i = 0;i<static_cast<int>(List<E>::elements().size());i++) {
                std::shared_ptr<scls::__Image_Base> to_add = List<E>::case_image(i, generator, current_style);
                total_height += to_add.get()->height(); elements_image.push_back(to_add);
                if(max_width < to_add.get()->width()){max_width = to_add.get()->width() + inline_border_width;}
            } max_width += border_width * 2;
            max_width = 72; total_height = 300;

            // Create the final image
            int current_y = total_height - border_width;
            std::shared_ptr<scls::__Image_Base> to_return = std::make_shared<scls::__Image_Base>(max_width, total_height, scls::Color(255, 255, 255, 255));
            // Draw the border
            to_return.get()->fill_rect(0, 0, border_width, to_return.get()->height(), scls::Color(0, 0, 0));
            to_return.get()->fill_rect(to_return.get()->width() - border_width, 0, border_width, to_return.get()->height(), scls::Color(0, 0, 0));
            to_return.get()->fill_rect(0, to_return.get()->height() - border_width, to_return.get()->width(), border_width, scls::Color(0, 0, 0));
            // Draw the elements
            for(int i = 0;i<static_cast<int>(elements_image.size());i++) {
                std::shared_ptr<scls::__Image_Base> current_image = elements_image[i];
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

    class __Graph_Base {
        // Class representating a graph
    public:
        // Node in a __Graph_Base
        class __Node_Base {
        public:
            // Links for a node in a graph
            enum Link_Type{LT_Middle, LT_Angle, LT_Bottom, LT_Left, LT_Right, LT_Top, LT_X, LT_Y};
            struct Link{std::shared_ptr<scls::__Image_Base> ponderation;Link_Type type = Link_Type::LT_Middle;std::weak_ptr<__Node_Base> target;};

            // __Node_Base constructor
            __Node_Base(std::shared_ptr<scls::__Image_Base> new_image, scls::Fraction x, scls::Fraction y, int id);
            __Node_Base();

            // Links this node with another node
            Link* link(std::shared_ptr<__Node_Base> node);
            // Returns a link by the linked object
            Link* link_by_object(__Node_Base* node);
            Link* link_by_object(std::shared_ptr<__Node_Base> node);

            // Returns the value into an image
            //template <typename X = E> std::enable_if<!std::is_base_of<X,std::string>::value,std::shared_ptr<scls::__Image_Base>>::type image_value() {std::shared_ptr<scls::__Image_Base> to_return = scls::to_image(a_value.get());return to_return;};
            //template <typename X = E> std::enable_if<std::is_base_of<X,std::string>::value,std::shared_ptr<scls::__Image_Base>>::type image_value() {style().set_alignment_horizontal(scls::H_Center);std::shared_ptr<scls::__Image_Base> to_return = scls::to_image(a_value.get(), style());return to_return;};
            // Returns a node to an std::string
            virtual std::string to_xml_text(std::string node_object_name, std::string node_content);
            std::string to_xml_text_x();
            std::string to_xml_text_y();

            // Getters and setters
            inline int id() const {return a_id;};
            inline scls::__Image_Base* image() const {return a_image.get();};
            inline std::shared_ptr<scls::__Image_Base> image_shared_ptr() const {return a_image;};
            inline std::vector<Link>& links() {return a_links;};
            inline scls::Text_Style& style() {return a_style;};
            inline void set_image(std::shared_ptr<scls::__Image_Base> new_value){a_image = new_value;};
            inline void set_image(std::string value){style().set_alignment_horizontal(scls::H_Center);std::shared_ptr<scls::__Image_Base> to_return = scls::to_image(&value, style());set_image(to_return);};
            inline void set_style(scls::Text_Style new_style){a_style = new_style;};
            inline void set_x(scls::Fraction new_x){a_x = new_x;};
            inline void set_y(scls::Fraction new_y){a_y = new_y;};
            inline scls::Fraction x() const {return a_x;};
            inline scls::Fraction y() const {return a_y;};
        private:
            // ID of the node
            int a_id = 0;
            // Value in the node
            std::shared_ptr<scls::__Image_Base> a_image;
            // Links to others nodes
            std::vector<Link> a_links;
            // Style of the node
            scls::Text_Style a_style;
            // X value of the node
            scls::Fraction a_x;
            // Y value of the node
            scls::Fraction a_y;
        }; typedef __Node_Base::Link Link;typedef __Node_Base::Link_Type Link_Type;

        // __Graph_Base constructor
        __Graph_Base(){};

        // Adds a node in the graph
        virtual std::shared_ptr<__Node_Base> __create_node(std::shared_ptr<scls::__Image_Base> image, scls::Fraction x, scls::Fraction y){std::shared_ptr<__Graph_Base::__Node_Base>to_return=std::make_shared<__Node_Base>(image, x, y, a_nodes.size());return to_return;};
        int add_node(std::shared_ptr<scls::__Image_Base> image, scls::Fraction x, scls::Fraction y);
        int add_node(std::shared_ptr<scls::__Image_Base> image);
        int add_node();
        // Links two nodes in the graph
        bool link_nodes(int id_1, int id_2);
        // Sets the ponderation of a link node
        void set_link_ponderation(int id_1, int id_2, std::shared_ptr<scls::__Image_Base> needed_ponderation);
        void set_link_ponderation(int id_1, int id_2, std::string needed_ponderation, scls::Text_Style style, scls::Text_Image_Generator* tig);
        void set_link_ponderation(int id_1, int id_2, std::string needed_ponderation, scls::Text_Style style);

        // Getters and setters
        __Node_Base* node(int id)const;
        std::vector<std::shared_ptr<__Node_Base>>& nodes();

        // Returns the tree in an image
        std::shared_ptr<scls::__Image_Base> to_image(std::shared_ptr<scls::Text_Style> needed_style);
        std::shared_ptr<scls::__Image_Base> to_image();
        // Returns the graph to an XML text
        virtual std::string to_xml_text(){return to_xml_text(std::string("graph"));};
        virtual std::string to_xml_text(std::string object_name){return std::string();};
        virtual std::string to_xml_text_content(){return std::string();};

    private:
        // Nodes in the tree
        std::vector<std::shared_ptr<__Node_Base>> a_nodes;
    };
    template <typename E = std::string> class Graph : public __Graph_Base{
        // Class representating a graph specified for a particular data
    public:

        // Node in a Graph
        class Node : public __Node_Base {
        public:
            // __Node_Base constructor
            Node(std::shared_ptr<scls::__Image_Base> new_image, scls::Fraction x, scls::Fraction y, int id):__Graph_Base::__Node_Base(new_image,x,y,id){};
            Node():__Graph_Base::__Node_Base(){};

            // Getters and setters
            inline void set_value(E new_value){a_value = std::make_shared<E>(new_value);};
            inline E* value() const {return a_value.get();};
        private:
            // Value of the node
            std::shared_ptr<E> a_value;
        };

        // Graph constructor
        Graph():__Graph_Base(){};

        // Adds a node to the graph
        virtual std::shared_ptr<__Node_Base> __create_node(std::shared_ptr<scls::__Image_Base> image, scls::Fraction x, scls::Fraction y){std::shared_ptr<Node>to_return=std::make_shared<Node>(image, x, y, nodes().size());return to_return;};
        int add_node(E value, scls::Fraction x, scls::Fraction y){Node* new_node = reinterpret_cast<Node*>(node(__Graph_Base::add_node(scls::to_image(&value), x, y)));new_node->set_value(value);return new_node->id();};
        int add_node(E value){return add_node(value, 0, 0);};

        // Returns the graph to an XML text
        virtual std::string to_xml_text(){return to_xml_text(std::string("graph"));};
        virtual std::string to_xml_text(std::string object_name){std::string to_return = std::string("<") + object_name + std::string(">\n") + to_xml_text_content() + std::string("</") + object_name + std::string(">");return to_return;};
        virtual std::string to_xml_text_content(){
            std::string to_return = std::string();
            for(int i = 0;i<static_cast<int>(nodes().size());i++){
                if(nodes().at(i).get()!=0){
                    if(reinterpret_cast<Node*>(nodes().at(i).get())->value()!=0){to_return += nodes().at(i).get()->to_xml_text("node", *reinterpret_cast<Node*>(nodes().at(i).get())->value()) + std::string("\n");}
                    else{to_return += nodes().at(i).get()->to_xml_text("node", E()) + std::string("\n");}
                }
            }
            return to_return;
        };
    };

    //******************
    // Linked list
    //******************

    class Linked_List {
        // Class representating a linked-list (in a graph environment)
    public:
        // Linked_List constructor
        Linked_List(std::shared_ptr<__Graph_Base> graph, int root_id);
        Linked_List();

        // Add a child in the graph
        std::shared_ptr<Linked_List> add_child(std::shared_ptr<scls::__Image_Base> image);
        std::shared_ptr<Linked_List> add_child(std::string value);
        // Place the child in the graph
        void place_child();
        void __place_child_directly(Linked_List* needed_child);
        void place_child_directly();

        // Return the image of the graph attached to the linked-list
        std::shared_ptr<scls::__Image_Base> to_image(std::shared_ptr<scls::Text_Style> needed_style);
        std::shared_ptr<scls::__Image_Base> to_image();

        // Getters and setters
        inline std::shared_ptr<Linked_List> child() const {return a_child;};
        inline bool ignore_for_placement() const{return a_ignore_for_placement;};
        inline auto& nodes() {return a_graph.get()->nodes();};
        inline Linked_List* parent() const{return a_parent.lock().get();};
        inline auto* root() {return nodes()[a_root_id].get();};
        inline int root_id() const {return a_root_id;};
        inline void set_ignore_for_placement(bool new_ignore_for_placement){a_ignore_for_placement = new_ignore_for_placement;};
        inline void set_image(std::shared_ptr<scls::__Image_Base> new_value){root()->set_image(new_value);};
        inline void set_image(std::string value){root()->set_image(value);};
        inline void set_y(scls::Fraction new_y){a_root_y = new_y;};

    private:

        // Children of this list
        std::shared_ptr<Linked_List> a_child;
        // Attached graph for this tree
        std::shared_ptr<__Graph_Base> a_graph = std::make_shared<__Graph_Base>();
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

    template <typename E = std::string> class Tree {
        // Class representating a tree
    public:
        // Tree constructor
        Tree(std::shared_ptr<Graph<E>> graph, int root_id):a_graph(graph),a_root_id(root_id){};
        Tree():a_root_id(a_graph.get()->__Graph_Base::add_node()){};

        // Return the image of the graph attached to the tree
        std::shared_ptr<scls::__Image_Base> to_image(){place_nodes();return a_graph.get()->to_image();};

        // Adds a node in the tree
        inline Tree* add_node(std::shared_ptr<scls::__Image_Base> new_image){int current_id = a_graph.get()->__Graph_Base::add_node(new_image);a_children.push_back(std::make_shared<Tree>(a_graph, current_id));Tree* current = a_children[a_children.size() - 1].get();current->set_direction(a_direction);a_graph.get()->link_nodes(a_root_id,current_id);return current;}
        inline Tree* add_node(std::string value){int current_id = a_graph.get()->add_node(value);a_children.push_back(std::make_shared<Tree>(a_graph, current_id));Tree* current = a_children[a_children.size() - 1].get();current->set_direction(a_direction);a_graph.get()->link_nodes(a_root_id,current_id);return current;}
        inline Tree* add_node(int sub_id, std::shared_ptr<scls::__Image_Base> new_image){Tree* needed_child = child(sub_id);if(needed_child==0){return 0;}return needed_child->add_node(new_image);};
        inline Tree* add_node(int sub_id, std::string value){return add_node(sub_id, scls::to_image(&value));}
        inline Tree* add_node(int sub_id_1, int sub_id_2, std::shared_ptr<scls::__Image_Base> new_image){Tree* needed_child = child(sub_id_1);if(needed_child==0){return 0;}return needed_child->add_node(sub_id_2, new_image);};
        inline Tree* add_node(int sub_id_1, int sub_id_2, std::string value){return add_node(sub_id_1, sub_id_2, scls::to_image(&value));}
        // Places the nodes in the tree
        void place_nodes(){
            // Place the root
            a_graph.get()->node(a_root_id)->set_x(a_root_x);
            a_graph.get()->node(a_root_id)->set_y(a_root_y);

            // Get the needed datas
            scls::Point_2D direction = a_direction.normalized();
            scls::Point_2D director = direction.rotated(90);
            scls::Fraction distance_child_child = 1;
            //scls::Fraction distance_child_parent = 1;
            scls::Fraction needed_width = total_width(distance_child_child);
            if(a_children.size() > 0){needed_width -= a_children.at(0).get()->total_width(distance_child_child);}
            scls::Fraction start_x = (a_root_x + direction.x()) - (needed_width * direction.y()) / 2;
            scls::Fraction start_y = (a_root_y + direction.y()) + (needed_width * direction.x()) / 2;

            // Place the children
            scls::Fraction current_x = start_x;
            scls::Fraction current_y = start_y;
            for(int i = 0;i<static_cast<int>(a_children.size());i++) {
                scls::Fraction child_width = a_children[i].get()->total_width(distance_child_child);
                a_children[i].get()->a_root_x = current_x;
                a_children[i].get()->a_root_y = current_y;
                a_children[i].get()->place_nodes();
                current_x += director.x() * child_width;
                current_y += director.y() * child_width;
            }
        };
        // Returns a sub-tree in the tree
        //inline Tree<E>* sub_tree(int n){while(static_cast<int>(a_nodes.size()) <= n){a_nodes.push_back(std::make_shared<Tree>());}return a_nodes[n].get();};
        // Returns the total width of the object
        inline scls::Fraction width(){scls::Fraction to_return = 0;for(int i = 0;i<static_cast<int>(a_children.size());i++){to_return+=a_children[i].get()->width();}if(to_return<=1){return 1;}return to_return;};

        // Getters and setters
        inline Tree* child(int id){for(int i = 0;i<static_cast<int>(a_children.size());i++){if(a_children.at(i).get()->root_id()==id){return a_children.at(i).get();}}return 0;};
        inline std::vector<std::shared_ptr<Tree>>& children(){return a_children;};
        inline Graph<E>* graph() const {return a_graph.get();};
        inline scls::__Image_Base* image() const {return a_graph.get()->node(a_root_id)->image();};
        inline std::shared_ptr<scls::__Image_Base> image_shared_ptr() const {return a_graph.get()->node(a_root_id)->image_shared_ptr();};
        inline auto& nodes() {return a_graph.get()->nodes();};
        inline auto* root() {return nodes()[a_root_id].get();};
        inline int root_id() const {return a_root_id;};
        inline void set_direction(scls::Point_2D new_direction) {a_direction = new_direction;for(int i = 0;i<static_cast<int>(a_children.size());i++){a_children.at(i).get()->a_direction=new_direction;}place_nodes();};
        inline void set_image(std::shared_ptr<scls::__Image_Base> new_image){nodes().at(a_root_id).get()->set_image(new_image);};
        inline void set_image(std::string value){nodes().at(a_root_id).get()->set_image(value);};

    private:
        // Total width of the tree
        scls::Fraction total_width(scls::Fraction separation_child_child){
            if(a_children.size() <= 0){return separation_child_child;}

            scls::Fraction to_return = 0;
            for(int i = 0;i<static_cast<int>(a_children.size());i++){to_return += a_children.at(i).get()->total_width(separation_child_child);}
            return to_return;
        };

        // Children of this tree
        std::vector<std::shared_ptr<Tree>> a_children = std::vector<std::shared_ptr<Tree>>();
        // Attached graph for this tree
        std::shared_ptr<Graph<E>> a_graph = std::make_shared<Graph<E>>();

        // Datas for the root
        // Direction for the tree
        scls::Point_2D a_direction = scls::Point_2D(0, -1);
        // ID of the roots for this tree
        int a_root_id = 0;
        // X value of the root
        scls::Fraction a_root_x = 0;
        // Y value of the root
        scls::Fraction a_root_y = 0;
    };
}

#endif // PLEOS_IT_DATA_STRUCTURE
