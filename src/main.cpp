#include "Application.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc == 4)
    {
        auto width = std::atoi(argv[1]);
        auto height = std::atoi(argv[2]);
        auto file = std::string(argv[3]);

        Application application(width, height, file);
        application.run();
    }
    else
    {
        if (argc != 1)
        {
            std::printf("==\nIncorrect arguments. Using defaults\n");
            std::printf("Usage: ./AStar [width] [height] [file]\n==\n\n");
        }

        Application application(600, 600, 50);
        application.run();
    }

    return 0;
}
