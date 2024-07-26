#ifndef CALCULATOR_ENUMS_HPP
#define CALCULATOR_ENUMS_HPP

#include <string_view>

enum class ASCII
{
	zero = 48,
	one = 49,
	two = 50,
	three = 51,
	four = 52,
	five = 53,
	six = 54,
	seven = 55,
	eight = 56,
	nine = 57,
	decimal = 46,
	plus = 43,
	minus = 45,
	asterisk = 42,
	slash = 47,
	equals = 61,
	percent = 4,
};

enum class ButtonID
{
	zero,
	one,
	two,
	three,
	four,
	five,
	six,
	seven,
	eight,
	nine,
	decimal,
	clear,
	clearEntry,
	plus,
	minus,
	asterisk,
	slash,
	equals,
	percent,
	traceON,
	traceOFF,
};

namespace Symbol
{
    constexpr char none{ ' ' };
    constexpr char zero{ '0' };
    constexpr char one{ '1' };
    constexpr char two{ '2' };
    constexpr char three{ '3' };
    constexpr char four{ '4' };
    constexpr char five{ '5' };
    constexpr char six{ '6' };
    constexpr char seven{ '7' };
    constexpr char eight{ '8' };
    constexpr char nine{ '9' };
    constexpr char decimal{ '.' };
    constexpr char add{ '+' };
    constexpr char subtract{ '-' };
    constexpr char negative{ 'n' };
    constexpr char multiply{ '*' };
    constexpr char divide{ '/' };
    constexpr char percent{ '%' };
	constexpr char overflow{ 'O' };
	constexpr char underflow{ 'U' };
	constexpr char divideByZero{ 'Z' };
	constexpr char invalid{ 'I' };
}

namespace Word
{
	constexpr std::string_view error{ "ERROR" };
	constexpr std::string_view overflow{ "OVERFLOW" };
	constexpr std::string_view underflow{ "UNDERFLOW" };
}

enum class Prescedence
{
    negative = 30,
    multiplyDivide = 20,
    addSubtract = 10,
    notApplicable = 0,
};

#endif