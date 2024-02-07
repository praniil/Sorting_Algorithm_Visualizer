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

int Sort()
{
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Bubble sort algorithm");
    // window.create(sf::VideoMode::getFullscreenModes()[0], "SFML Fullscreen Example", sf::Style::Fullscreen);

    // random array generated
    std::vector<int> arr(200);
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

    // bubble_sort(window, arr);
    for (int i = 2; i <= arr.size() - 1; i++)
    {
        Insert(arr, i);
    }
    for (int i = arr.size() - 1; i > 1; i--)
    {
        Delete(window, arr, i);
    }

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