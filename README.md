# Sorting Algorithm Visualizer

An SFML-powered C++ application for watching sorting algorithms transform a shuffled sequence into sorted order. Each value is drawn as a vertical bar: the bar's height represents the value, and animation frames expose the mutations made by the algorithm.

The repository contains two implementations:

- **Default implementation** (`main.cpp`, `sort.cpp`) — the historical version built by the included Makefile. It currently launches the quick-sort visualization.
- **Refactored implementation** (`main_refactored.cpp`, `sort_refactored.*`) — a cleaner, extensible version with four interchangeable algorithms, highlighted operations, statistics, and a renderer separated from the sorting logic.

> The refactored implementation is the best starting point for development. It represents the intended architecture, but it is not yet wired into the repository's Makefile; use the separate build command below.

## What it looks like

The visualizer creates a `1600 × 800` desktop window and generates a shuffled permutation of the integers `1` through `256`.

- Each number becomes a `5 px`-wide vertical bar.
- Bars are spaced `6 px` apart and anchored to the bottom of the window.
- Cyan represents the array's normal state.
- In the refactored version, red marks the indices reported by the current algorithm update (for example, swapped indices or partition boundaries).
- Animation frames pause for `50 ms` by default.
- The final frame remains visible until the window is closed.

Because the input is a permutation, every bar has a unique height and a successful sort produces a smooth ascending staircase.

## Algorithms

The refactored implementation provides the following strategies:

| Algorithm | Best | Average | Worst | Extra space | Stable | Animation update |
| --- | ---: | ---: | ---: | ---: | :---: | --- |
| Bubble sort | `O(n)` | `O(n²)` | `O(n²)` | `O(1)` | Yes | After every swap |
| Bottom-up merge sort | `O(n log n)` | `O(n log n)` | `O(n log n)` | `O(n)` | Yes | After every merged run |
| Quick sort | `O(n log n)` | `O(n log n)` | `O(n²)` | `O(log n)` average, `O(n)` worst-case recursion | No | After every partition |
| Heap sort | `O(n log n)` | `O(n log n)` | `O(n log n)` | `O(log n)` recursion in this implementation | No | After every root extraction |

Quick sort uses the first element of each partition as its pivot. That keeps the partitioning code easy to follow, but makes already sorted or reverse-sorted data a worst-case input.

The statistics printed by the refactored program are implementation counters, not universal theoretical totals:

- comparisons are incremented at the explicit comparison sites in each strategy;
- swaps are counted only when the shared `swap` helper is used;
- merge sort copies values rather than swapping them, so its swap count remains zero.

## Requirements

- A C++ compiler with C++14 support or newer (`g++` or `clang++`)
- GNU Make for the default build
- **SFML 2.x** development headers and libraries:
  - `sfml-graphics`
  - `sfml-window`
  - `sfml-system`
  - `sfml-audio` (linked by the default Makefile, although audio is not currently used)
- A graphical desktop session; the application cannot display its window in a headless terminal without a virtual display

The source uses the SFML 2 API. SFML 3 introduced breaking API changes and is not a drop-in replacement for the current code.

### Install dependencies

Ubuntu/Debian:

```bash
sudo apt update
sudo apt install build-essential libsfml-dev
```

Fedora:

```bash
sudo dnf install gcc-c++ make SFML-devel
```

On other platforms, install an SFML 2.x development package and ensure its headers and libraries are visible to the compiler. See the [official SFML tutorials](https://www.sfml-dev.org/tutorials/2.6/) for platform-specific setup.

## Build and run

Clone the repository:

```bash
git clone https://github.com/praniil/Sorting_Algorithm_Visualizer.git
cd Sorting_Algorithm_Visualizer
```

### Option 1: Default build

The included Makefile compiles `main.cpp` and `sort.cpp` as C++11 and produces `algo_visualizer`:

```bash
make clean
make
./algo_visualizer
```

The active call in `sort.cpp` is:

```cpp
quickSort(window, arr, 0, arr.size() - 1);
```

Close the SFML window to exit. The program then prints `hello world` to the terminal.

### Option 2: Refactored build (recommended)

Compile the refactored entry point directly. It requires C++14 because the factory uses `std::make_unique`:

```bash
g++ -std=c++14 -Wall -Wextra -Wpedantic \
    main_refactored.cpp sort_refactored.cpp \
    -o algo_visualizer_refactored \
    -lsfml-graphics -lsfml-window -lsfml-system

./algo_visualizer_refactored
```

This executable:

1. opens a window and visualizes quick sort;
2. waits for the window to be closed;
3. prints the visualization's comparison and swap counts;
4. runs all four algorithms without animation on new shuffled arrays and prints their counts.

The final section is a lightweight operation-count comparison, not a timing benchmark. Each strategy receives a separately shuffled array, so totals are illustrative rather than a controlled comparison on identical input.

## Controls

There are currently no keyboard or mouse controls. Close the window using the window manager's close button when the animation is finished.

In the refactored version, window events are processed on every rendered frame. The historical implementation processes close events only after its active sort returns.

## Choose an algorithm

In `main_refactored.cpp`, change the factory argument:

```cpp
auto algorithm = SortingAlgorithmFactory::createAlgorithm(
    SortingAlgorithmFactory::AlgorithmType::QUICK
);
```

Available values are:

```cpp
AlgorithmType::BUBBLE
AlgorithmType::MERGE
AlgorithmType::QUICK
AlgorithmType::HEAP
```

Recompile after changing the selection. Bubble sort emits a frame for every swap, so visualizing all 256 values can take considerably longer than the other algorithms.

## Customize the visualization

The refactored version centralizes its main settings in `sort_refactored.h`:

```cpp
struct VisualizationConfig {
    static constexpr float BAR_WIDTH = 5.0f;
    static constexpr float BAR_SPACING = 6.0f;
    static constexpr int ANIMATION_DELAY_MS = 50;
    static constexpr int WINDOW_WIDTH = 1600;
    static constexpr int WINDOW_HEIGHT = 800;
    static constexpr int ARRAY_SIZE = 256;
};
```

Useful adjustments include:

- lower `ANIMATION_DELAY_MS` for faster playback;
- lower `ARRAY_SIZE` to make bubble sort practical to watch;
- change the colors in `SortingVisualizer::render`;
- keep `ARRAY_SIZE × BAR_SPACING` within `WINDOW_WIDTH` so every bar remains visible;
- keep generated values within `WINDOW_HEIGHT` so bars fit vertically.

## Architecture

The refactored implementation separates the application into four roles:

```text
                       main_refactored.cpp
                         │             │
                         ▼             ▼
SortingAlgorithmFactory ──creates──► strategy ◄── SortingVisualizer
                                      │                  ▲
                                      │ sorts and        │ renders with
                                      │ calls back       │ SFML
                                      └──────────────────┘
```

### Strategy interface

`SortingAlgorithm` defines the common `sort` and `getName` interface and owns the comparison/swap counters. `BubbleSort`, `MergeSort`, `QuickSort`, and `HeapSort` contain only algorithm behavior.

### Callback-based rendering

Algorithms accept an optional function with this shape:

```cpp
using VisualizationCallback =
    std::function<void(const std::vector<int>&, int, int)>;
```

The two indices identify the operation to highlight. When the callback is absent, the same algorithm runs without opening or depending on a renderer, which is how the console comparison is performed.

### Factory

`SortingAlgorithmFactory` maps an `AlgorithmType` to a `std::unique_ptr<SortingAlgorithm>`. Adding or selecting a strategy therefore does not require changing the visualizer.

## Project structure

```text
.
├── main.cpp               # Entry point used by make
├── sort.h                 # Declaration for the historical Sort() driver
├── sort.cpp               # Historical algorithms and direct SFML rendering
├── main_refactored.cpp    # Refactored demo and operation-count comparison
├── sort_refactored.h      # Interfaces, strategies, factory, and configuration
├── sort_refactored.cpp    # Refactored algorithms and SFML renderer
├── makefile               # Builds the historical implementation
└── README.md
```

The repository also currently tracks several object files and Linux executables from earlier builds. Rebuild from source on your own machine instead of relying on those binaries; native executables are platform- and library-version-specific.

## Current limitations and known rough edges

- Algorithm selection, speed, array size, pause, reset, and reshuffle are compile-time choices rather than UI controls.
- The Makefile builds only the historical implementation and specifies C++11; the refactored target is not integrated into it.
- The historical executable's window title says “Merge sort algorithm” even though quick sort is currently active.
- The historical quick-sort partition starts its right-hand scan at `high - 1`, leaving the element at `high` out of that scan. Some input orders can therefore finish unsorted; the refactored quick sort uses inclusive bounds correctly.
- Historical quick-sort update frames use the pivot index for every rectangle's geometry, so intermediate frames can appear as one repeated/collapsed bar. The refactored renderer draws each array index correctly.
- The historical source contains bubble sort, merge sort, and heap helper routines, but only quick sort is called by `Sort()`.
- Rendering and sorting run on the same thread, so long delays or dense animations can make interaction feel sluggish.
- The window and bar dimensions are fixed; resizing does not rescale the visualization.
- There is no automated test suite or command-line mode for verifying sorted output.
- The project does not currently include a license file.

## Extending the project

To add another algorithm to the refactored design:

1. derive a class from `SortingAlgorithm`;
2. implement `sort` and `getName`;
3. call `resetStatistics()` at the start of each run;
4. use `compare` and `swap` where their counters match the operation being performed;
5. invoke the optional callback at meaningful animation points;
6. add a value to `SortingAlgorithmFactory::AlgorithmType`;
7. construct the new strategy in `createAlgorithm`;
8. add it to the list in `main_refactored.cpp` if it should appear in the console comparison.

Good next improvements would be a single build system for both targets, runtime algorithm controls, shared input for fair comparisons, playback controls, responsive scaling, and correctness tests that exercise every strategy with empty, duplicate, sorted, reverse-sorted, and random inputs.

## Troubleshooting

### `SFML/Graphics.hpp: No such file or directory`

The SFML development headers are missing or are installed outside the compiler's include path. Install the development package, not only the runtime libraries.

### `cannot find -lsfml-graphics`

The linker cannot locate SFML. Install the SFML 2.x libraries or add their directory to your linker search path with `-L/path/to/lib`.

### `undefined reference to sf::...`

Ensure the SFML libraries appear **after** the source/object files in the compile command, as shown above.

### The window does not open on a remote or CI machine

SFML needs a display server. Run the executable in a graphical session or provide a virtual display such as Xvfb. Compilation itself does not require a display.

### The refactored build fails on `std::make_unique`

Compile with `-std=c++14` or newer. The provided Makefile's `-std=c++11` setting applies only to the historical implementation.

## Contributing

Issues and pull requests are welcome. Keep algorithm logic independent of SFML where possible, preserve the callback boundary, and include correctness tests when changing a sorting strategy.
