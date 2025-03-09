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
        scls::Text_Style style; style.set_font_size(24);
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

    // Calculate the GCD of two numbers
    long long arithmetic_gcd(Arithmetic_Object* object, std::string* redaction) {
        // Add the start of the redaction
        long long first = object->value_1.value_to_double(0);
        long long second = object->value_2.value_to_double(0);
        if(second > first){long long temp = first;first = second;second = temp;}
        const long long start_first = first;
        const long long start_second = second;
        if(redaction != 0){
            (*redaction) += std::string("Nous cherchons le plus grand commun diviseur de ") + std::to_string(first) + std::string(" et ") + std::to_string(second) + std::string(". ");
            (*redaction) += std::string("Pour cela, utilisons l'algorithme d'Euclide. </br></br>");
        }

        // Do the algorithm
        object->euclid_algorithm_value.push_back(first);
        object->euclid_algorithm_value.push_back(second);
        while(first % second != 0) {
            // Get the rest
            long long first_temp = first;
            long long second_temp = second;
            second = first % second;
            first = second_temp;
            object->euclid_algorithm_value.push_back(second);

            // Add the start of the redaction
            if(redaction != 0) {
                (*redaction) += std::string("Nous cherchons le reste de ") + std::to_string(first_temp) + std::string(" et ") + std::to_string(first) + std::string(".</br>");
                (*redaction) += std::to_string(first_temp) + std::string(" % ") + std::to_string(first) + std::string(" = ") + std::to_string(second) + std::string("</br>");
                if(first % second != 0) {
                    (*redaction) += std::string("Le reste n'est pas nul, nous continuons donc l'algorithme.</br>");
                }
            }
        }

        // Finish the redaction
        if(redaction != 0) {
            (*redaction) += std::string("Le plus grand commun diviseur de ") + std::to_string(start_first) + std::string(" et ") + std::to_string(start_second) + std::string(" est ") + std::to_string(second) + std::string(". ");
            (*redaction) += std::string("En effet, ") + std::to_string(start_first) + std::string(" / ") + std::to_string(second) + std::string(" = ") + std::to_string(start_first / second) + std::string(" et ") + std::to_string(start_second) + std::string(" / ") + std::to_string(second) + std::string(" = ") + std::to_string(start_second / second) + std::string(". ");
        }

        return second;
    };

    // Calculate the Bezout identity of two numbers
    void arithmetic_bezout_identity(Arithmetic_Object* object, std::string* redaction) {
        // Add the start of the redaction
        long long first = object->value_1.value_to_double(0);
        long long second = object->value_2.value_to_double(0);
        if(second > first){long long temp = first;first = second;second = temp;}
        const long long start_first = first;
        const long long start_second = second;
        if(redaction != 0){
            (*redaction) += std::string("Nous cherchons l'identité de Bézout de ") + std::to_string(first) + std::string(" et ") + std::to_string(second) + std::string(". ");
            (*redaction) += std::string("Pour cela, utilisons les résultats de l'algorithme d'Euclide. </br></br>");
        }

        // Do the algorithm
        std::vector<int> combination_values = std::vector<int>();
        std::vector<int>& values = object->euclid_algorithm_value;
        for(int i = 0;i<static_cast<int>(values.size() - 2);i++) {
            // Get the rest
            long long x = values[i];
            long long y = values[i + 1];
            long long c = -(x - values[i + 2]) / y;
            // Do the good calculation
            // A
            long long current_a = 1;
            long long a = current_a;
            if(i > 1) {a = combination_values[combination_values.size() - 4] + combination_values[combination_values.size() - 2] * c;}
            else if(i > 0) {a = combination_values[combination_values.size() - 2] * c;}
            // B
            long long current_b = c;
            long long b = current_b;
            if(i > 1) {b = combination_values[combination_values.size() - 3] + combination_values[combination_values.size() - 1] * c;}
            else if(i > 0) {b = combination_values[combination_values.size() - 1] * c + current_a;}
            combination_values.push_back(a);
            combination_values.push_back(b);

            // Add the start of the redaction
            if(redaction != 0) {
                (*redaction) += std::string("Nous cherchons la combinaison linéaire de ") + std::to_string(x) + std::string(" et ") + std::to_string(y) + std::string(".</br>");
                (*redaction) += std::to_string(x) + std::string(" * ") + std::to_string(current_a) + std::string(" + ") + std::to_string(y) + std::string(" * ") + std::to_string(current_b) + std::string(" = ") + std::to_string(values[i + 2]) + std::string("</br>");
                if(i > 0) {
                    (*redaction) += std::to_string(start_first) + std::string(" * ") + std::to_string(a) + std::string(" + ") + std::to_string(start_second) + std::string(" * ") + std::to_string(b) + std::string(" = ") + std::to_string(values[i + 2]) + std::string("</br>");
                }
            }
        }

        // Finish the redaction
        long long a = combination_values[combination_values.size() - 2];
        long long b = combination_values[combination_values.size() - 1];
        long long gcd = values[values.size() - 1];
        if(redaction != 0) {
            (*redaction) += std::string("L'identité de Bézout de ") + std::to_string(start_first) + std::string(" et ") + std::to_string(start_second) + std::string(" est ");
            (*redaction) += std::to_string(start_first) + std::string(" * ") + std::to_string(a) + std::string(" + ") + std::to_string(start_second) + std::string(" * ") + std::to_string(b) + std::string(" = ") + std::to_string(gcd) + std::string(". ");
        }
    }
}
