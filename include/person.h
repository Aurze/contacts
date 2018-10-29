#ifndef PERSON_H
#define PERSON_H

#include <string>

namespace pim {
class Person {
  public :
   std::string key;
   std::string first_name;
   std::string last_name;

   Person() {}
   Person(std::string k, std::string f, std::string l)
       : key(k), first_name(f), last_name(l) {}
};
}  // namespace pim

#endif
