#include "tokenizer.h"
#include "../../include/lith.hpp"
#include "utils.hpp"
#include <vector>
#include <cstddef>

auto tokenize(const char *src, const size_t size){

    std::vector<LithTokens> tokens;
    tokens.reserve(size / 3);

    for (auto end = src + size; src < end; src++){
        if (isSpace(src))
            continue;
        

    }
    
}