#include "contact_list.h"
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iostream>

namespace contact_list{
// TODO create implementation here!

/**
 * Given a contact storage, create a new contact entry by name and number.
 */

bool add(storage& contacts, std::string_view name, number_t number){
    if(std::find(contacts.names.begin(), contacts.names.end(), name) != contacts.names.end()){
        return false;
    }
    else{
        if (name.empty()){
            return false;
        }
        else{
            contacts.numbers.push_back(number);
            contacts.names.emplace_back(name);
            return true;
        }       
    }   
}


/**
 * Given a contact storage, how many contacts are currently stored?
 */
size_t size(const storage& contacts){
    return contacts.names.size();
}


/**
 * Fetch a contact number from storage given a name.
 */
contact_list::number_t get_number_by_name(storage& contacts, std::string_view name){
    //static_cast<std::string>(name);
    //std::vector<int>::iterator it = std::find(contacts.names.begin(), contacts.names.end(), name);
    int names_size = contacts.names.size();
    for(int i=0; i<names_size; i++){
        if(contacts.names[i] == name){
            return contacts.numbers.at(i);
        }
        else{
            continue;
        }
    }
    return -1;
}


/**
 * Return a string representing the contact list.
 */
std::string to_string(const storage& contacts){
    size_t store_size = contact_list::size(contacts);
    for(int i=0; i<store_size; i++){
        std::cout << contacts.names.at(i) <<  " - " << contacts.numbers.at(i) << "\n";
    }
}


/**
 * Remove a contact by name from the contact list.
 */
bool remove(storage& contacts, std::string_view name){
    //std::vector<int>::iterator it = std::find(contacts.names.begin(), contacts.names.end(), name);
    //if(it != contacts.names.end()){
    int names_size = contacts.names.size();
    std::vector<std::string> rname{};
    for(int i=0; i<names_size; i++){
        if(contacts.names[i] == name){
            contacts.numbers.erase(contacts.numbers.begin()+i);
            return true;
        }
        else{
            continue;
        }
    }
    return false;
}


/**
 * Sort the contact list in-place by name.
 */
void sort(storage& contacts){
    //bool compareFunction (std::string a, std::string b){
    //    return a<b;
    //   };
    std::vector<std::string> copy_names = contacts.names;
    std::vector<number_t> copy_numbers = contacts.numbers;
    std::sort(contacts.names.begin(), contacts.names.end(),[](const std::string & a, const std::string & b) -> bool
    {return a < b;});
    int names_size = contacts.names.size();
    for(int i=0; i<names_size; i++){
        std::string old = copy_names.at(i);
        for(int j=0; j<names_size; j++){
            if(contacts.names[j] == old){
                contacts.numbers[j] = copy_numbers[i];
            }
        }
    }
}


/**
 * Fetch a contact name from storage given a number.
 */
std::string get_name_by_number(storage& contacts, number_t number){
    int number_size = contacts.numbers.size();
    for(int i=0; i<number_size; i++){
        if(contacts.numbers[i] == number){
            return contacts.names[i];
        }
    }
    return "";
}
}