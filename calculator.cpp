#include "stack.cpp"
#include <iostream>
#include <math.h>
#include <string>

using namespace std;
// union for operand and operator
// type = 0 for operator and 1 for operand
union token
{
    double operand;
    char operator_;
    int type;
};

// tree node
struct node
{
    token data;
    node *left;
    node *right;
};

enum preced
{
    PCPAR = 0,
    PADD = 10,
    PMULT = 15,
    PEXP = 20,
    PSIGN = 25,
    POPAR = 30
}; // defining precedance of operators
enum Operators
{
    CPAR = ')',
    ADD = '+',
    SUB = '-',
    MULT = '*',
    DIV = '/',
    EXP = '^',
    OPAR = '('
}; // operators

enum symbolType
{
    OPERATOR,
    OPERAND,
    UNKNOWN
}; // defining symbol types in expression

int symbolType(char symbol)
{
    // given a character from the input string determines whether the character is operator or operand or other.
    if (symbol == CPAR || symbol == ADD || symbol == SUB || symbol == MULT || symbol == DIV || symbol == EXP || symbol == OPAR)
        return OPERATOR;
    else if (symbol >= '0' && symbol <= '9')
        return OPERAND;
    else
        return UNKNOWN;
}

int precValue(char symbol)
{
    // Given an operator symbol it returns the precedence value of that operator.
    if (symbol == CPAR)
        return PCPAR;
    else if (symbol == ADD || symbol == SUB)
        return PADD;
    else if (symbol == MULT || symbol == DIV)
        return PMULT;
    else if (symbol == EXP)
        return PEXP;
    else if (symbol == OPAR)
        return POPAR;
    else
        return -1;
}

int cmpPrec(char oprStk, char oprInp)
{
    // Compares the operator on stack(oprStk) with the operator on the input(oprInp) and return -1 if the precedence oprStk is less than that of oprInp, 0 if they are equal and 1 otherwise.
    if (precValue(oprStk) < precValue(oprInp))
        return -1;
    else if (precValue(oprStk) == precValue(oprInp))
        return 0;
    else
        return 1;
}

int associativity(char opr)
{
    // returns whether the operator(opr) is right associative(1) or left associative(0);
    if (opr == EXP)
        return 1;
    else
        return 0;
}

int toPostfix(string infix, string &postfix)
{
    // returns the postfix representation of the given infix expression through the parameter postfix. The function returns error status through the return.
    Stack<char> oprStk;
    int i = 0;
    while (i < infix.length())
    {
        if (symbolType(infix[i]) == OPERAND)
        {
            postfix += infix[i];
        }
        else if (symbolType(infix[i]) == OPERATOR)
        {
            if (oprStk.isEmpty())
            {
                oprStk.push(infix[i]);
            }
            else
            {
                if (infix[i] == CPAR)
                {
                    while (oprStk.peep() != OPAR)
                    {
                        postfix += oprStk.peep();
                        oprStk.pop();
                    }
                    oprStk.pop();
                }
                else if (infix[i] == OPAR)
                {
                    oprStk.push(infix[i]);
                }
                else
                {
                    while (!oprStk.isEmpty() && oprStk.peep() != OPAR && cmpPrec(oprStk.peep(), infix[i]) != -1 && (cmpPrec(oprStk.peep(), infix[i]) != 0 || associativity(infix[i]) == 0))
                    {
                        postfix += oprStk.peep();
                        oprStk.pop();
                    }
                    oprStk.push(infix[i]);
                }
            }
        }
        else
        {
            return -1;
        }
        i++;
    }
}

// function to convert the postfix string to treenode
node *toTree(string postfix)
{
    cout << "To tree..." << endl;
    // returns the root of the tree representation of the given postfix expression through the parameter root. The function returns error status through the return.
    Stack<node *> oprStk;
    int i = 0;
    while (i < postfix.length())
    {
        if (symbolType(postfix[i]) == OPERAND)
        {
            node *temp = new node;
            temp->data.type = 1;
            temp->data.operand = stod(postfix.substr(i, postfix.find(' ', i) - i));
            temp->left = NULL;
            temp->right = NULL;
            oprStk.push(temp);
        }
        else if (symbolType(postfix[i]) == OPERATOR)
        {
            node *temp = new node;
            temp->data.type = 0;
            temp->data.operator_ = postfix[i];
            temp->left = oprStk.peep();
            oprStk.pop();
            temp->right = oprStk.peep();
            oprStk.pop();
            oprStk.push(temp);
        }
        else
        {
            return NULL;
        }
        i++;
    }
    return oprStk.peep();
}

// function to evaluate the tree
double evaluate(node *root)
{
    // returns the value of the given tree through the parameter value. The function returns error status through the return.
    if (root->data.type == 1)
    {
        return root->data.operand;
    }
    else
    {
        double left = evaluate(root->left);
        double right = evaluate(root->right);
        if (root->data.operator_ == ADD)
        {
            return left + right;
        }
        else if (root->data.operator_ == SUB)
        {
            return left - right;
        }
        else if (root->data.operator_ == MULT)
        {
            return left * right;
        }
        else if (root->data.operator_ == DIV)
        {
            return left / right;
        }
        else if (root->data.operator_ == EXP)
        {
            return pow(left, right);
        }
        else
        {
            return 0;
        }
    }
}

int main()
{
    string infix, postfix;
    cout << "Enter the infix expression: ";
    cin >> infix;
    toPostfix(infix, postfix);
    cout << "Postfix expression: " << postfix << endl;
    node *root = toTree(postfix);
    cout << "Evaluation: " << evaluate(root) << endl;
    return 0;
}
