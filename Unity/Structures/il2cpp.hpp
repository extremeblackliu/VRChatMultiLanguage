#pragma once

namespace Unity
{
	struct il2cppClass
	{
		void* m_pImage;
		void* m_pGC;
		const char* m_pName;
		const char* m_pNamespace;
		void* m_pValue;
		void* m_pArgs;
		il2cppClass* m_pElementClass;
		il2cppClass* m_pCastClass;
		il2cppClass* m_pDeclareClass;
		il2cppClass* m_pParentClass;
		void* m_pGenericClass;
		void* m_pTypeDefinition;
		void* m_pInteropData;
		void* m_pFields;
		void* m_pEvents;
		void* m_pProperties;
		void** m_pMethods;
		il2cppClass** m_pNestedTypes;
		il2cppClass** m_ImplementedInterfaces;
		void* m_pInterfaceOffsets;
		void* m_pStaticFields;
		void* m_pRGCTX;
	};

	struct il2cppObject
	{
		il2cppClass* m_pClass = nullptr;
		void* m_pMonitor = nullptr;
	};

	struct il2cppFieldInfo
	{
		const char* m_pName;
		void* m_pType;
		il2cppClass* m_pParentClass;
		int m_iOffset;
		int m_iAttributeIndex;
		unsigned int m_uToken;
	};

	struct il2cppMethodInfo
	{
		void* m_pMethodPointer;
		void* m_pInvokerMethod;
		const char* m_pName;
		il2cppClass* m_pClass;
		void* m_pReturnType;
		void* m_pParameters;

		union
		{
			void* m_pRGCTX;
			void* m_pMethodDefinition;
		};

		union
		{
			void* m_pGenericMethod;
			void* m_pGenericContainer;
		};

		unsigned int m_uToken;
		unsigned short m_uFlags;
		unsigned short m_uFlags2;
		unsigned short m_uSlot;
		unsigned char m_uArgsCount;
		unsigned char m_uGeneric : 1;
		unsigned char m_uInflated : 1;
		unsigned char m_uWrapperType : 1;
		unsigned char m_uMarshaledFromNative : 1;
	};

	struct il2cppPropertyInfo
	{
		il2cppClass* m_pParentClass;
		const char* m_pName;
		il2cppMethodInfo* m_pGet;
		il2cppMethodInfo* m_pSet;
		unsigned int m_uAttributes;
		unsigned int m_uToken;
	};

	struct il2cppType
	{
		union
		{
			void* m_pDummy;
			unsigned int m_uClassIndex;
			il2cppType* m_pType;
			void* m_pArray;
			unsigned int m_uGenericParameterIndex;
			void* m_pGenericClass;
		};
		unsigned int m_uAttributes : 16;
		unsigned int m_uType : 8;
		unsigned int m_uMods : 6;
		unsigned int m_uByref : 1;
		unsigned int m_uPinned : 1;
	};

	struct il2cppArrayBounds
	{
		uintptr_t m_uLength;
		int m_iLowerBound;
	};

	template<typename T>
	struct il2cppArray
	{
		il2cppObject m_Object;
		il2cppArrayBounds* m_pBounds;
		uintptr_t m_uMaxLength;
		T* m_pArray[65536];
	};
}