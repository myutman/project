#include "model.h"

std::ofstream out;

void print_tabs(int tabs, bool ret = 0){
	for (int i = 0; i < tabs; i++) out << "\t";
	if (ret) out << "return ";
}

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

void Number::gen_code(int tabs, bool ret){
	print_tabs(tabs, ret);
	out << value_;
}

FunctionAST::FunctionAST(vector<string> *args, vector<Object*> *body):args_(args), body_(body) {}

Object *FunctionAST::evaluate(Scope &scope){
	Object *tmp = NULL;
	for (auto var: *body_)
		tmp = var->evaluate(scope);
	return tmp;
}

FunctionDefinition::FunctionDefinition(string name, FunctionAST *function):name_(name), function_(function){}

void FunctionDefinition::gen_code(int tabs, bool ret){
	print_tabs(tabs);
	out << "int " << name_ << "(";
	for (int i = 0; i < function_->args_size(); i++){
		out << "int " << function_->get_args(i);
		if (i != function_->args_size() - 1) out << ", ";
	}
	out << ") {\n";
	for (int i = 0; i < function_->body_size(); i++){
		Object *obj = function_->get_body(i);
		obj->gen_code(tabs + 1, i == function_->body_size() - 1);
		out << ";\n";
	}
	print_tabs(tabs);
	out << "}";
}

Object *FunctionDefinition::evaluate(Scope &scope){
	scope[name_] = function_;
	return function_;
}

Conditional::Conditional(Object *condition, vector<Object*> *if_true, vector<Object*> *if_false): condition_(condition), if_true_(if_true), if_false_(if_false){}

Object *Conditional::evaluate(Scope &scope){	
	Number *nm = dynamic_cast<Number*>(condition_->evaluate(scope));
	vector<Object*> *branch = nm->get_val() ? if_true_ : if_false_;
	Object *tmp = NULL;
	if (branch){
		for (auto var: *branch)
			tmp = var->evaluate(scope);
	}
	return tmp;
}

void Conditional::gen_code(int tabs, bool ret){
	print_tabs(tabs);
	out << "if (";
	condition_->gen_code(0, 0);
	out << ") {\n";
	for (int i = 0; i < if_true_->size(); i++){
		Object *obj = (*if_true_)[i];
		obj->gen_code(tabs + 1, ret && (i == if_true_->size() - 1));
		out << ";\n";
	}
	if (if_false_ != NULL){
		print_tabs(tabs);
		out << "} else {\n";
		for (int i = 0; i < if_false_->size(); i++){
			Object *obj = (*if_false_)[i];
			obj->gen_code(tabs + 1, ret && (i == if_false_->size() - 1));
			out << ";\n";
		}
	}
	print_tabs(tabs);
	out << "}";
}

Print::Print(Object *expr):expr_(expr){}

Object *Print::evaluate(Scope &scope){
	Object *tmp = expr_->evaluate(scope);
	out << dynamic_cast<Number*>(tmp)->get_val() << endl;
	return this;
}

void Print::gen_code(int tabs, bool ret){
	print_tabs(tabs);
	out << "printf(\"%d\\n\", ";
	expr_->gen_code(0, 0);
	out << ")";
}

Read::Read(string name):name_(name){}

Object *Read::evaluate(Scope &scope){
	int val;
	cin >> val;
	scope[name_] = new Number(val);
	return scope[name_];
}

void Read::gen_code(int tabs, bool ret){
	print_tabs(tabs);
	out << "int " << name_ << ";\n";
	print_tabs(tabs);
	out << "scanf(\"%d\", &" << name_ << ")";
}

FunctionCall::FunctionCall(Object *fun_expr, vector<Object*> *args):fun_expr_(fun_expr), args_(args){}

Object *FunctionCall::evaluate(Scope &scope){
	FunctionAST *function = dynamic_cast<FunctionAST*>(fun_expr_->evaluate(scope));
	Scope *call_scope = new Scope(&scope);
	for (size_t i = 0; i < args_->size(); i++){
		string name = function->get_args(i);
		Object *arg = (*args_)[i];
		call_scope->add_ref(name, arg->evaluate(scope));
	}
	Object* tmp = function->evaluate(*call_scope);
	return tmp;
}

void FunctionCall::gen_code(int tabs, bool ret){
	print_tabs(tabs, ret);
	fun_expr_->gen_code(0, 0);
	out << "(";
	for (int i = 0; i < args_->size(); i++){
		(*args_)[i]->gen_code(0, 0);
		if (i != args_->size() - 1) out << ", ";
	}
	out << ")";
}

Reference::Reference(string name): name_(name){}

Object *Reference::evaluate(Scope &scope){
	return scope[name_];
}

void Reference::gen_code(int tabs, bool ret){
	print_tabs(tabs, ret);
	out << name_;
}

BinaryOperation::BinaryOperation(Object *lhs, string op, Object *rhs):lhs_(lhs), rhs_(rhs), op_(op){}

Object *BinaryOperation::evaluate(Scope &scope){
	Number *lhs = dynamic_cast<Number*>(lhs_->evaluate(scope)), *rhs = dynamic_cast<Number*>(rhs_->evaluate(scope));
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

void BinaryOperation::gen_code(int tabs, bool ret){
	print_tabs(tabs, ret);
	out << "(";
	lhs_->gen_code(0, 0);
	out << " " << op_ << " ";
	rhs_->gen_code(0, 0);
	out << ")";
}

UnaryOperation::UnaryOperation(string op, Object *expr):op_(op), expr_(expr){}

Object *UnaryOperation::evaluate(Scope &scope){
	Number *expr = dynamic_cast<Number*>(expr_->evaluate(scope));
	if (op_ == "!")
		return new Number(!expr->get_val());
	if (op_ == "-")
		return new Number(-expr->get_val());
	throw std::runtime_error("invalid kind of symbol");
}

void UnaryOperation::gen_code(int tabs, bool ret){
	print_tabs(tabs, ret);
	out << "(" << op_;
	expr_->gen_code(0, 0);
	out << ")";
}
