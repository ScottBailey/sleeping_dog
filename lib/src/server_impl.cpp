#include <server_impl.hpp>
#include <listener.hpp>
#include <http_session.hpp>

#include <sb/sleeping_dog/router.hpp>


#include <iostream> // debugging, remove!!!!


namespace {

void load_server_certificate(boost::asio::ssl::context& ctx) {

  // openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes

  ctx.use_certificate_chain_file("cert.pem");
  ctx.use_private_key_file("key.pem", boost::asio::ssl::context::pem);


  // Set SSL options
  ctx.set_options(boost::asio::ssl::context::default_workarounds
      | boost::asio::ssl::context::no_sslv2
      | boost::asio::ssl::context::no_sslv3
      | boost::asio::ssl::context::single_dh_use);


  ctx.set_options(boost::asio::ssl::context::tlsv12 | boost::asio::ssl::context::tlsv13);


  if (!SSL_CTX_set_cipher_list(ctx.native_handle(), "ECDHE+AESGCM:ECDHE+CHACHA20")) {
    std::cout << "Failed to set cipher list" << std::endl;
    throw std::runtime_error("Failed to set cipher list");
  }
}

} // namespace


namespace sb::sleeping_dog {


server_impl::server_impl(private_token /*key*/, size_t thread_count)
  : thread_count_{thread_count}
  , ioc_(thread_count_)
  , work_guard_{boost::asio::make_work_guard(ioc_)}
{
  // nothing to do here
}


server_impl::~server_impl() = default;


//--- alphabetic -------------------------------------------------------------------------------------------------------


void server_impl::cancel() {
  work_guard_.reset();
  //for (auto& a : threads_) {
  //   a.cancel(); ?
  //}
}


std::shared_ptr<server_impl> server_impl::create(size_t thread_count) {
  if (!thread_count || thread_count > 10) {
    throw "***fix*** bad thread count";
  }
  auto temp = std::make_shared<server_impl>(private_token{}, thread_count);
  temp->initialize();
  return temp;
}


void server_impl::initialize() {
  // Generate I/O and SSL contexts
  ctx_ = std::make_unique<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);

  // load certificates:
  //  see   boost/libs/beast/example/common/server_certificate.hpp
  load_server_certificate(*ctx_);

}


void server_impl::join() {

  // ***fix***
  for (auto& a : threads_) {
    a.join();
  }
}


void server_impl::listen(unsigned port) {
  listen("0.0.0.0", port);
}


void server_impl::listen(std::string_view addr, unsigned port) {
  address_str_ = addr;
  port_ = port;
}


sleeping_dog::router* server_impl::router() {
    // throws smth if router is nullptr
  if (!router_)
    throw "smth";

  return router_.get();
}


void server_impl::run() {
  start();
  join();
}


void server_impl::session_add(std::shared_ptr<http_session> session_ptr) {
  if (!session_ptr) {
    return;
  }
  session_del(session_ptr);
  sessions_.push_back(session_ptr);
}


void server_impl::session_del(std::shared_ptr<http_session> session_ptr) {
  for (auto i = sessions_.begin(); i != sessions_.end(); ) {  // should be a remove_if
    if (*i == session_ptr) {
      i = sessions_.erase(i);
    }
    else {
      ++i;
    }
  }
}


/// non-blocking call
void server_impl::start() {

  // what happens if we've already started?


  // start the threads here:
  for (size_t i=0; i < thread_count_; ++i) {
    threads_.emplace_back([&]{ioc_.run();});
  }


  const auto addr = boost::asio::ip::make_address(address_str_);
  const auto port = static_cast<unsigned short>(port_);
  boost::asio::ip::tcp::endpoint endpoint{addr, port};


  // Create the listner
  listener_ = listener::create(shared_from_this(), ioc_, *ctx_, endpoint);
  listener_->run();

}


void server_impl::take(std::unique_ptr<sleeping_dog::router>&& r) {
  router_ = std::move(r);
}


bool server_impl::try_join() {
  return true;
}


} // namespace sb::sleeping_dog
