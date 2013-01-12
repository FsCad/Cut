<%@ Page Language="C#" %>

<%--
Copyright (C) 2004-2007  Autodesk, Inc.
This library is free software; you can redistribute it and/or
modify it under the terms of version 2.1 of the GNU Lesser
General Public License as published by the Free Software Foundation.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
--%>

<%@ Import Namespace="System" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Collections.Specialized" %>
<%@ Import Namespace="OSGeo.MapGuide" %>

<%
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ����ͼ����Ӳ�
    //
    //         ���ߣ�Qin H.X.
    //
    //         ���ڣ� 2007.08.23
    //          
    //         �޸���ʷ���� 
    //          
    //---------------------------------------------------------------------------------------   
    Response.Charset = "utf-8";
    String sessionId = Request.Form.Get("SESSION");
    try
    {
        try
        {         
            // ������վ������ӣ�ͨ����װ��ʵ�֣� 
            UtilityClass utility = new UtilityClass();
            utility.InitializeWebTier(Request);
            utility.ConnectToServer(sessionId);
            MgSiteConnection siteConnection = utility.GetSiteConnection();
            if (siteConnection == null)
            {
                Response.Write("������վ�������ʧ�ܣ��˳�");
                 return;
            }
            // ������Դ����
            MgResourceService resourceService = siteConnection.CreateService(MgServiceType.ResourceService) as MgResourceService;

            String layerDefinition = @"Library://MgTutorial/Layers/Hydrography.LayerDefinition";
            MgResourceIdentifier layerDefId = new MgResourceIdentifier(layerDefinition);
            //l�򿪵�ͼ����
            MgMap map = new MgMap();
            map.Open(resourceService, "Sheboygan");
            // ��ȡ�㼯�϶���
            MgLayerCollection layers = map.GetLayers();
            // �����²㣬����������Ĳ��Ѿ����ڣ����׳��쳣
            MgLayer layer  = new MgLayer(layerDefId, resourceService);
            layer.SetName("Hydrography2");
            layer.SetDisplayInLegend(true);
            layer.SetLegendLabel("Hydrography2");
            layer.SetVisible(true);
            layers.Add(layer);
            Response.Write("���ͼ������˲�Hydrography2<br>");
            map.Save(resourceService);
        }
        catch (MgException exc)
        {
            Response.Write("��Hydrography2�Ѿ��ڵ�ͼ��<br>");
            Response.Write(exc.GetMessage());
            return;
        }

    }
    finally
    {
    }
%>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <title>Create Points Task</title>
    <script type="text/javascript">
        function RefreshMap() {
            parent.parent.GetMapFrame().Refresh();
        }
    </script>
</head>
<body onload="RefreshMap()">

</body>
</html>