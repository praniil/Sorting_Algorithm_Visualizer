#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <bits/stdc++.h>
#include <vector>
#include <random>
#include "bubbleSort.h"

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

int BubbleSort()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bubble sort algorithm");

    // random array generated
    std::vector<int> arr(400);
    std::iota(arr.begin(), arr.end(), 1);
    std::shuffle(arr.begin(), arr.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

    for (int i = 0; i < arr.size(); i++)
    {
        printf("%d\t", arr[i]);
    }

    for (int j = 0; j < arr.size(); ++j)
    {
        sf::RectangleShape rect(sf::Vector2f(5.f, arr[j]));
        rect.setFillColor(sf::Color::Cyan);
        rect.setPosition(j * 6.f, window.getSize().y - arr[j]);
        window.draw(rect);
    }
    window.display();

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