#pragma once

#include <sb/sleeping_dog/router.hpp>
#include <unordered_map>

namespace sb::sleeping_dog {


struct router::router_data {
  /// The map key is a verb and path
  using key_type = std::pair<verb_type,path_type>;
  /// hash_type for the map key
  struct hash_type {
    std::size_t operator()(const key_type& k) const {
      std::size_t h1 = std::hash<unsigned>{}(unsigned(k.first));
      std::size_t h2 = std::hash<std::string>{}(k.second);
      return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
  };
  std::unordered_map<key_type,router::route_callback_type,hash_type> map;
};


} // namespace sb::sleeping_dog
