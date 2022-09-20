// InterpreterA <main.cpp>
// EE 312 Project 8 submission by
// Adrian Gallegos
// ag76424
// Slip days used: 2
// Fall 2021

#include <iostream>
#include "Parse.h"
#include <stack>
#include <string>
#include <vector>
#include <map>

using namespace std;

// reassigns value to existing variable
void set();
// deciphers a prefix expression by reading it from left to right
int blipRec();
// stores expression for later use by recursion function
void storeExpression();
// runs the program in main
void run();
// deals with all operators and operands by completing math
int doSomeMath(string op, int operand1, int operand2);
// initializes variable with a value
void Variable();

map<string, int> variables;   // map for variables and their corresponding values
vector<string> expression;     // vector for reading/holding a prefix expression


void Variable() {
    string newVar = string(next_token());     // newVar is set to the next token
    storeExpression();  // stores expression onto expression vector
    if (variables.find(newVar) != variables.end()) {  // newVar was not initialized correctly
        cout << "variable ";
        cout << newVar.c_str();
        cout << " incorrectly re-initialized";
        cout << endl;
    }
    variables[newVar] = blipRec();  // inserts a new element and returns a reference to its mapped value
}


void set() {    // nearly the same as Variable function
    string newVar = string(next_token());   // newVar is set to the next token
    storeExpression();  // stores expression onto expression vector
    if (variables.find(newVar) == variables.end()) {  // this is the first instance of the variable
        cout << "variable ";
        cout << newVar.c_str();
        cout << " not declared";
        cout << endl;
    }
    variables[newVar] = blipRec();  // returns a reference to the mapped value of newVar
}


void storeExpression() {
    // store the contents of the next peeked token
    string peek = string(peek_next_token());
    // strings to hold commands in order to check for later
    string var = string("var");
    string sets = string("set");
    string text = string("text");
    string output = string("output");
    // Continues to peek next token as well as read the next token as long as the next token
    // is not a blip command nor the end token (indicating the end of the file)
    while (!(next_token_type == END || peek == var || peek == sets || peek == text || peek == output)) {
        read_next_token();  // get next token
        string newElement = string(next_token());  // creates a string to hold the next token
        if (newElement == "//")  // checks for comments
            skip_line();
        expression.push_back(newElement);    // adds the token (operator or operand) to the expression vector
        peek = string(peek_next_token());   // stores the contents of the next peeked token
    }
}


int blipRec() {
    string exprOp = expression.front();   // reads expression from left to right
    // this if statement executes when the first element is not at the end of the vector
    // used by var and text commands
    if (variables.find(exprOp) != variables.end()) {
        expression.erase(expression.begin());   // removes the first element from the expression vector
        return variables[exprOp];   // returns a reference to value in variables map
    }
    // this if statement executes when the current first element is an operator
    if (exprOp == "+" || exprOp == "-" || exprOp == "*" || exprOp == "/" || exprOp == "%" || exprOp == "&&" ||
        exprOp == "||" || exprOp == "<" || exprOp == ">" || exprOp == "<=" || exprOp == ">=" || exprOp == "==" ||
        exprOp == "!=" || exprOp == "!" || exprOp == "~") {
        expression.erase(expression.begin());   // removes the first element from the expression vector
        int operand1 = blipRec();
        int operand2;
        // if the operator is "!" or "~", then the second operand is not needed
        // else we perform recursion to obtain the second operand
        if (exprOp == "!" || exprOp == "~")
            operand2 = 0;
        else
            operand2 = blipRec();
        return doSomeMath(exprOp, operand1, operand2);  // returns result of operator on operand(s)
    }
    // this else executes if the first element is a char string representing a number
    else {
        int result = 0, i = 0;    // Initialize result and counter
        expression.erase(expression.begin());   // removes the first element from the expression vector
        // iterate through the input string and update result
        while (exprOp[i] != '\0') { // continue as long as the null-terminator hasn't been reached
            result = result * 10 + exprOp[i] - '0';   // parses string and converts from character to integer
            i++;    // increment counter to get next char in string
        }
        return result;  // return integer value of string
    }
}


int doSomeMath(string op, int operand1, int operand2) {
    // unary operators
    if (op == "!") {
        if (operand1 == 0)  // 0 -> false but '!' causes the boolean result to become true
            return true;
        if (operand1 != 0)  // not 0 -> true but '!' cause the boolean result to become false
            return false;
    }
    if (op == "~") {
        operand1 = operand1 * -1;   // negates the value
        return operand1;    // return negated value
    }
    // math operators
    if (op == "+")
        return (operand1 + operand2);
    if (op == "-")
        return (operand1 - operand2);
    if (op == "*")
        return (operand1 * operand2);
    if (op == "/")
        return (operand1 / operand2);
    if (op == "%")
        return (operand1 % operand2);
    // logic operators
    if (op == "&&")
        return (operand1 && operand2);
    if (op == "||")
        return (operand1 || operand2);
    // comparison operators
    if (op == "<")
        return (operand1 < operand2);
    if (op == ">")
        return (operand1 > operand2);
    if (op == "<=")
        return (operand1 <= operand2);
    if (op == ">=")
        return (operand1 >= operand2);
    if (op == "==")
        return (operand1 == operand2);
    if (op == "!=")
        return (operand1 != operand2);
}


void run() {
    // read next token
    read_next_token();
    // strings to hold blip commands in order to check for later
    string sets = string("set");
    string var = string("var");
    string output = string("output");
    string text = string("text");
    while (next_token_type != END){ // continue as long as the end of the file hasn't been reached
        string curr_token = string(next_token());
        if (curr_token == "//") // current token is a comment
            skip_line();
        if (curr_token == var) {  // current token is var command
            read_next_token();  // read next token
            Variable(); // execute var command
            expression.clear(); // clears expression vector of current expression
        }
        if (curr_token == sets) {  // current token is set command
            read_next_token();  // read next token
            set();  // execute set command
            expression.clear(); // clears expression vector of current expression
        }
        if (curr_token == text) {  // current token is text command
            while (next_token() == text)    // prevents the word 'text' from being output initially
                read_next_token();
            string text_string = string(next_token());  // store string after 'text' in order to print later
            if (text_string == "//")    // checks if next token is a comment
                skip_line();
            cout << next_token();   // prints out the string following 'text' in the file, excluding comments
        }
        if (curr_token == output) {  // current token is output command
            storeExpression();  // stores expression onto expression vector
            cout << blipRec();  // prints the resulting output of the blip expression following the word 'output'
            expression.clear(); // clears expression vector of current expression
        }
        read_next_token();  // reads next token
    }
    variables.clear();  // clear variables map of all variables. Technically only needed if tests are run back to back in main
}


int main() {
    set_input("test_grader.blip");
    run();
}
