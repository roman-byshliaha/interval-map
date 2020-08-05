// Copyright (C) 2020 - Roman Byshliaha

#include <gtest/gtest.h>

#include <cstdint>
#include <utility>

#include "interval_map.hpp"

namespace interval_map_test {

namespace {

constexpr uint8_t MIN = std::numeric_limits<uint8_t>::lowest();
constexpr uint8_t MAX = std::numeric_limits<uint8_t>::max();

class IntervalMapTest : public testing::Test {
 protected:
  IntervalMapTest()
      // Whole interval of 'a's.
      : intervals_{'a'} {}

  interval_map<uint8_t, char> intervals_;
};

}  // namespace

TEST_F(IntervalMapTest, InsertAnotherIntoStart) {
  // [0, 100):'b', [100, 255]: 'a'.
  intervals_.insert(MIN, 100, 'b');

  ASSERT_EQ(intervals_.get_intervals().size(), 2);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 100);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, InsertAnotherIntoEnd) {
  // [0, 100):'a', [100, 255): 'b', [255, 255]: 'a'.
  intervals_.insert(100, MAX, 'b');

  ASSERT_EQ(intervals_.get_intervals().size(), 3);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 100);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, MAX);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, InsertSameIntoStartNothingChanges) {
  // [0, 255]: 'a'.
  intervals_.insert(MIN, 100, 'a');

  ASSERT_EQ(intervals_.get_intervals().size(), 1);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, InsertSameIntoEndNothingChanges) {
  // [0, 255]: 'a'.
  intervals_.insert(100, MAX, 'a');

  ASSERT_EQ(intervals_.get_intervals().size(), 1);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, IdealReplaceInStart) {
  // [0, 100):'b', [100, 255]: 'a'.
  intervals_.insert(MIN, 100, 'b');

  // [0, 100):'c', [100, 255]: 'a'.
  intervals_.insert(MIN, 100, 'c');

  ASSERT_EQ(intervals_.get_intervals().size(), 2);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'c');
  ++it;
  ASSERT_EQ(it->first, 100);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, IdealReplaceInEnd) {
  // [0, 100):'b', [100, 255]: 'a'.
  intervals_.insert(100, MAX, 'b');

  // [0, 100):'a', [100, 255]: 'c', [255, 255]: 'a'..
  intervals_.insert(100, MAX, 'c');

  ASSERT_EQ(intervals_.get_intervals().size(), 3);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 100);
  ASSERT_EQ(it->second, 'c');
  ++it;
  ASSERT_EQ(it->first, MAX);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, ExtendAtStart) {
  // [0, 100):'b', [100, 255]: 'a'.
  intervals_.insert(MIN, 100, 'b');

  // [0, 200):'b', [200, 255]: 'a'.
  intervals_.insert(MIN, 200, 'b');

  ASSERT_EQ(intervals_.get_intervals().size(), 2);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 200);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, ExtendAtEnd) {
  // [0, 100):'a', [100, 255): 'b', [255, 255]: 'a'.
  intervals_.insert(100, MAX, 'b');

  // [0, 50):'a', [50, 255): 'b', [255, 255]: 'a'.
  intervals_.insert(50, MAX, 'b');

  ASSERT_EQ(intervals_.get_intervals().size(), 3);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 50);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, MAX);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, ExtendInTheMiddleToLeft) {
  // [0, 100): 'a', [100, 200):'b', [200, 255]: 'a'.
  intervals_.insert(100, 200, 'b');

  // [0, 50): 'a', [50, 200):'b', [200, 255]: 'a'.
  intervals_.insert(50, 200, 'b');

  ASSERT_EQ(intervals_.get_intervals().size(), 3);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 50);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 200);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, ExtendInTheMiddleToRight) {
  // [0, 100): 'a', [100, 200):'b', [200, 255]: 'a'.
  intervals_.insert(100, 200, 'b');

  // [0, 100): 'a', [100, 250):'b', [250, 255]: 'a'.
  intervals_.insert(100, 250, 'b');

  ASSERT_EQ(intervals_.get_intervals().size(), 3);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 100);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 250);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, ExtendInTheMiddleBothSides) {
  // [0, 100): 'a', [100, 200):'b', [200, 255]: 'a'.
  intervals_.insert(100, 200, 'b');

  // [0, 50): 'a', [50, 250):'b', [250, 255]: 'a'.
  intervals_.insert(50, 250, 'b');

  ASSERT_EQ(intervals_.get_intervals().size(), 3);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
  ++it;
  ASSERT_EQ(it->first, 50);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 250);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, OverlapStartToMiddle) {
  // [0, 100): 'b', [100, 255]: 'a'.
  intervals_.insert(MIN, 100, 'b');

  // [0, 100): 'b', [100, 200):'c', [200, 255]: 'a'.
  intervals_.insert(100, 200, 'c');

  // [0, 150): 'd', [150, 200):'c', [200, 255]: 'a'.
  intervals_.insert(MIN, 150, 'd');

  ASSERT_EQ(intervals_.get_intervals().size(), 3);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'd');
  ++it;
  ASSERT_EQ(it->first, 150);
  ASSERT_EQ(it->second, 'c');
  ++it;
  ASSERT_EQ(it->first, 200);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, OverlapMiddleToEnd) {
  // [0, 100): 'b', [100, 255]: 'a'.
  intervals_.insert(MIN, 100, 'b');

  // [0, 100): 'b', [100, 200):'c', [200, 255]: 'a'.
  intervals_.insert(100, 200, 'c');

  // [0, 50): 'b', [50, 200):'d', [200, 255]: 'a'.
  intervals_.insert(50, 200, 'd');

  ASSERT_EQ(intervals_.get_intervals().size(), 3);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 50);
  ASSERT_EQ(it->second, 'd');
  ++it;
  ASSERT_EQ(it->first, 200);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, OverlapMiddleToMiddle) {
  // [0, 100): 'b', [100, 255]: 'a'.
  intervals_.insert(MIN, 100, 'b');

  // [0, 100): 'b', [100, 200):'c', [200, 255]: 'a'.
  intervals_.insert(100, 200, 'c');

  // [0, 50): 'b', [50, 150): 'd', [150, 200):'c', [200, 255]: 'a'.
  intervals_.insert(50, 150, 'd');

  ASSERT_EQ(intervals_.get_intervals().size(), 4);
  auto it = intervals_.get_intervals().begin();
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

TEST_F(IntervalMapTest, OverlapMiddleToMiddleEatingInterval) {
  // [0, 100): 'b', [100, 255]: 'a'.
  intervals_.insert(MIN, 100, 'b');

  // [0, 100): 'b', [100, 200):'c', [200, 255]: 'a'.
  intervals_.insert(100, 200, 'c');

  // [0, 50): 'b', [50, 220): 'd', [220, 255]: 'a'.
  intervals_.insert(50, 220, 'd');

  ASSERT_EQ(intervals_.get_intervals().size(), 3);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'b');
  ++it;
  ASSERT_EQ(it->first, 50);
  ASSERT_EQ(it->second, 'd');
  ++it;
  ASSERT_EQ(it->first, 220);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, ConcatenateIntervalsAtStart) {
  // [0, 100): 'b', [100, 255]: 'a'.
  intervals_.insert(MIN, 100, 'b');

  // [0, 255): 'a'.
  intervals_.insert(MIN, 100, 'a');

  ASSERT_EQ(intervals_.get_intervals().size(), 1);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
}

TEST_F(IntervalMapTest, ConcatenateIntervalsInTheMiddle) {
  // [0, 100): 'a', [100, 200): 'b', [200, 255]: 'a'.
  intervals_.insert(100, 200, 'b');

  // [0, 255): 'a'.
  intervals_.insert(100, 200, 'a');

  ASSERT_EQ(intervals_.get_intervals().size(), 1);
  auto it = intervals_.get_intervals().begin();
  ASSERT_EQ(it->first, MIN);
  ASSERT_EQ(it->second, 'a');
}

}  // namespace interval_map_test
