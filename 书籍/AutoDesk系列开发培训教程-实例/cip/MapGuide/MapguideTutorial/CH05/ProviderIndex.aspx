<%@ Page Language="C#" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head id="Head1" runat="server">
    <title>Ҫ�ط���</title>
</head>
<body>
    <ul>
        <li><a href="FDOProviders.aspx" target="_blank"><span style="color: #000000"></span>
            ��ȡFDO Providers</a></li><li>��ȡFDO ����
        <ul>
            <li><a href="FDOCapabilities.aspx?provider=SHP" target="_blank">OSGeo.SHP.3.1</a></li>
            <li><a href="FDOCapabilities.aspx?provider=SDF" target="_blank">OSGeo.SDF.3.1</a></li>
            <li><a href="FDOCapabilities.aspx?provider=ODBC" target="_blank">OSGeo.ODBC.3.1</a></li>
            <li><a href="FDOCapabilities.aspx?provider=MYSQL" target="_blank">OSGeo.MySQL.3.1</a></li>
            <li><a href="FDOCapabilities.aspx?provider=ARCSDE" target="_blank">OSGeo.ArcSDE.3.1</a></li>
            <li><a href="FDOCapabilities.aspx?provider=WMS" target="_blank">OSGeo.WMS.3.1</a></li>
            <li><a href="FDOCapabilities.aspx?provider=WFS" target="_blank">OSGeo.WFS.3.1</a></li>
            <li><a href="FDOCapabilities.aspx?provider=SQLSERVER" target="_blank">Autodesk.SqlServer.3.1</a></li>
            <li><a href="FDOCapabilities.aspx?provider=ORACLE" target="_blank">Autodesk.Oracle.3.1</a></li>
            <li><a href="FDOCapabilities.aspx?provider=RASTER" target="_blank">Autodesk.Raster.3.1</a></li>
        </ul>
        </li>
        <li>��ȡҪ��ģʽ<ul>
            <li><a href="GetFeatureSchema.aspx?source=city" target="_blank">City Limits</a></li>
            <li><a href="GetFeatureSchema.aspx?source=parcel" target="_blank">Parcels</a></li>
        </ul>
        </li>
        <li></li>
    </ul>
</body>
</html>
