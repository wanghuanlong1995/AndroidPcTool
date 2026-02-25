#pragma once
#include <string>

/**
 * 判断给定的数据是否为GBK编码
 * @param data 指向待检测数据的指针
 * @param length 待检测数据的长度
 * @return 如果数据是GBK编码，则返回TRUE，否则返回FALSE
 */
BOOL IsGBK(const BYTE* data, int length);

/**
 * 判断给定的数据是否为UTF-8编码
 * @param data 指向待检测数据的指针
 * @param length 待检测数据的长度
 * @return 如果数据是UTF-8编码，则返回TRUE，否则返回FALSE
 */
BOOL IsUTF8(const BYTE* data, int length);

/**
 * 将GBK编码的字符串转换为UTF-8编码
 * @param gbkStr GBK编码的字符串
 * @return 转换后的UTF-8编码字符串
 */
std::string ConvertGBKToUTF8(const std::string& gbkStr);