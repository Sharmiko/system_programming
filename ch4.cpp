#include <iostream>
#include <any>

namespace X {
    namespace Y {
        namespace Z {
            auto msg = "Hello World";
        }
    }
}

namespace X1::Y1::Z1 {
    auto msg = "Hello World";
}

std::pair<const char *, int> give_me_a_pair()
{
    return {"The answer is: ", 42};
}

struct mystruct
{
    int data;
};


int main()
{

    // initializers in if/switch statements
    if (auto i = 42; i > 0)
    {
        std::cout << "Hello World" << std::endl;
    }

    std::cout << X::Y::Z::msg << std::endl;
    std::cout << X1::Y1::Z1::msg << std::endl;

    auto [msg, answer] = give_me_a_pair();
    std::cout << msg << answer << std::endl;

    auto myany = std::make_any<int>(42);
    std::cout << std::any_cast<int>(myany) << std::endl;

    myany = 4.2;
    std::cout << std::any_cast<double>(myany) << std::endl;



    return 0;
}