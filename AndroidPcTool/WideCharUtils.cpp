#include "pch.h"
#include "WideCharUtils.h"

/**
 * 判断给定的字节序列是否为GBK编码
 * @param data 指向字节序列的指针
 * @param length 字节序列的长度
 * @return 如果是GBK编码则返回TRUE，否则返回FALSE
 */
BOOL IsGBK(const BYTE* data, int length) {
    // 将字节序列尝试转换为宽字符，CP_ACP表示使用ANSI代码页（通常对应GBK）
    int wideLen = MultiByteToWideChar(CP_ACP, 0, (LPCCH)data, length, NULL, 0);
    return (wideLen > 0);
}

/**
 * 判断给定的字节序列是否为UTF-8编码
 * @param data 指向字节序列的指针
 * @param length 字节序列的长度
 * @return 如果是UTF-8编码则返回TRUE，否则返回FALSE
 */
BOOL IsUTF8(const BYTE* data, int length) {
    // 将字节序列尝试转换为宽字符，CP_UTF8表示使用UTF-8代码页
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)data, length, NULL, 0);
    return (wideLen > 0);
}

/**
 * 将GBK编码的字符串转换为UTF-8编码
 * @param gbkStr GBK编码的字符串
 * @return 转换后的UTF-8编码字符串
 */
std::string ConvertGBKToUTF8(const std::string& gbkStr) {
    // Step 1: GBK → Unicode (UTF-16)
    // 首先将GBK编码的字符串转换为Unicode（UTF-16），936是GBK代码页的标识
    int wlen = MultiByteToWideChar(936, 0, gbkStr.c_str(), -1, nullptr, 0);
    wchar_t* wbuf = new wchar_t[wlen];
    MultiByteToWideChar(936, 0, gbkStr.c_str(), -1, wbuf, wlen);

    // Step 2: Unicode → UTF-8
    // 然后将Unicode（UTF-16）编码的字符串转换为UTF-8编码
    int ulen = WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, nullptr, 0, nullptr, nullptr);
    char* ubuf = new char[ulen];
    WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, ubuf, ulen, nullptr, nullptr);

    // 构造并返回转换后的UTF-8编码字符串
    std::string utf8Str(ubuf);
    // 释放转换过程中使用的内存
    delete[] wbuf;
    delete[] ubuf;
    return utf8Str;
}