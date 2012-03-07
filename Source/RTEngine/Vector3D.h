#pragma once
/*
	vector<3,double>.h

	definition of the vector<3,double> class which represents a 
	std::vector in three dimensions. Single points should be
	represented this way as position vectors
*/

#include <math.h>
#include "stdarg.h"

template<int dim,class t>
class vector
{
public:				
	/*vector(vector<dim,t> &cp)		//copy constructors
	{
		for(int i = 0; i < dim; i++)
			componants[i] = cp.GetComponant(i);
	}	  */

	vector(vector<dim,t> *cp)
	{
		componants = new t[dim];

		for(int i = 0; i < dim; i++)
			componants[i] = cp->GetComponant(i);

		clip = (t)0;
	}

	vector(t first, t second, t third)				//componant representations
	{
		componants = new t[dim];

		/*va_list arguments;*/

		componants[0] = first;
		componants[1] = second;
		componants[2] = third;

		clip = (t)0;

		/*va_start(arguments,first);
		for(int i = 1; i < dim; i++)
			componants[i] = va_arg(arguments,t);
		va_end(arguments);*/
	}

	vector(t first, t second, t third, t fourth)				//componant representations
	{
		componants = new t[dim];

		/*va_list arguments;*/

		componants[0] = first;
		componants[1] = second;
		componants[2] = third;
		componants[3] = fourth;

		clip = (t)0;

		/*va_start(arguments,first);
		for(int i = 1; i < dim; i++)
			componants[i] = va_arg(arguments,t);
		va_end(arguments);*/

	}

	vector(t componants[dim])     
	{
		this->componants = new t[dim];

		for(int i = 0; i < dim; i++)
		{
			this->componants[i] = componants[i];
		}

		clip = (t)0;
	}

	~vector(void){}

	void operator += ( vector<dim,t> *a_V ) 
	{ 
		componants[0] += a_V->GetI(); 
		componants[1] += a_V->GetJ(); 
		componants[2] += a_V->GetK();

		if(clip)
		{
			for(int i = 0; i < 3; i++)
			{
				componants[i] = componants[i] > clip ? clip : componants[i];
			}
		}
	}

	void operator += ( vector<dim,t> &a_V ) 
	{ 
		componants[0] += a_V.GetI(); 
		componants[1] += a_V.GetJ(); 
		componants[2] += a_V.GetK(); 

		if(clip)
		{
			for(int i = 0; i < 3; i++)
			{
				componants[i] = componants[i] > clip ? clip : componants[i];
			}
		}
	}
																		  
	//access componants
	//
	t GetI(){return componants[0];}
	t GetJ(){return componants[1];}
	t GetK(){return componants[2];}
	t GetComponant(int index){return componants[index];}
	
	void Clip(t clipAt){clip = clipAt;}
	t Clip(){return clip;}

	//dot product for a reference and a pointer
	//
	t Dot(vector<dim,t> &against)
	{
		t dot = 0;
		for(int i = 0; i < dim; i++)
			dot += componants[i] * against.GetComponant(i);

		return dot;
	}

	t Dot(vector<dim,t> *against)
	{
		t dot = 0;
		for(int i = 0; i < dim; i++)
			dot += componants[i] * against->GetComponant(i);

		return dot;
	}

	//cross product
	vector<dim,t> *Cross(vector<dim,t> &against){return 0;}

	//make unit vector
	vector<dim,t> Normalize()
	{
		for(int i = 0; i < dim; i++)
			componants[i] /= Magnitude();

		return *this;
	}

	//magnitude and magnitude squared
	//
	t Magnitude()
	{
		//taylor expansion of the square root
		//return 1 + (1.0/2.0)*(SquareMagnitude()) - (1.0/8.0)*(SquareMagnitude()*SquareMagnitude()) + (1.0/16.0)*(SquareMagnitude()*SquareMagnitude()*SquareMagnitude())
		//	     - (5.0/128.0)*(SquareMagnitude()*SquareMagnitude()*SquareMagnitude()*SquareMagnitude());

		return (t)( sqrt( (double)(SquareMagnitude()) ) );
	}

	//double Magnitude();
	t SquareMagnitude()
	{
		t smag = 0;

		for(int i = 0; i < dim; i++)
			smag += componants[i]*componants[i];

		return smag;
	}

	//NORMALIZE at the class level
	static inline vector * _cdecl NORMALIZE(vector<dim,t> &in){return new vector<dim,t>( in.GetI()/in.Magnitude(),in.GetJ()/in.Magnitude(),in.GetK()/in.Magnitude() );}
	static inline vector * _cdecl NORMALIZE(vector<dim,t> *in){return new vector<dim,t>( in->GetI()/in->Magnitude(),in->GetJ()/in->Magnitude(),in->GetK()/in->Magnitude() );}

private:
	t *componants;
	t clip;
	
};

template<int dim,class t>
vector<dim,t> _cdecl operator-( vector<dim,t> &v1, vector<dim,t> &v2 )
{
	t componants[dim];
	for(int i = 0; i < dim; i++)
		componants[i] = v1.GetComponant(i) - v2.GetComponant(i);

	if(v1.Clip())
	{
		for(int i = 0; i < 3; i++)
		{
			componants[i] = componants[i] > v1.Clip() ? v1.Clip() : componants[i];
		}
	}

	return vector<dim,t>(componants);
}

template<int dim,class t>
vector<dim,t> _cdecl operator*( t scaler, vector<dim,t> &v2 )
{
	t componants[dim];

	for(int i = 0; i < dim; i++)
		componants[i] = scaler * v2.GetComponant(i);

	if(v2.Clip())
	{
		for(int i = 0; i < 3; i++)
		{
			componants[i] = componants[i] > v2.Clip() ? v2.Clip() : componants[i];
		}
	}

	return vector<dim,t>(componants);
}

template<int dim,class t>
vector<dim,t> _cdecl operator*( vector<dim,t> &v1, vector<dim,t> &v2 )
{
	t componants[dim];

	for(int i = 0; i < dim; i++)
		componants[i] = v1.GetComponant(i) * v2.GetComponant(i);

	if(v1.Clip())
	{
		for(int i = 0; i < 3; i++)
		{
			componants[i] = componants[i] > v1.Clip() ? v1.Clip() : componants[i];
		}
	}

	return vector<dim,t>(componants);
}										