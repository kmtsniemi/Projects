#include <QtTest>

// add necessary includes here

class Statistics : public QObject
{
    Q_OBJECT

public:
    Statistics();
    ~Statistics();

private slots:
    void test_case1();

};

Statistics::Statistics()
{

}

Statistics::~Statistics()
{

}

void Statistics::test_case1()
{

}

QTEST_APPLESS_MAIN(Statistics)

#include "tst_statistics.moc"
