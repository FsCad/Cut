<%@ Page Language="C#" %>

<%@ Import Namespace="System" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Collections.Specialized" %>
<%@ Import Namespace="System.Text" %>
<%@ Import Namespace="OSGeo.MapGuide" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<% 
    
    String sessionId = Request.Form.Get("SESSION");

    //
    UtilityClass utility = new UtilityClass();
    utility.InitializeWebTier(Request);
    utility.ConnectToServer(sessionId);
    MgSiteConnection siteConnection = utility.GetSiteConnection();
    if (siteConnection == null)
    {
        Response.Write("����վ��ʧ�ܣ��˳�");
        return;
    }

    bool result = utility.PlotMap();
   

%>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>Plot Map</title>
</head>
<body>
    <%
        if (result)
            Response.Write(@" DWF�ļ������ɹ�. ��鿴 C:\temp\Map.DWF");
    %>
</body>
</html>
