#include "pch.h"
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestEngine
{
	TEST_CLASS(UnitTestEngine)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
		
		}
	};
}



//void SATCollisionTestColliding1(){
//    //Arrange
//    auto r1Pos = glm::vec2(0.0f, 0.0f);
//    auto r2Pos = glm::vec2(10.0f, 0.1f);
//    Rectangle<glm::vec2> r1(glm::vec2(10.0f,10.0f), r1Pos), r2(glm::vec2(10.0f, 10.0f), r2Pos);
// 
//    //Act
//    auto isCollision = SATCollision(RectToArray(r1), RectToArray(r2));
//
//    //Assert
//    assert(isCollision == true);
//}
//
//void SATCollisionTestNotColliding1() {
//    //Arrange
//    auto r1Pos = glm::vec2(0.0f, 0.0f);
//    auto r2Pos = glm::vec2(21.0f, 0.0f);
//    Rectangle<glm::vec2> r1(glm::vec2(10.0f, 10.0f), r1Pos), r2(glm::vec2(10.0f, 10.0f), r2Pos);
//
//    //Act
//    auto isCollision = SATCollision(RectToArray(r1), RectToArray(r2));
//
//    //Assert
//    assert(isCollision == false);
//}


void Tests() {
	//SATCollisionTestColliding1();
	//SATCollisionTestNotColliding1();
}
