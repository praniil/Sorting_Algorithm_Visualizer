#include "sort_refactored.h"
#include "SFML/Graphics.hpp"
#include <algorithm>
#include <random>
#include <chrono>
#include <memory>

// BubbleSort Implementation

void BubbleSort::sort(std::vector<int>& arr, VisualizationCallback callback) {
    resetStatistics();
    const size_t n = arr.size();
    bool swapped;
    
    do {
        swapped = false;
        // Fixed: prevent out-of-bounds access (i < n-1 instead of i < n)
        for (size_t i = 0; i < n - 1; i++) {
            if (compare(arr, i, i + 1)) {
                swap(arr, i, i + 1);
                swapped = true;
                
                // Trigger visualization callback if provided
                if (callback) {
                    callback(arr, i, i + 1);
                }
            }
        }
    } while (swapped);
}

// MergeSort Implementation

void MergeSort::merge(std::vector<int>& arr, int low, int mid, int high) {
    std::vector<int> temp(high - low + 1);
    int i = low, j = mid + 1, k = 0;
    
    // Merge the two sorted halves
    while (i <= mid && j <= high) {
        comparisonCount++;
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    
    // Copy remaining elements from left half
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    
    // Copy remaining elements from right half
    while (j <= high) {
        temp[k++] = arr[j++];
    }
    
    // Copy merged array back to original
    for (i = low, k = 0; i <= high; i++, k++) {
        arr[i] = temp[k];
    }
}

void MergeSort::iterativeMergeSort(std::vector<int>& arr, VisualizationCallback callback) {
    const size_t n = arr.size();
    
    // Perform merge sort in bottom-up manner
    for (int size = 1; size < static_cast<int>(n); size *= 2) {
        for (int leftStart = 0; leftStart < static_cast<int>(n) - 1; leftStart += 2 * size) {
            int mid = std::min(leftStart + size - 1, static_cast<int>(n) - 1);
            int rightEnd = std::min(leftStart + 2 * size - 1, static_cast<int>(n) - 1);
            
            merge(arr, leftStart, mid, rightEnd);
            
            // Trigger visualization callback if provided
            if (callback) {
                callback(arr, leftStart, rightEnd);
            }
        }
    }
}

void MergeSort::sort(std::vector<int>& arr, VisualizationCallback callback) {
    resetStatistics();
    iterativeMergeSort(arr, callback);
}

// QuickSort Implementation

int QuickSort::partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[low];
    int i = low;
    int j = high;
    
    while (i < j) {
        // Find element greater than pivot from left
        while (i < high && arr[i] <= pivot) {
            i++;
            comparisonCount++;
        }
        
        // Find element smaller than or equal to pivot from right
        while (j > low && arr[j] > pivot) {
            j--;
            comparisonCount++;
        }
        
        // Swap if indices haven't crossed
        if (i < j) {
            swap(arr, i, j);
        }
    }
    
    // Place pivot in correct position
    // Fixed: correctly swap pivot with element at position j
    swap(arr, low, j);
    
    return j;
}

void QuickSort::quickSortRecursive(std::vector<int>& arr, int low, int high, VisualizationCallback callback) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        
        // Trigger visualization callback if provided
        if (callback) {
            callback(arr, low, high);
        }
        
        // Recursively sort left and right partitions
        quickSortRecursive(arr, low, pivotIndex - 1, callback);
        quickSortRecursive(arr, pivotIndex + 1, high, callback);
    }
}

void QuickSort::sort(std::vector<int>& arr, VisualizationCallback callback) {
    resetStatistics();
    if (!arr.empty()) {
        quickSortRecursive(arr, 0, arr.size() - 1, callback);
    }
}

// HeapSort Implementation

void HeapSort::heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    // Check if left child is larger than root
    if (left < n) {
        comparisonCount++;
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }
    
    // Check if right child is larger than current largest
    if (right < n) {
        comparisonCount++;
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }
    
    // If largest is not root, swap and continue heapifying
    if (largest != i) {
        swap(arr, i, largest);
        heapify(arr, n, largest);
    }
}

void HeapSort::buildMaxHeap(std::vector<int>& arr) {
    const size_t n = arr.size();
    
    // Build heap (rearrange array)
    // Start from last non-leaf node and heapify each node
    for (int i = static_cast<int>(n) / 2 - 1; i >= 0; i--) {
        heapify(arr, static_cast<int>(n), i);
    }
}

void HeapSort::sort(std::vector<int>& arr, VisualizationCallback callback) {
    resetStatistics();
    const size_t n = arr.size();
    
    // Build initial max heap
    buildMaxHeap(arr);
    
    // Extract elements from heap one by one
    for (int i = static_cast<int>(n) - 1; i > 0; i--) {
        // Move current root to end
        swap(arr, 0, i);
        
        // Trigger visualization callback if provided
        if (callback) {
            callback(arr, 0, i);
        }
        
        // Heapify the reduced heap
        heapify(arr, i, 0);
    }
}

// Visualizer Implementation

SortingVisualizer::SortingVisualizer() : window(nullptr) {
    createWindow();
}

SortingVisualizer::~SortingVisualizer() {
    delete window;
}

void SortingVisualizer::createWindow() {
    window = new sf::RenderWindow(
        sf::VideoMode(VisualizationConfig::WINDOW_WIDTH, VisualizationConfig::WINDOW_HEIGHT),
        "Sorting Algorithm Visualizer"
    );
}

void SortingVisualizer::handleEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
    }
}

bool SortingVisualizer::isWindowOpen() const {
    return window && window->isOpen();
}

void SortingVisualizer::render(const std::vector<int>& arr, int highlightIndex1, int highlightIndex2) {
    if (!window) return;
    
    handleEvents();
    window->clear(sf::Color::Black);
    
    // Draw each element as a vertical bar
    for (size_t i = 0; i < arr.size(); ++i) {
        sf::RectangleShape bar(sf::Vector2f(
            VisualizationConfig::BAR_WIDTH,
            static_cast<float>(arr[i])
        ));
        
        // Color coding: highlight compared/swapped elements
        if (static_cast<int>(i) == highlightIndex1 || static_cast<int>(i) == highlightIndex2) {
            bar.setFillColor(sf::Color::Red);  // Highlight active comparisons
        } else {
            bar.setFillColor(sf::Color::Cyan);  // Normal color
        }
        
        // Position bar from bottom of window
        bar.setPosition(
            i * VisualizationConfig::BAR_SPACING,
            window->getSize().y - arr[i]
        );
        
        window->draw(bar);
    }
    
    window->display();
    sf::sleep(sf::milliseconds(VisualizationConfig::ANIMATION_DELAY_MS));
}

std::vector<int> SortingVisualizer::generateRandomArray(int size) {
    std::vector<int> arr(size);
    
    // Fill with sequential values (1 to size)
    std::iota(arr.begin(), arr.end(), 1);
    
    // Shuffle to create random order
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(arr.begin(), arr.end(), std::default_random_engine(seed));
    
    return arr;
}

void SortingVisualizer::visualize(SortingAlgorithm& algorithm) {
    // Generate random array
    auto arr = generateRandomArray(VisualizationConfig::ARRAY_SIZE);
    
    // Display initial state
    render(arr);
    
    // Create visualization callback
    auto callback = [this](const std::vector<int>& arr, int idx1, int idx2) {
        this->render(arr, idx1, idx2);
    };
    
    // Perform sorting with visualization
    algorithm.sort(arr, callback);
    
    // Display final sorted state
    render(arr);
    
    // Keep window open until user closes it
    while (isWindowOpen()) {
        handleEvents();
    }
}

// Factory Implementation

std::unique_ptr<SortingAlgorithm> SortingAlgorithmFactory::createAlgorithm(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::BUBBLE:
            return std::make_unique<BubbleSort>();
        case AlgorithmType::MERGE:
            return std::make_unique<MergeSort>();
        case AlgorithmType::QUICK:
            return std::make_unique<QuickSort>();
        case AlgorithmType::HEAP:
            return std::make_unique<HeapSort>();
        default:
            return nullptr;
    }
}
