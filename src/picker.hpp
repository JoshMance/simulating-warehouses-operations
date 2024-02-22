#include <iostream>
#include <string.h>
#include "order.hpp"
#pragma once
using namespace std;

enum Status {no_order, has_order, completed_order};

class Picker {
    public:

    int id;
    int location;

    Order order;
    int order_index;
    Status order_status;

   Picker(int id, int location);
   void Accept_Order(Order new_order);
};