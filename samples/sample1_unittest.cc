// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// A sample program demonstrating using Google C++ testing framework.
//
// Author: wan@google.com (Zhanyong Wan)


// 이 예제에서는 Google C++ 테스팅 프레임워크를 사용하여,
// 함수에 대한 간단한 유닛 테스트 작성법을 보여준다.
// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Google C++ 테스팅 프레임워크를 사용하여 유닛 테스트를 작성하는 것은
// 숫자 1-2-3 만큼이나 쉽다.
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// 1 단계. 테스트 하고자 하는 함수(logic)가 선언된 헤더파일을 포함(include)하라.
// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// gtest.h 파일도 꼭 포함하라. (여기에 테스팅 프로임워크가 들어 있다.)
// Don't forget gtest.h, which declares the testing framework.

#include <limits.h>
#include "sample1.h"
#include "gtest/gtest.h"


// 2 단계. TEST 매크로를 사용하여 테스트를 정의(define) 하라.
// Step 2. Use the TEST macro to define your tests.
//
// TEST 는 두 개의 파라미터가 있는데, '테스트 케이스 이름' 과 '테스트 이름' 이다.
// TEST 매크로를 사용한 다음에는, 한 쌍의 중괄호(braces) 사이에 테스트 로직을 
// 넣어야 한다. EXPECT_TRUE 나 EXPECT_EQ 등의 매크로를 사용하여 테스트의 
// 성공 이나 실패를 체크 하면 된다. 매크로 전체 목록은 gtest.h 를 참조하라.
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// Google Test 의 테스트 케이스는 테스트들의 묶음(group)을 나타낸다. 
// 이것으로 테스트 코드를 잘 조직화 할 수 있다. 논리적으로 연관된 테스트들을 
// 같은 테스트 케이스에 넣으면 된다.
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// '테스트 케이스 이름'과 '테스트 이름' 은 C++ 에서 유효한 식별자(identifiers)여야 한다.
// 또한 이름들에 밑줄(_)을 사용해서도 안된다.
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test 는 당신이 정의(define)한 각 테스트들이 한번씩만 수행된다는 것을 보장한다.
// 하지만 그들의 실행 순서는 보장하지 않는다. 그러므로, 그들의 실행 순서에 
// 의존하지 않도록 테스트를 작성해야 한다.
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>


// Factorial() 테스트.
// Tests Factorial().

// 음수에 대한 factorial 테스트.
// Tests factorial of negative numbers.
TEST(FactorialTest, Negative) {
  // This test is named "Negative", and belongs to the "FactorialTest"
  // test case.
  EXPECT_EQ(1, Factorial(-5));
  EXPECT_EQ(1, Factorial(-1));
  EXPECT_GT(Factorial(-10), 0);

  // <TechnicalDetails>
  //
  // EXPECT_EQ(expected, actual) 와 
  //
  //   EXPECT_TRUE((expected) == (actual)) 은 
  //
  // 동일하다. 위의 것은 어썰션(assertion) 실패시에 인자값(expected, actual)을
  // 출력한다는 것만 다르다. 이것이 디버깅에 더 도움이 될것이기에 여기서는 
  // EXPECT_EQ 선호해서 사용한다.
  // 
  // 반면에, EXPECT_TRUE 는 어떠한 불(Boolean) 표현식도 받아 들이므로,
  // 더욱 일반적이다.
  //
  // EXPECT_EQ(expected, actual) is the same as
  //
  //   EXPECT_TRUE((expected) == (actual))
  //
  // except that it will print both the expected value and the actual
  // value when the assertion fails.  This is very helpful for
  // debugging.  Therefore in this case EXPECT_EQ is preferred.
  //
  // On the other hand, EXPECT_TRUE accepts any Boolean expression,
  // and is thus more general.
  //
  // </TechnicalDetails>
}

// 0 에 대한 factorial 테스트. 
// Tests factorial of 0.
TEST(FactorialTest, Zero) {
  EXPECT_EQ(1, Factorial(0));
}

// 양수에 대한 factorial 테스트
// Tests factorial of positive numbers.
TEST(FactorialTest, Positive) {
  EXPECT_EQ(1, Factorial(1));
  EXPECT_EQ(2, Factorial(2));
  EXPECT_EQ(6, Factorial(3));
  EXPECT_EQ(40320, Factorial(8));
}


// IsPrime() 테스트
// Tests IsPrime()

// 음수 입력값을 테스트.
// Tests negative input.
TEST(IsPrimeTest, Negative) {
  // 이 테스트는 테스트 케이스 'IsPrimeTest' 에 속한다.
  // This test belongs to the IsPrimeTest test case.

  EXPECT_FALSE(IsPrime(-1));
  EXPECT_FALSE(IsPrime(-2));
  EXPECT_FALSE(IsPrime(INT_MIN));
}

// 몇몇 사소한(trivial) 경우를 테스트 
// Tests some trivial cases.
TEST(IsPrimeTest, Trivial) {
  EXPECT_FALSE(IsPrime(0));
  EXPECT_FALSE(IsPrime(1));
  EXPECT_TRUE(IsPrime(2));
  EXPECT_TRUE(IsPrime(3));
}

// 양수 입력값을 테스트.
// Tests positive input.
TEST(IsPrimeTest, Positive) {
  EXPECT_FALSE(IsPrime(4));
  EXPECT_TRUE(IsPrime(5));
  EXPECT_FALSE(IsPrime(6));
  EXPECT_TRUE(IsPrime(23));
}

// 단계 3. main() 에서 RUN_ALL_TESTS() 를 호출하라.
//
// 우리는 여기서 src/gtest_main.cc 파일을 링크했다. 그 파일에 RUN_ALL_TESTS()
// 을 호출하는 main() 이 있다.
//
// 거기에서 당신이 정의(define)한 모든 테스트를 실행하고, 결과를 출력하고, 그리고
// 성공시에는 0, 실패시에는 1 을 리턴한다.
//
// 우리가 테스트를 등록하지 않았다는 것을 알아챘는가? RUN_ALL_TESTS() 는
// 만든 테스트들을 머법처럼 안다. 편리하지 않은가?
//
// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
//
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Isn't this convenient?
