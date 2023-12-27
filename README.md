# NanoRtti

[![License](https://img.shields.io/github/license/mashape/apistatus.svg)](LICENSE)
[![Build Status](https://gongminmin.visualstudio.com/NanoRtti/_apis/build/status%2Fgongminmin.NanoRtti?branchName=main)](https://gongminmin.visualstudio.com/NanoRtti/_build/latest?definitionId=7&branchName=main)

The NanoRtti library provides customized run-time type information (RTTI) support. It can provide the RTTI ability on specific classes when the compiler one is turned off. It doesn't have 3rd party dependencies.

## Example

Add the NANO_RTTI_REGISTER_RUNTIME_CLASS to the public section of classes that need RTTI. Its parameters are base classes. Here is an example.

        class Base
        {
        public:
            NANO_RTTI_REGISTER_RUNTIME_CLASS();
        
            virtual ~Base(){};
        };

        class Derived : public Base
        {
        public:
            NANO_RTTI_REGISTER_RUNTIME_CLASS(Base);
        };

Now you can use DynCast as a drop-in replacement of dynamic_cast, or TypeId<T>() to get the type info of a type.

        Base* pb = new Derived;
        Derived* pd = DynCast<Derived*>(pb);
        ...

        TypeInfo ty = TypeId<Derived>();
        ...

## License

NanoRtti is distributed under the terms of MIT License. See [LICENSE](LICENSE) for details.
