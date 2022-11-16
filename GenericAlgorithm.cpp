#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>

float foo(float _x, float _y, float _z)
{
    return 6.0f * std::pow(_x, 3.0f) + 9.0f * std::pow(_y, 2.0f) + 90.0f * _z - 25.0f;
}

float fitness(float _x, float _y, float _z)
{
    float answer = foo(_x, _y, _z);

    if (answer == 0.0f)
        return 99999.0f;
    else
        return std::abs(1 / answer);
}

int main()
{
    // setup for the random uniform generator
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0f, 10'000.0f);

    // Generate random solutions
    std::vector<std::tuple<float, float, float>> solutions {};
    for (int i = 0; i < 1000; i++)
        solutions.push_back(std::make_tuple(distribution(generator), distribution(generator), distribution(generator)));
    

    for (int i = 0; i < 10'000; i++)
    {
        std::vector<std::tuple<float, std::tuple<float, float, float>>> rankedSolutions {};
        for (auto& solution : solutions)
            rankedSolutions.push_back(std::make_tuple(fitness(std::get<0> (solution), std::get<1> (solution), std::get<2> (solution)), solution));
        
        std::sort(rankedSolutions.rbegin(), rankedSolutions.rend());
        
        std::printf("=== Generation %d best solution ===\n", i);
        std::printf("(%f, (%f, %f, %f))\n", std::get<0> (rankedSolutions.at(0)), 
                                          std::get<0> (std::get<1> (rankedSolutions.at(0))),
                                          std::get<1> (std::get<1> (rankedSolutions.at(0))),
                                          std::get<2> (std::get<1> (rankedSolutions.at(0))));

        if (std::get<0> (rankedSolutions.at(0)) > 999)
        {   
            std::cout << "\nWe expect the output to be 0 \nThe actual answer is:" << std::endl;
            float answer = foo(std::get<0> (std::get<1> (rankedSolutions.at(0))), 
                               std::get<1> (std::get<1> (rankedSolutions.at(0))), 
                               std::get<2> (std::get<1> (rankedSolutions.at(0))));
            std::cout << answer << ", if we make a int value from the answer we get: " << static_cast<int>(answer) << std::endl;
            break;
        }

        std::vector<std::tuple<float, std::tuple<float, float, float>>> bestSolutions = rankedSolutions;
        bestSolutions.resize(100);

        std::vector<float> elements {};
        for (auto& bs : bestSolutions)
        {
            elements.push_back(std::get<0> (std::get<1> (bs)));
            elements.push_back(std::get<1> (std::get<1> (bs)));
            elements.push_back(std::get<2> (std::get<1> (bs)));
        }


        std::uniform_real_distribution<float> distribution(0.99f, 1.01f);
        std::vector<std::tuple<float, float, float>> newGeneration {};
        for (int i = 0; i < 10'000; i++)
        {
            float element1 = elements.at(std::rand() % elements.size()) * distribution(generator);
            float element2 = elements.at(std::rand() % elements.size()) * distribution(generator);
            float element3 = elements.at(std::rand() % elements.size()) * distribution(generator);

            newGeneration.push_back(std::make_tuple(element1, element2, element3));
        }

        solutions = newGeneration;
    }
        
    return 0;
}