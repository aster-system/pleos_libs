//******************
//
// pleos_mathematics_probabilities.cpp
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
// This file contains the source code of pleos_mathematics_probabilities.cpp.
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
#include "../../pleos_mathematics/pleos_mathematics_probabilities.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Add an event to the universe
    void Probability_Universe::add_event(std::string name, scls::Fraction probability, int universe_partition) {
        // Create the partition if needed
        if(universe_partition == 0 && a_partitions.size() <= 0) {a_partitions.push_back(Probability_Universe_Partition());}
        Probability_Universe_Partition& used_partition = a_partitions[universe_partition];

        Probability_Universe_Event event;
        event.name = name; event.probability = probability;
        used_partition.events.push_back(std::make_shared<Probability_Universe_Event>(event));
    };

    // Add an event conditionally to the universe
    void Probability_Universe::add_event_conditionally(std::string name, scls::Fraction probability, std::shared_ptr<Probability_Universe_Event> condition) {
        // Create the partition if needed
        if(a_partitions.size() <= 1) {a_partitions.push_back(Probability_Universe_Partition());}
        Probability_Universe_Partition& used_partition = a_partitions[a_partitions.size() - 1];

        Probability_Universe_Event event;
        event.name = name; event.probability_condition = probability; event.condition_used = condition;
        // Add the event
        std::shared_ptr<Probability_Universe_Event> to_add = std::make_shared<Probability_Universe_Event>(event);
        used_partition.events.push_back(to_add);
        if(!condition.get()->contains_conditions_from_this_one(to_add.get())) {
            condition.get()->conditions_from_this_one.push_back(to_add);
        }
    }

    // Returns a redacted description of this universe
    std::string Probability_Universe::description() {
        std::string to_return = "";
        to_return += "Nous avons un univers U. ";
        int partition_size = static_cast<int>(a_partitions.size());
        if(partition_size > 1) {
            to_return += "Cette univers compte " + std::to_string(partition_size) + " partitions.";
        } else {to_return += "Cette univers compte 1 partition.";}

        // Redact each partitions
        for(int i = 0;i<partition_size;i++) {
            // Redact the current partition
            Probability_Universe_Partition& used_partition = a_partitions[i];
            int element_size = static_cast<int>(used_partition.events.size());
            to_return += " La partition " + std::to_string(i + 1) + " est constituée de " + std::to_string(element_size) + " éléments : ";
            for(int j = 0;j<element_size;j++) {
                to_return += used_partition.events[j].get()->name;
                if(j < element_size - 1) to_return += ", ";
            } to_return += ".";
        }

        return to_return;
    }

    // Create a tree of probability
    std::shared_ptr<scls::Image> Probability_Universe::tree(int universe_partition) {
        // Get the good partition
        Probability_Universe_Partition& used_partition = a_partitions[universe_partition];

        // Define the graphics settings
        int first_layer_y_separation = 50;

        // Create the first layer of probability
        // Create the needed generator
        scls::Text_Image_Generator generator;
        scls::Text_Style style; style.font_size = 50;
        // Create the needed images for the first layer
        std::vector<std::shared_ptr<scls::Image>> first_layer;
        std::vector<std::shared_ptr<Probability_Universe_Event>> other_layer_conditions;
        int first_layer_height = 0;
        if(used_partition.events.size() == 1) {
            // Create the event
            scls::Fraction needed_probability = used_partition.events[0].get()->probability;
            std::string needed_text = used_partition.events[0].get()->name + std::string(" -> ");
            needed_text += scls::format_number_to_text(needed_probability.to_double());
            std::shared_ptr<scls::Image> image = generator.image_shared_ptr(needed_text, style);
            first_layer.push_back(image); first_layer_height += image.get()->height() + first_layer_y_separation;
            // Create the not event
            needed_probability = scls::Fraction(1) - used_partition.events[0].get()->probability;
            needed_text = std::string("!") + used_partition.events[0].get()->name + std::string(" -> ");
            needed_text += scls::format_number_to_text(needed_probability.to_double());
            image = generator.image_shared_ptr(needed_text, style);
            first_layer.push_back(image); first_layer_height += image.get()->height();

            // Add the possible others events
            for(int i = 0;i<static_cast<int>(used_partition.events[0].get()->conditions_from_this_one.size());i++) {
                bool contains = false;
                std::shared_ptr<Probability_Universe_Event> current = used_partition.events[0].get()->conditions_from_this_one[i];
                for(int j = 0;j<static_cast<int>(other_layer_conditions.size());j++) {if(other_layer_conditions[j].get()==current.get()){contains=true;break;}}
                if(!contains) {
                    // Add condition to study
                    other_layer_conditions.push_back(current);
                }
            }
        }
        // Create the other layers


        // Create and return the image
        int first_lines_width = 100;
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(1000, first_layer_height, scls::Color(255, 255, 255));
        // Draw the first layer
        int current_y = 0;
        for(int i = 0;i<static_cast<int>(first_layer.size());i++) {
            to_return.get()->paste(first_layer[i].get(), first_lines_width, current_y);
            current_y += first_layer[i].get()->height() + first_layer_y_separation;
        }

        return to_return;
    }
}
