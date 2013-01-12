<%@ Page Language="C#" Debug="true" %>
<%@ Import Namespace="System" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Xml" %>
<%@ Import Namespace="OSGeo.MapGuide" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<%
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ���ȡ��Դ������Ϣ 
    //
    //         ���ߣ�Qin H.X.
    //
    //         ���ڣ� 2007.08.23
    //          
    //         �޸���ʷ���� 
    //          
    //---------------------------------------------------------------------------------------   
	String UserID = "Administrator";
	String Password = "admin";
    try
    {
        string realPath = Request.ServerVariables["APPL_PHYSICAL_PATH"];

        //  ��ȡ�����ļ�     
        String configPath = @"C:\Program Files\Autodesk\MapGuideEnterprise2007\WebServerExtensions\www\webconfig.ini";
        if (!File.Exists(configPath))
        {
            Response.Write("�ļ�û���ҵ� webconfig.ini ,��ֹ");
            Response.End();
        }
        //ָ������ļ�����·��
        String MapResContFilePath = realPath + @"Support\MapResourceHeader.xml";
        //��ʼ�������
        MapGuideApi.MgInitializeWebTier(configPath);

        //����վ������
        MgUserInformation userInfo = new MgUserInformation(UserID, Password);
        MgSiteConnection siteConn = new MgSiteConnection();
        siteConn.Open(userInfo);
        
        MgSite site = new MgSite();
        site.Open(userInfo);
        
        //�����Ự
        String sessionId = site.CreateSession();
         // ������Դ����
        MgResourceService resService = (MgResourceService)siteConn.CreateService(MgServiceType.ResourceService);

        MgResourceIdentifier resourceID = new MgResourceIdentifier("Library://MgTutorial/Maps/Sheboygan.MapDefinition");
        ///��ȡ��Դ������Ϣ     
        MgByteReader byteReader = resService.GetResourceHeader(resourceID);
        //����Ϊ�ļ�
        MgByteSink byteSink = new MgByteSink(byteReader);
        byteSink.ToFile(MapResContFilePath);
        Response.Write("<b> ��ͼ���壨��Դ���⣩" + MapResContFilePath + " �������.");
      
    }
     
    catch (MgException Mge)
    {
        Response.Write(Mge.GetMessage());
        Response.Write("<br>");
        Response.Write(Mge.GetDetails());
    }


%>



    


