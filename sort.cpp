#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <bits/stdc++.h>
#include <vector>
#include <random>
#include "sort.h"

void bubble_sort(sf::RenderWindow &window, std::vector<int> &arr)
{
    bool swapped;
    do
    {
        swapped = false;
        for (int i = 0; i < arr.size(); i++)
        {

            if (arr[i] > arr[i + 1])
            {
                int temp;
                temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swapped = true;
                window.clear();
                for (int j = 0; j < arr.size(); ++j)
                {
                    sf::RectangleShape rect(sf::Vector2f(5.f, arr[j]));
                    rect.setFillColor(sf::Color::Cyan);
                    rect.setPosition(j * 6.f, window.getSize().y - arr[j]);
                    window.draw(rect);
                }
                window.display();
            }
        }
    } while (swapped);
}

void Insert(std::vector<int> &array, int index)
{
    int i = index;
    int temp = array[i];
    while (i > 1 && temp > array[i / 2])
    {
        array[i] = array[i / 2];
        i = i / 2;
    }
    array[i] = temp;
}

int Delete(sf::RenderWindow &window, std::vector<int> &array, int length)
{
    int deleted = array[1];
    int temp = array[length];
    array[1] = temp;
    array[length] = deleted;
    int i = 1, j = 2 * i;
    while (j < length - 1)
    {
        if (array[j] < array[j + 1])
        {
            j = j + 1;
        }
        if (array[i] < array[j])
        {
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i = j;
            j = i * 2;
            window.clear();
            for (int j = 2; j < array.size() - 1; ++j)
            {
                sf::RectangleShape rect(sf::Vector2f(5.f, array[j]));
                rect.setFillColor(sf::Color::Cyan);
                rect.setPosition(j * 6.f, window.getSize().y - array[j]);
                window.draw(rect);
            }
            window.display();
        }
        else
        {
            break;
        }
    }
    return deleted;
}

void merge(sf::RenderWindow &window, std::vector<int> &A, int low, int mid, int high)
{
    int i = low, j = mid + 1, k = low;
    std::vector<int> B(high + 1);
    while (i <= mid && j <= high)
    {
        if (A[i] < A[j])
        {
            B[k++] = A[i++];
        }
        else
        {
            B[k++] = A[j++];
        }
    }
    for (; i <= mid; i++)
    {
        B[k++] = A[i];
    }
    for (; j <= high; j++)
    {
        B[k++] = A[j];
    }
    for (i = low; i <= high; i++)
    {
        A[i] = B[i];
    }
}

void iterativeMergeSort(sf::RenderWindow &window, std::vector<int> &array, int length)
{
    int pass, low, high, mid, i;
    for (pass = 2; pass <= length; pass = pass * 2)
    {
        for (i = 0; i + pass - 1 < length; i = i + pass)
        {
            low = i;
            high = i + pass - 1;
            mid = (low + high) / 2;
            merge(window, array, low, mid, high);
            window.clear();

            // Draw the array
            for (int j = 0; j < array.size(); ++j)
            {
                sf::RectangleShape rect(sf::Vector2f(5.f, array[j]));
                rect.setFillColor(sf::Color::Cyan);
                rect.setPosition(j * 6.f, window.getSize().y - array[j]);
                window.draw(rect);
            }
            window.display();
            sf::sleep(sf::milliseconds(50));
        }
        // Clear the window
        window.clear();

        // Draw the array
        for (int j = 0; j < array.size(); ++j)
        {
            sf::RectangleShape rect(sf::Vector2f(5.f, array[j]));
            rect.setFillColor(sf::Color::Cyan);
            rect.setPosition(j * 6.f, window.getSize().y - array[j]);
            window.draw(rect);
        }
        window.display();
        sf::sleep(sf::milliseconds(50));
    }
    if (pass / 2 < length)
    {
        merge(window, array, 0, pass / 2 - 1, length - 1);
        window.clear();
        for (int j = 0; j < array.size() - 1; ++j)
        {
            sf::RectangleShape rect(sf::Vector2f(5.f, array[j]));
            rect.setFillColor(sf::Color::Cyan);
            rect.setPosition(j * 6.f, window.getSize().y - array[j]);
            window.draw(rect);
        }

        // Display the window
        window.display();

        // Introduce a delay for visualization
        sf::sleep(sf::milliseconds(50));
    }
}

int Sort()
{
    sf::RenderWindow window(sf::VideoMode(1600, 800), "Merge sort algorithm");

    std::vector<int> arr(256);
    std::iota(arr.begin(), arr.end(), 1);
    std::shuffle(arr.begin(), arr.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

    // Draw the initial array
    for (int j = 0; j < arr.size(); ++j)
    {
        sf::RectangleShape rect(sf::Vector2f(5.f, arr[j]));
        rect.setFillColor(sf::Color::Cyan);
        rect.setPosition(j * 6.f, window.getSize().y - arr[j]);
        window.draw(rect);
    }
    window.display();

    // Start sorting
    iterativeMergeSort(window, arr, arr.size());

    // Keep the window open
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }

    return 0;
}
