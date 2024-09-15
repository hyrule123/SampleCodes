# SampleCodes

유용하게 써먹을 수 있는 짧은 샘플 코드 저장소

For unions, NRVO typically cannot be applied effectively, and here's why:

Why NRVO Doesn't Work Well with Unions:
Union Size and Structure: A union can contain multiple types, but only one of them can be active at a time. Since the types in a union can be of different sizes or have different constructors and destructors, the compiler can't reliably determine which object is being returned when dealing with union members. This ambiguity makes NRVO inapplicable because NRVO depends on clear knowledge of the return value's type at compile time.

Constructors/Destructors: In C++, when you return a non-union object, the compiler can typically determine how to construct and destroy the object in the correct location for NRVO. In the case of unions, this isn't straightforward because the compiler would need to understand which specific union member will be returned, and the union's object might need special handling (or a specific destructor).

Type Safety: NRVO relies on the return type and the destination type being compatible. Since a union can store different types and there's no guarantee which one will be active at a given time, applying NRVO would be risky and error-prone.