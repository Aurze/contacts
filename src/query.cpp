#include <fuerte/fuerte.h>
#include <velocypack/Iterator.h>
#include <velocypack/Parser.h>
#include <velocypack/Slice.h>
#include <velocypack/velocypack-aliases.h>
#include <string>
#include <vector>

#include "../include/person.h"
#include "../include/query.h"

namespace pim {

auto initConnection(arangodb::fuerte::EventLoopService& eventLoopService) {
  return arangodb::fuerte::ConnectionBuilder()
      .host("vst://localhost:8529")
      .user("root")
      .password("")
      .connect(eventLoopService);
}

std::unique_ptr<arangodb::fuerte::Response> request(
    std::shared_ptr<arangodb::fuerte::Connection> conn, std::string r) {
  auto request =
      createRequest(arangodb::fuerte::RestVerb::Post, "/_api/cursor");
  VPackBuilder builder;
  builder.openObject();
  builder.add("query", VPackValue(std::string{r}));
  builder.close();
  request->addVPack(builder.slice());
  return conn->sendRequest(std::move(request));
};

std::vector<Person> get_all() {
  arangodb::fuerte::EventLoopService eventLoopService;
  auto conn = initConnection(eventLoopService);
  auto result = request(std::move(conn), "FOR p IN person RETURN p");
  auto slice = result->slices().front().get("result");
  std::vector<Person> persons;

  for (auto const& it : arangodb::velocypack::ArrayIterator(slice)) {
    Person person(it.get("_key").copyString(),
                  it.get("first_name").copyString(),
                  it.get("last_name").copyString());
    persons.push_back(person);
    std::cout << person.first_name << std::endl;
    std::cout << person.last_name << std::endl;
  }

  return persons;
};
}
