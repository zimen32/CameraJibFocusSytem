#pragma once
class Encoder
{
public:

	int		Input;
	int		MinValue;
	int		MaxValue;
	int		Value;	
	float	StartAngle;
	float	StartHeadAngle;
	float	RawHeadAngle; 
	float	EncHeadXDiff;
	float	ArmLength;	
	float	StartDistance;
	float	RelativeStartHeadAngle;

	float	GetAngle();
	float	GetArc();
	float	GetChord();
	float	GetHeadAngle();
	float	GetBeta();
	float	GetGamma();
	float	GetEpsilon();
	float	GetDelta();
	float	GetTX();
	float	GetDistance();
	void	SetRawHeadAngle(float angle);
	void	SetStartAngle();
	void	SetHeadStartAngle();
	void	SetStartDistance(float distance);
	void	SetRelativeStartHeadAngle();

	Encoder();
	Encoder(int input);
	Encoder(int input, int minValue, int maxValue);
	~Encoder();
};	

