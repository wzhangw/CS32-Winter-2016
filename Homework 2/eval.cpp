#include <string>
#include <iostream>
#include <stack>
#include <cassert>

using namespace std;

int evaluate(string infix, const bool values[], string& postfix, bool& result);
bool syntaxIsCorrect(const string& a);
bool precedenceComparison(const char& a, const char& b);
int convertCharToInt(const char& a);
char returnTorFChar(const bool values[], const bool& desired);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	// eliminate all spaces in infix
	string newInfix;
	for (int i = 0; i < infix.size(); i++) 
	{
		if (infix[i] != ' ')
			newInfix += infix[i];
	}

	// return 1 if the infix is essentially empty
	if (newInfix.empty())
		return 1;

	// check the syntax of infix
	if (!syntaxIsCorrect(newInfix))
		return 1;

	// convert infix to postfix
	postfix = "";
	stack<char> opStack;
	char temp;
	for (int i = 0; i < newInfix.size(); i++)
	{
		temp = newInfix[i];
		switch (temp)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			postfix += temp;
			break;
		case '(':
			opStack.push(temp);
			break;
		case ')':
			while (opStack.top() != '(')
			{
				postfix += opStack.top();
				opStack.pop();
			}
			opStack.pop();
			break;
		case '&':
		case '!':
		case '|':
			while (!opStack.empty() && opStack.top() != '(' && precedenceComparison(temp, opStack.top()))
			{
				postfix += opStack.top();
				opStack.pop();
			}
			opStack.push(temp);
			break;
		default:
			break;
		}
	}
	while (!opStack.empty())
	{
		postfix += opStack.top();
		opStack.pop();
	}

	// evaluation of postfix expression
	stack<char> valueStack;
	char operand1, operand2;
	int num1, num2;
	int tempsize = postfix.size();
	for (int i = 0; i < tempsize; i++)
	{
		temp = postfix[i];
		if (isdigit(temp))
			valueStack.push(temp);
		else if (temp == '!') // meet a unary operator '!'
		{
			operand1 = valueStack.top();
			valueStack.pop();
			num1 = convertCharToInt(operand1);
			valueStack.push(returnTorFChar(values, !values[num1]));
		}
		else // ch is a binary operator
		{
			operand2 = valueStack.top();
			valueStack.pop();
			operand1 = valueStack.top();
			valueStack.pop();
			num1 = convertCharToInt(operand1);
			num2 = convertCharToInt(operand2);
			if (temp == '&')
			{
				if (values[num1] && values[num2])
					valueStack.push(returnTorFChar(values, true));
				else
					valueStack.push(returnTorFChar(values, false));
			}
			else // temp is '|'
			{
				if (values[num1] || values[num2])
					valueStack.push(returnTorFChar(values, true));
				else
					valueStack.push(returnTorFChar(values, false));
			}
		}
	}

	// the only value left in stack will be the result
	result = values[convertCharToInt(valueStack.top())];
	valueStack.pop();
	return 0;
}

bool syntaxIsCorrect(const string& a)
{
	int sumParenthesis = 0; // increment when meet '(', decrement when meet ')'
							// eventually should be 0.
	bool haveBeenHandled; // shows whether a character has been checked by any category

	for (int i = 0; i < a.size(); i++)
	{
		haveBeenHandled = false;
		// when reach digits
		if (isdigit(a[i]))
		{
			haveBeenHandled = true;
			if (i != a.size() - 1)
			{
				if (isdigit(a[i + 1]) || a[i + 1] == '(' || a[i + 1] == '!')
					return false;
			}
		}

		// when reach binary operators
		if (a[i] == '&' || a[i] == '|')
		{
			haveBeenHandled = true;
			if (i == 0 || i == a.size() - 1) // can't be at the beginning or the end
				return false;
			// if (!isdigit(a[i - 1]) && a[i - 1] != ')')
				// return false;
			if (!isdigit(a[i + 1]) && a[i + 1] != '!' && a[i + 1] != '(')
				return false;
		}

		// when reach unary operators
		if (a[i] == '!')
		{
			haveBeenHandled = true;
			if (i == a.size() - 1 || (!isdigit(a[i + 1]) && a[i + 1] != '(' && a[i + 1] != '!'))
				return false;
		}
			

		// when reach open parenthesis
		if (a[i] == '(')
		{
			haveBeenHandled = true;
			sumParenthesis++;
			if (i == a.size() - 1 || (!isdigit(a[i + 1]) && a[i + 1] != '!' && a[i + 1] != '('))
				return false;
		}
			

		// when reach close parenthesis
		if (a[i] == ')')
		{
			haveBeenHandled = true;
			sumParenthesis--;
			if (i == a.size() - 1)
				continue;
			if (i == 0 || (a[i + 1] != '&' && a[i + 1] != '|' && a[i + 1] != ')'))
				return false;
		}

		// when reach a character that is not a valid expression
		if (!haveBeenHandled)
			return false;
	}

	// at this point, only need to make sure parentheses are balanced.
	return sumParenthesis == 0;
}

bool precedenceComparison(const char& a, const char& b)
{
	// check whether operator a is smaller or equal than operator b
	return (a == b || a == '|' || b == '!');
}

int convertCharToInt(const char& a)
{
	return (a - '0');
}

char returnTorFChar(const bool values[], const bool& desired)
{
	for (char i = '0'; i <= '9'; i++)
		if (values[convertCharToInt(i)] == desired)
			return i;
}

int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf = "default";
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("1!", ba, pf, answer) == 1);
	assert(evaluate("1!2", ba, pf, answer) == 1);
	assert(evaluate(" & 1", ba, pf, answer) == 1);
	assert(evaluate(" | 1", ba, pf, answer) == 1);
	assert(evaluate("1&|2", ba, pf, answer) == 1);
	assert(evaluate("1&!|2", ba, pf, answer) == 1);
	assert(evaluate("   ", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
	assert(evaluate("1&2&3&4&5&6", ba, pf, answer) == 0);
	assert(evaluate("4  | | 3 & (0&3) ", ba, pf, answer) == 1);
	assert(evaluate("  1|2       ", ba, pf, answer) == 0);
	assert(evaluate("  1|2               ", ba, pf, answer) == 0);
	assert(evaluate("  1&2               ", ba, pf, answer) == 0);
	assert(evaluate("      1&(9|1&1|9) | (9&1&9)           ", ba, pf, answer) == 0);

	// tests below run well on Visual C++, but cause stack-buffer-overflow in g32
	assert(evaluate("!1", ba, pf, answer) == 0 && !answer);
	assert(evaluate("4|!3& (0&3) ", ba, pf, answer) == 0 && pf == "43!03&&|" && !answer);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0 && pf == "43!03&&|" && !answer);	
	assert(evaluate("   !1       ", ba, pf, answer) == 0);

	// So far, the conditions of infix that will lead to stack-buffer-overflow are
	// 1. infix string must contain ! operator
	// 2. infix string must be syntactically correct
	// if any one of the 2 conditions is not satisfied, there will be no stack-buffer-overflow
	cout << "Passed all tests" << endl;
}
