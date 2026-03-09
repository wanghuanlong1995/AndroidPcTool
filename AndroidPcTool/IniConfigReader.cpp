#include "pch.h"
#include "IniConfigReader.h"


// IniConfigReader实现
IniConfigReader::IniConfigReader(const CString& iniPath) : m_iniPath(iniPath) {}

bool IniConfigReader::GetSectionItems(const CString& section, std::vector<CString>& items)
{
    items.clear();
    TCHAR buffer[4096] = { 0 };

    // 获取节下所有键名
    DWORD result = ::GetPrivateProfileString(
        section, nullptr, nullptr, buffer, sizeof(buffer), m_iniPath);

    if (result == 0) return false;

    // 拆分缓冲区中的键名（以\0分隔）
    TCHAR* pItem = buffer;
    while (*pItem != '\0')
    {
        items.push_back(CString(pItem));
        pItem += _tcslen(pItem) + 1;
    }
    return true;
}

bool IniConfigReader::GetItemValues(const CString& section, const CString& itemName, std::vector<CString>& values)
{
    values.clear();
    TCHAR buffer[1024] = { 0 };

    // 获取配置项值
    ::GetPrivateProfileString(
        section, itemName, nullptr, buffer, sizeof(buffer), m_iniPath);

    CString valueStr(buffer);
    if (valueStr.IsEmpty()) return false;

    // 按|分割值列表
    int pos = 0;
    CString token;
    while ((pos = valueStr.Find(_T('|'))) != -1)
    {
        token = valueStr.Left(pos);
        values.push_back(token);
        valueStr = valueStr.Mid(pos + 1);
    }
    values.push_back(valueStr); // 添加最后一个值
    return true;
}