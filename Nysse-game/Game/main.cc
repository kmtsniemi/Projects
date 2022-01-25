/* Nysse
 *
 * Desc:
 * Nysse is game where you try eat exactly 50 bus passenger.
 * Avatar can be moved up, down and sideways.
 *
 * Program author
 * Name: Julia Harttunen and Krista Mätäsniemi
 * Student number: 291740 and 292064
 * UserID: mqjuha and sgkrma
 * E-Mail: julia.harttunen@tuni.fi and krista.matasniemi@tuni.fi
 *
 * */

#include <memory>
#include <QApplication>
#include "graphics/simplemainwindow.hh"
#include "mainwindow.hh"
#include "startwindow.hh"
#include <QImage>
#include "offlinereader.hh"
#include "city.hh"
#include "core/logic.hh"
#include "statistics.hh"
#include "errors/gameerror.hh"
#include "errors/initerror.hh"
#include <QDebug>


int main(int argc, char *argv[])
{
    try
    {
        QApplication a(argc, argv);
        Q_INIT_RESOURCE(offlinedata);

        std::shared_ptr<CourseSide::Logic> logic=std::make_shared<CourseSide::Logic>();
        std::shared_ptr<City> c=std::make_shared<City>();
        std::shared_ptr<Statistics> s = std::make_shared<Statistics>();

        QImage img = QImage(":/offlinedata/offlinedata/kartta_pieni_500x500.png");
        QImage b_img = QImage(":/offlinedata/offlinedata/kartta_pieni_500x500.png");
        c->setBackground(img, b_img);
        c->getGameWindow()->takeCity(c);

        c->getGameWindow()->takeStatistics(s);
        c->setStatistics(s);

        logic->takeCity(c);
        logic->fileConfig();
        logic->finalizeGameStart();
        logic->addNewBuses();

        c->setLogic(logic);
        return a.exec();
    }
    catch (Interface::InitError const& virhe)
    {
        qDebug() << virhe.giveMessage();
        std::cerr << virhe.what() << std::endl;
    }
}

