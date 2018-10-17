#include <string>
#include <fuerte/fuerte.h>
#include <velocypack/Parser.h>
#include <velocypack/Iterator.h>
#include <velocypack/velocypack-aliases.h>
#include <velocypack/Slice.h>

int main(){
  using namespace arangodb::fuerte;
  EventLoopService eventLoopService;
  auto conn = ConnectionBuilder().host("vst://localhost:8529")
    .user("root")
    .password("")
    .connect(eventLoopService);
  auto request = createRequest(RestVerb::Get, "/_api/version");
  auto result = conn->sendRequest(std::move(request));
  auto slice = result->slices().front();
  std::cout << slice.get("version").copyString();
  std::cout << slice.get("server").copyString();


  auto request2 = createRequest(RestVerb::Post, "/_api/cursor");
  VPackBuilder builder;
  builder.openObject();
  builder.add("query", VPackValue(std::string{"FOR p IN person RETURN p"}));
  builder.close();
  request2->addVPack(builder.slice());
  auto result2 = conn->sendRequest(std::move(request2));
  auto slice2  =  result2->slices().front();
  std::cout << "Slice: " << slice2 << std::endl;
  std::cout << "Type: " << slice2.type() << std::endl;
  std::cout << "Bytesize: " << slice2.byteSize() << std::endl;
  std::cout << "Members: " << slice2.length() << std::endl;
  std::cout << to_string(slice2) << std::endl;

  auto slice3 = slice2.get("result");
  std::cout << "Slice: " << slice3 << std::endl;
  std::cout << "Type: " << slice3.type() << std::endl;
  std::cout << "Bytesize: " << slice3.byteSize() << std::endl;
  std::cout << "Members: " << slice3.length() << std::endl;
  std::cout << to_string(slice3) << std::endl;

  std::cout << slice3.get("first_name").copyString();
}
