# Graph Operator

The **Graph Operator** is an automation tool that collectivizes common graph manipulations and functions for a user-inputted connected graph of any scale or size. It outputs the results of common counting, MST, and Shortest Path operations and algorithms in aid of any user looking to solve or operate on a graph.

---

## Quick Download (Recommended)

For casual users, this only downloads an executable app for easy access, suited to your OS. This format removes the tedium of extra installations, configurations, or compilations.

***Important Note:*** For some systems with auto-protection or malware scanners, the executable may be flagged as unknown. For users to continue, simply bypass (ex. clicking "Run Anyway"). The source code is all in this repository and I assure all that this project has no harmful intentions.

[![Windows](https://img.shields.io/badge/Download-Windows-blue?style=for-the-badge&logo=windows)](https://github.com/hgd-dev/Graph-Operator/actions/runs/17212814688/artifacts/3844737471)
[![macOS](https://img.shields.io/badge/Download-macOS-lightgrey?style=for-the-badge&logo=apple)](https://github.com/hgd-dev/Graph-Operator/actions/runs/17212814688/artifacts/3844729160)
[![Linux](https://img.shields.io/badge/Download-Linux-yellow?style=for-the-badge&logo=linux)](https://github.com/hgd-dev/Graph-Operator/actions/runs/17212814688/artifacts/3844727694)

***Caveat for all runs:*** This project has automatic input fail-safe detectors to ensure smooth running and correct input of data. While it is built-in to catch typos or mistaken keystrokes, please refrain from deliberately attempting to break the program or use corrupted data. Undefined behavior can have unintended consequences.

---

## Project Structure

```
Graph-Operator/
├── .github/ # GitHub Actions workflows
│ └── workflows/
│ └── build.yml
├── CMakeLists.txt # Build configuration for CMake
├── input_functions.cpp # Input source file
├── mst_functions.cpp # MST source file
├── compute_functions.cpp # Compute source file
├── shortest_path_functions.cpp # Shortest path source file
└── graph_operator.cpp # Single-file version of the project
```

---

## Features

- From user-inputted connected graphs of any magnitude, it can display the current graph; input or manipulate its various parts; count vertices, edges, paths, degrees, etc; construct a Minimum or Maximum Spanning Tree (MST) using algorithms; and compute shortest paths using algorithms.
- Outputs every result clearly, with algorithmic calculations or manipulations if applicable, in the terminal.  
- Cross-platform support: Windows, macOS, Linux.  
- Includes both multi-file CMake project and a single-file `graph_operator.cpp` version.  

---

## Build and Run Instructions (From project source code)

### **Requirements**

| Component                       | Version / Notes                          |
|---------------------------------|-----------------------------------------|
| C++ Compiler                     | C++17 compatible                         |
| CMake                            | 3.10+                                    |
| Operating System                 | Windows, macOS, Linux                    |

---

### **1. Multi-file Project (CMake)**

**Build:**

```bash
git clone https://github.com/hgd-dev/MST-Builder.git
cd MST-Builder

mkdir build && cd build
cmake ..
make
```

**Run:**

```
./GraphOperator
```

### **2. Single-file Version (graph_operator.cpp)**

**Compile:**

```bash
# Linux/macOS
g++ -std=c++17 graph_operator.cpp -o GraphOperator

# Windows (MinGW example)
g++ -std=c++17 graph_operator.cpp -o GraphOperator.exe
```

**Run:**

```bash
./GraphOperator   # or NewtonPolygonizer.exe on Windows

```

