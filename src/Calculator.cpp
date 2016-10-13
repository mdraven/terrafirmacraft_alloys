
#include "Calculator.hpp"

Calculator::Calculator(const std::string& expr)
    : m_status(Status::ok)
    , m_res(0.0) {
    std::queue<PostfixElem> postfix;

    m_status = getPostfix(expr, postfix);
    if(m_status != Status::ok)
        return;

    m_status = evalPostfix(postfix, m_res);
}

Calculator::Status Calculator::evalPostfix(std::queue<PostfixElem>& postfix,
                                           double& res) const {
    std::stack<double> stack;

    while(!postfix.empty()) {
        double* d = boost::get<double>(&postfix.front());
        if(d)
            stack.push(*d);
        else {
            if(stack.size() < 2)
                return Status::fail;

            Token tok = boost::get<Token>(postfix.front());

            double b = stack.top();
            stack.pop();
            double a = stack.top();
            stack.pop();

            if(tok == Token::add)
                stack.push(a+b);
            else if(tok == Token::sub)
                stack.push(a-b);
            else if(tok == Token::mul)
                stack.push(a*b);
            else if(tok == Token::div)
                stack.push(a/b);
            else
                return Status::fail;
        }

        postfix.pop();
    }

    if(stack.size() != 1)
        return Status::fail;

    res = stack.top();
    return Status::ok;
}

Calculator::Status
Calculator::getPostfix(const std::string& expr,
                       std::queue<PostfixElem>& postfix) const {
    while(!postfix.empty())
        postfix.pop();

    std::stack<Token> stack;

    auto begin = expr.begin();
    const auto end = expr.end();

    while(true) {
        begin = skipWhitespace(begin, end);
        auto tok_end = getEndToken(begin, end);

        Token cur_tok;

        if(std::isdigit(*begin)) {
            char* e;
            double res = std::strtod(&*begin, &e);
            if(e != &*tok_end)
                return Status::fail;
            postfix.push(res);
        } else if(tryGetToken(*begin, cur_tok)) {
            if(cur_tok != Token::open_par) {
                while(!stack.empty()) {
                    Token stack_tok = stack.top();
                    if(stack_tok == Token::open_par)
                        break;

                    if((cur_tok == Token::mul || cur_tok == Token::div)
                       && (stack_tok == Token::add || stack_tok == Token::sub))
                        break;

                    postfix.push(stack_tok);
                    stack.pop();
                }
            }

            stack.push(cur_tok);
        } else if(*begin == ')') {
            bool has_open_par = false;
            while(!stack.empty()) {
                Token stack_tok = stack.top();

                if(stack_tok == Token::open_par) {
                    has_open_par = true;
                    stack.pop();
                    break;
                }

                postfix.push(stack_tok);
                stack.pop();
            }
            if(!has_open_par)
                return Status::parenthesis;
        } else
            return Status::fail;

        begin = tok_end;
        if(begin == end)
            break;
    }

    while(!stack.empty()) {
        Token stack_tok = stack.top();

        if(stack_tok == Token::open_par)
            return Status::parenthesis;

        postfix.push(stack_tok);
        stack.pop();
    }

    return Status::ok;
}

bool Calculator::tryGetToken(char ch, Token& token) {
    const char ops[] = {'(', '+', '-', '/', '*'};
    const Token tok[] = {Token::open_par, Token::add, Token::sub, Token::div,
                         Token::mul};
    for(std::size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); ++i) {
        if(ch == ops[i]) {
            token = tok[i];
            return true;
        }
    }

    return false;
}

std::string::const_iterator
Calculator::skipWhitespace(std::string::const_iterator begin,
                           std::string::const_iterator end) {
    while(true) {
        if(begin == end)
            break;

        if(*begin != ' ')
            break;

        ++begin;
    }

    return begin;
}

std::string::const_iterator
Calculator::getEndToken(std::string::const_iterator begin,
                        std::string::const_iterator end) {
    if(begin == end)
        return begin;

    if(*begin == ')')
        return begin + 1;

    Token unused;
    if(tryGetToken(*begin, unused))
        return begin + 1;

    if(std::isdigit(*begin)) {
        while(true) {
            ++begin;

            if(begin == end)
                return begin;

            bool is_num = std::isdigit(*begin);
            bool is_delim = (*begin == '.' || *begin == '\'');
            if(!is_num && !is_delim)
                return begin;
        }
    }

    return begin;
}

boost::optional<double> Calculator::get() const {
    if(m_status != Status::ok)
        return boost::none;
    return m_res;
}

QString Calculator::status() const {
    switch(m_status) {
    case Status::ok:
        return "Ok";
    case Status::parenthesis:
        return "Umbalanced parenthesis";
    case Status::fail:
        return "Error in expression";
    }

    return "Unknown error";
}

Calculator::~Calculator() {}
