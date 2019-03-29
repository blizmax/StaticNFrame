// -------------------------------------------------------------------------
//    @FileName         :    NFCommon.h
//    @Author           :    GaoYi
//    @Date             :    2018-11-27
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <cassert>
#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <stack>
#include <vector>

#include "common/lexical_cast.hpp"
#include "NFPlatform.h"

using namespace std;

/////////////////////////////////////////////////
/**
* @file NFCommon.h
* @brief  ������,���Ǿ�̬����,�ṩһЩ���õĺ��� .
*
*/
/////////////////////////////////////////////////

/**
* @brief  ���������࣬�ṩ��һЩ�ǳ������ĺ���ʹ��.
*
* ��Щ���������Ծ�̬�����ṩ�� �������¼��ֺ���:
*
* Trim�ຯ��,��Сдת������,�ָ��ַ���������ֱ�ӷָ��ַ�����
*
* ���ֵȣ�,ʱ����غ���,�ַ���ת������,�������ַ�����ת����,
*
* �滻�ַ�������,Ipƥ�亯��,�ж�һ�����Ƿ���������
*/
class _NFExport NFCommon
{
public:

	/**
	* @brief  ȥ��ͷ���Լ�β�����ַ����ַ���.
	*
	* @param sStr    �����ַ���
	* @param s       ��Ҫȥ�����ַ�
	* @param bChar   ���Ϊtrue, ��ȥ��s��ÿ���ַ�; ���Ϊfalse, ��ȥ��s�ַ���
	* @return string ����ȥ������ַ���
	*/
	static std::string trim(const std::string &sStr, const std::string &s = " \r\n\t", bool bChar = true);

	/**
	* @brief  ȥ����ߵ��ַ����ַ���.
	*
	* @param sStr    �����ַ���
	* @param s       ��Ҫȥ�����ַ�
	* @param bChar   ���Ϊtrue, ��ȥ��s��ÿ���ַ�; ���Ϊfalse, ��ȥ��s�ַ���
	* @return string ����ȥ������ַ���
	*/
	static std::string trimleft(const std::string &sStr, const std::string &s = " \r\n\t", bool bChar = true);

	/**
	* @brief  ȥ���ұߵ��ַ����ַ���.
	*
	* @param sStr    �����ַ���
	* @param s       ��Ҫȥ�����ַ�
	* @param bChar   ���Ϊtrue, ��ȥ��s��ÿ���ַ�; ���Ϊfalse, ��ȥ��s�ַ���
	* @return string ����ȥ������ַ���
	*/
	static std::string trimright(const std::string &sStr, const std::string &s = " \r\n\t", bool bChar = true);

	/**
	* @brief  �ַ���ת����Сд.
	*
	* @param sString  �ַ���
	* @return string  ת������ַ���
	*/
	static std::string lower(const std::string &sString);

	/**
	* @brief  �ַ���ת���ɴ�д.
	*
	* @param sString  �ַ���
	* @return string  ת����Ĵ�д���ַ���
	*/
	static std::string upper(const std::string &sString);

	/**
	* @brief  �ַ����Ƿ������ֵ�.
	*
	* @param sString  �ַ���
	* @return bool    �Ƿ�������
	*/
	static bool isdigit(const std::string &sInput);

	/**
	* @brief  �ַ���ת����ʱ��ṹ.
	*
	* @param sString  �ַ���ʱ���ʽ
	* @param sFormat  ��ʽ
	* @param stTm     ʱ��ṹ
	* @return         0: �ɹ�, -1:ʧ��
	*/
	static int str2tm(const std::string &sString, const std::string &sFormat, struct tm &stTm);

	/**
	* @brief GMT��ʽ��ʱ��ת��Ϊʱ��ṹ
	*
	* eg.Sat, 06 Feb 2010 09:29:29 GMT, %a, %d %b %Y %H:%M:%S GMT
	*
	* ������mktime����time_t, ����ע��ʱ�� ������mktime(&stTm)
	*
	* - timezone���ɱ��ص���(time(NULL)ֵ��ͬ) timezone��ϵͳ�� ,
	*
	* ��Ҫextern long timezone;
	*
	* @param sString  GMT��ʽ��ʱ��
	* @param stTm     ת�����ʱ��ṹ
	* @return         0: �ɹ�, -1:ʧ��
	*/
	static int strgmt2tm(const std::string &sString, struct tm &stTm);

	/**
	* @brief  ʱ��ת�����ַ���.
	*
	* @param stTm     ʱ��ṹ
	* @param sFormat  ��Ҫת����Ŀ���ʽ��Ĭ��Ϊ���ո�ʽ
	* @return string  ת�����ʱ���ַ���
	*/
	static std::string tm2str(const struct tm &stTm, const std::string &sFormat = "%Y%m%d%H%M%S");

	/**
	* @brief  ʱ��ת�����ַ���.
	*
	* @param t        ʱ��ṹ
	* @param sFormat  ��Ҫת����Ŀ���ʽ��Ĭ��Ϊ���ո�ʽ
	* @return string  ת�����ʱ���ַ���
	*/
	static std::string tm2str(const time_t &t, const std::string &sFormat = "%Y%m%d%H%M%S");

	/**
	* @brief  ��ǰʱ��ת���ɽ��ո�ʽ�ַ���
	* @param sFormat ��ʽ��Ĭ��Ϊ���ո�ʽ
	* @return string ת�����ʱ���ַ���
	*/
	static std::string now2str(const std::string &sFormat = "%Y%m%d%H%M%S");

	/**
	* @brief  ʱ��ת����GMT�ַ�����GMT��ʽ:Fri, 12 Jan 2001 18:18:18 GMT
	* @param stTm    ʱ��ṹ
	* @return string GMT��ʽ��ʱ���ַ���
	*/
	static std::string tm2GMTstr(const struct tm &stTm);

	/**
	* @brief  ʱ��ת����GMT�ַ�����GMT��ʽ:Fri, 12 Jan 2001 18:18:18 GMT
	* @param stTm    ʱ��ṹ
	* @return string GMT��ʽ��ʱ���ַ���
	*/
	static std::string tm2GMTstr(const time_t &t);

	/**
	* @brief  ��ǰʱ��ת����GMT�ַ�����GMT��ʽ:Fri, 12 Jan 2001 18:18:18 GMT
	* @return string GMT��ʽ��ʱ���ַ���
	*/
	static std::string now2GMTstr();

	/**
	* @brief  ��ǰ������(������)ת�����ַ���(%Y%m%d).
	*
	* @return string (%Y%m%d)��ʽ��ʱ���ַ���
	*/
	static std::string nowdate2str();

	/**
	* @brief  ��ǰ��ʱ��(ʱ����)ת�����ַ���(%H%M%S).
	*
	* @return string (%H%M%S)��ʽ��ʱ���ַ���
	*/
	static std::string nowtime2str();

	/**
	* @brief  ��ȡ��ǰʱ��ĵĺ�����.
	*
	* @return int64_t ��ǰʱ��ĵĺ�����
	*/
	static int64_t now2ms();

	/**
	* @brief  ȡ����ǰʱ���΢��.
	*
	* @return int64_t ȡ����ǰʱ���΢��
	*/
	static int64_t now2us();

#if NF_PLATFORM == NF_PLATFORM_WIN
	/**
	* @brief  �ַ���ת����T�ͣ����T����ֵ����, ���strΪ��,��TΪ0.
	*
	* @param sStr  Ҫת�����ַ���
	* @return T    T������
	*/
	template<typename T>
	static T strto(const std::string &sStr)
	{
		using strto_type = typename std::conditional<std::is_arithmetic<T>::value, p::strto1<T>, p::strto2<T>>::type;

		return strto_type()(sStr);
	}

	/**
	* @brief  �ַ���ת����T��.
	*
	* @param sStr      Ҫת�����ַ���
	* @param sDefault  ȱʡֵ
	* @return T        ת�����T����
	*/
	template<typename T>
	T strto(const std::string &sStr, const std::string &sDefault)
	{
		string s;

		if (!sStr.empty())
		{
			s = sStr;
		}
		else
		{
			s = sDefault;
		}

		return strto<T>(s);
	}
#endif

	typedef bool(*depthJudge)(const std::string& str1, const std::string& str2);

#if NF_PLATFORM == NF_PLATFORM_WIN
	/**
	* @brief  �����ַ���,�÷ָ����ŷָ�,������vector��
	*
	* ����: |a|b||c|
	*
	* ���withEmpty=trueʱ, ����|�ָ�Ϊ:"","a", "b", "", "c", ""
	*
	* ���withEmpty=falseʱ, ����|�ָ�Ϊ:"a", "b", "c"
	*
	* ���T����Ϊint����ֵ����, ��ָ����ַ���Ϊ"", ��ǿ��ת��Ϊ0
	*
	* @param sStr      �����ַ���
	* @param sSep      �ָ��ַ���(ÿ���ַ�����Ϊ�ָ���)
	* @param withEmpty true����յ�Ҳ��һ��Ԫ��, falseʱ�յĹ���
	* @param depthJudge �Էָ����ַ��ٴν����ж�
	* @return          ��������ַ�vector
	*/
	template<typename T>
	static std::vector<T> sepstr(const std::string &sStr, const std::string &sSep, bool withEmpty = false, depthJudge judge = nullptr)
	{
		std::vector<T> vt;

		std::string::size_type pos = 0;
		std::string::size_type pos1 = 0;
		int pos_tmp = -1;

		while (true)
		{
			std::string s;
			std::string s1;
			pos1 = sStr.find_first_of(sSep, pos);
			if (pos1 == std::string::npos)
			{
				if (pos + 1 <= sStr.length())
				{
					s = sStr.substr(-1 != pos_tmp ? pos_tmp : pos);
					s1 = "";
				}
			}
			else if (pos1 == pos && (pos1 + 1 == sStr.length()))
			{
				s = "";
				s1 = "";
			}
			else
			{
				s = sStr.substr(-1 != pos_tmp ? pos_tmp : pos, pos1 - (-1 != pos_tmp ? pos_tmp : pos));
				s1 = sStr.substr(pos1 + 1);
				if (-1 == pos_tmp)
					pos_tmp = pos;
				pos = pos1;
			}

			if (nullptr == judge || judge(s, s1))
			{
				if (withEmpty)
				{
					vt.push_back(strto<T>(s));
				}
				else
				{
					if (!s.empty())
					{
						T tmp = strto<T>(s);
						vt.push_back(tmp);
					}
				}
				pos_tmp = -1;
			}

			if (pos1 == string::npos)
			{
				break;
			}

			pos++;
		}

		return vt;
	}

	/**
	* @brief T��ת�����ַ�����ֻҪT�ܹ�ʹ��ostream������<<����,�����Ա��ú���֧��
	* @param t Ҫת��������
	* @return  ת������ַ���
	*/
	template<typename T>
	static std::string tostr(const T &t)
	{
		ostringstream sBuffer;
		sBuffer << t;
		return sBuffer.str();
	}

	/**
	* @brief  vectorת����string.
	*
	* @param t Ҫת����vector�͵�����
	* @return  ת������ַ���
	*/
	template<typename T>
	static std::string tostr(const std::vector<T> &t)
	{
		string s;
		for (size_t i = 0; i < t.size(); i++)
		{
			s += tostr(t[i]);
			s += " ";
		}
		return s;
	}


	/**
	* @brief  ��map���Ϊ�ַ���.
	*
	* @param map<K, V, D, A>  Ҫת����map����
	* @return                    string ������ַ���
	*/
	template<typename K, typename V, typename D, typename A>
	static std::string tostr(const std::map<K, V, D, A> &t)
	{
		string sBuffer;
		typename map<K, V, D, A>::const_iterator it = t.begin();
		while (it != t.end())
		{
			sBuffer += " [";
			sBuffer += tostr(it->first);
			sBuffer += "]=[";
			sBuffer += tostr(it->second);
			sBuffer += "] ";
			++it;
		}
		return sBuffer;
	}

	/**
	* @brief  map���Ϊ�ַ���.
	*
	* @param multimap<K, V, D, A>  map����
	* @return                      ������ַ���
	*/
	template<typename K, typename V, typename D, typename A>
	static std::string tostr(const std::multimap<K, V, D, A> &t)
	{
		string sBuffer;
		typename multimap<K, V, D, A>::const_iterator it = t.begin();
		while (it != t.end())
		{
			sBuffer += " [";
			sBuffer += tostr(it->first);
			sBuffer += "]=[";
			sBuffer += tostr(it->second);
			sBuffer += "] ";
			++it;
		}
		return sBuffer;
	}

	/**
	* @brief  pair ת��Ϊ�ַ�������֤map�ȹ�ϵ��������ֱ����tostr�����
	* @param pair<F, S> pair����
	* @return           ������ַ���
	*/
	template<typename F, typename S>
	static std::string tostr(const std::pair<F, S> &itPair)
	{
		string sBuffer;
		sBuffer += "[";
		sBuffer += tostr(itPair.first);
		sBuffer += "]=[";
		sBuffer += tostr(itPair.second);
		sBuffer += "]";
		return sBuffer;
	}

	/**
	* @brief  container ת�����ַ���.
	*
	* @param iFirst  ������
	* @param iLast   ������
	* @param sSep    ����Ԫ��֮��ķָ���
	* @return        ת������ַ���
	*/
	template <typename InputIter>
	static std::string tostr(InputIter iFirst, InputIter iLast, const std::string &sSep = "|")
	{
		std::string sBuffer;
		InputIter it = iFirst;

		while (it != iLast)
		{
			sBuffer += tostr(*it);
			++it;

			if (it != iLast)
			{
				sBuffer += sSep;
			}
			else
			{
				break;
			}
		}

		return sBuffer;
	}

	template <>
	static std::string tostr<bool>(const bool &t)
	{
		char buf[2];
		buf[0] = t ? '1' : '0';
		buf[1] = '\0';
		return std::string(buf);
	}

	template <>
	static std::string tostr<char>(const char &t)
	{
		char buf[2];
		snprintf(buf, 2, "%c", t);
		return std::string(buf);
	}

	template <>
	static std::string tostr<unsigned char>(const unsigned char &t)
	{
		char buf[2];
		snprintf(buf, 2, "%c", t);
		return std::string(buf);
	}

	template <>
	static std::string tostr<short>(const short &t)
	{
		char buf[16];
		snprintf(buf, 16, "%d", t);
		return std::string(buf);
	}

	template <>
	static std::string tostr<unsigned short>(const unsigned short &t)
	{
		char buf[16];
		snprintf(buf, 16, "%u", t);
		return std::string(buf);
	}

	template <>
	static std::string tostr<int>(const int &t)
	{
		char buf[16];
		snprintf(buf, 16, "%d", t);
		return std::string(buf);
	}

	template <>
	static std::string tostr<unsigned int>(const unsigned int &t)
	{
		char buf[16];
		snprintf(buf, 16, "%u", t);
		return std::string(buf);
	}

	template <>
	static std::string tostr<long>(const long &t)
	{
		char buf[32];
		snprintf(buf, 32, "%ld", t);
		return std::string(buf);
	}

	template <>
	static std::string tostr<long long>(const long long &t)
	{
		char buf[32];
		snprintf(buf, 32, "%lld", t);
		return std::string(buf);
	}


	template <>
	static std::string tostr<unsigned long>(const unsigned long &t)
	{
		char buf[32];
		snprintf(buf, 32, "%lu", t);
		return std::string(buf);
	}

	template <>
	static std::string tostr<float>(const float &t)
	{
		char buf[32];
		snprintf(buf, 32, "%.5f", t);
		std::string s(buf);

		//ȥ����Ч0, eg. 1.0300 -> 1.03;1.00 -> 1
		bool bFlag = false;
		int pos = int(s.size() - 1);
		for (; pos > 0; --pos)
		{
			if (s[pos] == '0')
			{
				bFlag = true;
				if (s[pos - 1] == '.')
				{
					//-2Ϊ��ȥ��"."��
					pos -= 2;
					break;
				}
			}
			else
			{
				break;
			}
		}

		if (bFlag)
			s = s.substr(0, pos + 1);

		return s;
	}

	template <>
	static std::string tostr<double>(const double &t)
	{
		char buf[32];
		snprintf(buf, 32, "%.5f", t);
		std::string s(buf);

		//ȥ����Ч0, eg. 1.0300 -> 1.03;1.00 -> 1
		bool bFlag = false;
		int pos = int(s.size() - 1);
		for (; pos > 0; --pos)
		{
			if (s[pos] == '0')
			{
				bFlag = true;
				if (s[pos - 1] == '.')
				{
					//-2Ϊ��ȥ��"."��
					pos -= 2;
					break;
				}
			}
			else
			{
				break;
			}
		}

		if (bFlag)
			s = s.substr(0, pos + 1);

		return s;

	}

	template <>
	static std::string tostr<long double>(const long double &t)
	{
		char buf[32];
		snprintf(buf, 32, "%Lf", t);
		std::string s(buf);

		//ȥ����Ч0, eg. 1.0300 -> 1.03;1.00 -> 1
		bool bFlag = false;
		int pos = int(s.size() - 1);
		for (; pos > 0; --pos)
		{
			if (s[pos] == '0')
			{
				bFlag = true;
				if (s[pos - 1] == '.')
				{
					//-2Ϊ��ȥ��"."��
					pos -= 2;
					break;
				}
			}
			else
			{
				break;
			}
		}

		if (bFlag)
			s = s.substr(0, pos + 1);

		return s;
	}

	template <>
	static std::string tostr<std::string>(const std::string &t)
	{
		return t;
	}
#endif

	/**
	* @brief  ����������ת�����ַ���.
	*
	* @param buf     ������buffer
	* @param len     buffer����
	* @param sSep    �ָ���
	* @param lines   ���ٸ��ֽڻ�һ��, Ĭ��0��ʾ������
	* @return        ת������ַ���
	*/
	static std::string bin2str(const void *buf, size_t len, const std::string &sSep = "", size_t lines = 0);

	/**
	* @brief  ����������ת�����ַ���.
	*
	* @param sBinData  ����������
	* @param sSep     �ָ���
	* @param lines    ���ٸ��ֽڻ�һ��, Ĭ��0��ʾ������
	* @return         ת������ַ���
	*/
	static std::string bin2str(const std::string &sBinData, const std::string &sSep = "", size_t lines = 0);

	/**
	* @brief   �ַ���ת���ɶ�����.
	*
	* @param psAsciiData �ַ���
	* @param sBinData    ����������
	* @param iBinSize    ��Ҫת�����ַ�������
	* @return            ת�����ȣ�С�ڵ���0���ʾʧ��
	*/
	static int str2bin(const char *psAsciiData, unsigned char *sBinData, int iBinSize);

	/**
	* @brief  �ַ���ת���ɶ�����.
	*
	* @param sBinData  Ҫת�����ַ���
	* @param sSep      �ָ���
	* @param lines     ���ٸ��ֽڻ�һ��, Ĭ��0��ʾ������
	* @return          ת����Ķ���������
	*/
	static std::string str2bin(const std::string &sBinData, const std::string &sSep = "", size_t lines = 0);

	/**
	* @brief  �滻�ַ���.
	*
	* @param sString  �����ַ���
	* @param sSrc     ԭ�ַ���
	* @param sDest    Ŀ���ַ���
	* @return string  �滻����ַ���
	*/
	static std::string replace(const std::string &sString, const std::string &sSrc, const std::string &sDest);

	/**
	* @brief  �����滻�ַ���.
	*
	* @param sString  �����ַ���
	* @param mSrcDest  map<ԭ�ַ���,Ŀ���ַ���>
	* @return string  �滻����ַ���
	*/
	static std::string replace(const std::string &sString, const std::map<std::string, std::string>& mSrcDest);

	/**
	* @brief ƥ����.�ָ����ַ�����pat��*�����ͨ���������ǿյ��κ��ַ���
	* sΪ��, ����false ��patΪ��, ����true
	* @param s    ��ͨ�ַ���
	* @param pat  ��*��ƥ����ַ���������ƥ��ip��ַ
	* @return     �Ƿ�ƥ��ɹ�
	*/
	static bool matchPeriod(const std::string& s, const std::string& pat);

	/**
	* @brief  ƥ����.�ָ����ַ���.
	*
	* @param s   ��ͨ�ַ���
	* @param pat vector�е�ÿ��Ԫ�ض��Ǵ�*��ƥ����ַ���������ƥ��ip��ַ
	* @return    �Ƿ�ƥ��ɹ�
	*/
	static bool matchPeriod(const std::string& s, const std::vector<std::string>& pat);

	/**
	* @brief  �ж�һ�����Ƿ�Ϊ����.
	*
	* @param n  ��Ҫ���жϵ�����
	* @return   true������������false��ʾ������
	*/
	static bool isPrimeNumber(size_t n);

	/**
	* @brief  daemon
	*/
	static void daemon();

	/**
	* @brief  ���Թܵ��쳣
	*/
	static void ignorePipe();

	/**
	* @brief  ��һ��string����ת��һ���ֽ� .
	*
	* @param sWhat Ҫת�����ַ���
	* @return char    ת������ֽ�
	*/
	static char x2c(const std::string &sWhat);

	/**
	* @brief  ��С�ַ��������ֽ�����֧��K, M, G���� ����: 1K, 3M, 4G, 4.5M, 2.3G
	* @param s            Ҫת�����ַ���
	* @param iDefaultSize ��ʽ����ʱ, ȱʡ�Ĵ�С
	* @return             �ֽ���
	*/
	static size_t toSize(const std::string &s, size_t iDefaultSize);
};

namespace p
{
	template<typename D>
	struct strto1
	{
		D operator()(const std::string &sStr)
		{
			std::string s = "0";

			if (!sStr.empty())
			{
				s = sStr;
			}

			istringstream sBuffer(s);

			D t;
			sBuffer >> t;

			return t;
		}
	};

	template<>
	struct strto1<char>
	{
		char operator()(const std::string &sStr)
		{
			if (!sStr.empty())
			{
				return sStr[0];
			}
			return 0;
		}
	};

	template<>
	struct strto1<short>
	{
		short operator()(const std::string &sStr)
		{
			if (!sStr.empty())
			{
				return atoi(sStr.c_str());
			}
			return 0;
		}
	};

	template<>
	struct strto1<unsigned short>
	{
		unsigned short operator()(const std::string &sStr)
		{
			if (!sStr.empty())
			{
				return strtoul(sStr.c_str(), NULL, 10);
			}
			return 0;
		}
	};

	template<>
	struct strto1<int>
	{
		int operator()(const std::string &sStr)
		{
			if (!sStr.empty())
			{
				return atoi(sStr.c_str());
			}
			return 0;
		}
	};

	template<>
	struct strto1<unsigned int>
	{
		unsigned int operator()(const std::string &sStr)
		{
			if (!sStr.empty())
			{
				return strtoul(sStr.c_str(), NULL, 10);
			}
			return 0;
		}
	};

	template<>
	struct strto1<long>
	{
		long operator()(const std::string &sStr)
		{
			if (!sStr.empty())
			{
				return atol(sStr.c_str());
			}
			return 0;
		}
	};

	template<>
	struct strto1<long long>
	{
		long long operator()(const std::string &sStr)
		{
			if (!sStr.empty())
			{
				return atoll(sStr.c_str());
			}
			return 0;
		}
	};

	template<>
	struct strto1<unsigned long>
	{
		unsigned long operator()(const std::string &sStr)
		{
			if (!sStr.empty())
			{
				return strtoul(sStr.c_str(), NULL, 10);
			}
			return 0;
		}
	};

	template<>
	struct strto1<float>
	{
		float operator()(const std::string &sStr)
		{
			if (!sStr.empty())
			{
				return atof(sStr.c_str());
			}
			return 0;
		}
	};

	template<>
	struct strto1<double>
	{
		double operator()(const std::string &sStr)
		{
			if (!sStr.empty())
			{
				return atof(sStr.c_str());
			}
			return 0;
		}
	};

	template<typename D>
	struct strto2
	{
		D operator()(const std::string &sStr)
		{
			istringstream sBuffer(sStr);

			D t;
			sBuffer >> t;

			return t;
		}
	};

	template<>
	struct strto2<std::string>
	{
		std::string operator()(const std::string &sStr)
		{
			return sStr;
		}
	};

}

#if NF_PLATFORM == NF_PLATFORM_LINUX

template<typename T>
T NFCommon::strto(const std::string &sStr)
{
	using strto_type = typename std::conditional<std::is_arithmetic<T>::value, p::strto1<T>, p::strto2<T>>::type;

	return strto_type()(sStr);
}

template<typename T>
T NFCommon::strto(const std::string &sStr, const std::string &sDefault)
{
	string s;

	if (!sStr.empty())
	{
		s = sStr;
	}
	else
	{
		s = sDefault;
	}

	return strto<T>(s);
}


template<typename T>
std::vector<T> NFCommon::sepstr(const std::string &sStr, const std::string &sSep, bool withEmpty, NFCommon::depthJudge judge)
{
	std::vector<T> vt;

	std::string::size_type pos = 0;
	std::string::size_type pos1 = 0;
	int pos_tmp = -1;

	while (true)
	{
		std::string s;
		std::string s1;
		pos1 = sStr.find_first_of(sSep, pos);
		if (pos1 == std::string::npos)
		{
			if (pos + 1 <= sStr.length())
			{
				s = sStr.substr(-1 != pos_tmp ? pos_tmp : pos);
				s1 = "";
			}
		}
		else if (pos1 == pos && (pos1 + 1 == sStr.length()))
		{
			s = "";
			s1 = "";
		}
		else
		{
			s = sStr.substr(-1 != pos_tmp ? pos_tmp : pos, pos1 - (-1 != pos_tmp ? pos_tmp : pos));
			s1 = sStr.substr(pos1 + 1);
			if (-1 == pos_tmp)
				pos_tmp = pos;
			pos = pos1;
		}

		if (nullptr == judge || judge(s, s1))
		{
			if (withEmpty)
			{
				vt.push_back(strto<T>(s));
			}
			else
			{
				if (!s.empty())
				{
					T tmp = strto<T>(s);
					vt.push_back(tmp);
				}
			}
			pos_tmp = -1;
		}

		if (pos1 == string::npos)
		{
			break;
		}

		pos++;
	}

	return vt;
}
template<typename T>
std::string NFCommon::tostr(const T &t)
{
	ostringstream sBuffer;
	sBuffer << t;
	return sBuffer.str();
}

template<typename T>
std::string NFCommon::tostr(const std::vector<T> &t)
{
	string s;
	for (size_t i = 0; i < t.size(); i++)
	{
		s += tostr(t[i]);
		s += " ";
	}
	return s;
}

template<typename K, typename V, typename D, typename A>
std::string NFCommon::tostr(const std::map<K, V, D, A> &t)
{
	string sBuffer;
	typename map<K, V, D, A>::const_iterator it = t.begin();
	while (it != t.end())
	{
		sBuffer += " [";
		sBuffer += tostr(it->first);
		sBuffer += "]=[";
		sBuffer += tostr(it->second);
		sBuffer += "] ";
		++it;
	}
	return sBuffer;
}

template<typename K, typename V, typename D, typename A>
std::string NFCommon::tostr(const std::multimap<K, V, D, A> &t)
{
	string sBuffer;
	typename multimap<K, V, D, A>::const_iterator it = t.begin();
	while (it != t.end())
	{
		sBuffer += " [";
		sBuffer += tostr(it->first);
		sBuffer += "]=[";
		sBuffer += tostr(it->second);
		sBuffer += "] ";
		++it;
	}
	return sBuffer;
}

template<typename F, typename S>
std::string NFCommon::tostr(const std::pair<F, S> &itPair)
{
	string sBuffer;
	sBuffer += "[";
	sBuffer += tostr(itPair.first);
	sBuffer += "]=[";
	sBuffer += tostr(itPair.second);
	sBuffer += "]";
	return sBuffer;
}

template <typename InputIter>
std::string NFCommon::tostr(InputIter iFirst, InputIter iLast, const std::string &sSep)
{
	std::string sBuffer;
	InputIter it = iFirst;

	while (it != iLast)
	{
		sBuffer += tostr(*it);
		++it;

		if (it != iLast)
		{
			sBuffer += sSep;
		}
		else
		{
			break;
		}
	}

	return sBuffer;
}

template <>
std::string NFCommon::tostr<bool>(const bool &t)
{
	char buf[2];
	buf[0] = t ? '1' : '0';
	buf[1] = '\0';
	return std::string(buf);
}


template <>
std::string NFCommon::tostr<char>(const char &t)
{
	char buf[2];
	snprintf(buf, 2, "%c", t);
	return std::string(buf);
}

template <>
std::string NFCommon::tostr<unsigned char>(const unsigned char &t)
{
	char buf[2];
	snprintf(buf, 2, "%c", t);
	return std::string(buf);
}

template <>
std::string NFCommon::tostr<short>(const short &t)
{
	char buf[16];
	snprintf(buf, 16, "%d", t);
	return std::string(buf);
}

template <>
std::string NFCommon::tostr<unsigned short>(const unsigned short &t)
{
	char buf[16];
	snprintf(buf, 16, "%u", t);
	return std::string(buf);
}

template <>
std::string NFCommon::tostr<int>(const int &t)
{
	char buf[16];
	snprintf(buf, 16, "%d", t);
	return std::string(buf);
}

template <>
std::string NFCommon::tostr<unsigned int>(const unsigned int &t)
{
	char buf[16];
	snprintf(buf, 16, "%u", t);
	return std::string(buf);
}

template <>
std::string NFCommon::tostr<long>(const long &t)
{
	char buf[32];
	snprintf(buf, 32, "%ld", t);
	return std::string(buf);
}

template <>
std::string NFCommon::tostr<long long>(const long long &t)
{
	char buf[32];
	snprintf(buf, 32, "%lld", t);
	return std::string(buf);
}


template <>
std::string NFCommon::tostr<unsigned long>(const unsigned long &t)
{
	char buf[32];
	snprintf(buf, 32, "%lu", t);
	return std::string(buf);
}

template <>
std::string NFCommon::tostr<float>(const float &t)
{
	char buf[32];
	snprintf(buf, 32, "%.5f", t);
	std::string s(buf);

	//ȥ����Ч0, eg. 1.0300 -> 1.03;1.00 -> 1
	bool bFlag = false;
	int pos = int(s.size() - 1);
	for (; pos > 0; --pos)
	{
		if (s[pos] == '0')
		{
			bFlag = true;
			if (s[pos - 1] == '.')
			{
				//-2Ϊ��ȥ��"."��
				pos -= 2;
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (bFlag)
		s = s.substr(0, pos + 1);

	return s;
}

template <>
std::string NFCommon::tostr<double>(const double &t)
{
	char buf[32];
	snprintf(buf, 32, "%.5f", t);
	std::string s(buf);

	//ȥ����Ч0, eg. 1.0300 -> 1.03;1.00 -> 1
	bool bFlag = false;
	int pos = int(s.size() - 1);
	for (; pos > 0; --pos)
	{
		if (s[pos] == '0')
		{
			bFlag = true;
			if (s[pos - 1] == '.')
			{
				//-2Ϊ��ȥ��"."��
				pos -= 2;
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (bFlag)
		s = s.substr(0, pos + 1);

	return s;

}

template <>
std::string NFCommon::tostr<long double>(const long double &t)
{
	char buf[32];
	snprintf(buf, 32, "%Lf", t);
	std::string s(buf);

	//ȥ����Ч0, eg. 1.0300 -> 1.03;1.00 -> 1
	bool bFlag = false;
	int pos = int(s.size() - 1);
	for (; pos > 0; --pos)
	{
		if (s[pos] == '0')
		{
			bFlag = true;
			if (s[pos - 1] == '.')
			{
				//-2Ϊ��ȥ��"."��
				pos -= 2;
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (bFlag)
		s = s.substr(0, pos + 1);

	return s;

}

template <>
std::string NFCommon::tostr<std::string>(const std::string &t)
{
	return t;
}
#endif




