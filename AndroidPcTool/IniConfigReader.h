#pragma once
// INI配置读取工具类
#include <afxwin.h>
#include <vector>
#include <map>

class IniConfigReader
{
public:
    IniConfigReader(const CString& iniPath);

    // 获取指定节下的所有配置项
    bool GetSectionItems(const CString& section, std::vector<CString>& items);
    // 获取指定配置项的可选值列表
    bool GetItemValues(const CString& section, const CString& itemName, std::vector<CString>& values);

private:
    CString m_iniPath;
};


