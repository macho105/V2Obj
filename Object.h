#pragma once
#include "Typedefs.h"
#include "Attribute.h"
#include "HasFather.h"
#include "DecisiveSystem.h"
#include "Rule.h"
#include <cassert>

namespace AI
{
	class InformativeSystem;
	class DecisiveSystem;

	class Object 
		: public HasFather<InformativeSystem> 
		, std::enable_shared_from_this<Object>
	{
	private:
		Array<Attribute> _attributes;
		int _index;
		std::shared_ptr<Rule> _rule;
	public:
		Object(Array<std::string> attributes, int index, std::shared_ptr<InformativeSystem> father = nullptr);
		Object(Object& other) = default;
		AtributeType GetAtributeType(int index);

		int GetDecision();

		Attribute operator[](int index);

		virtual Attribute At(int index);

		size_t GetSize() const { return _attributes.size(); }

		//Exercise 2 additions:
		bool HasRule() const { return _rule.get() != nullptr; }
		void SetRule(std::shared_ptr<Rule> rule) { assert(HasRule() == false); _rule = rule; }

	};
}

