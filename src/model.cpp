#include "model.h"

//extern std::ofstream out;

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
		if (parent_ != nullptr){
			Object* &ob = (*parent_)[s];
			if (ob != nullptr)
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

Value *Number::gen_IR(Module *mod){
	return ConstantInt::get(TheContext, value_);
}

FunctionAST::FunctionAST(vector<string> *args, vector<Object*> *body):args_(args), body_(body) {}

/*Function::~Function(){
	delete args_;
	for (auto var: *body_)
		delete var;
	delete body_;
}*/

Object *FunctionAST::evaluate(Scope &scope){
	Object *tmp = nullptr;
	for (auto var: *body_)
		tmp = var->evaluate(scope);
	return tmp;
}

Value *FunctionAST::gen_IR(Module* mod){
	Value* tmp = nullptr;
	for (auto var: *body_)
		tmp = var->gen_IR(mod);
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

Value *FunctionDefinition::gen_IR(Module* mod){
	vector<Type *> vt;
	for (int i = 0; i < function_->args_size(); i++){
		vt.push_back(Type::getInt32Ty(TheContext));
	}
	ArrayRef<Type *> argsRef(vt);
	FunctionType *funcType = FunctionType::get(Type::getInt32Ty(TheContext), argsRef, false);
	Function *func = Function::Create(funcType, GlobalValue::ExternalLinkage, name_, mod);
	Function::arg_iterator args = func->arg_begin();
	for (int i = 0; i < function_->args_size(); i++){
		(args++)->setName(function_->get_args(i));
	}
	BasicBlock *block = BasicBlock::Create(TheContext, "entry", func);
	function_->gen_IR(block);
	return func;
}

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
	if (if_false_ != nullptr){
		for (auto var: *if_false_)
			delete var;
		delete if_false_;
	}
}*/

Object *Conditional::evaluate(Scope &scope){	
	Number *nm = dynamic_cast<Number*>(condition_->evaluate(scope));
	vector<Object*> *branch = nm->get_val() ? if_true_ : if_false_;
	Object *tmp = nullptr;
	for (auto var: *branch)
		tmp = var->evaluate(scope);
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
	if (if_false_ != nullptr){
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

Value *Conditional::gen_IR(Module* mod){
	BasicBlock* if_block = BasicBlock::Create(TheContext, "name", mod->getParent());
	if (if_false_ != nullptr){
		for (auto var: *if_true_)
			var->gen_IR(if_block);
	}
	BasicBlock* else_block = BasicBlock::Create(TheContext, "name", mod->getParent());
	if (if_false_ != nullptr){
		for (auto var: *if_false_)
			var->gen_IR(else_block);
	}
	BranchInst *br = BranchInst::Create(if_block, else_block, condition_->gen_IR(mod), mod);
	return br;
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

/*FunctionCall::~FunctionCall(){
	delete fun_expr_;
	delete args_;
}*/

Object *FunctionCall::evaluate(Scope &scope){
	FunctionAST *function = dynamic_cast<Function*>(fun_expr_->evaluate(scope));
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

Value *FunctionCall::gen_IR(Module *mod){
	vector<Value *> vt;
	for (auto val: *args_){
		vt.push_back(val->gen_IR(mod));
	}
	ArrayRef<Value *> argRef = ArrayRef(vt);
	CallInst ci = CallInst::Create(fun_expr->gen_IR(mod), argRef, "", mod);
	return ci;
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

void BinaryOperation::gen_code(int tabs, bool ret){
	print_tabs(tabs, ret);
	out << "(";
	lhs_->gen_code(0, 0);
	out << " " << op_ << " ";
	rhs_->gen_code(0, 0);
	out << ")";
}

Value *BinaryOperation::gen_IR(Module *mod){
	Value *lhs = lhs_->gen_IR(mod);
	Value *rhs = rhs_->gen_IR(mod);
	if (op_ == "+")
		return BinaryOperator::Create(Instruction::Add, lhs, rhs, "", mod);
	if (op_ == "*")
		return BinaryOperator::Create(Instruction::Mul, lhs, rhs, "", mod);
	if (op_ == "-")
		return BinaryOperator::Create(Instruction::Sub, lhs, rhs, "", mod);
	if (op_ == "/")
		return BinaryOperator::Create(Instruction::Div, lhs, rhs, "", mod);
	if (op_ == "%") {
		BinaryOperator *t1 = BinaryOperator::Create(Instruction::Sub, lhs, rhs, "", mod);
		BinaryOperator *t2 = BinaryOperator::Create(Instruction::Mul, t1, rhs, "", mod);
		return BinaryOperator::Create(Instruction::Sub, lhs, t2, "", mod);
	}
	if (op_ == "==")
		return CmpInst::Create(Instruction::OtherOpsBegin, CmpInst::ICMP_EQ, lhs, rhs, "", mod);
	if (op_ == "!=")
		return CmpInst::Create(Instruction::OtherOpsBegin, CmpInst::ICMP_NEQ, lhs, rhs, "", mod);
	if (op_ == "<")
		return CmpInst::Create(Instruction::OtherOpsBegin, CmpInst::ICMP_SLT, lhs, rhs, "", mod);
	if (op_ == ">")
		return CmpInst::Create(Instruction::OtherOpsBegin, CmpInst::ICMP_SGT, lhs, rhs, "", mod);
	if (op_ == "<=")
		return CmpInst::Create(Instruction::OtherOpsBegin, CmpInst::ICMP_SLE, lhs, rhs, "", mod);
	if (op_ == ">=")
		return CmpInst::Create(Instruction::OtherOpsBegin, CmpInst::ICMP_SGE, lhs, rhs, "", mod);
	if (op_ == "||")
		return BinaryOperator::Create(Instruction::Or, lhs, rhs, "", mod);
	if (op_ == "&&")
		return BinaryOperator::Create(Instruction::And, lhs, rhs, "", mod);
	throw std::runtime_error("invalid kind of symbol");
}

UnaryOperation::UnaryOperation(string op, Object *expr):op_(op), expr_(expr){}

/*UnaryOperation::~UnaryOperation(){
	delete expr_;
}*/

Object *UnaryOperation::evaluate(Scope &scope){
	Value *expr = expr_->gen_IR;
	if (op_ == "!")
		return BinaryOperator::Create(Instruction::Xor, ConstantInt::get(TheContext, 1), expr, "", mod);
	if (op_ == "-")
		return BinaryOperator::Create(Instruction::Sub, ConstantInt::get(TheContext, 0), expr, "", mod);
	throw std::runtime_error("invalid kind of symbol");
}

void UnaryOperation::gen_code(int tabs, bool ret){
	print_tabs(tabs, ret);
	out << "(" << op_;
	expr_->gen_code(0, 0);
	out << ")";
}
