#include "model.h"

bool compare(Object* expr, int value){
	Scope scope;
	Number *num = dynamic_cast<Number*>(expr->evaluate(scope));
	return num->get_val() == value;
}

class BinaryOperationTest : public CxxTest::TestSuite {
public:
	void test_add(){
		TS_ASSERT(compare(new BinaryOperation(new Number(1), "+", new Number(1)), 2));
	}
	
	void test_sub(){
		TS_ASSERT(compare(new BinaryOperation(new Number(3), "-", new Number(1)), 2));
	}
	
	void test_mult(){
		TS_ASSERT(compare(new BinaryOperation(new Number(2), "*", new Number(1)), 2));
	}	
		
	void test_eq(){
		TS_ASSERT(!compare(new BinaryOperation(new Number(1), "==", new Number(1)), false) && !compare(new BinaryOperation(new Number(1), "==", new Number(1)), 0));
	}
	
	void test_neq(){
		TS_ASSERT(compare(new BinaryOperation(new Number(1), "!=", new Number(1)), false) || compare(new BinaryOperation(new Number(1), "!=", new Number(1)), 0));
	}
};

class UnaryOperationTest : public CxxTest::TestSuite {
public:
	void test_neg(){
		TS_ASSERT(compare(new UnaryOperation("-", new Number(1)), -1));
	}
	
	void test_not(){
		TS_ASSERT(compare(new UnaryOperation("!", new Number(2)), false) || compare(new UnaryOperation("!", new Number(2)), 0));
	}
};

class ConditionalTest : public CxxTest::TestSuite {
public:
	void test_true(){
		TS_ASSERT(compare(new Conditional(new Number(1), new vector<Object*>{new Number(2)}, new vector<Object*>{new Number(3)}), 2));
	}
	
	void test_false(){
		TS_ASSERT(compare(new Conditional(new Number(0), new vector<Object*>{new Number(2)}, new vector<Object*>{new Number(3)}), 3));
	}
	
	void test_true_empty(){
		TS_ASSERT(compare(new Conditional(new Number(1), new vector<Object*>{new Number(2)}), 2));
	}
	
	void test_false_empty(){
		Scope scope;
		Conditional(new Number(0), new vector<Object*>{new Number(2)}).evaluate(scope);
	}
	
};

class ReferenceTest : public CxxTest::TestSuite {
public:
	void test_ref(){
		Scope scope;
		scope["a"] = new Number(3);
		TS_ASSERT(compare(Reference("a").evaluate(scope), 3));
	}

};

class ScopeTest : public CxxTest::TestSuite {
public:
	void test_scope(){
		Scope par;
		par["a"] = new Number(1);
		Scope sc(&par);
		sc["b"] = new Number(2);
		TS_ASSERT(compare(BinaryOperation(new Reference("a"), "+", new Reference("b")).evaluate(sc), 3));
	}
};

class FunctionTest : public CxxTest::TestSuite {
public:
	void test_func(){
		//Example
		Scope parent;
		parent["bar"] = new Number(10);
		Scope scope(&parent);
		parent["foo"] = new FunctionAST(new vector<string>{"hello", "world"}, new vector<Object *>{new BinaryOperation(new Reference("hello"), "+", new Reference("world"))});
		TS_ASSERT(compare(FunctionCall(new FunctionDefinition("foo", dynamic_cast<FunctionAST *>(parent["foo"])), new vector<Object *>{new Number(2), new UnaryOperation("-", new Number(3))}).evaluate(scope), -1));
	}

	void test_empty(){
		Scope scope;
		scope["aba"] = new FunctionAST(new vector<string>{"a"}, new vector<Object *>()); 
		FunctionCall(new Reference("aba"), new vector<Object *>{new Number(5)}).evaluate(scope);
	}
};
