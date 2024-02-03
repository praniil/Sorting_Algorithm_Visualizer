#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <bits/stdc++.h>
#include <vector>
#include <random>
#include "bubbleSort.h"

void bubble_sort(sf::RenderWindow &window, std::vector<int> &arr)
{
}

int BubbleSort()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bubble sort algorithm");

    // random array generated
    std::vector<int> arr(800);
    std::iota(arr.begin(), arr.end(), 1);
    std::shuffle(arr.begin(), arr.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

    for (int i = 0; i < arr.size(); i++)
    {
        printf("%d\t", arr[i]);
    }

    bubble_sort(window, arr);

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