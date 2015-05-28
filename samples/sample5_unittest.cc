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
//
// Author: wan@google.com (Zhanyong Wan)


// 이 예제는 test fixture 를 상속 받는 sub-fixture 를 이용하여
// 여러 테스트에서 text fixture 를 재사용 하는 방법을 알려준다. 
// This sample teaches how to reuse a test fixture in multiple test
// cases by deriving sub-fixtures from it.
//
// test fixture 를 정의할때, 이 fixture 에서 사용할 테스트 케이스의
// 이름도 명시한다. 그러므로, 하나의 test fixture 은 오직 하나의 테스트 
// 케이스에서만 사용된다.
// When you define a test fixture, you specify the name of the test
// case that will use this fixture.  Therefore, a test fixture can
// be used by only one test case.
//
// 
// 가끔은, 하나 이상의 테스트 케이스에서, 동일하거나 아주 약간만 다른 test fixture
// 를 사용하고 싶을 수도 있다. 예를 들어, GUI 라이브러리를 테스트 하는 모든 테스트에서
// 폰트나 브러시 같은 중요한 시스템의 리소스가 누수되지 않게 하고 싶을수 있다.
// 이럴 경우 Google Test 에서는, 공용 로직을 super test fixture (super 클래스)에 
// 넣은 후, 상속받은 fixture 에서 테스트 케이스를 만들면 된다.
// Sometimes, more than one test cases may want to use the same or
// slightly different test fixtures.  For example, you may want to
// make sure that all tests for a GUI library don't leak important
// system resources like fonts and brushes.  In Google Test, you do
// this by putting the shared logic in a super (as in "super class")
// test fixture, and then have each test case use a fixture derived
// from this super fixture.

#include <limits.h>
#include <time.h>
#include "sample3-inl.h"
#include "gtest/gtest.h"
#include "sample1.h"

// 이번 예제에서는, 각 테스트가 5초 이내에 완료되는 지를 보장하고 싶다. 
// 만약 테스트가 오래 걸린다면, 우린 이것을 실패로 간주해 볼수 있다.
// In this sample, we want to ensure that every test finishes within
// ~5 seconds.  If a test takes longer to run, we consider it a
// failure.
//
// 시간 측정을 하는 코드를 "QuickTest" 라는 test fixture 에 넣었다. 
// QuickTest 는 다른 fixture 에서 상속받을 수 있게 super fixture 로 만들었으므로, 
// "QuickTest"에 대한 테스트 케이스는 없다. 없어도 문제 없다.
// We put the code for timing a test in a test fixture called
// "QuickTest".  QuickTest is intended to be the super fixture that
// other fixtures derive from, therefore there is no test case with
// the name "QuickTest".  This is OK.
//
// 나중에, QuickTest 를 여러 test fixture 에서 상속받을 것이다.
// Later, we will derive multiple test fixtures from QuickTest.
class QuickTest : public testing::Test {
 protected:
  // SetUp() 는 하나의 테스트가 시작되기 바로 직전에 실행된다.
  // 테스트가 시작된 시간을 저장하기 좋은 곳이다.
  // Remember that SetUp() is run immediately before a test starts.
  // This is a good place to record the start time.
  virtual void SetUp() {
    start_time_ = time(NULL);
  }

  // TearDown() 은 하나의 테스트가 완료된 직후에 호출된다. 여기에서 우리는
  // 테스트가 느렸는지를 체크한다.
  // TearDown() is invoked immediately after a test finishes.  Here we
  // check if the test was too slow.
  virtual void TearDown() {
    // 완료 시간을 얻어온다.
    // Gets the time when the test finishes
    const time_t end_time = time(NULL);

    // 테스트가 5초 이상 걸렸는지 검증(assert)한다. 어썰션(assertions)을
    // SetUp() 이나 TearDown() 에서도 사용할 수 있다는 것을 알고 있니?
    // Asserts that the test took no more than ~5 seconds.  Did you
    // know that you can use assertions in SetUp() and TearDown() as
    // well?
    EXPECT_TRUE(end_time - start_time_ <= 5) << "The test took too long.";
  }

  // 테스트가 시작했을때의 UTC 시간(초단위) 
  // The UTC time (in seconds) when the test starts
  time_t start_time_;
};


// QuickTest를 상속 받는 IntegerFunctionTest 라는 test fixture 를 만든다.
// 이것을 사용하는 모든 테스트는 빨리(quick) 실행되어야 한다.
// We derive a fixture named IntegerFunctionTest from the QuickTest
// fixture.  All tests using this fixture will be automatically
// required to be quick.
class IntegerFunctionTest : public QuickTest {
  // QuickTest fixture 에 대부분의 로직이 있으므로  추가할게 없다.
  // 그러므로 몸통(body)이 비었다.
  // We don't need any more logic than already in the QuickTest fixture.
  // Therefore the body is empty.
};


// 이제 IntegerFunctionTest 의 테스트 케이스를 작성한다.
// Now we can write tests in the IntegerFunctionTest test case.

// Factorial() 을 테스트.
// Tests Factorial()
TEST_F(IntegerFunctionTest, Factorial) {
  // 음수에 대한 factorial 을 테스트.
  // Tests factorial of negative numbers.
  EXPECT_EQ(1, Factorial(-5));
  EXPECT_EQ(1, Factorial(-1));
  EXPECT_GT(Factorial(-10), 0);

  // 0 의 factorial 테스트.
  // Tests factorial of 0.
  EXPECT_EQ(1, Factorial(0));

  // 양수에 대한 factorial 테스트.
  // Tests factorial of positive numbers.
  EXPECT_EQ(1, Factorial(1));
  EXPECT_EQ(2, Factorial(2));
  EXPECT_EQ(6, Factorial(3));
  EXPECT_EQ(40320, Factorial(8));
}


//IsPrime() 을 테스트.
// Tests IsPrime()
TEST_F(IntegerFunctionTest, IsPrime) {
  // 음수 입력값을 테스트.
  // Tests negative input.
  EXPECT_FALSE(IsPrime(-1));
  EXPECT_FALSE(IsPrime(-2));
  EXPECT_FALSE(IsPrime(INT_MIN));

  // 일반적인 케이스를 테스트.
  // Tests some trivial cases.
  EXPECT_FALSE(IsPrime(0));
  EXPECT_FALSE(IsPrime(1));
  EXPECT_TRUE(IsPrime(2));
  EXPECT_TRUE(IsPrime(3));

  // 양수 입력값을 테스트.
  // Tests positive input.
  EXPECT_FALSE(IsPrime(4));
  EXPECT_TRUE(IsPrime(5));
  EXPECT_FALSE(IsPrime(6));
  EXPECT_TRUE(IsPrime(23));
}


//
// 이번에 보여줄, "QueueTest" 라고 이름붙은 테스트 케이스들도  
// 빨라야(quick) 한다. 고로 QuickTest 를 상속받는 fixture 를 하나 더 만든다.
// The next test case (named "QueueTest") also needs to be quick, so
// we derive another fixture from QuickTest.
//
// test fixture 인 QueueTest 는 추가적인 로직과  객체를 가지고 있다.
// 그래서, 추가적인 것들을 몸통(body)에 정의했다. 
// The QueueTest test fixture has some logic and shared objects in
// addition to what's in QuickTest already.  We define the additional
// stuff inside the body of the test fixture, as usual.
class QueueTest : public QuickTest {
 protected:
  virtual void SetUp() {
    // 우선, super fixture(QuickTest) 를 준비(set up) 시킨다.
    // First, we need to set up the super fixture (QuickTest).
    QuickTest::SetUp();

    // 그 다음으로, 이 fixture 를 위한 추가적인 준비를(setup) 한다.
    // Second, some additional setup for this fixture.
    q1_.Enqueue(1);
    q2_.Enqueue(2);
    q2_.Enqueue(3);
  }

  // 디폴트로, TearDown() 은 QuickTest::TearDown() 을 상속받는다.
  // QueueTest 에는 추가적인 정리(cleaning)작업이 필요없으므로, 생략한다.
  // By default, TearDown() inherits the behavior of
  // QuickTest::TearDown().  As we have no additional cleaning work
  // for QueueTest, we omit it here.
  //
  // virtual void TearDown() {
  //   QuickTest::TearDown();
  // }

  Queue<int> q0_;
  Queue<int> q1_;
  Queue<int> q2_;
};


// 이제, QueueTest fixture 를 사용하는 테스트를 작성해 보자.
// Now, let's write tests using the QueueTest fixture.

// 기본 생성자를 테스트.
// Tests the default constructor.
TEST_F(QueueTest, DefaultConstructor) {
  EXPECT_EQ(0u, q0_.Size());
}

// Dequeue() 를 테스트.
// Tests Dequeue().
TEST_F(QueueTest, Dequeue) {
  int* n = q0_.Dequeue();
  EXPECT_TRUE(n == NULL);

  n = q1_.Dequeue();
  EXPECT_TRUE(n != NULL);
  EXPECT_EQ(1, *n);
  EXPECT_EQ(0u, q1_.Size());
  delete n;

  n = q2_.Dequeue();
  EXPECT_TRUE(n != NULL);
  EXPECT_EQ(2, *n);
  EXPECT_EQ(1u, q2_.Size());
  delete n;
}

// 필요하다면, 상속 받은 fixture 자체를 다른 fixture 에서 상속 받을 수도 있다.
// 예를 들면, QueueTest 로 부터 상속 받는 또다른 fixture 를 만들수 있다. 
// Google Test 는 상속 계층 구조가 얼마가 되던지 간에, 거기에 대한 대한 제약이 없다.
// If necessary, you can derive further test fixtures from a derived
// fixture itself.  For example, you can derive another fixture from
// QueueTest.  Google Test imposes no limit on how deep the hierarchy
// can be.  In practice, however, you probably don't want it to be too
// deep as to be confusing.
