/**
 *
 * Copyright (c) 2001, Frank Buﬂ
 *
 * project: Formula
 * version: $Revision: 1.3 $ $Name:  $
 *
 * The Formula class is the facade class for accessing the fomula system.
 */

#ifndef FORMULA_H
#define FORMULA_H

#include "Exception.h"

using namespace std;

class Parser;

class Formula
{
public:
	Formula();
	Formula(std::string formula);
	~Formula();
	void setExpression(std::string expression);
	void setVariable(std::string name, float value);
	float* getVariableAddress(std::string name);
	void setFunction(std::string name, float(*function)());
	void setFunction(std::string name, float(*function)(float));
	void setFunction(std::string name, float(*function)(float, float));
	float eval();

private:
	Parser* m_parser;
};


#endif
