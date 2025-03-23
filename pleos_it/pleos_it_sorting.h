//******************
//
// pleos_it_sorting.h
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
// This file contains the some animations about sorting algorithms.
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

#ifndef PLEOS_IT_SORTING
#define PLEOS_IT_SORTING

// Include SCLS Graphic Benoit
#include "../../../scls-graphic-benoit/scls_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Datas for a sorting algorithm
    struct Sort_Datas {
        // Current index of the algorithm
        int algorithms_sort_comparaison_index = 0;
        // Moved index of the comparaison
        int algorithms_sort_comparaison_moved_index_max = -1;
        int algorithms_sort_comparaison_moved_index_min = -1;
        // Number of comparaisons
        int a_algorithms_sort_comparaison_number = 0;
        inline int algorithms_sort_comparaison_number(){int to_return=a_algorithms_sort_comparaison_number;for(int i=0;i<static_cast<int>(sub_sorts.size());i++){to_return += sub_sorts.at(i).get()->algorithms_sort_comparaison_number();}return to_return;};;
        // Number of permutations
        int a_algorithms_sort_comparaison_permutation_number = 0;
        inline int algorithms_sort_comparaison_permutation_number(){int to_return=a_algorithms_sort_comparaison_permutation_number;for(int i=0;i<static_cast<int>(sub_sorts.size());i++){to_return += sub_sorts.at(i).get()->algorithms_sort_comparaison_permutation_number();}return to_return;};

        // If the list is usable or not
        bool good = false;
        // Sub sorts for this short
        int current_sub_sort = 0;
        std::vector<std::shared_ptr<Sort_Datas>> sub_sorts;
        // Values for the sorting
        std::shared_ptr<std::vector<double>> values_shared_ptr;
        std::vector<double>::iterator values_end;
        std::vector<double>::iterator values_start;

        // Returns the datas
        inline std::vector<double>& values(){return *values_shared_ptr.get();};
        // Returns the buffer of the datats
        inline std::vector<double> buffer(){std::vector<double> to_return;std::vector<double>::iterator current = values_start;while(current!=values_end){to_return.push_back(*current);current++;}return to_return;};
    };

    // Do a comparaison part of the algorithm with the bubble algorithm
    void algorithms_comparaison_bubble(Sort_Datas* datas);

    // Does a division in an fusion sort algorithm
    void algorithms_comparaison_fusion_division(Sort_Datas* datas);
    void __algorithms_comparaison_fusion(Sort_Datas* datas);
    // Do a comparaison part of the algorithm with the fusion algorithm
    bool algorithms_comparaison_fusion(Sort_Datas* datas);
    // Comparaison needed by some datas
    int algorithms_comparaison_fusion_time(Sort_Datas* datas);

    // Do a comparaison part of the algorithm with the insertion algorithm
    void algorithms_comparaison_insertion(Sort_Datas* datas);

    // Do a comparaison part of the algorithm with the selection algorithm
    void algorithms_comparaison_selection(Sort_Datas* datas);

    // Creates and returns a new sorting datas
    std::shared_ptr<Sort_Datas> algorithms_sort_creation(int value_number);

    // Returns an image with the sort in it
    std::shared_ptr<scls::Image> algorithms_sort_image(std::shared_ptr<scls::Image> base_image, Sort_Datas* datas, int x, int y, int width, int height);
    std::shared_ptr<scls::Image> algorithms_sort_image(Sort_Datas* datas, int image_width);
}

#endif PLEOS_IT_SORTING
