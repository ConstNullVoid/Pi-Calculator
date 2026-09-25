#include "pi.h"
#include <print>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include <cstdint>

using Clock = std::chrono::steady_clock;

static std::string format_duration(Clock::duration duration) {
    double seconds = std::chrono::duration<double>(duration).count();
    if (seconds < 1.0)
        return std::format("{:.3f} ms", seconds * 1000.0);
    if (seconds < 60.0)
        return std::format("{:.3f} seconds", seconds);
    double minutes = seconds / 60.0;
    if (minutes < 60.0)
        return std::format("{:.3f} minutes", minutes);
    double hours = minutes / 60.0;
    if (hours < 24.0)
        return std::format("{:.3f} hours", hours);
    return std::format("{:.3f} days", hours / 24.0);
}

static std::string format_gb(long double bytes) {
    return std::format("{:.9f} GB", static_cast<double>(bytes / 1000000000.0L));
}

static bool parse_digits(const std::string& input, std::uint64_t& digits) {
    if (input.empty())
        return false;
    for (char c : input)
        if (c < '0' || c > '9')
            return false;
    try {
        unsigned long long value = std::stoull(input);
        if (value == 0)
            return false;
        digits = value;
        return true;
    } catch (...) {
        return false;
    }
}

int main() {
    std::println("Pi Calculator");
    std::println();
    std::uint64_t digits = 0;
    while (true) {
        std::print("How many digits to calculate? ");
        std::string input;
        std::getline(std::cin, input);
        if (parse_digits(input, digits))
            break;
        std::println("Please enter a positive whole number.");
    }
    std::println();
    std::println("Estimated file size: {}", format_gb(estimate_file_size(digits)));
    std::println("Estimated RAM required: {}", format_gb(estimate_ram(digits)));
    std::println();
    while (true) {
        std::print("Continue? (y/n): ");
        std::string answer;
        std::getline(std::cin, answer);
        if (answer == "y" || answer == "Y")
            break;
        if (answer == "n" || answer == "N")
            return 0;
        std::println("Please enter y or n.");
    }
    std::println();
    std::println("Calculating Pi to {} digits...", digits);
    auto calculation_start = Clock::now();
    std::string pi;
    if (!calculate_pi(digits, pi)) {
        std::println("Calculation failed.");
        return 1;
    }
    auto calculation_end = Clock::now();
    std::println("Calculation complete in {}.", format_duration(calculation_end - calculation_start));
    std::println("Writing pi.txt...");
    auto writing_start = Clock::now();
    std::ofstream file("pi.txt", std::ios::binary);
    if (!file) {
        std::println("Failed to open pi.txt for writing.");
        return 1;
    }
    file.write(pi.data(), static_cast<std::streamsize>(pi.size()));
    file.close();
    if (!file) {
        std::println("Failed while writing pi.txt.");
        return 1;
    }
    auto writing_end = Clock::now();
    std::println("Writing pi.txt complete in {}.", format_duration(writing_end - writing_start));
    std::println();
    std::println("Pi calculation finished successfully.");
    return 0;
}