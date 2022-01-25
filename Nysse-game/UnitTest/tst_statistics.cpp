#include <QtTest>
#include "statistics.hh"

class statistics : public QObject
{
    Q_OBJECT

public:
    statistics();
    ~statistics();

private slots:

    /**
     * @brief testAddPoints sets points, add more points
     * and compares the result
     */
    void testAddPoints();

    /**
     * @brief testOverPoints sets points, add points so
     * much that it goes over 50 and compares the result
     * to 20
     */
    void testOverPoints();

    /**
     * @brief testReadFile sets filename, reads file
     * and compares the file to info
     */
    void testReadFile();

    /**
     * @brief testSetFilename sets filename and compares
     * the filename
     */
    void testSetFilename();

    /**
     * @brief testSetPoints sets points and compares
     * the points
     */
    void testSetPoints();

    /**
     * @brief testSetZeroPoints sets points, add zero
     * points and compares the result
     */
    void testSetZeroPoints();

    /**
     * @brief testReadFile2 sets filename, reads file
     * and compares the times
     */
    void testReadFile2();

    /**
     * @brief testDouplePointsOver sets points that is
     * over 25, doubles the points and compares the
     * result to 20
     */
    void testDouplePointsOver();

    /**
     * @brief testDouplePoints sets points, doubles
     * the points and compares the result
     */
    void testDouplePoints();

    /**
     * @brief testSetNameAndTime sets name and time and
     * compares them
     */
    void testSetNameAndTime();

private:
    std::map<int, std::string> info = {{120,"Krista"},{166, "Julia"},
                                       {89, "Juho"},{42,"Otto"},{219,"Matias"}};
    std::list<int> times = {120, 166, 89, 42, 219};
    int points = 37;
    std::string filename = "test_top10list.txt";
    std::string name = "Jere";
    int time = 333;
};

statistics::statistics()
{

}

statistics::~statistics()
{

}

void statistics::testAddPoints()
{
    Statistics s;
    s.setPoints(points);
    s.passengerDied(2);
    QCOMPARE(s.givePoints(), 39);
}

void statistics::testOverPoints()
{
    Statistics s;
    s.setPoints(points);
    s.passengerDied(20);
    QCOMPARE(s.givePoints(), 20);
}

void statistics::testReadFile()
{
    Statistics s;
    s.setFilename(filename);
    s.readOldTopList();
    QCOMPARE(s.giveInfo(), info);
}

void statistics::testSetFilename()
{
    Statistics s;
    s.setFilename(filename);
    QCOMPARE(s.giveFilename(), filename);
}

void statistics::testSetPoints()
{
    Statistics s;
    s.setPoints(points);
    QCOMPARE(s.givePoints(), points);
}

void statistics::testSetZeroPoints()
{
    Statistics s;
    s.setPoints(points);
    s.passengerDied(0);
    QCOMPARE(s.givePoints(), points);
}

void statistics::testReadFile2()
{
    Statistics s;
    s.setFilename(filename);
    s.readOldTopList();
    QCOMPARE(s.giveTimes(), times);
}

void statistics::testDouplePointsOver()
{
    Statistics s;
    s.setPoints(points);
    s.douplePoints();
    QCOMPARE(s.givePoints(), 20);
}

void statistics::testDouplePoints()
{
    Statistics s;
    s.setPoints(10);
    s.douplePoints();
    QCOMPARE(s.givePoints(), 20);
}

void statistics::testSetNameAndTime()
{
    Statistics s;
    s.setNameAndTime(name, time);
    QCOMPARE(s.giveName(), name);
    QCOMPARE(s.giveTime(), time);
}

QTEST_APPLESS_MAIN(statistics)

#include "tst_statistics.moc"
