<%@ Page Language="C#" Debug="true" %>
<%@ Import Namespace="System" %>
<%@ Import Namespace="System.IO" %>
<%@ Import Namespace="System.Xml" %>
<%@ Import Namespace="OSGeo.MapGuide" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<%

    //---------------------------------------------------------------------------------------
    //
    //        功能：获取仓储标题信息
    //
    //         作者：Qin H.X.
    //
    //         日期： 2007.08.23
    //          
    //         修改历史：无 
    //          
    //--------------------------------------------------------------------------------------- 
	String UserID = "Administrator";
	String Password = "admin";
    try
    {
        string realPath = Request.ServerVariables["APPL_PHYSICAL_PATH"];

        //  获取配置文件     
        String configPath = @"C:\Program Files\Autodesk\MapGuideEnterprise2007\WebServerExtensions\www\webconfig.ini";

         if (!File.Exists(configPath))
        {
            Response.Write("配置文件 webconfig.ini没有找到，终止");
            Response.End();
        }
        //指定输出文件及其路径
        String MapResContFilePath = realPath + @"Support\RepositoryHeader.xml";
        // 初始化网路层 
        MapGuideApi.MgInitializeWebTier(configPath);

        //建立到站点的链接
        MgUserInformation userInfo = new MgUserInformation(UserID, Password);  
        MgSiteConnection siteConn = new MgSiteConnection();
        siteConn.Open(userInfo);
        MgSite site = new MgSite();
        site.Open(userInfo);
      
        //创建会话
        String sessionId = site.CreateSession();
        // 创建资源服务
        MgResourceService resService = (MgResourceService)siteConn.CreateService(MgServiceType.ResourceService);
        MgResourceIdentifier resourceID = new MgResourceIdentifier("Library://");

        ///获取仓储标题信息     
        MgByteReader byteReader = resService.GetRepositoryHeader(resourceID);
        //保存文件
        MgByteSink byteSink = new MgByteSink(byteReader);
        byteSink.ToFile(MapResContFilePath);
        Response.Write("<b> 仓储标题文件 " + MapResContFilePath + " 完成创建.");
      
    }
     
    catch (MgException Mge)
    {
        Response.Write(Mge.GetMessage());
        Response.Write("<br>");
        Response.Write(Mge.GetDetails());
        
    }


%>



    


