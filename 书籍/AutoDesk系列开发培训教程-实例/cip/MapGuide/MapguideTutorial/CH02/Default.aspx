<%@ Page Language="C#" %>

<%@ Import Namespace="OSGeo.MapGuide" %>
<%
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ����ƽ��沼��
    //
    //         ���ߣ�Qin H.X.
    //
    //         ���ڣ� 2007.08.23
    //          
    //         �޸���ʷ���� 
    //          
    //--------------------------------------------------------------------------------------- 
    //��ʼ�������
    MapGuideApi.MgInitializeWebTier(@"C:\Program Files\Autodesk\MapGuideEnterprise2007\WebServerExtensions\www\webconfig.ini");
    // ������վ�������
    MgUserInformation userInfo = new MgUserInformation("Anonymous", "");
    MgSite site = new MgSite();
    site.Open(userInfo);
    String sessionId = site.CreateSession();
    // ָ�������õ����粼�� ��WebLayout��
    String webLayout = @"Library://Sample_World/Layouts/Sample_World _CH02.WebLayout";
    
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head id="Head1" runat="server">
    <title>CH02-Mapguide���ƽ��沼��</title>
</head>
<frameset rows="*,80">
    <frameset cols="*, 250">
        <frame src="/mapguide/mapviewernet/ajaxviewer.aspx?SESSION=<%= sessionId %>&WEBLAYOUT=<%= webLayout %>"
            name="ViewerFrame" />
        <frameset rows="*,*">
            <frame src="InfoPage.htm" name="InfoFrame" />
            <frame src="" name="DynaFrame" />
        </frameset>
    </frameset>
    <frame src="Toolbar.htm" name="ControlFrame" />

</frameset>
</html>
