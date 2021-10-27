#pragma once

namespace Unity
{
	struct System_String : il2cppObject
	{
		int m_iLength;
		wchar_t m_wString[1024];

		std::string ToString()
		{
			std::string sRet(m_iLength + 1, '\0');
			WideCharToMultiByte(CP_UTF8, 0, m_wString, m_iLength, &sRet[0], m_iLength, 0, 0);
			return sRet;
		}
	};
}