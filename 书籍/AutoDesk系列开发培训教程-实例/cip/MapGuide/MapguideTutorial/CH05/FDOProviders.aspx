<%@ Page Language="C#" ContentType="text/xml" %>
<%@ Import Namespace="OSGeo.MapGuide" %>

<%

    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ���ȡϵͳ֧�ֵ�FDO provider �б�
    //
    //         ���ߣ�Qin H.X.
    //
    //         ���ڣ� 2007.5.23
    //          
    //         �޸���ʷ���� 
    //          
    //--------------------------------------------------------------------------------------- 
    // ��ʼ�������   
    MapGuideApi.MgInitializeWebTier(Request.ServerVariables["APPL_PHYSICAL_PATH"] + "webconfig.ini");
    //����վ������
    MgSiteConnection siteConnection = new MgSiteConnection();
    siteConnection.Open(new MgUserInformation("Anonymous", ""));
    // ����Ҫ�ط���
    MgFeatureService featureService = (MgFeatureService)siteConnection.CreateService(MgServiceType.FeatureService);
     
    // ��ȡϵͳ֧�ֵ�FDO provider �б�,���������� 
        
     MgByteReader reader = featureService.GetFeatureProviders();
    String providers = reader.ToString();
    Response.Write(providers);
%>



