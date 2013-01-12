using System;
using System.Collections.Generic;
using System.Text;
using Inventor;

namespace Chapter9
{
    class myTranslatorAddIn
    {
        public void PublishDWF(Inventor.Application ThisApplication)
        {
            //���DWF translator
            TranslatorAddIn DWFAddIn;
            DWFAddIn = (TranslatorAddIn)ThisApplication.ApplicationAddIns.get_ItemById("{0AC6FD95-2F4D-42CE-8BE0-8AEA580399E4}");

            //���õ������ĵ���Ҫ����ΪDWF���ĵ���������
            Document doc;
            doc = ThisApplication.ActiveDocument;

            TranslationContext context;
            context = ThisApplication.TransientObjects.CreateTranslationContext();
            context.Type = IOMechanismEnum.kFileBrowseIOMechanism;

            //�����µ�NameValueMap����
            Inventor.NameValueMap options;
            options = ThisApplication.TransientObjects.CreateNameValueMap();

            //�����µ�DataMedium����
            DataMedium dataMedium;
            dataMedium = ThisApplication.TransientObjects.CreateDataMedium();

            //���translator�Ƿ������SaveCopyAs��ѡ��
            if (DWFAddIn.get_HasSaveCopyAsOptions(dataMedium, context, options))
            {
                options.set_Value("Launch_Viewer", 1);
                //����ѡ��Ϊ��
                //options.set_Value("Publish_All_Component_Props", 1);
                //options.set_Value("Publish_All_Physical_Props", 1);
                //options.set_Value("Password", 0);

                if (doc.DocumentType == DocumentTypeEnum.kDrawingDocumentObject)
                {
                    //����ͼѡ��
                    options.set_Value("Publish_Mode", DWFPublishModeEnum.kCustomDWFPublish);
                    options.set_Value("Publish_All_Sheets", 0);

                    //���"Publish_All_Sheets"Ϊtrue(1)��������ָ����ҳ��
                    Inventor.NameValueMap sheets;
                    sheets = ThisApplication.TransientObjects.CreateNameValueMap();

                    //������һ��ҳ�漰����άģ��
                    Inventor.NameValueMap sheet1Options;
                    sheet1Options = ThisApplication.TransientObjects.CreateNameValueMap();

                    sheet1Options.Add("Name", "ͼֽ:1");
                    sheet1Options.Add("3DModel", true);

                    sheets.set_Value("Sheet1", sheet1Options);

                    //����������ҳ�浫����������άģ��
                    Inventor.NameValueMap sheet3Options;
                    sheet3Options = ThisApplication.TransientObjects.CreateNameValueMap();

                    sheet3Options.Add("Name", "ͼֽ3:3");
                    sheet3Options.Add("3DModel", false);

                    sheets.set_Value("Sheet2", sheet3Options);

                    //����ҳ��ѡ��
                    options.set_Value("Sheets", sheets);
                }
            }
            //����DWF�ļ���
            dataMedium.FileName = "c:\\temp\\test.dwf";

            //�����ĵ�
            DWFAddIn.SaveCopyAs(doc, context, options, dataMedium);
        }
    }
}
