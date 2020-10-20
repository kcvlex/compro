#pragma once

template <typename T>
T ceil_div(T a, T b) { return a / b + !!(a % b); }
