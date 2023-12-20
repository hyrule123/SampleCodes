#include <type_traits>
#include <vector>
#include <memory>

enum class eComponentCategory
{
    foo,
    bar
};


//class for type classification
template <class BaseComponentType, eComponentCategory _category>
class Component
{
public:
    Component() {};
    virtual ~Component() {};

    static constexpr eComponentCategory category = _category;

    template <class CompareType>
    static constexpr bool IsBaseComponentType()
    {
        return std::is_same_v<BaseComponentType, CompareType>;
    }
};

class FooComponent
    : public Component<FooComponent, eComponentCategory::foo>
{
public:
    FooComponent() {}
    virtual ~FooComponent() {}
};


class ComponentHolder
{
public:
    ComponentHolder() {};
    virtual ~ComponentHolder() {};

    template <class ComponentType>
    ComponentType* GetComponent()
    {
        ComponentType* retCom = nullptr;

        //type check
        //if constexpr (ComponentType::IsBaseComponentType<ComponentType>())
//compile time error  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        if constexpr (ComponentType::template IsBaseComponentType<ComponentType>())
        {
            //do Something
        }

        return retCom;
    }

};


int main()
{
    ComponentHolder holder{};

    holder.GetComponent<FooComponent>();


    return 0;
}