#pragma once

#include "type_traits_Ex.h"
#include "define_Enum.h"

#include <filesystem>
#include <iostream>
#include <cstdint>
#include <vector>


	class Serializer
	{
	protected:
		Serializer() {};
		virtual ~Serializer() {};
	public:
		virtual eResult SaveFile(std::filesystem::path const& _fullPath) = 0;
		virtual eResult LoadFile(std::filesystem::path const& _fullPath) = 0;
	};


	//다중상속 용도
	template <typename T> requires std::is_base_of_v<Serializer, T>
	class Serializable
	{
	protected:
		Serializable() {};
		virtual ~Serializable() {};

	public:
		inline eResult SaveFile(std::filesystem::path const& _fullPath);
		inline eResult LoadFile(std::filesystem::path const& _fullPath);

		virtual eResult Serialize(T& _ser) = 0;
		virtual eResult DeSerialize(T& _ser) = 0;
	};


	template<typename T> requires std::is_base_of_v<Serializer, T>
	inline eResult Serializable<T>::SaveFile(std::filesystem::path const& _fullPath)
	{
		T ser{};

		eResult result = Serialize(ser);
		if (eResultFail(result))
		{
			return eResult::Fail;
		}

		return ser.SaveFile(_fullPath);
	}

	template<typename T> requires std::is_base_of_v<Serializer, T>
	inline eResult Serializable<T>::LoadFile(std::filesystem::path const& _fullPath)
	{
		T ser{};
		eResult result = ser.LoadFile(_fullPath);
		if (eResultFail(result))
		{
			return eResult::Fail;
		}

		return DeSerialize(ser);
	}



