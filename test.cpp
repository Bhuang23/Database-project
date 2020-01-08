/*test.cpp*/

//
// Lab week 09 / project #04
// 
// Testing for util.cpp?
//

#include <iostream>
#include <vector>

#include "avl.h"
#include "util.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

TEST_CASE("(1)") 
{
	REQUIRE(GetRecord("students", 0, 5)[1] == "pooja");

}
TEST_CASE("(2)") 
{
	REQUIRE(LinearSearch("students", 82, 5, "kim", 3)[0] == 164);
	REQUIRE(LinearSearch("students", 82, 5, "kim", 3)[1] == 410);

}