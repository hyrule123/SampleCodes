#include <iostream>

//전방 선언
template <typename T>
unsigned int getTypeID(const T* typeTag = nullptr);

// Custom type tag class
class TypeTag 
{
public:
    TypeTag() {};
    virtual ~TypeTag() {};


    virtual unsigned int GetType() { return getTypeID<TypeTag>(); }
#define TYPE_TAG(_ty) virtual unsigned int GetType() { return getTypeID<_ty>(); }

private:
    // Generate a unique type ID for each tag
    static unsigned int getNextTypeId() 
    {
        static unsigned int typeId = 0;
        return ++typeId;
    }

    template <typename T>
    friend unsigned int getTypeID(const T* typeTag);
};

// Function to get type ID based on type tag
template <typename T>
unsigned int getTypeID(const T* typeTag)
{
    static unsigned int typeId = TypeTag::getNextTypeId();
    return typeId;
}

class DerivedA : public TypeTag
{
public:
    DerivedA() {};
    virtual ~DerivedA() {};

    TYPE_TAG(DerivedA);
};

class DerivedB : public TypeTag
{
public:
    DerivedB() {};
    virtual ~DerivedB() {};

    TYPE_TAG(DerivedB);
};

class DerivedC : public DerivedB
{
public:
    DerivedC() {};
    virtual ~DerivedC() {};

    TYPE_TAG(DerivedC);
};


// Type-specific tags
struct IntTypeTag : public TypeTag {};
struct DoubleTypeTag : public TypeTag {};
struct StringTypeTag : public TypeTag {};
struct TripleTypeTag : public DoubleTypeTag {};




int main() {
    // Get type IDs
    IntTypeTag itg{};
    unsigned int intTypeId0 = getTypeID(&itg);
    unsigned int intTypeId1 = getTypeID<IntTypeTag>();

    DoubleTypeTag dtg0{};
    unsigned int doubleTypeId0 = getTypeID(&dtg0);

    DoubleTypeTag dtg1{};
    unsigned int doubleTypeId1 = getTypeID(&dtg1);

    unsigned int doubleTypeId2 = getTypeID<DoubleTypeTag>();

    StringTypeTag stg0{};
    unsigned int stringTypeId0 = getTypeID(&stg0);
    unsigned int stringTypeId1 = getTypeID<StringTypeTag>();
    unsigned int stringTypeId2 = getTypeID(&stg0);


    unsigned int TripleTypeId0 = getTypeID<TripleTypeTag>();
    unsigned int TripleTypeId1 = getTypeID<TripleTypeTag>();


    TripleTypeTag tg{};
    unsigned int ttI = getTypeID(&tg);

    DerivedA a{};
    std::cout << a.GetType() << std::endl;
    DerivedB b{};
    std::cout << b.GetType() << std::endl;
    DerivedC c{};
    std::cout << c.GetType() << std::endl;


    return 0;
}
