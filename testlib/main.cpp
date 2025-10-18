#include <QCoreApplication>
#include <QtTest>
#include "breadboardmodel.h"

class TestBreadboardModel : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void testBuildHoles();
    void testFindHole();
    void testIsHoleOccupied();
    void testSameGroup();
    void testAddConnection();
    void testAddComponent();
    void testMoveComponent();
    void testDeleteComponent();
    void testClear();
    void testIsHoleOccupied_moving();
    void testMoveConnection();

private:
    BreadboardModel* m_model;
};

void TestBreadboardModel::init()
{
    m_model = new BreadboardModel();
}

void TestBreadboardModel::cleanup()
{
    delete m_model;
}

void TestBreadboardModel::testBuildHoles()
{
    QList<QList<Hole>> holes = m_model->getHoles();
    QCOMPARE(holes.size(), 14); // 2 + 5 + 5 + 2
    QCOMPARE(holes[0].size(), 30);
    QCOMPARE(holes[0][0].band, "power-top");
}

void TestBreadboardModel::testFindHole()
{
    Hole* h = m_model->findHole("power-top:0:0");
    QVERIFY(h != nullptr);
    QCOMPARE(h->band, QString("power-top"));
    QCOMPARE(h->r, 0);
    QCOMPARE(h->c, 0);

    Hole* not_found = m_model->findHole("non-existent:0:0");
    QVERIFY(not_found == nullptr);
}

void TestBreadboardModel::testIsHoleOccupied()
{
    QVERIFY(!m_model->isHoleOccupied("power-top:0:0"));
    m_model->addConnection("power-top:0:0", "power-top:0:1", "#ff0000", {});
    QVERIFY(m_model->isHoleOccupied("power-top:0:0"));
}

void TestBreadboardModel::testSameGroup()
{
    Hole h1 = {"power-top", 0, 0, 0, 0};
    Hole h2 = {"power-top", 0, 1, 0, 0};
    Hole h3 = {"power-top", 1, 0, 0, 0};
    Hole h4 = {"term-top", 0, 0, 0, 0};

    QVERIFY(m_model->sameGroup(h1, h2));
    QVERIFY(!m_model->sameGroup(h1, h3));
    QVERIFY(!m_model->sameGroup(h1, h4));
}

void TestBreadboardModel::testAddConnection()
{
    m_model->addConnection("power-top:0:0", "power-top:0:1", "#ff0000", {});
    QCOMPARE(m_model->getConnections().size(), 1);
    QVERIFY(m_model->isHoleOccupied("power-top:0:0"));
    QVERIFY(m_model->isHoleOccupied("power-top:0:1"));
}

void TestBreadboardModel::testAddComponent()
{
    m_model->addComponent({"term-top:0:0", "term-top:1:0"});
    QCOMPARE(m_model->getComponents().size(), 1);
    QVERIFY(m_model->isHoleOccupied("term-top:0:0"));
    QVERIFY(m_model->isHoleOccupied("term-top:1:0"));
}

void TestBreadboardModel::testMoveComponent()
{
    m_model->addComponent({"term-top:0:0", "term-top:1:0"});
    QString componentId = m_model->getComponents()[0].id;
    m_model->moveComponent(componentId, "term-top:0:0", "term-top:0:1");
    QVERIFY(!m_model->isHoleOccupied("term-top:0:0"));
    QVERIFY(m_model->isHoleOccupied("term-top:0:1"));
}

void TestBreadboardModel::testDeleteComponent()
{
    m_model->addComponent({"term-top:0:0", "term-top:1:0"});
    QString componentId = m_model->getComponents()[0].id;
    m_model->deleteComponent(componentId);
    QCOMPARE(m_model->getComponents().size(), 0);
    QVERIFY(!m_model->isHoleOccupied("term-top:0:0"));
}

void TestBreadboardModel::testClear()
{
    m_model->addConnection("power-top:0:0", "power-top:0:1", "#ff0000", {});
    m_model->addComponent({"term-top:0:0", "term-top:1:0"});
    m_model->clear();
    QCOMPARE(m_model->getConnections().size(), 0);
    QCOMPARE(m_model->getComponents().size(), 0);
}

void TestBreadboardModel::testIsHoleOccupied_moving()
{
    m_model->addComponent({"term-top:0:0", "term-top:1:0"});
    QString componentId = m_model->getComponents()[0].id;
    m_model->setMovingComponentId(componentId);
    QVERIFY(!m_model->isHoleOccupied("term-top:0:0"));
}

void TestBreadboardModel::testMoveConnection()
{
    m_model->addConnection("power-top:0:0", "power-top:0:1", "#ff0000", {});
    QString connectionId = m_model->getConnections()[0].id;
    m_model->setMovingConnectionId(connectionId);
    m_model->moveConnection(connectionId, "power-top:0:1", "power-top:0:2");
    QVERIFY(!m_model->isHoleOccupied("power-top:0:1"));
    QVERIFY(m_model->isHoleOccupied("power-top:0:2"));
}

QTEST_MAIN(TestBreadboardModel)
#include "main.moc"