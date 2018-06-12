#include "config.h"
#include "materialparameter.h"

namespace Render
{
MaterialParameter::MaterialParameter() : type(ParameterTypes::Void)
{
}

MaterialParameter::~MaterialParameter()
{

}

ParameterTypes MaterialParameter::GetType()
{
	return type;
}
}
