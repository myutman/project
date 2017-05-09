#include "model.h"

Scope::Scope(Scope *parent): parent_(parent){}

Scope::~Scope(){
	for (auto p: dictionary_)
		delete p.second;
}

Object* &Scope::operator[](string s){
	if (!dictionary_.count(s)){
		if (parent_ != NULL){
			Object* &ob = (*parent_)[s];
			if (ob != NULL)
				return ob;
		}
	}
	return dictionary_[s];
}

void Scope::add_ref(string s, Object* expr){
	dictionary_[s] = expr;
}

Number::Number(int value):value_(value){}

Function::Function(vector<string> *args, vector<Object*> *body):args_(args), body_(body) {}

/*Function::~Function(){
	delete args_;
	for (auto var: *body_)
		delete var;
	delete body_;
}*/

Object *Function::evaluate(Scope &scope){
	Object *tmp = NULL;
	for (auto var: *body_)
		tmp = var->evaluate(scope);
	return tmp;
}

FunctionDefinition::FunctionDefinition(string name, Function *function):name_(name), function_(function){}

/*FunctionDefinition::~FunctionDefinition(){
	delete function_;
}*/

Object *FunctionDefinition::evaluate(Scope &scope){
	scope[name_] = function_;
	return function_;
}

Conditional::Conditional(Object *condition, vector<Object*> *if_true, vector<Object*> *if_false): condition_(condition), if_true_(if_true), if_false_(if_false){}

/*Conditional::~Conditional(){
	delete condition_;
	for (auto var: *if_true_)
		delete var;
	delete if_true_;
	if (if_false_ != NULL){
		for (auto var: *if_false_)
			delete var;
		delete if_false_;
	}
}*/

Object *Conditional::evaluate(Scope &scope){	
	Number *nm = dynamic_cast<Number*>(condition_->evaluate(scope));
	vector<Object*> *branch = nm->get_val() ? if_true_ : if_false_;
	Object *tmp = NULL;
	for (auto var: *branch)
		tmp = var->evaluate(scope);
	return tmp;
}

Print::Print(Object *expr):expr_(expr){}

Object *Print::evaluate(Scope &scope){
	Object *tmp = expr_->evaluate(scope);
	cout << dynamic_cast<Number*>(tmp)->get_val() << endl;
	return this;
}

Read::Read(string name):name_(name){}

Object *Read::evaluate(Scope &scope){
	int val;
	cin >> val;
	scope[name_] = new Number(val);
	return scope[name_];
}

FunctionCall::FunctionCall(Object *fun_expr, vector<Object*> *args):fun_expr_(fun_expr), args_(args){}

/*FunctionCall::~FunctionCall(){
	delete fun_expr_;
	delete args_;
}*/

Object *FunctionCall::evaluate(Scope &scope){
	Function *function = dynamic_cast<Function*>(fun_expr_->evaluate(scope));
	Scope *call_scope = new Scope(&scope);
	for (size_t i = 0; i < args_->size(); i++){
		string name = function->get_args(i);
		Object *arg = (*args_)[i];
		call_scope->add_ref(name, arg->evaluate(scope));
	}
	Object* tmp = function->evaluate(*call_scope);
	return tmp;
}

Reference::Reference(string name): name_(name){}

Object *Reference::evaluate(Scope &scope){
	return scope[name_];
}

BinaryOperation::BinaryOperation(Object *lhs, string op, Object *rhs):lhs_(lhs), rhs_(rhs), op_(op){}

/*BinaryOperation::~BinaryOperation(){
	delete lhs_;
	delete rhs_;
}*/

Object *BinaryOperation::evaluate(Scope &scope){
	Number *lhs = dynamic_cast<Number*>(lhs_->evaluate(scope)), *rhs = dynamic_cast<Number*>(rhs_->evaluate(scope));
	//cerr << lhs->get_val() << " " << rhs->get_val() << endl;
	if (op_ == "+")
		return new Number(lhs->get_val() + rhs->get_val());
	if (op_ == "*")
		return new Number(lhs->get_val() * rhs->get_val());
	if (op_ == "-")
		return new Number(lhs->get_val() - rhs->get_val());
	if (op_ == "/")
		return new Number(lhs->get_val() / rhs->get_val());
	if (op_ == "%")
		return new Number(lhs->get_val() % rhs->get_val());
	if (op_ == "==")
		return new Number(lhs->get_val() == rhs->get_val());
	if (op_ == "!=")
		return new Number(lhs->get_val() != rhs->get_val());
	if (op_ == "<")
		return new Number(lhs->get_val() < rhs->get_val());
	if (op_ == ">")
		return new Number(lhs->get_val() > rhs->get_val());
	if (op_ == "<=")
		return new Number(lhs->get_val() <= rhs->get_val());
	if (op_ == ">=")
		return new Number(lhs->get_val() >= rhs->get_val());
	if (op_ == "||")
		return new Number(lhs->get_val() || rhs->get_val());
	if (op_ == "&&")
		return new Number(lhs->get_val() && rhs->get_val());
	throw std::runtime_error("invalid kind of symbol");
}

UnaryOperation::UnaryOperation(string op, Object *expr):op_(op), expr_(expr){}

/*UnaryOperation::~UnaryOperation(){
	delete expr_;
}*/

Object *UnaryOperation::evaluate(Scope &scope){
	Number *expr = dynamic_cast<Number*>(expr_->evaluate(scope));
	if (op_ == "!")
		return new Number(!expr->get_val());
	if (op_ == "-")
		return new Number(-expr->get_val());
	throw std::runtime_error("invalid kind of symbol");
}