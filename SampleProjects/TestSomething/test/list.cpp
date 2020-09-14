#include <ArduinoUnitTests.h>
#include <List.h>

unittest(list_constructor) {
    List<int> list1;
    List<bool> *pList2 = new List<bool>;
    List<bool> *pList3 = new List<bool>();
    List<bool> *pList4 = new List<bool>(0);
    List<bool> *pList5 = new List<bool>(99);
    assertEqual(0, list1.size());
    assertEqual(0, pList2->size());
    assertEqual(0, pList3->size());
    assertEqual(0, pList4->size());
    assertEqual(0, pList5->size());
}

unittest(list_reset) {
    List<int> list;
    assertEqual(0, list.size());
    list.pushBack(0);
    assertEqual(1, list.size());
    list.reset();
    assertEqual(0, list.size());
}

unittest(list_isEmpty) {
    List<int> list;
    assertTrue(list.isEmpty());
    list.pushBack(0);
    assertFalse(list.isEmpty());
    list.reset();
    assertTrue(list.isEmpty());
}

unittest(list_at) {
    List<int> list;
    list.pushBack(100);
    list.pushBack(101);
    list.pushBack(102);
    assertEqual(100, list.at(0));
    assertEqual(101, list.at(1));
    assertEqual(102, list.at(2));
    try {
        list.at(-1);
        assertTrue(false);
    } catch (std::runtime_error& e) {
        assertTrue(true);
    }
    try {
        list.at(3);
        assertTrue(false);
    } catch (std::runtime_error& e) {
        assertTrue(true);
    }
}

unittest(list_atPut) {
    List<int> list;
    list.pushBack(100);
    list.pushBack(101);
    list.pushBack(102);
    list.atPut(0, 200);
    list.atPut(1, 201);
    list.atPut(2, 202);
    assertEqual(200, list.at(0));
    assertEqual(201, list.at(1));
    assertEqual(202, list.at(2));
    try {
        list.atPut(-1, 0);
        assertTrue(false);
    } catch (std::runtime_error& e) {
        assertTrue(true);
    }
    try {
        list.atPut(3, 300);
        assertTrue(false);
    } catch (std::runtime_error& e) {
        assertTrue(true);
    }
}

unittest(list_popFront) {
    List<int> list;
    list.pushBack(100);
    list.pushBack(101);
    list.pushBack(102);
    assertEqual(100, list.popFront());
    assertEqual(101, list.popFront());
    assertEqual(102, list.popFront());
    try {
        list.popFront();
        assertTrue(false);
    } catch (std::runtime_error& e) {
        assertTrue(true);
    }
}

unittest(list_peekHead) {
    List<int> list;
    list.pushBack(100);
    list.pushBack(101);
    list.pushBack(102);
    assertEqual(3, list.size());
    assertEqual(100, list.peekHead());
    assertEqual(3, list.size());
    assertEqual(100, list.peekHead());
    list.reset();
    try {
        list.peekHead();
        assertTrue(false);
    } catch (std::runtime_error& e) {
        assertTrue(true);
    }
}

unittest(list_peekTail) {
    List<int> list;
    list.pushBack(100);
    list.pushBack(101);
    list.pushBack(102);
    assertEqual(3, list.size());
    assertEqual(102, list.peekTail());
    assertEqual(3, list.size());
    assertEqual(102, list.peekTail());
    list.reset();
    try {
        list.peekTail();
        assertTrue(false);
    } catch (std::runtime_error& e) {
        assertTrue(true);
    }
}

unittest(list_indexOf) {
    List<int> list;
    list.pushBack(100);
    list.pushBack(101);
    list.pushBack(102);
    assertEqual(-1, list.indexOf( 99));
    assertEqual( 0, list.indexOf(100));
    assertEqual( 1, list.indexOf(101));
    assertEqual( 2, list.indexOf(102));
    assertEqual(-1, list.indexOf(103));
}

unittest(list_removeAt) {
    List<int> list;
    list.pushBack(100);
    list.pushBack(101);
    list.pushBack(102);
    assertEqual(3, list.size());
    assertEqual(100, list.removeAt(0));
    assertEqual(2, list.size());
    assertEqual(101, list.at(0));
    assertEqual(102, list.at(1));
    assertEqual(102, list.removeAt(1));
    assertEqual(1, list.size());
    assertEqual(101, list.at(0));
    try {
        list.removeAt(-1);
        assertTrue(false);
    } catch (std::runtime_error& e) {
        assertTrue(true);
    }
    try {
        list.removeAt(2);
        assertTrue(false);
    } catch (std::runtime_error& e) {
        assertTrue(true);
    }
}

unittest(list_remove) {
    List<int> list;
    list.pushBack(100);
    list.pushBack(101);
    list.pushBack(102);
    assertEqual(3, list.size());
    assertEqual(100, list.remove(100));
    assertEqual(2, list.size());
    assertEqual(101, list.at(0));
    assertEqual(102, list.at(1));
    assertEqual(102, list.remove(102));
    assertEqual(1, list.size());
    assertEqual(101, list.at(0));
    try {
        list.remove(99);
        assertTrue(false);
    } catch (std::runtime_error& e) {
        assertTrue(true);
    }
}

unittest_main()
