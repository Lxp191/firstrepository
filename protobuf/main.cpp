#include <string>
#include <iostream>
#include "test.pb.h"

int main(int argc, char *argv[])
{
    example::Test t;
    t.set_name("hello world");
    t.add_age(1);
    t.add_age(2);
    t.add_age(3);

    auto outPut = t.SerializeAsString();

    example::Test t2;
    t2.ParseFromString(outPut);

    std::cout << t2.name() << std::endl;
    for (int i = 0; i < t2.age_size(); i++)
    {
        std::cout << t2.age(i) << std::endl;
    }

    return 0;
}