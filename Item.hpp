#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
using namespace std;

class Item {
public:
    Item(int my_ID, int my_health_value, string my_name);
    virtual ~Item() = default;
    
    int get_health_value() const;
    int get_ID() const;
    string get_name() const;

private:
    int ID;
    int health_value;
    string name;
};

#endif