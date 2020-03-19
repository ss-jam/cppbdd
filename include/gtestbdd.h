#ifndef GTESTBDD_H_
#define GTESTBDD_H_

#include <gtest/gtest.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

namespace gtestbdd
{
    class Narrative {
    public:
        Narrative(const std::string &description) : mDescription(description) {
            printPart("NARRATIVE: ", mDescription);
        }

        virtual ~Narrative() {
            if(!mAs_a && !mI_want && !mSo_that) {
                return;
            }

            if(!mAs_a) {
                printError("AS_A clause missing.");
                assert(false);
            }

            if(!mI_want) {
                printError("I_WANT clause missing.");
                assert(false);
            }

            if(!mSo_that) {
                printError("SO_THAT clause missing.");
                assert(false);
            }
        }

        void as_a(const std::string &description) {
            mAs_a = true;
            printPart("AS_A ", description);
        }

        void i_want(const std::string &description) {
            if(mAs_a) {
                mI_want = true;
                printPart("I_WANT ", description);
            } else {
                printError("AS_A clause missing.");
                assert(false);
            }
        }

        void so_that(const std::string &description) {
            if(mI_want) {
                mSo_that = true;
                printPart("SO_THAT ", description);
            } else {
                printError("I_WANT clause missing.");
                assert(false);
            }
        }

        void et(const std::string &description) {
            if(mAs_a || mI_want || mSo_that) {
                printPart("AND ", description);
            } else {
                printError("AND must follow one of AS_A, I_WANT, or SO_THAT");
                assert(false);
            }
        }

    private:
        void printError(const std::string &message) {
            std::cout << "ERROR " << mDescription << " " << message << std::endl;
        }

        void printPart(const std::string &predicate, const std::string &description) {
            std::cout << std::right << std::setw(16) << predicate << description << std::endl;
        }

        const std::string mDescription;
        bool mAs_a = false;
        bool mI_want = false;
        bool mSo_that = false;

    };

    class Scenario {
    public:
        Scenario(const std::string &description) :
            mDescription(description) {
            printPart("SCENARIO ", mDescription);
        }

        virtual ~Scenario() {
            if (!mGiven && !mWhen && !mThen) {
                return;
            }

            if(!mGiven) {
                printError("GIVEN clause missing.");
                assert(false);
            }

            if(!mWhen) {
                printError("WHEN clause missing.");
                assert(false);
            }

            if(!mThen) {
                printError("THEN clause missing.");
                assert(false);
            }
        }

        void given(const std::string &description) {
            mGiven = true;
            printPart("GIVEN ", description);
        }

        void when(const std::string &description) {
            if(mGiven) {
                mWhen = true;
                printPart("WHEN ", description);
            } else {
                printError("GIVEN clause missing.");
                assert(false);
            }
        }

        void then(const std::string &description) {
            if(mWhen) {
                mThen = true;
                printPart("THEN ", description);
            } else {
                printError("WHEN clause missing");
                assert(false);
            }
        }

        void et(const std::string &description) {
            if(mGiven || mWhen || mThen) {
                printPart("AND ", description);
            } else {
                printError("AND must follow one of GIVEN, WHEN or THEN");
                assert(false);
            }
        }

    private:
        void printError(const std::string &message) {
            std::cout << "ERROR " << mDescription << " " << message << std::endl;
        }

        void printPart(const std::string &predicate, const std::string &description) {
            std::cout << std::right << std::setw(16) << predicate << description << std::endl;
        }
        
        const std::string mDescription;
        bool mGiven = false;
        bool mWhen = false;
        bool mThen = false;
    };
}

//utility
#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define MAKE_UNIQUE(x) CONCATENATE(x, __LINE__)
#define GTEST_STRINGIFY_(name) #name

//gtestbdd
#define FEATURE(description)\
    TEST(MAKE_UNIQUE(Feature), Init)\
    {\
        std::cout << std::right << std::setw(12) << "FEATURE " << description << std::endl;\
    }\
    namespace MAKE_UNIQUE(Feature)

#define MAKE_NARRATIVE(TestClass, description, FixtureClass) \
    class TestClass : public gtestbdd::Narrative, public FixtureClass\
    {\
    public:\
        TestClass() :\
            gtestbdd::Narrative(description)\
        {\
        }\
    private:\
        virtual void TestBody();\
        static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_;\
        GTEST_DISALLOW_COPY_AND_ASSIGN_(TestClass);\
    };\
    \
    ::testing::TestInfo* const TestClass\
      ::test_info_ =\
        ::testing::internal::MakeAndRegisterTestInfo(\
            #FixtureClass, description, NULL, NULL, \
            ::testing::internal::CodeLocation(__FILE__, __LINE__), \
            (::testing::internal::GetTypeId<FixtureClass>()), \
            FixtureClass::SetUpTestCase, \
            FixtureClass::TearDownTestCase, \
            new ::testing::internal::TestFactoryImpl<\
            TestClass>);\
    void TestClass::TestBody()

#define NARRATIVE(description) MAKE_NARRATIVE(MAKE_UNIQUE(Narrative_Fixture_Line), description, ::testing::Test)

#define AS_A(description)\
    as_a(description);

#define I_WANT(description)\
    i_want(description);

#define SO_THAT(description)\
    so_that(description);

#define MAKE_SCENARIO(TestClass, description, FixtureClass) \
    class TestClass : public gtestbdd::Scenario, public FixtureClass\
    {\
    public:\
        TestClass() :\
            gtestbdd::Scenario(description)\
        {\
        }\
    private:\
        virtual void TestBody();\
        static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_;\
        GTEST_DISALLOW_COPY_AND_ASSIGN_(TestClass);\
    };\
    \
    ::testing::TestInfo* const TestClass\
      ::test_info_ =\
        ::testing::internal::MakeAndRegisterTestInfo(\
            #FixtureClass, description, NULL, NULL, \
            ::testing::internal::CodeLocation(__FILE__, __LINE__), \
            (::testing::internal::GetTypeId<FixtureClass>()), \
            FixtureClass::SetUpTestCase, \
            FixtureClass::TearDownTestCase, \
            new ::testing::internal::TestFactoryImpl<\
            TestClass>);\
    void TestClass::TestBody()



#define MAKE_SCENARIO_P(TestClass, TestName, Description, FixtureClass, Values)\
    class TestClass : public gtestbdd::Scenario, public FixtureClass\
    {\
    public:\
        TestClass() :\
            gtestbdd::Scenario(Description)\
        {\
        }\
        virtual void TestBody();\
    private:\
        static int AddToRegistry() {\
            ::testing::UnitTest::GetInstance()->parameterized_test_registry().\
                GetTestCasePatternHolder<FixtureClass>(\
                    #FixtureClass, __FILE__, __LINE__)->AddTestPattern(\
                        #FixtureClass,\
                        Description,\
                        new ::testing::internal::TestMetaFactory<TestClass>());\
            return 0;\
        }\
        static int gtest_registering_dummy_;\
        GTEST_DISALLOW_COPY_AND_ASSIGN_(TestClass);\
    };\
    \
    int TestClass::gtest_registering_dummy_ =\
        TestClass::AddToRegistry();\
    INSTANTIATE_TEST_CASE_P(TestName, FixtureClass, Values);\
    void TestClass::TestBody()

#define SCENARIO_P_VERBOSE(testname, description, fixture, values) \
    MAKE_SCENARIO_P(\
        MAKE_UNIQUE(Scenario_##fixture##_Line), \
        testname, \
        description, \
        fixture, \
        values)

#define SCENARIO_P(description, fixture, values) \
    SCENARIO_P_VERBOSE(\
        TEST_P, \
        description, \
        fixture, \
        values)

#define SCENARIO_F(description, fixture) MAKE_SCENARIO(MAKE_UNIQUE(Scenario_##fixture##_Line), description, fixture)

#define SCENARIO(description) MAKE_SCENARIO(MAKE_UNIQUE(Scenario_Fixture_Line), description, ::testing::Test)


#define GIVEN(description)\
    given(description);

#define WHEN(description)\
    when(description);

#define THEN(description)\
    then(description);

#define AND(description)\
    et(description);

#endif // GTESTBDD_H_