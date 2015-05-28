// Copyright 2008 Google Inc.
// All Rights Reserved.
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

// 이 예제에서는, 인터페이스의 여러 구현체(implementations)가 있을 경우
// 공통된 속성을 테스트 하는 방법을 보여준다. (일명, 인터페이스 테스트)
// This sample shows how to test common properties of multiple
// implementations of the same interface (aka interface tests).

// 인터페이스와 그 구현체들(implementations)이 있는 헤더 파일.
// The interface and its implementations are in this header.
#include "prime_tables.h"

#include "gtest/gtest.h"

// 우선, 구현체들의 인스턴스를 생성하는 팩토리 함수를 정의한다. 만약 당신의 
// 구현체들을 동일한 방식으로 생성할수 있다면 생략할수 있다.
// First, we define some factory functions for creating instances of
// the implementations.  You may be able to skip this step if all your
// implementations can be constructed the same way.

template <class T>
PrimeTable* CreatePrimeTable();

template <>
PrimeTable* CreatePrimeTable<OnTheFlyPrimeTable>() {
  return new OnTheFlyPrimeTable;
}

template <>
PrimeTable* CreatePrimeTable<PreCalculatedPrimeTable>() {
  return new PreCalculatedPrimeTable(10000);
}

// test fixture 를 템플릿 클래스로 정의한다.
// Then we define a test fixture class template.
template <class T>
class PrimeTableTest : public testing::Test {
 protected:
  // 생성자가 타입 T 의 팩토리 함수를 호출하여 해당하는 구현체의 
  // 소수(prime) 테이블을 만든다.
  // The ctor calls the factory function to create a prime table
  // implemented by T.
  PrimeTableTest() : table_(CreatePrimeTable<T>()) {}

  virtual ~PrimeTableTest() { delete table_; }

  // 우리는 구현체(implementation)를 테스트 하기 위해, 
  // 실제 구현체 클래스가 아닌 베이스 인터페이스를 통해서 한다는 것에 주목하라.
  // 현실 세계에서도 베이스 인터페이스를 통해 구현체를 호출하기 때문에 현실과
  // 유사하게 유지하는게 중요하다. 구현체에도 베이스 인터페이스의 함수와 
  // 이름이 같은(인자값의 타입은 약간 다르고) 함수가 있다면, 구현체에서 베이스의 
  // 함수를 가릴 수 있기 때문에 실수할 가능성을 줄여준다.
  // Note that we test an implementation via the base interface
  // instead of the actual implementation class.  This is important
  // for keeping the tests close to the real world scenario, where the
  // implementation is invoked via the base interface.  It avoids
  // got-yas where the implementation class has a method that shadows
  // a method with the same name (but slightly different argument
  // types) in the base interface, for example.
  PrimeTable* const table_;
};

#if GTEST_HAS_TYPED_TEST

using testing::Types;

// Google Test 는 다른 타입들을 테스트 할수 있도록, 테스트를 재사용하는 방법 
// 두가지를 제공한다. 그 첫째가 "typed 테스트"이다. 테스트에 사용할 
// *모든* 타입들을 이미 알고 있을때만 사용해야한다. 
// Google Test offers two ways for reusing tests for different types.
// The first is called "typed tests".  You should use it if you
// already know *all* the types you are gonna exercise when you write
// the tests.

// 'typed 테스트 케이스'를 작성하려면, 먼저 아래 매크로를 사용하여 선언(declare)하고
//
//   TYPED_TEST_CASE(TestCaseName, TypeList);
//
// 타입 파라미터를 지정하라. TEST_F 에서처럼 TestCaseName 는 
// 'test fixture 이름'과 일치해야 한다.
//
// To write a typed test case, first use
//
//   TYPED_TEST_CASE(TestCaseName, TypeList);
//
// to declare it and specify the type parameters.  As with TEST_F,
// TestCaseName must match the test fixture name.

// 테스트할 타입 목록
// The list of types we want to test.
typedef Types<OnTheFlyPrimeTable, PreCalculatedPrimeTable> Implementations;

TYPED_TEST_CASE(PrimeTableTest, Implementations);

// 'typed 테스트'를 정의(define)하기 위해 TYPED_TEST(TestCaseName, TestName)
// 를 사용하라. TEST_F 와 비슷하다.
// Then use TYPED_TEST(TestCaseName, TestName) to define a typed test,
// similar to TEST_F.
TYPED_TEST(PrimeTableTest, ReturnsFalseForNonPrimes) {
  // 테스트 몸통(body)에서는, 타입 파라미터를 TypeParam 로 참조할수 있고,
  // fixture 클래스를 TestFixture 로 참조할수 있다. 이 예제에서는 그것들이
  // 필요없다.
  // Inside the test body, you can refer to the type parameter by
  // TypeParam, and refer to the fixture class by TestFixture.  We
  // don't need them in this example.

  // 우리가 템플릿 세상안에 있기 때문에, C++ 에서 fixture 클래스의 멤버를
  // 참조하려면 'this->' 을 명시적으로 사용해야 한다.  이것이 함께 살아가기 위해
  // 배워야할 것들이다.
  // Since we are in the template world, C++ requires explicitly
  // writing 'this->' when referring to members of the fixture class.
  // This is something you have to learn to live with.
  EXPECT_FALSE(this->table_->IsPrime(-5));
  EXPECT_FALSE(this->table_->IsPrime(0));
  EXPECT_FALSE(this->table_->IsPrime(1));
  EXPECT_FALSE(this->table_->IsPrime(4));
  EXPECT_FALSE(this->table_->IsPrime(6));
  EXPECT_FALSE(this->table_->IsPrime(100));
}

TYPED_TEST(PrimeTableTest, ReturnsTrueForPrimes) {
  EXPECT_TRUE(this->table_->IsPrime(2));
  EXPECT_TRUE(this->table_->IsPrime(3));
  EXPECT_TRUE(this->table_->IsPrime(5));
  EXPECT_TRUE(this->table_->IsPrime(7));
  EXPECT_TRUE(this->table_->IsPrime(11));
  EXPECT_TRUE(this->table_->IsPrime(131));
}

TYPED_TEST(PrimeTableTest, CanGetNextPrime) {
  EXPECT_EQ(2, this->table_->GetNextPrime(0));
  EXPECT_EQ(3, this->table_->GetNextPrime(2));
  EXPECT_EQ(5, this->table_->GetNextPrime(3));
  EXPECT_EQ(7, this->table_->GetNextPrime(5));
  EXPECT_EQ(11, this->table_->GetNextPrime(7));
  EXPECT_EQ(131, this->table_->GetNextPrime(128));
}

// 이제 전부다. Google Test 가 TYPED_TEST_CASE 에 지정된 
// 각 타입별로 TYPED_TEST 를 반복할 것이다. 당신이 여러번 정의(define)할 
// 필요가 없다. 의자에 편히 누워 행복을 만끽하라.
// That's it!  Google Test will repeat each TYPED_TEST for each type
// in the type list specified in TYPED_TEST_CASE.  Sit back and be
// happy that you don't have to define them multiple times.

#endif  // GTEST_HAS_TYPED_TEST

#if GTEST_HAS_TYPED_TEST_P

using testing::Types;

// 하지만 가끔 테스트할 모든 타입을 아직 모르는 경우가 있다. 예를들어, 당신이
// 인터페이스를 만들고 다른 사람들이 그것을 구현(implement)하기를 원하는 경우일때,
// 각각의 구현체가 몇몇 기본적인 요구사항을 만족하는 지를 체크하기 위해 
// 당신이 직접 테스트를 작성하고 싶을 수도 있다. 하지만 당신은 미래에 쓰여질 
// 구현체(implementations)에 대해서는 모른다.
// Sometimes, however, you don't yet know all the types that you want
// to test when you write the tests.  For example, if you are the
// author of an interface and expect other people to implement it, you
// might want to write a set of tests to make sure each implementation
// conforms to some basic requirements, but you don't know what
// implementations will be written in the future.
//
// 타입 파라미터를 결정하지 않고 어떻게 테스트를 작성할수 있을까? 
// 이것이 바로 "type-parameterized 테스트" 가 하는 일이다. 
// 'typed tests' 보다는 좀더 관여를 하긴 하지만, 이것을 통해 얻은 테스트 패턴을
// 여러 컨텍스트에서 재사용할 수 있다. 아주 큰 이득이다. 자, 어떻게 하는 지를 보자.
// How can you write the tests without committing to the type
// parameters?  That's what "type-parameterized tests" can do for you.
// It is a bit more involved than typed tests, but in return you get a
// test pattern that can be reused in many contexts, which is a big
// win.  Here's how you do it:

// 먼저, test fixture 를 템플릿 클래스로 정의한다. 여기서는 
// 위에서 정의한 PrimeTableTest 을 재사용한다.
// First, define a test fixture class template.  Here we just reuse
// the PrimeTableTest fixture defined earlier:

template <class T>
class PrimeTableTest2 : public PrimeTableTest<T> {
};

// 그 다음으로는, 테스트 케이스를 선언(declare)한다. 인자값으로는 test fixture 의 
// 이름을 넣는다. (늘 그렇듯이, 테스트 케이스의 이름이기도 하다.). 접미사 _P 는 
// "parameterized" 나 "pattern" 을 의미한다.
// Then, declare the test case.  The argument is the name of the test
// fixture, and also the name of the test case (as usual).  The _P
// suffix is for "parameterized" or "pattern".
TYPED_TEST_CASE_P(PrimeTableTest2);

// 다음으로, TYPED_TEST_P(TestCaseName, TestName) 를 사용하여 테스트를
// 정의(define)하라. TEST_F 를 사용할때와 유사하다.
// Next, use TYPED_TEST_P(TestCaseName, TestName) to define a test,
// similar to what you do with TEST_F.
TYPED_TEST_P(PrimeTableTest2, ReturnsFalseForNonPrimes) {
  EXPECT_FALSE(this->table_->IsPrime(-5));
  EXPECT_FALSE(this->table_->IsPrime(0));
  EXPECT_FALSE(this->table_->IsPrime(1));
  EXPECT_FALSE(this->table_->IsPrime(4));
  EXPECT_FALSE(this->table_->IsPrime(6));
  EXPECT_FALSE(this->table_->IsPrime(100));
}

TYPED_TEST_P(PrimeTableTest2, ReturnsTrueForPrimes) {
  EXPECT_TRUE(this->table_->IsPrime(2));
  EXPECT_TRUE(this->table_->IsPrime(3));
  EXPECT_TRUE(this->table_->IsPrime(5));
  EXPECT_TRUE(this->table_->IsPrime(7));
  EXPECT_TRUE(this->table_->IsPrime(11));
  EXPECT_TRUE(this->table_->IsPrime(131));
}

TYPED_TEST_P(PrimeTableTest2, CanGetNextPrime) {
  EXPECT_EQ(2, this->table_->GetNextPrime(0));
  EXPECT_EQ(3, this->table_->GetNextPrime(2));
  EXPECT_EQ(5, this->table_->GetNextPrime(3));
  EXPECT_EQ(7, this->table_->GetNextPrime(5));
  EXPECT_EQ(11, this->table_->GetNextPrime(7));
  EXPECT_EQ(131, this->table_->GetNextPrime(128));
}

// 'Type-parameterized 테스트' 에서는 한 단계가 더 필요한데, 
// 당신이 정의(define)한 테스트들을 나열해야 하는 것이다.
// Type-parameterized tests involve one extra step: you have to
// enumerate the tests you defined:
REGISTER_TYPED_TEST_CASE_P(
    PrimeTableTest2, // 첫번째 인자값은 '테스트 케이스 이름'이다. 
                     // The first argument is the test case name.
    // 나머지 인자값들은 테스트들의 이름이다.
    // The rest of the arguments are the test names.
    ReturnsFalseForNonPrimes, ReturnsTrueForPrimes, CanGetNextPrime);

// 현재 시점에서 테스트 패턴은 완료되었다. 그러나, 테스트에 어떤 타입을 사용할지를
// 알려주지 않았기 때문에 아직 어떠한 실제적인 테스트도 가지고 있지 않다. 
// At this point the test pattern is done.  However, you don't have
// any real test yet as you haven't said which types you want to run
// the tests with.

// 추상적인 테스트 패턴을 실제적인 테스트로 바꾸기 위해서는, 타입의 목록으로 
// 인스턴스화 해야 한다. 테스트 패턴은 대개 .h 파일에 정의되어 있을 것이고, 누구나
// 인클루드(#include)하고 인스턴스화 할수 있다. 또한 한 프로그램내에서 여러번 
// 인스턴스화 할 수도 있다. 인스턴스들을 서로 구분하기 위해서 각각에 이름을 주어야 한다.
// '인스턴스 이름'은 '테스트 케이스 이름'의 일부가 되고, 테스트 필터에서 사용된다.
// To turn the abstract test pattern into real tests, you instantiate
// it with a list of types.  Usually the test pattern will be defined
// in a .h file, and anyone can #include and instantiate it.  You can
// even instantiate it more than once in the same program.  To tell
// different instances apart, you give each of them a name, which will
// become part of the test case name and can be used in test filters.

// 테스트 할 타입 목록. 주목할 사항은, TYPED_TEST_P() 를 만들 당시에 정의(define)할 
// 필요가 없다는 접이다. 
// The list of types we want to test.  Note that it doesn't have to be
// defined at the time we write the TYPED_TEST_P()s.
typedef Types<OnTheFlyPrimeTable, PreCalculatedPrimeTable>
    PrimeTableImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(OnTheFlyAndPreCalculated,    // 인스턴스 이름
                              PrimeTableTest2,             // 테스트 케이스 이름
                              PrimeTableImplementations);  // 타입 목록

#endif  // GTEST_HAS_TYPED_TEST_P
