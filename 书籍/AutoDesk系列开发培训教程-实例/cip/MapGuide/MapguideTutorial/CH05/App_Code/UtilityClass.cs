using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Collections.Specialized;
using OSGeo.MapGuide;
using System.Collections;

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
    MgSiteConnection siteConnection;
    String selectionResult;
    String selectionXML;

    public String SelectionResult
    {
        get { return selectionResult; }
        set { selectionResult = value; }
    }

    public String SelectionXML
    {
        get { return selectionXML; }
        set { selectionXML = value; }
    }

    public NameValueCollection GetParameters(HttpRequest Request)
    {
        return Request.HttpMethod == "GET" ? Request.QueryString : Request.Form;
    }

    public void InitializeWebTier(HttpRequest Request)
    {
        string realPath = Request.ServerVariables["APPL_PHYSICAL_PATH"];
        String configPath = realPath + "webconfig.ini";
        MapGuideApi.MgInitializeWebTier(configPath);
    }

    public void ConnectToServer(String sessionID)
    {
        MgUserInformation userInfo = new MgUserInformation(sessionID);
        siteConnection = new MgSiteConnection();
        siteConnection.Open(userInfo);
    }

    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ�����ѯ���ת��ΪXML��ʽ���ַ�
    //
    //         ���ߣ� 
    //
    //         ���ڣ� 2007.5.23
    //          
    //         �޸���ʷ���� 
    //          
    //--------------------------------------------------------------------------------------- 
    public void CreateSelectionXML(String queryString)
    {
        //������Դ��Ҫ�ط���
        MgResourceService resService = (MgResourceService)siteConnection.CreateService(MgServiceType.ResourceService);
        MgFeatureService featureService = (MgFeatureService)siteConnection.CreateService(MgServiceType.FeatureService);
        //��ȡҪ��ѯ��Ҫ��������ڵĲ�
        MgMap map = new MgMap();
        map.Open(resService, "Sheboygan");
        MgLayer layer = map.GetLayers().GetItem("Parcels");
        //ִ��Ҫ�ز�ѯ
        MgResourceIdentifier resId = new MgResourceIdentifier(layer.GetFeatureSourceId());
        MgFeatureQueryOptions queryOption = new MgFeatureQueryOptions();
        queryOption.SetFilter(queryString);
        MgFeatureReader featureReader = featureService.SelectFeatures(resId, "Parcels", queryOption);
          //����ѡ��
        MgSelection selection = new MgSelection(map);
        selection.AddFeatures(layer, featureReader, 0);
        //����ѡ�񼯣���ת��ΪXML
        OutputSelectionInHTML(selection, featureService);
        selectionXML = selection.ToXml();
    }
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ������ѯ���
    //
    //         ���ߣ� 
    //
    //         ���ڣ� 2007.5.23
    //          
    //         �޸���ʷ���� 
    //          
    //--------------------------------------------------------------------------------------- 
    public void OutputSelectionInHTML(MgSelection selection, MgFeatureService featureService)
    {
        //���ر�ѡ�еĲ�
        MgReadOnlyLayerCollection layers = selection.GetLayers();
        String outString = null;
        MgFeatureReader featReader = null;
        if (layers != null)
        {
            //��ÿһ���㴦��
            for (int i = 0; i < layers.GetCount(); i++)
            {
                MgLayer layer = layers.GetItem(i);
                if ((layer != null) && (layer.GetName() == "Parcels"))
                {
                    //����ѡ�е�Ҫ�����
                    String layerClassName = layer.GetFeatureClassName();
                    // ����ѡ�񼯹�������
                    String selectString = selection.GenerateFilter(layer, layerClassName);
                    String layerFeatureIdString = layer.GetFeatureSourceId();
                    MgResourceIdentifier layerResId = new MgResourceIdentifier(layerFeatureIdString);
                    MgFeatureQueryOptions queryOptions = new MgFeatureQueryOptions();
                    queryOptions.SetFilter(selectString);
                    // Ϊѡ�е�Ҫ�ش���MgFeatureReader����
                    featReader = featureService.SelectFeatures(layerResId, layerClassName, queryOptions);

                    ///����MgFeatureReader ����, ��ȡÿ��ѡ�е�Ҫ��
                   outString = "\n";
                    outString = outString + "<table border=\"1\">\n";

                    double acre = 0;
                    // 
                    outString = outString + "<tr>\n";

                    outString = outString + "<td>RNAME</td>\n";
                    outString = outString + "<td>RPROPAD</td>\n";
                    outString = outString + "<td>RACRE</td>\n";
                    outString = outString + "</tr>\n";
                    while (featReader.ReadNext())
                    {
                        outString = outString + "<tr>\n";
                        outString = outString + "<td>";
                        outString = outString + featReader.GetString("RNAME");
                        outString = outString + "</td>\n";
                        outString = outString + "<td>";
                        outString = outString + featReader.GetString("RPROPAD");
                        outString = outString + "</td>\n";
                        outString = outString + "<td>";
                        String acreString = featReader.GetString("RACRE");
                        acre = acre + (acreString == "" ? 0 : Convert.ToDouble(acreString));
                        outString = outString + acreString;
                        outString = outString + "</tr>\n";
                    }
                    outString = outString + "</table>\n";
                }
            }

        }
        selectionResult = outString;
    }
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ��г���ǰѡ���Ҫ������
    //
    //         ���ߣ� 
    //
    //         ���ڣ� 2007.5.23
    //          
    //         �޸���ʷ���� 
    //          
    //--------------------------------------------------------------------------------------- 
    public int ListSelections()
    {
        // ������Դ�����Ҫ�ط���
        MgResourceService resService = (MgResourceService)siteConnection.CreateService(MgServiceType.ResourceService);
        MgFeatureService featService = (MgFeatureService)siteConnection.CreateService(MgServiceType.FeatureService);

        //�򿪵�ͼ���󲢴���ѡ�񼯶���
        MgMap map = new MgMap();
        map.Open(resService, "Sheboygan");
        MgSelection mapSelection = null;
        mapSelection = new MgSelection(map);
        mapSelection.Open(resService, "Sheboygan");
        // ����ѡ��
        if (mapSelection.GetLayers() != null)
        {
            OutputSelectionInHTML(mapSelection, featService);
            return 1;
        }
        else
        {
            return 0;
        }
    }
    //---------------------------------------------------------------------------------------
    //
    //        ���ܣ���ѯҪ��
    //
    //         ���ߣ� 
    //
    //         ���ڣ� 2007.5.23
    //          
    //         �޸���ʷ���� 
    //          
    //--------------------------------------------------------------------------------------- 
    public ArrayList MakeQuery(int districtNum, String name)
    {
        ArrayList list = null;
        //��ȡ��Դ�����Ҫ�ط���
        MgResourceService resService = (MgResourceService)siteConnection.CreateService(MgServiceType.ResourceService);
        MgFeatureService featureService = (MgFeatureService)siteConnection.CreateService(MgServiceType.FeatureService);

        MgMap map = new MgMap();
        map.Open(resService, "Sheboygan");
        // ָ����ѯ�Ĺ�������
        MgFeatureQueryOptions districtQuery = new MgFeatureQueryOptions();
        districtQuery.SetFilter("Autogenerated_SDF_ID = " + districtNum);
        ////ָ����ѯ��Ҫ��Դ����Դ��ʶ����ͶƱѡ��VotingDistricts��
        MgResourceIdentifier districtId = new MgResourceIdentifier("Library://MgTutorial/Data/VotingDistricts.FeatureSource");
        MgFeatureReader featureReader = featureService.SelectFeatures(districtId, "VotingDistricts", districtQuery);
        // �����ѯ�����ͶƱѡ��VotingDistricts��
        featureReader.ReadNext();
        MgByteReader geometryBytes = featureReader.GetGeometry("Data");
        MgAgfReaderWriter agfReader = new MgAgfReaderWriter();
        MgGeometry districtGeometry = agfReader.Read(geometryBytes);

        // ָ����ѯ�Ĺ�����������ָ��ͶƱѡ��VotingDistricts��Χ�ڲ�ѯ
        MgFeatureQueryOptions nameQuery = new MgFeatureQueryOptions();
        nameQuery.SetFilter("RNAME LIKE '" + name + "%'");
        nameQuery.SetSpatialFilter("SHPGEOM", districtGeometry, MgFeatureSpatialOperations.Inside);

        ////ָ����ѯ��Ҫ��Դ����Դ��ʶ�����ؿ�Parcels��
        MgResourceIdentifier parcelId =new MgResourceIdentifier("Library://MgTutorial/Data/Parcels.FeatureSource");
        featureReader = featureService.SelectFeatures(parcelId, "Parcels", nameQuery);
        // �����ѯ���
        list = new ArrayList();
        while (featureReader.ReadNext())
        {
            // ��ȡRPROPAD���Զζ�Ӧ�����ݣ�String���ͣ�
            list.Add(featureReader.GetString("RPROPAD"));
        }

        String schema = featureService.DescribeSchemaAsXml(parcelId, "");

        return list;
    }


      //---------------------------------------------------------------------------------------
    //
    //        ���ܣ�GetPropertyType()������GetPropertyName()������ʹ��
    //
    //         ���ߣ� 
    //
    //         ���ڣ� 2007.5.23
    //          
    //         �޸���ʷ���� 
    //          
    //--------------------------------------------------------------------------------------- 
       public void OutputReader(MgFeatureReader featureReader)
       {
           while (featureReader.ReadNext())
           {
               int propCount = featureReader.GetPropertyCount();
               for (int j = 0; j < propCount; j++)
               {
                   string propertyName = featureReader.GetPropertyName(j);
                   bool boolVal = featureReader.IsNull(propertyName);
                   if (boolVal)
                   {
                       continue;
                   }
                   //MgPropertyType propertyType = (MgPropertyType)featureReader.GetPropertyType(propertyName);
                   int proType = featureReader.GetPropertyType(propertyName);
                   GetPropertyValue(featureReader, proType, propertyName);
               }
           }

       }

     
       //---------------------------------------------------------------------------------------
       //
       //        ���ܣ�δ֪��ѯ����е��������ݵ����͵Ĵ�����
       //
       //
       //         ���ߣ� 
       //
       //         ���ڣ� 2007.5.23
       //          
       //         �޸���ʷ���� 
       //          
       //---------------------------------------------------------------------------------------   
    
    public void GetPropertyValue(MgFeatureReader featureReader, int propertyType, string propertyName)
       {
           switch (propertyType )
           {
               case MgPropertyType.Null:
                   break;
               case MgPropertyType.Boolean:
                   bool val1 = featureReader.GetBoolean(propertyName);
                   break;
               case MgPropertyType.Byte:
                   Byte val2 = featureReader.GetByte(propertyName);
                   break;
               case MgPropertyType.DateTime:
                   MgDateTime val3 = featureReader.GetDateTime(propertyName);
                   break;
               case MgPropertyType.Single:
                   float val4 = featureReader.GetSingle(propertyName);
                   break;
               case MgPropertyType.Double:
                   double val5 = featureReader.GetDouble(propertyName);
                   break;
               case MgPropertyType.Int16:
                   Int16 val6 = featureReader.GetInt16(propertyName);
                   break;
               case MgPropertyType.Int32:
                   Int32 val7 = featureReader.GetInt32(propertyName);
                   break;
               case MgPropertyType.Int64:
                   Int64 val8 = featureReader.GetInt64(propertyName);
                   break;
               case MgPropertyType.String:
                   string val9 = featureReader.GetString(propertyName);
                   break;
               case MgPropertyType.Blob:
                   string val10 = propertyName + " ��Blob����";
                   break;
               case MgPropertyType.Clob:
                   string val11 = propertyName + " ��Clob����";
                   break;
               case MgPropertyType.Feature:
                   MgFeatureReader val12 = featureReader.GetFeatureObject(propertyName);
                   break;
               case MgPropertyType.Geometry:
                   MgByteReader va13l = featureReader.GetGeometry(propertyName);
                   MgAgfReaderWriter agfReader = new MgAgfReaderWriter();
                   MgGeometry districtGeometry = agfReader.Read(va13l);
                   break;
               case MgPropertyType.Raster:
                   MgRaster val14 = featureReader.GetRaster(propertyName) ;
                   break;
               default:
                   string val13 =  "δ֪����";
                   break;
           }
       }



}
