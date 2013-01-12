using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;

namespace CH05
{
    public class Class1
    {

         //--------------------------------------------------------------
        // ����:�����չ����XDATA
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //----------------------------------------------------------------
        [CommandMethod("AddXData")]
        public void AddXData()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ed.WriteMessage("�����������XDATA\n");
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

                RegAppTable appTbl = trans.GetObject(db.RegAppTableId, OpenMode.ForWrite) as RegAppTable ;
                if (!appTbl.Has("MyAppName"))
                {
                    RegAppTableRecord appTblRcd = new RegAppTableRecord();
                    appTblRcd.Name = "MyAppName";
                    appTbl.Add(appTblRcd);
                    trans.AddNewlyCreatedDBObject(appTblRcd, true);
                }
                ResultBuffer resBuf = new ResultBuffer();//new TypedValue(1001, "MyAppName"), new TypedValue(1000, "��������"));

                resBuf.Add(new TypedValue(1001, "MyAppName"));//ע���������
                resBuf.Add(new TypedValue(1000 , " ����"));//����
                resBuf.Add(new TypedValue(1000 , " ���̲�"));//����
                resBuf.Add(new TypedValue(1040, 2000.0));//нˮ
                ent.XData =  resBuf;
                trans.Commit();
            }
 
        }


        //--------------------------------------------------------------
        // ����:��ȡ��չ����XDATA
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //------------------------------------------------------------
        [CommandMethod("GETXDATA")]
        public void GETXDATA()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ed.WriteMessage("��ȡ��������XDATA\n");

            PromptEntityOptions entOps = new PromptEntityOptions("ѡ�����չ���ݵĶ���");
            PromptEntityResult entRes = ed.GetEntity(entOps);
            if (entRes.Status != PromptStatus.OK)
            {
                ed.WriteMessage("ѡ�����ʧ�ܣ��˳�");
                return;
            }
            Database db = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Database;
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {

                Entity ent = (Entity)trans.GetObject(entRes.ObjectId, OpenMode.ForRead);
                ResultBuffer resBuf = ent.XData;
                if (resBuf != null)
                {
                    //
                    IEnumerator iter = resBuf.GetEnumerator();
                    while (iter.MoveNext())
                    {
                        TypedValue tmpVal = (TypedValue)iter.Current;
                        ed.WriteMessage(tmpVal.TypeCode.ToString() + ":");
                        ed.WriteMessage(tmpVal.Value.ToString() + "\n");
                    }
                }
            }
        }

        //--------------------------------------------------------------
        // ����:����������ʵ����������
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //------------------------------------------------------------
        [CommandMethod("AddInNOD")]
        public void AddInNOD()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ed.WriteMessage("����������ʵ����������\n");
            Database db = HostApplicationServices.WorkingDatabase;
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                //��ȡ��������ʵ䣨NOD)
                DBDictionary NOD =trans.GetObject(db.NamedObjectsDictionaryId, OpenMode.ForWrite) as DBDictionary ;
                // ����һ���µĴʵ�
                DBDictionary copyrightDict;
                // �ж��Ƿ����COPYRIGHT�ʵ䣬û���򴴽�
                try
                {
                    // ��ȡCOPYRIGHT�ʵ�
                    copyrightDict = (DBDictionary)trans.GetObject(NOD.GetAt("COPYRIGHT"), OpenMode.ForRead);
                }
                catch
                {
                    //��NOD�´���COPYRIGHT�ʵ�
                    copyrightDict = new DBDictionary();
                    NOD.SetAt("COPYRIGHT", copyrightDict);
                    trans.AddNewlyCreatedDBObject(copyrightDict, true);
                }

                // ��copyrightDict�У���ȡ�򴴽� "author" �ʵ�
                DBDictionary authorDict;
                try
                {
                    authorDict = (DBDictionary)trans.GetObject(copyrightDict.GetAt("Author"), OpenMode.ForWrite);
                }
                catch
                {
                    authorDict = new DBDictionary();
                    //"author" doesn't exist, create one
                    copyrightDict.UpgradeOpen();
                    copyrightDict.SetAt("Author", authorDict);
                    trans.AddNewlyCreatedDBObject(authorDict, true);
                }

                // ͨ��Xrecord��ResultBuffer�����չ����
                Xrecord authorRec;
                try
                {
                    authorRec = (Xrecord)trans.GetObject(authorDict.GetAt("AuthorInfo"), OpenMode.ForWrite);
                }
                catch
                {
                    authorRec = new Xrecord();
                    authorRec.Data = new ResultBuffer(new TypedValue((int)DxfCode.Text, "����"));
                    authorDict.SetAt("AuthorInfo", authorRec);
                    trans.AddNewlyCreatedDBObject(authorRec, true);
                }
                trans.Commit();
            }
        }

        //--------------------------------------------------------------
        // ����:��ȡ��������ʵ��е�����
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //------------------------------------------------------------
        [CommandMethod("GetInNOD")]
        public void GetInNod()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ed.WriteMessage("��ȡ��������ʵ�������\n");

            Database db = HostApplicationServices.WorkingDatabase;
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                // ��ȡNOD 
                DBDictionary NOD = (DBDictionary)trans.GetObject(db.NamedObjectsDictionaryId, OpenMode.ForRead, false);
                // ��ȡCOPYRIGHT�ʵ�
                DBDictionary copyrightDict = (DBDictionary)trans.GetObject(NOD.GetAt("COPYRIGHT"), OpenMode.ForRead);
                // ��ȡAuthor�ʵ�
                DBDictionary AuthorDict = (DBDictionary)trans.GetObject(copyrightDict.GetAt("Author"), OpenMode.ForRead);
                // ��ȡAuthorInfo��չ��¼Xrecord
                Xrecord authorXRec = (Xrecord)trans.GetObject(AuthorDict.GetAt("AuthorInfo"), OpenMode.ForRead);
                ResultBuffer resBuf = authorXRec.Data;
                TypedValue val = resBuf.AsArray()[0];
                ed.WriteMessage("��ͼֽ��{0}���\n", val.Value);
            }
        }

        //--------------------------------------------------------------
        // ����:������ݵ����ݿ�������չ�ʵ���
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //------------------------------------------------------------
        [CommandMethod("AddExtDict")]
        public void AddExtDict()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ed.WriteMessage("����������չ�ʵ�\n");

            PromptEntityOptions entOps = new PromptEntityOptions("ѡ��Ҫ�����չ���ݵĿ�\n");
            PromptEntityResult entRes = ed.GetEntity(entOps);
            if (entRes.Status != PromptStatus.OK)
            {
                ed.WriteMessage("ѡ�����ʧ�ܣ��˳�");
                return;
            }
            Database db = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Database;
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                DBObject obj = trans.GetObject(entRes.ObjectId, OpenMode.ForWrite) as DBObject;
                BlockReference blkRef;
                if (obj is BlockReference)
                {
                    blkRef = obj as BlockReference;
                }
                else
                {
                    return;
                }

                // �����������չ�ʵ�
                blkRef.CreateExtensionDictionary();
                DBDictionary extensionDict = (DBDictionary)trans.GetObject(blkRef.ExtensionDictionary, OpenMode.ForWrite, false);
               
                // ͨ��Xrecord׼��������������
                Xrecord xRec = new Xrecord();
                xRec.Data = new ResultBuffer(
                  new TypedValue((int)DxfCode.Text, "����"),// ����
                  new TypedValue((int)DxfCode.Real, 1200.0),//нˮ
                  new TypedValue((int)DxfCode.Text, "������"));// ����         
               // ����չ�ʵ��������չ��¼
                extensionDict.SetAt("EmployeeInfomation", xRec); 
                trans.AddNewlyCreatedDBObject(xRec, true);

                trans.Commit();
            }

        }


        //--------------------------------------------------------------
        // ����:��ȡ���ݿ�������չ�ʵ��е�����
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //
        //------------------------------------------------------------
        [CommandMethod("GetExtDict")]
        public void GetExtDict()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            ed.WriteMessage("��ȡ������չ�ʵ���Ϣ\n");

            PromptEntityOptions entOps = new PromptEntityOptions("ѡ���������չ���ݵĿ�\n");
            PromptEntityResult entRes = ed.GetEntity(entOps);
            if (entRes.Status != PromptStatus.OK)
            {
                ed.WriteMessage("ѡ�����ʧ�ܣ��˳�");
                return;
            }
            Database db = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Database;
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                DBObject obj = trans.GetObject(entRes.ObjectId, OpenMode.ForWrite) as DBObject;
                BlockReference blkRef;
                if (obj is BlockReference)
                {
                    blkRef = obj as BlockReference;
                }
                else
                {
                    ed.WriteMessage("ѡ������ǿ飬�˳�\n");
                    return;
                }

                // �����������չ�ʵ�
                DBDictionary extensionDict = (DBDictionary)trans.GetObject(blkRef.ExtensionDictionary, OpenMode.ForWrite, false);
                // ��ȡAuthorInfo��չ��¼Xrecord
                Xrecord EmpXRec = (Xrecord)trans.GetObject(extensionDict.GetAt("EmployeeInfomation"), OpenMode.ForRead);
                ResultBuffer resBuf = EmpXRec.Data;
                TypedValue val = resBuf.AsArray()[0];
                ed.WriteMessage("��Ա������:{0}\n", val.Value);
                val = resBuf.AsArray()[1];
                ed.WriteMessage("��Ա����нˮ:{0}\n", val.Value);
                val = resBuf.AsArray()[2];
                ed.WriteMessage("��Ա������:{0}\n", val.Value);
 
                trans.Commit();
            }

        }
        
        ////


    }
}
