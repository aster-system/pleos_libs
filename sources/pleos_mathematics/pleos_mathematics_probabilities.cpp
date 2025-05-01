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

    //******************
    // The Probability_Universe_Event class
    //******************

    // Checks the opposed event probability for an event and returns if the probability was found
    bool __check_opposed_probability(std::shared_ptr<Probability_Universe_Event> needed_event, std::shared_ptr<Probability_Universe_Event> condition, std::string& redaction, scls::Textual_Math_Settings* settings) {
        std::string needed_name = needed_event.get()->complete_name();
        if(condition.get() == 0) {
            // No condition is needed
            std::shared_ptr<Probability_Universe_Event> opposed_event;
            if(needed_event.get()->is_opposed()) {opposed_event = needed_event.get()->opposed_event_as_opposed();} else {opposed_event = needed_event.get()->opposed_event();}
            if(opposed_event.get() != 0 && opposed_event.get()->probability() != -1) {
                needed_event.get()->set_probability(scls::Fraction(1) - opposed_event.get()->probability());
                redaction += "Pour réaliser l'arbre, nous avons besoin de la probabilité de " + needed_name + ". ";
                redaction += "Or, l'évènement opposé à " + needed_name + ", qui est ";
                if(needed_name.size() > 0 && needed_name[0] == '!') redaction += needed_name.substr(1, needed_name.size() - 1);
                else {redaction += std::string("!") + needed_name;}
                redaction += ", a une probabilité de " + opposed_event.get()->probability().to_std_string(settings) + ". ";
                redaction += "Donc, la probabilité de " + needed_name + " est de 1 - " + opposed_event.get()->probability().to_std_string(settings) + ", soit " + needed_event.get()->probability().to_std_string(settings) + ". ";
                return true;
            }
        }
        else {
            // Condition is needed
            std::string condition_name = condition.get()->name();
            std::shared_ptr<Probability_Universe_Event> opposed_event;
            if(needed_event.get()->is_opposed()) {opposed_event = needed_event.get()->opposed_event_as_opposed();} else {opposed_event = needed_event.get()->opposed_event();}
            std::shared_ptr<Probability_Universe_Event::Probability_Universe_Event_Condition> current_condition = needed_event.get()->condition_used(condition);
            std::shared_ptr<Probability_Universe_Event::Probability_Universe_Event_Condition> opposed_condition = opposed_event.get()->condition_used(condition);
            if(opposed_condition.get()->probability != -1) {
                current_condition.get()->probability = scls::Fraction(1) - opposed_condition.get()->probability;
                redaction += "Pour réaliser l'arbre, nous avons besoin de la probabilité de " + needed_name + " sachant " + condition_name + ". ";
                redaction += "Or, l'évènement opposé à " + needed_name + " sachant " + condition_name + ", qui est ";
                if(needed_name.size() > 0 && needed_name[0] == '!') redaction += needed_name.substr(1, needed_name.size() - 1);
                else {redaction += std::string("!") + needed_name;}
                redaction += " sachant " + condition_name + ", a une probabilité de " + opposed_condition.get()->probability.to_std_string(settings) + ". ";
                redaction += "Donc, la probabilité de " + needed_name + " sachant " + condition_name + " est de 1 - " + opposed_condition.get()->probability.to_std_string(settings) + ", soit " + current_condition.get()->probability.to_std_string(settings) + ". ";
                return true;
            }
        } return false;
    }

    // Opposite of the event
    std::shared_ptr<Probability_Universe_Event> Probability_Universe_Event::opposed_event() {
        if(a_opposed_event_as_opposed.lock().get() != 0) {return std::shared_ptr<Probability_Universe_Event>();};
        if(a_opposed_event.get() == 0) {
            a_opposed_event = std::make_shared<Probability_Universe_Event>(a_name, a_partition_parent);
            a_opposed_event.get()->a_opposed_event_as_opposed = a_this_event;
            a_opposed_event.get()->a_this_event = a_opposed_event;
        } return a_opposed_event;
    };

    //******************
    // The Probability_Universe_Tree class
    //******************

    // Create a case of the tree
    std::shared_ptr<scls::Image> Probability_Universe::Probability_Universe_Tree::create_case(std::shared_ptr<Probability_Universe_Event> needed_event, std::shared_ptr<Probability_Universe_Event> condition) {
        // Create the neeeded datas
        scls::Fraction needed_probability = needed_event.get()->probability();
        scls::Textual_Math_Settings* settings = 0;
        if(condition.get() == 0) {
            // Calculate the probability if needed without condition
            needed_probability = needed_event.get()->probability();
        } else {
            // Calculate the probability if needed with condition
            std::shared_ptr<Probability_Universe_Event::Probability_Universe_Event_Condition> needed_condition = needed_event.get()->condition_used(condition);
            needed_probability = needed_condition.get()->probability;
            needed_probability = needed_condition.get()->probability;
        }

        // Create the event
        std::string needed_text = needed_event.get()->complete_name() + std::string(" -> ");
        needed_text += scls::format_number_to_text(needed_probability.to_double());
        std::shared_ptr<scls::Image> image = a_generator.image_shared_ptr(needed_text, style);
        a_cases.push_back(image);

        // Add the possible others events
        for(int i = 0;i<static_cast<int>(needed_event.get()->conditions_from_this_one().size());i++) {
            bool contains = false;
            std::shared_ptr<Probability_Universe_Event::Probability_Universe_Event_Condition> current = needed_event.get()->conditions_from_this_one()[i].get()->condition_used(needed_event);
            for(int j = 0;j<static_cast<int>(a_other_layer_conditions.size());j++) {if(a_other_layer_conditions[j].get()==current.get()){contains=true;break;}}
            if(!contains) {
                // Add condition to study
                a_other_layer_conditions.push_back(current);
            }
        } return image;
    };

    // Create the first layer of the tree
    void Probability_Universe::Probability_Universe_Tree::create_first_layer(std::shared_ptr<Probability_Universe_Event> condition) {
        // Add the missing events
        a_first_layer_height = 100;
        if(a_used_events.size() == 1) {a_used_events.push_back(a_used_events[0].get()->opposed_event());}

        // Create the needed images for the first layer
        a_first_layer_max_width = 0;
        std::shared_ptr<scls::Image> last_case;
        a_differences_y = std::vector<int>(a_used_events.size(), -1);
        for(int i = 0;i<static_cast<int>(a_used_events.size());i++) {
            std::shared_ptr<scls::Image> current_case = create_case(a_used_events[i], condition);
            if(current_case.get()->width() > a_first_layer_max_width) {a_first_layer_max_width = current_case.get()->width();}

            // Create the needed y
            if(i <= 0) {a_differences_y[i] = 0;}
            else {a_differences_y[i] = last_case.get()->height();}
            last_case = current_case;
        }
    }

    // Create the other layers of the tree
    void Probability_Universe::Probability_Universe_Tree::create_other_layers() {}

    // Draw the tree
    std::shared_ptr<scls::Image> Probability_Universe::Probability_Universe_Tree::image() {
        // Create and return the image
        int final_height = a_other_layers_height;
        if(a_first_layer_height > final_height) final_height = a_first_layer_height;
        int first_layer_y_separation = 0;
        int first_lines_width = 100;
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(1000, final_height, scls::Color(255, 255, 255));

        // Draw the second layer
        int current_x = first_lines_width + a_first_layer_max_width;
        int current_y = 0;
        int last_layer_height = 0;
        std::vector<int> needed_y = std::vector<int>(a_used_events.size(), 0);
        for(int i = 0;i<static_cast<int>(a_other_layers.size());i++) {
            // Paste the tree
            to_return.get()->paste(a_other_layers[i].get(), current_x, current_y);
            // Add the needed y
            int position = 0; for(int j = 0;j<static_cast<int>(a_used_events.size());j++){if(a_used_events[j].get()==a_other_layer_conditions[i].get()->event_parent.lock().get()){position=j;break;}}
            if(i <= 0) {needed_y[position] = a_other_layers[i].get()->height() / 2;}
            else {needed_y[position] = a_other_layers[i].get()->height() / 2 + last_layer_height / 2;}
            current_y += a_other_layers[i].get()->height() + first_layer_y_separation;
            last_layer_height = a_other_layers[i].get()->height();
            a_other_layers[i].reset();
        }

        // Filter the needed Y
        if(a_cases.size() > 0 && needed_y.size() > 0 && needed_y[0] <= 0) {needed_y[0] = a_cases[0].get()->height() / 2;}
        for(int i = 1;i<static_cast<int>(needed_y.size());i++) {
            if(needed_y[i] < a_differences_y[i]) {
                needed_y[i] = a_differences_y[i];
            }
        }

        // Draw the first layer
        current_x = 0;
        current_y = 0;
        std::vector<int> final_y = std::vector<int>(needed_y.size(), 0);
        for(int i = 0;i<static_cast<int>(a_cases.size());i++) {
            current_y += needed_y[i]; final_y[i] = current_y;
            to_return.get()->paste(a_cases[i].get(), first_lines_width, current_y - a_cases[i].get()->height() / 2);
            a_cases[i].reset();
        }

        // Create the lines
        current_x = first_lines_width;
        current_y = final_height / 2;
        for(int i = 0;i<static_cast<int>(needed_y.size());i++) {
            to_return.get()->draw_line(0, current_y, current_x, final_y[i], scls::Color(0, 0, 0), 2);
        } return to_return;
    }

    //******************
    // The Probability_Universe class
    //******************

    // Add an event to the universe
    void Probability_Universe::add_event(std::string name, scls::Fraction probability, int universe_partition) {
        // Create the partition if needed
        if(universe_partition == 0 && a_partitions.size() <= 0) {a_partitions.push_back(Probability_Universe_Partition());}
        Probability_Universe_Partition& used_partition = a_partitions[universe_partition];

        // Create the event
        bool opposed = false;
        if(name[0] == '!'){name = name.substr(1, name.size() - 1);opposed=true;}
        std::shared_ptr<Probability_Universe_Event> event = std::make_shared<Probability_Universe_Event>(name, universe_partition);
        event.get()->set_this_event(event);
        if(opposed) {event.get()->opposed_event().get()->set_probability(probability);}
        else {event.get()->set_probability(probability);}
        used_partition.events.push_back(event);
    };

    // Add an event conditionally to the universe
    void Probability_Universe::add_event_conditionally(std::string name, scls::Fraction probability, std::shared_ptr<Probability_Universe_Event> condition) {
        // Create the partition if needed
        if(a_partitions.size() <= 1) {a_partitions.push_back(Probability_Universe_Partition());}
        Probability_Universe_Partition& used_partition = a_partitions[a_partitions.size() - 1];

        // Create the condition
        std::shared_ptr<Probability_Universe_Event> event = used_partition.event(name);
        if(event.get() == 0) {
            // Create the event
            bool opposed = false;
            if(name[0] == '!'){name = name.substr(1, name.size() - 1);opposed=true;}
            event = std::make_shared<Probability_Universe_Event>(name, 1);
            event.get()->set_this_event(event);
            if(opposed){event = event.get()->opposed_event();}
            // Add the event
            used_partition.events.push_back(event);
        }

        // The event already exists
        std::shared_ptr<Probability_Universe_Event::Probability_Universe_Event_Condition> event_condition = event->condition_used(condition);
        event_condition.get()->probability = probability;
        // Add the information to the parent
        condition.get()->add_condition_from_this_one(event);
    }

    // Returns a condition, stores according to which other it is
    std::shared_ptr<Probability_Universe_Event::Probability_Universe_Event_Condition> Probability_Universe_Event::condition_used(std::shared_ptr<Probability_Universe_Event> condition) {
        for(int i=0;i<static_cast<int>(a_conditions_used.size());i++){
            if(a_conditions_used.at(i).get()->event_parent.lock().get()==condition.get()){
                return a_conditions_used.at(i);
            }
        }
        std::shared_ptr<Probability_Universe_Event::Probability_Universe_Event_Condition> to_add = std::make_shared<Probability_Universe_Event::Probability_Universe_Event_Condition>();
        to_add.get()->event = a_this_event;
        to_add.get()->event_parent = condition;
        a_conditions_used.push_back(to_add);
        return to_add;
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
                to_return += used_partition.events[j].get()->name();
                if(j < element_size - 1) to_return += ", ";
            } to_return += ".";
        }

        return to_return;
    }

    // Create a tree of probability
    std::shared_ptr<scls::Image> Probability_Universe::__tree(int universe_partition, std::shared_ptr<Probability_Universe_Event> condition, std::string& redaction) {
        // Get the good partition
        Probability_Universe_Partition& used_partition = a_partitions[universe_partition];
        std::shared_ptr<Probability_Universe_Tree> final_tree = std::make_shared<Probability_Universe_Tree>(redaction);
        std::vector<std::shared_ptr<Probability_Universe_Event>>& used_events = final_tree.get()->used_events();
        used_events = used_partition.events;

        // Create the first layer
        final_tree.get()->create_first_layer(condition);
        // Create the other layers
        //final_tree.get()->create_other_layers();
        std::vector<std::shared_ptr<scls::Image>> first_layer = final_tree.get()->cases();
        std::vector<std::shared_ptr<Probability_Universe_Event::Probability_Universe_Event_Condition>>& other_layer_conditions = final_tree.get()->other_layer_conditions();
        std::vector<std::shared_ptr<scls::Image>>& other_layers = final_tree.get()->other_layers();
        int other_layers_height = 0;
        for(int j = 0;j<static_cast<int>(other_layer_conditions.size());j++) {
            std::shared_ptr<Probability_Universe_Event> current = other_layer_conditions[j].get()->event.lock();
            std::shared_ptr<scls::Image> current_image = __tree(current.get()->partition_parent(), other_layer_conditions[j].get()->event_parent.lock(), redaction);
            other_layers.push_back(current_image); other_layers_height += current_image.get()->height();
        }

        // Create and return the image
        final_tree.get()->set_other_layers_height(other_layers_height);
        return final_tree.get()->image();
    }
    std::shared_ptr<scls::Image> Probability_Universe::tree(int universe_partition, std::shared_ptr<Probability_Universe_Event> condition, std::string& redaction) {
        // Start the redaction
        redaction += "Créeons un arbre pondéré avec cet univers. ";
        redaction += "Cet arbre partira de la partition " + std::to_string(universe_partition) + " de l'univers. ";
        return __tree(universe_partition, condition, redaction);
    }
}
