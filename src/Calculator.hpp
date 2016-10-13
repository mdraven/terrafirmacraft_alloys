
#ifndef _CALCULATOR_H_48EFE7A5C8C7_
#define _CALCULATOR_H_48EFE7A5C8C7_

#include "external.hpp"

class Calculator final {
    enum class Status { ok, fail, parenthesis };

    enum class Token { open_par, add, sub, mul, div };

    Status m_status;
    double m_res;

    using PostfixElem = boost::variant<double, Token>;

    static std::string::const_iterator
    getEndToken(std::string::const_iterator begin,
                std::string::const_iterator end);
    static std::string::const_iterator
    skipWhitespace(std::string::const_iterator begin,
                   std::string::const_iterator end);
    static bool tryGetToken(char ch, Token& token);
    Status getPostfix(const std::string& expr,
                      std::queue<PostfixElem>& postfix) const;
    Status evalPostfix(std::queue<PostfixElem>& postfix, double& res) const;
public:
    Calculator(const std::string& expr);

    boost::optional<double> get() const;
    QString status() const;

    ~Calculator();
};

#endif /* _CALCULATOR_H_48EFE7A5C8C7_ */
