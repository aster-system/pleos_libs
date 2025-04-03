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
// This file contains the core of the table part for PLEOS.
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

#ifndef PLEOS_TABLE
#define PLEOS_TABLE

// Include SCLS Graphic Benoit
#include "../../scls-graphic-benoit/scls_graphic.h"
#include "pleos_mathematics/pleos_mathematics_functions.h"
#include "pleos_it.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //*********
	//
	// PLEOS Table handler
	//
	//*********

	class Table {
        // Class representating a more table handler for PLEOS
    public:

        // Cases in the table
        struct Table_Case {
            // Image in the case
            std::shared_ptr<std::shared_ptr<scls::Image>> image;
            // Margin of the case
            int margin = 20;
            // If the case is merged or not
            bool merged = false;
            // Style of the case
            scls::Text_Style style;

            // Height of the case
            inline int height() const {return image.get()->get()->height();};
            // Width of the case
            inline int width() const {return image.get()->get()->width() + margin * 2;};

            // Getters and setters
            inline scls::Color background_color() const {return style.background_color();};
            inline void set_background_color(scls::Color new_color) {style.set_background_color(new_color);};
        };

        // Table constructor
        Table(){};

        // Returns the case at a certain position
        Table_Case* case_at(int x, int y);
        inline std::shared_ptr<scls::Image>& image_at(int x, int y){return *(case_at(x, y)->image.get());};
        // Returns the number of column in the table
        inline int column_number() const{return a_cases.size();};
        // Returns the width of a column
        int column_width(int column) const;
        // Returns the number of lines in the table
        int line_number() const;
        // Returns the height of a line
        int line_height(int line) const;
        // Returns the total height / width of the image
        inline int total_height() const {int to_return = 0;int needed_width = line_number();for(int i = 0;i<static_cast<int>(needed_width);i++){to_return += line_height(i);};return to_return;};
        inline int total_width() const {int to_return = 0;int needed_width = column_number();for(int i = 0;i<static_cast<int>(needed_width);i++){to_return += column_width(i);};return to_return;};

        // Merges cases
        void merge_cases(int x, int y, int width, int height){
            for(int i = 0;i<static_cast<int>(width);i++){
                for(int j = 0;j<static_cast<int>(height);j++){
                    if(!(i == 0 && j == 0)){
                        case_at(x + i, y + j);
                        a_cases[x + i][y + j].get()->image = a_cases[x][y].get()->image;
                    }
                }
            }
        };

        // Set the value of an std::string case
        void set_case_value(int x, int y, std::string value, scls::Text_Style* needed_style, scls::Text_Image_Generator* tig){(*case_at(x, y)->image.get()) = tig->image_shared_ptr(value, *needed_style);};

        // Returns the table to an image
        std::shared_ptr<scls::Image> to_image();

    private:
        // Cases in the table ([x][y])
        std::vector<std::vector<std::shared_ptr<Table_Case>>> a_cases;
    };
}

#endif //PLEOS_TABLE
