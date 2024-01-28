#pragma once


	enum class eResult
	{
		//���� = ����
		//�� �Ʒ��� ���� ���� ������ ����
		Success = -1,

		Fail_NotImplemented = 0,

		//��� = ����
		Fail,

		Fail_Open,

		Fail_Create,

		Fail_Write,
		Fail_Read,

		Fail_Nullptr,

		Fail_InValid,

		Fail_Empty
	};
	inline bool eResultSuccess(eResult _result) { return ((int)_result < 0); }
	inline bool eResultFail(eResult _result) { return ((int)_result >= 0); }

	enum class eLayerType
	{
		None = -1,
		Com_Camera,
		Grid,
		Monster,
		Player,

		tParticle,
		PostProcess,
		UI,

		END
	};

	enum class eProjectionType
	{
		None = -1,
		Perspective,
		Orthographic,
	};

	enum class eColliderType
	{
		None,
		Rect,
		Circle,
		Box,
		Sphere,
		END,
	};

	enum class eLightType
	{
		Directional,
		Point,
		Spot,
		END,
	};

	enum class eDimensionType
	{
		NOT_SET,
		_2D,
		_3D
	};

