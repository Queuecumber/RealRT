#pragma once

#include "..\RTEngine\RTEngine.h"
#include "MShape3D.h"

using namespace System;

namespace RTEngineInvoke
{

	public ref class MSphere : public MShape3D
	{
	public:
		property double Center
		{
			double get();
			void set(double c);
		}

		property double Radius
		{
			double get();
			void set(double r);
		}

		property sphere *Native
		{
			sphere *get();
			void set(sphere *);
		}

	public:
		MSphere(Material ^mat,Vector3D ^center, double radius);
		~MSphere();

	public:
		  virtual double Intersection(Ray ^incident, bool &flipnormals) override;
		  virtual Vector3D ^Normal(Vector3D ^pt) override;

	private:
		sphere *nativeSphere;
	};

}
