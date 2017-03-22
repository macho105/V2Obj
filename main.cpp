#include <Windows.h>
#include <string>
#include <iostream>
#include "DecisiveSystemReader.h"
#include "Object.h"
#include <numeric>

#define AUSTRALIAN_SYSTEM_FILENAME	"C:\\australian.txt"
#define AUSTRALIAN_TYPES_FILENAME	"C:\\australian-type.txt"

#define DECISIVE_SYSTEM_FILENAME "C:\\SystemDecyzyjny.txt"

void DisplayExercise(std::string zestaw, std::string pkt = "")
{
	if (zestaw.size())
		std::cout << "========== " << zestaw << (pkt.size() ? (" pkt: " + pkt) : "") << "===========" << std::endl;
	else
		std::cout << "=============== Koniec ===============" << std::endl << std::endl << std::endl;
}

void Zestaw1()
{
	AI::DecisiveSystemReader reader(AUSTRALIAN_SYSTEM_FILENAME, AUSTRALIAN_TYPES_FILENAME);
	auto system = reader.ReadDecisiveSystem();

	auto Pkt1_A = [&]()
	{
		DisplayExercise("Zestaw 1", "A 1 ");
		auto uniques = system.GetUniqueDecisions();
		for (auto& uniq : uniques)
			std::cout << uniq << std::endl;
		DisplayExercise("");
	};
	auto Pkt1_B = [&]()
	{
		DisplayExercise("Zestaw 1", "A 2 ");

		auto uniques = system.GetUniqueDecisions();
		auto decisions = system.GetDecisions();
		for (auto& uniq : uniques)
			std::cout << uniq << " : " 
				<< std::count(decisions.begin(), decisions.end(), uniq) << std::endl;

		DisplayExercise("");
	};

	auto Pkt1_C = [&]()
	{
		DisplayExercise("Zestaw 1", "A 3 ");
		std::vector<std::pair<AI::Attribute, AI::Attribute>> ret;

		for(auto i = 0; i < system.GetObjectAtIndex(0)->GetSize(); i++)
		{
			if(system.GetAtributeType(i) == AI::kSymbolic)
			{
				ret.push_back({{"",-1},{"",-1}});
				continue;
			}
			auto attributes = system.GetAttributesAtIndex(i);
			auto toAdd = std::minmax_element(attributes.begin(), attributes.end(),
				[](AI::Attribute first, AI::Attribute seccond) {return first.GetAsFloat() < seccond.GetAsFloat(); });
		
			ret.push_back({ attributes[toAdd.first-attributes.begin()],attributes[toAdd.second - attributes.begin()] });
		}
		for (auto i = 0; i < ret.size(); i++)
			if (ret[i].first.GetAsString().empty())
				std::cout << "Index : " << i << " was symbolic." << std::endl;
			else
				std::cout << "Index : " << i << " min : " << ret[i].first.ToString() << " max : " << ret[i].second.ToString() << std::endl;
		DisplayExercise("");
	};
	auto Pkt2_A = [&]()
	{
		DisplayExercise("Zestaw 1", "B 1 ");
		AI::Array2D<AI::Attribute> ret;
		for (auto i = 0; i < system.GetObjectAtIndex(0)->GetSize(); i++)
			ret.push_back(system.GetUniqueAttributes(i));

		for (auto i = 0; i < ret.size(); i++)
		{
			std::cout << "Attribute " << i << " : " << "size : " << ret[i].size() << std::endl;
		}
		DisplayExercise("");
	};
	auto Pkt2_B = [&]()
	{
		DisplayExercise("Zestaw 1", "B 2 ");
		AI::Array2D<AI::Attribute> ret;
		for (auto i = 0; i < system.GetObjectAtIndex(0)->GetSize(); i++)
			ret.push_back(system.GetUniqueAttributes(i));

		for(auto i = 0; i < ret.size(); i++)
		{
			std::cout << "Attribute " << i << " : ";
			for (auto d = 0; d < ret[i].size(); d++)
			{
				std::cout << ret[i][d].GetAsString();
				std::cout << (d != ret[i].size() - 1 ? ", " : "\n");
			}
			std::cout << std::endl;
		}
		DisplayExercise("");

	};
	auto Pkt2_C = [&]()
	{
		DisplayExercise("Zestaw 1", "B 3 ");
		auto calcStandardDeviation = [&](std::vector<float> col)->float {
			auto sum = std::accumulate(col.begin(), col.end(), 0.0);
			double mean = sum / col.size();

			double sq_sum = std::inner_product(col.begin(), col.end(), col.begin(), 0.0);
			return std::sqrt(sq_sum / col.size() - mean * mean);
		};
		AI::Array<float> ret;
		for(auto i = 0; i < system.GetObjectAtIndex(0)->GetSize(); i++)
		{
			const auto& attributes = system.GetAttributesValues(i);
			ret.push_back(calcStandardDeviation(attributes));
		}
		auto decisions = system.GetAttributesValues(-1);
		ret.push_back(calcStandardDeviation(decisions));
		for(auto i = 0; i < ret.size(); i++)
		{
			if (i != ret.size() - 1)
				std::cout << "[" << i << "] : ";
			else
				std::cout << "Decision : ";

			std::cout << ret[i] << std::endl;
		}

		DisplayExercise("");
	};


	Pkt1_A();
	Pkt1_B();
	Pkt1_C();

	Pkt2_A();
	Pkt2_B();
	Pkt2_C();
}
void Zestaw2()
{
	AI::DecisiveSystemReader reader(DECISIVE_SYSTEM_FILENAME);
	auto system = reader.ReadDecisiveSystem();
	auto p_sys = std::shared_ptr<AI::DecisiveSystem>(&system);
	AI::Array<AI::Rule> rules;
	auto test = p_sys->GetObjectAtIndex(0)->GetFather();
	auto father = std::dynamic_pointer_cast<AI::DecisiveSystem>(p_sys->GetObjectAtIndex(0)->GetFather());
	auto Pkt1 = [&]()
	{
		auto sys = system;
		auto finished = [](AI::DecisiveSystem* system)
		{
			for (auto i = 0; i < system->GetObjectsCount(); i++)
				if (!system->GetObjectAtIndex(i)->HasRule())
					return false;
			return true;
		};
		auto level = 1;
		while(!finished(&sys))
		{
			for(auto i = 0; i < sys.GetObjectsCount(); i++)
			{
				auto obj = sys.GetObjectAtIndex(i);
				auto possibilities = AI::Rule::GeneratePotentialRules(obj, level,std::make_shared<AI::DecisiveSystem>(sys));

				for(auto& pos : possibilities)
				{
					if(pos.Check() && !obj->HasRule())
					{
						obj->SetRule(std::make_shared<AI::Rule>(pos));
						rules.push_back(pos);
					}
				}
			}
		}
	};

	Pkt1();
	return;
}

DWORD main(int argc, char* argv[])
{
	Zestaw1();
	Zestaw2();

	system("pause");
	return S_OK;
}
