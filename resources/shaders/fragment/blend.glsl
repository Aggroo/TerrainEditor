float Multiply(float a, float b)
{
	return a*b;
}

float Darken(float a, float b)
{
	return min(b,a);
}

float Lighten(float a, float b)
{
	return max(b,a);
}

float Difference(float a, float b)
{
	return abs(a-b);
}

float ColorBurn(float a, float b)
{
	return (b == 0.0) ? b: max((1.0 -((1.0-a) / b)), 0.0);
}

float LinearBurn(float a, float b)
{
	return max(a+b-1.0, 0.0);
}

float Screen(float a, float b)
{
	return 1.0 - ((1.0-a)*(1.0-b));
}

float ColorDodge(float a, float b)
{
	return (b == 1.0) ? b: min((a / 1.0-b),1.0);
}

float LinearDodge(float a, float b)
{
	return min(a+b, 1.0);
}

float SoftLight(float a, float b)
{
	if(b < 0.5)
	{
		return 2.0*a*b+a*a*(1.0-2.0*b);
	}
	return (sqrt(a)*(2.0*b-1.0)+2.0*a*(1.0-b));
}

float Overlay(float a, float b)
{
	if(a < 0.5)
	{
		return 2.0*a*b;
	}
	return 1.0 - 2.0*(1.0-a)*(1.0-b);
}