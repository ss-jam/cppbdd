# CppBdd #
CppBdd is a [BDD](https://en.wikipedia.org/wiki/Behavior-driven_development) style unit test extension for popular C++ unit testing frameworks licensed under MIT open source license. It is a derivative work based on Michael Valch's cppbdd. I limited
the scope to googletest framework and added ability to include Narratives such as
a user story for the scenario.


## Introduction ##
There are number of unit testing frameworks available like [Google Test]( https://github.com/google/googletest ), CppUnit, QTest, Catch and many others. Only few of them support BDD in some form (e.g. Catch, Igloo) and typically come as their own framework making it harder to adopt for projects that are already using something else. This implementation of CppBdd is a collection of extensions for the popular
testing frameworks GoogleTest. These extensions offer BDD style Scenarios inside the framework. Developers can mix TDD and BDD approaches or adopt BDD gradually or just take it for a spin and see how things turn out.


## Overview ##

* include/

Contains header-only implementation of BDD for the GoogleTest (gtest) framework.

* Example: example-spec.cc

An example source file demonstrating the usage.
__Dependencies__:
* Requires the gtest framework to compile.
* g++ -o example-spec -lgtest example-spec.cc


## Supported Frameworks ##

* [Google Test]( https://github.com/google/googletest )

Available as **include/gtestbdd.h**

* [CppUnit]( https://sourceforge.net/projects/cppunit/ )

WIP - possibly

## Requirements ##

* C++11 capable compiler
* Testing framework (see above)


## Reference ##

* FEATURE(description)

(Optional) Scenarios are typically bundled into features. This lets you declare the feature.

* NARRATIVE(description)

Declares the user story narrative of this feature.

    * AS_A(description)

    (Mandatory) Description of the user story actor

    * I_WANT(description)

    (Mandatory) Description of the desired action

    * SO_THAT(description)

    (Mandatory) Description of the expected result

* SCENARIO(description)

Declares the test using the fixture. Must be followed by {} that will hold all of the steps.

    * GIVEN(description)

    (Mandatory) Description of first step of the scenario typically setting things up. Code can precede this if it does not fit into its description.

    * WHEN(description)

    (Mandatory) Description of action that is being taken. Must follow at least one GIVEN.

    * THEN(description)

    (Mandatory) Verification of outputs.

    * AND(description)

    (Optional) Can follow any of GIVEN, WHEN or THEN. Cannot be first. Helps separate logical sections of the scenario or given step.


## Usage ##

CppBdd is header only based set of macros that are named after BDD custom and enforces structuring of tests for human readability and understandability: 

Example using BDD with Google Test:

```cpp
#include "inclulde/gtestbdd.h"
#include <gmock/gmock.h>

using ::testing::StartsWith;

class Fixture : public ::testing::Test {
    // The global context for this Feature and it's Scenarios
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
        AND("respect them")
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
```

First, create a `Fixture` class that glues the scenarios together in the feature and provides, for example, mock objects or other initializations. It can of course contain usual Google Test SetUpTestCase, SetUp, TearDown and TearDownTestCase methods and they will be executed as usual. NARRATIVE and SCENARIO declare TEST_F as part of the usual test suite of the framework but adds the ability to call BDD style methods. These track the scenario making sure it is well formed, that is, a correct sequence of steps, and contains all the required steps.

## Framework Specific Notes ##

* Google Test

### Parameterized SCENARIO

Parameterized scenarios require a fixture and can be achieved using either of 

```cpp
SCENARIO_P("This is my scenario description", MyUniqueFixture, Values()) {
}
```
or
```cpp
SCENARIO_P_VERBOSE(ExplicitTestName, "This is my scenario description", MyFixture, Values(..)) {
}
```

Both of these macros deal with `INSTANTIATE_TEST_CASE_P`, so that it is not required in the scenarios.
They differ only in what appears in the output. `SCENARIO_P` will, by default, prefix test output with
`TEST_P` and assumes the fixture name and description are sufficient enough to give meaningful test output.
In the case where this is not sufficient, a scenario can substitute additional information for the `ExplicitTestName` placeholder.

Currently fixtures need to be unique to the parameterized scenario, otherwise it can lead to cross-talk between tests that share
fixtures which may not be desirable. The simplest way to avoid this happening in the case where a fixture is shared
is to prefix `SCENARIO_P` with

```cpp
using UniqueFixtureName = SharedFixture;
```

for example 

```cpp
using UniqueFixtureName = SharedFixture;
const auto emails = Values("£not#valid!!", "valid@email.com")
SCENARIO_P("User updates their email address", UniqueFixtureName, emails) {
}
```

### DISABLED tests

To disable a `SCENARIO`, just prefix the scenario string with `"DISABLED_"` as you would for a regular TEST
In the output you will see the number of tests disabled as usual.

```bash
  YOU HAVE 1 DISABLED TESTS
```

* CppUnit

Nothing yet
