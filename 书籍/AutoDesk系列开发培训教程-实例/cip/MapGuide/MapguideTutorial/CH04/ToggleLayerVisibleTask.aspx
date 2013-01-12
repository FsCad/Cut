<%@ Page Language="C#" %>
<%@ Import Namespace="System" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Collections.Specialized" %>
<%@ Import Namespace="System.Text" %>
<%@ Import Namespace="OSGeo.MapGuide" %>


<%
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ����Ʋ��ڵ�ͼ�еĿɼ���
    //
    //         ���ߣ�Qin H.X.
    //
    //         ���ڣ� 2007.08.23
    //          
    //         �޸���ʷ���� 
    //          
    //--------------------------------------------------------------------------------------- 
    String sessionId = Request.Form.Get("SESSION");
    try
    {
        UtilityClass utility = new UtilityClass();
        utility.InitializeWebTier(Request);
        utility.ConnectToServer(sessionId);
        MgSiteConnection siteConnection = utility.GetSiteConnection();
        if (siteConnection == null)
        {
            Response.Write("������Mapguideվ������ʱ��ʧ�ܣ��˳�");
            return;
        }

        MgResourceService resService = (MgResourceService)siteConnection.CreateService(MgServiceType.ResourceService);
        MgLayer tmpLayer = null;

        MgMap map = new MgMap();
        map.Open(resService, "Sheboygan");
        tmpLayer = utility.GetLayerByName(map, "Hydrography");
        tmpLayer.SetVisible(!tmpLayer.IsVisible());
        tmpLayer.ForceRefresh();
        map.Save(resService);
        
        if (tmpLayer.IsVisible())
            Response.Write("<p><b> ��Hydrography�� </b></p>");
        else
            Response.Write("<p><b>�ر�Hydrography�� </b></p>");
           
    }
    catch (MgException ex)
    {
        Response.Write(ex.GetMessage ());
        Response.Write(ex.GetDetails());
    }
    

%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Change Layer Visibility</title>

<script type="text/javascript">
    function RefreshMap() {
       parent.parent.Refresh();
    }
</script>

   
</head>
<body onload="javascript:RefreshMap()">

</body>
</html>
