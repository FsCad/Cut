<%@ Page Language="C#" Debug="true" %>
<%@ Import Namespace="System" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="OSGeo.MapGuide" %>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<%
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ���ɴ����û��Ự
    //
    //         ���ߣ�Qin H.X.
    //
    //         ���ڣ� 2007.08.23
    //          
    //         �޸���ʷ���� 
    //          
    //---------------------------------------------------------------------------------------   
    
    String UserID = Request.Form["UserName"];
	String Password = Request.Form["Password"];	
    try
    {

        // ��ȡ���������ļ�
        // ���������ļ���Ҫ�Ӱ�װĿ¼���Ƶ�վ�����ڵ�Ŀ¼.
        string realPath = Request.ServerVariables["APPL_PHYSICAL_PATH"];
        String configPath = realPath + "webconfig.ini";
        if (!File.Exists(configPath))
        {
            Response.Write(configPath);
            Response.Write("�ļ�û���ҵ� webconfig.ini ,��ֹ");
            Response.End();
        }
        //��ʼ�������
        MapGuideApi.MgInitializeWebTier(configPath);
        //�����û���Ϣ����
        MgUserInformation userInfo = new MgUserInformation(UserID, Password);
        // ͨ���û���Ϣ���󴴽�վ������       
        MgSiteConnection siteConn = new MgSiteConnection();
        siteConn.Open(userInfo);
        //��ȡվ�����
        MgSite site = new MgSite();
        site.Open(userInfo);
        //ͨ��վ����󴴽��û��Ự
        String sessionId = site.CreateSession();
        Response.Write("�������û��Ự IDΪ: ");
        Response.Write(sessionId);
    }
    catch (MgException Mge)
    {
        Response.Write(Mge.GetMessage());
        Response.Write("<br>");
        Response.Write(Mge.GetDetails());
    }
%>



    


