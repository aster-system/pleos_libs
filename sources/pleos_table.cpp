//******************
//
// pleos_text.h
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
// This file contains the source code of pleos_table.cpp.
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
#include "../pleos_table.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //*********
	//
	// PLEOS Table handler
	//
	//*********

	// Height / width of the case
    int __Table_Case::Table_Case::height() const {if(image.get() == 0){return 0;} return image.get()->height() + margin_bottom + margin_top;};
    int __Table_Case::Table_Case::width() const {if(image.get() == 0){return 0;}return image.get()->width() + margin * 2;};

	// Handle the title
    std::shared_ptr<scls::__Image_Base> Illustrator::title_image(scls::Text_Image_Generator* tig){if(a_title == std::string()){return std::shared_ptr<scls::__Image_Base>();}return tig->image_shared_ptr(a_title, *title_style());};

    // Checks the merge of the case
    void __Table_Case::check_merge() {
        // Reset merging
        for(int i = 0;i<static_cast<int>(a_cases.size());i++) {
            for(int j = 0;j<static_cast<int>(a_cases.at(i).size());j++) {
                case_at(i, j)->merged = __Table_Case::Table_Case::Merge_State::MS_No;
            }
        }

        // Check merging
        for(int i = 0;i<static_cast<int>(a_cases.size());i++) {
            for(int j = 0;j<static_cast<int>(a_cases.at(i).size());j++) {
                __Table_Case::Table_Case* current_case = case_at(i, j);
                if(current_case->merged != __Table_Case::Table_Case::Merge_State::MS_Merged){
                    // Check the merged cases
                    const int temp_i = i;const int temp_j = j;
                    int case_width = 1;bool stop = false;
                    while(i < static_cast<int>(a_cases.size())) {
                        j = temp_j;if(i == temp_i){j++;}
                        while(j < static_cast<int>(a_cases.at(i).size())) {
                            if(!(i == temp_i && j == temp_j)) {
                                __Table_Case::Table_Case* analysed_case = case_at(i, j);
                                if(current_case->image.get() == analysed_case->image.get()){
                                    analysed_case->merged = __Table_Case::Table_Case::Merge_State::MS_Merged;
                                    current_case->merged = __Table_Case::Table_Case::Merge_State::MS_Merged_Main;
                                    case_width++;
                                }
                                else{if(j <= temp_j){stop = true;}break;}
                            }
                            j++;
                        }
                        if(stop){break;}i++;
                    }
                    i = temp_i;j = temp_j;
                }
            }
        }
    }

    // Returns the width of a column
    int __Table_Case::column_width(int column) const{
        if(column >= static_cast<int>(a_cases.size())){return 0;}

        int to_return = 0;
        for(int i = 0;i<static_cast<int>(a_cases.at(column).size());i++){
            if(a_cases.at(column).at(i).get() != 0 && a_cases.at(column).at(i).get()->width() > to_return && a_cases.at(column).at(i).get()->merged == __Table_Case::Table_Case::Merge_State::MS_No){
                to_return = a_cases.at(column).at(i).get()->width();
            }
        }

        // Return the result
        if(to_return < a_minimum_case_width){return a_minimum_case_width;}
        return to_return;
    };
    int __Table_Case::column_width(int column, int width) const{int to_return = 0;for(int i = 0;i<width;i++){to_return += column_width(i + column);if(i>0){to_return += column_separation();}}return to_return;};

    // Returns the height of a line
    int __Table_Case::line_height(int line) const {
        int to_return = 0;
        for(int i = 0;i<static_cast<int>(a_cases.size());i++){
            if(static_cast<int>(a_cases.at(i).size()) > line && a_cases.at(i)[line].get() != 0){
                if(a_cases.at(i).at(line).get()->height() > to_return && a_cases.at(i).at(line).get()->merged == __Table_Case::Table_Case::Merge_State::MS_No){
                    to_return = a_cases.at(i).at(line).get()->height();
                }
            }
        }

        if(to_return == 0) {
            // Only merged case avaible
            for(int i = 0;i<static_cast<int>(a_cases.size());i++){
                if(static_cast<int>(a_cases.at(i).size()) > line && a_cases.at(i)[line].get() != 0){
                    if(a_cases.at(i).at(line).get()->height() > to_return){
                        to_return = a_cases.at(i).at(line).get()->height();
                    }
                }
            }
        }

        return to_return;
    }

    // Returns the number of lines in the table
    int __Table_Case::line_number() const {
        int to_return = 0;
        for(int i = 0;i<static_cast<int>(a_cases.size());i++){
            if(static_cast<int>(a_cases.at(i).size()) > to_return){
                to_return = a_cases.at(i).size();
            }
        }
        return to_return;
    }

    // Returns the case at a certain position
    __Table_Case::Table_Case* __Table_Case::case_at(int x, int y){
        while(static_cast<int>(a_cases.size()) <= x){a_cases.push_back(std::vector<std::shared_ptr<__Table_Case::Table_Case>>());}
        while(static_cast<int>(a_cases[x].size()) <= y){a_cases[x].push_back(std::shared_ptr<__Table_Case::Table_Case>());}
        if(a_cases[x][y].get()==0){
            // Create the case
            a_cases[x][y] = std::make_shared<__Table_Case::Table_Case>();
            a_cases[x][y].get()->image = std::make_shared<scls::Image>();
            a_cases[x][y].get()->parent_table_weak_ptr = a_this_object;
        }
        return a_cases[x][y].get();
    };

    // Loads cases in the table
    void __Table_Case::load_cases(std::shared_ptr<scls::__XML_Text_Base> cases, scls::Text_Style style, scls::Text_Image_Generator* tig) {
        // Get the needed datas
        std::vector<scls::XML_Attribute>& attributes = cases.get()->xml_balise_attributes();
        int height = 0;int width = 0;int x = 0;int y = 0;std::string to_load = std::string();
        for(int i = 0;i<static_cast<int>(attributes.size());i++) {
            if(attributes[i].name == std::string("load")){to_load = attributes[i].value;}
            else if(attributes[i].name == std::string("height")){height = std::stoi(attributes[i].value);}
            else if(attributes[i].name == std::string("width")){width = std::stoi(attributes[i].value);}
            else if(attributes[i].name == std::string("x")){x = std::stoi(attributes[i].value);}
            else if(attributes[i].name == std::string("y")){y = std::stoi(attributes[i].value);}
        }
        std::vector<std::string> cutted = scls::cut_string(to_load, std::string("-"));

        // Loads each cases
        if(cutted.at(0) == std::string("sequence_arithmetic")){
            if(cutted.size() < 2){scls::print("Warning", std::string("PLEOS Table"), std::string("Not enough argument to load the cases \"") + cases.get()->text() + std::string("."));return;}
            scls::Formula first_part = scls::string_to_formula(cutted.at(1));
            scls::Formula reason = scls::string_to_formula(cutted.at(2));

            // Get the cases
            scls::Textual_Math_Settings settings;settings.set_hide_if_0(false);
            scls::Formula current = first_part;
            int needed_size = std::max(width, height);
            for(int i = 0;i<needed_size;i++) {
                int needed_x = x;int needed_y = y;
                if(width > 1){needed_x += i;}else{needed_y += i;}
                set_case_value(needed_x, needed_y, current.to_std_string(&settings), style, tig);
                current += reason;
            }
        }
        else if(cutted.at(0) == std::string("sequence_geometric")){
            if(cutted.size() < 2){scls::print("Warning", std::string("PLEOS Table"), std::string("Not enough argument to load the cases \"") + cases.get()->text() + std::string("."));return;}
            scls::Formula first_part = scls::string_to_formula(cutted.at(1));
            scls::Formula reason = scls::string_to_formula(cutted.at(2));

            // Get the cases
            scls::Textual_Math_Settings settings;settings.set_hide_if_0(false);
            scls::Formula current = first_part;
            int needed_size = std::max(width, height);
            for(int i = 0;i<needed_size;i++) {
                int needed_x = x;int needed_y = y;
                if(width > 1){needed_x += i;}else{needed_y += i;}
                set_case_value(needed_x, needed_y, scls::format_number_to_text(current.value_to_double()), style, tig);
                current *= reason;
            }
        }
    }

    // Merges cases
    void __Table_Case::merge_cases(int x, int y, int width, int height){
        case_at(x, y)->merged_height = height;case_at(x, y)->merged_width = width;
        for(int i = 0;i<static_cast<int>(width);i++){
            for(int j = 0;j<static_cast<int>(height);j++){
                if(!(i == 0 && j == 0)){
                    case_at(x + i, y + j);
                    a_cases[x + i][y + j].get()->image = a_cases[x][y].get()->image;
                }
            }
        }
    }

    // Set the value of an std::string case
    std::shared_ptr<scls::__Image_Base> __Table_Case::case_image_from_text(std::string value, scls::Text_Style needed_style, scls::Text_Image_Generator* tig){return tig->image_shared_ptr(value, needed_style);}
    void __Table_Case::set_case_value(int x, int y, std::string value, scls::Text_Style needed_style, scls::Text_Image_Generator* tig){(*case_at(x, y)->image.get()) = case_image_from_text(value, needed_style, tig);case_at(x, y)->content = value;case_at(x, y)->style = needed_style;};
    void __Table_Case::set_cases_value(int x, int y, int width, int height, std::string value, scls::Text_Style needed_style, scls::Text_Image_Generator* tig) {
        std::shared_ptr<scls::__Image_Base> img = case_image_from_text(value, needed_style, tig);
        (*case_at(x, y)->image.get()) = img;
        case_at(x, y)->content = value;
        merge_cases(x, y, width, height);
    }

    // Returns the table to an image
    scls::Image __Table_Case::to_image() {
        // Get the needed datas
        int bottom_border = 2;
        int left_border = 2;
        int line_separation = 2;
        int needed_columns = column_number();
        int needed_lines = line_number();
        int right_border = 2;
        int top_border = 2;

        // Handle illustrator
        scls::Text_Image_Generator tig;
        std::shared_ptr<scls::__Image_Base> needed_title_image = title_image(&tig);

        // Create the image
        check_merge();
        scls::Color background_color = scls::Color(255, 255, 255);
        int needed_total_height = total_height_in_pixel() + (line_separation * (needed_lines - 1)) + top_border + bottom_border;
        int needed_total_width = total_width_in_pixel() + (column_separation() * (needed_columns - 1)) + left_border + right_border;
        int final_total_width = needed_total_width;
        if(needed_title_image.get() != 0){needed_total_height += needed_title_image.get()->height();}
        if(needed_title_image.get() != 0 && needed_title_image.get()->width() > final_total_width){final_total_width = needed_title_image.get()->width();if(final_total_width-needed_total_width%2==1){final_total_width++;}}
        scls::Image to_return = scls::Image(final_total_width, needed_total_height, background_color);

        // Get the datas for the drawing
        // X datas
        std::vector<int> needed_width = std::vector<int>(needed_columns, 0);
        int x_start = left_border;if(needed_title_image.get() != 0 && needed_title_image.get()->width() == final_total_width){x_start = (final_total_width - needed_total_width) / 2 + left_border;}
        std::vector<int> needed_x = std::vector<int>(needed_columns, x_start);
        for(int i = 0;i<static_cast<int>(a_cases.size());i++){needed_width[i] = column_width(i);if(i > 0){needed_x[i] = needed_x[i - 1] + needed_width[i - 1] + column_separation();}}
        // Y datas
        std::vector<int> needed_height = std::vector<int>(needed_lines, 0);
        int y_start = top_border;if(needed_title_image.get() != 0){y_start += needed_title_image.get()->height();}
        std::vector<int> needed_y = std::vector<int>(needed_lines, y_start);
        for(int i = 0;i<needed_lines;i++){needed_height[i] = line_height(i);if(i > 0){needed_y[i] = needed_y[i - 1] + needed_height[i - 1] + line_separation;}}

        // Draw the border
        scls::Color border_color = scls::Color(0, 0, 0);
        if(needed_title_image.get() != 0 && needed_title_image.get()->width() == final_total_width){x_start = (final_total_width - needed_total_width) / 2 + left_border;}else{x_start = 0;}
        if(needed_title_image.get() != 0){y_start = needed_title_image.get()->height();}else{y_start = 0;}
        to_return.fill_rect(x_start, y_start, left_border, to_return.height(), border_color);
        to_return.fill_rect(x_start, to_return.height() - bottom_border, needed_total_width, bottom_border, border_color);
        to_return.fill_rect((x_start + needed_total_width) - right_border, y_start, right_border, to_return.height(), border_color);
        to_return.fill_rect(x_start, y_start, needed_total_width, top_border, border_color);

        // Draw each columns
        for(int i = 0;i<static_cast<int>(a_cases.size());i++) {
            for(int j = 0;j<static_cast<int>(a_cases.at(i).size());j++) {
                __Table_Case::Table_Case* current_case = case_at(i, j);
                scls::Image* current_image = current_case->image.get();
                if(current_case->merged != __Table_Case::Table_Case::Merge_State::MS_Merged){
                    // Handle merging
                    int current_width = column_width(i, current_case->merged_width);
                    int width_in_pixel = current_image->width();

                    // Set the background color
                    if(current_case->background_color() != background_color){
                        to_return.fill_rect(needed_x[i], needed_y[j], current_width, needed_height[j], current_case->background_color());
                    }

                    // Paste the image
                    if(current_image != 0 && current_image->image() != 0){
                        int current_x = needed_x[i] + (current_width / 2 - width_in_pixel / 2);
                        int current_y = needed_y[j] + (needed_height[j] / 2 - current_image->height() / 2);
                        to_return.paste(current_image, current_x, current_y);
                    }

                    // Draw the separation
                    scls::Color separation_color = scls::Color(0, 0, 0);
                    if(i > 0 && case_at(i - 1, j)->right_border){to_return.fill_rect(needed_x[i] - column_separation(), needed_y[j], column_separation(), needed_height[j] + line_separation, separation_color);}
                    if(j > 0){to_return.fill_rect(needed_x[i], needed_y[j] - line_separation, current_width + column_separation(), line_separation, separation_color);}
                }
            }

            // Missing cases
            for(int j = a_cases.at(i).size();j<static_cast<int>(needed_lines);j++) {
                case_at(i, j);

                // Draw the separation
                scls::Color separation_color = scls::Color(0, 0, 0);
                if(i > 0){to_return.fill_rect(needed_x[i] - column_separation(), needed_y[j], column_separation(), needed_height[j] + line_separation, separation_color);}
                if(j > 0){to_return.fill_rect(needed_x[i], needed_y[j] - line_separation, needed_width[i] + column_separation(), line_separation, separation_color);}
            }
        }

        // Paste the title
        if(needed_title_image.get() != 0){to_return.paste(needed_title_image.get(), to_return.width() / 2 - needed_title_image.get()->width() / 2, 0);}

        // Return the result
        return to_return;
    }

    // Creates and returns a table from an std::string
	std::shared_ptr<__Table_Case> table_from_xml(std::shared_ptr<__Table_Case> to_return, std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style) {
	    scls::Text_Image_Generator tig;

	    // Handle the attributes
	    std::vector<scls::XML_Attribute>& attributes = xml.get()->xml_balise_attributes();
	    std::string to_load = std::string();
	    for(int i = 0;i<static_cast<int>(attributes.size());i++) {
            if(attributes[i].name == std::string("color")){needed_style.set_color(scls::Color::from_std_string(attributes[i].value));}
            else if(attributes[i].name == std::string("font_size")){needed_style.set_font_size(std::stoi(attributes[i].value));}
            else if(attributes[i].name == std::string("load")){to_load = attributes[i].value;}
            else if(attributes[i].name == std::string("minimum_case_width")){to_return.get()->set_minimum_case_width(std::stoi(attributes[i].value));}
            else if(attributes[i].name == std::string("title")){to_return.get()->set_title(attributes[i].value);}
	    }

	    // Built-in loaded tables
	    std::vector<std::string> cutted = scls::cut_string(to_load, std::string(";"));
	    to_return.get()->set_loaded(to_load);
	    if(cutted.size() > 0) {
            if(cutted.at(0) == std::string("arithmetic_base")) {
                int height = 5; int width = 4;

                // Create the parts
                scls::Text_Style style = needed_style;

                // Titles
                style.set_color(scls::Color(0, 0, 0));
                to_return.get()->set_case_value(0, 0, std::string("Base 10"), style.new_child(), &tig);
                to_return.get()->set_case_value(1, 0, std::string("Base 2"), style.new_child(), &tig);
                to_return.get()->set_case_value(2, 0, std::string("Base 8"), style.new_child(), &tig);
                to_return.get()->set_case_value(3, 0, std::string("Base 16"), style.new_child(), &tig);
                style.set_color(scls::Color(0, 0, 0, 0));

                // Needed number
                std::vector<int> bases = {10, 2, 8, 16};
                std::vector<int> to_convert = {1, 2, 5, 10, 20, 100, 287};
                for(int i = 0;i<static_cast<int>(to_convert.size());i++) {
                    for(int j = 0;j<static_cast<int>(bases.size());j++) {
                        to_return.get()->set_case_value(j, i + 1, scls::format_number_to_text(to_convert.at(i), -1, bases.at(j)) , style.new_child(), &tig);
                    }
                }
            }
            else if(cutted.at(0) == std::string("cartesian_product")) {
                int height = 5; int width = 5;

                // Relation
                bool parity_relation = false;
                for(int i = 1;i<static_cast<int>(cutted.size());i++){
                    if(cutted.at(i) == std::string("parity")){parity_relation = true;}
                }

                // Create the parts
                scls::Text_Style style = needed_style;
                for(int i = 0;i<width + 1;i++) {
                    for(int j = 0;j<height + 1;j++) {
                        int needed_x = i;int needed_y = j;
                        if(needed_x == 0 && needed_y != 0){to_return.get()->case_at(needed_x, needed_y)->style.set_background_color(scls::Color(255, 200, 200));style.set_background_color(scls::Color(255, 200, 200));to_return.get()->set_case_value(needed_x, needed_y, std::to_string(j), style.new_child(), &tig);}
                        else if(needed_y == 0 && needed_x != 0){to_return.get()->case_at(needed_x, needed_y)->style.set_background_color(scls::Color(200, 200, 255));style.set_background_color(scls::Color(200, 200, 255));to_return.get()->set_case_value(needed_x, needed_y, std::to_string(i), style.new_child(), &tig);}
                        else if(needed_x != 0 && needed_y != 0){
                            // Get the color by relation
                            scls::Color needed_color = scls::Color(255, 255, 255, 0);
                            if(parity_relation){if(i % 2 == j % 2){needed_color = scls::Color(200, 255, 200);}}

                            to_return.get()->case_at(needed_x, needed_y)->style.set_background_color(needed_color);
                            style.set_background_color(needed_color);
                            to_return.get()->set_case_value(needed_x, needed_y, std::string("(") + std::to_string(j) + std::string(", ") + std::to_string(i) + std::string(")"), style.new_child(), &tig);
                        }
                    }
                }
            }
            else if(cutted.at(0) == std::string("group_cyclic")) {
                int height = 10;int width = 5;

                // Create the parts
                scls::Text_Style style = needed_style;
                for(int i = 0;i<width;i++) {
                    for(int j = 0;j<height;j++) {
                        int needed_x = i;int needed_y = j;
                        scls::Color needed_color = scls::Color(255, 255, 255, 0);

                        to_return.get()->case_at(needed_x, needed_y)->style.set_background_color(needed_color);
                        style.set_background_color(needed_color);
                        scls::Text_Style needed_style = style.new_child();needed_style.set_background_color(scls::Color(0, 0, 0, 0));
                        to_return.get()->set_case_value(needed_x, needed_y, std::to_string(j * width + i + 1), needed_style, &tig);
                    }
                }
            }
	    }

	    // Handle a lot of balises
	    for(int i = 0;i<static_cast<int>(xml->sub_texts().size());i++){
            std::string current_balise_name = xml->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = xml->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "case" || current_balise_name == "case_plus"){
                scls::Color background_color = scls::Color(255, 255, 255);
                scls::Color color = scls::Color(255, 255, 255);bool color_used = false;
                std::string content = std::string();bool content_used = false;
                scls::Text_Style case_style = needed_style.new_child();case_style.set_border_width(0);
                int height = 1;int width = 1;bool right_border = true;int x = 0;int y = 0;
                for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                    if(!scls::text_style_from_xml_attribute(&attributes[i], case_style)) {
                        if(attributes[i].name == std::string("background_color")){background_color = scls::Color::from_std_string(attributes[i].value);}
                        else if(attributes[i].name == std::string("color")){color = scls::Color::from_std_string(attributes[i].value);color_used=true;}
                        else if(attributes[i].name == std::string("content")){content = attributes[i].value;content_used=true;}
                        else if(attributes[i].name == std::string("height")){height = std::stoi(attributes[i].value);}
                        else if(attributes[i].name == std::string("right_border")){if(attributes[i].value==std::string("0")||attributes[i].value==std::string("false")||attributes[i].value==std::string("no")){right_border=false;}}
                        else if(attributes[i].name == std::string("width")){width = std::stoi(attributes[i].value);}
                        else if(attributes[i].name == std::string("x")){x = std::stoi(attributes[i].value);}
                        else if(attributes[i].name == std::string("y")){y = std::stoi(attributes[i].value);}
                    }
                }

                // Create the result
                case_style.set_background_color(scls::Color(0, 0, 0, 0));
                case_style.set_font_size(needed_style.font_size());
                if(color_used){case_style.set_color(color);}
                if(content_used || current_balise_name == "case_plus") {
                    if(current_balise_name == "case_plus"){
                        content = xml->sub_texts()[i].get()->text();
                        if(case_style.max_width() == -1){case_style.set_max_width(to_return.get()->column_width(x, width));}
                    }
                    to_return.get()->set_case_value(x, y, content, case_style, &tig);
                }
                to_return.get()->case_at(x, y)->right_border = right_border;
                to_return.get()->case_at(x, y)->set_background_color(background_color);
                to_return.get()->case_at(x, y)->style = case_style;
                to_return.get()->merge_cases(x, y, width, height);
            }
            else if(current_balise_name == "cases"){to_return.get()->load_cases(xml->sub_texts()[i], needed_style.new_child(), &tig);}
        }

        // Return the result
        return to_return;
	}
}
