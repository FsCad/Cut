using System;
using System.Collections.Generic;
using System.Text;

using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;
namespace CH04
{
    public class Class1
    {


        //--------------------------------------------------------------
        // ����:ͨ��ObjectId�򿪶���
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //----------------------------------------------------------------
        [CommandMethod("OpenEnt")]
        public void OpenEnt()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ed.WriteMessage("ͨ��ObjectId�򿪶���\n");
            PromptEntityOptions entOps = new PromptEntityOptions("ѡ��Ҫ�򿪵Ķ���\n");
            PromptEntityResult entRes;
            entRes = ed.GetEntity(entOps);
            if (entRes.Status != PromptStatus.OK)
            {
                ed.WriteMessage("ѡ�����ʧ�ܣ��˳�");
                return;
            }
            ObjectId objId = entRes.ObjectId;
            Database db = HostApplicationServices.WorkingDatabase;
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                Entity ent = trans.GetObject(objId, OpenMode.ForWrite) as Entity ;
                ent.ColorIndex = 1;
                trans.Commit();
            }
 
        }

        //--------------------------------------------------------------
        // ����:����ʶ���ת��
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //----------------------------------------------------------------
        [CommandMethod("GetType")]
        public void GetType()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ed.WriteMessage("���ݿ���������ʶ���ת��\n");

            PromptEntityOptions entOps = new PromptEntityOptions("ѡ��Ҫ�򿪵Ķ���");
            PromptEntityResult entRes;
            entRes = ed.GetEntity(entOps);
            if (entRes.Status != PromptStatus.OK)
            {
                ed.WriteMessage("ѡ�����ʧ�ܣ��˳�");
                return;
            }
            ObjectId objId = entRes.ObjectId;
            Database db = HostApplicationServices.WorkingDatabase;
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                Entity ent = trans.GetObject(objId, OpenMode.ForWrite) as Entity;
                ed.WriteMessage("ent.GetRXClass().Name :" + ent.GetRXClass().Name + "\n");

                if (ent is Line)
                {
                    Line aLine = ent as Line;
                    aLine.ColorIndex = 1;
                }
                else if (ent.GetType() == typeof(Circle))
                {
                    Circle cir = (Circle)ent;
                    cir.ColorIndex = 2;
                }

                trans.Commit();
            }
        }
        //--------------------------------------------------------------
        // ����:ʵ����������
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //----------------------------------------------------------------
        [CommandMethod("EntPro")]
        public void EntPro()
        {
            
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ed.WriteMessage("ʵ����������\n");

            PromptEntityOptions entOps = new PromptEntityOptions("ѡ��Ҫ�򿪵Ķ���\n");
            PromptEntityResult entRes;
            entRes = ed.GetEntity(entOps);
            if (entRes.Status != PromptStatus.OK)
            {
                ed.WriteMessage("ѡ�����ʧ�ܣ��˳�\n");
                return;
            }
            ObjectId objId = entRes.ObjectId;
            Database db = HostApplicationServices.WorkingDatabase;
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                Entity ent = trans.GetObject(objId, OpenMode.ForWrite) as Entity;
                ed.WriteMessage("��ȡ������ʵ�������\n");
                ed.WriteMessage("ʵ���ԭ�ȵ�����Ϊ :" + ent.Linetype + "\n");
                // ��ȡ���ͱ��¼
                LinetypeTable lineTypeTbl = trans.GetObject(db.LinetypeTableId, OpenMode.ForRead) as LinetypeTable;
                // ȷ��DOT�������Ѿ����ص���ǰ���ݿ�
                LinetypeTableRecord lineTypeTblRec = trans.GetObject(lineTypeTbl["DOT"], OpenMode.ForRead) as LinetypeTableRecord;
                // ����ʵ�������
                ent.LinetypeId = lineTypeTblRec.ObjectId;

                // ����ʵ������ͱ���
                ed.WriteMessage("����ʵ������ͱ���Ϊ2.0\n");
                ent.LinetypeScale = 2.0;

                //����ʵ��Ŀɼ���
                ent.Visible = true;

                 //����ʵ�����ڵĲ�
                ed.WriteMessage("ʵ���ԭ�����ڵĲ�Ϊ :" + ent.Layer + "\n");
                ent.Layer = "layer0";
                trans.Commit();
            }
        }



        ///////////////////////////
    }
}
