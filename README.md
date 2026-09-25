# Pi Calculator

[![C++](https://img.shields.io/badge/language-C%2B%2B23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![GMP](https://img.shields.io/badge/dependency-GMP-orange.svg)](https://gmplib.org/)
[![FLINT](https://img.shields.io/badge/dependency-FLINT-brightgreen.svg)](https://www.flintlib.org/)

A high-performance command-line application that computes the mathematical constant **Pi (\(\pi\)) to an arbitrary number of digits**. It utilizes mathematical split-algorithms and low-level big-integer libraries to maximize computing throughput.

## Key Features

*   **Chudnovsky Algorithm:** Utilizes the fastest known algorithm for calculating digits of Pi, yielding roughly 14 decimal places per iteration term.
*   **Binary Splitting Optimization:** Accelerates the calculation by dividing the series factorial components into a balanced evaluation tree via `fmpz_t`.
*   **Resource Pre-Flight Estimation:** Evaluates targeted digit thresholds before processing to safely forecast memory utilization and output file footprint.
*   **Modern C++ Design:** Built around structural C++23 standards featuring native time benchmarking via `<chrono>` and clean text streams with `<print>`.

## Requirements

To execute this software, ensure you have the following system dependencies installed and configured within your runtime environment path:

*   **C++ Standard Library:** A modern runtime supporting **C++23** syntax (e.g., GCC 13+, Clang 16+, or MSVC 2022+).
*   **GMP Library:** GNU Multiple Precision Arithmetic Library for final floating-point division and square root evaluation.
*   **FLINT Library:** Fast Library for Number Theory for handling fast arbitrary-precision integer binary splitting operations.

## Project Architecture

The codebase splits concerns across calculation mechanics and the consumer CLI interface:

*   `main.cpp`: Manages the terminal user interface lifecycle, monitors time-to-compute, benchmarks file storage execution, and outputs telemetry data.
*   `pi.cpp`: Contains the operational arithmetic, memory-limb equations, and core calculation pipeline.
*   `pi.h`: Exposes the top-level calculations interfaces and tracking signatures.

## Usage Example

Once executing, the interactive console program safely steps you through your system resource thresholds before writing results:

```text
Pi Calculator

How many digits to calculate? 1000000

Estimated file size: 0.001000003 GB
Estimated RAM required: 0.071283921 GB

Continue? (y/n): y

Calculating Pi to 1000000 digits...
Calculation complete in 241.380 ms.
Writing pi.txt...
Writing pi.txt complete in 12.114 ms.

Pi calculation finished successfully.
```

### Result Data
The calculated string matches modern parsing metrics and automatically writes out formatted string arrays directly to a binary stream buffer named `pi.txt`:

```text
3.14159265358979323846264338327950288419716939937510582097494459230781640628620899...
```