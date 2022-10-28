#pragma once

#include <vector>
#include <string>
#include "immintrin.h"

namespace msimd {
    using std::vector;
    using std::string;

    class Tokenizer {
    public:
        static vector<string> tokenize(string token) {
            assert(token.length() > 0);

            if (token.length() > 8) return _tokenize(token, 8);
            else if (token.length() > 4) return _tokenize(token, 4);
            else if (token.length() > 2) return _tokenize(token, 2);
            else return _tokenize(token, 1);
        }

    private:
        static vector<string> _tokenize(string token, int subLength) {
            vector<string> substrings;

            for (int i=0; i<token.length()-subLength; i++) {
                substrings.push_back(token.substr(i, subLength));
            }
            return substrings;
        }

    };
}