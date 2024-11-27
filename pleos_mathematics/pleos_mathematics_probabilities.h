//******************
//
// pleos_mathematics_probabilities.h
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
// This file contains the handling of probabilities in the PLEOS mathematics file.
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

#ifndef PLEOS_MATHEMATICS_PROBABILITIES
#define PLEOS_MATHEMATICS_PROBABILITIES

// Include SCLS Graphic Benoit
#include "../../../scls-graphic-benoit/scls_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    class Probability_Universe {
        // Class representating a probability universe
    public:
        // Struct representating an element in a probability universe
        struct Probability_Universe_Event {
            // Name of the event
            std::string name = "";
            // Probability of the event
            scls::Fraction probability = scls::Fraction(-1);

            // Opposite of the event
            std::shared_ptr<Probability_Universe_Event> opposed_event;
            // Opposite of the event, if this event is the opposite of an event
            std::weak_ptr<Probability_Universe_Event> opposed_event_as_opposed;

            // If the condition is used for a conditionnal, stores according to which other it is
            std::vector<std::shared_ptr<Probability_Universe_Event>> conditions_from_this_one;
            inline bool contains_conditions_from_this_one(Probability_Universe_Event* test){for(int i=0;i<static_cast<int>(conditions_from_this_one.size());i++){if(conditions_from_this_one[i].get()==test){return true;}}return false;};
            // If the condition is conditionnal, stores according to which other it is
            std::weak_ptr<Probability_Universe_Event> condition_used;
            // Probability of the event according to the condition
            scls::Fraction probability_condition = scls::Fraction(-1);
        };
        // Struct representating a partition in a probability universe
        struct Probability_Universe_Partition {
            // Returns an even by its name
            inline std::shared_ptr<Probability_Universe_Event> event(std::string name) {
                for(int i = 0;i<static_cast<int>(events.size());i++) {
                    if(events[i].get()->name==name) {
                        return events[i];
                    } else if(name.size() > 1 && events[i].get()->name == name.substr(1, name.size()-1) && name[0] == '!') {
                        // Create the opposed event if needed
                        if(events[i].get()->opposed_event.get() == 0) {
                            std::shared_ptr<Probability_Universe_Event> needed_event = std::make_shared<Probability_Universe_Event>();
                            needed_event.get()->name = events[i].get()->name;
                            events[i].get()->opposed_event = needed_event;
                        } return events[i].get()->opposed_event;
                    }
                } return std::shared_ptr<Probability_Universe_Event>();
            };

            // Each possibles events
            std::vector<std::shared_ptr<Probability_Universe_Event>> events;
        };

        // Probability_Universe constructor
        Probability_Universe(){};

        // Returns a redacted description of this universe
        std::string description();
        // Create a tree of probability
        std::shared_ptr<scls::Image> tree(int universe_partition);
        inline std::shared_ptr<scls::Image> tree(){return tree(0);};

        // Add an event to the universe
        void add_event(std::string name, scls::Fraction probability, int universe_partition);
        inline void add_event(std::string name, scls::Fraction probability) {add_event(name, probability, 0);};
        // Add an event conditionally to the universe
        void add_event_conditionally(std::string name, scls::Fraction probability, std::shared_ptr<Probability_Universe_Event> condition);
        inline void add_event_conditionally(std::string name, scls::Fraction probability, std::string condition_name){std::shared_ptr<Probability_Universe_Event> needed_event = a_partitions[0].event(condition_name);if(needed_event.get() != 0) {add_event_conditionally(name, probability, needed_event);}};
    private:
        // Each possibles partitions
        std::vector<Probability_Universe_Partition> a_partitions;
    };
}

#endif // PLEOS_MATHEMATICS_PROBABILITIES
