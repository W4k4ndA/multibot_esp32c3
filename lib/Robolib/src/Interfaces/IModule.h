#ifndef MODULE_H
#define MODULE_H


class IModule
{
public:
    virtual ~IModule() = default;
    virtual void init() = 0;
    virtual void update() = 0;//opcional
};




#endif