#include <regex>
#include <cctype>

#include <locale>
#include <string>

#include <iostream>
#include <algorithm>

// удаляет ведущие нули из строки, содержащей число
std::string trimLeadingZeros(const std::string& numStr) 
{
	size_t first_non_zero = numStr.find_first_not_of('0');
	
	// все нули или пустая строка
	if (first_non_zero == std::string::npos) 
	{
		return "0";
	}
	return numStr.substr(first_non_zero);
}

// получаем номер рейса из строки
std::string extractNumberString(const std::string& s) {
	
	size_t start_pos = 0;
	while (start_pos < s.size() && !std::isdigit(static_cast<unsigned char>(s[start_pos]))) 
	{
		++start_pos;
	}
	
	// если цифр нет, возвращаем пустую строку
	if (start_pos == s.size()) {
		return "";
	}
	
	return s.substr(start_pos);
}

// проверяет наличие хотя бы одной буквы
bool containsLetter(const std::string& s) 
{
	return std::any_of(s.begin(), s.end(), [](unsigned char c) 
					   {
		return std::isalpha(c);
	});
}

// Валидация Кода авиакомпании
bool isValidFlightCode(const std::string& code) 
{
	std::regex pattern1("[A-Z]{2}");
	std::regex pattern2("[A-Z]{3}");
	
	std::regex pattern3("[A-Z]");
	std::regex pattern4("[A-Z][A-Z] ");
	
	std::regex pattern5("[A-Z0-9]{2}");
	std::regex pattern6("[A-Z0-9][A-Z0-9] ");
	
	// пустая строка
	if (code.empty())
		return true;
	
	if (std::regex_search(code, pattern1))
		return true;
	if (std::regex_search(code, pattern2))
		return true;
	if (std::regex_search(code, pattern3))
		return true;
	if (std::regex_search(code, pattern4))
		return true;
	if (std::regex_search(code, pattern5) && containsLetter(code))
		return true;
	if (std::regex_search(code, pattern6) && containsLetter(code))
		return true;
	
	return false;
}

// Проверка наличия номера рейса (последовательности из 1-5 цифр)
bool hasValidFlightNumber(const std::string& s) 
{
	std::regex numberPattern("\\d{1,5}");
	std::regex longNumberPattern("\\d{6,}");
	
	// Проверяем наличие допустимой последовательности
	bool hasValidSeq = std::regex_search(s, numberPattern);
	
	// Проверяем наличие слишком длинной последовательности
	bool hasTooLongSeq = std::regex_search(s, longNumberPattern);
	
	// Возвращаем true только если есть допустимая и нет слишком длинной
	return hasValidSeq && !hasTooLongSeq;
}

// Максимальный размер строки — 7 символов
bool isLengthValid(const std::string& s) {
	return s.size() <= 7;
}

std::string extractLeadingLetters(const std::string& s) 
{
	std::string result;
	for (char ch : s) 
	{
		if (std::isalpha(static_cast<unsigned char>(ch))) 
		{
			result += ch;
		} 
		else 
		{
			break; // останавливаемся при первом не-буквенном символе
		}
	}
	
	return result;
}

// проверяем есть ли пробел в строке
std::string getPartBeforeSpace(const std::string& s) 
{
	size_t pos = s.find(' ');
	if (pos == std::string::npos) 
	{
		// Если пробела нет, возвращаем всю строку
		return s;
	} 
	else 
	{
		// Возвращаем часть строки до пробела
		return s.substr(0, pos);
	}
}

// проверяем есть ли в строке пробел
bool containsSpace(const std::string& s) 
{
	
	if (s.find(' ') != std::string::npos) 
	{
		return true;
	} 
	else 
	{
		return false;
	}
}

// функция сравнения двух строк str1 и str2
bool compareFlightCodes(const std::string& str1, const std::string& str2) 
{
	// Валидация размера строки (7 символов)
	if (!isLengthValid(str1) || !isLengthValid(str2))
		return false;
	
	std::string code1;
	std::string code2;
	
	// Получаем код авиакомпании из строки str1
	if (containsSpace(str1))
	{
		code1 = getPartBeforeSpace(str1);
	}
	else
	{
		code1 = extractLeadingLetters(str1);
	}
	
	// Получаем код авиакомпании из строки str2
	if (containsSpace(str2))
	{
		code2 = getPartBeforeSpace(str2);
	}
	else
	{
		code2 = extractLeadingLetters(str2);
	}
	
	// Валидация Кода авиакомпании
	if (!isValidFlightCode(code1) || !isValidFlightCode(code2))
		return false;
	
	// Валидация номера рейса (последовательности из 1-5 цифр)
	if (!hasValidFlightNumber(str1) || !hasValidFlightNumber(str2))
		return false;
	
	// удаляем все пробелы в строке
	auto remove_spaces = [](const std::string& s) 
	{
		std::string res;
		res.reserve(s.size());
		
		for (char c : s) 
		{
			if (c != ' ')
				res += c;
		}
		return res;
	};
	
	// удаляем все пробелы в строках
	auto numStr1 = remove_spaces(str1);
	auto numStr2 = remove_spaces(str2);
	
	// получаем номер рейса
	numStr1 = extractNumberString(numStr1);
	numStr2 = extractNumberString(numStr2);
	
	// удаляем ведущие нули из номера рейса
	numStr1 = trimLeadingZeros(numStr1);
	numStr2 = trimLeadingZeros(numStr2);
	
	// очищаем коды рейса от чисел
	code1 = extractLeadingLetters(code1);
	code1 = extractLeadingLetters(code2);
	
	return code1.append(numStr1) == code2.append(numStr2);
}

// вывод результата сравнения двух строк
void printResultCompare(const std::string& codeA, const std::string& codeB) 
{
	if (compareFlightCodes(codeA, codeB)) 
	{
		std::cout << "Коды: " << codeA << " и " << codeB << " - равны"<< std::endl;
	} 
	else 
	{
		std::cout << "Коды: " << codeA << " и " << codeB << " - не равны"<< std::endl;
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	
	std::string codeA;
	std::string codeB;
	
	codeA = "AB 123";     // допустимый формат с номером рейса
	codeB = "ab 123";     // тот же код в нижнем регистре
	
	// вывод результата сравнения
	printResultCompare(codeA, codeB);
	
	codeA = "ABC 225";      
	codeB = "ABC 123";
	
	// вывод результата сравнения
	printResultCompare(codeA, codeB);
	
	codeA = "AFL1";  
	codeB = "AFL0001";
	
	// вывод результата сравнения
	printResultCompare(codeA, codeB);
	
	codeA = "ABC";        // без номера рейса -> невалидно
	codeB = "ABC0007";
	
	// вывод результата сравнения
	printResultCompare(codeA, codeB);
	
	codeA = "007";      // без номера рейса
	codeB = "00007";   // без номера рейса
	
	// вывод результата сравнения
	printResultCompare(codeA, codeB);
	
	codeA = "D2 25";      
	codeB = "D225";
	
	// вывод результата сравнения
	printResultCompare(codeA, codeB);
	
	return 0;
}
