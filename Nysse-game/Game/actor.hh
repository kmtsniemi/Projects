/* Nysse
 *
 * Class: Actor
 * ----------
 *
 * Program author
 * Name: Julia Harttunen and Krista Mätäsniemi
 * Student number: 291740 and 292064
 * UserID: mqjuha and sgkrma
 * E-Mail: julia.harttunen@tuni.fi and krista.matasniemi@tuni.fi
 *
 * */

#ifndef ACTOR_HH
#define ACTOR_HH
#include "interfaces/iactor.hh"
#include "core/location.hh"
#include <graphics/simpleactoritem.hh>

class Actor : public Interface::IActor
{

public:

    /**
      * @brief Default constructor
      * @post Actor isn't in removed-state by default.
      */
    Actor();
    
    /**
      * @brief Defaultdestructor
      */
    ~Actor() = default;
    
    /**
     * @brief giveLocation returns the location of the actor.
     * @pre -
     * @return Actors location.
     * @post Exception guarantee: strong.
     * @exception GameError - actor wasn't given a location.
     */
    Interface::Location giveLocation() const;
    
    /**
     * @brief move-method moves the actor to given location.
     * @param loc Actors new location.
     * @pre -
     * @post Actors location is sij. Excaption guarantee: strong.
     * @exception GameError Location is not possible.
     */
    void move(Interface::Location loc);
    
    /**
     * @brief isRemoved tells if the actor is removed ingame.
     * @pre -
     * @return `true`, if actor is removed ingame, otherwise `false`.
     * @post Exception guarantee: nothrow.
     */
    bool isRemoved() const;
    
    /**
     * @brief remove marks the actor as removed.
     * @pre Actor is not removed already.
     * @post Actor is removed, after this isRemoved() returns `true`. Exception guarantee: basic.
     */
    void remove();

    /**
     * @brief setLocation sets new location to the actor
     * @param locations x-coordinate
     * @param locations y-coordinate
     */
    void setLocation(int& x, int& y);

private:
    // Location object
    Interface::Location location_;
    // Tells if item is removed
    bool removed_;
};

#endif // ACTOR_HH
