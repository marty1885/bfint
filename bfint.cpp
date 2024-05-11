#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <cstdint>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if(!file.is_open())
    {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    size_t fsize = 0;
    file.seekg(0, std::ios::end);
    fsize = file.tellg();
    file.seekg(0, std::ios::beg);


    std::string code;
    code.reserve(fsize);
    std::stringstream ss;
    ss << file.rdbuf();
    code = ss.str();

    std::unordered_map<size_t, size_t> forward_brackets;
    std::unordered_map<size_t, size_t> backward_brackets;
    std::stack<size_t> bracket_stack;

    for(size_t i = 0; i < code.size(); i++)
    {
        if(code[i] == '[')
        {
            bracket_stack.push(i);
        }
        else if(code[i] == ']')
        {
            if(bracket_stack.empty())
            {
                std::cerr << "Error: Unmatched bracket at position " << i << std::endl;
                return 1;
            }
            forward_brackets[bracket_stack.top()] = i;
            backward_brackets[i] = bracket_stack.top();
            bracket_stack.pop();
        }
    }

    size_t pc = 0;
    std::vector<uint8_t> tape(30000, 0);

    for(size_t i = 0; i < code.size(); i++)
    {
        switch(code[i])
        {
            case '>':
                pc++;
                break;
            case '<':
                pc--;
                break;
            case '+':
                tape[pc]++;
                break;
            case '-':
                tape[pc]--;
                break;
            case '.':
                std::cout << tape[pc];
                break;
            case ',':
                std::cin >> tape[pc];
                break;
            case '[':
                if(tape[pc] == 0)
                {
                    i = forward_brackets[i];
                }
                break;
            case ']':
                if(tape[pc] != 0)
                {
                    i = backward_brackets[i];
                }
                break;
            default:
                break;
        }
    }
    std::cout << "\n\n* Execution complete\n";
    return 0;

}
