<%@ Page Language="C#" Debug="true" %>
<%@ Import Namespace="System" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Xml" %>
<%@ Import Namespace="OSGeo.MapGuide" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<%
    
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ�������Դ
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
    String MapName;
    try
    {
        String configPath = @"C:\Program Files\Autodesk\MapGuideEnterprise2007\WebServerExtensions\www\webconfig.ini";

        if (!File.Exists(configPath))
        {
            Response.Write("�ļ�û���ҵ� webconfig.ini ,��ֹ");
            Response.End();
        }
        // ��ʼ�������
        MapGuideApi.MgInitializeWebTier(configPath);
        //����վ������
        MgUserInformation userInfo = new MgUserInformation(UserID, Password);

        MgSiteConnection siteConn = new MgSiteConnection();
        siteConn.Open(userInfo);
        MgSite site = new MgSite();
        site.Open(userInfo);
        String sessionId = site.CreateSession();
   
        // ������Դ����
        MgResourceService resService = (MgResourceService)siteConn.CreateService(MgServiceType.ResourceService);
        //ָ��Ҫ��������Դ
        MgResourceIdentifier resourceID = new MgResourceIdentifier("Library://");

        // ��ȡ��ǰ���е����е�ͼ����      
        MgByteReader byteRdr = resService.EnumerateResources(resourceID, -1, "MapDefinition");
         //�����ؽ��
        String MapResStr = byteRdr.ToString();
        //����XML�ĵ������н��������еĵ�ͼ����
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(MapResStr);
        //��ȡ��ͼ����        
        XmlNodeList MapResIdNodeList;
        XmlElement root = doc.DocumentElement;
        MapResIdNodeList = root.SelectNodes("//ResourceId");
        int nRes = MapResIdNodeList.Count;

        Response.Write("<form>");
        Response.Write("<strong><span style=\"font-size: 16pt\">Site Maps</span></strong>");
        Response.Write("<br /><br />");
        Response.Write("<select Name=\"lstMapRes\" Style=\"z-index: 101; Width=\"115px\"; left: 51px; position: absolute; top: 223px\">");
        for (int i = 0; i < nRes; i++)
        {
            XmlNode MapResIdNode = MapResIdNodeList.Item(i);
            String MapRes = MapResIdNode.InnerText;
            int index1 = MapRes.LastIndexOf('/') + 1;
            int index2 = MapRes.IndexOf("MapDefinition") - 2;
            int length = index2 - index1 + 1;
            MapName = MapRes.Substring(index1, length);
            Response.Write(" <option value=\"" + MapRes + "\"\"Value\">" + MapName + "</option>");
        }

        Response.Write("</select>");
        Response.Write("<br /><br />");
        Response.Write("</form>");
    }
     
    catch (MgException Mge)
    {
        Response.Write(Mge.GetMessage());
        Response.Write("<br>");
        Response.Write(Mge.GetDetails());
    }

%>
