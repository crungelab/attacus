#include <gtest/gtest.h>

#include <attacus/core/component.h>

using namespace attacus;

namespace test_component {

static const char *s_dummy_events = ComponentKit::Intern("dummy_events");

class TestEvents : public Component
{
};

void set_k_dummy_events(Component &self, TestEvents *that)
{
    self.components_[s_dummy_events] = that;
}

TestEvents *k_dummy_events(Component &self)
{
    return static_cast<TestEvents *>(self.components_[s_dummy_events]);
}

class TestConsumer : public Component
{
};

// Demonstrate some basic assertions.
TEST(TestComponent, BasicAssertions)
{
    TestEvents events;
    TestConsumer consumer;

    set_k_dummy_events(consumer, &events);
    TestEvents* rEvents = k_dummy_events(events);
    
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

} // namespace test_component