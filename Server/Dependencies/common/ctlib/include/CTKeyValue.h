/*************************************************
Author: will
Date: 4/18/2013
Description:
Notes: 
*************************************************/

#ifndef __CTKeyValue_h__
#define __CTKeyValue_h__

#include "CTSizeString.h"
#include "CTArray.h"
#include "CTCodeConvert.h"

namespace CTLib
{

class CTKeyValueData : public CTCodeObject
{
public:
    enum 
    {
        MAX_KEY_VALUE_LEN = 40960,
        MAX_KEY_VALUE_COUNT = 30,  //KEY-VALUE对的最大数量
        MAX_KEY_LEN = 50,          //KEY的最大长度
        MAX_VALUE_LEN = 2048,      //VALUE的最大长度
    };

    CTKeyValueData()
    {
        Clear();
    }

    void Clear()
    {   
        _szKey.Set("");
        _szValue.Set("");
        _ucNextValueIndex = 0;
        _ucHasOtherData = 0;
        _ucIsHeadData = 1;
    }

    static const char VALUE_DEL_CHAR = '|'; 
       
    CTSizeString<MAX_KEY_LEN> _szKey;    
    CTSizeString<MAX_VALUE_LEN> _szValue;
    unsigned char _ucNextValueIndex;        //下个组合VALUE的下标(0表示没有后续Value)
    unsigned char _ucHasOtherData;          //是否还有其他数据
    unsigned char _ucIsHeadData;            //是否是头部数据
};

class CTKeyValue : public CTCodeObject
{
public:
    CTKeyValue()
    {

    }

    void Clear()
    {
        _astKeyValueData.Clear();
    }

    //从HTTP请求参数串中解析
    int ParseFromHttpParam(const char* szHttpParam, int iParaLen, bool bIsJoin = true)
    {
        _astKeyValueData.Clear();

        int i = 0;
        int iLen = 0;
        bool bNowFindKey = true;
        CTKeyValueData stKeyValueData;
 
        //要求有key有value
        while (i < iParaLen)
        {
            if (bNowFindKey) //找键
            {
                while(i < iParaLen &&
                      ((char)szHttpParam[i] == '&' ||
                       (char)szHttpParam[i] == '=' ||
                       (char)szHttpParam[i] == ' ' ||
                       (char)szHttpParam[i] == '\r' ||
                       (char)szHttpParam[i] == '\n' ||
                       (char)szHttpParam[i] == '?')){++i;}

                if (i >= iParaLen){break;}

                stKeyValueData._szKey.Set("");
                const char* pQu = strchr(szHttpParam + i, '?');
                const char* pEq = strchr(szHttpParam + i, '=');
                if (pQu && pEq && pEq - pQu > 0)
                {
                    i = pQu - szHttpParam;
                    bNowFindKey = true;
                }
                else if (pEq) //找到
                {
                    iLen = pEq - szHttpParam - i;
                    if (iLen > 0 && iLen < CTKeyValueData::MAX_KEY_LEN)
                    {
                        memcpy(stKeyValueData._szKey.GetBuffer(), szHttpParam + i, iLen);
                        stKeyValueData._szKey.SetDataLen(iLen);
                    }

                    i += iLen;
                    bNowFindKey = false; //找key只能找一次应转为找值
                }
                else
                {
                    break;
                }
            }

            if (!bNowFindKey) //找值
            {
                while(i < iParaLen &&
                      ((char)szHttpParam[i] == '=' ||
                       (char)szHttpParam[i] == ' ' ||
                       (char)szHttpParam[i] == '\r' ||
                       (char)szHttpParam[i] == '\n')){++i;}

                if (i >= iParaLen){break;}

                stKeyValueData._szValue.Set("");
                const char* pQu = strchr(szHttpParam + i, '?');
                const char* pAnd = strchr(szHttpParam + i, '&');
                if (pQu && pAnd && pAnd - pQu > 0)
                {
                    i += pAnd - pQu + 1;
                }
                else if (pAnd) //找到
                {
                    iLen = pAnd - szHttpParam - i;
                    if (iLen > 0 && iLen < CTKeyValueData::MAX_VALUE_LEN)
                    {
                        memcpy(stKeyValueData._szValue.GetBuffer(), szHttpParam + i, iLen);
                        stKeyValueData._szValue.SetDataLen(iLen);
                    }

                    i += iLen;
                }
                else //达到最后
                {
                    iLen = iParaLen - i;
                    if (iLen > 0 && iLen < CTKeyValueData::MAX_VALUE_LEN)
                    {
                        memcpy(stKeyValueData._szValue.GetBuffer(), szHttpParam + i, iLen);
                        stKeyValueData._szValue.SetDataLen(iLen);
                    }

                    i += iLen;
                }

                bNowFindKey = true; //找值只能找一次应重新找key
            }

            if (bNowFindKey && stKeyValueData._szKey.GetDataLen() > 0 &&
                stKeyValueData._szValue.GetDataLen() > 0)
            {
                if (bIsJoin)
                {
                    SetValue(stKeyValueData._szKey(), stKeyValueData._szValue());
                }
                else
                {
                    SetValueWithoutJoin(stKeyValueData._szKey(), stKeyValueData._szValue());
                }
            }
        }

        return 0;
    }

    //从json转换
    int ParseFromJson(const char* szJson)
    {
        int i = 0;
        _astKeyValueData.Clear();

        //检查
        if (NULL == szJson)
        {
            return -1;
        }

        const char* p1 = strchr(szJson, '{');
        if (NULL == p1)
        {
            return -2;
        }
        i++;

        const char* p2 = strchr(p1, '}');
        if (NULL == p2)
        {
            return -3;
        }

        //去掉大括号
        int len = p2 - p1;
        if (len >= CTKeyValueData::MAX_KEY_VALUE_LEN || len < 0) //避免溢出
        {
            return -4;
        }

        if (len == 0)
        {
            return 0;
        }

        bool bIsInQuot = false;     //是否在引号内
        bool bIsInSquareBrackets = false; //是否在方括号内
        bool bIsKey = true;

        CTKeyValueData stKeyValueData;

        for (;; ++i)
        {
            if (_astKeyValueData.Size() >= _astKeyValueData.GetCount())
            {
                return -8;
            }

            else if (p1[i] == '"')
            {
                bIsInQuot = !bIsInQuot;
                continue;
            }
            else if (bIsInQuot || ((p1[i] >= '0' && p1[i] <= '9') || p1[i] == '-'))
            {
                if (bIsKey)
                {
                    if (stKeyValueData._szKey.GetDataLen() >= stKeyValueData._szKey.GetBufferLen())
                    {
                        return -9;
                    }

                    stKeyValueData._szKey.Cat("%c", p1[i]);
                }
                else
                {
                    if (stKeyValueData._szValue.GetDataLen() >= stKeyValueData._szValue.GetBufferLen())
                    {
                        return -10;
                    }

                    stKeyValueData._szValue.Cat("%c", p1[i]);
                }

                continue;
            }
            else if (p1[i] == ':')
            {
                if (!bIsKey)
                {
                    return -5;
                }

                bIsKey = false;
                continue;
            }
            else if (p1[i] == ',')
            {
                if (!bIsInSquareBrackets)
                {
                    if (bIsKey)
                    {
                        return -6;
                    }

                    SetValue(stKeyValueData._szKey(), stKeyValueData._szValue());
                    stKeyValueData.Clear();

                    bIsKey = true;
                }
                else
                {
                    SetValue(stKeyValueData._szKey(), stKeyValueData._szValue());
                    stKeyValueData._szValue.SetDataLen(0);
                }
                
                continue;
            }
            else if (p1[i] == '[')
            {
                if (bIsInSquareBrackets)
                {
                    return -11;
                }

                bIsInSquareBrackets = true;
            }
            else if (p1[i] == ']')
            {
                if (!bIsInSquareBrackets)
                {
                    return -12;
                }

                SetValue(stKeyValueData._szKey(), stKeyValueData._szValue());
                stKeyValueData.Clear();

                bIsInSquareBrackets = false;
            }
            else if (p1[i] == '{')
            {               
                CTKeyValue stKeyValue;
                int iDealLen = stKeyValue.ParseFromJson(p1 + i);
                if (iDealLen < 0)
                {
                    return iDealLen;
                }

                i += iDealLen;

                for (int j = 0; j < stKeyValue._astKeyValueData.Size(); ++j)
                {
                    CTKeyValueData stSubKeyValueData;
                    stSubKeyValueData._szKey.Set("%s_%s", stKeyValueData._szKey(), stKeyValue._astKeyValueData[j]._szKey());
                    stSubKeyValueData._szValue.Set("%s", stKeyValue._astKeyValueData[j]._szValue());

                    SetValue(stSubKeyValueData._szKey(), stSubKeyValueData._szValue());
                }
                continue;
            }
            else if (p1[i] == '}')
            {
                i++;
                break;
            }
        }

        if (stKeyValueData._szKey.GetDataLen() > 0)
        {
            SetValue(stKeyValueData._szKey(), stKeyValueData._szValue());
            stKeyValueData.Clear();
        }

        if (bIsInQuot || bIsKey)
        {
            return -7;
        }
    
        return i;
    }

    int SetValueWithoutJoin(const char* szInputKey, const char* szValue)
    {
        int iKeyLen = strlen(szInputKey);
        int iValueLen = strlen(szValue);

        if (szInputKey == NULL || szValue == NULL || iKeyLen == 0 || iValueLen == 0)
        {
            return -1;
        }

        const char* pKeyB = strchr(szInputKey, ' ');
        const char* pValueB = strchr(szValue, ' ');
        const char* pKeyR = strstr(szInputKey, "\r\n");
        const char* pValueR = strstr(szValue, "\r\n");

        if (pKeyB && pKeyB - szInputKey < iKeyLen) {iKeyLen = pKeyB - szInputKey;}
        if (pKeyR && pKeyR - szInputKey < iKeyLen) {iKeyLen = pKeyR - szInputKey;}
        if (pValueB && pValueB - szValue < iValueLen) {iValueLen = pValueB - szValue;}
        if (pValueR && pValueR - szValue < iValueLen) {iValueLen = pValueR - szValue;}

        if (iKeyLen <= 0 || iValueLen <= 0)
        {
            return -1;
        }

        CTKeyValueData stKeyValueData;
        memcpy(stKeyValueData._szKey.GetBuffer(), szInputKey, iKeyLen);
        memcpy(stKeyValueData._szValue.GetBuffer(), szValue, iValueLen);
        stKeyValueData._szKey.SetDataLen(iKeyLen);
        stKeyValueData._szValue.SetDataLen(iValueLen);

        for (int i = 0; i < _astKeyValueData.Size(); ++i)   //效率低
        {
            if (iKeyLen == _astKeyValueData[i]._szKey.GetDataLen()
                && 0 == strcmp(stKeyValueData._szKey(), _astKeyValueData[i]._szKey())) //寻找已经存在的KEY
            {
                _astKeyValueData[i]._szValue.Set("%s", stKeyValueData._szValue());
                return 0;
            }
        }

        if (_astKeyValueData.Size() >= _astKeyValueData.GetCount())
        {
            return -3;
        }

        _astKeyValueData.AddOneItem(stKeyValueData);
        
        return 0;
    }

    int SetValue(const char* szInputKey, const char* szValue, unsigned char ucNextValueIndex = 0, bool bHasSaveDel = true)
    {
        int iKeyLen = strlen(szInputKey);
        int iValueLen = strlen(szValue);

        if (szInputKey == NULL || szValue == NULL || iKeyLen == 0 || iValueLen == 0)
        {
            return -1;
        }

        if (iKeyLen >= CTKeyValueData::MAX_KEY_LEN - 3) //后3字节是预留扩展的
        {
            return -2;
        }

        if (strchr(szValue, CTKeyValueData::VALUE_DEL_CHAR) != NULL) //预留分隔符，禁止重复
        {
            return -4;
        }

        const char * szOrgKey = szInputKey;
        CTSizeString<CTKeyValueData::MAX_KEY_LEN> szDestKey;
        unsigned char ucIsHeadData = 1;     //是否是头部数据
        if (ucNextValueIndex == 0)
        {
            szDestKey.Set("%s", szInputKey);
            ucIsHeadData = 1; 
        }
        else
        {
            szDestKey.Set("%s_%d", szInputKey, ucNextValueIndex);
            ucIsHeadData = 0;
        }
        char* szKey = szDestKey();
        iKeyLen = szDestKey.GetDataLen();

        for (int i = 0; i < _astKeyValueData.Size(); ++i)   //效率低
        {
            if (iKeyLen == _astKeyValueData[i]._szKey.GetDataLen()
                && 0 == strcmp(szKey, _astKeyValueData[i]._szKey())) //寻找已经存在的KEY
            {
                //追加VALUE
                int iCanSaveLen = _astKeyValueData[i]._szValue.GetBufferLen() - _astKeyValueData[i]._szValue.GetDataLen() - 1; //够放多少, 1表示\0
                if (iCanSaveLen <= 0)
                {
                    _astKeyValueData[i]._ucHasOtherData = 1;
                    _astKeyValueData[i]._ucNextValueIndex = ucNextValueIndex + 1;
                    return SetValue(szInputKey, szValue, ucNextValueIndex + 1, false);
                }

                int iHasSaveValueLen = 0; //已经保存的字节数 

                //先放分割符
                bool bHasSaveDel = false;
                if (iCanSaveLen > 0)
                {
                    _astKeyValueData[i]._szValue.Cat("%c", CTKeyValueData::VALUE_DEL_CHAR);
                    iCanSaveLen--;
                    bHasSaveDel = true;

                    if (iCanSaveLen > 0)
                    {
                        int iSaveLen = CT_MIN(iCanSaveLen, iValueLen);
                        int iLen = _astKeyValueData[i]._szValue.GetDataLen();
                        memcpy(_astKeyValueData[i]._szValue.GetBuffer() + iLen, szValue, iSaveLen);
                        _astKeyValueData[i]._szValue.SetDataLen(iLen + iSaveLen);
                        iHasSaveValueLen += iSaveLen;
                    }

                    if (iHasSaveValueLen >= iValueLen)
                    {
                        return 0;
                    }
                }
                
                if (iHasSaveValueLen < iValueLen)
                {
                    _astKeyValueData[i]._ucHasOtherData = 1;
                    _astKeyValueData[i]._ucNextValueIndex = ucNextValueIndex + 1;
                    return SetValue(szOrgKey, szValue + iHasSaveValueLen, ucNextValueIndex + 1, bHasSaveDel);
                }                
            }
        }

        if (_astKeyValueData.Size() >= _astKeyValueData.GetCount())
        {
            return -3;
        }

        CTKeyValueData stKeyValueData;
        stKeyValueData._ucNextValueIndex = ucNextValueIndex;
        stKeyValueData._szKey.Set("%s", szKey);
        stKeyValueData._ucIsHeadData = ucIsHeadData;

        if (ucNextValueIndex > 0 && !bHasSaveDel)
        {
            stKeyValueData._szValue.Set("%c", CTKeyValueData::VALUE_DEL_CHAR);
            bHasSaveDel = true;
        }

        int iCanSaveLen = stKeyValueData._szValue.GetBufferLen() - 1 - stKeyValueData._szValue.GetDataLen();
        if (iValueLen > iCanSaveLen)
        {
            int iLen = stKeyValueData._szValue.GetDataLen();
            memcpy(stKeyValueData._szValue.GetBuffer() + iLen, szValue, iCanSaveLen);
            stKeyValueData._szValue.SetDataLen(iLen + iCanSaveLen);
            stKeyValueData._ucHasOtherData = 1;
            stKeyValueData._ucNextValueIndex = ucNextValueIndex + 1;
            _astKeyValueData.AddOneItem(stKeyValueData);
            
            return SetValue(szOrgKey, szValue + iCanSaveLen, ucNextValueIndex + 1, bHasSaveDel);
        }
        else
        {
            stKeyValueData._szValue.Cat("%s", szValue);
            _astKeyValueData.AddOneItem(stKeyValueData);
        }
        
        return 0;
    }

    const char* GetValue(const char* szInputKey, unsigned char ucNextValueIndex = 0)
    {
        if (szInputKey == NULL)
        {
            return "";
        }

        const char * szOrgKey = szInputKey;
        CTSizeString<CTKeyValueData::MAX_KEY_LEN> szDestKey;
        if (ucNextValueIndex == 0)
        {
            szDestKey.Set("%s", szInputKey);
        }
        else
        {
            szDestKey.Set("%s_%d", szInputKey, ucNextValueIndex);
        }
        char* szKey = szDestKey();

        if (strlen(szKey) > CTKeyValueData::MAX_KEY_LEN || strlen(szKey) == 0)
        {
            return "";
        }

        static CTSizeString<CTKeyValueData::MAX_KEY_VALUE_LEN> _szGetValueBuf;
        if (ucNextValueIndex == 0)
        {
            _szGetValueBuf.SetDataLen(0);
        }
        
        for (int i = 0; i < _astKeyValueData.Size(); ++i)   //效率低
        {
            if ((int)strlen(szKey) == _astKeyValueData[i]._szKey.GetDataLen()
                && 0 == strcasecmp(szKey, _astKeyValueData[i]._szKey()))
            {
                _szGetValueBuf.Cat("%s", _astKeyValueData[i]._szValue());
                if (_astKeyValueData[i]._ucHasOtherData == 1)
                {   
                    GetValue(szOrgKey, _astKeyValueData[i]._ucNextValueIndex);
                }
                break;
            }
        }

        return _szGetValueBuf();
    }

    void OrderByLetter()
    {
        CTArray<CTKeyValueData, CTKeyValueData::MAX_KEY_VALUE_COUNT> astKeyValueDataTmp = _astKeyValueData;
        _astKeyValueData.Clear();

        while (astKeyValueDataTmp.Size() > 0)
        {
            int iNowIndex = -1;
            CTSizeString<CTKeyValueData::MAX_KEY_LEN> szNowKey;

            for (int i = 0; i < astKeyValueDataTmp.Size(); ++i)
            {
                if (iNowIndex < 0)
                {
                    iNowIndex = i;
                    szNowKey = astKeyValueDataTmp[i]._szKey;
                    continue;
                }

                if (strcasecmp(astKeyValueDataTmp[i]._szKey(), szNowKey()) < 0)
                {
                    szNowKey = astKeyValueDataTmp[i]._szKey;
                    iNowIndex = i;
                }
            }

            _astKeyValueData.AddOneItem(astKeyValueDataTmp[iNowIndex]);
            astKeyValueDataTmp.DelOneItem(iNowIndex);
        }
    }

public:
    CTArray<CTKeyValueData, CTKeyValueData::MAX_KEY_VALUE_COUNT> _astKeyValueData;
};

}

BEGIN_CODE_CONVERT(CTKeyValueData)
CODE_CONVERT(_szKey)
CODE_CONVERT(_szValue)
CODE_CONVERT(_ucNextValueIndex)
CODE_CONVERT(_ucHasOtherData)
CODE_CONVERT(_ucIsHeadData)
END_CODE_CONVERT(CTKeyValueData)


BEGIN_CODE_CONVERT(CTKeyValue)
CODE_CONVERT(_astKeyValueData)
END_CODE_CONVERT(CTKeyValue)


#endif // __CTKeyValue_h__

