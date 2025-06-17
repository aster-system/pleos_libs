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

    // Probability_Condition constructor
    Probability_Universe_Event::Probability_Condition::Probability_Condition(std::shared_ptr<Probability_Universe_Event>event):events(std::vector<std::shared_ptr<Probability_Universe_Event>>(1,event)){};
    Probability_Universe_Event::Probability_Condition::Probability_Condition(){};

    // Returns if 2 conditions are equals or not
    bool Probability_Universe_Event::Probability_Condition::is_equal(Probability_Condition other) const {
        if(events.size() != other.events.size()){return false;}

        std::vector<std::shared_ptr<Probability_Universe_Event>> events_copy = events;
        for(int i = 0;i<static_cast<int>(events_copy.size());i++) {
            bool good = false;
            for(int j = 0;j<static_cast<int>(other.events.size());j++) {
                if(events_copy.at(i).get() == other.events.at(j).get()){other.events.erase(other.events.begin() + j);good = true;break;}
            }
            if(!good){return false;}
            events_copy.erase(events_copy.begin() + i);i--;
        }
        return true;
    }

    // Returns the parent condition of this one
    Probability_Universe_Event::Probability_Condition Probability_Universe_Event::Probability_Condition::parent_condition(){
        Probability_Condition parent = Probability_Condition();
        parent.events=events;
        if(parent.events.size()>0){parent.events.pop_back();}
        return parent;
    };

    // Returns an even by its name
    std::shared_ptr<Probability_Universe_Event> Probability_Universe_Event::Probability_Universe_Partition::event(std::string name) {
        for(int i = 0;i<static_cast<int>(events.size());i++) {if(events[i].get()->name() == name) {return events[i];}}
        return std::shared_ptr<Probability_Universe_Event>();
    };

    // Missing probabilities in the partition
    std::vector<std::shared_ptr<Probability_Universe_Event>> Probability_Universe_Event::Probability_Universe_Partition::missing_probabilities(Probability_Condition* condition) {
        std::vector<std::shared_ptr<Probability_Universe_Event>> missing_probabilities;
        for(int i = 0;i<static_cast<int>(events.size());i++){if(events.at(i).get()->probability(condition)==-1){missing_probabilities.push_back(events.at(i));}}
        return missing_probabilities;
    }

    // Name of the probability
    std::string Probability_Universe_Event::name(Probability_Condition* condition) const {
        std::string final_name = name();
        for(int i = 0;i<static_cast<int>(condition->events.size());i++){final_name = condition->events.at(i).get()->name() + std::string(" inter ") + final_name;}

        return final_name;
    }

    // Sets the probability of an event
    scls::Fraction Probability_Universe_Event::probability(Probability_Condition condition) const{return probability(&condition);}
    scls::Fraction Probability_Universe_Event::probability(Probability_Universe_Event* condition) const {
        if(condition == 0){return probability();};
        for(int i = 0;i<static_cast<int>(a_probabilities_condition.size());i++) {if(a_probabilities_condition.at(i).events.size() == 1 && a_probabilities_condition.at(i).events.at(0).get() == condition){return a_probabilities.at(i);}}
        return -1;
    }
    scls::Fraction Probability_Universe_Event::probability(Probability_Condition* condition) const {
        if(condition == 0 || condition->events.size() == 0){return probability();};
        for(int i = 0;i<static_cast<int>(a_probabilities_condition.size());i++) {if(a_probabilities_condition.at(i).is_equal(*condition)){return a_probabilities.at(i);}}
        return -1;
    }
    void Probability_Universe_Event::set_probability(std::shared_ptr<Probability_Universe_Event> condition, scls::Fraction new_probability) {Probability_Condition needed_condition=Probability_Condition(condition);set_probability(&needed_condition, new_probability);};
    void Probability_Universe_Event::set_probability(Probability_Condition* condition, scls::Fraction new_probability) {
        int index = -1;
        for(int i = 0;i<static_cast<int>(a_probabilities_condition.size());i++) {if(a_probabilities_condition.at(i).is_equal(*condition)){index = i;break;}}

        if(index == -1) {
            a_probabilities_condition.push_back(*condition);
            a_probabilities.push_back(new_probability);
        }
        else {a_probabilities[index] = new_probability;}
    }

    // Directly solve the partition
    scls::Fraction __probabilities_sum(std::vector<std::shared_ptr<Probability_Universe_Event>>& probabilities, Probability_Universe_Event::Probability_Condition* condition){scls::Fraction probability = 0;for(int i = 0;i<static_cast<int>(probabilities.size());i++){if(probabilities.at(i).get()->probability(condition) != -1){probability+=probabilities.at(i).get()->probability(condition);}}return probability;}
    int Probability_Universe_Event::Probability_Universe_Partition::solve(std::string* redaction, Probability_Universe_Partition* other_partition) {int total=0;for(int i = 0;i<static_cast<int>(other_partition->events.size());i++){total+=solve(redaction, Probability_Universe_Event::Probability_Condition(other_partition->events.at(i)));}return total;}
    int Probability_Universe_Event::Probability_Universe_Partition::solve(std::string* redaction, Probability_Condition condition){return solve(redaction, &condition);}
    int Probability_Universe_Event::Probability_Universe_Partition::solve(std::string* redaction, Probability_Condition* condition) {
        // Assert
        std::vector<std::shared_ptr<Probability_Universe_Event>> current_missing_probabilities = missing_probabilities(condition);
        if(static_cast<int>(current_missing_probabilities.size()) <= 0){return 0;}

        if(static_cast<int>(current_missing_probabilities.size()) == 1) {
            // Only one probability is missing
            scls::Fraction probability = __probabilities_sum(events, condition);
            Probability_Universe_Event* missing_probability = current_missing_probabilities.at(0).get();
            missing_probability->set_probability(condition, 1 - probability);

            // Redaction
            if(redaction != 0) {
                (*redaction) += std::string("L'évènement \"") + missing_probability->probability_name(condition) + std::string("\" a une probabilité inconnue.");
                (*redaction) += std::string(" Or, tous les éléments constituant l'évènement opposé à \"") + missing_probability->name() + std::string("\" ont une probabilité de ") + probability.to_std_string(0) + std::string(".");
                (*redaction) += std::string(" Donc, cet élément a une probabilité de 1 - ") + probability.to_std_string(0) + std::string(", soit ") + (1 - probability).to_std_string(0) + std::string(".");
            }
        }
        else if(static_cast<int>(current_missing_probabilities.size()) == static_cast<int>(events.size())) {
            // All the probabilities are missing
            Probability_Condition parent_condition = condition->parent_condition();
            if(!condition->is_equal(parent_condition)) {
                for(int i = 0;i<static_cast<int>(events.size());i++) {
                    scls::Fraction total_probability = events.at(i).get()->probability(parent_condition);
                    std::vector<std::shared_ptr<Probability_Universe_Event>>& parent_probabilities = condition->last_event()->partition_parent()->events;
                    scls::Fraction other_probabilities = 0;int missing_probabilities_number = 0;
                    for(int j = 0;j<static_cast<int>(parent_probabilities.size());j++){
                        if(parent_probabilities.at(j).get() != condition->last_event()){
                            Probability_Condition current_condition = parent_condition;
                            current_condition.events.push_back(parent_probabilities.at(j));
                            if(events.at(i).get()->probability(current_condition) == -1){missing_probabilities_number++;}
                            else{other_probabilities += events.at(i).get()->probability(current_condition) * parent_probabilities.at(j).get()->probability(parent_condition);}
                        }
                    }

                    // If the conditions are good
                    if(missing_probabilities_number == 0) {
                        scls::Fraction final_probability = (1 - other_probabilities / total_probability) * total_probability;
                        events.at(i).get()->set_probability(condition, final_probability / condition->last_event()->probability(parent_condition));
                        // Sets the other object too
                        Probability_Condition other_condition = parent_condition;
                        other_condition.events.push_back(events.at(i));
                        if(condition->last_event()->probability(&other_condition) == -1){
                            condition->last_event()->set_probability(&other_condition, final_probability / events.at(i).get()->probability(parent_condition));
                        }

                        // Redaction
                        if(redaction != 0) {
                            (*redaction) += std::string("L'évènement \"") + events.at(i).get()->probability_name(condition) + std::string("\" a une probabilité inconnue.");
                            (*redaction) += std::string(" Or, tous les éléments constituant l'évènement \"") + events.at(i).get()->name(&parent_condition) + std::string("\" ont une probabilité de ") + total_probability.to_std_string(0) + std::string(" dans l'univers.");
                            (*redaction) += std::string(" De plus, tous les éléments constituant l'évènement \"") + events.at(i).get()->name() + std::string("\" n'étant pas ") + events.at(i).get()->probability_name(condition) + std::string(" ont une probabilité de ") + (other_probabilities / total_probability).to_std_string(0) + std::string(" en sachant ") + events.at(i).get()->name(&parent_condition) + std::string(".");
                            (*redaction) += std::string(" Donc, la probabilité de ") + events.at(i).get()->probability_name(condition) + std::string(" sachant ") + events.at(i).get()->name(&parent_condition) + std::string(" est 1 - ") + (other_probabilities / total_probability).to_std_string(0) + std::string(" = ") + (1 - other_probabilities / total_probability).to_std_string(0) + std::string(".");
                            (*redaction) += std::string(" En ne sachant pas ") + events.at(i).get()->name(&parent_condition) + std::string(", la probabilité de ") + events.at(i).get()->probability_name(condition) + std::string(" devient ") + final_probability.to_std_string(0) + std::string(".");
                            (*redaction) += std::string(" En sachant ") + events.at(i).get()->name(&parent_condition) + std::string(", la probabilité devient ") + final_probability.to_std_string(0) + std::string(" / ") + condition->last_event()->probability(parent_condition).to_std_string(0) + std::string(" = ") + (final_probability / condition->last_event()->probability(parent_condition)).to_std_string(0) + std::string(".");
                        }
                    }
                    else{return -1;}
                }
            }
        }

        return static_cast<int>(current_missing_probabilities.size());
    }

    //******************
    // The Probability_Universe class
    //******************

    // Probability_Universe constructor
    Probability_Universe::Probability_Universe():Probability_Universe_Event(){}

    // Add an event to the universe
    void Probability_Universe::add_event(std::string name, scls::Fraction probability) {add_event(name, probability, partitions().size());};
    void Probability_Universe::add_event(std::string name, scls::Fraction probability, int universe_partition) {
        // Create the partition if needed
        while(static_cast<int>(partitions().size()) <= universe_partition) {partitions().push_back(std::make_shared<Probability_Universe_Partition>());}
        Probability_Universe_Partition* used_partition = partitions().at(universe_partition).get();

        // Create the event
        bool opposed = false;
        if(name.at(0) == '!'){name = name.substr(1, name.size() - 1);opposed=true;}
        std::shared_ptr<Probability_Universe_Event> event = std::make_shared<Probability_Universe_Event>(name, universe_partition);
        event.get()->set_partition_parent(partitions().at(universe_partition));
        event.get()->set_probability(probability);
        event.get()->set_this_event(event);
        used_partition->events.push_back(event);
        // Create the opposite event
        if(opposed){name = name.substr(1, name.size() - 1);}else{name = std::string("!") + name;}
        event = std::make_shared<Probability_Universe_Event>(name, universe_partition);
        event.get()->set_partition_parent(partitions().at(universe_partition));
        event.get()->set_this_event(event);
        used_partition->events.push_back(event);
    };

    // Add an event conditionally to the universe
    void Probability_Universe::add_event_conditionally(std::string condition_name, std::string name, scls::Fraction probability){std::shared_ptr<Probability_Universe_Event> needed_event = event_shared_ptr(condition_name);if(needed_event.get() != 0) {add_event_conditionally(needed_event, name, probability);}};
    void Probability_Universe::add_event_conditionally(std::shared_ptr<Probability_Universe_Event> condition, std::string name, scls::Fraction probability) {
        // Create the partition if needed
        std::shared_ptr<Probability_Universe_Event> final_event = event_shared_ptr(name);
        final_event.get()->set_probability(condition, probability / condition.get()->probability());
        // Create the countrary event
        Probability_Condition other_condition = final_event;
        condition.get()->set_probability(&other_condition, probability / final_event->probability());
    }

    // Returns an event by its name
    std::shared_ptr<Probability_Universe_Event> Probability_Universe::event_shared_ptr(std::string name){
        for(int i = 0;i<static_cast<int>(partitions().size());i++){
            std::shared_ptr<Probability_Universe_Event>current_event=partitions().at(i).get()->event(name);
            if(current_event.get()!=0){return current_event;}
        }
        return std::shared_ptr<Probability_Universe_Event>();
    };

    // Returns a redacted description of this universe
    std::string Probability_Universe::description() {
        std::string to_return = "";
        to_return += "Nous avons un univers U. ";
        int partition_size = static_cast<int>(partitions().size());
        if(partition_size > 1) {
            to_return += "Cette univers compte " + std::to_string(partition_size) + " partitions.";
        } else {to_return += "Cette univers compte 1 partition.";}

        // Redact each partitions
        for(int i = 0;i<partition_size;i++) {
            // Redact the current partition
            Probability_Universe_Partition* used_partition = partitions().at(i).get();
            int element_size = static_cast<int>(used_partition->events.size());
            to_return += " La partition " + std::to_string(i + 1) + " est constituée de " + std::to_string(element_size) + " éléments : ";
            for(int j = 0;j<element_size;j++) {
                to_return += used_partition->events[j].get()->name();
                if(j < element_size - 1) to_return += ", ";
            } to_return += ".";
        }

        return to_return;
    }

    // Solve the universe
    void Probability_Universe::solve(std::string* redaction) {
        // Solve every universe alone
        int total = 0;int total_current = 0;
        for(int i = 0;i<static_cast<int>(partitions().size());i++){
            int current = partitions().at(i).get()->solve(redaction, Probability_Universe_Event::Probability_Condition());
            if(current > 0){(*redaction) += std::string("</br>");total_current+=current;}
        }
        total += total_current;

        // Solve every universe conditionnaly
        if(total_current > 0){(*redaction) += std::string("</br>");}total_current=0;
        for(int i = 0;i<static_cast<int>(partitions().size());i++){
            for(int j = 0;j<static_cast<int>(partitions().size());j++){
                if(i != j){
                    int current = partitions().at(i).get()->solve(redaction, partitions().at(j).get());
                    if(current > 0){(*redaction) += std::string("</br>");total+=current;}
                }
            }
        }
        total += total_current;

        // Return the result
        if(total > 0){(*redaction) += std::string("</br>");}
    }

    // Create a tree of probability
    std::vector<std::shared_ptr<Tree<std::string>>> Probability_Universe::__tree(Probability_Condition* condition, std::shared_ptr<Tree<std::string>> used_tree, int universe_partition, std::string& redaction) {
        // Get the good partition
        Probability_Universe_Partition* used_partition = partitions().at(universe_partition).get();
        used_tree.get()->set_direction(scls::Point_2D(1, 0));

        // Create the layers
        std::vector<std::shared_ptr<Tree<std::string>>> to_return;
        for(int i = 0;i<static_cast<int>(used_partition->events.size());i++) {
            scls::Fraction needed_probability = used_partition->events.at(i).get()->probability(condition);
            if(needed_probability == -1) {used_tree.get()->add_node(used_partition->events.at(i).get()->name() + std::string(" - ?"));}
            else{used_tree.get()->add_node(used_partition->events.at(i).get()->name() + std::string(" - ") + needed_probability.to_std_string(0));}
            to_return.push_back(used_tree.get()->children().at(used_tree.get()->children().size() - 1));
        }
        return to_return;
    }
    std::shared_ptr<Tree<std::string>> Probability_Universe::tree(std::string& redaction){return tree(0, redaction);}
    std::shared_ptr<Tree<std::string>> Probability_Universe::tree(int universe_partition_1, int universe_partition_2, std::string& redaction) {
        // Start the redaction
        redaction += "Créeons un arbre pondéré avec cet univers. ";
        redaction += "Cet arbre partira de la partition " + std::to_string(universe_partition_1) + " de l'univers. ";
        std::shared_ptr<Tree<std::string>> probability_tree = std::make_shared<Tree<std::string>>();
        Probability_Universe_Partition* used_partition = partitions().at(universe_partition_1).get();
        Probability_Condition condition;
        std::vector<std::shared_ptr<Tree<std::string>>> sub_trees = __tree(&condition, probability_tree, universe_partition_1, redaction);
        for(int i = 0;i<static_cast<int>(sub_trees.size());i++) {condition.events.push_back(used_partition->events.at(i));__tree(&condition, sub_trees.at(i), universe_partition_2, redaction);condition.events.pop_back();}
        probability_tree.get()->place_nodes();
        return probability_tree;
    }
    std::shared_ptr<Tree<std::string>> Probability_Universe::tree(int universe_partition, std::string& redaction) {
        // Start the redaction
        redaction += "Créeons un arbre pondéré avec cet univers. ";
        redaction += "Cet arbre partira de la partition " + std::to_string(universe_partition) + " de l'univers. ";
        std::shared_ptr<Tree<std::string>> probability_tree = std::make_shared<Tree<std::string>>();
        Probability_Universe_Partition* used_partition = partitions().at(universe_partition).get();
        Probability_Condition condition;
        std::vector<std::shared_ptr<Tree<std::string>>> sub_trees = __tree(&condition, probability_tree, universe_partition, redaction);
        for(int i = 0;i<static_cast<int>(sub_trees.size());i++) {condition.events.push_back(used_partition->events.at(i));__tree(&condition, sub_trees.at(i), universe_partition + 1, redaction);condition.events.pop_back();}
        probability_tree.get()->place_nodes();
        return probability_tree;
    }
}
