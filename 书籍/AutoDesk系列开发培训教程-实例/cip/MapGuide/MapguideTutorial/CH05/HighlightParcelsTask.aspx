<%@ Page Language="C#" %>

<%
    String sessionID = Request.Form.Get("SESSION");
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <meta http-equiv="content-script-type" content="text/javascript" />
    <meta http-equiv="content-style-type" content="text/css" />
    <link href="styles/globalstyles.css" rel="stylesheet" type="text/css"/>
    <link href="styles/alphastyles.css" rel="stylesheet" type="text/css"/>
    <title>Hightlight Parcels</title>
</head>
<body class="AppFrame">
    <h1 class="AppHeading">
        ������ʾ��ѯ����Ҫ��</h1>
    <form action="HighlightParcels.aspx" method="post">
        <input name="SESSION" type="hidden" value="<%= sessionID %>"/>
        <label>
            �����ѯ������
        </label>
        <br/>
        <input name="QuertyString" type="text" size="30"/>
        <br/>
        <br/>
        <input type="submit" value="��ѯ"/>
    </form>
    <br/>
    <br/>
    <label>
        <strong>����: RNAME LIKE 'Schmitt%'</strong></label>
</body>
</html>

