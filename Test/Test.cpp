// Copyright(c) Minmin Gong. All rights reserved.
// Licensed under the MIT License.

#include <NanoRtti/NanoRtti.hpp>

#include <iostream>
#include <memory>

void Verify(bool v)
{
    if (!v)
    {
        throw;
    }
}

using namespace NanoRtti;

// Class hierarchy
//
// Base -- Derived_1_0 -- Derived_1_0_0
//                     |- Derived_1_0_1
//      |- Derived_1_1 -- Derived_1_1_0

class Base
{
public:
    NANO_RTTI_REGISTER_RUNTIME_CLASS();

    virtual ~Base(){};
};

class Derived_1_0 : public Base
{
public:
    NANO_RTTI_REGISTER_RUNTIME_CLASS(Base);
};

class Derived_1_1 : public Base
{
public:
    NANO_RTTI_REGISTER_RUNTIME_CLASS(Base);
};

class Derived_1_0_0 : public Derived_1_0
{
public:
    NANO_RTTI_REGISTER_RUNTIME_CLASS(Derived_1_0);

    int Func()
    {
        return 1;
    }
    int Func() const
    {
        return 2;
    }
};

class Derived_1_0_1 : public Derived_1_0
{
public:
    NANO_RTTI_REGISTER_RUNTIME_CLASS(Derived_1_0);
};

class Derived_1_1_0 : public Derived_1_1
{
public:
    NANO_RTTI_REGISTER_RUNTIME_CLASS(Derived_1_1);
};

void MultiLevelsDeriving()
{
    std::cout << "Testing multi-level deriving.\n";

    std::unique_ptr<Base> up = std::make_unique<Derived_1_0_0>();
    auto* p = up.get();

    Verify(DynCast<Derived_1_0_0*>(p) != nullptr);
    Verify(DynCast<Derived_1_0*>(p) != nullptr);
    Verify(DynCast<Base*>(p) != nullptr);
    Verify(DynCast<Derived_1_0_1*>(p) == nullptr);
    Verify(DynCast<Derived_1_1*>(p) == nullptr);
    Verify(DynCast<Derived_1_1_0*>(p) == nullptr);

    Verify(DynCast<Derived_1_0*>(static_cast<Base*>(nullptr)) == nullptr);

    auto* l1p = DynCast<Derived_1_0_0*>(p);
    Verify(l1p != nullptr);
    Verify(l1p->Func() == 1);

    const auto* l1cp = DynCast<const Derived_1_0_0*>(p);
    Verify(l1cp != nullptr);
    Verify(l1cp->Func() == 2);

    std::cout << "SUCCESS.\n";
}

class Derived_2_0_And_1 : public Derived_1_0, public Derived_1_1
{
public:
    NANO_RTTI_REGISTER_RUNTIME_CLASS(Derived_1_0, Derived_1_1);
};

void MultipleInheritance()
{
    std::cout << "Testing multiple inheritance.\n";

    std::unique_ptr<Derived_2_0_And_1> up = std::make_unique<Derived_2_0_And_1>();
    auto* p = up.get();

    Verify(DynCast<Derived_2_0_And_1*>(p) != nullptr);
    Verify(DynCast<Base*>(p) == nullptr);

    Verify(DynCast<Derived_1_0*>(p) != nullptr);
    Verify(DynCast<Derived_1_1*>(p) != nullptr);
    Verify(DynCast<Derived_1_0_0*>(p) == nullptr);
    Verify(DynCast<Derived_1_0_1*>(p) == nullptr);

    std::cout << "SUCCESS.\n";
}

class Derived_2_Virtual_0_And_1 : virtual public Derived_1_0, virtual public Derived_1_1
{
public:
    NANO_RTTI_REGISTER_RUNTIME_CLASS(Derived_1_0, Derived_1_1);
};

void VirtualInheritance()
{
    std::cout << "Testing virtual inheritance.\n";

    std::unique_ptr<Derived_2_Virtual_0_And_1> up = std::make_unique<Derived_2_Virtual_0_And_1>();
    auto* p = up.get();

    Verify(DynCast<Derived_2_Virtual_0_And_1*>(p) != nullptr);
    Verify(DynCast<Base*>(p) == nullptr);

    Verify(DynCast<Derived_1_0*>(p) != nullptr);
    Verify(DynCast<Derived_1_1*>(p) != nullptr);
    Verify(DynCast<Derived_1_0_0*>(p) == nullptr);
    Verify(DynCast<Derived_1_0_1*>(p) == nullptr);

    std::cout << "SUCCESS.\n";
}

int main()
{
    MultiLevelsDeriving();
    MultipleInheritance();
    VirtualInheritance();
}
