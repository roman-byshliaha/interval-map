// Copyright (C) 2020 - Roman Byshliaha

#include <gtest/gtest.h>

#include <cstdint>
#include <utility>

#include "interval_map.hpp"

namespace interval_map_test {

namespace {

constexpr uint8_t MIN = std::numeric_limits<uint8_t>::lowest();
constexpr uint8_t MAX = std::numeric_limits<uint8_t>::max();

}  // namespace

TEST(IntervalMapTest, InsertAnotherIntoStart) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100):'b', [100, 255]: 'a'.
  intervals.insert(MIN, 100, 'b');

  ASSERT_EQ(intervals.get_intervals().size(), 2);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 100);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, InsertAnotherIntoEnd) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100):'a', [100, 255): 'b', [255, 255]: 'a'.
  intervals.insert(100, MAX, 'b');

  ASSERT_EQ(intervals.get_intervals().size(), 3);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 100);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, MAX);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, InsertSameIntoStartNothingChanges) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 255]: 'a'.
  intervals.insert(MIN, 100, 'a');

  ASSERT_EQ(intervals.get_intervals().size(), 1);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, InsertSameIntoEndNothingChanges) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 255]: 'a'.
  intervals.insert(100, MAX, 'a');

  ASSERT_EQ(intervals.get_intervals().size(), 1);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, IdealReplaceInStart) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100):'b', [100, 255]: 'a'.
  intervals.insert(MIN, 100, 'b');

  // [0, 100):'c', [100, 255]: 'a'.
  intervals.insert(MIN, 100, 'c');

  ASSERT_EQ(intervals.get_intervals().size(), 2);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'c');
  ++it;
  ASSERT_EQ(it->first, 100);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, IdealReplaceInEnd) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100):'b', [100, 255]: 'a'.
  intervals.insert(100, MAX, 'b');

  // [0, 100):'a', [100, 255]: 'c', [255, 255]: 'a'..
  intervals.insert(100, MAX, 'c');

  ASSERT_EQ(intervals.get_intervals().size(), 3);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 100);
  ASSERT_EQ(it->second, 'c');
  ++it;
  ASSERT_EQ(it->first, MAX);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, ExtendAtStart) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100):'b', [100, 255]: 'a'.
  intervals.insert(MIN, 100, 'b');

  // [0, 200):'b', [200, 255]: 'a'.
  intervals.insert(MIN, 200, 'b');

  ASSERT_EQ(intervals.get_intervals().size(), 2);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 200);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, ExtendAtEnd) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100):'a', [100, 255): 'b', [255, 255]: 'a'.
  intervals.insert(100, MAX, 'b');

  // [0, 50):'a', [50, 255): 'b', [255, 255]: 'a'.
  intervals.insert(50, MAX, 'b');

  ASSERT_EQ(intervals.get_intervals().size(), 3);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 50);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, MAX);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, ExtendInTheMiddleToLeft) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100): 'a', [100, 200):'b', [200, 255]: 'a'.
  intervals.insert(100, 200, 'b');

  // [0, 50): 'a', [50, 200):'b', [200, 255]: 'a'.
  intervals.insert(50, 200, 'b');

  ASSERT_EQ(intervals.get_intervals().size(), 3);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 50);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 200);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, ExtendInTheMiddleToRight) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100): 'a', [100, 200):'b', [200, 255]: 'a'.
  intervals.insert(100, 200, 'b');

  // [0, 100): 'a', [100, 250):'b', [250, 255]: 'a'.
  intervals.insert(100, 250, 'b');

  ASSERT_EQ(intervals.get_intervals().size(), 3);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 100);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 250);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, ExtendInTheMiddleBothSides) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100): 'a', [100, 200):'b', [200, 255]: 'a'.
  intervals.insert(100, 200, 'b');

  // [0, 50): 'a', [50, 250):'b', [250, 255]: 'a'.
  intervals.insert(50, 250, 'b');

  ASSERT_EQ(intervals.get_intervals().size(), 3);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 50);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 250);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, OverlapStartToMiddle) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100): 'b', [100, 255]: 'a'.
  intervals.insert(MIN, 100, 'b');

  // [0, 100): 'b', [100, 200):'c', [200, 255]: 'a'.
  intervals.insert(100, 200, 'c');

  // [0, 150): 'd', [150, 200):'c', [200, 255]: 'a'.
  intervals.insert(MIN, 150, 'd');

  ASSERT_EQ(intervals.get_intervals().size(), 3);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'd');
  ++it;
  ASSERT_EQ(it->first, 150);
  ASSERT_EQ(it->second, 'c');
  ++it;
  ASSERT_EQ(it->first, 200);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, OverlapMiddleToEnd) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100): 'b', [100, 255]: 'a'.
  intervals.insert(MIN, 100, 'b');

  // [0, 100): 'b', [100, 200):'c', [200, 255]: 'a'.
  intervals.insert(100, 200, 'c');

  // [0, 50): 'b', [50, 200):'d', [200, 255]: 'a'.
  intervals.insert(50, 200, 'd');

  ASSERT_EQ(intervals.get_intervals().size(), 3);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 50);
  ASSERT_EQ(it->second, 'd');
  ++it;
  ASSERT_EQ(it->first, 200);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, OverlapMiddleToMiddle) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100): 'b', [100, 255]: 'a'.
  intervals.insert(MIN, 100, 'b');

  // [0, 100): 'b', [100, 200):'c', [200, 255]: 'a'.
  intervals.insert(100, 200, 'c');

  // [0, 50): 'b', [50, 150): 'd', [150, 200):'c', [200, 255]: 'a'.
  intervals.insert(50, 150, 'd');

  ASSERT_EQ(intervals.get_intervals().size(), 4);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 50);
  ASSERT_EQ(it->second, 'd');
  ++it;
  ASSERT_EQ(it->first, 150);
  ASSERT_EQ(it->second, 'c');
  ++it;
  ASSERT_EQ(it->first, 200);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, OverlapMiddleToMiddleEatingInterval) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100): 'b', [100, 255]: 'a'.
  intervals.insert(MIN, 100, 'b');

  // [0, 100): 'b', [100, 200):'c', [200, 255]: 'a'.
  intervals.insert(100, 200, 'c');

  // [0, 50): 'b', [50, 220): 'd', [220, 255]: 'a'.
  intervals.insert(50, 220, 'd');

  ASSERT_EQ(intervals.get_intervals().size(), 3);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 50);
  ASSERT_EQ(it->second, 'd');
  ++it;
  ASSERT_EQ(it->first, 220);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, ConcatenateIntervalsAtStart) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100): 'b', [100, 255]: 'a'.
  intervals.insert(MIN, 100, 'b');

  // [0, 255): 'a'.
  intervals.insert(MIN, 100, 'a');

  ASSERT_EQ(intervals.get_intervals().size(), 1);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
}

TEST(IntervalMapTest, ConcatenateIntervalsInTheMiddle) {
  // Whole interval of 'a's.
  interval_map<uint8_t, char> intervals{'a'};

  // [0, 100): 'a', [100, 200): 'b', [200, 255]: 'a'.
  intervals.insert(100, 200, 'b');

  // [0, 255): 'a'.
  intervals.insert(100, 200, 'a');

  ASSERT_EQ(intervals.get_intervals().size(), 1);
  auto it = intervals.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
}

}  // namespace interval_map_test
