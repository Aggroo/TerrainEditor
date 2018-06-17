
namespace Math
{

float static InverseLerp(float a, float b, float value)
{
	if (a == b)
	{
		if (value < a)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return ((value - a) / (b - a));
	}
}

float static Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

}



