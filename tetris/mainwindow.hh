/* Class: MainWindow
  * ----------
  * This class is the mainwindow of Tetris game.
  *
  * Name: Krista Mätäsniemi
  * E-Mail: krista.matasniemi@tuni.fi
  **/
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <random>
#include <QGraphicsRectItem>
#include <vector>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void on_startButton_clicked();

    void on_leftButton_clicked();

    void on_rightButton_clicked();

    void on_newGameButton_clicked();

    void on_helpButton_clicked();

    void on_pushButton_pressed();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene_;
    
    //Time recording items
    double minutes_;
    double seconds_;
    
    //Parts of a falling block
    std::vector<QGraphicsRectItem*> tetromino_;
    
    //Grid about blocks in place
    std::vector<std::vector<int>> grid_;
    
    //Timer object
    QTimer timer_;
    
    //Score of the game
    int points_;
    
    //Tetromino shapes
    const std::vector<std::vector<std::vector<int>>> SHAPES = {
        { { 4, 0 }, { 5, 0 }, { 6, 0 }, { 7, 0 } },         //I
        { { 4, 0  }, { 5, 0  }, { 5, 1 }, { 6, 1 } },       //Z
        { { 5, 0  }, { 6, 0  }, { 5, 1 }, { 6, 1 } },       //O
        { { 4, 1 }, { 5, 0  }, { 5, 1 }, { 6, 0 } },        //S
        { { 4, 0  }, { 4, 1 }, { 5, 0  }, { 6, 0  } },      //T
        { { 4, 0  }, { 5, 0  }, { 6, 1 }, { 6, 0  } },      //L
        { { 4, 0  }, { 6, 0  }, { 5, 0  }, { 5, 1 } } };    //J

    //Colors of the tetrominos
    const std::vector<QBrush> COLORS = {Qt::red, Qt::blue, Qt::magenta, Qt::green, Qt::yellow, Qt::cyan, Qt::gray};

    //Tetrominos moving directions
    const std::string MOVE_RIGHT = "R";
    const std::string MOVE_LEFT = "L";
    const std::string MOVE_DOWN = "D";

    // Constants describing scene coordinates
    // Copied from moving circle example and modified a bit
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480; // 260; (in moving circle)
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 240; // 680; (in moving circle)

    // Size of a tetromino component
    const int SQUARE_SIDE = 20;
    // Number of horizontal cells (places for tetromino components)
    const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;
    // Number of vertical cells (places for tetromino components)
    const int ROWS = BORDER_DOWN / SQUARE_SIDE;

    // Constants for different tetrominos and the number of them
    enum Tetromino_kind {HORIZONTAL,
                         LEFT_CORNER,
                         RIGHT_CORNER,
                         SQUARE,
                         STEP_UP_RIGHT,
                         PYRAMID,
                         STEP_UP_LEFT,
                         NUMBER_OF_TETROMINOS};
    // From the enum values above, only the last one is needed in this template.
    // Recall from enum type that the value of the first enumerated value is 0,
    // the second is 1, and so on.
    // Therefore the value of the last one is 7 at the moment.
    // Remove those tetromino kinds above that you do not implement,
    // whereupon the value of NUMBER_OF_TETROMINOS changes, too.
    // You can also remove all the other values, if you do not need them,
    // but most probably you need a constant value for NUMBER_OF_TETROMINOS.


    // For randomly selecting the next dropping tetromino
    std::default_random_engine randomEng;
    std::uniform_int_distribution<int> distr;

    // More constants, attibutes, and methods

    /**
     * @brief new_block
     * Creates a new tetromino
     */
    void new_block();
    
    /**
     * @brief move_down
     * Moves the falling tetrominos down
     */
    void move_down();
    
    /**
     * @brief move_side
     * @param direction "moving direction"
     * Moves the tetromino in the desired direction
     */
    void move_side(std::string direction);
    
    /**
     * @brief has_set
     * @param direction "moving direction"
     * @return true if tetrimino stops the outlines of gameboard of other tetrimono
     *          otherwise false
     * Checks can tetrimino keep going moving
     */
    bool has_set(std::string direction);
    
    /**
     * @brief second_gone
     * Counts seconds and updates time
     */
    void second_gone();

    /**
     * @brief create_grid
     * Creates a grid
     */
    void create_grid();

    /**
     * @brief game_over
     * Checks if tetriminos touch top of gameboard
     */
    void game_over();

    /**
     * @brief add_points
     * Checks full tetrininolines and updates score
     */
    void add_points();
};

#endif // MAINWINDOW_HH
