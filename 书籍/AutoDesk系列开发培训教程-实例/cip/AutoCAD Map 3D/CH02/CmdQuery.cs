//
// (C) Copyright 2004-2007 by Autodesk, Inc.
//
//
// By using this code, you are agreeing to the terms
// and conditions of the License Agreement that appeared
// and was accepted upon download or installation
// (or in connection with the download or installation)
// of the Autodesk software in which this code is included.
// All permissions on use of this code are as set forth
// in such License Agreement provided that the above copyright
// notice appears in all authorized copies and that both that
// copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC.
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//


using System;
using System.Collections.Generic;
using System.Text;

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Runtime;

using Autodesk.Gis.Map;
using Autodesk.Gis.Map.Utilities;
using Autodesk.Gis.Map.Project;
using Autodesk.Gis.Map.Query;
using Autodesk.Gis.Map.Constants;



namespace CH02
{
    public class CmdQuery
    {
        //--------------------------------------------------------------------------
        //
        // ���ܣ�ִ�в�ѯ
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        [CommandMethod("RunQuery")]
        public void RunQuery()
        {
            // ��ȡ��ͼӦ�ó������͹��̶���
            MapApplication mapApi = HostMapApplicationServices.Application;
            ProjectModel proj = mapApi.ActiveProject;
            DrawingSet drawingSet = proj.DrawingSet;

           //������ͼ�μ��߽�
            proj.DrawingSet.ZoomExtents();

          //ʹ��LocationCondition������һ����ѯ����
            LocationCondition qryCondition  =   new LocationCondition ();
            qryCondition.LocationType  = LocationType.LocationInside ;
            qryCondition.JoinOperator = JoinOperator .OperatorAnd ;
           // ����Բ�߽�
            CircleBoundary boundary = null;

            PromptPointResult pointRes = Utility.AcadEditor.GetPoint("\nѡ��ԭ��: ");
            if (pointRes.Status == PromptStatus.OK)
            {
                Point3d point = pointRes.Value;
                PromptDistanceOptions distOptions = new PromptDistanceOptions("\nѡ��뾶: ");
                distOptions.BasePoint = point;
                distOptions.UseBasePoint = true;
                distOptions.DefaultValue = 0.0;
                PromptDoubleResult doubleRes = Utility.AcadEditor.GetDistance(distOptions);
                if (doubleRes.Status == PromptStatus.OK)
                {
                    double rad = doubleRes.Value;
                    boundary = new CircleBoundary(new Point3d(point.X, point.Y, 0.0), rad);
                }
            }
            //
            qryCondition.Boundary = boundary;
            //����һ��������֧
             QueryBranch qryBranch  =  QueryBranch.Create ();
            qryBranch.JoinOperator = JoinOperator.OperatorAnd;
            qryBranch.AppendOperand(qryCondition);
             ////ʹ��PropertyCondition������һ�����Բ�ѯ����(LWPOLYLINE)
            PropertyCondition propCondition= new PropertyCondition();
             //JoinOperator.OperatorAnd, PropertyType.EntityType, ConditionOperator.ConditionEqual, "LWPOLYLINE";
            propCondition.JoinOperator = JoinOperator.OperatorAnd ;
            propCondition.PropertyType  = PropertyType.EntityType ;
            propCondition.ConditionOperator = ConditionOperator.ConditionEqual ;
            propCondition.Value = "LWPOLYLINE";

            //	ͨ��QueryBranch�Ĺ��캯��������һ��������ѯ��֧��
            QueryBranch  subBranch  = new QueryBranch(JoinOperator.OperatorAnd);
             //	����һ�����в�ѯ�����ͷ�֧�Ĳ�ѯ����
            subBranch.AppendOperand(propCondition);
            qryBranch.AppendOperand(subBranch);
            //  ����һ����ѯ��
            QueryModel qryModel  = proj.CreateQuery (true);
            //	��ɲ�ѯ�Ķ��塣
            qryModel.Define (qryBranch);
            //���û���ģʽ
            qryModel.Mode = QueryType.QueryDraw;
            //�����ѯ
            qryModel.Define(qryBranch);
            //ִ�в�ѯ��
            qryModel.Run();

            Utility.AcadEditor.WriteMessage("\n ��ɲ�ѯ ��");
   }

   //--------------------------------------------------------------------------
   //
   // ���ܣ������ѯ������
   //
   //  ���ߣ� 
   //
   //  ���ڣ�200708
   //
   //   ��ʷ��
   //--------------------------------------------------------------------------
        [CommandMethod("PrintQuery")]
        public void PrintQueryLibrary()
        {
            MapApplication mapApi = HostMapApplicationServices.Application;
            ProjectModel proj = mapApi.ActiveProject;
            QueryLibrary qryLib = proj.QueryCategories;

            if (qryLib.CategoryCount <= 0)
                return;

            Utility.AcadEditor.WriteMessage("\n***** ��ѯ�� *****");
            for (int i = 0; i < qryLib.CategoryCount; i++)
            {
                QueryCategory qryCat = qryLib[i];
                Utility.AcadEditor.WriteMessage(string.Format("\n*���: {0}", qryCat.Name));

                for (int j = 0; j < qryCat.QueryCount; j++)
                {
                    QueryAttribute qryAttr = qryCat[j];
                    Utility.AcadEditor.WriteMessage(string.Format("\n   ��ѯ: {0} - {1}",
                        qryAttr.Name, qryAttr.Description));
                }
            }
        }
    }
}
