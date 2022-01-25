#include "actor.hh"
#include "interfaces/iactor.hh"
#include "errors/gameerror.hh"
#include <QDebug>

Actor::Actor() : location_()
{
    
}

Interface::Location Actor::giveLocation() const
{
    return location_;
}

void Actor::move(Interface::Location loc)
{
    location_ = loc;
    qDebug() << "Actors location is sij.";
}

bool Actor::isRemoved() const
{
    return removed_;
}

void Actor::remove()
{
    removed_ = true;
    qDebug() << "Actor is removed, after this isRemoved() returns `true`.";
}

void Actor::setLocation(int &x, int &y)
{
    location_.setXY(x, y);
}




