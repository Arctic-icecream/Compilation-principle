/*
Exp -> AddExp

    Exp.v

Number -> IntConst | floatConst

PrimaryExp -> '(' Exp ')' | Number
    PrimaryExp.v

UnaryExp -> PrimaryExp | UnaryOp UnaryExp
    UnaryExp.v

UnaryOp -> '+' | '-'

MulExp -> UnaryExp { ('*' | '/') UnaryExp }
    MulExp.v

AddExp -> MulExp { ('+' | '-') MulExp }
    AddExp.v
*/
#include<map>
#include<cassert>
#include<string>
#include<iostream>
#include<vector>
#include<set>
#include<queue>

#define TODO assert(0 && "TODO")
// #define DEBUG_DFA
// #define DEBUG_PARSER

// enumerate for Status
enum State {
    Empty,              // space, \n, \r ...
    IntLiteral,         // int literal, like '1' '01900', '0xAB', '0b11001'
    op                  // operators and '(', ')'
};
std::string toString(State s) {
    switch (s) {
    case State::Empty: return "Empty";
    case State::IntLiteral: return "IntLiteral";
    case State::op: return "op";
    default:
        assert(0 && "invalid State");
    }
    return "";
}

// enumerate for Token type
enum TokenType{
    INTLTR,        // int literal
    PLUS,        // +
    MINU,        // -
    MULT,        // *
    DIV,        // /
    LPARENT,        // (
    RPARENT,        // )
};
std::string toString(TokenType type) {
    switch (type) {
    case TokenType::INTLTR: return "INTLTR";
    case TokenType::PLUS: return "PLUS";
    case TokenType::MINU: return "MINU";
    case TokenType::MULT: return "MULT";
    case TokenType::DIV: return "DIV";
    case TokenType::LPARENT: return "LPARENT";
    case TokenType::RPARENT: return "RPARENT";
    default:
        assert(0 && "invalid token type");
        break;
    }
    return "";
}

// definition of Token
struct Token {
    TokenType type;
    std::string value;
};

// definition of DFA
struct DFA {
    /**
     * @brief constructor, set the init state to State::Empty
     */
    DFA();
    
    /**
     * @brief destructor
     */
    ~DFA();
    
    // the meaning of copy and assignment for a DFA is not clear, so we do not allow them
    DFA(const DFA&) = delete;   // copy constructor
    DFA& operator=(const DFA&) = delete;    // assignment

    /**
     * @brief take a char as input, change state to next state, and output a Token if necessary
     * @param[in] input: the input character
     * @param[out] buf: the output Token buffer
     * @return  return true if a Token is produced, the buf is valid then
     */
    bool next(char input, Token& buf);

    /**
     * @brief reset the DFA state to begin
     */
    void reset();

private:
    State cur_state;    // record current state of the DFA
    std::string cur_str;    // record input characters
};


DFA::DFA(): cur_state(State::Empty), cur_str() {}

DFA::~DFA() {}

// helper function, you are not require to implement these, but they may be helpful
bool isoperator(char c) {
if (c>='0'&& c <= '9')
        return false;
    return true;


}

TokenType get_op_type(std::string s) {
if(s == "+")
        return TokenType::PLUS;
    if(s == "-")
        return TokenType::MINU;
    if(s == "*")
        return TokenType::MULT;
    if(s == "/")
        return TokenType::DIV;
    if(s == "(")
        return TokenType::LPARENT;
    if(s == ")")
        return TokenType::RPARENT;


}

bool DFA::next(char input, Token& buf) {
std::string s;
    s += input;
    int end = 0;
    if((input <'0'|input > '9')& (input != '+') & (input != '-') & (input != '*') & (input != '/') & (input != '(') & (input != ')') & (input != '\n')& !((input == 'x')| (input == 'b')| (input == 'A')|(input == 'B')|(input == 'C')|(input == 'D')|(input == 'E')|(input == 'F'))){
        std::cout<<input<<std::endl;
        return false;
    }
    if(cur_state == State::IntLiteral){
        buf.type = TokenType::INTLTR;
        buf.value = cur_str;
    }
    if(cur_state == State::op){
        buf.type = get_op_type(cur_str);
        buf.value = cur_str;
        end = 1;
    } 
    if(cur_state == State::Empty){
        if ((input >= '0' && input <= '9')| (input == '-')|(input == 'x')| (input == 'b')| (input == 'A')|(input == 'B')|(input == 'C')|(input == 'D')|(input == 'E')|(input == 'F')){
            cur_state = State::IntLiteral;
            cur_str = input;
        }
        else{
            cur_state = State::op;
            cur_str = input;
        }
    }
    else if(cur_state == State::IntLiteral){
        if ((input >= '0' && input <= '9')| (input == 'x')| (input == 'b')| (input == 'A')|(input == 'B')|(input == 'C')|(input == 'D')|(input == 'E')|(input == 'F')){
            cur_str += input;
        }
        else{
            cur_state = State::op;
            cur_str = input;
            end = 1;
        }
    }
    else if(cur_state == State::op){
        if((cur_str == "("|cur_str == "+"|cur_str == "-"|cur_str == "*"|cur_str == "/")&& (input == '-')){
            cur_state = State::IntLiteral;
            cur_str = input;
        }
        if ((input >= '0' && input <= '9')||input == 'x'| input == 'b'| input == 'A'|input == 'B'|input == 'C'|input == 'D'|input == 'E'|input == 'F'){
            cur_state = State::IntLiteral;
            cur_str = input;
        }
        else{
            cur_str = input;
            end = 1;
        }
    }
    if(end){
        return true;
    }
    return false;


}

void DFA::reset() {
    cur_state = State::Empty;
    cur_str = "";
}

// hw2
enum NodeType {
    TERMINAL,       // terminal lexical unit
    EXP,
    NUMBER,
    PRIMARYEXP,
    UNARYEXP,
    UNARYOP,
    MULEXP,
    ADDEXP,
    NONE
};
std::string toString(NodeType nt) {
    switch (nt) {
    case NodeType::TERMINAL: return "Terminal";
    case NodeType::EXP: return "Exp";
    case NodeType::NUMBER: return "Number";
    case NodeType::PRIMARYEXP: return "PrimaryExp";
    case NodeType::UNARYEXP: return "UnaryExp";
    case NodeType::UNARYOP: return "UnaryOp";
    case NodeType::MULEXP: return "MulExp";
    case NodeType::ADDEXP: return "AddExp";
    case NodeType::NONE: return "NONE";
    default:
        assert(0 && "invalid node type");
        break;
    }
    return "";
}

// tree node basic class
struct AstNode{
    int value;
    NodeType type;  // the node type
    AstNode* parent;    // the parent node
    std::vector<AstNode*> children;     // children of node

    /**
     * @brief constructor
     */
    AstNode(NodeType t = NodeType::NONE, AstNode* p = nullptr): type(t), parent(p), value(0) {} 

    /**
     * @brief destructor
     */
    virtual ~AstNode() {
        for(auto child: children) {
            delete child;
        }
    }

    // rejcet copy and assignment
    AstNode(const AstNode&) = delete;
    AstNode& operator=(const AstNode&) = delete;
};

// definition of Parser
// a parser should take a token stream as input, then parsing it, output a AST
struct Parser {
    uint32_t index; // current token index
    const std::vector<Token>& token_stream;

    /**
     * @brief constructor
     * @param tokens: the input token_stream
     */
    Parser(const std::vector<Token>& tokens): index(0), token_stream(tokens) {}

    /**
     * @brief destructor
     */
    ~Parser() {}
    
    /**
     * @brief creat the abstract syntax tree
     * @return the root of abstract syntax tree
     */
    AstNode* get_abstract_syntax_tree() {
               return Exp();

    
    }

    // u can define member funcition of Parser here
    AstNode* Exp();
    AstNode* AddExp();
    AstNode* MulExp();
    AstNode* UnaryExp();
    AstNode* PrimaryExp();
int base= 0;
    int toN(int number, int base) {
    std::string result = "";
    while (number > 0) {
        int remainder = number % base;
        if (remainder < 10) {
            result = std::to_string(remainder) + result;
        } else {
            result = char('A' + remainder - 10) + result;
        }
        number /= base;
    }
    int out = std::stoi(result);
    return out;
}


    
};

// u can define funcition here
AstNode* Parser::Exp() {
    return AddExp();
}
AstNode* Parser::AddExp() {
    AstNode* left = MulExp();
    int sig = 0;
    while (index < token_stream.size() && token_stream[index].type ==TokenType::PLUS || token_stream[index].type == TokenType::MINU) {
        NodeType op_type = NodeType::ADDEXP;
        if(token_stream[index].type == TokenType::PLUS){
            sig = 0;
        }
        if(token_stream[index].type == TokenType::MINU){
            sig = 1;
        }
        index++;
        AstNode* right = MulExp();
        AstNode* root = new AstNode(op_type, nullptr);
        root->value = left->value;
        root->children.push_back(left);
        root->children.push_back(right);
        if( sig == 0){
            root->value += right->value;
        }
        if( sig ==1){
            root->value -= right->value;
        }
        left = root;
    }
    return left;
}

AstNode* Parser::MulExp() {
    AstNode* left = UnaryExp();
    int sig = 0;
    while (index < token_stream.size() & token_stream[index].type == TokenType::MULT | token_stream[index].type == TokenType::DIV) {
        NodeType op_type = NodeType::MULEXP;
        if(token_stream[index].type == TokenType::MULT){
            sig = 0;
        }
        if(token_stream[index].type == TokenType::DIV){
            sig = 1;
        }
        index++;
        AstNode* right = UnaryExp();
        AstNode* root = new AstNode(op_type, nullptr);
        root->value = left->value;
        root->children.push_back(left);
        root->children.push_back(right);
        if( sig == 0){
            root->value *= right->value;
        }
        if( sig == 1){
            root->value /= right->value;
        }
        left = root;
    }
    return left;
}

AstNode* Parser::UnaryExp() {
    if (index < token_stream.size() & (token_stream[index].type == TokenType::PLUS | token_stream[index].type == TokenType::MINU)) {
        NodeType op_type= NodeType::UNARYEXP;
        index++;
        AstNode* exp_node = UnaryExp();
        AstNode* root = new AstNode(op_type, nullptr);
        root->children.push_back(exp_node);
        return root;
    } else {
        return PrimaryExp();
    }
}

AstNode* Parser::PrimaryExp() {
    if (index >= token_stream.size()) {
        return nullptr;
    }

    if (token_stream[index].type == TokenType::INTLTR) {
        std::string value_str = token_stream[index].value;
        int value = 0;
        int base = 10;
        if (value_str.substr(0, 2) == "0b") {
            value = std::strtol(value_str.substr(2).c_str(), nullptr, 2);
            base = 2;
        } 
        else if (value_str.substr(0, 2) == "0x") {
            value = std::strtol(value_str.substr(2).c_str(), nullptr, 16);
            base = 16;
        } else if (value_str.substr(0, 1) == "0") {
            value = std::strtol(value_str.substr(1).c_str(), nullptr, 8);
            base = 8;
        } else {
            value = std::stoi(value_str);
        }
        AstNode* node = new AstNode(NodeType::NUMBER, nullptr);
            node->value = value;
            Parser::base = base;
            index++;
            return node;
        }

    if (token_stream[index].type ==TokenType::LPARENT) {
        index++;
        AstNode* node = Exp();
        if (index >= token_stream.size() || token_stream[index].type != TokenType::RPARENT) {
            throw std::runtime_error("missing right parenthesis");
        }
        index++;
        return node;
    }

    throw std::runtime_error("invalid syntax");
}

int main(){
    std::string stdin_str;
    std::getline(std::cin, stdin_str);
    stdin_str += "\n";
    DFA dfa;
    Token tk;
    std::vector<Token> tokens;
    for (size_t i = 0; i < stdin_str.size(); i++) {
        if(dfa.next(stdin_str[i], tk)){
            tokens.push_back(tk); 
        }
    }

    // hw2
    Parser parser(tokens);
    auto root = parser.get_abstract_syntax_tree();
    // u may add function here to analysis the AST, or do this in parsing
    // like get_value(root);
    

    std::cout << root->value;

    return 0;
}

