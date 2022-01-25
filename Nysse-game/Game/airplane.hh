/* Nysse
 *
 * Class: Airplane
 * ----------
 *
 * Program author
 * Name: Julia Harttunen and Krista Mätäsniemi
 * Student number: 291740 and 292064
 * UserID: mqjuha and sgkrma
 * E-Mail: julia.harttunen@tuni.fi and krista.matasniemi@tuni.fi
 *
 * */

#ifndef AIRPLANE_HH
#define AIRPLANE_HH

#include "interfaces/ivehicle.hh"
#include "interfaces/icity.hh"

class Airplane : public Interface::IActor
{

public:

    /**
     * @brief Default constructor
     */
    Airplane();

    /**
      * @brief Default destructor
      */
    ~Airplane();

    /**
     * @brief giveLocation returns the location of the airplane.
     * @pre -
     * @return Airplanes location.
     * @post Exception guarantee: strong.
     * @exception GameError Airplane wasn't given a location.
     */
    Interface::Location giveLocation() const;

    /**
     * @brief move-method moves the airplane to given location.
     * @param loc Airplane new location.
     * @pre -
     * @post Airplane location is sij. Excaption guarantee: strong.
     * @exception GameError Location is not possible.
     */
    void move(Interface::Location loc);

    /**
     * @brief isRemoved tells if the airplane is removed ingame.
     * @pre -
     * @return `true`, if actor is removed ingame, otherwise `false`.
     * @post Exception guarantee: nothrow.
     */
    bool isRemoved() const;

    /**
     * @brief remove marks the airplane as removed.
     * @pre Airplane is not removed already.
     * @post Airplane is removed, after this isRemoved() returns `true`. Exception guarantee: basic.
     */
    void remove();

    /**
     * @brief setLocation sets new coordinates to the airplane.
     * @param new x-coordinate
     * @param new y-coordinate
     */
    void setLocation(int& x, int& y);

private:
    // Location object
    Interface::Location location_;
    // Tells if item is removed
    bool removed_;
};

#endif // AIRPLANE_HH
