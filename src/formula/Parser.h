/**
 *
 * Copyright (c) 2001, Frank Buﬂ
 *
 * project: Formula
 * version: $Revision: 1.3 $ $Name:  $
 *
 * Parser class.
 */

#ifndef PARSER_H
#define PARSER_H

#include "Evaluator.h"
#include <string>
#include <vector>
#include <stack>

using namespace std;

class Token;

class Parser
{
	friend class OperatorToken;
	friend class AddToken;
	friend class SubToken;
	friend class NumberToken;
	friend class OpenBracketToken;
	friend class CloseBracketToken;
	friend class IdentifierToken;
	friend class CommaToken;

public:
	Parser(std::string expression);
	~Parser();
	void setExpression(std::string expression);
	void setVariable(std::string name, float value) {
		m_evaluator.setVariable(name, value);
	}
	float* getVariableAddress(std::string name) {
		return m_evaluator.getVariableAddress(name);
	}
	void setFunction(std::string name, NoArgumentFunction function);
	void setFunction(std::string name, OneArgumentFunction function);
	void setFunction(std::string name, TwoArgumentsFunction function);
	NoArgumentFunction getNoArgumentFunction(std::string name);
	OneArgumentFunction getOneArgumentFunction(std::string name);
	TwoArgumentsFunction getTwoArgumentsFunction(std::string name);
	
	std::string getPostfix() {
		return m_postfix;
	}
	float eval() {
		return m_evaluator.eval();
	}


private:
	void deleteTokens();
	std::string parseNumber(char c);
	std::string parseIdentifier(char c);
	char peekChar();
	void skipChar();
	char skipAndPeekChar();
	Token* peekToken();
	Token* peekNextToken();
	Token* peekLastToken();
	void skipToken();

	std::string m_expression;
	int m_currentIndex;
	int m_currentTokenIndex;
	std::string m_postfix;
	Evaluator m_evaluator;
	stack<Token*> m_operators;
	vector<Token*> m_tokens;
	stack<int> m_functionArgumentCountStack;
	map<std::string, NoArgumentFunction> m_noArgumentFunctions;
	map<std::string, OneArgumentFunction> m_oneArgumentFunctions;
	map<std::string, TwoArgumentsFunction> m_twoArgumentsFunctions;
};


#endif
