#include "picker.hpp"

Picker::Picker(int id, int location) {
    id = id;
    location = location;
    order_index = 0;
    order_status = Status::no_order;
}

void Picker::Accept_Order(Order new_order) {
    order = new_order;
    order_index = 0;
    order_status = Status::has_order;
};