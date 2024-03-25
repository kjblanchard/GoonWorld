#pragma once
class ITween
{

public:
    virtual void Update(double& deltaSeconds) = 0;
    virtual ~ITween() {}
};