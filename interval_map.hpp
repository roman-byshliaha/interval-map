// Copyright (C) 2020 - Roman Byshliaha

#ifndef INTERVAL_MAP_HPP_
#define INTERVAL_MAP_HPP_

#include <cstdint>
#include <iterator>
#include <limits>
#include <map>
#include <utility>

template <typename K, typename V>
class interval_map final {
 public:
  std::map<K, V> map_;

 public:
  // Constructor associates whole range of K with val by inserting (K_min, val)
  // into the map.
  explicit interval_map(const V& val) {
    map_.insert(map_.end(),
                std::make_pair(std::numeric_limits<K>::lowest(), val));
  }

  interval_map() = delete;

  interval_map(const interval_map& another) = delete;
  interval_map(interval_map&& another) = delete;

  interval_map& operator=(const interval_map& another) = delete;
  interval_map& operator=(interval_map&& another) = delete;

  // Assigns value val to interval [key_begin, key_end).
  // Overwrites previous values in this interval.
  // Conforming to the C++ Standard Library conventions, the interval includes
  // key_begin, but excludes key_end. If !( key_begin < key_end ), this
  // designates an empty interval, and assign must do nothing.
  void assign(const K& key_begin, const K& key_end, const V& val) {
    if (key_begin >= key_end) {
      return;
    }

    auto equals = [](const K& l, const K& r) -> bool {
      return (!(l < r) && !(r < l));
    };

    auto start_bound = map_.lower_bound(key_begin);
    auto end_bound = map_.lower_bound(key_end);

    auto start_prev_it = std::prev(start_bound);
    auto end_prev_it = std::prev(end_bound);

    auto start_insert_it = start_bound;
    auto end_insert_it = end_bound;

    // If |key_begin| is the minimal value possible of K type.
    if (equals(key_begin, std::numeric_limits<K>::lowest())) {
      map_.begin()->second = val;

      if (end_bound == map_.end() ||
          (end_bound != map_.end() && !equals(key_end, end_bound->first))) {
        end_insert_it = map_.insert(
            end_prev_it, std::make_pair(key_end, end_prev_it->second));
      }

      map_.erase(std::next(map_.begin()), end_insert_it);
    } else {
      if ((end_bound == map_.end() || val == end_bound->second)) {
        if (!(val == end_prev_it->second ||
              equals(key_end, end_bound->first))) {
          end_insert_it = map_.insert(
              end_prev_it, std::make_pair(key_end, end_prev_it->second));
        }
      }

      if (val == start_prev_it->second) {
        map_.erase(start_insert_it, end_insert_it);
      } else {
        if (equals(key_begin, start_bound->first)) {
          start_bound->second = val;
        } else {
          start_insert_it =
              map_.insert(start_prev_it, std::make_pair(key_begin, val));
        }
        map_.erase(std::next(start_insert_it), end_insert_it);
      }
    }
  }

  // Looks up the value associated with key.
  const V& operator[](const K& key) const {
    return (--map_.upper_bound(key))->second;
  }
};

#endif  // INTERVAL_MAP_HPP_