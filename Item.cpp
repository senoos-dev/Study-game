#include <iostream>
#include <vector>

#include "Item.hpp"

using namespace std;


Item::Item(int my_ID, int my_health_value, string my_name) {
    ID = my_ID;
    health_value = my_health_value;
    name = my_name;
}

int Item::get_health_value() const { return health_value; }
int Item::get_ID() const { return ID; }
string Item::get_name() const { return name; }

