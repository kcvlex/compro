#pragma once
#include "includes.hpp"

namespace init__ { 
struct InitIO { 
    InitIO() { 
        std::cin.tie(nullptr); 
        std::ios_base::sync_with_stdio(false); 
        std::cout << std::fixed << std::setprecision(30); 
    } 
} init_io; 
}
