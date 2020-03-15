/**
 * @file ecs_test.cpp
 * @author HenryAWE
 * @brief Test the ECS framework
 */

#define BOOST_TEST_MODULE ecs_test
#include <boost/test/included/unit_test.hpp>
#include <sr/player/entity/entity.hpp>
#include <sr/player/component/transform.hpp>
#include <sr/player/component/move.hpp>
#include <sr/player/system/move.hpp>


BOOST_AUTO_TEST_CASE(test_entitiy, *boost::unit_test::tolerance(0.005))
{
    using namespace srose::player;

    using Id = entity::Entity::Id;
    Id id(1,1);
    BOOST_TEST_REQUIRE(id.id() == 0x100000001ul);

    entity::EntityManager mgr;
    auto e = mgr.CreateEntity();
    BOOST_TEST_REQUIRE(e.GetManager() == &mgr);
    BOOST_TEST_REQUIRE(e.GetId() == Id(1, 1));
    BOOST_TEST_REQUIRE(mgr.CreateEntity().GetId() == Id(2, 1));
    BOOST_TEST_REQUIRE(mgr.GetEntity(Id(1,1)).GetId() == e.GetId());
    BOOST_TEST_REQUIRE(mgr.ValidateEntity(Id(1, 1)));
    BOOST_TEST_REQUIRE(mgr.ValidateEntity(Id(2, 1)));
    BOOST_TEST_REQUIRE(mgr.GetEntity(Id(-1, -1)).GetId() == Id::INVALID);
    mgr.DestroyEntity(Id(1, 1));
    BOOST_TEST_REQUIRE(mgr.ValidateEntity(Id(1, 1)) == false);
    BOOST_TEST_REQUIRE(mgr.CreateEntity().GetId() == Id(1, 2));

    mgr.AssignComponent<component::Move>(Id(2,1), 1.0f, 0.0f);
    BOOST_TEST_REQUIRE(mgr.HasComponent<component::Move>(Id(2, 1)));
    BOOST_TEST_REQUIRE(mgr.HasComponent<component::Transform>(Id(2, 1)) == false);
    auto* c = mgr.GetComponent<component::Move>(Id(2, 1));
    BOOST_TEST(c->speed == 1.0f);
    BOOST_TEST(c->direction == 0.0f);
    c = mgr.AssignComponent<component::Move>(Id(2, 1), 0.0f, 0.0f);
    BOOST_TEST(c->speed == 0.0f);
    BOOST_TEST(c->direction == 0.0f);
    mgr.RemoveComponent<component::Move>(Id(2, 1));
    BOOST_TEST_REQUIRE(mgr.GetComponent<component::Move>(Id(2, 1)) == nullptr);
    BOOST_TEST_REQUIRE(mgr.HasComponent<component::Move>(Id(2, 1)) == false);

    mgr.AssignComponent<component::Move>(Id(2, 1));
    BOOST_TEST_REQUIRE(mgr.HasComponent<component::Move>(Id(2, 1)));
    mgr.DestroyEntity(Id(2, 1));
    BOOST_TEST_REQUIRE(mgr.GetComponent<component::Move>(Id(2, 1)) == nullptr);
    BOOST_TEST_REQUIRE(mgr.HasComponent<component::Move>(Id(2, 1)) == false);
}

BOOST_AUTO_TEST_CASE(test_component)
{
    using namespace srose::player;
    component::Move m;
    component::Transform t;
    BOOST_TEST_REQUIRE(m.GetFamily() != t.GetFamily());
}

BOOST_AUTO_TEST_CASE(test_ecs)
{
    using namespace srose::player;
}
