using System;
using System.Collections.Generic;
using System.Text;

using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Colors;

using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.ApplicationServices;

using Autodesk.AutoCAD.Windows;

namespace CH07
{
    public class Class1
    {
        //ȫ�ֱ���
        bool bCmdActive;
        bool bReposition;
        ObjectIdCollection blkObjIDs = new ObjectIdCollection();
        Point3dCollection blkPositions = new Point3dCollection();

        [CommandMethod("AddEvents")]
        public void AddEvents()
        {
            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            try
            {
                Database db;
                Document doc;
                //
                doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
                db = HostApplicationServices.WorkingDatabase;
                db.ObjectOpenedForModify += new ObjectEventHandler(objOpenedForMod);
                doc.CommandWillStart += new CommandEventHandler(cmdWillStart);
                doc.CommandEnded += new CommandEventHandler(cmdEnded);
                bCmdActive = false;
                bReposition = false;
            }
            catch
            {
                ed.WriteMessage("����¼�����\n");
            }
        }

        [CommandMethod("RemoveEvents")]
        public void removeDbEvents()
        {
            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            try
            {
                Database db;
                Document doc;
                doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
                db = HostApplicationServices.WorkingDatabase;
                db.ObjectOpenedForModify -= new ObjectEventHandler(objOpenedForMod);
                doc.CommandEnded -= new CommandEventHandler(cmdEnded);
                doc.CommandWillStart -= new CommandEventHandler(cmdWillStart);
                bCmdActive = false;
                bReposition = false;
            }
            catch
            {
                ed.WriteMessage("�Ƴ��¼�����\n");
            }
        }

        public void objOpenedForMod(object o, ObjectEventArgs e)
        {
            if (bCmdActive == false || bReposition == true)
                return;
            ObjectId objId = e.DBObject.ObjectId;
            Database db = HostApplicationServices.WorkingDatabase;
            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            try
            {
                using (Transaction trans = db.TransactionManager.StartTransaction())
                {
                    Entity ent = (Entity)trans.GetObject(objId, OpenMode.ForRead, false);
                    //�ж϶����Ƿ�Ϊ��
                    if (ent is BlockReference)
                    { 
                        BlockReference br = (BlockReference)ent;
                        //��ȡObjectID�Ͳ���㣬�������Ǳ��浽ȫ�ֱ���
                                    blkObjIDs.Add(objId);
                                    blkPositions.Add(br.Position);
                    }
                    trans.Commit();
                }
            }
            catch
            {
                ed.WriteMessage("ObjectOpenedForModify�¼��Ĵ���������");
            }
        }

        public void cmdWillStart(object o, CommandEventArgs e)
        {
            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            try
            {
                if (e.GlobalCommandName == "MOVE")
                {
                    //����ȫ�ֱ���
                    bCmdActive = true;
                    bReposition = false;
                    //�������еı�����Ϣ
                    blkObjIDs.Clear();
                    blkPositions.Clear();
                }
            }
            catch
            {
                ed.WriteMessage("CommandWillStart�¼��Ĵ���������");
            }
        }

        public void cmdEnded(object o, CommandEventArgs e)
        {
            //�жϼ�������Ƿ��Ǽ����
            if (bCmdActive == false)
                return;
            bCmdActive = false;

            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            try
            {
                bReposition = true;
                Database db = HostApplicationServices.WorkingDatabase;
                for (int i = 0; i < blkObjIDs.Count; i++)
                {
                    Point3d oldpos;
                    Point3d newpos;
                    using (Transaction trans = db.TransactionManager.StartTransaction())
                    {
                        BlockTable bt = (BlockTable)trans.GetObject(db.BlockTableId, OpenMode.ForRead);
                        Entity ent = (Entity)trans.GetObject(blkObjIDs[i], OpenMode.ForWrite);
                        //ʵ�������ж�
                        if (ent is BlockReference )
                        { 
                            BlockReference br = (BlockReference)ent;
                            newpos = br.Position;
                            oldpos = blkPositions[i];

                            //����Ϊ��ʼ��λ��
                            if (!oldpos.Equals(newpos))
                                br.Position = oldpos;
                        }
                        trans.Commit();
                    }
                }
            }
            catch
            {
                ed.WriteMessage("CommandEnded�¼�����������");
            }
        }
    } 
}
