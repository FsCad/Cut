using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using OSGeo.MapGuide;
using System.Collections;
using System.Text;
using System.Xml;
using System.IO;
//---------------------------------------------------------------------------------------
//
//        ���ܣ������࣬���վ������ӣ���װ���ܺ���
//
//         ���ߣ� 
//
//         ���ڣ� 2007.5.23
//          
//         �޸���ʷ���� 
//          
//--------------------------------------------------------------------------------------- 
public class UtilityClass
{
    // վ��siteConnection����ʵ�� 
    MgSiteConnection siteConnection;

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
    // �� �ܣ� ͨ��������ȡMgLayer�����
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public MgLayer GetLayerByName(MgMap map, String layerName)
    {
        MgLayer layer = null;
        for (int i = 0; i < map.GetLayers().GetCount(); i++)
        {
            MgLayer nextLayer = map.GetLayers().GetItem(i);
            if (nextLayer.GetName() == layerName)
            {
                layer = nextLayer;
                break;
            }

        }
        return layer;
    }

    //----------------------------------------------------------------------------------------
    // �� �ܣ� �ж���Դ�Ƿ����
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public bool IsDataSourceExists(MgResourceService resourceService, MgResourceIdentifier dataSourceId)
    {
        try
        {
            MgByteReader cnt = resourceService.GetResourceContent(dataSourceId);
            return true;
        }
        catch (MgResourceNotFoundException)
        {
            return false;
        }
    }

    //----------------------------------------------------------------------------------------
    // �� �ܣ� ����Ҫ��Դ
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------

    public void CreateFeatureSource(String sessionId)
    {
        MgFeatureService featureService = (MgFeatureService)siteConnection.CreateService(MgServiceType.FeatureService);

        // ΪҪ��Դ������ʱҪ����

        MgClassDefinition classDefinition = new MgClassDefinition();
        classDefinition.SetName("Points");
        classDefinition.SetDescription("��ʱҪ���ࣨ�㣩");
        classDefinition.SetDefaultGeometryPropertyName("GEOM");

        // ����Ҫ�����ʶ����
        MgDataPropertyDefinition identityProperty = new MgDataPropertyDefinition("KEY");
        identityProperty.SetDataType(MgPropertyType.Int32);
        identityProperty.SetAutoGeneration(true);
        identityProperty.SetReadOnly(true);

        classDefinition.GetIdentityProperties().Add(identityProperty);
        classDefinition.GetProperties().Add(identityProperty);

        // ������ͨ���ԣ�NAME)
        MgDataPropertyDefinition nameProperty = new MgDataPropertyDefinition("NAME");
        nameProperty.SetDataType(MgPropertyType.String);
        classDefinition.GetProperties().Add(nameProperty);

        // �����������ԣ�GEOM)
        MgGeometricPropertyDefinition geometryProperty = new MgGeometricPropertyDefinition("GEOM");
        geometryProperty.SetGeometryTypes(MgFeatureGeometricType.Point);
        classDefinition.GetProperties().Add(geometryProperty);

        // ����Ҫ��ģʽ
        MgFeatureSchema featureSchema = new MgFeatureSchema("PointSchema", "this is Point schema");
        featureSchema.GetClasses().Add(classDefinition);


        // ����Ҫ��Դ
        String featureSourceName = "Session:" + sessionId + "//Points.FeatureSource";
        MgResourceIdentifier resourceIdentifier = new MgResourceIdentifier(featureSourceName);

        String wkt = "LOCALCS[\"*XY-MT*\",LOCAL_DATUM[\"*X-Y*\",10000],UNIT[\"Meter\", 1],AXIS[\"X\",EAST],AXIS[\"Y\",NORTH]]";
        MgCreateSdfParams sdfParams = new MgCreateSdfParams("ArbitraryXY", wkt, featureSchema);

        featureService.CreateFeatureSource(resourceIdentifier, sdfParams);

    }

    //----------------------------------------------------------------------------------------
    // �� �ܣ� �ͷ�����
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    void ReleaseReader(MgPropertyCollection res, MgFeatureCommandCollection commands)
    {
        if (res == null)
            return;

        for (int i = 0; i < res.GetCount(); i++)
        {
            MgFeatureCommand cmd = commands.GetItem(i);
            if (cmd is MgInsertFeatures)
            {
                MgFeatureProperty resProp = res.GetItem(i) as MgFeatureProperty;
                if (resProp != null)
                {
                    MgFeatureReader reader = resProp.GetValue() as MgFeatureReader;
                    if (reader == null)
                        return;
                    reader.Close();
                }
            }
        }
    }

    //----------------------------------------------------------------------------------------
    // �� �ܣ� ���ڴ�������ݣ�x��y�������ռ��Ҫ������
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public MgPropertyCollection MakePoint(string name, double x, double y)
    {
        MgPropertyCollection propertyCollection = new MgPropertyCollection();
        MgStringProperty nameProperty = new MgStringProperty("NAME", name);
        propertyCollection.Add(nameProperty);

        MgWktReaderWriter wktReaderWriter = new MgWktReaderWriter();
        MgAgfReaderWriter agfReaderWriter = new MgAgfReaderWriter();

        MgGeometry geometry = wktReaderWriter.Read("POINT XY (" + x.ToString() + " " + y.ToString() + ")");
        MgByteReader geometryByteReader = agfReaderWriter.Write(geometry);
        MgGeometryProperty geometryProperty = new MgGeometryProperty("GEOM", geometryByteReader);
        propertyCollection.Add(geometryProperty);

        return propertyCollection;
    }


    //----------------------------------------------------------------------------------------
    // �� �ܣ� ��Ӳ㵽���飬������鲻���ڣ��򴴽�
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public MgLayer CreateLayerResource(MgResourceIdentifier layerResourceID, MgResourceService resourceService, string layerName, string layerLegendLabel, MgMap map)
    {
        MgLayer newLayer = new MgLayer(layerResourceID, resourceService);
        //  �����㲢��Ӽҵ���ͼ��
        newLayer.SetName(layerName);
        newLayer.SetVisible(true);				
        newLayer.SetLegendLabel(layerLegendLabel);
        newLayer.SetDisplayInLegend(true);
        MgLayerCollection layerCollection = map.GetLayers();
        if (!layerCollection.Contains(layerName))
        {
            // ��Insert��������㣬�´����Ĳ�λ�ڻ��ƴ�������
            layerCollection.Insert(0, newLayer);
        }
        newLayer.SetDisplayInLegend(true);
        return newLayer;
    }



    
    //----------------------------------------------------------------------------------------
    // �� �ܣ� ��Ӳ㵽���飬������鲻���ڣ��򴴽�
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public void AddLayerToGroup(MgLayer layer, string layerGroupName, string layerGroupLegendLabel, MgMap map)
    {

        // ��ȡ����
        MgLayerGroupCollection layerGroupCollection = map.GetLayerGroups();
        MgLayerGroup layerGroup = null;
        if (layerGroupCollection.Contains(layerGroupName))
        {
            layerGroup = layerGroupCollection.GetItem(layerGroupName);
        }
        else
        {
            // ���û�д��ڣ��򴴽�����
            layerGroup = new MgLayerGroup(layerGroupName);
            layerGroup.SetVisible(true);
            layerGroup.SetDisplayInLegend(true);
            layerGroup.SetLegendLabel(layerGroupLegendLabel);
            layerGroupCollection.Add(layerGroup);
        }
        layerGroup.SetDisplayInLegend(true);
        // ��Ӳ㵽����
        layer.SetGroup(layerGroup);

    }

    //----------------------------------------------------------------------------------------
    // �� �ܣ� ��ȡ��ͼ������ϵͳ
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public String GetMapSrs(MgMap map)
    {
        String srs = map.GetMapSRS();
        if (srs != "")
            return srs;

        return "LOCALCS[\"*XY-MT*\",LOCAL_DATUM[\"*X-Y*\",10000],UNIT[\"Meter\", 1],AXIS[\"X\",EAST],AXIS[\"Y\",NORTH]]";
    }

    public UtilityClass()
    {
    }
    //----------------------------------------------------------------------------------------
    // �� �ܣ� ������ʱ��
    //
    // �� �ߣ� 
    //
    //
    // �� �ڣ�2007.05.#
    //
    //-----------------------------------------------------------------------------------------
    public bool CreatePointsLayer(String rootPath, String sessionId)
    {
        // ��ȡҪ�ط������Դ����
        MgResourceService resourceService = (MgResourceService)siteConnection.CreateService(MgServiceType.ResourceService);
        MgFeatureService featureService = (MgFeatureService)siteConnection.CreateService(MgServiceType.FeatureService);

        //  �򿪵�ͼ
        MgMap map = new MgMap();
        map.Open(resourceService, "Sheboygan");
        // ---Ҫ�����Ҫ�ز������û��������������滹Ҫ���ܣ�--��ʼ
        // ���������ݵ�Ҫ��Դ
        CreateFeatureSource(sessionId);
        String featureSourceName = "Session:" + sessionId + "//Points.FeatureSource";
        MgResourceIdentifier resourceIdentifier = new MgResourceIdentifier(featureSourceName);

        MgBatchPropertyCollection batchPropertyCollection = new MgBatchPropertyCollection();
        MgWktReaderWriter wktReaderWriter = new MgWktReaderWriter();
        MgAgfReaderWriter agfReaderWriter = new MgAgfReaderWriter();
        MgGeometryFactory geometryFactory = new MgGeometryFactory();

        // �������¼
        MgPropertyCollection propertyCollection = MakePoint("Point A", -87.727, 43.748);
        batchPropertyCollection.Add(propertyCollection);

        propertyCollection = MakePoint("Point B", -87.728, 43.730);
        batchPropertyCollection.Add(propertyCollection);

        propertyCollection = MakePoint("Point C", -87.726, 43.750);
        batchPropertyCollection.Add(propertyCollection);

        propertyCollection = MakePoint("Point D", -87.728, 43.750);
        batchPropertyCollection.Add(propertyCollection);

        // ��������Ҫ��������ӵ�Ҫ��Դ
        MgInsertFeatures Insertcmd = new MgInsertFeatures("Points", batchPropertyCollection);
        MgFeatureCommandCollection featureCommandCollection = new MgFeatureCommandCollection();
        featureCommandCollection.Add(Insertcmd);
        featureService.UpdateFeatures(resourceIdentifier, featureCommandCollection, false);
        MgResourceIdentifier resourceID = null;
        //--------------Ҫ�����Ҫ�ز��� ����


        // �����㣬ͨ���㹤��LayerDefinitionFactory
        LayerDefinitionFactory factory = new LayerDefinitionFactory();
        factory.RootDirectoryPath = rootPath;

        //-------------------��������ʽ------------------------//
        // ������Ƿ���l
        string resourceSymbel = "Library://MgTutorial/Symbols/BasicSymbols.SymbolLibrary";
        string symbolName = "PushPin";
        string width = "24";  // unit = points
        string height = "24"; // unit = points
        string color = "FFFF0000";
        string markSymbol = factory.CreateMarkSymbol(resourceSymbel, symbolName, width, height, color);

        // �����ı�
        string text = "ID";
        string fontHeight = "12";
        string foregroundColor = "FF000000";
        string textSymbol = factory.CreateTextSymbol(text, fontHeight, foregroundColor);

        // ���������
        string legendLabel = "trees";
        string filter = "";
        string pointRule = factory.CreatePointRule(legendLabel, filter, textSymbol, markSymbol);

        // ��������ʽ
        string pointTypeStyle = factory.CreatePointTypeStyle(pointRule);

        // �������ŷ�Χ
        string minScale = "0";
        string maxScale = "1000000000000";
        string pointScaleRange = factory.CreateScaleRange(minScale, maxScale, pointTypeStyle);
        
        // �����㶨��
        string featureName = "PointSchema:Points";
        string geometry = "GEOM";
        string layerDefinition = factory.CreateLayerDefinition(featureSourceName, featureName, geometry, pointScaleRange);

        MgByteSource byteSource = new MgByteSource(Encoding.UTF8.GetBytes(layerDefinition), layerDefinition.Length);
        MgByteReader byteReader = byteSource.GetReader();


        resourceID = new MgResourceIdentifier("Session:" + sessionId + "//Points.LayerDefinition");
        resourceService.SetResource(resourceID, byteReader, null);
        //��������Ĳ㶨�����ݣ������ã�
        MgByteSink byteSink = new MgByteSink(resourceService.GetResourceContent(resourceID));
        string filePath = "C:\\Temp\\LayerDefinition.xml";
        byteSink.ToFile(filePath);

        // �����㲢��ӵ�����͵�ͼ��
        MgLayer newLayer = CreateLayerResource(resourceID, resourceService, "Points", "��ʱ��", map);

        AddLayerToGroup(newLayer, "Analysis", "����", map);

        MgLayerCollection layerCollection = map.GetLayers();
        if (layerCollection.Contains("Points"))
        {
            MgLayer pointsLayer = layerCollection.GetItem("Points");
            pointsLayer.SetVisible(true);
        }
        // �����ͼ
        map.Save(resourceService);

        return true;
    }




}
