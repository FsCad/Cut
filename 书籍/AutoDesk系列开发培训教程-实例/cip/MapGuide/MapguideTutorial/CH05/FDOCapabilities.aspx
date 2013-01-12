<%@ Page Language="C#" ContentType="text/xml"%>
<%@ Import Namespace="OSGeo.MapGuide" %>

<%
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ���ȡFDO provider ������
    //
    //         ���ߣ�Qin H.X.
    //
    //         ���ڣ� 2007.5.23
    //          
    //         �޸���ʷ���� 
    //          
    //--------------------------------------------------------------------------------------- 
    String provider = Request.QueryString.Get("provider");
    String fullProviderName = null;
    switch (provider)
    {
        case "SHP":
            fullProviderName = "OSGeo.SHP.3.1";
            break;
        case "SDF":
            fullProviderName = "OSGeo.SDF.3.1";
            break;
        case "ODBC":
            fullProviderName = "OSGeo.ODBC.3.1";
            break;
        case "MYSQL":
            fullProviderName = "OSGeo.MySQL.3.1";
            break;
        case "ARCSDE":
            fullProviderName = "OSGeo.ArcSDE.3.1";
            break;
        case "WMS":
            fullProviderName = "OSGeo.WMS.3.1";
            break;
        case "WFS":
            fullProviderName = "OSGeo.WFS.3.1";
            break;
        case "SQLSERVER":
            fullProviderName = "Autodesk.SqlServer.3.1";
            break;
        case "ORACLE":
            fullProviderName = "Autodesk.Oracle.3.1";
            break;
        case "RASTER":
            fullProviderName = "Autodesk.Raster.3.1";
            break;
    }
    


        MapGuideApi.MgInitializeWebTier(Request.ServerVariables["APPL_PHYSICAL_PATH"] + "webconfig.ini");

        MgSiteConnection siteConnection = new MgSiteConnection();
        siteConnection.Open(new MgUserInformation("Anonymous", ""));

        MgFeatureService featureService = (MgFeatureService)siteConnection.CreateService(MgServiceType.FeatureService);

        //��ȡFDO provider ������
           
         MgByteReader reader = featureService.GetCapabilities(fullProviderName);
        String capabilities = reader.ToString();
        Response.Write(capabilities);
   
%>
