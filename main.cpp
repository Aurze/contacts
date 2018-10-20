#include <string>
#include <fuerte/fuerte.h>
#include <velocypack/Parser.h>
#include <velocypack/Iterator.h>
#include <velocypack/velocypack-aliases.h>
#include <velocypack/Slice.h>

#include "person.h"

auto initConnection(arangodb::fuerte::EventLoopService& eventLoopService) {
  return arangodb::fuerte::ConnectionBuilder().host("vst://localhost:8529")
    .user("root")
    .password("")
    .connect(eventLoopService);
}

std::unique_ptr<arangodb::fuerte::Response> request(std::shared_ptr<arangodb::fuerte::Connection> conn, std::string r) {
  auto request = createRequest(arangodb::fuerte::RestVerb::Post, "/_api/cursor");
  VPackBuilder builder;
  builder.openObject();
  builder.add("query", VPackValue(std::string{r}));
  builder.close();
  request->addVPack(builder.slice());
  return conn->sendRequest(std::move(request));
}

std::unique_ptr<arangodb::fuerte::Response> p_add(std::shared_ptr<arangodb::fuerte::Connection> c, Person p) {
  VPackBuilder b;
  b.openObject();
  b.add("first_name", arangodb::velocypack::Value(p.first_name));
  b.add("last_name", arangodb::velocypack::Value(p.last_name));
  b.close();

  auto request = createRequest(arangodb::fuerte::RestVerb::Post, "/_api/document/person");
  request->addVPack(b.slice());
  return c->sendRequest(std::move(request));
}

std::unique_ptr<arangodb::fuerte::Response> del(std::shared_ptr<arangodb::fuerte::Connection> connection, std::string key){
  auto request = arangodb::fuerte::createRequest(
      arangodb::fuerte::RestVerb::Delete, "/_api/document/person/" + key);
  return connection->sendRequest(std::move(request));
}

int main(){
  arangodb::fuerte::EventLoopService eventLoopService;
  auto conn = initConnection(eventLoopService);

  auto result = request(std::move(conn), "FOR p IN person RETURN p");

  auto slice  =  result->slices().front().get("result");

  std::vector<Person> persons;

  for (auto const& it : arangodb::velocypack::ArrayIterator(slice)) {
    Person person(it.get("_key").copyString(), it.get("first_name").copyString(),
        it.get("last_name").copyString());
    persons.push_back(person);
    std::cout << person.first_name << std::endl;
    std::cout << person.last_name << std::endl;
  }

  Person p("", "Pierre", "Lavoie");

  conn = initConnection(eventLoopService);
  auto result2 = p_add(conn, p);


  std::cout<< arangodb::fuerte::to_string(*result2) << std::endl;
  std::cout << arangodb::fuerte::to_string(result2->slices().front()) << std::endl;

  p.key = result2->slices().front().get("_key").copyString();
  result = request(conn, "FOR p IN person RETURN p");

  slice  =  result->slices().front().get("result");

  persons.clear();

  for (auto const& it : arangodb::velocypack::ArrayIterator(slice)) {
    Person person(it.get("_key").copyString(), it.get("first_name").copyString(),
        it.get("last_name").copyString());
    persons.push_back(person);
    std::cout << person.first_name << std::endl;
    std::cout << person.last_name << std::endl;
  }

  auto result3 = del(conn, p.key);
  std::cout<< arangodb::fuerte::to_string(*result3) << std::endl;
  std::cout << arangodb::fuerte::to_string(result3->slices().front()) << std::endl;
}
