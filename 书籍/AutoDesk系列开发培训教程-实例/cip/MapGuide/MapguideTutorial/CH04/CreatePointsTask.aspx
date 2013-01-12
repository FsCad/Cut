<%@ Page Language="C#" AutoEventWireup="true" %>

<%@ Import Namespace="System" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Collections.Specialized" %>
<%@ Import Namespace="System.Text" %>
<%@ Import Namespace="OSGeo.MapGuide" %>
<%
    String sessionId = Request.Form.Get("SESSION");
    UtilityClass utility = new UtilityClass();
    utility.InitializeWebTier(Request);
    utility.ConnectToServer(sessionId);
    MgSiteConnection siteConnection = utility.GetSiteConnection();
    if (siteConnection == null)
    {
        Response.Write("��ȡվ������ʧ�ܣ��˳�");
        return;
    }
    bool res; 
    try
    {
         res = utility.CreatePointsLayer(Request.ServerVariables["APPL_PHYSICAL_PATH"], sessionId);
    
    }
    catch (ManagedException ex)
    {
        Response.Write(ex.Message );
    } 
     
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>������ʱ��</title>

    <script type="text/javascript">
        function RefreshMap() {
            parent.parent.GetMapFrame().Refresh();
        }
    </script>

</head>
<body onload="RefreshMap()">
<%
    Response.Write("��ɴ�����ʱ��");     
%>
</body>
</html>
