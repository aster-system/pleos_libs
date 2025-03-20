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

    // Returns the width of a column
    int Table::column_width(int column) const{
        int to_return = 0;
        for(int i = 0;i<static_cast<int>(a_cases.at(column).size());i++){
            if(a_cases.at(column).at(i).get() != 0 && a_cases.at(column).at(i).get()->width() > to_return){
                to_return = a_cases.at(column).at(i).get()->width();
            }
        }
        return to_return;
    };

    // Returns the height of a line
    int Table::line_height(int line) const {
        int to_return = 0;
        for(int i = 0;i<static_cast<int>(a_cases.size());i++){
            if(a_cases.at(i).size() > line && a_cases.at(i)[line].get() != 0){
                if(a_cases.at(i).at(line).get()->height() > to_return){
                    to_return = a_cases.at(i).at(line).get()->height();
                }
            }
        }
        return to_return;
    }

    // Returns the number of lines in the table
    int Table::line_number() const {
        int to_return = 0;
        for(int i = 0;i<static_cast<int>(a_cases.size());i++){
            if(a_cases.at(i).size() > to_return){
                to_return = a_cases.at(i).size();
            }
        }
        return to_return;
    }

    // Returns the case at a certain position
    Table::Table_Case* Table::case_at(int x, int y){
        while(a_cases.size() <= x){a_cases.push_back(std::vector<std::shared_ptr<Table::Table_Case>>());}
        while(a_cases[x].size() <= y){a_cases[x].push_back(std::shared_ptr<Table::Table_Case>());}
        if(a_cases[x][y].get()==0){a_cases[x][y]=std::make_shared<Table::Table_Case>();}
        return a_cases[x][y].get();
    };

	// Returns the table to an image
    std::shared_ptr<scls::Image> Table::to_image() {
        // Get the needed datas
        int bottom_border = 2;
        int column_separation = 2;
        int left_border = 2;
        int line_separation = 2;
        int needed_columns = column_number();
        int needed_lines = line_number();
        int right_border = 2;
        int top_border = 2;

        // Create the image
        scls::Color background_color = scls::Color(255, 255, 255);
        int needed_total_height = total_height() + (line_separation * (needed_lines - 1)) + top_border + bottom_border;
        int needed_total_width = total_width() + (column_separation * (needed_columns - 1)) + left_border + right_border;
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(needed_total_width, needed_total_height, background_color);

        // Get the datas for the drawing
        // X datas
        std::vector<int> needed_width = std::vector<int>(needed_columns, 0);
        std::vector<int> needed_x = std::vector<int>(needed_columns, left_border);
        for(int i = 0;i<a_cases.size();i++){needed_width[i] = column_width(i);if(i > 0){needed_x[i] = needed_x[i - 1] + needed_width[i - 1] + column_separation;}}
        // Y datas
        std::vector<int> needed_height = std::vector<int>(needed_lines, 0);
        std::vector<int> needed_y = std::vector<int>(needed_lines, top_border);
        for(int i = 0;i<needed_lines;i++){needed_height[i] = line_height(i);if(i > 0){needed_y[i] = needed_y[i - 1] + needed_height[i - 1] + line_separation;}}

        // Draw the border
        scls::Color border_color = scls::Color(0, 0, 0);
        to_return.get()->fill_rect(0, 0, left_border, to_return.get()->height(), border_color);
        to_return.get()->fill_rect(0, to_return.get()->height() - bottom_border, to_return.get()->width(), bottom_border, border_color);
        to_return.get()->fill_rect(to_return.get()->width() - right_border, 0, right_border, to_return.get()->height(), border_color);
        to_return.get()->fill_rect(0, 0, to_return.get()->width(), top_border, border_color);

        // Draw each columns
        for(int i = 0;i<static_cast<int>(a_cases.size());i++) {
            for(int j = 0;j<static_cast<int>(a_cases.at(i).size());j++) {
                Table::Table_Case* current_case = case_at(i, j);
                scls::Image* current_image = current_case->image.get();

                // Set the background color
                if(current_case->background_color() != background_color){
                    to_return.get()->fill_rect(needed_x[i], needed_y[j], needed_width[i], needed_height[j], current_case->background_color());
                }

                // Paste the image
                if(current_image != 0){
                    int current_x = needed_x[i] + (needed_width[i] / 2 - current_image->width() / 2);
                    int current_y = needed_y[j] + (needed_height[j] / 2 - current_image->height() / 2);
                    to_return.get()->paste(current_image, current_x, current_y);
                }

                // Draw the separation
                scls::Color separation_color = scls::Color(0, 0, 0);
                if(i > 0){to_return.get()->fill_rect(needed_x[i] - column_separation, needed_y[j], column_separation, needed_height[j] + line_separation, separation_color);}
                if(j > 0){to_return.get()->fill_rect(needed_x[i], needed_y[j] - line_separation, needed_width[i] + column_separation, line_separation, separation_color);}
            }

            // Missing cases
            for(int j = a_cases.at(i).size();j<static_cast<int>(needed_lines);j++) {
                Table::Table_Case* current_case = case_at(i, j);

                // Draw the separation
                scls::Color separation_color = scls::Color(0, 0, 0);
                if(i > 0){to_return.get()->fill_rect(needed_x[i] - column_separation, needed_y[j], column_separation, needed_height[j] + line_separation, separation_color);}
                if(j > 0){to_return.get()->fill_rect(needed_x[i], needed_y[j] - line_separation, needed_width[i] + column_separation, line_separation, separation_color);}
            }
        }

        // Return the result
        return to_return;
    }
}
