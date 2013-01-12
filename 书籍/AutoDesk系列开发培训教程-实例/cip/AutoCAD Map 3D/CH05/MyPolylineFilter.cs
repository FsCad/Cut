using System;
using System.Collections.Generic;
using System.Text;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.DatabaseServices;

using Autodesk.Gis.Map.Filters;

namespace CH05
{

    public  class MyPolylineFilter : ObjectFilter
    {

        // ��������ߵ���С���Ⱥ���󳤶ȣ��������˵�����
        private double m_MinLen;
        private double m_MaxLen;

        public double MinLen
        {
            get
            {
                return m_MinLen;
            }
            set
            {

                m_MinLen = value;
            }
        }

        public double MaxLen
        {
            get
            {
                return m_MaxLen;
            }
            set
            {
                m_MaxLen = value;
            }
        }


        //--------------------------------------------------------------------------
        //
        // ���ܣ������Զ������
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        public virtual void FilterObjects(ObjectIdCollection outputIds,ObjectIdCollection inputIds)
        {

            // �������д����objectID 
            Transaction trans = HostApplicationServices.WorkingDatabase.TransactionManager.StartTransaction();
            try
            {
                ObjectId objId;
                foreach (ObjectId tempLoopVar_objId in inputIds)
                {
                    objId = tempLoopVar_objId;
                    Entity ent;
                    //�򿪶���
                    ent = trans.GetObject(objId, OpenMode.ForRead )as Entity ;
                    // �����ж�
                    if (ent is Polyline)
                    {
                        Polyline pl = ent as Polyline;
                        // ��������ߵĳ��ȣ�Ȼ���ж��䳤���Ƿ���ָ����������С����֮��
                        double Len = GetPolylineLength(pl);
                        // ������������򽫶���id���棬������
                        if (Len >= m_MinLen && Len <= m_MaxLen)
                        {
                            outputIds.Add(objId);
                        }
                    }
                }
                trans.Commit();
            }
            catch (Autodesk.AutoCAD.Runtime.Exception e)
            {
                Utility.AcadEditor.WriteMessage(e.Message);
            }


        }


        //--------------------------------------------------------------------------
        //
        // ���ܣ���������ߵĳ���
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        private double GetPolylineLength(Polyline polyline)
        {
            return polyline.GetDistanceAtParameter(polyline.EndParam);
        }

    }
}
