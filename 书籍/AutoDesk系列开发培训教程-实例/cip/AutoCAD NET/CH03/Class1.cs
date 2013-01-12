using System;
using System.Collections.Generic;
using System.Text;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;

namespace CH03
{
    public class Class1
    {

        //--------------------------------------------------------------
        // ����:����һ���²�
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //----------------------------------------------------------------

        [CommandMethod("CreateLayer")]
        public void CreateLayer()
        {
            ObjectId layerId;
            Database db = HostApplicationServices.WorkingDatabase;
            //��ʼһ������
            Transaction trans = db.TransactionManager.StartTransaction();
            try
            {
                //����ȡ�ò��
                LayerTable lt = (LayerTable)trans.GetObject(db.LayerTableId, OpenMode.ForWrite);
                //���MyLayer���Ƿ����
                if (lt.Has("MyLayer"))
                {
                    layerId = lt["MyLayer"];
                }
                else
                {
                    //���MyLayer�㲻���ڣ��ʹ�����
                    LayerTableRecord ltr = new LayerTableRecord();
                    ltr.Name = "MyLayer"; //���ò������
                    layerId = lt.Add(ltr);
                    trans.AddNewlyCreatedDBObject(ltr, true);
                }
                //�ύ����
                trans.Commit();
            }
            catch (Autodesk.AutoCAD.Runtime.Exception e)
            {
                //��������
                trans.Abort();
            }
            finally
            {
                // ��ʽ���ͷ�
                trans.Dispose();
            }
        }

        //--------------------------------------------------------------
        // ����:����һ��Բ
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //----------------------------------------------------------------
        [CommandMethod("CreateCircle")]
        public void  CreateCircle()
        {
            Database db = HostApplicationServices.WorkingDatabase;
            // ʹ�� "using" ���������Զ���������� "Dispose" 
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                //��ȡ����ģ�Ϳռ�
                BlockTable bt = (BlockTable)(trans.GetObject(db.BlockTableId, OpenMode.ForRead));
                BlockTableRecord btr = (BlockTableRecord)trans.GetObject(bt[BlockTableRecord.ModelSpace], OpenMode.ForWrite);

                //����һ��Բ����ӵ�����¼��ģ�Ϳռ䣩
                Point3d center = new Point3d(10, 10, 0);
                Circle circle = new Circle(center, Vector3d.ZAxis, 10.0);
                circle.ColorIndex = 1;

                btr.AppendEntity(circle);
                trans.AddNewlyCreatedDBObject(circle, true);
                trans.Commit();
            }

        }

        //--------------------------------------------------------------
        // ����:����һ���鶨�壨����¼��
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //   
        //----------------------------------------------------------------
        public ObjectId CreateBlkDef()
        {
            //���庯���ķ���ֵObjectId
            ObjectId blkObjId = new ObjectId(); 
            Database db = HostApplicationServices.WorkingDatabase; 

            // ʹ�� "using"�ؼ���ָ������ı߽�
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                //��ȡ���
                BlockTable bt = (BlockTable)trans.GetObject(db.BlockTableId, OpenMode.ForWrite);
                //ͨ������myBlkName�жϿ�����Ƿ��������¼
                if ((bt.Has("myBlkName")))
                {
                    blkObjId = bt["myBlkName"];//����Ѿ����ڣ�ͨ��������ȡ���Ӧ��ObjectId
                }
                else
                {
                    //����һ��Բ
                    Point3d center = new Point3d(10, 10, 0); 
                    Circle circle = new Circle(center, Vector3d.ZAxis, 2);
                    circle.ColorIndex = 1;      
                    //�����ı�Text:
                    MText text = new MText();
                    text.Contents = " ";
                    text.Location = center;
                    text.ColorIndex = 2;

                    //�����µĿ���¼ myBlkName
                    BlockTableRecord newBtr = new BlockTableRecord();
                    newBtr.Name = "myBlkName";
                    newBtr.Origin = center;
                    //�������¼�����
                    blkObjId = bt.Add(newBtr); // ���ؿ��Ӧ��ObjectId
                    trans.AddNewlyCreatedDBObject(newBtr, true); //Let the transaction know about any object/entity you add to the database!
                   
                    //�����´�����ʵ�嵽����¼
                    newBtr.AppendEntity(circle); 
                    newBtr.AppendEntity(text);
                    // ֪ͨ�����´����˶���
                    trans.AddNewlyCreatedDBObject(circle, true);
                    trans.AddNewlyCreatedDBObject(text, true);
                }
                trans.Commit(); //�ύ����
            }
            return blkObjId;
        }


        //--------------------------------------------------------------
        // ����:����һ���²�
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //----------------------------------------------------------------
        [CommandMethod("CreateBlk")]
        public void CreateBlkRef()
        {
            
             
            //��ȡ��Ĳ����
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            PromptPointOptions ptOps = new PromptPointOptions("ѡ���Ĳ����");
            PromptPointResult ptRes;
            ptRes = ed.GetPoint(ptOps);
            Point3d ptInsert;
            if (ptRes.Status == PromptStatus.OK)
            {
                ptInsert = ptRes.Value ;
            }
            else
            {
                ptInsert = new Point3d(0, 0, 0);
            }

            Database db = HostApplicationServices.WorkingDatabase;
            // ʹ�� "using"�ؼ���ָ������ı߽�
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                //��ȡ����ģ�Ϳռ�
                BlockTable bt = (BlockTable)(trans.GetObject(db.BlockTableId, OpenMode.ForWrite));
                BlockTableRecord btr = (BlockTableRecord)trans.GetObject(bt[BlockTableRecord.ModelSpace], OpenMode.ForWrite);
                
                //����������
                BlockReference blkRef = new BlockReference(ptInsert,CreateBlkDef());// ָ�������������õĿ���¼
                blkRef.Rotation = 1.57;//ָ����ת�ǣ�������

                //�����´����Ŀ����õ�ģ�Ϳռ�    
                btr.AppendEntity(blkRef); 
                trans.AddNewlyCreatedDBObject(blkRef, true);    // ֪ͨ�����´����˶���

                trans.Commit(); //�ύ����
            }
  
        }

        //--------------------------------------------------------------
        // ����:����һ���²�
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //----------------------------------------------------------------
        [CommandMethod("OpenEnt")]
        public void OpenEnt()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
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
                Entity ent = trans.GetObject(objId, OpenMode.ForWrite);
                ent.ColorIndex = 1;
                trans.Commit();
            }

        }
        ///////////////////////////////////////////////////
    }
}
