#include <sb/sleeping_dog.hpp>


int main(int,char**) {

  //auto controller = std::make_unique<some_controller>("path/to/our.db");
  sb::sleeping_dog::server puppy;
  auto router = std::make_unique<sb::sleeping_dog::router>();

  // A router can have a prefix
  router->prefix("/some");
  /*
  // idk how authentication should work. This probably isn't it, but it's here as a placeholder.
  router->auth("/login",
      // This is probably wrong. What happened to some_connection_data? How do we respond with a token? How does that token time out?
      [](std::string_view user, std::string_view pass) -> bool {
        return lookup(user,pass);
      });

  //
  router->add(GET, "/x/y",
      [](some_connection_data con, some_payload_data payload) -> resp {
        controller->do_something(payload);
        //con.done();
        return some_response;
      });

  router->drop(GET, "/x/y");
  */
  puppy.take(std::move(router)); // moved


  puppy.listen("addr_any", 8080);

  puppy.run();  // blocking call, for non-blocking use: start() & join()

  return 0;
}
