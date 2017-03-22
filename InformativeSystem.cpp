#include "InformativeSystem.h"
#include "Object.h"
#include <cassert>

AI::InformativeSystem::InformativeSystem(Array2D<std::string> matrix, Array<std::string> types): _attributeTypes(types)
{
	auto this_ptr = std::make_shared<InformativeSystem>(*this);
	for (auto i = 0; i < matrix.size(); i++)
		_matrix.push_back(std::make_shared<Object>(matrix[i], i, this_ptr));
}

AI::AtributeType AI::InformativeSystem::GetAtributeType(int index) const { return _attributeTypes.size() ? _attributeTypesMap.at(_attributeTypes[index]) : kNumeric; }

AI::Array<AI::Attribute>& AI::InformativeSystem::GetAttributesAtIndex(int index)
{
	assert(index < _matrix.size());

	// Lazy initialization:
	auto& ret = _mapByIndex[index];
	if (ret.empty())
	{
		for (auto i = 0; i < _matrix.size(); i++)
		{
			ret.push_back(_matrix[i]->At(index));
		}
	}
	return ret;
}

AI::Array<float>& AI::InformativeSystem::GetAttributesValues(int index)
{
	assert(index < _matrix.size());
	if (!_atributeToFloat.count(index))
	{
		auto& att = GetAttributesAtIndex(index);
		std::vector<float> ret;
		for (Attribute& ent : att)
			ret.push_back(ent.GetAsFloat());

		_atributeToFloat[index] = ret;
	}

	return _atributeToFloat[index];
}
