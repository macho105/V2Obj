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

		auto uniques = system.GetUniqueDecisions();
		for(auto i = 0; i < uniques.size(); i++)
		{
			for(auto d = 0; d < system.GetObjectAtIndex(0)->GetSize(); d++)
			{
				auto col = system.GetAttributesAtIndex(d);
				AI::Array<float> toCalc;
				for(auto x = 0; x < col.size(); x++)
				{
					if (col[x].GetFather()->GetDecision() == std::stoi(uniques[i]))
						toCalc.push_back(col[x].GetAsFloat());

				}
				std::cout << "std dev for att " << d << " decision " << uniques[i] << 
					" is: " <<calcStandardDeviation(toCalc) << "\n";


			}
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
	auto finished = [](AI::DecisiveSystem* system)
	{
		for (auto i = 0; i < system->GetObjectsCount(); i++)
			if (!system->GetObjectAtIndex(i)->HasRule())
				return false;
		return true;
	};


	auto Pkt1 = [&]()
	{
		auto sys = system;
		auto level = 1;
		AI::Array<AI::Rule> rules;
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
			level++;
		}
	};
	auto Pkt2 = [&]()
	{
		auto sys = system;
		auto diff = sys.ProduceDiffArray();
		auto contains = [](AI::Array<int> indexes, AI::Array<int> obiekt)->bool
		{
			for (auto i : indexes)
				if (std::find(std::begin(obiekt), std::end(obiekt), i) == std::end(obiekt))
					return false;
			return true;
		};
		auto colContains = [&](AI::Array<int> indexes,AI::Array2D<int> col)->bool
		{
			for (auto box : col)
				if (!contains(indexes, box))
					return false;

			return true;
		};

		auto level = 1;
		AI::Array<AI::Attribute> ret;
		while(!finished(&sys))
		{
			AI::Array<AI::Array<int>> out;
			for(auto i = 0; i < diff.size(); i++)
			{
				out.clear();
				AI::Array<AI::Array<int>> indexes = AI::Rule::comb(sys.GetObjectAtIndex(0)->GetSize(), level);
				AI::Array<AI::Attribute> buff;

				//if(colContains(indexes[i],diff[i]))
				//{
				//	
				//}
				

				/*
				for(auto arr : indexes)
				{
					for (auto d = 0; d < diff[i].size(); d++)
					{
						int good = 0;
						for (auto el : arr)
						{
							for (auto sec : diff[i][d])
								if (sec.GetIndex() == el)
									good++;
							if (good == arr.size())
								continue;
						}
					}
					out.push_back(arr);

				}
				

				if (out.size() == sys.GetObjectAtIndex(0)->GetSize())
					continue;
				else
				{
					// Let's get indexes which didn't occur:
					AI::Array<bool> all; all.assign(sys.GetObjectAtIndex(0)->GetSize(), false);
					for (auto el : buff)
						all[el.GetIndex()] = true;

					
					for(auto x = 0; x < all.size();x++)
						if(all[x] == false)
						{
							ret.push_back(sys.GetObjectAtIndex(i)->At(x));
						}
				}
				*/
			}
			level++;
		}

		return;
	};
	//Pkt1();
	Pkt2();


	auto choinka = []()
	{
		int width = 237, treeWidth = 1;
		auto drawSpaces = [](int i) {while (i) { printf(" "); i--; }};
		for (int i = 0; i < 59; i++)
		{
			int start = abs(treeWidth / 2 + 1);
			start = width / 2 - start;
			drawSpaces(start - 1);
			for (int d = 0; d < treeWidth; d++)
				printf("*");
			//drawSpaces(start + treeWidth + 1);
			printf("\n");
			treeWidth += 2;
		}
	};
	//choinka();
	return;
}

DWORD main(int argc, char* argv[])
{
	Zestaw1();
	//Zestaw2();

	system("pause");
	return S_OK;
}
