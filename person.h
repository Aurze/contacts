#include <string>

class Person {
  public :
    std::string key;
    std::string first_name;
    std::string last_name;

    Person(std::string k, std::string f, std::string l) :
      key(k), first_name(f), last_name(l)
    {}
};
