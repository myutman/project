/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_BinaryOperationTest_init = false;
#include "/home/myutman/Documents/SPbAU/cpp/project/unittest/test.h"

static BinaryOperationTest suite_BinaryOperationTest;

static CxxTest::List Tests_BinaryOperationTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_BinaryOperationTest( "unittest/test.h", 9, "BinaryOperationTest", suite_BinaryOperationTest, Tests_BinaryOperationTest );

static class TestDescription_suite_BinaryOperationTest_test_add : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_BinaryOperationTest_test_add() : CxxTest::RealTestDescription( Tests_BinaryOperationTest, suiteDescription_BinaryOperationTest, 11, "test_add" ) {}
 void runTest() { suite_BinaryOperationTest.test_add(); }
} testDescription_suite_BinaryOperationTest_test_add;

static class TestDescription_suite_BinaryOperationTest_test_sub : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_BinaryOperationTest_test_sub() : CxxTest::RealTestDescription( Tests_BinaryOperationTest, suiteDescription_BinaryOperationTest, 15, "test_sub" ) {}
 void runTest() { suite_BinaryOperationTest.test_sub(); }
} testDescription_suite_BinaryOperationTest_test_sub;

static class TestDescription_suite_BinaryOperationTest_test_mult : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_BinaryOperationTest_test_mult() : CxxTest::RealTestDescription( Tests_BinaryOperationTest, suiteDescription_BinaryOperationTest, 19, "test_mult" ) {}
 void runTest() { suite_BinaryOperationTest.test_mult(); }
} testDescription_suite_BinaryOperationTest_test_mult;

static class TestDescription_suite_BinaryOperationTest_test_eq : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_BinaryOperationTest_test_eq() : CxxTest::RealTestDescription( Tests_BinaryOperationTest, suiteDescription_BinaryOperationTest, 23, "test_eq" ) {}
 void runTest() { suite_BinaryOperationTest.test_eq(); }
} testDescription_suite_BinaryOperationTest_test_eq;

static class TestDescription_suite_BinaryOperationTest_test_neq : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_BinaryOperationTest_test_neq() : CxxTest::RealTestDescription( Tests_BinaryOperationTest, suiteDescription_BinaryOperationTest, 27, "test_neq" ) {}
 void runTest() { suite_BinaryOperationTest.test_neq(); }
} testDescription_suite_BinaryOperationTest_test_neq;

static UnaryOperationTest suite_UnaryOperationTest;

static CxxTest::List Tests_UnaryOperationTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_UnaryOperationTest( "unittest/test.h", 32, "UnaryOperationTest", suite_UnaryOperationTest, Tests_UnaryOperationTest );

static class TestDescription_suite_UnaryOperationTest_test_neg : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UnaryOperationTest_test_neg() : CxxTest::RealTestDescription( Tests_UnaryOperationTest, suiteDescription_UnaryOperationTest, 34, "test_neg" ) {}
 void runTest() { suite_UnaryOperationTest.test_neg(); }
} testDescription_suite_UnaryOperationTest_test_neg;

static class TestDescription_suite_UnaryOperationTest_test_not : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UnaryOperationTest_test_not() : CxxTest::RealTestDescription( Tests_UnaryOperationTest, suiteDescription_UnaryOperationTest, 38, "test_not" ) {}
 void runTest() { suite_UnaryOperationTest.test_not(); }
} testDescription_suite_UnaryOperationTest_test_not;

static ConditionalTest suite_ConditionalTest;

static CxxTest::List Tests_ConditionalTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_ConditionalTest( "unittest/test.h", 43, "ConditionalTest", suite_ConditionalTest, Tests_ConditionalTest );

static class TestDescription_suite_ConditionalTest_test_true : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ConditionalTest_test_true() : CxxTest::RealTestDescription( Tests_ConditionalTest, suiteDescription_ConditionalTest, 45, "test_true" ) {}
 void runTest() { suite_ConditionalTest.test_true(); }
} testDescription_suite_ConditionalTest_test_true;

static class TestDescription_suite_ConditionalTest_test_false : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ConditionalTest_test_false() : CxxTest::RealTestDescription( Tests_ConditionalTest, suiteDescription_ConditionalTest, 49, "test_false" ) {}
 void runTest() { suite_ConditionalTest.test_false(); }
} testDescription_suite_ConditionalTest_test_false;

static class TestDescription_suite_ConditionalTest_test_true_empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ConditionalTest_test_true_empty() : CxxTest::RealTestDescription( Tests_ConditionalTest, suiteDescription_ConditionalTest, 53, "test_true_empty" ) {}
 void runTest() { suite_ConditionalTest.test_true_empty(); }
} testDescription_suite_ConditionalTest_test_true_empty;

static class TestDescription_suite_ConditionalTest_test_false_empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ConditionalTest_test_false_empty() : CxxTest::RealTestDescription( Tests_ConditionalTest, suiteDescription_ConditionalTest, 57, "test_false_empty" ) {}
 void runTest() { suite_ConditionalTest.test_false_empty(); }
} testDescription_suite_ConditionalTest_test_false_empty;

static ReferenceTest suite_ReferenceTest;

static CxxTest::List Tests_ReferenceTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_ReferenceTest( "unittest/test.h", 64, "ReferenceTest", suite_ReferenceTest, Tests_ReferenceTest );

static class TestDescription_suite_ReferenceTest_test_ref : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ReferenceTest_test_ref() : CxxTest::RealTestDescription( Tests_ReferenceTest, suiteDescription_ReferenceTest, 66, "test_ref" ) {}
 void runTest() { suite_ReferenceTest.test_ref(); }
} testDescription_suite_ReferenceTest_test_ref;

static ScopeTest suite_ScopeTest;

static CxxTest::List Tests_ScopeTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_ScopeTest( "unittest/test.h", 74, "ScopeTest", suite_ScopeTest, Tests_ScopeTest );

static class TestDescription_suite_ScopeTest_test_scope : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ScopeTest_test_scope() : CxxTest::RealTestDescription( Tests_ScopeTest, suiteDescription_ScopeTest, 76, "test_scope" ) {}
 void runTest() { suite_ScopeTest.test_scope(); }
} testDescription_suite_ScopeTest_test_scope;

static FunctionTest suite_FunctionTest;

static CxxTest::List Tests_FunctionTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_FunctionTest( "unittest/test.h", 85, "FunctionTest", suite_FunctionTest, Tests_FunctionTest );

static class TestDescription_suite_FunctionTest_test_func : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_FunctionTest_test_func() : CxxTest::RealTestDescription( Tests_FunctionTest, suiteDescription_FunctionTest, 87, "test_func" ) {}
 void runTest() { suite_FunctionTest.test_func(); }
} testDescription_suite_FunctionTest_test_func;

static class TestDescription_suite_FunctionTest_test_empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_FunctionTest_test_empty() : CxxTest::RealTestDescription( Tests_FunctionTest, suiteDescription_FunctionTest, 96, "test_empty" ) {}
 void runTest() { suite_FunctionTest.test_empty(); }
} testDescription_suite_FunctionTest_test_empty;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
