<%@ Page Language="C#" AutoEventWireup="true" CodeFile="user_auth.aspx.cs" Inherits="user_auth" %>
<%@ Import Namespace = "System" %>
<%@ Import Namespace = "System.IO" %>
<%@ import Namespace = "OSGeo.MapGuide" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ�ͨ���û�������������û���֤
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
        // ��ȡ���������ļ���������ÿ�ԴMapguide���޸�Ϊ��Ӧ���ļ�·��
        String configPath = @"C:\Program Files\Autodesk\MapGuideEnterprise2007\WebServerExtensions\www\webconfig.ini";
        if (!File.Exists(configPath))
        {
            Response.Write("�ļ�û���ҵ� webconfig.ini ,��ֹ");
            Response.End();
        }
        // ��ʼ�������
        MapGuideApi.MgInitializeWebTier(configPath);
        //����MgUserInformation����
        MgUserInformation userInfo = new MgUserInformation(UserID, Password);

        //����վ�����Ӷ��󲢴�վ�����
        MgSiteConnection siteConn = new MgSiteConnection();
        siteConn.Open(userInfo);
        Response.Write("<b>��ӭ��½ MapGuide վ�� <br>��ǰ��½���û���Ϊ��"); 
        Response.Write(UserID);
    }
    catch (MgException Mge)
    {
        Response.Write(Mge.GetMessage());
        Response.Write("<br>");
        Response.Write(Mge.GetDetails());
    }

%>
