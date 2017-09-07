/*
Sort the columns of a csv-file
You get a string with the content of a csv-file.

CSV Description:
The columns are separated by commas (,).
The rows are separated by a single newline (\n)
The first line contains the names of the columns.
A blank space counts as an empty string.
Treat every value as a string.

Your Task
Write a method that sorts the columns by the names of the columns alphabetically, and case-insensitive.


Example Input

Raw Input:
Beth,Charles,Danielle,Adam,Eric\n17945,10091,10088,3907,10132\n2,12,13,48,11

As a Table:
Beth  Charles Danielle Adam Eric
17945 10091	  10088	   3907 10132
2     12      13       48   11


Example Output

Raw output:
Adam,Beth,Charles,Danielle,Eric\n3907,17945,10091,10088,10132\n48,2,12,13,11

As a Table:
Adam Beth  Charles Danielle Eric
3907 17945 10091   10088    10132
48   2     12      13       11
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

////////////////////////////////////////////////////////////////////////////////
// Change all capital letters of a string into non-capital leters
std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); }
                  );
    return s;
}

////////////////////////////////////////////////////////////////////////////////
// Sort a vector returning the indexes order
template <typename T>
std::vector<size_t> sort_indexes(const std::vector<T> &v)
{
  // initialize original index locations
  std::vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  std::sort(idx.begin(),
            idx.end(),
            [&v](size_t i1, size_t i2) { return str_tolower(v[i1]) < str_tolower(v[i2]); });

  return idx;
}

////////////////////////////////////////////////////////////////////////////////
// Transform a string of n elements separated by delim
// into a vector of n strings
template<typename O>
void split(const std::string& s, char delim, O out) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(out++) = item;
    }
}

auto split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

////////////////////////////////////////////////////////////////////////////////
// Required function
std::string sort_csv_columns(std::string csv_data)
{
    std::string res;

    // Separate input by newline charaters, spliting lines
    std::vector<std::vector<std::string>> lines;
    for (const auto& e : split(csv_data, '\n'))
        lines.push_back(split(e, ','));

    // Order names keeping indexes
    auto id = sort_indexes(*lines.begin());

    // Traverse lines to generate the resulting string
    std::string delim = "";
    for (const auto& e : lines)
    {
        auto s = std::accumulate(std::next(id.begin()), id.end(),
                                 e[*id.begin()],
                                 [&e](std::string acc, int i)
                                 { return acc + ',' + e[i]; });
        res += delim + s;
        delim = "\n";
    }

    return res;
}

////////////////////////////////////////////////////////////////////////////////
// Not needed in assessment
int main()
{
    std::string inp = "Beth,Charles,Danielle,Adam,Eric\n17945,10091,10088,3907,10132\n2,12,13,48,11\nb,c,d,a,e\n200,300,400,100,500";
    std::cout << "Input: \n" << inp << "\n\n";
    auto res2 = sort_csv_columns(inp);
    std::cout << "Result: \n" << res2;

    return 0;
}
