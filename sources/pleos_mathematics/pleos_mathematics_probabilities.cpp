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

        // Create the event
        std::shared_ptr<Probability_Universe_Event> event = std::make_shared<Probability_Universe_Event>();
        event.get()->name = name; event.get()->probability = probability; event.get()->this_event = event;
        event.get()->partition_parent = universe_partition;
        used_partition.events.push_back(event);
    };

    // Add an event conditionally to the universe
    void Probability_Universe::add_event_conditionally(std::string name, scls::Fraction probability, std::shared_ptr<Probability_Universe_Event> condition) {
        // Create the partition if needed
        if(a_partitions.size() <= 1) {a_partitions.push_back(Probability_Universe_Partition());}
        Probability_Universe_Partition& used_partition = a_partitions[a_partitions.size() - 1];

        // Create the condition
        Probability_Universe_Event::Probability_Universe_Event_Condition event_condition;
        event_condition.event_parent = condition;
        event_condition.probability = probability;
        std::shared_ptr<Probability_Universe_Event> event = used_partition.event(name);
        if(event == 0) {
            // Create the event
            std::shared_ptr<Probability_Universe_Event> event = std::make_shared<Probability_Universe_Event>();
            event.get()->name = name; event.get()->conditions_used.push_back(event_condition);
            event.get()->partition_parent = 1; event.get()->this_event = event;
            // Add the event
            used_partition.events.push_back(event);

            // Add the information to the parent
            if(!condition.get()->contains_conditions_from_this_one(event.get())) {
                condition.get()->conditions_from_this_one.push_back(event);
            }
        } else {event->conditions_used.push_back(event_condition);};
    }

    // Create a case of the tree
    void Probability_Universe::Probability_Universe_Tree::create_case(std::shared_ptr<Probability_Universe_Event> needed_event) {
        // Create the event
        scls::Fraction needed_probability = needed_event.get()->probability;
        std::string needed_text = needed_event.get()->name + std::string(" -> ");
        if(needed_event.get()->is_opposed()) {needed_text = std::string("!") + needed_text;}
        needed_text += scls::format_number_to_text(needed_probability.to_double());
        std::shared_ptr<scls::Image> image = a_generator.image_shared_ptr(needed_text, style);
        a_cases.push_back(image);

        // Add the possible others events
        for(int i = 0;i<static_cast<int>(needed_event.get()->conditions_from_this_one.size());i++) {
            bool contains = false;
            std::shared_ptr<Probability_Universe_Event> current = needed_event.get()->conditions_from_this_one[i];
            for(int j = 0;j<static_cast<int>(a_other_layer_conditions.size());j++) {if(a_other_layer_conditions[j].get()==current.get()){contains=true;break;}}
            if(!contains) {
                // Add condition to study
                a_other_layer_conditions.push_back(current);
            }
        }
    };

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
    std::shared_ptr<scls::Image> Probability_Universe::tree(int universe_partition, std::shared_ptr<Probability_Universe_Event> condition) {
        // Get the good partition
        Probability_Universe_Partition& used_partition = a_partitions[universe_partition];
        std::vector<std::shared_ptr<Probability_Universe_Event>> used_events = used_partition.events;
        std::shared_ptr<Probability_Universe_Tree> final_tree = std::make_shared<Probability_Universe_Tree>();

        // Define the graphics settings
        int first_layer_y_separation = 0;

        // Create the first layer of probability
        // Create the needed images for the first layer
        int first_layer_height = 100;
        if(used_events.size() == 1) {
            // Create the needed cases
            final_tree.get()->create_case(used_events[0]);
            final_tree.get()->create_case(used_events[0].get()->opposed_event());
        }
        // Create the other layers
        std::vector<std::shared_ptr<scls::Image>> first_layer = final_tree.get()->cases();
        std::vector<std::shared_ptr<Probability_Universe_Event>> other_layer_conditions = final_tree.get()->other_layer_conditions();
        std::vector<std::shared_ptr<scls::Image>> other_layers;
        int other_layers_height = 0;
        for(int i = 0;i<static_cast<int>(other_layer_conditions.size());i++) {
            std::shared_ptr<Probability_Universe_Event> current = other_layer_conditions[i];
            std::shared_ptr<scls::Image> current_image = tree(current.get()->partition_parent, std::shared_ptr<Probability_Universe_Event>());
            other_layers.push_back(current_image); other_layers_height += current_image.get()->height();
        }

        // Create and return the image
        int final_height = other_layers_height;
        if(first_layer_height > final_height) final_height = first_layer_height;
        int first_lines_width = 100;
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(1000, final_height, scls::Color(255, 255, 255));
        // Draw the first layer
        int current_x = 0;
        int current_y = 0;
        for(int i = 0;i<static_cast<int>(first_layer.size());i++) {
            to_return.get()->paste(first_layer[i].get(), first_lines_width, current_y);
            current_y += first_layer[i].get()->height() + first_layer_y_separation;
            if(current_x < first_layer[i].get()->width()) current_x = first_layer[i].get()->width();
            first_layer[i].reset();
        } current_x += first_lines_width;
        // Draw the second layer
        current_y = 0;
        for(int i = 0;i<static_cast<int>(other_layers.size());i++) {
            to_return.get()->paste(other_layers[i].get(), current_x, current_y);
            current_y += other_layers[i].get()->height() + first_layer_y_separation;
            other_layers[i].reset();
        }

        return to_return;
    }
}
