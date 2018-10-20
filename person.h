#include <string>

class Person {
  public :
    std::string first_name;
    std::string last_name;

    Person(std::string f, std::string l) :
      first_name(f), last_name(l)
    {}
};
