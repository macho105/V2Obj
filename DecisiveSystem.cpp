#include "DecisiveSystem.h"
#include <cassert>

AI::DecisiveSystem::DecisiveSystem(Array2D<std::string> matrix, Array<std::string> decisions, Array<std::string> types)
: InformativeSystem(matrix, types), _decisions(decisions) { }

std::vector<float>& AI::DecisiveSystem::GetAttributesValues(int index)
{
	assert(index == -1 || index < _matrix.size());

	if (index != -1)
		return InformativeSystem::GetAttributesValues(index);
	
	if (_fDecisions.size())
		return _fDecisions;

	if (index == -1)
		for (auto i = 0; i < GetDecisions().size(); i++)
			_fDecisions.push_back(GetDecision(i));
	else
		throw std::exception("Invalid index");
	
	return _fDecisions;
}




