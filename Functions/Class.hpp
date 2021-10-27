#pragma once

namespace IL2CPP
{
	enum m_eClassPropType
	{
		Unknown = 0,
		Field,			// Member of class that can be accessed directly by RVA
		Property,		// Member of class that can be accessed by calling function
		Method,			// Function of class
	};

	class Class
	{
	public:
		Unity::il2cppObject m_Object = { 0 };
		void* m_CachedPtr = nullptr;

		m_eClassPropType GetPropType(const char* m_pPropType)
		{
			Unity::il2cppFieldInfo* pField = reinterpret_cast<Unity::il2cppFieldInfo*(IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Data.Functions.m_pClassGetFieldFromName)(m_Object.m_pClass, m_pPropType);
			if (pField) return m_eClassPropType::Field;

			Unity::il2cppPropertyInfo* pProperty = reinterpret_cast<Unity::il2cppPropertyInfo*(IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Data.Functions.m_pClassGetPropertyFromName)(m_Object.m_pClass, m_pPropType);
			if (pProperty) return m_eClassPropType::Property;

			Unity::il2cppMethodInfo* pMethod = reinterpret_cast<Unity::il2cppMethodInfo*(IL2CPP_CALLING_CONVENTION)(void*, const char*, int)>(Data.Functions.m_pClassGetMethodFromName)(m_Object.m_pClass, m_pPropType, -1);
			if (pMethod) return m_eClassPropType::Method;

			return m_eClassPropType::Unknown;
		}
		
		// This function is used to get pointer to function of desired class
		void* GetMethodPointer(const char* m_MethodName, int iArgs = -1)
		{
			Unity::il2cppMethodInfo* pMethod = reinterpret_cast<Unity::il2cppMethodInfo*(IL2CPP_CALLING_CONVENTION)(void*, const char*, int)>(Data.Functions.m_pClassGetMethodFromName)(m_Object.m_pClass, m_MethodName, iArgs);
			if (!pMethod) return nullptr;

			return pMethod->m_pMethodPointer;
		}

		// Automatically resolve Member, no matter if its Field or Property (Might impact performance)
		template<typename T>
		T GetMemberValue(const char* m_pMemberName)
		{
			Unity::il2cppFieldInfo* pField = reinterpret_cast<Unity::il2cppFieldInfo*(IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Data.Functions.m_pClassGetFieldFromName)(m_Object.m_pClass, m_pMemberName);
			if (pField)
			{
				if (pField->m_iOffset >= 0) return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + pField->m_iOffset);
			}
			else
			{
				Unity::il2cppPropertyInfo* pProperty = reinterpret_cast<Unity::il2cppPropertyInfo*(IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Data.Functions.m_pClassGetPropertyFromName)(m_Object.m_pClass, m_pMemberName);
				if (pProperty && pProperty->m_pGet) return reinterpret_cast<T(UNITY_CALLING_CONVENTION)(void*)>(pProperty->m_pGet->m_pMethodPointer)(this);
			}

			T tDefault = { 0 };
			return tDefault;
		}

		template<typename T>
		void SetMemberValue(const char* m_pMemberName, T tValue)
		{
			Unity::il2cppFieldInfo* pField = reinterpret_cast<Unity::il2cppFieldInfo * (IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Data.Functions.m_pClassGetFieldFromName)(m_Object.m_pClass, m_pMemberName);
			if (pField)
			{
				if (pField->m_iOffset >= 0) *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + pField->m_iOffset) = tValue;
				return;
			}
			
			Unity::il2cppPropertyInfo* pProperty = reinterpret_cast<Unity::il2cppPropertyInfo * (IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Data.Functions.m_pClassGetPropertyFromName)(m_Object.m_pClass, m_pMemberName);
			if (pProperty && pProperty->m_pSet) return reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, T)>(pProperty->m_pSet->m_pMethodPointer)(this, tValue);
		}
	};
}