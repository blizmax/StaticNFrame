using NPOI.HSSF.UserModel;
using NPOI.SS.UserModel;
using NPOI.XSSF.UserModel;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace FileProcessor
{
    class Program
    {
        static string strBasePath = "../DataConfig/";
        static string strExcelPath = "Excel/";
        static string strStructPath = "Struct/Class/";
        static string strLogiClassFile = "../DataConfig/Struct/LogicClass.xml";
        static string strCPPFile = "../DataConfig/proto/NFDataDefine.hpp";

        static StreamWriter cppWriter;

        static string strCPPIObjectInfo;

        static void CreateProtoFile()
        {
            if (File.Exists(strCPPFile))
            {
                File.Delete(strCPPFile);
            }

            cppWriter = new StreamWriter(strCPPFile);

            string strCPPHead = "#pragma once\n\n#include<string>\n#include<vector>\n#include<NFComm/NFCore/NFPlatform.h>\n\nnamespace NFrame {\n";
            cppWriter.WriteLine(strCPPHead);
        }

        static void CreateStructFile()
        {
            Console.WriteLine("Start to generate struct files");

            //generate proto file for C++ & C#
            CreateProtoFile();

            //load all excel
            XmlDocument xmlDoc = new XmlDocument();
            XmlDeclaration xmlDecl;
            xmlDecl = xmlDoc.CreateXmlDeclaration("1.0", "UTF-8", "yes");
            xmlDoc.AppendChild(xmlDecl);

            XmlElement root = xmlDoc.CreateElement("", "XML", "");
            xmlDoc.AppendChild(root);

            XmlElement classElement = xmlDoc.CreateElement("", "Class", "");
            root.AppendChild(classElement);

            String[] xStructXLSXList = Directory.GetFiles(strBasePath + strExcelPath, "*", SearchOption.AllDirectories);
            foreach (string file in xStructXLSXList)
            {
                //TODO:此处要考虑多个同名文件，但是归属不同人的表格
                int nLastPoint = file.LastIndexOf(".") + 1;
                int nLastSlash = file.LastIndexOf("/") + 1;
                string strFileName = file.Substring(nLastSlash, nLastPoint - nLastSlash - 1);
                string strFileExt = file.Substring(nLastPoint, file.Length - nLastPoint);

                int nTempExcelPos = file.IndexOf("$");
                if (nTempExcelPos >= 0)
                {
                    // 打开excel之后生成的临时文件，略过
                    continue;
                }

                // 不是excel文件，默认跳过
                if (strFileExt != "xls" && strFileExt != "xlsx")
                {
                    continue;
                }

                // 单个excel文件转为xml
                if (!CreateStructXML(file, strFileName))
                {
                    Console.WriteLine("Create " + file + " failed, please check it!!!");
                    return;
                }

                // LogicClass.xml中的索引配置
                XmlElement subClassElement = xmlDoc.CreateElement("", "Class", "");
                classElement.AppendChild(subClassElement);

                subClassElement.SetAttribute("Id", strFileName);
                subClassElement.SetAttribute("Type", "TYPE_" + strFileName.ToUpper());
                subClassElement.SetAttribute("Path", "DataConfig/" + strStructPath + strFileName + ".xml");
                subClassElement.SetAttribute("Public", "0");
                subClassElement.SetAttribute("Desc", strFileName);
            }

            cppWriter.WriteLine("}");
            cppWriter.Close();

            xmlDoc.Save(strLogiClassFile);
        }

        static bool CreateStructXML(string file, string strFileName)
        {
            Console.WriteLine("Processing [" + file + "]");

            string curDir = Directory.GetCurrentDirectory();
            XSSFWorkbook workBook;
            using (FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read))
            {
                workBook = new XSSFWorkbook(fs);
            }

            if (workBook == null)
            {
                return false;
            }

            // 开始创建xml
            XmlDocument structDoc = new XmlDocument();
            XmlDeclaration xmlDecl;
            xmlDecl = structDoc.CreateXmlDeclaration("1.0", "UTF-8", "yes");
            structDoc.AppendChild(xmlDecl);

            // 写入XML root标签
            XmlElement root = structDoc.CreateElement("", "XML", "");
            structDoc.AppendChild(root);

            // 写入Propertys标签
            XmlElement dataNodes = structDoc.CreateElement("", "DataNodes", "");
            root.AppendChild(dataNodes);

            // 写入ListTable标签
            XmlElement listTable = structDoc.CreateElement("", "ListTable", "");
            root.AppendChild(listTable);

            // 写入Records标签
            XmlElement dataTables = structDoc.CreateElement("", "DataTables", "");
            root.AppendChild(dataTables);

            //cpp
            string strCPPInfo = "";
            string strCPPTableInfo = "";
            string strCPPTableEnumInfo = "";
            string strCPPTableEnumNameInfo = "";
            string strTableEnumInfo = "";
            string strCPPEnumInfo = "";
            string strCPPEnumNameInfo = "";
            strCPPInfo = strCPPInfo + "class " + strFileName + "\n{\npublic:\n";
            strCPPInfo = strCPPInfo + "\t//Class name\n\tstatic const std::string& ThisName(){ static std::string x" +
                         strFileName + " = \"" + strFileName + "\";" + " return x" + strFileName + "; }\n";
            strCPPEnumInfo = strCPPEnumInfo + "\n\tenum Enum" + strFileName + "Node\n\t{\n";
            strCPPEnumNameInfo = strCPPEnumNameInfo + "\t\tstatic const std::vector<std::string> " + strFileName +
                                 "NodeName =\n\t\t{\n";
            strCPPTableEnumInfo = strCPPTableEnumInfo + "\n\tenum Enum" + strFileName + "Table\n\t{\n";
            strCPPTableEnumNameInfo = strCPPTableEnumNameInfo + "\t\tstatic const std::vector<std::string> " +
                                      strFileName + "TableName = \n\t\t{\n";

            for (int i = 0; i < workBook.NumberOfSheets; ++i)
            {
                ISheet sheet = workBook.GetSheetAt(i);
                string sheetSubName = sheet.SheetName;
                int dash_pos = sheet.SheetName.IndexOf('_');
                if (dash_pos != -1)
                {
                    sheetSubName = sheet.SheetName.Substring(0, dash_pos);
                }

                sheetSubName = sheetSubName.ToLower();
                switch (sheetSubName)
                {
                    case "datanode":
                        //Data nodes
                    {
                        IRow headerRow = sheet.GetRow(sheet.FirstRowNum);

                        for (int row = sheet.FirstRowNum + 1; row <= sheet.LastRowNum; ++row)
                        {
                            var dataNode = structDoc.CreateElement("", "DataNode", "");
                            dataNodes.AppendChild(dataNode);
                            IRow rowData = sheet.GetRow(row);
                            ICell FirstCellData = rowData.GetCell(headerRow.FirstCellNum);
                            string strDataNodeName = FirstCellData.ToString();
                            string strDataNodeType = "";
                            for (int col = headerRow.FirstCellNum; col < headerRow.LastCellNum; col++)
                            {
                                ICell headerData = headerRow.GetCell(col);
                                string name = headerData.ToString();
                                ICell cellData = rowData.GetCell(col);
                                string strValue = cellData.ToString();
                                if (name.ToLower() == "type")
                                {
                                    strDataNodeType = strValue;
                                }
                                dataNode.SetAttribute(name, strValue);
                            }

                            strCPPEnumInfo = strCPPEnumInfo + "\t\tEnum_Node_" + strDataNodeName + ",\t\t//" +
                                             strDataNodeType + "\n";

                            strCPPEnumNameInfo = strCPPEnumNameInfo + "\t\t\t\"" + strDataNodeName + "\",\t\t//" +
                                                 strDataNodeType + "\n";
                        }
                    }
                        break;
                    case "listtable":
                    {
                        IRow headerRow = sheet.GetRow(sheet.FirstRowNum);

                        for (int row = sheet.FirstRowNum + 1; row <= sheet.LastRowNum; ++row)
                        {
                            IRow rowData = sheet.GetRow(row);
                            var tableNode = structDoc.CreateElement("Table");
                            listTable.AppendChild(tableNode);
                            for (int col = headerRow.FirstCellNum; col < headerRow.LastCellNum; col++)
                            {
                                ICell headerData = headerRow.GetCell(col);
                                string name = headerData.ToString();
                                ICell cellData = rowData.GetCell(col);
                                string strValue = cellData.ToString();
                                tableNode.SetAttribute(name, strValue);
                            }
                        }
                    }
                        break;
                    case "datatable":
                    {
                        if (dash_pos == -1 || dash_pos + 1 >= sheet.SheetName.Length) continue;

                        string strTableName = sheet.SheetName.Substring(dash_pos + 1);

                        IRow headerRow = sheet.GetRow(sheet.FirstRowNum);
                        var tableNode = structDoc.CreateElement("DataTable");
                        dataTables.AppendChild(tableNode);

                        strTableEnumInfo = strTableEnumInfo + "\n\tenum " + strTableName + "\n\t{\n";

                        strCPPTableEnumInfo = strCPPTableEnumInfo + "\t\tEnum_Table_" + strTableName + ",\t\t\n";
                        strCPPTableEnumNameInfo = strCPPTableEnumNameInfo + "\t\t\t\"" + strTableName + "\",\t\t\n";

                        tableNode.SetAttribute("TableName", strTableName);

                        for (int row = sheet.FirstRowNum + 1; row <= sheet.LastRowNum; ++row)
                        {
                            IRow rowData = sheet.GetRow(row);

                            XmlElement colNode = structDoc.CreateElement("Col");
                            tableNode.AppendChild(colNode);

                            ICell FirstCellData = rowData.GetCell(headerRow.FirstCellNum);
                            string strField = FirstCellData.ToString();
                            string strType = "";

                            for (int col = headerRow.FirstCellNum; col < headerRow.LastCellNum; col++)
                            {
                                ICell headerData = headerRow.GetCell(col);
                                string name = headerData.ToString();
                                ICell cellData = rowData.GetCell(col);
                                string strValue = cellData.ToString();
                                colNode.SetAttribute(name, strValue);
                                if (name.ToLower() == "type")
                                {
                                    strType = strValue;
                                }
                            }

                            strTableEnumInfo += "\t\t" + strTableName + "_" + strField + ",\t\t //" + strField + " -> " +
                                                strType + "\n";
                        }
                        strTableEnumInfo += "\n\t};\n";
                    }
                        break;
                    case "component":
                        //Do nothing
                        break;
                    default:
                        break;
                }
            }

            strCPPEnumInfo = strCPPEnumInfo + "\t\tEnum_Max_Node,\n\t};\n";
            strCPPEnumNameInfo = strCPPEnumNameInfo + "\t\t};\n";
            strCPPTableEnumInfo = strCPPTableEnumInfo + "\t\tEnum_Max_Table,\n\t};\n";
            strCPPTableEnumNameInfo = strCPPTableEnumNameInfo + "\t\t};\n";

            string strCPPGetEnumInfo = "\n\tstatic const std::string& Get" + strFileName + "NodeName(Enum" + strFileName + "Node index)\n\t{\n";
                strCPPGetEnumInfo = strCPPGetEnumInfo + strCPPEnumNameInfo; 
                strCPPGetEnumInfo = strCPPGetEnumInfo + "\t\tNF_ASSERT(index < " + strFileName + "NodeName.size());\n";
                strCPPGetEnumInfo = strCPPGetEnumInfo + "\t\treturn " + strFileName + "NodeName[index];\n";
                strCPPGetEnumInfo = strCPPGetEnumInfo + "\t}\n";

            string strCPPGetTableEnumInfo = "\n\tstatic const std::string& Get" + strFileName + "TableName(Enum" +
                                                strFileName + "Table index)\n\t{\n";
            strCPPGetTableEnumInfo = strCPPGetTableEnumInfo + strCPPTableEnumNameInfo;
            strCPPGetTableEnumInfo = strCPPGetTableEnumInfo + "\t\tNF_ASSERT(index < " + strFileName +
                                     "TableName.size());\n";
            strCPPGetTableEnumInfo = strCPPGetTableEnumInfo + "\t\treturn " + strFileName + "TableName[index];\n";
            strCPPGetTableEnumInfo = strCPPGetTableEnumInfo + "\t}\n";

            // cpp
            strCPPInfo += "\t//DataTables\n" + strCPPTableInfo + strTableEnumInfo + 
                strCPPEnumInfo + strCPPGetEnumInfo + 
                strCPPTableEnumInfo + strCPPGetTableEnumInfo + "\n};\n";
            cppWriter.WriteLine(strCPPInfo);

            string strXMLFile = strBasePath + strStructPath + strFileName + ".xml";
            structDoc.Save(strXMLFile);

            return true;
        }

        static void Main(string[] args)
        {
            var now = DateTime.Now;
            CreateStructFile();

            var dis = DateTime.Now - now;
            Console.WriteLine("Generate all files successful, use time = {0:3} s", string.Format("{0:N3}", dis.TotalSeconds));
            Console.Write("Press any key to quit...");
            Console.ReadKey();
        }
    }
}
