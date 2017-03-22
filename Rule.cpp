#include "Rule.h"
#include "Object.h"

AI::Rule::Rule(Array<Attribute> attributes, float decision, std::shared_ptr<DecisiveSystem> system)
	: _system(system),
	  _decision(decision),
	  _fragments(attributes)
{
	
}

bool AI::Rule::Check()
{
	auto system = _system.GetFather();
	if (!system)
		throw std::exception("Error, system was not set.");
	
	auto good = 0;
	for(auto i = 0; i < system->GetObjectsCount();i++)
	{
		for(auto& el : _fragments)
		{
			if (system->GetObjectAtIndex(i)->At(el.GetIndex()).GetAsFloat() == el.GetAsFloat() &&
				el.GetFather()->GetDecision() == _decision)
				good++;
			if (good == this->_fragments.size())
				return false;
		}
		good = 0;
	}
	return !good;
}

static std::vector<std::vector<int>> comb(int size, int level)
{
	std::vector<std::vector<int>> ret;
	std::string bitmask(level, 1); // level leading 1's
	bitmask.resize(size, 0); // size-level trailing 0's

							 // print integers and permute bitmask
	do {
		std::vector<int> toAdd;
		for (auto i = 0; i < size; ++i) // [0..size-1] integers
		{
			if (bitmask[i]) toAdd.push_back(i);
		}
		ret.push_back(toAdd);
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));
	return ret;
}
AI::Array<AI::Rule> AI::Rule::GeneratePotentialRules(
	std::shared_ptr<Object> object,
	int level,
	std::shared_ptr<DecisiveSystem> system
)
{
	Array<AI::Rule> ret;
	auto decision = object->GetDecision();
	auto rules = comb(object->GetSize(), level);
	for(auto i = 0; i < rules.size(); i++)
	{
		Array<Attribute> fragments;
		for(auto d = 0; d < rules[i].size(); d++)
		{
			auto val = object->At(rules[i][d]).GetAsString();
			auto index = rules[i][d];

			fragments.push_back(Attribute(val, index));
		}
		ret.push_back(Rule(fragments, decision, system));
	}
	return ret;
}
