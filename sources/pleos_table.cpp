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

	// Handle the title
    std::shared_ptr<scls::__Image_Base> Illustrator::title_image(scls::Text_Image_Generator* tig){if(a_title == std::string()){return std::shared_ptr<scls::__Image_Base>();}return tig->image_shared_ptr(a_title, *title_style());};

    // Checks the merge of the case
    void Table::check_merge() {
        // Reset merging
        for(int i = 0;i<static_cast<int>(a_cases.size());i++) {
            for(int j = 0;j<static_cast<int>(a_cases.at(i).size());j++) {
                case_at(i, j)->merged = Table::Table_Case::Merge_State::MS_No;
            }
        }

        // Check merging
        for(int i = 0;i<static_cast<int>(a_cases.size());i++) {
            for(int j = 0;j<static_cast<int>(a_cases.at(i).size());j++) {
                Table::Table_Case* current_case = case_at(i, j);
                if(current_case->merged != Table::Table_Case::Merge_State::MS_Merged){
                    // Check the merged cases
                    const int temp_i = i;const int temp_j = j;
                    int case_width = 1;bool stop = false;
                    while(i < static_cast<int>(a_cases.size())) {
                        j = temp_j;if(i == temp_i){j++;}
                        while(j < static_cast<int>(a_cases.at(i).size())) {
                            Table::Table_Case* analysed_case = case_at(i, j);
                            if(current_case->image.get() == analysed_case->image.get()){
                                analysed_case->merged = Table::Table_Case::Merge_State::MS_Merged;
                                current_case->merged = Table::Table_Case::Merge_State::MS_Merged_Main;
                                case_width++;
                            }
                            else{if(j < temp_j + 1 && i >= temp_i + 1){stop = true;}break;}
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
    int Table::column_width(int column) const{
        if(column >= static_cast<int>(a_cases.size())){return 0;}

        int to_return = 0;
        for(int i = 0;i<static_cast<int>(a_cases.at(column).size());i++){
            if(a_cases.at(column).at(i).get() != 0 && a_cases.at(column).at(i).get()->width() > to_return && a_cases.at(column).at(i).get()->merged == Table::Table_Case::Merge_State::MS_No){
                to_return = a_cases.at(column).at(i).get()->width();
            }
        }

        // Return the result
        if(to_return < a_minimum_case_width){return a_minimum_case_width;}
        return to_return;
    };
    int Table::column_width(int column, int width) const{int to_return = 0;for(int i = 0;i<width;i++){to_return += column_width(i + column);if(i>0){to_return += column_separation();}}return to_return;};

    // Returns the height of a line
    int Table::line_height(int line) const {
        int to_return = 0;
        for(int i = 0;i<static_cast<int>(a_cases.size());i++){
            if(static_cast<int>(a_cases.at(i).size()) > line && a_cases.at(i)[line].get() != 0){
                if(a_cases.at(i).at(line).get()->height() > to_return && a_cases.at(i).at(line).get()->merged == Table::Table_Case::Merge_State::MS_No){
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
    int Table::line_number() const {
        int to_return = 0;
        for(int i = 0;i<static_cast<int>(a_cases.size());i++){
            if(static_cast<int>(a_cases.at(i).size()) > to_return){
                to_return = a_cases.at(i).size();
            }
        }
        return to_return;
    }

    // Returns the case at a certain position
    Table::Table_Case* Table::case_at(int x, int y){
        while(static_cast<int>(a_cases.size()) <= x){a_cases.push_back(std::vector<std::shared_ptr<Table::Table_Case>>());}
        while(static_cast<int>(a_cases[x].size()) <= y){a_cases[x].push_back(std::shared_ptr<Table::Table_Case>());}
        if(a_cases[x][y].get()==0){
            // Create the case
            a_cases[x][y] = std::make_shared<Table::Table_Case>();
            a_cases[x][y].get()->image = std::make_shared<std::shared_ptr<scls::__Image_Base>>();
        }
        return a_cases[x][y].get();
    };

    // Set the value of an std::string case
    void Table::set_cases_value(int x, int y, int width, int height, std::string value, scls::Text_Style* needed_style, scls::Text_Image_Generator* tig) {
        std::shared_ptr<scls::__Image_Base> img = tig->image_shared_ptr(value, *needed_style);
        (*case_at(x, y)->image.get()) = img;
        for(int i = 0;i<width;i++) {
            for(int j = 0;j<height;j++) {
                if(!(i == 0 && j == 0)) {
                    (*case_at(x + i, y + j)->image.get()) = img.get()->copy_image();
                }
            }
        }
    }

	// Returns the table to an image
    std::shared_ptr<scls::__Image_Base> Table::to_image() {
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
        int needed_total_height = total_height() + (line_separation * (needed_lines - 1)) + top_border + bottom_border;
        int needed_total_width = total_width() + (column_separation() * (needed_columns - 1)) + left_border + right_border;
        int final_total_width = needed_total_width;
        if(needed_title_image.get() != 0){needed_total_height += needed_title_image.get()->height();}
        if(needed_title_image.get() != 0 && needed_title_image.get()->width() > final_total_width){final_total_width = needed_title_image.get()->width();if(final_total_width-needed_total_width%2==1){final_total_width++;}}
        std::shared_ptr<scls::__Image_Base> to_return = std::make_shared<scls::__Image_Base>(final_total_width, needed_total_height, background_color);

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
        to_return.get()->fill_rect(x_start, y_start, left_border, to_return.get()->height(), border_color);
        to_return.get()->fill_rect(x_start, to_return.get()->height() - bottom_border, needed_total_width, bottom_border, border_color);
        to_return.get()->fill_rect((x_start + needed_total_width) - right_border, y_start, right_border, to_return.get()->height(), border_color);
        to_return.get()->fill_rect(x_start, y_start, needed_total_width, top_border, border_color);

        // Draw each columns
        for(int i = 0;i<static_cast<int>(a_cases.size());i++) {
            for(int j = 0;j<static_cast<int>(a_cases.at(i).size());j++) {
                Table::Table_Case* current_case = case_at(i, j);
                scls::__Image_Base* current_image = current_case->image.get()->get();
                if(current_case->merged != Table::Table_Case::Merge_State::MS_Merged){
                    // Handle merging
                    int current_width = column_width(i, current_case->merged_width);

                    // Set the background color
                    if(current_case->background_color() != background_color){
                        to_return.get()->fill_rect(needed_x[i], needed_y[j], current_width, needed_height[j], current_case->background_color());
                    }

                    // Paste the image
                    if(current_image != 0){
                        int current_x = needed_x[i] + (current_width / 2 - current_image->width() / 2);
                        int current_y = needed_y[j] + (needed_height[j] / 2 - current_image->height() / 2);
                        to_return.get()->paste(current_image, current_x, current_y);
                    }

                    // Draw the separation
                    scls::Color separation_color = scls::Color(0, 0, 0);
                    if(i > 0 && case_at(i - 1, j)->right_border){to_return.get()->fill_rect(needed_x[i] - column_separation(), needed_y[j], column_separation(), needed_height[j] + line_separation, separation_color);}
                    if(j > 0){to_return.get()->fill_rect(needed_x[i], needed_y[j] - line_separation, current_width + column_separation(), line_separation, separation_color);}
                }
            }

            // Missing cases
            for(int j = a_cases.at(i).size();j<static_cast<int>(needed_lines);j++) {
                case_at(i, j);

                // Draw the separation
                scls::Color separation_color = scls::Color(0, 0, 0);
                if(i > 0){to_return.get()->fill_rect(needed_x[i] - column_separation(), needed_y[j], column_separation(), needed_height[j] + line_separation, separation_color);}
                if(j > 0){to_return.get()->fill_rect(needed_x[i], needed_y[j] - line_separation, needed_width[i] + column_separation(), line_separation, separation_color);}
            }
        }

        // Paste the title
        if(needed_title_image.get() != 0){to_return.get()->paste(needed_title_image.get(), to_return.get()->width() / 2 - needed_title_image.get()->width() / 2, 0);}

        // Return the result
        return to_return;
    }
}
