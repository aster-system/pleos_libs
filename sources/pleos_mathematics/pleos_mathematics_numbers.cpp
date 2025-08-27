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
    // Analysed numbers
    std::shared_ptr<std::vector<std::shared_ptr<Number>>> numbers;
    std::shared_ptr<std::vector<std::shared_ptr<Number>>> prime_numbers;
    // Create the first elements of the list
    void __first_numbers(){
        // Create the first elements of the list
        numbers = std::make_shared<std::vector<std::shared_ptr<Number>>>(10);
        for(int i = 0;i<static_cast<int>(10);i++) {
            (*numbers.get())[i] = std::make_shared<Number>();
            (*numbers.get())[i].get()->number=i;
        }

        // Set the prime number
        prime_numbers = std::make_shared<std::vector<std::shared_ptr<Number>>>(4);
        (*numbers.get())[2].get()->a_is_prime = 1;(*prime_numbers.get())[0] = ((*numbers.get())[2]);
        (*numbers.get())[3].get()->a_is_prime = 1;(*prime_numbers.get())[1] = ((*numbers.get())[3]);
        (*numbers.get())[5].get()->a_is_prime = 1;(*prime_numbers.get())[2] = ((*numbers.get())[5]);
        (*numbers.get())[7].get()->a_is_prime = 1;(*prime_numbers.get())[3] = ((*numbers.get())[7]);
    };
    // Update the list to a precise position
    void __update_numbers(long long position) {
        if(numbers.get() == 0){__first_numbers();}
        if(static_cast<long long>(numbers.get()->size()) > position){return;}
        std::shared_ptr<std::vector<std::shared_ptr<Number>>>temp=numbers;
        numbers = std::make_shared<std::vector<std::shared_ptr<Number>>>(position + 1);
        if(temp.get() != 0) {for(int i = 0;i<static_cast<int>(temp.get()->size());i++){(*numbers.get())[i] = (*temp.get())[i];}}
    }

    // Check if the number is prime with existing values
    bool __number_is_prime_existing(long long tested_number) {
        // Check
        if(prime_numbers.get() == 0){__first_numbers();}
        long long limit = tested_number / 2;long long i = 0;
        while(i < static_cast<long long>(prime_numbers.get()->size()) && (*prime_numbers.get())[i].get()->number < limit) {
            if(tested_number % (*prime_numbers.get())[i].get()->number == 0){return false;}
            i++;
        }
        return true;
    }
    // Get the next prime number
    void __next_prime(){
        if(prime_numbers.get() == 0){__first_numbers();}
        long long to_add = 0;
        for(int i = (*prime_numbers.get())[prime_numbers.get()->size() - 1].get()->number + 2;i>0;i+=2) {if(__number_is_prime_existing(i)){to_add = i;break;}}
        __update_numbers(to_add);
        // Create the number if needed
        if((*numbers.get())[to_add].get() == 0){
            (*numbers.get())[to_add] = std::make_shared<Number>();
            (*numbers.get())[to_add].get()->number=to_add;
        }
        (*numbers.get())[to_add].get()->a_is_prime = 1;
        prime_numbers.get()->push_back((*numbers.get())[to_add]);
    }
    // Check if an analysed number is prime or not
    void __number_is_prime(std::shared_ptr<Number> tested_number) {
        // Check
        long long limit = tested_number.get()->number / 2;long long i = 0;
        tested_number.get()->a_is_prime = 1;
        while((*prime_numbers.get())[i].get()->number < limit) {
            if(tested_number.get()->number % (*prime_numbers.get())[i].get()->number == 0){tested_number.get()->a_is_prime = 0;break;}
            i++;

            // Get the next prime number
            if(static_cast<long long>(prime_numbers.get()->size()) <= i){__next_prime();}
        }
    }

    // Generates prime number until a certain number
    void generate_prime_number(long long limit) {while(static_cast<long long>(prime_numbers.get()->size()) <= limit){__next_prime();}}
    void generate_prime_number_until(long long limit) {while((*prime_numbers.get())[prime_numbers.get()->size() - 1].get()->number <= limit){__next_prime();}}
    // Returns an analysed number
    std::shared_ptr<Number> number_shared_ptr(long long position){
        // Update the list if needed
        __update_numbers(position);

        // Create the number if needed
        if((*numbers.get())[position].get() == 0 || (*numbers.get())[position].get()->number != position){
            (*numbers.get())[position] = std::make_shared<Number>();
            (*numbers.get())[position].get()->number=position;
            __number_is_prime((*numbers.get())[position]);
        }

        // Returns the needed element
        return (*numbers.get())[position];
    };
    Number* number(long long position){return number_shared_ptr(position).get();}
    // Returns an analysed prime number
    std::shared_ptr<Number> prime_number_shared_ptr(long long position){generate_prime_number(position);return (*prime_numbers.get())[position];}
    Number* prime_number(long long position){return prime_number_shared_ptr(position).get();}

    // Returns a division circle
    // This function is inspired by this (french) video from Mickael Launay : https://youtu.be/-X49VQgi86E?si=wvdvNiM0ZBgUUii4.
    std::shared_ptr<scls::__Image_Base> division_circle(int image_width, int circle_radius, double modulo, int point_number) {
        std::shared_ptr<scls::__Image_Base> to_return = std::make_shared<scls::__Image_Base>(image_width, image_width, scls::Color(255, 255, 255));
        to_return.get()->draw_circle(image_width / 2, image_width / 2, circle_radius, 0, 0, 0, 255, 10);

        // Draw the text
        scls::Text_Image_Generator generator;
        scls::Text_Style style; style.set_font_size(24);
        std::shared_ptr<scls::__Image_Base> needed_text = generator.image_shared_ptr("Modulo : " + scls::format_number_to_text(modulo) + "</br>Points : " + std::to_string(point_number), style);

        // Draw the lines
        scls::Color point_color(0, 0, 0);
        std::shared_ptr<scls::model_maker::Polygon> points = scls::model_maker::regular_polygon_points(point_number);
        for(int i = 0;i<static_cast<int>(points.get()->points.size());i++) {
            int current_x = static_cast<double>(image_width) / 2.0 + points.get()->points[i].x() * static_cast<double>(circle_radius * 2);
            int current_y = static_cast<double>(image_width) / 2.0 + points.get()->points[i].z() * static_cast<double>(circle_radius * 2);

            int final_modulo = static_cast<int>(i * modulo) % point_number;
            int final_x = static_cast<double>(image_width) / 2.0 + points.get()->points[final_modulo].x() * static_cast<double>(circle_radius * 2);
            int final_y = static_cast<double>(image_width) / 2.0 + points.get()->points[final_modulo].z() * static_cast<double>(circle_radius * 2);

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

    // Calculate the decomposition of a number
    void arithmetic_decomposition(Arithmetic_Object* object, std::string* redaction) {
        long long number_value = object->value_1.value_to_double(0);
        const long long number_value_start = number_value;
        if(redaction != 0 && false){(*redaction) += std::string("Nous cherchons la décomposition en produit de facteurs premiers de ") + std::to_string(number_value) + std::string(". ");}

        std::vector<std::shared_ptr<Number>> decomposition;
        std::vector<std::shared_ptr<Number>> needed_numbers;
        // Decompose the number
        std::shared_ptr<Number> needed_number = number_shared_ptr(number_value);
        if(needed_number.get()->is_prime()) {decomposition.push_back(needed_number);}
        else {
            int current_divisor = 0;
            while(number_value > 1) {
                if(number_value % prime_number(current_divisor)->number == 0) {
                    decomposition.push_back(prime_number_shared_ptr(current_divisor));
                    number_value /= prime_number(current_divisor)->number;
                }
                else{current_divisor++;}
            }
        }

        // Set the result
        if(redaction != 0){
            (*redaction) += std::string("La décomposition en produit de facteurs premiers de ") + std::to_string(number_value_start) + std::string(" est ");
            (*redaction) += std::to_string(decomposition[0].get()->number);
            for(int i = 1;i<static_cast<int>(decomposition.size());i++) {
                (*redaction) += std::string(" * ") + std::to_string(decomposition[i].get()->number);
            }
            (*redaction) += std::string(".");
        }
    }

    // Returns an erathostene sieve
    scls::Image erathostene_sieve(int image_width, int start_number, int end_number) {
        // Create a table
        int line_width = 5;int line_number = 1 + (end_number - start_number) / line_width;
        std::shared_ptr<__Table_Case> sieve = __Table_Case::new_table();
        scls::Text_Image_Generator tig;scls::Text_Style needed_style;

        int current_number = start_number;
        int current_x = 0;int current_y = 0;
        for(int i = 0;i<line_number;i++) {
            for(;current_x<line_width;current_x++) {
                __Table_Case::Table_Case* current_case = sieve.get()->case_at(current_x, current_y);
                if(number(current_number)->is_prime()) {
                    current_case->set_background_color(scls::Color(255, 200, 200));
                    needed_style.set_background_color(scls::Color(255, 200, 200));
                }
                else{needed_style.set_background_color(scls::Color(255, 255, 255));}
                current_case->set_image(tig.image_shared_ptr(std::to_string(current_number), needed_style));
                current_number++;
            }
            current_x=0;current_y++;
        }

        // Returns the result
        return sieve.get()->to_image();
    }
}

