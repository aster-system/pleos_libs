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
    class Stack {
        // Class representating a stack
    public:

        // Stack constructor
        Stack(){};

        // Returns the stack in an image
        std::shared_ptr<scls::Image> image() {
            // Get each needed images for elements
            int border_width = 4; int inline_border_width = 2;
            scls::Text_Style current_style; current_style.font_size = 70;
            std::vector<std::shared_ptr<scls::Image>> elements_image;
            scls::Text_Image_Generator generator;
            int max_width = 0;
            int total_height = border_width;
            for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                std::shared_ptr<scls::Image> to_add = generator.image_shared_ptr(a_elements[i].to_std_string(), current_style);
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
        inline void pile_up(E element){a_elements.push_back(element);};
        // Removes an element
        inline E unstack() {E to_return = a_elements[a_elements.size() - 1];a_elements.erase(a_elements.end() - 1);return to_return;}

    private:
        // Elements in the stack
        std::vector<E> a_elements;
    };
}

#endif // PLEOS_IT_DATA_STRUCTURE
