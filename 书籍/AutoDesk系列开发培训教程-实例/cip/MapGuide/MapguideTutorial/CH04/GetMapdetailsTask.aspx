<%@ Page Language="C#" AutoEventWireup="true"  %>
<%@ Import Namespace="System" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Collections.Specialized" %>
<%@ Import Namespace="System.Text" %>
<%@ Import Namespace="OSGeo.MapGuide" %>


<%
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ���ȡ��ͼ����
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

        MgMap map = new MgMap();
        map.Open(resService, "Sheboygan");

        //�����ͼ����
        Response.Write("<b>  ��ͼ���� : </b>  " + map.GetName() + "</p>");
        Response.Write("<b>  ��ͼ�ĻỰID: </b>" + map.GetSessionId() + "</p>");
        Response.Write("<b>  ����� ID :    </b> " + map.GetObjectId() + "</p>");
        Response.Write("<b>  ��ʾ���� :  </b> " + map.GetViewScale() + "</p>");

        MgEnvelope envelope = map.GetMapExtent();
        MgCoordinate lowerLeft = envelope.GetLowerLeftCoordinate();
        MgCoordinate upperRight = envelope.GetUpperRightCoordinate();
        Response.Write("<b>  ���½�����:  </b>");
        Response.Write("(" + lowerLeft.GetX() + "," + lowerLeft.GetY() + "," + lowerLeft.GetZ() + ")");
        Response.Write("<BR>");
        Response.Write("<b> ���Ͻ�����: </b>");
        Response.Write("(" + upperRight.GetX() + "," + upperRight.GetY() + "," + upperRight.GetZ() + ")");

        Response.Write("<p><b>��ǰ��ͼ�������²���:</b></p>");
        MgLayerGroupCollection LayerGrpCollection = map.GetLayerGroups();
        for (int j = 0; j < LayerGrpCollection.GetCount(); j++)
        {
            Response.Write("<p>     " + LayerGrpCollection.GetItem(j).GetName() + "</p>");
        }
        MgLayerCollection layerCollection;
        layerCollection = map.GetLayers();
        MgLayer layer = null;
        Response.Write("<p><b>�������²�:</b></p>");
        for (int i = 0; i < layerCollection.GetCount(); i++)
        {
            layer = layerCollection.GetItem(i);
            Response.Write("<p>     " + layer.GetName() + "</p>");
        }
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
    <title>��ȡ��ͼ���� </title>
</head>
<body>
<form id="form1" runat="server">

</form>
</body>
</html>

