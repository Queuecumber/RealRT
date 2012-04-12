#pragma once


class Light
{
public:
    virtual ~Light(void){}

	bool IsLight(){return true;}

protected:
    Light(){}
};
