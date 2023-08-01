#include <iostream>
#include <typeinfo>
#include <typeindex>

#include "json-cpp/json.h"

struct Vector3
{
    float V[3];
    Vector3() : V{} {}
    Vector3(float _x, float _y, float _z) : V{ _x, _y, _z } {}
};

struct Matrix
{
    float M[16];
};

int main() 
{
    {
        Vector3 TestVec = Vector3(1.f, 2.f, 3.f);

        std::string Vec2Str((const char*)&TestVec, sizeof(TestVec));

        Json::Value jVal;
        jVal["Test"] = Vec2Str;

        std::string Str2Vec = jVal["Test"].asString();
        Vector3 ReturnVec{};
        memcpy(&ReturnVec, Str2Vec.data(), sizeof(Vector3));
    }

    {
        Matrix TestMat{};
        for (int i = 0; i < 16; ++i)
        {
            TestMat.M[i] = (float)i;
        }

        std::string Mat2Str((const char*)&TestMat, sizeof(TestMat));

        Json::Value jVal;
        jVal["Test"] = Mat2Str;

        std::string Str2Mat = jVal["Test"].as<std::string>();
        Matrix ReturnVec{};
        memcpy(&ReturnVec, Str2Mat.data(), sizeof(Matrix));


        size_t MatSize = sizeof(Matrix);
        size_t MatToStrSize = Str2Mat.size();

        int a = 0;
    }



    return 0;
}