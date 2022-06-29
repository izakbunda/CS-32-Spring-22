//
//  main.cpp
//
//  Created by Izak Bunda on 4/25/22.
//

#include <iostream>
#include <string>
#include <stack>
#include <cassert>

using namespace std;

bool infixCheck( const string& infix );
void toPostfix( const string& infix, string& postfix );

int evaluate(string infix, string& postfix, bool& result) {
    
    if ( infixCheck( infix ) ) {
        
        toPostfix( infix, postfix );
        
        stack<char> operand; // Initialize the operand stack to empty
        char operator1;
        char operator2;
        
        for ( int i = 0; i < postfix.length(); i++ ) { // For each character ch in the postfix string
            
            if ( postfix[i] == 'T' || postfix[i] == 'F' ) { // if ch is an operand
                operand.push( postfix[i] ); // push the value that ch represents onto the operand stack
            }
            else if ( postfix[i] == '!' ) {
                
                operator1 = operand.top();
                operand.pop();
                
                if ( operator1 == 'T' ) {
                    operand.push('F'); }
                else {
                    operand.push('T'); }
            }
            else {
                
                operator2 = operand.top();
                operand.pop();
                operator1 = operand.top();
                operand.pop();
                
                if ( postfix[i] == '&' ) {
                    if ( operator1 == 'T' && operator2 == 'T' ) {
                        operand.push('T'); }
                    else {
                        operand.push('F'); }
                }
                if ( postfix[i] == '|' ) {
                    if ( operator1 == 'F' && operator2 == 'F' ) {
                        operand.push('F'); }
                    else {
                        operand.push('T'); }
                }
            }
        }
        
        if (operand.empty() ) {
            return 1;
        }
        
        
        if ( operand.top() == 'T' ) {
            result = true; }
        else {
            result = false; }
        return 0;
    }
    
    else {
        return 1; }
}

bool infixCheck( const string& infix )
{
    
    string noSpace;
    
    // remove all spaces
    for (int i = 0; i < infix.length(); i++) {
        if (infix[i] != ' ' ) {
            noSpace += infix[i];
        }
    }
    
    // if length is zero
    if ( noSpace.length() == 0 ) {
        return false;
    }
    
    // if length is one
    if ( noSpace.length() == 1) {
        if (noSpace[0] != 'F' && noSpace[0] != 'T') {
            return false;
        }
        return true;
    }
    
    // check if for any invalid characters
    for (int i = 0; i < noSpace.length(); i++ ) {
        if ( (noSpace[i] != 'F' ) && (noSpace[i] != 'T') && (noSpace[i] != '!') && (noSpace[i] != '|') && (noSpace[i] != '(') && (noSpace[i] != ')') && (noSpace[i] != '&') ) {
            return false;
        }
    }
    
    // check if each parentheses has a partner <3
    int preCounter = 0;
    int postCounter = 0;
    
    for (int i = 0; i < noSpace.length(); i++ ) {
        if (noSpace[i] == '(' ) {
            preCounter++; }
        if (noSpace[i] == ')' ) {
            postCounter++; }
    }
    
    if ( preCounter != postCounter ) {
        return false; }
    
    // check that the next char after a '(' isn't a ')'
    for (int i = 0; i < noSpace.length(); i++ ) {
        if ( i+1 < noSpace.length() ) { // takes care of indexing out of range
            if (noSpace[i] == '(' ) {
                if (noSpace[i+1] == ')' ) {
                    return false; } }
        }
    }
    
    // check that the next char after either 'T' or 'F' is not 'T' or 'F'
    for (int i = 0; i < noSpace.length(); i++ ) {
        if ( i+1 < noSpace.length() ) { // takes care of indexing out of range
            if (noSpace[i] == 'T' ) {
                if (noSpace[i+1] == 'T' || noSpace[i+1] == 'F') {
                    return false; } }
            if (noSpace[i] == 'F' ) {
                if (noSpace[i+1] == 'T' || noSpace[i+1] == 'F') {
                    return false; } }
        }
    }
    // check front characters
    if ( noSpace[0] == ')' || noSpace[0] == '&' || noSpace[0] == '|' ) {
        return false;
    }
    
    // check last characters
    for (int i = 0; i < noSpace.length(); i++ ) {
        if ( noSpace[noSpace.length()-1] == '!' || noSpace[noSpace.length()-1] == '&' || noSpace[noSpace.length()-1] == '(' || noSpace[noSpace.length()-1] == '|' ) {
            return false;
        }
    }
    
    // final individual checks
    
    // for T
    for (int i = 0; i < noSpace.length(); i++ ) {
        if ( i+1 < noSpace.length() ) { // takes care of indexing out of range
            if (noSpace[i] == 'T' ) {
                if (noSpace[i+1] == 'F' || noSpace[i+1] == 'T' || noSpace[i+1] == '(' || noSpace[i+1] == '!' ) {
                    return false; } }
        }
    }
    // for F
    for (int i = 0; i < noSpace.length(); i++ ) {
        if ( i+1 < noSpace.length() ) { // takes care of indexing out of range
            if (noSpace[i] == 'F' ) {
                if (noSpace[i+1] == 'F' || noSpace[i+1] == 'T' || noSpace[i+1] == '(' || noSpace[i+1] == '!' ) {
                    return false; } }
        }
    }
    // for (
    for (int i = 0; i < noSpace.length(); i++ ) {
        if ( i+1 < noSpace.length() ) { // takes care of indexing out of range
            if (noSpace[i] == '(' ) {
                if (noSpace[i+1] == ')' || noSpace[i+1] == '|' || noSpace[i+1] == '&' ) {
                    return false; } }
        }
    }
    // for )
    for (int i = 0; i < noSpace.length(); i++ ) {
        if ( i+1 < noSpace.length() ) { // takes care of indexing out of range
            if (noSpace[i] == ')' ) {
                if (noSpace[i+1] == '!' || noSpace[i+1] == 'T' || noSpace[i+1] == 'F' ) {
                    return false; } }
        }
    }
    // for !
    for (int i = 0; i < noSpace.length(); i++ ) {
        if ( i+1 < noSpace.length() ) { // takes care of indexing out of range
            if (noSpace[i] == '!' ) {
                if (noSpace[i+1] == '|' || noSpace[i+1] == '&' || noSpace[i+1] == ')' ) {
                    return false; } }
        }
    }
    // for &
    for (int i = 0; i < noSpace.length(); i++ ) {
        if ( i+1 < noSpace.length() ) { // takes care of indexing out of range
            if (noSpace[i] == '&' ) {
                if (noSpace[i+1] == '|' || noSpace[i+1] == '&' || noSpace[i+1] == ')' ) {
                    return false; } }
        }
    }
    // for |
    for (int i = 0; i < noSpace.length(); i++ ) {
        if ( i+1 < noSpace.length() ) { // takes care of indexing out of range
            if (noSpace[i] == '|' ) {
                if (noSpace[i+1] == '|' || noSpace[i+1] == '&' || noSpace[i+1] == ')' ) {
                    return false; } }
        }
    }
    
    
    return true;
}



void toPostfix( const string& infix, string& postfix ) {
    
    postfix = ""; // Initialize postfix to empty
    stack<char> operators; // Initialize the operator stack to empty

    for (int i = 0; i < infix.length(); i++ ) { // For each character ch in the infix string
        switch ( infix[i] ) { // Switch (ch)
            case 'T': // case operand:
                postfix += infix[i]; // append ch to end of postfix
                break; // break
            case 'F': // case operand:
                postfix += infix[i]; // append ch to end of postfix
                break; // break
                
            case '(': // case '(':
                operators.push(infix[i]); // append ch to end of postfix
                break; // break
            case ')': // case ')':  // pop stack until matching '('
                while( operators.top() != '(' ) { //  While stack top is not '('
                    postfix += operators.top(); // append the stack top to postfix
                    operators.pop(); // pop the stack
                }
                operators.pop(); // pop the stack  // remove the '('
                break; // break
        
            case '!': // case operator:
                while ( !operators.empty() && operators.top() != '(' && operators.top() != '&' && operators.top() != '|' ) { // While the stack is not empty and the stack top is not '(' and precedence of ch <= precedence of stack top
                    postfix += operators.top(); // append the stack top to postfix
                    operators.pop(); // pop the stack
                }
                operators.push( infix[i] ); // push ch onto the stack
                break; // break
            case '&': // case operator:
                while ( !operators.empty() && operators.top() != '(' && operators.top() != '|' ) { // While the stack is not empty and the stack top is not '(' and precedence of ch <= precedence of stack top
                    postfix += operators.top(); // append the stack top to postfix
                    operators.pop(); // pop the stack
                }
                operators.push( infix[i] ); // push ch onto the stack
                break; // break
            case '|': // case operator:
                while ( !operators.empty() && operators.top() != '(' ) { // While the stack is not empty and the stack top is not '(' and precedence of ch <= precedence of stack top
                    postfix += operators.top(); // append the stack top to postfix
                    operators.pop(); // pop the stack
                }
                operators.push( infix[i] ); // push ch onto the stack
                break; // break
            default:
                break;
    }
    }

    while ( !operators.empty() ) { // While the stack is not empty
        postfix += operators.top(); //append the stack top to postfix
        operators.pop(); // pop the stack
    }
}



int main() {

    string pf;
            bool answer;
            assert(evaluate("T| F", pf, answer) == 0  &&  pf == "TF|"  &&  answer);
            assert(evaluate("T|", pf, answer) == 1);
            assert(evaluate("F F", pf, answer) == 1);
            assert(evaluate("TF", pf, answer) == 1);
            assert(evaluate("()", pf, answer) == 1);
            assert(evaluate("()T", pf, answer) == 1);
            assert(evaluate("T(F|T)", pf, answer) == 1);
            assert(evaluate("T(&T)", pf, answer) == 1);
            assert(evaluate("(T&(F|F)", pf, answer) == 1);
            assert(evaluate("T+F", pf, answer) == 1);
            assert(evaluate("", pf, answer) == 1);
            assert(evaluate("F  |  !F & (T&F) ", pf, answer) == 0 &&  pf == "FF!TF&&|"  &&  !answer);
            assert(evaluate(" F  ", pf, answer) == 0 &&  pf == "F"  &&  !answer);
            assert(evaluate("((T))", pf, answer) == 0 &&  pf == "T"  &&  answer);
            cout << "Passed all tests" << endl;
}

