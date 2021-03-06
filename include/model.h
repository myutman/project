#ifndef _MODEL_H_
#define _MODEL_H_

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using std::cin;
using std::cout;
using std::map;
using std::string;
using std::vector;
using std::endl;
using std::cerr;

extern std::ofstream out;

class Scope;

class Object{
public:
	virtual void gen_code(int tabs, bool ret) {}
	virtual Object *evaluate(Scope &scope){ return this; }
	virtual ~Object() {}
};

typedef struct {
	std::string str;
	Object* oper;
	vector<Object*> *args;
	vector<string> *ids;
} YYSTYPE_t;


class Scope{
public:
	Scope(Scope *parent=NULL);
	~Scope();
		
	//const Object* operator[](string s) const;
	Object* &operator[](string s);
	void add_ref(string s, Object* expr);

private:
	map<string, Object*> dictionary_;
	Scope *parent_;
};

class Number: public Object{
public:
	Number(int value=0);
	int get_val() const {return value_; }
	void gen_code(int tabs, bool ret);
	
private:
	int value_;
};

class FunctionAST: public Object{
public:
	FunctionAST(vector<string> *args, vector<Object*> *body);
	Object *evaluate(Scope &scope);
	int args_size(){ return args_->size(); }
	string get_args(size_t i){ return (*args_)[i]; } 
	int body_size(){ return body_->size(); }
	Object* get_body(size_t i){ return (*body_)[i]; } 

private:
	vector<string> *args_;
	vector<Object*> *body_;
};

class FunctionDefinition: public Object{
public:
	FunctionDefinition(string name, FunctionAST *function);
	Object *evaluate(Scope &scope);
	void gen_code(int tabs, bool ret);
	
private:
	string name_;
	FunctionAST *function_;
};

class Conditional: public Object{
public:
	Conditional(Object *condition, vector<Object*> *if_true, vector<Object*> *if_false = NULL);
	Object *evaluate(Scope &scope);
	void gen_code(int tabs, bool ret);
	
private:
	Object *condition_;
	vector<Object*> *if_true_, *if_false_;
};

class Print: public Object{
public:
	Print(Object *expr);
	Object *evaluate(Scope &scope);
	void gen_code(int tabs, bool ret);
	
private:
	Object *expr_;
};

class Read: public Object{
public:	
	Read(string name);
	Object *evaluate(Scope &scope);
	void gen_code(int tabs, bool ret);

private:
	string name_;
};

class FunctionCall: public Object{
public:
	FunctionCall(Object *fun_expr, vector<Object*> *args);
	Object *evaluate(Scope &scope);
	void gen_code(int tabs, bool ret);
	
private:
	Object *fun_expr_;
	vector<Object*> *args_;
};

class Reference: public Object{
public:	
	Reference(string name);
	Object *evaluate(Scope &scope);
	void gen_code(int tabs, bool ret);

private:
	string name_;
};

class BinaryOperation: public Object{
public:
	BinaryOperation(Object *lhs, string op, Object *rhs);
	Object *evaluate(Scope &scope);
	void gen_code(int tabs, bool ret);
	
private:
	Object *lhs_, *rhs_;
	string op_;
};

class UnaryOperation: public Object{
public:
	UnaryOperation(string op, Object *expr);
	Object *evaluate(Scope &scope);
	void gen_code(int tabs, bool ret);
	
private:
	string op_;
	Object *expr_;
};


#endif
