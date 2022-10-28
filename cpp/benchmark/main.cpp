#include <iostream>
#include <string>
#include <vector>
#include "immintrin.h"

#include "msimd.hpp"



int main() {
    using std::vector;
    using std::cout;
    using std::string;

    vector<string> a  = msimd::Tokenizer::tokenize("meylis");
    
    for (int i=0; i<a.size(); i++)
        cout << a.at(i) << std::endl;
    return 0;
}