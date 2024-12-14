#include <http_session.hpp>

namespace sb::sleeping_dog {

//---

http_session::http_session(private_token /*key*/, boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx, const std::shared_ptr<const std::string>& doc_root)
  : stream_(std::move(socket), ctx)
  , doc_root_(doc_root)
{
  static_assert(queue_limit_ > 0, "queue limit must be positive");
}


http_session::~http_session() = default;


//---


std::shared_ptr<http_session> http_session::create(boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx, const std::shared_ptr<const std::string>& doc_root) {
  return std::make_unique<http_session>(private_token{}, std::move(socket), ctx, doc_root);
}


void http_session::run() {
}





} // namespace sb::sleeping_dog
