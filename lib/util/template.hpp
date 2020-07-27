#pragma once
#include "templates/includes.hpp"
#include "templates/alias.hpp"
#include "templates/var-func.hpp"
#include "templates/multi-dim-container.hpp"
#include "templates/init.hpp"

#define DEBUGGING
#ifdef DEBUGGING
#else
#define DEBUG(...)
#endif
