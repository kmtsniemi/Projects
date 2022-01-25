/* Nysse
 *
 * Class: StartWindow
 * ----------
 *
 * Program author
 * Name: Julia Harttunen and Krista Mätäsniemi
 * Student number: 291740 and 292064
 * UserID: mqjuha and sgkrma
 * E-Mail: julia.harttunen@tuni.fi and krista.matasniemi@tuni.fi
 *
 * */

#ifndef STARTWINDOW_HH
#define STARTWINDOW_HH

#include <QDialog>
#include "mainwindow.hh"

namespace Ui {
class StartWindow;
}

class StartWindow : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief  Default constructor
     * @param QWidget object
     */
    explicit StartWindow(QWidget *parent = nullptr);

    /**
      * @brief Default destructor
      */
    ~StartWindow();

signals:

    /**
     * @brief Send player's name
     * @param Player's name
     */
    void wantstoPlay(QString name);

private slots:

    /**
     * @brief Shows welcome text
     * @param Player's name
     */
    void on_nameLineEdit_textChanged(const QString &arg1);

    /**
     * @brief Opens game window
     */
    void on_playPushButton_clicked();

private:

    // Start window
    Ui::StartWindow *ui;
    // Player's name
    QString name;
};

#endif // STARTWINDOW_HH
