//******************
//
// pleos_mathematics_matrices.h
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
// This file contains the source code of pleos_mathematics_matrices.cpp.
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

// Include pleos_mathematics_probabilities.h
#include "../../pleos_mathematics/pleos_mathematics_matrices.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Converts a fraction matrice to an image
    std::shared_ptr<scls::__Image_Base> matrice_to_image(Matrice<scls::Fraction> matrice, scls::Text_Image_Generator* generator, scls::Text_Style style) {
        // Asserts
        if(matrice.dimension().dimension_number() > 2){return std::shared_ptr<scls::__Image_Base>();}

        if(matrice.dimension().dimension_number() == 1) {
            // For a one dimension matrice
            std::vector<std::shared_ptr<scls::__Image_Base>> images = std::vector<std::shared_ptr<scls::__Image_Base>>();
            // Create each images
            int max_height = 0;
            int number = matrice.dimension();
            int total_width = 0;
            for(int i = 0;i<number;i++) {
                // Get each images
                std::shared_ptr<scls::__Image_Base> current_image = generator->image_shared_ptr(matrice.at(i).to_mathml(), style);
                images.push_back(current_image);
                if(max_height < current_image.get()->height()){max_height = current_image.get()->height();}
                total_width += current_image.get()->width();
            }

            // Include the separation
            int image_end_size = 40; scls::Text_Style style_end = style; style_end.set_font_size(image_end_size);
            scls::Text_Style style_start = style; style_start.set_font_size(image_end_size);
            std::shared_ptr<scls::__Image_Base> image_start = generator->image_shared_ptr("(", style_start);
            std::shared_ptr<scls::__Image_Base> image_end = generator->image_shared_ptr(")", style_end);
            int separation_width = style.font_size();
            total_width += (static_cast<int>(images.size()) - 1) * separation_width + image_start.get()->width() + image_end.get()->width();
            if(image_end.get()->height() > max_height){max_height = image_end.get()->height();}
            if(image_start.get()->height() > max_height){max_height = image_start.get()->height();}

            // Create the final image
            int current_x = 0; int current_y = 0;
            std::shared_ptr<scls::__Image_Base> final_image = std::make_shared<scls::__Image_Base>(total_width, max_height, style.background_color());
            final_image.get()->paste(image_start.get(), current_x, current_y);
            current_x += image_start.get()->width();
            // Draw each fractions
            for(int i = 0;i<static_cast<int>(images.size());i++){
                current_y = 0;
                final_image.get()->paste(images[i].get(), current_x, current_y);
                current_x += images[i].get()->width();
                // Draw the separation
                if(i < static_cast<int>(images.size()) - 1) {
                    current_x += separation_width;
                }
            }
            final_image.get()->paste(image_end.get(), current_x, current_y);
            return final_image;
        }
        else {
            // For a two dimension matrice
            std::vector<std::vector<std::shared_ptr<scls::__Image_Base>>> images = std::vector<std::vector<std::shared_ptr<scls::__Image_Base>>>();
            // Create each images
            int number = matrice.dimension().first_dimension();
            int number_dimensions = matrice.dimension().last_dimension();
            std::vector<int> max_height = std::vector<int>(number);
            std::vector<int> max_width = std::vector<int>(number_dimensions);
            for(int i = 0;i<number;i++) {
                // Get each sub-matrices
                std::vector<std::shared_ptr<scls::__Image_Base>> current_matrice;
                for(int j = 0;j<number_dimensions;j++) {
                    // Get each images
                    int x = j; int y = i;
                    std::shared_ptr<scls::__Image_Base> current_image = generator->image_shared_ptr(matrice.matrice_at(x)->at(y), style);
                    if(max_height[y] < current_image.get()->height()){max_height[y] = current_image.get()->height();}
                    if(max_width[x] < current_image.get()->width()){max_width[x] = current_image.get()->width();}
                    current_matrice.push_back(current_image);
                }
                images.push_back(current_matrice);
            }

            // Create the formating
            int separation_width = style.font_size();

            // Create the final image
            int total_height = 0; for(int i = 0;i<static_cast<int>(max_height.size());i++){total_height+=max_height[i];}
            int total_width = 0; for(int i = 0;i<static_cast<int>(max_width.size());i++){total_width+=max_width[i];}
            total_width += (static_cast<int>(max_width.size()) - 1) * separation_width;
            int current_x = 0; int current_y = 0;
            std::shared_ptr<scls::__Image_Base> final_image = std::make_shared<scls::__Image_Base>(total_width, total_height, style.background_color());
            //final_image.get()->paste(image_start.get(), current_x, current_y);
            //current_x += image_start.get()->width();
            // Draw each fractions
            for(int i = 0;i<static_cast<int>(images.size());i++){
                for(int j = 0;j<static_cast<int>(images[i].size());j++){
                    int needed_x = current_x + ((max_width[j] / 2) - (images[i][j].get()->width() / 2));
                    final_image.get()->paste(images[i][j].get(), needed_x, current_y);
                    current_x += max_width[j];
                    // Draw the separation
                    if(j < static_cast<int>(images.size()) - 1) {
                        current_x += separation_width;
                    }
                }
                current_x = 0;
                current_y += max_height[i];
            }
            //final_image.get()->paste(image_end.get(), current_x, current_y);
            return final_image;
        }
    };
}
