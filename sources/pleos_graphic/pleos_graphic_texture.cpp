//******************
//
// pleos_graphic_texture.h
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
// This file contains the source code of pleos_graphic.h for the Graphic_Texture_Object class.
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

// Include pleos_graphic.h
#include "../../pleos_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // Graphic sub-objects
    //
    //******************

    // Graphic_Texture_Object constructor
    Graphic::Graphic_Texture_Object::Graphic_Texture_Object(std::weak_ptr<Graphic> graphic_base):Graphic_Base_Object(graphic_base){};

    // Draws the object on an image
    void Graphic::Graphic_Texture_Object::draw_on_image(std::shared_ptr<scls::__Image_Base> image){
        // Asserts
        if(!a_drew_on_image || texture() == 0){return;}

        // Create the needed datas
        int needed_x = 0;
        int needed_y = 0;

        // Loads the texture
        load_last_texture();
        scls::Image needed_texture = a_last_texture;

        // Paste the image
        needed_x = graphic_x_to_pixel_x(absolute_x()) - needed_texture.width() / 2;
        needed_y = graphic_y_to_pixel_y_inversed(absolute_y()) - needed_texture.height() / 2;
        image.get()->paste(needed_texture, needed_x, needed_y, opacity());
    }

    // Coordinates datas
    int Graphic::Graphic_Texture_Object::graphic_x_to_texture_x(scls::Fraction x){scls::Fraction local_x = x - min_x();return ((local_x / width()).to_double() * static_cast<double>(texture()->width()));}
    int Graphic::Graphic_Texture_Object::graphic_y_to_texture_y(scls::Fraction y){scls::Fraction local_y = y - min_y();return (static_cast<double>(texture()->height()) - (local_y / height()).to_double() * static_cast<double>(texture()->height()));}
    double Graphic::Graphic_Texture_Object::texture_x_to_graphic_x(int x){double normal = (attached_transform()->absolute_x() - attached_transform()->absolute_scale_x() / 2) + ((static_cast<double>(x) / a_texture.get()->width()) * attached_transform()->absolute_scale_x());return normal;}
    double Graphic::Graphic_Texture_Object::texture_y_to_graphic_y(int y){double normal = (-attached_transform()->absolute_y() - attached_transform()->absolute_scale_y() / 2) + ((static_cast<double>(y) / a_texture.get()->height()) * attached_transform()->absolute_scale_y());return normal;}

    // Loads the last texture of the object
    void Graphic::Graphic_Texture_Object::load_last_texture(){
        if(a_texture_displaying == Texture_Displaying::TD_Fill) {
            // Fill the texture
            int needed_height = height_in_pixel();
            int needed_width = width_in_pixel();
            if(texture()->height() == needed_height && texture()->width() == needed_width){a_last_texture = texture_shared_ptr();}
            else if(a_last_texture.get() == 0 || a_last_texture.get()->height() != needed_height || a_last_texture.get()->width() != needed_width){
                a_last_texture = texture()->resize_adaptative(needed_width, needed_height);
                a_last_texture_height=needed_height;a_last_texture_width=needed_width;
            }
        }
        else if(a_texture_displaying == Texture_Displaying::TD_From_Width) {
            // Fill the texture according to the width
            int needed_width = width_in_pixel();int needed_height = (static_cast<double>(needed_width) * (static_cast<double>(texture()->height()) /static_cast<double>(texture()->width())));
            if(texture()->height() == needed_height && texture()->width() == needed_width){a_last_texture = texture_shared_ptr();}
            else{
                a_last_texture = texture()->resize_adaptative(needed_width, needed_height);
                a_last_texture_height=needed_height;a_last_texture_width=needed_width;
            }
        }
        else {a_last_texture = texture_shared_ptr();}

        // Rotation
        if(rotation() != 0){a_last_texture.get()->rotate(rotation().to_double());}
    }

    // Returns the source to a XML text
    std::string Graphic::Graphic_Texture_Object::to_displayed_text(){return std::string("objet texturé");}
    std::string Graphic::Graphic_Texture_Object::to_xml_text(){return std::string("<") + to_xml_text_object_name() + to_xml_text_name() + to_xml_text_x() + to_xml_text_y() + to_xml_text_opacity() + to_xml_text_width() + to_xml_text_height() + to_xml_text_rotation() + to_xml_text_source() + to_xml_text_tags() + std::string(">");}
    std::string Graphic::Graphic_Texture_Object::to_xml_text_base(){return __Graphic_Object_Base::to_xml_text_base() + to_xml_text_source();}
    std::string Graphic::Graphic_Texture_Object::to_xml_text_object_name(){return std::string("texture_object");}
    std::string Graphic::Graphic_Texture_Object::to_xml_text_source(){return std::string(" source=\"") + source() + std::string("\"");}
    std::string Graphic::Graphic_Texture_Object::to_xml_text_texture_displaying(){if(a_texture_displaying == Texture_Displaying::TD_Fill){return std::string(" texture_displying=fill");}else if(a_texture_displaying == Texture_Displaying::TD_From_Height){return std::string(" texture_displying=from_height");}else if(a_texture_displaying == Texture_Displaying::TD_From_Width){return std::string(" texture_displying=from_width");}else if(a_texture_displaying == Texture_Displaying::TD_Pixel_Size){return std::string(" texture_displying=pixel_size");}return std::string();}
}
