#include <iostream>
#include <numeric>
#include <random>
#include <algorithm>
#include "sort_refactored.h"

/**
 * Example usage of the refactored sorting visualizer
 * Demonstrates clean separation of concerns and use of design patterns
 */
int main() {
    std::cout << "=== Sorting Algorithm Visualizer (Refactored) ===\n" << std::endl;
    
    // Create visualizer instance
    SortingVisualizer visualizer;
    
    // Example 1: Using Factory Pattern to create algorithm
    auto algorithm = SortingAlgorithmFactory::createAlgorithm(
        SortingAlgorithmFactory::AlgorithmType::QUICK
    );
    
    std::cout << "Visualizing: " << algorithm->getName() << std::endl;
    std::cout << "Close the window when done viewing." << std::endl;
    
    // Visualize the sorting process
    visualizer.visualize(*algorithm);
    
    // Display statistics
    std::cout << "\nStatistics:" << std::endl;
    std::cout << "  Comparisons: " << algorithm->getComparisonCount() << std::endl;
    std::cout << "  Swaps: " << algorithm->getSwapCount() << std::endl;
    
    // Example 2: Using different algorithms without visualization (benchmarking)
    std::cout << "\n=== Benchmarking All Algorithms ===\n" << std::endl;
    
    const std::vector<SortingAlgorithmFactory::AlgorithmType> algorithms = {
        SortingAlgorithmFactory::AlgorithmType::BUBBLE,
        SortingAlgorithmFactory::AlgorithmType::MERGE,
        SortingAlgorithmFactory::AlgorithmType::QUICK,
        SortingAlgorithmFactory::AlgorithmType::HEAP
    };
    
    for (auto type : algorithms) {
        auto algo = SortingAlgorithmFactory::createAlgorithm(type);
        
        // Generate test array
        std::vector<int> testArray(256);
        std::iota(testArray.begin(), testArray.end(), 1);
        std::shuffle(testArray.begin(), testArray.end(), 
                    std::default_random_engine(std::random_device{}()));
        
        // Sort without visualization (faster)
        algo->sort(testArray);
        
        // Display results
        std::cout << algo->getName() << ":" << std::endl;
        std::cout << "  Comparisons: " << algo->getComparisonCount() << std::endl;
        std::cout << "  Swaps: " << algo->getSwapCount() << std::endl;
        std::cout << std::endl;
    }
    
    return 0;
}
