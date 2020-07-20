// Copyright (C) 2020 - Roman Byshliaha

#ifndef INCLUDE_INTERVAL_MAP_HPP_
#define INCLUDE_INTERVAL_MAP_HPP_

#include <iterator>
#include <limits>
#include <map>
#include <utility>

template <typename K, typename V>
class interval_map final {
 public:
  explicit interval_map(const V& val)
      : map_{{std::numeric_limits<K>::lowest(), val}} {}

  interval_map() = delete;

  interval_map(const interval_map& another) = delete;
  interval_map(interval_map&& another) = delete;

  interval_map& operator=(const interval_map& another) = delete;
  interval_map& operator=(interval_map&& another) = delete;

  // Inserts new interval into interval_map.
  // All values from |key_begin| (including) and |key_end| (excluding) will have
  // value |val|.
  void insert(const K& key_begin, const K& key_end, const V& val) {
    if (key_begin >= key_end) {
      return;
    }

    auto equals = [](const K& l, const K& r) -> bool {
      return (!(l < r) && !(r < l));
    };

    // Start from updating |key_end|, since updating |key_end| might influence
    // on further end processing.
    auto end_bound = map_.lower_bound(key_end);
    auto end_prev_it = std::prev(end_bound);
    auto end_insert_it = end_bound;

    if (end_bound != map_.end() && end_bound->first == key_end &&
        end_bound->second == val) {
      // If |key_end| coincides with starting of another interval, which has the
      // same value as |val|, take its end as end of inteval.
      end_insert_it = std::next(end_bound);
    } else if (end_prev_it->second != val) {
      // If previout interval had the same value as |val|, its end value will be
      // taken as end of interval. Otherwise insert new element with previous
      // value on |key_end| position.
      end_insert_it = map_.insert(end_prev_it,
                                  std::make_pair(key_end, end_prev_it->second));
    }

    auto start_bound = map_.lower_bound(key_begin);
    auto start_prev_it = std::prev(start_bound);
    auto start_insert_it = start_bound;

    if (start_bound == map_.begin() || start_prev_it->second != val) {
      if (start_bound != map_.end() && equals(key_begin, start_bound->first)) {
        // Update value of the start of interval, since std::map_insert()
        // doesn't update value, if elemnt already exists.
        start_insert_it->second = val;
      } else {
        // Insert new element with |val| at |key_begin| position.
        start_insert_it =
            map_.insert(start_prev_it, std::make_pair(key_begin, val));
      }
    } else {
      // If previous interval has the same value, concatenate intervals.
      start_insert_it = start_prev_it;
    }

    // Erase everything, starting next element till end of bound.
    map_.erase(std::next(start_insert_it), end_insert_it);
  }

  const V& operator[](const K& key) const {
    return (--map_.upper_bound(key))->second;
  }

  const std::map<K, V>& get_intervals() const { return map_; }

 private:
  std::map<K, V> map_;
};

#endif  // INCLUDE_INTERVAL_MAP_HPP_
