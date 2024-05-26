#pragma once
#include <queue>
#include <list>
#include <string>
#include <QVariantList>
#include "queryhandler.h"

extern std::list<std::string> ScheduleFilters;
extern std::list<std::string> ExperienceFilters;
extern std::list<std::string> EmploymentFilters;

struct ExtraInfo
{
	ExtraInfo() {}
	ExtraInfo(std::string Role, std::string Schedule, std::string Experience, std::string Employment) :
		Role(Role), Schedule(Schedule), Experience(Experience), Employment(Employment) {}
	std::string Role = "";
	std::string Schedule = "";
	std::string Experience = "";
	std::string Employment = "";
	bool IsEmpty() const {return (Role.empty()); }
};
std::queue<ExtraInfo> GetExtra(QVariantList& RolesList);
