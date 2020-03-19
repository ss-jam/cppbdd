// Copyrite 2020, Scott Sickles.
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
//     * The names of contributors may not be used to endorse or promote 
// products derived from this software without specific prior written permission.
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

// Compile using: g++ -o example-spec -lgtest example-spec.cc
// Requires dependancies of gtest, gtest-devel, gmock and gmock-devel 
// on RHEL/CentOS/Fedora

#include "include/gtestbdd.h"
#include <gmock/gmock.h>

using ::testing::StartsWith;

class Fixture : public ::testing::Test {
    // The global context for this Feature and it's Scenarios
public:
    static std::string Foo(int region = 0) {
        switch(region) {
            case 0:
                return "Hello, friend!";
            case 1:
                return "Howdy, partner!";
            default:
                return "Ciao, bella mondo!";
        }
    }
};

FEATURE("BDD style unit tests with Google Test") {
    //indentation is there for readability but is otherwise irrelevant

    NARRATIVE("Use Case") {
        AS_A("bean counter")
        AND("human being")
        I_WANT("to add numbers together")
        AND("Greet people")
        SO_THAT("I can pay people appropriately")
        AND("show them respect")
    }

    SCENARIO("Demonstrate simple core features") {
        int x, y;

        GIVEN("Value of 1")
            x = 1;
        AND("Value of 2")
            y = 2;
        WHEN("The values are added together")
            int result = x + y;
        THEN("The result should be 3")
            ASSERT_EQ(3, result);
    }

    SCENARIO("Demonstrate additional scenario with mock matcher") {
        int region;
        Fixture *f;

        GIVEN("A greeting function Foo()")
        AND("A region 2")
            region = 2;
        WHEN("The greeting is requested for the region")
        THEN("The result should start with Ciao")
        EXPECT_THAT(f->Foo(region), StartsWith("Ciao"));
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
