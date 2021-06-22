#include <ctime>
#include <iostream>

#include <time.h>
#include <unistd.h>

int main()
{
    auto t = std::time(nullptr);
    std::cout << "time: " << t << '\n';

    std::cout << "time: " << std::ctime(&t);
    std::cout << "time: " << std::asctime(std::localtime(&t));
    std::cout << "time: " << std::asctime(std::gmtime(&t));

    char buf[256] {};
    std::strftime(buf, sizeof(buf), "%m/%d/%Y", std::localtime(&t));
    std::cout << "time: " << buf << '\n';

    auto t1 = std::time(nullptr);
    sleep(2);
    auto t2 = std::time(nullptr);

    std::cout << "diff: " << std::difftime(t2, t1) << '\n';

    auto lt = std::localtime(&t1);
    auto t3 = std::mktime(lt);
    std::cout << "time: " << std::ctime(&t3);

    std::cout << "clock: " << std::clock() << '\n';


    auto c1 = std::clock();
    sleep(2);
    auto c2 = std::clock();

    std::cout << "clock: " << 
        static_cast<double>(c2 - c1) / CLOCKS_PER_SEC << '\n';   
    
    return 0;
}