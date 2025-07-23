#include "pch.h"
#include "WideCharUtils.h"

/**
 * �жϸ������ֽ������Ƿ�ΪGBK����
 * @param data ָ���ֽ����е�ָ��
 * @param length �ֽ����еĳ���
 * @return �����GBK�����򷵻�TRUE�����򷵻�FALSE
 */
BOOL IsGBK(const BYTE* data, int length) {
    // ���ֽ����г���ת��Ϊ���ַ���CP_ACP��ʾʹ��ANSI����ҳ��ͨ����ӦGBK��
    int wideLen = MultiByteToWideChar(CP_ACP, 0, (LPCCH)data, length, NULL, 0);
    return (wideLen > 0);
}

/**
 * �жϸ������ֽ������Ƿ�ΪUTF-8����
 * @param data ָ���ֽ����е�ָ��
 * @param length �ֽ����еĳ���
 * @return �����UTF-8�����򷵻�TRUE�����򷵻�FALSE
 */
BOOL IsUTF8(const BYTE* data, int length) {
    // ���ֽ����г���ת��Ϊ���ַ���CP_UTF8��ʾʹ��UTF-8����ҳ
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)data, length, NULL, 0);
    return (wideLen > 0);
}

/**
 * ��GBK������ַ���ת��ΪUTF-8����
 * @param gbkStr GBK������ַ���
 * @return ת�����UTF-8�����ַ���
 */
std::string ConvertGBKToUTF8(const std::string& gbkStr) {
    // Step 1: GBK �� Unicode (UTF-16)
    // ���Ƚ�GBK������ַ���ת��ΪUnicode��UTF-16����936��GBK����ҳ�ı�ʶ
    int wlen = MultiByteToWideChar(936, 0, gbkStr.c_str(), -1, nullptr, 0);
    wchar_t* wbuf = new wchar_t[wlen];
    MultiByteToWideChar(936, 0, gbkStr.c_str(), -1, wbuf, wlen);

    // Step 2: Unicode �� UTF-8
    // Ȼ��Unicode��UTF-16��������ַ���ת��ΪUTF-8����
    int ulen = WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, nullptr, 0, nullptr, nullptr);
    char* ubuf = new char[ulen];
    WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, ubuf, ulen, nullptr, nullptr);

    // ���첢����ת�����UTF-8�����ַ���
    std::string utf8Str(ubuf);
    // �ͷ�ת��������ʹ�õ��ڴ�
    delete[] wbuf;
    delete[] ubuf;
    return utf8Str;
}