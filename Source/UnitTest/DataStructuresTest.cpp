#include "CppUnitTest.h"
#include "..\Engine\Array.h"
#include "..\Engine\HashTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(ArrayTest)
	{
	public:
		
        TEST_METHOD(ValuePushTest)
		{
            Array<int32> testArr;
            testArr.Push(3);
            testArr.Push(2);

            Assert::AreEqual(3, testArr[0]);
            Assert::AreEqual(2, testArr[1]);
            Assert::AreEqual(2u, testArr.Size());

            testArr.Push(1);
            testArr[0] = 4;

            Assert::AreEqual(4, testArr[0]);
            Assert::AreEqual(1, testArr[2]);
            Assert::AreEqual(3u, testArr.Size());
		}

        TEST_METHOD(ValuePopTest)
        {
            Array<int32> testArr;
            testArr.Push(3);
            testArr.Push(2);
            testArr.Push(1);
            testArr[0] = 4;

            Assert::AreEqual(1, testArr.Pop());
            Assert::AreEqual(2u, testArr.Size());
            Assert::AreEqual(2, testArr.Pop());
            Assert::AreEqual(1u, testArr.Size());
            Assert::AreEqual(4, testArr.Pop());
            Assert::AreEqual(0u, testArr.Size());
            testArr.Push(1);
            Assert::AreEqual(1, testArr.Pop());
            Assert::AreEqual(0u, testArr.Size());
        }

        TEST_METHOD(PointerPushTest)
        {
            int intArr[3] = { 3, 4, 5 };
            Array<int32 *> testArr;
            testArr.Push(&intArr[0]);
            testArr.Push(&intArr[1]);
            testArr.Push(&intArr[2]);
            Assert::AreEqual(3u, testArr.Size());

            *testArr[0] = 6;
            *testArr[1] = 7;
            *testArr[2] = 8;

            Assert::AreEqual(6, intArr[0]);
            Assert::AreEqual(7, intArr[1]);
            Assert::AreEqual(8, intArr[2]);
        }

        TEST_METHOD(DeleteTest)
        {
            Array<int32> testArr;
            testArr.Push(3);
            testArr.Push(2);
            testArr.Push(1);
            testArr[0] = 4;
            Assert::AreEqual(4, testArr[0]);
            Assert::AreEqual(2, testArr[1]);
            Assert::AreEqual(1, testArr[2]);
            Assert::AreEqual(3u, testArr.Size());

            testArr.Delete(0);
            Assert::AreEqual(1, testArr[0]);
            Assert::AreEqual(2, testArr[1]);
            Assert::AreEqual(2u, testArr.Size());

            testArr.Push(3);
            Assert::AreEqual(3u, testArr.Size());
        }

        TEST_METHOD(CopyTest)
        {
            Array<int32> testArr;
            testArr.Push(1);
            testArr.Push(2);
            testArr.Push(3);

            Array<int32> testArr2 = testArr;
            testArr.Clear();

            Assert::AreEqual(0u, testArr.Size());
            Assert::AreEqual(3u, testArr2.Size());
            Assert::AreEqual(1, testArr2[0]);
            Assert::AreEqual(2, testArr2[1]);
            Assert::AreEqual(3, testArr2[2]);
        }

        TEST_METHOD(StressTest)
        {
            Array<int32> testArr;
            for (uint32 i = 0; i < 100; ++i) {
                testArr.Push(i);
            }
            Assert::AreEqual(100u, testArr.Size());
            for (uint32 i = 0; i < 100; ++i) {
                Assert::AreEqual((int)(99 - i), testArr.Pop());
            }
            Assert::AreEqual(0u, testArr.Size());
            for (uint32 i = 0; i < 100; ++i) {
                testArr.Push(i);
            }
            Assert::AreEqual(100u, testArr.Size());
            Assert::AreEqual(100u, testArr.Size());
            for (uint32 i = 0; i < 100; ++i) {
                Assert::AreEqual((int)i, testArr[i]);
            }
            Assert::AreEqual(100u, testArr.Size());
        }

	};

    TEST_CLASS(HashTableTest)
    {
    public:

        TEST_METHOD(ValueInsertTest)
        {
            HashTable<EntityId, uint32> testTable;
            testTable[1] = 3u;
            testTable[2] = 4u;
            testTable[3] = 5u;

            Assert::AreEqual(5u, testTable[3]);
            Assert::AreEqual(3u, testTable[1]);
            Assert::AreEqual(4u, testTable[2]);
            Assert::AreEqual(3u, testTable.Size());
        }

        TEST_METHOD(StressTest)
        {
            HashTable<EntityId, uint32> testTable;
            for (uint32 i = 0; i < 100; ++i) {
                testTable[i] = i;
            }
            Assert::AreEqual(100u, testTable.Size());
            for (uint32 i = 0; i < 100; ++i) {
                Assert::AreEqual(i, testTable[i]);
            }
            testTable.Clear();
            Assert::AreEqual(0u, testTable.Size());
        }
    };
}