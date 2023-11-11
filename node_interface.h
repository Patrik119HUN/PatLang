#pragma once

#include "visitor_interface.h"
struct INode {
    virtual void accept(Visitor &visitor) = 0;
};