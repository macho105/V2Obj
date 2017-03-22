#pragma once
#include "DecisiveSystem.h"

namespace AI
{
	class Rule
	{
		HasFather<DecisiveSystem> _system;
		std::shared_ptr<Object>	  _object = nullptr;
		Array<Attribute>		  _fragments;
		float					  _decision;
	public:
		explicit Rule(Array<Attribute> attributes,
			float decision,
			std::shared_ptr<DecisiveSystem> system = nullptr);

		size_t GetLevel() const { return _fragments.size(); }
		bool Check();

		static Array<Rule> GeneratePotentialRules(std::shared_ptr<Object> object, int level, std::shared_ptr<DecisiveSystem> system = nullptr);

	};
}
