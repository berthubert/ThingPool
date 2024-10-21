#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "thingpool.hh"
using namespace std;

struct TestThing
{
  static unsigned int s_instances;
  TestThing()
  {
    ++s_instances;
  }
  ~TestThing()
  {
    --s_instances;
  }
};

unsigned int TestThing::s_instances;

TEST_CASE("basic test") {
  ThingPool<TestThing> tp;
  {
    auto lease1 = tp.getLease();
    CHECK(lease1->s_instances == 1);
    auto lease2 = tp.getLease();
    CHECK(lease1->s_instances == 2);
  }
  CHECK(TestThing::s_instances == 2);
  CHECK(tp.d_maxout==2);
  CHECK(tp.d_out==0);

  tp.clear();
  CHECK(TestThing::s_instances == 0);
}

TEST_CASE("release test") {
  ThingPool<TestThing> tp;

  auto lease1 = tp.getLease();
  CHECK(lease1->s_instances == 1);
  auto lease2 = tp.getLease();
  CHECK(lease1->s_instances == 2);
  CHECK(tp.d_out==2);
  lease2.release();
  
  CHECK(TestThing::s_instances == 2);
  CHECK(tp.d_maxout==2);
  CHECK(tp.d_out==1);

  tp.clear();
  CHECK(TestThing::s_instances == 1);
}

TEST_CASE("abandon test") {
  ThingPool<TestThing> tp;
  auto lease1 = tp.getLease();
  CHECK(lease1->s_instances == 1);

  auto lease2 = tp.getLease();
  CHECK(lease1->s_instances == 2);
  CHECK(tp.d_out==2);
  lease2.abandon();
  CHECK(lease1->s_instances == 1);
  CHECK(tp.d_out==1);
}

TEST_CASE("wrong destruction test") {
  auto tp = new ThingPool<TestThing>();
  auto lease1 = tp->getLease();
  CHECK(lease1->s_instances == 1);

  REQUIRE_THROWS_AS(delete tp, std::runtime_error);
}

