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


// 이번 예제에서는, Google Test의 고급 기능인 test fixture 대해 설명한다.
// In this example, we use a more advanced feature of Google Test called
// test fixture.
//
// test fixture 는 테스트 케이스의 모든 테스트가 공유하여 사용할 객체나 
// 함수들을 모아 놓은 곳이다.
// A test fixture is a place to hold objects and functions shared by
// all tests in a test case.  Using a test fixture avoids duplicating
// the test code necessary to initialize and cleanup those common
// objects for each test.  It is also useful for defining sub-routines
// that your tests need to invoke a lot.
//
// <TechnicalDetails>
//
// 테스트들을 코드 공유 관점에만 test fixture 를 공유한다. 데이터 공유는
// 하지 않는다. 각 테스트은 새로운 fixture 를 할당받게 된다. 하나의 
// 테스트에서 변경된 데이터가 다른 테스트로 넘어갈 거란 생각은 하지 마라.
// 나쁜 생각이다.
// The tests share the test fixture in the sense of code sharing, not
// data sharing.  Each test is given its own fresh copy of the
// fixture.  You cannot expect the data modified by one test to be
// passed on to another test, which is a bad idea.
//
// 이렇게 디자인한 이유는 테스트들이 독립적이어야하고 반복가능해야 하기 
// 때문이다. 특히, 테스트는 다른 테스트의 실패로 인한 영향을 받으면 안된다.
// 만약 다른 테스트가 만든 정보에 의존해야 한다면, 그 테스트를 포함하는 
// 하나의 큰 테스트를 만들어야 한다.
// The reason for this design is that tests should be independent and
// repeatable.  In particular, a test should not fail as the result of
// another test's failure.  If one test depends on info produced by
// another test, then the two tests should really be one big test.
//
// 성공과 실패를 나타내는 매크로(EXPECT_TRUE, FAIL 등)는 현재 하고 있는 
// 테스트가 무엇인지 알아야 한다(Google Test 가 테스트 결과를 출력할때 어떤 
// 테스트에 실패한 것인지 알려준다). 기술적으로 보면, 이러한 매크로들은 
// Test 클래스의 맴버 함수를 호출한다. 따라서, 이 매크로들을 전역함수에서
// 사용할 수 없다. 그러므로 테스트용 보조-루틴들은 test fixture 안에 있어야만 한다.
// The macros for indicating the success/failure of a test
// (EXPECT_TRUE, FAIL, etc) need to know what the current test is
// (when Google Test prints the test result, it tells you which test
// each failure belongs to).  Technically, these macros invoke a
// member function of the Test class.  Therefore, you cannot use them
// in a global function.  That's why you should put test sub-routines
// in a test fixture.
//
// </TechnicalDetails>

#include "sample3-inl.h"
#include "gtest/gtest.h"

// test fixture 를 사용하려면, testing::Test 를 상속받아라.
class QueueTest : public testing::Test {
 protected:  // 멤버를 protected 로 해서 보조-클래스들에서
             // 접근할수 있게 해야 한다.
             // You should make the members protected s.t. they can be
             // accessed from sub-classes.
             
  // virtual void SetUp() 은 각 테스트가 실행되기 전에 호출된다. 변수들을
  // 초기화 할 필요가 있을때 정의하면 된다. 초기화할게 없다면, 무시해도 된다.
  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
    q1_.Enqueue(1);
    q2_.Enqueue(2);
    q2_.Enqueue(3);
  }
  
  // virtual void TearDown() 은 각 테스트가 실행된 후에 호출된다. 
  // 뒷정리 해야 할게 있을때 정의하면 된다. 뒷정리 할게 없다면 
  // 무시하면 된다.
  // virtual void TearDown() will be called after each test is run.
  // You should define it if there is cleanup work to do.  Otherwise,
  // you don't have to provide it.
  //
  // virtual void TearDown() {
  // }

  // 몇몇 테스트가 사용할 헬퍼함수.
  // A helper function that some test uses.
  static int Double(int n) {
    return 2*n;
  }

  // Queue::Map() 을 테스트하기 위한 헬퍼함수
  // A helper function for testing Queue::Map().
  void MapTester(const Queue<int> * q) {
    // 새로운 큐를 만들고, 각 요소를 기존 q에 있는 요소에 두배한 값으로 
    // 채운다.
    // Creates a new queue, where each element is twice as big as the
    // corresponding one in q.
    const Queue<int> * const new_q = q->Map(Double);

    // 새로운 큐가 q와 같은 사이즈인지 검증한다.
    // Verifies that the new queue has the same size as q.
    ASSERT_EQ(q->Size(), new_q->Size());

    // 두 큐에 있는 각 요소들의 관계를 검즘한다.
    // Verifies the relationship between the elements of the two queues.
    for ( const QueueNode<int> * n1 = q->Head(), * n2 = new_q->Head();
          n1 != NULL; n1 = n1->next(), n2 = n2->next() ) {
      EXPECT_EQ(2 * n1->element(), n2->element());
    }

    delete new_q;
  }

  // 테스트가 사용할 변수들을 선언한다.
  // Declares the variables your tests want to use.
  Queue<int> q0_;
  Queue<int> q1_;
  Queue<int> q2_;
};

// test fixture 가 있다면, TEST 대신에 TEST_F 를 사용하여 테스트를
// 정의하라.
// When you have a test fixture, you define a test using TEST_F
// instead of TEST.

// 기본생성자를 테스트한다.
// Tests the default c'tor.
TEST_F(QueueTest, DefaultConstructor) {
  // You can access data in the test fixture here.
  EXPECT_EQ(0u, q0_.Size());
}

// Dequeue() 를 테스트한다.
// Tests Dequeue().
TEST_F(QueueTest, Dequeue) {
  int * n = q0_.Dequeue();
  EXPECT_TRUE(n == NULL);

  n = q1_.Dequeue();
  ASSERT_TRUE(n != NULL);
  EXPECT_EQ(1, *n);
  EXPECT_EQ(0u, q1_.Size());
  delete n;

  n = q2_.Dequeue();
  ASSERT_TRUE(n != NULL);
  EXPECT_EQ(2, *n);
  EXPECT_EQ(1u, q2_.Size());
  delete n;
}

// Queue::Map() 함수를 테스트한다.
// Tests the Queue::Map() function.
TEST_F(QueueTest, Map) {
  MapTester(&q0_);
  MapTester(&q1_);
  MapTester(&q2_);
}
