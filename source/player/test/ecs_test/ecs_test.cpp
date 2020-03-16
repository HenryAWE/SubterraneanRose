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
    BOOST_TEST_REQUIRE(mgr.GetEntityMask(Id(2, 1)).test(component::Move::GetFamily()));
    BOOST_TEST_REQUIRE((mgr.GetComponentMask<component::Move>() & mgr.GetEntityMask(Id(2, 1))).any());
    std::bitset<SROSE_PLAYER_ECS_MAXCOMPONENTS> bs;
    bs.set(component::Move::GetFamily());
    bs.set(component::Transform::GetFamily());
    BOOST_TEST_REQUIRE((mgr.GetComponentMask<component::Move>() & mgr.GetEntityMask(Id(2, 1))) != bs);
    BOOST_TEST_REQUIRE((mgr.GetComponentMask<component::Move, component::Transform>() & mgr.GetEntityMask(Id(2, 1))) != bs);

    BOOST_TEST_REQUIRE(mgr.HasComponent<component::Move>(Id(1, 2)) == false);
    BOOST_TEST_REQUIRE(mgr.EntitiesWithComponent<component::Move>().mask.test(component::Move::GetFamily()));
    for(const auto& i : mgr.EntitiesWithComponent<component::Move>())
    {
        BOOST_TEST_REQUIRE((mgr.GetComponentMask<component::Move>() & mgr.GetEntityMask(i.GetId())).any());
        BOOST_TEST_REQUIRE(mgr.HasComponent<component::Move>(i.GetId()));
        c = std::get<0>(mgr.GetComponentTuple<component::Move>(i.GetId()));
        BOOST_TEST_REQUIRE(c != nullptr);
        BOOST_TEST(c->speed == 0.0f);
        BOOST_TEST(c->direction == 0.0f);
    }
    for(const auto& i : mgr.EntitiesWithComponent<component::Move, component::Transform>())
    {
        BOOST_FAIL("Unreachable");
    }
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

    entity::EntityManager mgr;
    auto i = (mgr.GetComponentMask<component::Move>().test(component::Move::GetFamily()));
    BOOST_TEST_REQUIRE(mgr.GetComponentMask<>().none());
    std::bitset<SROSE_PLAYER_ECS_MAXCOMPONENTS> bs;
    bs.set(component::Move::GetFamily());
    bs.set(component::Transform::GetFamily());
    BOOST_TEST_REQUIRE((mgr.GetComponentMask<component::Move, component::Transform>() == bs));
    BOOST_TEST_REQUIRE((mgr.EntitiesWithComponent<component::Move, component::Transform>().mask == bs));
}

#include <sr/player/system/system.hpp>
#include <sr/player/system/move.hpp>


BOOST_AUTO_TEST_CASE(test_system)
{
    using namespace srose::player;
    entity::EntityManager emgr;
    system::SystemManager mgr(&emgr);

    mgr.Add<system::Move, std::tuple<component::Move, component::Transform>>();
    
    struct TestComponent : public component::Component<TestComponent>
    {
        int value;
        TestComponent(int value_ = 0)
            : Component::Component(), value(value_) {}
    };
    struct TestSystem
    {
        void Update(entity::Entity e, TestComponent& t)
        {
            t.value += 1;
        }
    };

    entity::Entity::Id id = emgr.CreateEntity().GetId();
    emgr.AssignComponent<TestComponent>(id, 0);
    BOOST_TEST_REQUIRE(emgr.GetComponent<TestComponent>(id)->value == 0);
    mgr.Add<TestSystem, std::tuple<TestComponent>>();
    BOOST_TEST_REQUIRE(mgr.Has<TestSystem>());
    mgr.UpdateAll();
    BOOST_TEST_REQUIRE(emgr.GetComponent<TestComponent>(id)->value == 1);

    struct TestComponent2 : public component::Component<TestComponent2>
    {
        std::string value;
        TestComponent2(std::string value_ = "")
            : value(value_) {}
    };
    struct TestSystem2
    {
        void Update(entity::Entity e, TestComponent& t1, TestComponent2& t2)
        {
            t1.value += 1;
            t2.value = "assigned";
        }
    };

    BOOST_TEST_REQUIRE(emgr.GetComponent<TestComponent>(id)->value == 1);
    BOOST_TEST_REQUIRE(TestComponent::GetFamily() != TestComponent2::GetFamily());
    auto* c = emgr.GetComponent<TestComponent>(id);
    BOOST_TEST_REQUIRE(c != nullptr);
    emgr.AssignComponent<TestComponent2>(id, "unassigned");
    BOOST_TEST(emgr.GetComponent<TestComponent2>(id)->value == "unassigned");
    BOOST_TEST_REQUIRE(emgr.HasComponent<TestComponent>(id));
    BOOST_TEST(emgr.GetComponent<TestComponent>(id)->value == 1);
    mgr.UpdateAll();
    BOOST_TEST(c->value == 2);

    mgr.Add<TestSystem2, std::tuple<TestComponent, TestComponent2>>();
    mgr.UpdateAll();
    BOOST_TEST(emgr.GetComponent<TestComponent2>(id)->value == "assigned");
    BOOST_TEST(c->value == 4);

    mgr.Remove<TestSystem>();
    BOOST_TEST(mgr.Has<TestSystem>() == false);}

BOOST_AUTO_TEST_CASE(test_ecs)
{
    using namespace srose::player;
}
