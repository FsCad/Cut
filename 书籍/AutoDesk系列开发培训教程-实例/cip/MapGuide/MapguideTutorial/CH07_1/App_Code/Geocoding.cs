using System;
using System.Data;
using System.Net;
using System.IO;
using System.Configuration;
using System.Web;
using System.Xml;

//---------------------------------------------------------------------------------------
//
//        ���ܣ������������
//
//         ���ߣ� 
//
//         ���ڣ� 2007.5.23
//          
//         �޸���ʷ���� 
//          
//--------------------------------------------------------------------------------------- 
public class Geocoding
{
    public GeocodeAddress RequestGeocodeAddress(String address)
    {
        //�����������geocoder��
        String urlString = "http://geocoder.us/service/rest/geocode?address=" + HttpUtility.UrlEncode(address);
        HttpWebRequest getRequest = (HttpWebRequest)WebRequest.Create(urlString);
        getRequest.Method = "GET";
        WebResponse response = null;

        response = getRequest.GetResponse();
        Stream responseStream = response.GetResponseStream();

        GeocodeAddress addr = new GeocodeAddress();
   
        XmlDocument doc = new XmlDocument();
        doc.Load(responseStream);

        addr.lon = doc.GetElementsByTagName("geo:long").Item(0).FirstChild.Value;
        addr.lat = doc.GetElementsByTagName("geo:lat").Item(0).FirstChild.Value;

        address = doc.GetElementsByTagName("dc:description").Item(0).FirstChild.Value;
        int sep = address.IndexOf(',');
        if (sep != -1)
        {
            addr.address1 = address.Substring(0, sep);
            addr.address2 = address.Substring(sep + 1);
        }
        else
        {
            addr.address1 = address;
        }

   
        return addr;
    }
    
}
//---------------------------------------------------------------------------------------
//
//        ���ܣ���ַ��Ϣ��
//
//         ���ߣ� 
//
//         ���ڣ� 2007.5.23
//          
//         �޸���ʷ���� 
//          
//--------------------------------------------------------------------------------------- 
public class GeocodeAddress
{
    public GeocodeAddress()
    {
        address1 = "";
        address2 = "";
        lat = "";
        lon = "";
    }

    public String address1;
    public String address2;
    public String lat;
    public String lon;
}
