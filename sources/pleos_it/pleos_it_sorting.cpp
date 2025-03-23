//******************
//
// pleos_it_sorting.cpp
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
// This file contains the source code of pleos_it_sorting.h.
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

// Include pleos_it_sorting.h
#include "../../pleos_it/pleos_it_sorting.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Do a comparaison part of the algorithm with the bubble algorithm
    void algorithms_comparaison_bubble(Sort_Datas* datas) {
        if(datas->algorithms_sort_comparaison_index < datas->values().size()) {
            int min_index = datas->algorithms_sort_comparaison_index;
            int i = datas->values().size() - 2;
            while(i >= min_index) {
                if(datas->values().at(i) > datas->values().at(i + 1)) {
                    double temp = datas->values().at(i);
                    datas->values()[i] = datas->values().at(i + 1);
                    datas->values()[i + 1] = temp;
                    datas->a_algorithms_sort_comparaison_permutation_number++;
                }
                i--;
                datas->a_algorithms_sort_comparaison_number++;
            }

            datas->algorithms_sort_comparaison_moved_index_max = min_index;
            datas->algorithms_sort_comparaison_moved_index_min = i;
            datas->algorithms_sort_comparaison_index++;
        }
    }

    // Do a division in an fusion sort algorithm
    void algorithms_comparaison_fusion_division(Sort_Datas* datas) {
        // Division
        if(datas->values_end == datas->values_start || datas->values_start + 1 == datas->values_end){return;}
        std::vector<double>::iterator middle = datas->values_start + (datas->values_end - datas->values_start) / 2;
        std::shared_ptr<Sort_Datas> sort_1 = std::make_shared<Sort_Datas>();
        sort_1.get()->values_start = datas->values_start;
        sort_1.get()->values_end = middle;
        datas->sub_sorts.push_back(sort_1);
        algorithms_comparaison_fusion_division(sort_1.get());
        std::shared_ptr<Sort_Datas> sort_2 = std::make_shared<Sort_Datas>();
        sort_2.get()->values_start = middle;
        sort_2.get()->values_end = datas->values_end;
        datas->sub_sorts.push_back(sort_2);
        algorithms_comparaison_fusion_division(sort_2.get());
    }

    void __algorithms_comparaison_fusion(Sort_Datas* datas) {
        // Sub sorts
        if(datas->sub_sorts.size() < 1){return;}
        else if(datas->sub_sorts.size() < 2){
            std::vector<double> buf_1 = datas->sub_sorts[0].get()->buffer(); int i_1 = 0;
            std::vector<double>::iterator current = datas->values_start;
            while(i_1 < buf_1.size()) {
                (*current) = buf_1[i_1];i_1++;
                current++;
                datas->a_algorithms_sort_comparaison_permutation_number++;
            }

            datas->good = true;
            datas->sub_sorts.clear();
            return;
        }

        // Fusion
        std::vector<double> buf_1 = datas->sub_sorts[0].get()->buffer(); int i_1 = 0;
        std::vector<double> buf_2 = datas->sub_sorts[1].get()->buffer(); int i_2 = 0;
        std::vector<double>::iterator current = datas->values_start;
        while(i_1 < buf_1.size() && i_2 < buf_2.size()) {
            if(buf_1[i_1] < buf_2[i_2]){(*current) = buf_1[i_1];i_1++;}
            else{(*current) = buf_2[i_2];i_2++;}
            current++;
            datas->a_algorithms_sort_comparaison_permutation_number++;
        }
        while(i_1 < buf_1.size()) {
            (*current) = buf_1[i_1];i_1++;
            current++;
            datas->a_algorithms_sort_comparaison_permutation_number++;
        }
        while(i_2 < buf_2.size()) {
            (*current) = buf_2[i_2];i_2++;
            current++;
            datas->a_algorithms_sort_comparaison_permutation_number++;
        }

        // End the algorithm
        datas->sub_sorts.clear();
        datas->good = true;
    }

    // Do a comparaison part of the algorithm with the fusion algorithm
    bool algorithms_comparaison_fusion(Sort_Datas* datas) {
        if(datas->good){return false;}
        if(datas->sub_sorts.size()<=0){algorithms_comparaison_fusion_division(datas);}

        if(datas->sub_sorts.size() > datas->current_sub_sort) {
            if(algorithms_comparaison_fusion(datas->sub_sorts[datas->current_sub_sort].get())){
                datas->current_sub_sort++;
            }
        }
        else {__algorithms_comparaison_fusion(datas);return true;}
        return false;
    }

    // Comparaison needed by some datas
    int algorithms_comparaison_fusion_time(Sort_Datas* datas) {return datas->values().size() * 2;};

    // Do a comparaison part of the algorithm with the insertion algorithm
    void algorithms_comparaison_insertion(Sort_Datas* datas) {
        if(datas->algorithms_sort_comparaison_index < datas->values().size()) {
            int current_index = datas->algorithms_sort_comparaison_index;
            int i = 0;
            while(i < datas->algorithms_sort_comparaison_index) {
                datas->a_algorithms_sort_comparaison_number++;
                if(datas->values()[i] > datas->values()[current_index]) {break;}
                i++;
            }

            // Permute the next variables
            int needed_index = i;
            while(i < datas->algorithms_sort_comparaison_index) {
                double temp = datas->values()[current_index];
                datas->values()[current_index] = datas->values()[i];
                datas->values()[i] = temp;
                i++;
                datas->a_algorithms_sort_comparaison_permutation_number++;
            }

            datas->algorithms_sort_comparaison_moved_index_max = needed_index;
            datas->algorithms_sort_comparaison_moved_index_min = current_index;
            datas->algorithms_sort_comparaison_index++;
        }
    }

    // Do a comparaison part of the algorithm with the selection algorithm
    void algorithms_comparaison_selection(Sort_Datas* datas) {
        if(datas->algorithms_sort_comparaison_index < datas->values().size()) {
            int min_index = datas->algorithms_sort_comparaison_index;
            int i = datas->algorithms_sort_comparaison_index + 1;
            while(i < datas->values().size()) {
                if(datas->values()[i] < datas->values()[min_index]) {
                    min_index = i;
                }
                i++;
                datas->a_algorithms_sort_comparaison_number++;
            }
            double temp = datas->values()[datas->algorithms_sort_comparaison_index];
            datas->values()[datas->algorithms_sort_comparaison_index] = datas->values()[min_index];
            datas->values()[min_index] = temp;
            datas->a_algorithms_sort_comparaison_permutation_number++;

            datas->algorithms_sort_comparaison_moved_index_max = min_index;
            datas->algorithms_sort_comparaison_moved_index_min = datas->algorithms_sort_comparaison_index;
            datas->algorithms_sort_comparaison_index++;
        }
    }

    // Creates and returns a new sorting datas
    std::shared_ptr<Sort_Datas> algorithms_sort_creation(int value_number) {
        // Create the sorting datas
        std::shared_ptr<Sort_Datas> to_return = std::make_shared<Sort_Datas>();
        to_return.get()->algorithms_sort_comparaison_index = 0;
        to_return.get()->a_algorithms_sort_comparaison_number = 0;
        to_return.get()->a_algorithms_sort_comparaison_permutation_number = 0;
        to_return.get()->algorithms_sort_comparaison_moved_index_max = -1;
        to_return.get()->algorithms_sort_comparaison_moved_index_min = -1;
        to_return.get()->sub_sorts.clear();

        // Values in the algorithm
        to_return.get()->values_shared_ptr = std::make_shared<std::vector<double>>(value_number, 0);
        std::vector<double>& values = to_return.get()->values();
        for(int i = 0;i<value_number;i++){values[i] = i + 1;}

        // Shuffle the list
        std::random_shuffle(values.begin(), values.end());
        to_return.get()->values_start = values.begin();
        to_return.get()->values_end = values.end();

        // Return the result
        return to_return;
    }

    // Returns an image with the sort in it
    std::shared_ptr<scls::Image> algorithms_sort_image(std::shared_ptr<scls::Image> base_image, Sort_Datas* datas, int x, int y, int width, int height) {
        // Draw the values
        int current_x = x;
        double maximum_height = height;
        int moved_index_max = datas->algorithms_sort_comparaison_moved_index_max;
        int moved_index_min = datas->algorithms_sort_comparaison_moved_index_min;
        std::vector<double> values = datas->buffer();
        double value_number = values.size();
        std::vector<long long> needed_width = scls::partition_number(width, value_number);
        for(int i = 0;i<static_cast<int>(values.size());i++) {
            double proportion = (values[i] / value_number);
            proportion *= 2.0;proportion -= 1;
            proportion = std::sqrt(1 - proportion * proportion);
            int needed_height = round(proportion * maximum_height);
            int part_width = needed_width[i];

            // Set the color
            //scls::Color current_color = scls::Color(255, 255 - 255.0 * proportion, 255 - 255.0 * proportion);
            scls::Color current_color = scls::Color(255, 0, 0);
            if(i == moved_index_max){current_color = scls::Color(255, 128, 0);}
            else if(i == moved_index_min){current_color = scls::Color(204, 204, 0);}

            // Draw the line
            int current_y = y + (maximum_height / 2 - needed_height / 2);
            //int current_y = y + proportion * maximum_height;
            base_image.get()->fill_rect(current_x, current_y, part_width, needed_height, current_color);
            current_x += part_width;
        }

        // Returns the result
        return base_image;
    }
    std::shared_ptr<scls::Image> algorithms_sort_image(Sort_Datas* datas, int image_width){
        // Return the image with a new image
        scls::Color background_color = scls::Color(255, 255, 255);
        int needed_width = image_width;
        std::shared_ptr<scls::Image> image = std::make_shared<scls::Image>(needed_width, needed_width, background_color);
        return algorithms_sort_image(image, datas, 0, 0, needed_width, needed_width);
    }
}
