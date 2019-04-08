/*******************************************************************************
 * Creator: Hemajun
 * Description: Entry of md-transformer.
*******************************************************************************/
#include <iostream>
#include "md-transformer.hpp"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <in> <out>" << std::endl;
    }
    else
    {
        std::cout << "Input file: " << argv[1]
                  << std::endl
                  << "Output file: " << argv[2]
                  << std::endl
                  << "Transforming..."
                  << std::endl
                  << (new hmj::MDTransformer(argv[1], argv[2]))->transform()
                  << std::endl;
    }
    return 0;
}
