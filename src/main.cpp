#include <iostream>
#include <string>
#include "node.hpp"
#include "tree.hpp"
#include "tree_manip.hpp"
#include "xstrom.hpp"

using namespace strom;

int main(int argc, const char * argv[])
    {
    std::cout << "Starting..." << std::endl;
    TreeManip tm;
    std::string newick = std::string("(1:0.3,2:0.3,(3:0.2,(4:0.1,5:0.1):0.1):0.1)");
    std::cout << "Input: " << newick << std::endl;
    try
        {
        tm.buildFromNewick(newick, false, false);
        std::cout << "Output: " << tm.makeNewick(3) << std::endl;
        }
    catch (XStrom x)
        {
        std::cout << "Error: " << x.what() << std::endl;
        }
    std::cout << "\nFinished!" << std::endl;

    return 0;
    }
