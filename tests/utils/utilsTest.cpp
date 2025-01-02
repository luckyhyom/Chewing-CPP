#include <gtest/gtest.h>
#include "utils.h"

TEST(UtilTest, Stack) {
  MyExcel::NumStack* num_stack = new MyExcel::NumStack();
  num_stack->push(1);
  num_stack->push(2);
  num_stack->push(3);
  EXPECT_EQ(3, num_stack->pop()) << "It Should be poped 3 from 123";
  EXPECT_EQ(2, num_stack->pop()) << "It Should be poped 2 from 12";
  EXPECT_EQ(1, num_stack->pop()) << "It Should be poped 1 from 1";
  EXPECT_EQ(0, num_stack->pop()) << "It Should be poped 0 from NULL";
}

