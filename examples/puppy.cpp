#include <sb/sleeping_dog.hpp>
#include <CLI/CLI.hpp>

#include <spdlog/spdlog.h>
#include <fmt/format.h>


using verb = sb::sleeping_dog::verb_type;
using response_type = sb::sleeping_dog::response_type;
using request_type = sb::sleeping_dog::request_type;

int main(int argc, char** argv) {

  spdlog::set_level(spdlog::level::trace);

  CLI::App app{"Sleeping Dog example"};

  unsigned port;
  app.add_option("port", port, "Port for the server to listen.")->required();

  CLI11_PARSE(app, argc, argv);


  //auto controller = std::make_unique<some_controller>("path/to/our.db");
  sb::sleeping_dog::server puppy;
  auto router = std::make_unique<sb::sleeping_dog::router>();

  // A router can have a prefix?
  router->prefix("/some");
  /*
  // idk how authentication should work. This probably isn't it, but it's here as a placeholder.
  router->auth("/login",
      // This is probably wrong. What happened to some_connection_data? How do we respond with a token? How does that token time out?
      [](std::string_view user, std::string_view pass) -> bool {
        return lookup(user,pass);
      });
  */
  router->add(verb::get, "/data",
      [](const request_type& r) -> response_type {
        //controller->do_something(payload);
        //con.done();
        return sb::sleeping_dog::make_response(r, fmt::format("Hi! I received a message with target: '{}'\n", r.target()));
      });

  /*
  router->drop(GET, "/x/y");
  */
  puppy.take(std::move(router)); // moved


  puppy.listen(port); // addr any when no address is provided.

  std::cout << "curl -k https://192.168.100.102:8080/data\n";

  puppy.run();  // blocking call, for non-blocking use: start() & join()

  return 0;
}
