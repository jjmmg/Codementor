/*
Morse Code

Morse Code is delivered in a series signals which are referred to as dashes (-) or dots (.). To keep things simple for the purposes of this challenge we'll only decode letters with a maximum length of three signals.

Morse Code Graph
Here is the Morse Code dichotomic search table courtesy of Wikipedia

Morse Code Examples
-.- translates to K
... translates to S
.- translates to A
-- translates to M
. translates to E

Background
You've started work as morse code translator. Unfortunately some of the signals aren't as distinguishable as others and there are times where a . seems indistinguishable from -. In these cases you write down a ? so that you can figure out what all the posibilities of that letter for that word are later.

Task
Write a function possibilities that will take a string word and return an array of possible characters that the morse code word could represent.

Examples with ?
? should return ['E','T']
?. should return ['I','N']
.? should return ['I','A']
?-? should return ['R','W','G','O']
*/

#include <iostream>
#include <tuple>
#include <list>
#include <vector>
#include <string>
#include <regex>

////////////////////////////////////////////////////////////////////////////////
// Find all ocurrences of a  subtring in a string and replace them by another one.
void find_and_replace(std::string& str,
                      const std::string& find,
                      const std::string& replace)
{
    std::string::size_type i=0;
    while ((i=str.find(find, i)) != std::string::npos)
    {
        str.replace(i, find.length(), replace);
        i += replace.length();
    }
}

std::list<std::string> possibilities(std::string word)
{
    // ??- is treated as a trigraph. I could see it generating a compilation error, which shows also compilation warnings:
    // -isystem /runner/frameworks/cpp/  warning: trigraph converted to '~' character [-Wtrigraphs]
    // To fix this I'll escape the question marks for the only trigraph that can be received: ??-
    if (word == "??-")
        word = "\?\?-";

    // Morse code dictionary, following the tree order
    std::vector<std::tuple<std::string, std::string>> morse =
    {
        {"E","."},
        {"T","-"},
        {"I",".."},
        {"A",".-"},
        {"N","-."},
        {"M","--"},
        {"S","..."},
        {"U","..-"},
        {"R",".-."},
        {"W",".--"},
        {"D","-.."},
        {"K","-.-"},
        {"G","--."},
        {"O","---"},
    };

    // Create regular expression, replacing '.' for '\.',since the dot is
    // a special character, and ? for a dot or dash choice
    find_and_replace(word, "?", "[.-]");
    find_and_replace(word, ".", "\\.");
    std::regex regex(word);

    std::list<std::string> result;
    // Iterate over the morse dictionary, saving those matching the pattern
    for (const auto& e : morse)
    {
        if (std::regex_match(std::get<1>(e), regex))
            result.push_back(std::get<0>(e));
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
// Not needed in assessment
int main() {
    auto res = possibilities("??-");
    for (const auto& e : res)
        std::cout << e << " ";
}
