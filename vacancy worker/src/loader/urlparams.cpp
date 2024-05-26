#include "urlparams.h"

std::list<std::string> ScheduleFilters{ "fullDay", "remote", "flexible" };
std::list<std::string> ExperienceFilters{ "between3And6", "between1And3", "noExperience", "moreThan6" };
std::list<std::string> EmploymentFilters {"full", "part", "project", "volunteer", "probation"};

std::queue<ExtraInfo> GetExtra(QVariantList& RolesList)
{
	std::queue<ExtraInfo> UrlsList;
	for (const auto& idRole : RolesList)
	{
		for(const auto& Schedule : ScheduleFilters)
		{
			for(const auto& Experience : ExperienceFilters)
			{
				for(const auto& Employment : EmploymentFilters)
				{
					ExtraInfo ExInfo(idRole.toString().toStdString(), Schedule, Experience, Employment);
					UrlsList.push(ExInfo);
				}
			}
		}
	}
	return UrlsList;
}
