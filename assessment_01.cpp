/*
For this challenge, you will be finding the most efficient set of steps to get from one item to another.

Inputs
You will write a function (called either findProcesses or find_processes depending on the programming language) which will be provided with a set of processes that each convert from one item to another.
These will be provided as an array, with each process in the form of process_name:from_item:to_item. For example, you might have a process named smelt that converts ore into a metal, like this: smelt:ore:metal.
You will also be passed in the initial start item and desired end item for the overall workflow.
Note: For simplicity, all processes and items will be simple alphanumeric strings.

Output
Given these three inputs, your task is to determine which processes will complete the task in the shortest number of steps. Once you have done so, return the names of these processes, in order, as an array.
If no steps are needed (because the start and end item is the same), or if you cannot find a valid set of processes to get from the start to the end, then return an empty array ([]).
Warning: make sure your code doesn't get caught in an infinite loop!

Examples
std::list<std:string> *processes = {
    "gather:field:wheat",
    "bake:flour:bread",
    "mill:wheat:flour"
};

find_processes("field", "bread", processes); // should return {"gather", "mill", "bake"}
find_processes("field", "ferrari", processes); // should return {}
find_processes("field", "field", processes); // should return {}, since no processes are needed
*/

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <tuple>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cassert>

// This whole application can be done, in a easier way, changing the tuple of
// strings by a vector of strings, but it's more challenging with tuples, since
// its size is set by requirements. For variable size, use a vector

const int TUPLE_SIZE = 3; // Processes size

////////////////////////////////////////////////////////////////////////////////
// Create a tuple typeof specified size and type
template <size_t I,typename T> 
struct tuple_n
{
    template< typename...Args>
    using type = typename tuple_n<I-1, T>::template type<T, Args...>;
};

template <typename T> 
struct tuple_n<0, T>
{
    template<typename...Args>
    using type = std::tuple<Args...>;   
};

template <size_t I,typename T>
using tuple_of = typename tuple_n<I,T>::template type<>;


////////////////////////////////////////////////////////////////////////////////
// Transform a vector into a tuple of N elements
template <typename T, std::size_t... Indices>
auto vectorToTupleHelper(const std::vector<T>& v,
                         std::index_sequence<Indices...>)
{
    return std::make_tuple(v[Indices]...);
}

template <std::size_t N, typename T>
auto vectorToTuple(const std::vector<T>& v)
{
    assert(v.size() >= N);
    return vectorToTupleHelper(v, std::make_index_sequence<N>());
}


////////////////////////////////////////////////////////////////////////////////
// Transform a string of N elements separated by delim
// into a vector of N strings
template<typename O>
void split(const std::string& s, char delim, O out)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim))
        *(out++) = item;
}

auto split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

////////////////////////////////////////////////////////////////////////////////
// Transform a container of strings on N elements
// into a container of tuples of N strings
template <std::size_t N, typename C>
auto string2tuple(C input)
{
    std::vector<tuple_of<N, std::string>> output;
    for (const auto& e : input)
        output.push_back(vectorToTuple<N>(split(e, ':')));
    return output;
}


////////////////////////////////////////////////////////////////////////////////
// Find, if exists,the shortest way to get from start to end using the steps
// described in tasks
// It returns a list with the steps needed in reverse order, since it creates
// the list when the final element is found, and then it adds elements to it
// towards the first element, so the minimum number of lists are needed
template <size_t N>
auto find_shortest_path(const std::string& start,
                        const std::string& end,
                        std::vector<tuple_of<N, std::string>> tasks)
{
    // If the start and the end are the same or no steps are defined, then exit
    if (start == end || tasks.empty())
        return std::list<std::string>{};

    // Find the first ocurrence of start in the steps matrix
    auto it = std::find_if(tasks.begin(),
                          tasks.end(),
                          [&start](const tuple_of<N, std::string>& e)
                          { return std::get<1>(e) == start; });
    // If it doesn't exist, then exit with empty list
    if (it == tasks.end())
        return std::list<std::string>{};

    std::list<std::string> result {}; // the shortest path
    // Loop over all ocurrences of start, saving the shortes one
    while (it != tasks.end())
    {
        auto end_found = std::get<2>(*it);
        auto proc_found = std::get<0>(*it);

        // We found the end step, so we're done here.        
        if (end_found == end)
            return std::list<std::string>{proc_found};

        // To avoid loops, we remove the current step from the steps matrix.
        // As we're modifing the steps matrix, we need it to be passed by value
        tasks.erase(it);
        // Then we continue searching next step
        auto aux = find_shortest_path<TUPLE_SIZE>(end_found, end, tasks);
        if (!aux.empty()) // final element not found
            aux.push_back(proc_found);

        // search for the shortest path
        if (!aux.empty() && (result.empty() || aux.size() < result.size()))
            std::swap(result, aux); // we don't need aux anymore

        // Look for next ocurrence of start in the steps matrix without
        // last ocurrence found
        it = std::find_if(tasks.begin(),
                          tasks.end(),
                          [&start](const tuple_of<N, std::string>& e)
                          { return std::get<1>(e) == start; });
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
// Transform input into more handable data
auto format_input(std::list<std::string> input)
{
    return string2tuple<TUPLE_SIZE>(input);
}

////////////////////////////////////////////////////////////////////////////////
// Required function
std::list<std::string> find_processes(std::string start_item, std::string end_item, std::list<std::string> tasks)
{
    auto tupled_tasks = format_input(tasks);
    std::list<std::string> ret = find_shortest_path<TUPLE_SIZE>(start_item, end_item, tupled_tasks);
    ret.reverse();
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
// Not needed in assessment
int main()
{
//    std::string start {"field"};
//    std::string end {"bread"};
//    std::list<std::string> tasks {"mill:wheat:flour","gather:field:wheat","bake:flour:bread"};
    std::string start {"1"};
    std::string end {"10"};
    std::list<std::string> tasks {
    	"add1+1=2:1:2",
            "add2+1=3:2:3",
            "add3+1=4:3:4",
            "add4+1=5:4:5",
            "add5+1=6:5:6",
            "add6+1=7:6:7",
            "add7+1=8:7:8",
            "add8+1=9:8:9",
            "add9+1=10:9:10",
            "sub2-1=1:2:1",
            "sub3-1=2:3:2",
            "sub4-1=3:4:3",
            "sub5-1=4:5:4",
            "sub6-1=5:6:5",
            "sub7-1=6:7:6",
            "sub8-1=7:8:7",
            "sub9-1=8:9:8",
            "mul2*2=4:2:4",
            "mul2*3=6:2:6",
            "mul2*4=8:2:8",
            "mul2*5=10:2:10",
    	};
    std::cout << start << " -> " << end << '\n';
    auto ret = find_processes(start, end, tasks);
    std::cout << "Output:" << '\n';
    if (ret.empty())
        std::cout << "not reachable" << '\n';
    else
        for (const auto& e : ret)
            std::cout << e << '\n';
    std::cout << end << '\n';

    return 0;
}
