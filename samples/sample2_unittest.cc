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


// 클래스가 다수의 멤버함수를 가지고 있을 경우의 좀더 복잡한 유닛 테스트를
// 작성하는 법을 보여준다.
// This sample shows how to write a more complex unit test for a class
// that has multiple member functions.
//
// 일반적으로, 클래스의 메소드당 하나의 테스트를 가지는 것이 좋다. 
// 항상 그렇게 할 필요는 없지만, 그렇게 하면 테스트들이 잘 구성되게 도와준다.
// 물론 필요하면 추가적인 테스트를 작성해도 된다.
// Usually, it's a good idea to have one test for each method in your
// class.  You don't have to do that exactly, but it helps to keep
// your tests organized.  You may also throw in additional tests as
// needed.

#include "sample2.h"
#include "gtest/gtest.h"

// In this example, we test the MyString class (a simple string).

// Tests the default c'tor.
TEST(MyString, DefaultConstructor) {
  const MyString s;

  // s.c_string() 이 NULL 을 리턴하는지 검증한다.
  // Asserts that s.c_string() returns NULL.
  //
  // <TechnicalDetails>
  //
  // 검증을 위해, 아래에 있는 것 대신에 NULL 을 사용한다면,
  // If we write NULL instead of
  //
  //   static_cast<const char *>(NULL)
  //
  // gcc 3.4 에는 warning 이 뜰 것이다. EXPECT_EQ 가 실패에 대한 출력을 
  // 하기 위해서는 매개변수들의 타입을 알아야 한다. NULL 은 0 으로 정의되어 
  // 있기 때문에 컴파일러는 int 형 포맷을 사용할 것이다. 하지만 gcc 는 
  // NULL 을 int 가 아닌 포인터로만 다루기 때문에 경고가 뜬다.
  // in this assertion, it will generate a warning on gcc 3.4.  The
  // reason is that EXPECT_EQ needs to know the types of its
  // arguments in order to print them when it fails.  Since NULL is
  // #defined as 0, the compiler will use the formatter function for
  // int to print it.  However, gcc thinks that NULL should be used as
  // a pointer, not an int, and therefore complains.
  //
  // 문제의 근본원인은 C++ 이 integer 숫자 0 과 null pointer constant 를
  // 구분하지 못하기 때문이다. 안타깝지만, 이런 사실은 인정하고 살아야 한다.
  // The root of the problem is C++'s lack of distinction between the
  // integer number 0 and the null pointer constant.  Unfortunately,
  // we have to live with this fact.
  //
  // </TechnicalDetails>
  EXPECT_STREQ(NULL, s.c_string());

  EXPECT_EQ(0u, s.Length());
}

const char kHelloString[] = "Hello, world!";

// Tests the c'tor that accepts a C string.
TEST(MyString, ConstructorFromCString) {
  const MyString s(kHelloString);
  EXPECT_EQ(0, strcmp(s.c_string(), kHelloString));
  EXPECT_EQ(sizeof(kHelloString)/sizeof(kHelloString[0]) - 1,
            s.Length());
}

// Tests the copy c'tor.
TEST(MyString, CopyConstructor) {
  const MyString s1(kHelloString);
  const MyString s2 = s1;
  EXPECT_EQ(0, strcmp(s2.c_string(), kHelloString));
}

// Tests the Set method.
TEST(MyString, Set) {
  MyString s;

  s.Set(kHelloString);
  EXPECT_EQ(0, strcmp(s.c_string(), kHelloString));

  // Set should work when the input pointer is the same as the one
  // already in the MyString object.
  s.Set(s.c_string());
  EXPECT_EQ(0, strcmp(s.c_string(), kHelloString));

  // Can we set the MyString to NULL?
  s.Set(NULL);
  EXPECT_STREQ(NULL, s.c_string());
}
