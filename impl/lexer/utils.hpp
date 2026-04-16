#include <array>
#include <cstdint>
#include <string_view>

// Assuming your enum is here
#include "../../include/lith.hpp" 

constexpr inline bool isSpace(const char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

struct TokenKey{
    std::string_view msg;
    LithTokens token;
}

// Helper to hash a string into a unique integer at compile time.
// This uses a simple FNV-1a style or similar rolling hash.
// Using 32-bit int is enough for your token set.
constexpr uint32_t hash(const char* str, size_t len) {
    uint32_t h = 2166136261u;
    for (size_t i = 0; i < len; ++i) {
        h ^= static_cast<uint32_t>(str[i]);
        h *= 16777619u;
    }
    return h;
}

constexpr TokenKey make(const char* msg, LithTokens token) {
    return {msg, token};
}

// Mapa stays mostly the same, but let's ensure it is clean
constexpr auto mapa = std::array{
    make("bit",    LithTokens::bit),
    make("bit8",   LithTokens::bit8),
    make("bit16",  LithTokens::bit16),
    make("bit32",  LithTokens::bit32),
    make("bit64",  LithTokens::bit64),
    make("bit128", LithTokens::bit128),
    make("bit256", LithTokens::bit256),
    make("void",   LithTokens::nothing),

    make("bring",  LithTokens::bring),
    make("send",   LithTokens::send),

    make("+",      LithTokens::plus),
    make("-",      LithTokens::minus),
    make("*",      LithTokens::star),
    make("/",      LithTokens::slash),
    make("!",      LithTokens::non),
    make("=",      LithTokens::assign),
    make(".",      LithTokens::dot),
    make("->",     LithTokens::arrow),
    make("<-",     LithTokens::unArrow),

    make("==",     LithTokens::equal),
    make("!=",     LithTokens::nonEqual),
    make("<=",     LithTokens::lessyr),
    make("<",      LithTokens::lesser),
    make(">=",     LithTokens::greaty),
    make(">",      LithTokens::greater),

    make("&",      LithTokens::both),
    make("|",      LithTokens::atLeast),
    make("^",      LithTokens::xOr),
    make("!|",     LithTokens::nOr),
    make("!&",     LithTokens::nAnd),
    make("!^",     LithTokens::xNor),

    make("+=",     LithTokens::addy),
    make("-=",     LithTokens::lessy),
    make("*=",     LithTokens::stary),
    make("/=",     LithTokens::slashy),
    make("&=",     LithTokens::andy),
    make("|=",     LithTokens::ory),
    make("^=",     LithTokens::xory),
    make("!|=",    LithTokens::nory),
    make("!&=",    LithTokens::nandy),
    make("!^=",    LithTokens::xnory),

    make("++",     LithTokens::incrementor),
    make("--",     LithTokens::decrementor),

    make("fork",   LithTokens::fork),
    make("merge",  LithTokens::merge),

    make("repeat", LithTokens::repeat),
    make("stop",   LithTokens::stop),
    make("if",     LithTokens::Conditional),
    make("throw",  LithTokens::Excpetion),
    make("goto",   LithTokens::togo),

    make("struct", LithTokens::structure),
    make("union",  LithTokens::unions),
    make("enum",   LithTokens::enumaration),
    make("class",  LithTokens::classy),

    make("macro",  LithTokens::macro)
};

constexpr auto maxTokenLen(){
    size_t i = 0;
    for (const auto I : mapa){
        if (I.msg.size() > i)
            i = I.msg.size();
    }
    return i;
}

// OPTIMIZED LOOKUP
constexpr auto lookKeyword(const char* src){
    // 1. Determine length first
    const char* start = src;
    size_t len = 0;
    while (src[len] != '\0' && !isSpace(src[len])) {
        // Add other terminator checks here if needed (e.g. operators)
        len++;
        if (len > maxTokenLen()) break; // Hard limit
    }

    if (len == 0) return LithTokens::identifier;

    // 2. Hash the input string
    uint32_t h = hash(start, len);

    // 3. Linear scan of the map comparing ONLY the hash
    // (This is much faster than string comparison)
    for (const auto& item : mapa) {
        if (item.msg.size() == len) { // Quick length check optimization
             // We can pre-hash the map at compile time, 
             // but for now, let's just compare the view or hash on the fly if map is small.
             // Since your map is small (58 items), re-hashing or comparing view is fine.
             // Let's do the string_view compare which is O(len) but only for matching lengths.
             
             // To be SUPER fast, we should pre-calculate hashes in the struct.
             // But for this snippet, let's stick to the simplest "Fast" logic:
             
             if (item.msg == std::string_view(start, len)) {
                 return item.token;
             }
        }
    }
    
    return LithTokens::identifier;
}