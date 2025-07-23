#pragma once
#include <string>

/**
 * �жϸ����������Ƿ�ΪGBK����
 * @param data ָ���������ݵ�ָ��
 * @param length ��������ݵĳ���
 * @return ���������GBK���룬�򷵻�TRUE�����򷵻�FALSE
 */
BOOL IsGBK(const BYTE* data, int length);

/**
 * �жϸ����������Ƿ�ΪUTF-8����
 * @param data ָ���������ݵ�ָ��
 * @param length ��������ݵĳ���
 * @return ���������UTF-8���룬�򷵻�TRUE�����򷵻�FALSE
 */
BOOL IsUTF8(const BYTE* data, int length);

/**
 * ��GBK������ַ���ת��ΪUTF-8����
 * @param gbkStr GBK������ַ���
 * @return ת�����UTF-8�����ַ���
 */
std::string ConvertGBKToUTF8(const std::string& gbkStr);