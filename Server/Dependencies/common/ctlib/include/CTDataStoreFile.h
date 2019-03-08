#ifndef __CT_CDATASTOREFILE_H__
#define __CT_CDATASTOREFILE_H__

#include "base/CTFile.h"

namespace CTLib
{

//在文件中保存数据,

class CDataStoreFile
{
protected:
    enum enumDataStoreFileStat
    {
        EDSFS_NOT_INIT    = 0, ///尚非做任何初始化
        EDSFS_INIT_FINISH = 1, ///数据初始化完毕
    };
    enum enumDataWriteStat
    {
        EDWS_BEGIN    = 0, ///开始状态第一次调用输入此状态
        EDWS_WRITEING = 1, ///还有后续数据
        EDWS_OVER     = 2, ///所有数据写入完毕了
    };

public:
    CDataStoreFile()
    {
        m_iReadDataCnt = 0;
        m_ucDataStat = EDSFS_NOT_INIT;
        m_iReadFlag = 0;
     }

    virtual ~CDataStoreFile()
    {

    }

protected:

    //获取一行数据,每个衍生类自行处理行字符串，解析成合理的数据并导入到自身的数据维护区
    virtual int ReadLineData(const char* pszData) = 0; 
    
    /**
    * 此接口由于框架顺序的获取字符串化后的数据节点
    * iWFlg 指定获取数据的标签,对标签具体的解读由实现时去做，这里只是框架给写入函数提供了
    * 一个持续的临时变量,记录当前的操作位置。
    *
    * enWriteStat 写入状态,作为框架和实现之间通讯用户的参数变量
    * 框架通过设置此参数通知 是开始了一次新的写入,还是在持续写入中, 
    * 实体通过这个参数通知框架所有的数据已经获取完成.
    *
    * stOutStr 获取到字符串数据
    */
    virtual int WriteLineData(int& iWFlg, enumDataWriteStat& enWriteStat, string& stOutStr) = 0;

    //获取需要写入的内容总量，每个不同的子类应用会单独根据自己的数据管理方式维护这个属性
    virtual size_t GetDataContentCnt() = 0;   

    //打开文件并逐行读入，调用子类的借口分析每一行数据
    virtual int ReadDataFile(const char* pszFileName, bool bIsNeedLoadData)
    {
        if (m_sFileName == "")
        {
            if (pszFileName == NULL)
            {
                CT_WARNING(("not data file name !"));
                return -1;
            }
            m_sFileName.Set("%s", pszFileName);
        }

        if (bIsNeedLoadData == false)
        {
            CT_TRACE((" do not need read data from data file!"));
            m_ucDataStat = EDSFS_INIT_FINISH;
            return 0;
        }

        int iRet = m_stFileReader.Open(m_sFileName());
        if (iRet)
        {
            CT_WARNING(("open data(%s) file false", m_sFileName()));

            //不存在数据文件也需要把数据设置为已经读取完毕
            m_ucDataStat = EDSFS_INIT_FINISH;
            return 0;
        }

        char * pstData = m_stFileReader.ReadLine();
        if (pstData == NULL)
        {
            CT_ERROR(("data file(%s) info fails!", m_sFileName()));
            return -2;
        }

        //parse the total row count of this data object
        m_iReadDataCnt = atoi(pstData);
        if (m_iReadDataCnt < 0)
        {
            CT_ERROR(("get data file(%s) data counts fails!(%d)", m_sFileName(), iRet));
            return -3;
        }

        m_iReadFlag = 0;

        while(m_iReadFlag < m_iReadDataCnt)
        {//读入一行数据
            iRet = GetLineData();
            if (iRet)
            {
                break;
            }
        }
        
        if (iRet == 0)
        {
            m_ucDataStat = EDSFS_INIT_FINISH;
        }
        return iRet;
    }

    //保存数据
    virtual int StoreDataFile()
    {
        if (m_ucDataStat != EDSFS_INIT_FINISH)
        {//如果当前不是完成初始化状态则不需要写入数据
            return 0;
        }

        size_t lDataItemCnt = GetDataContentCnt();
        if (lDataItemCnt < 0)
        {
            CT_ERROR(("StoreDataFile to file (%s), data cnt(%l) fails!", m_sFileName.Get(), lDataItemCnt));
            return -1;
        }

        if (lDataItemCnt == 0)
        {
            return 0;
        }

        //dump data into file if the content cnt is larger than zero
        //Open the file first.
        int iRet = m_stFileWriter.Open(m_sFileName(), false);
        if (0 != iRet)
        {
            CT_ERROR(("Failed to open file %s, ret = %d", m_sFileName.Get(), iRet));
            return -1;
        }
        
        CTSizeString<100> stTmpString;
        stTmpString.Set("%d", lDataItemCnt);

        ///写入数据总量
        iRet = m_stFileWriter.WriteLine(stTmpString());
        if (0 != iRet)
        {
            CT_WARNING(("Fail to write data at idx %ld, ret = %d", iRet));
            return iRet;
        }

        //写入数据
        int iWriteCount = 0;
        int iWFlg = 0;
        enumDataWriteStat enWriteStat = EDWS_BEGIN;
        do
        {
            ++iWriteCount;
            string stOutStr;
            int iRet = WriteLineData(iWFlg, enWriteStat, stOutStr);
            if (iRet == 0)
            {
                iRet = m_stFileWriter.WriteLine(stOutStr.c_str());
                if (0 != iRet)
                {
                    CT_WARNING(("Fail to write data at idx %ld, ret = %d", iRet));
                    return iRet;
                }
            }

            if (enWriteStat == EDWS_OVER)
            {//所有的数据都已经读完了
                break;
            }
        }
        while(iWriteCount < (int)lDataItemCnt);  

        if (iWriteCount != (int)lDataItemCnt)
        {
            CT_ERROR(("Store file(%s) data data cnt(%l) but writ cont(%d)",
                                m_sFileName.Get(), lDataItemCnt, iWriteCount));
        }
        return 0;
    }



    //获取一行数据
    int GetLineData()
    {
        if (m_stFileReader.HasOpened() == false)
        {
            CT_WARNING(("do not open file(%s) or open fails!", m_sFileName()));
            return -1;
        }

        //从文件中读取一行
        char * pstData = m_stFileReader.ReadLine();
        if (pstData == NULL)
        {
            CT_ERROR(("data file(%s) info fails!", m_sFileName()));
            return -2;
        }

        //调用子类自定义的行分析接口
        int iRet = ReadLineData(pstData);
        if (iRet)
        {
            CT_ERROR(("read data file(%s) data fails readflg(%d)", m_sFileName(), m_iReadFlag));
            return iRet;
        }

        ++m_iReadFlag;
        return 0;
    }


private:
    CTSizeString<1024> m_sFileName;
    int m_iReadDataCnt;
    int m_iReadFlag; 
    CTReadFile m_stFileReader;
    CTWriteFile m_stFileWriter;
    unsigned char m_ucDataStat;
};

}//namespace CTLib

#endif //__CT_CDATASTOREFILE_H__

