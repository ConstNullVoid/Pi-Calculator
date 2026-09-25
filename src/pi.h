#pragma once
#include <cstdint>
#include <string>

bool calculate_pi(std::uint64_t digits, std::string& output);
long double estimate_file_size(std::uint64_t digits);
long double estimate_ram(std::uint64_t digits);