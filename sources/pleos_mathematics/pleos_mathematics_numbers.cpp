//******************
//
// pleos_mathematics_numbers.cpp
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
// This file contains the source code of pleos_mathematics_numbers.cpp.
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

// Include pleos_mathematics_numbers.h
#include "../../pleos_mathematics/pleos_mathematics_numbers.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Returns a division circle
    // This function is inspired by this (french) video from Mickael Launay : https://youtu.be/-X49VQgi86E?si=wvdvNiM0ZBgUUii4.
    std::shared_ptr<scls::Image> division_circle(int image_width, int circle_radius, double modulo, int point_number) {
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(image_width, image_width, scls::Color(255, 255, 255));
        to_return.get()->draw_circle(image_width / 2, image_width / 2, circle_radius, 0, 0, 0, 255, 10);

        // Draw the text
        scls::Text_Image_Generator generator;
        scls::Text_Style style; style.font_size = 24;
        std::shared_ptr<scls::Image> needed_text = generator.image_shared_ptr("Modulo : " + scls::format_number_to_text(modulo) + "</br>Points : " + std::to_string(point_number), style);

        // Draw the lines
        scls::Color point_color(0, 0, 0);
        std::shared_ptr<scls::model_maker::Polygon> points = scls::model_maker::regular_polygon_points(point_number);
        for(int i = 0;i<static_cast<int>(points.get()->points.size());i++) {
            int current_x = static_cast<double>(image_width) / 2.0 + points.get()->points[i].x() * static_cast<double>(circle_radius * 2);
            int current_y = static_cast<double>(image_width) / 2.0 + points.get()->points[i].z() * static_cast<double>(circle_radius * 2);

            int final_modulo = static_cast<int>(i * modulo) % point_number;
            int final_x = static_cast<double>(image_width) / 2.0 + points.get()->points[final_modulo].x() * static_cast<double>(circle_radius * 2);
            int final_y = static_cast<double>(image_width) / 2.0 + points.get()->points[final_modulo].z() * static_cast<double>(circle_radius * 2);

            double distance = std::sqrt(std::pow(final_x - current_x, 2) + std::pow(final_y - current_y, 2));
            double ratio_distance = distance / static_cast<double>(circle_radius * 2);
            to_return.get()->fill_rect(current_x, current_y, 6, 6, point_color);
            //scls::Color final_color = line_colors[static_cast<int>(round(ratio_distance * lines_colors_size))];
            //scls::Color final_color = scls::Color(255 * ratio_distance, 0, 255 * (1.0 - ratio_distance));
            scls::Color final_color = scls::Color(0, 0, 0);
            to_return.get()->draw_line(current_x + 3, current_y + 3, final_x, final_y, final_color, 1);
        }

        // Paste the text
        to_return.get()->paste(needed_text.get(), 10, to_return.get()->height() - needed_text.get()->height());

        return to_return;
    }
}
