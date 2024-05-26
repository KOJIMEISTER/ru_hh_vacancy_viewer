#pragma once
#include <string>
#include <list>

struct Vacancy
{
	std::string VacancyId = "";
	std::string VacancyName = "";
	std::string CompanyName = "";
	std::string PaymentFrom = "0";
	std::string PaymentTo = "0";
	std::string PaymentCur = "";
	bool PaymentGross = false;
	bool Archive = false;
	std::string Area = "";
	std::string WorkExp = "";
	std::string Schedule = "";
	std::string Employment = "";
	std::string Description = "";
	std::string Published = "";
	std::string Role = "";
	std::list<std::string> KeySkills;
};
