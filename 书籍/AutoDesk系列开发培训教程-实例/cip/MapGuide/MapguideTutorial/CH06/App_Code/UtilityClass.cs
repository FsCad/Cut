using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
// the above are the defautl reference
// Mapguide reference library
using OSGeo.MapGuide;
using System.Collections;
using System.Text;
using System.Xml;
using System.IO;

public class UtilityClass
{
    // վ�����Ӷ��� 
    MgSiteConnection siteConnection;

    public UtilityClass()
    {
    }
 //----------------------------------------------------------------------------------------
    // �� �ܣ� ��ȡվ�����Ӷ���
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public MgSiteConnection GetSiteConnection()
    {
        return siteConnection;
    }
    

    //----------------------------------------------------------------------------------------
    // �� �ܣ� ��ʼ�������
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public void InitializeWebTier(HttpRequest Request)
    {
        string realPath = Request.ServerVariables["APPL_PHYSICAL_PATH"];
        String configPath = realPath + "webconfig.ini";
        MapGuideApi.MgInitializeWebTier(configPath);
    }

    //----------------------------------------------------------------------------------------
    // �� �ܣ� ����վ�������
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public void ConnectToServer(String sessionID)
    {
        MgUserInformation userInfo = new MgUserInformation(sessionID);
        siteConnection = new MgSiteConnection();
        siteConnection.Open(userInfo);
    }


    //----------------------------------------------------------------------------------------
    // �� �ܣ� ����վ�������
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public void ConnectToServer()
    {
        MgResourceService resourceService = (MgResourceService)siteConnection.CreateService(MgServiceType.ResourceService);
        MgRenderingService renderingService = (MgRenderingService)siteConnection.CreateService(MgServiceType.RenderingService);
    }

    //----------------------------------------------------------------------------------------
    // �� �ܣ� ����ͼ���ΪDWF�ļ�
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public bool PlotMap()
    {
        // ��ȡ��Դ����͵�ͼ�������
        MgResourceService resourceService = (MgResourceService)siteConnection.CreateService(MgServiceType.ResourceService);
        MgMappingService mappingService = (MgMappingService)siteConnection.CreateService(MgServiceType.MappingService);
        //  �򿪵�ͼ
        MgMap map = new MgMap();
        map.Open(resourceService, "Sheboygan");
        // ����DWF��������Ϣ
        MgDwfVersion dwfVer = new MgDwfVersion("6.01", "1.2");
        MgPlotSpecification plotSpec = new MgPlotSpecification(8.5f, 11.0f, MgPageUnitsType.Inches, 0f, 0f, 0f, 0f);
        plotSpec.SetMargins(0.5f, 0.5f, 0.5f, 0.5f);

         // ���DWF�ļ�
        MgResourceIdentifier layoutRes = new MgResourceIdentifier("Library://MgTutorial/Layouts/SheboyganMap.PrintLayout");
        MgLayout layout = new MgLayout(layoutRes, " DWF�ļ�����ʾ��", MgPageUnitsType.Inches);
        MgByteReader byteReader = mappingService.GeneratePlot(map, plotSpec, layout, dwfVer);
         //�����ļ�
        MgByteSink byteSink = new MgByteSink(byteReader);
        string filePath = "C:\\Temp\\Map.DWF";
        byteSink.ToFile(filePath);
        
        return true;
    }

    //----------------------------------------------------------------------------------------
    // �� �ܣ� ����ͼ���Ϊͼ���ļ�
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public bool GenerateMapImage()
    {
        // ��ȡ��Դ�������Ⱦ�������
        MgResourceService resourceService = (MgResourceService)siteConnection.CreateService(MgServiceType.ResourceService);
        MgRenderingService renderingService = (MgRenderingService)siteConnection.CreateService(MgServiceType.RenderingService);
        //  �򿪵�ͼ
        MgMap map = new MgMap();
        map.Open(resourceService, "Sheboygan");
      
        //  ����ѡ��
        MgSelection selection = new MgSelection(map);
        selection.Open(resourceService, "Sheboygan");
        // ָ����Χ
        MgEnvelope extent = map.GetMapExtent();
        double x = 0.5*extent.GetLowerLeftCoordinate().GetX();
        double y = 0.5*extent.GetLowerLeftCoordinate().GetY() ;

        MgColor color = new MgColor("FFFFBF20");
        // ���ɲ�����ͼ���ļ�
        MgByteReader byteReader = renderingService.RenderMap(map, selection, extent, 450, 613, color, "PNG");
        MgByteSink byteSink = new MgByteSink(byteReader);
        string filePath = "C:\\Temp\\Map.PNG";
        byteSink.ToFile(filePath);
        return true;
    }

}
