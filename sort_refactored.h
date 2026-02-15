#ifndef SORT_REFACTORED_H
#define SORT_REFACTORED_H

#include <vector>
#include <functional>
#include <cstddef>
#include <memory>

// Forward declaration
namespace sf {
    class RenderWindow;
}

/**
 * Configuration constants for visualization
 */
struct VisualizationConfig {
    static constexpr float BAR_WIDTH = 5.0f;
    static constexpr float BAR_SPACING = 6.0f;
    static constexpr int ANIMATION_DELAY_MS = 50;
    static constexpr int WINDOW_WIDTH = 1600;
    static constexpr int WINDOW_HEIGHT = 800;
    static constexpr int ARRAY_SIZE = 256;
};

/**
 * Callback type for visualization updates
 * Parameters: array reference, optional indices being compared/swapped
 */
using VisualizationCallback = std::function<void(const std::vector<int>&, int, int)>;

/**
 * Abstract base class for sorting algorithms
 * Implements Strategy Pattern for algorithm selection
 */
class SortingAlgorithm {
public:
    virtual ~SortingAlgorithm() = default;
    
    /**
     * Pure virtual function to be implemented by concrete sorting algorithms
     * @param arr Array to be sorted
     * @param callback Optional callback for visualization
     */
    virtual void sort(std::vector<int>& arr, VisualizationCallback callback = nullptr) = 0;
    
    /**
     * Get the name of the sorting algorithm
     */
    virtual const char* getName() const = 0;
    
    /**
     * Get comparison count (for analysis)
     */
    size_t getComparisonCount() const { return comparisonCount; }
    
    /**
     * Get swap count (for analysis)
     */
    size_t getSwapCount() const { return swapCount; }
    
    /**
     * Reset statistics
     */
    void resetStatistics() {
        comparisonCount = 0;
        swapCount = 0;
    }

protected:
    size_t comparisonCount = 0;
    size_t swapCount = 0;
    
    /**
     * Helper method to swap elements and update statistics
     */
    void swap(std::vector<int>& arr, int i, int j) {
        std::swap(arr[i], arr[j]);
        swapCount++;
    }
    
    /**
     * Helper method to compare elements and update statistics
     */
    bool compare(const std::vector<int>& arr, int i, int j) {
        comparisonCount++;
        return arr[i] > arr[j];
    }
};

/**
 * Bubble Sort Implementation - O(n²)
 */
class BubbleSort : public SortingAlgorithm {
public:
    void sort(std::vector<int>& arr, VisualizationCallback callback = nullptr) override;
    const char* getName() const override { return "Bubble Sort"; }
};

/**
 * Merge Sort Implementation - O(n log n)
 */
class MergeSort : public SortingAlgorithm {
public:
    void sort(std::vector<int>& arr, VisualizationCallback callback = nullptr) override;
    const char* getName() const override { return "Merge Sort"; }
    
private:
    void merge(std::vector<int>& arr, int low, int mid, int high);
    void iterativeMergeSort(std::vector<int>& arr, VisualizationCallback callback);
};

/**
 * Quick Sort Implementation - O(n log n) average
 */
class QuickSort : public SortingAlgorithm {
public:
    void sort(std::vector<int>& arr, VisualizationCallback callback = nullptr) override;
    const char* getName() const override { return "Quick Sort"; }
    
private:
    int partition(std::vector<int>& arr, int low, int high);
    void quickSortRecursive(std::vector<int>& arr, int low, int high, VisualizationCallback callback);
};

/**
 * Heap Sort Implementation - O(n log n)
 * Demonstrates heap data structure
 */
class HeapSort : public SortingAlgorithm {
public:
    void sort(std::vector<int>& arr, VisualizationCallback callback = nullptr) override;
    const char* getName() const override { return "Heap Sort"; }
    
private:
    void heapify(std::vector<int>& arr, int n, int i);
    void buildMaxHeap(std::vector<int>& arr);
};

/**
 * Visualizer class - Handles all rendering logic
 * Separates visualization concerns from algorithm logic
 */
class SortingVisualizer {
public:
    SortingVisualizer();
    ~SortingVisualizer();
    
    /**
     * Run visualization with specified sorting algorithm
     * @param algorithm Sorting algorithm to visualize
     */
    void visualize(SortingAlgorithm& algorithm);
    
    /**
     * Render the current state of the array
     * @param arr Array to render
     * @param highlightIndex1 First index to highlight (-1 for none)
     * @param highlightIndex2 Second index to highlight (-1 for none)
     */
    void render(const std::vector<int>& arr, int highlightIndex1 = -1, int highlightIndex2 = -1);
    
    /**
     * Initialize array with random values
     */
    std::vector<int> generateRandomArray(int size);
    
private:
    sf::RenderWindow* window;
    
    void createWindow();
    void handleEvents();
    bool isWindowOpen() const;
};

/**
 * Factory class for creating sorting algorithms
 * Implements Factory Pattern
 */
class SortingAlgorithmFactory {
public:
    enum class AlgorithmType {
        BUBBLE,
        MERGE,
        QUICK,
        HEAP
    };
    
    /**
     * Create a sorting algorithm instance
     * @param type Type of algorithm to create
     * @return Unique pointer to the algorithm
     */
    static std::unique_ptr<SortingAlgorithm> createAlgorithm(AlgorithmType type);
};

#endif // SORT_REFACTORED_H
