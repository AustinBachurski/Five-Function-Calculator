#include "tokenizer.hpp"

Tokenizer::Tokenizer(Tracelog& tracelog)
    : m_tracelog{ tracelog }
{ }

std::vector<Token> Tokenizer::tokenize(const std::string_view expression)
{
    std::vector<Token> tokens;

    auto pos = expression.begin();
    size_t numStart = 0;
    size_t numEnd = 0;
    bool wasNumber = false;

    while (pos != expression.end())
    {
        if (isOperator(*pos))
        {
            if (wasNumber)
            {
                numEnd = pos - expression.begin();
                std::string_view numberString{ expression.substr(numStart, numEnd - numStart) };

                long double number{};
                auto [ptr, err] = std::from_chars(numberString.data(), numberString.data() + numberString.size(), number);

                if (err == std::errc())
                {
					m_tracelog.logGenerateNumberToken(numberString);
					tokens.emplace_back(false, Symbol::none, number);
                }
                else
                {
                    m_tracelog.logInvalidNumber(numberString);
					tokens.emplace_back(false, Symbol::invalid, 0.0);
                }

                wasNumber = false;
            }

            m_tracelog.logGenerateOperatorToken(*pos);
            tokens.emplace_back(true, *pos );
            ++pos;
            continue;
        }

        m_tracelog.logFoundNumberComponent(*pos);
        if (!wasNumber)
        {
            wasNumber = true;
            numStart = pos - expression.begin();
        }
        
        ++pos;
    }

    if (wasNumber)
    {
		std::string_view numberString{ expression.substr(numStart) };

		long double number{};
		auto [ptr, err] = std::from_chars(numberString.data(), numberString.data() + numberString.size(), number);

		if (err == std::errc())
		{
			m_tracelog.logGenerateNumberToken(numberString);
			tokens.emplace_back(false, Symbol::none, number);
		}
		else
		{
			m_tracelog.logInvalidNumber(numberString);
			tokens.emplace_back(false, Symbol::invalid, 0.0);
		}
    }

    m_tracelog.logTokenizerGeneratedCount(tokens.size());
    return lex(tokens);
}

bool Tokenizer::isOperator(const char c)
{
    constexpr std::array<char, 5> operators{
        Symbol::add,
        Symbol::subtract,
        Symbol::multiply,
        Symbol::divide,
        Symbol::percent };

    bool result = std::ranges::count(operators, c);
    m_tracelog.logIsCharacterOperator(result);

    return result;
}

// Lexer
std::vector<Token> Tokenizer::lex(const std::vector<Token>& tokens)
{
    std::vector<Token> lexxed;

    auto pos = tokens.begin();

    while (pos != tokens.end())
    {
		// Handle numbers & percent operator.
        if (!pos->isOperator()) 
        {
            if (pos + 1 != tokens.end() && (pos + 1)->getSymbol() == Symbol::percent)
            {
                long double percentage = pos->getValue() / 100;
                m_tracelog.logDetectedPercentSymbol(pos->getValue(), percentage);
                lexxed.emplace_back(false, Symbol::percent, percentage);
                pos += 2;
                continue;
            }

			m_tracelog.logNoAnalysisNeeded(*pos);
            lexxed.push_back(*pos);
            ++pos;
            continue;
        }

		// Handle negative operator.
        if (pos->getSymbol() == Symbol::subtract && pos == tokens.begin() 
			|| pos->getSymbol() == Symbol::subtract && (pos - 1)->isOperator())
		{
            if (pos + 1 != tokens.end() && !(pos + 1)->isOperator())
            {
                m_tracelog.logDetectedNegativeSymbol((pos + 1)->getValue());
                Token negated = performNegation(*(pos + 1));
                m_tracelog.logCheckForOverflow(negated.getSymbol() == Symbol::overflow);

				lexxed.push_back(negated);
				pos += 2;
				continue;
            }
		}

		// Transfer the rest.
        m_tracelog.logNoAnalysisNeeded(*pos);
		lexxed.push_back(*pos); 
        ++pos;
    }

    m_tracelog.logLexerGeneratedCount(lexxed.size());
    return lexxed;
}

Token Tokenizer::performNegation(const Token& left)
{
    m_tracelog.logCheckForOverflow(LDBL_MIN == left.getValue());
    if (LDBL_MIN == left.getValue())
    {
        return Token{ false, Symbol::overflow, LDBL_MAX };
    }

    long double value = -left.getValue();
    return Token{ false, Symbol::none, value };
}
