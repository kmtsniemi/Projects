#include "airplane.hh"
#include "errors/gameerror.hh"

Airplane::Airplane()
{

}

Airplane::~Airplane()
{

}

Interface::Location Airplane::giveLocation() const
{
    return location_;
}

void Airplane::move(Interface::Location loc)
{
    location_ = loc;
}

bool Airplane::isRemoved() const
{
    return removed_;
}

void Airplane::remove()
{
    removed_ = true;
}

void Airplane::setLocation(int &x, int &y)
{
    location_.setXY(x, y);
}


