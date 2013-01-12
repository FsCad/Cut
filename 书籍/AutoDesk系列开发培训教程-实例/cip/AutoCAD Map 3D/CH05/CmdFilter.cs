using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Text;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.Gis.Map.Filters;

namespace CH05
{

    public  class CmdFilter
    {
        //--------------------------------------------------------------------------
        //
        // ���ܣ����л�������
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        [CommandMethod("BasicFilter")]
        public void BasicFilter()
        {
            BasicFilter bfilter = new BasicFilter();
            // �趨���������
            StringCollection layerCollection = new StringCollection();
             //layerCollection.Add("*");  // �������еĲ�
            layerCollection.Add("1");    //  ���˲� 1
            bfilter.SetLayers(layerCollection);

            //  �趨���������
            StringCollection blkCollection = new StringCollection();
            //����������еĿ飬���ÿ������ϰ��� "*"
            // ������Կ���ˣ����ÿ������ϰ��� ""  
            blkCollection.Add("");
            bfilter.SetBlocks(blkCollection);
            //�趨Ҫ�ع�������
            StringCollection blkClassification = new StringCollection();
            //blkClassification.Add("pipe"); // ����pipeҪ��
            blkClassification.Add("") ;// ����Ҫ�ع���
            bfilter.SetFeatureClasses(blkClassification);

            ObjectIdCollection selectedIDCollection = new ObjectIdCollection();
            selectedIDCollection = GetAllEntity();

            ObjectIdCollection outIDCollection = new ObjectIdCollection() ;
            bfilter.FilterObjects(ref outIDCollection, selectedIDCollection);

            // ������˵õ��Ķ���
            HighlightEntity(outIDCollection);

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
        [CommandMethod("CustFilter")]
        public void CustFilter()
        {
            try
            {
                // �����Զ�����˶��󣬲��趨��������
                MyPolylineFilter cfilter = new MyPolylineFilter();
                cfilter.MinLen = 500.0;
                cfilter.MaxLen = 2000.0;
                // ��ȡҪ���˵Ķ���
                ObjectIdCollection selectedIDCollection = new ObjectIdCollection();
                selectedIDCollection = GetAllEntity();
                // ִ�й���
                ObjectIdCollection outIDCollection = new ObjectIdCollection();
                cfilter.FilterObjects(outIDCollection, selectedIDCollection);

                // ������˵õ��Ķ���
                HighlightEntity(outIDCollection);
            }
            catch (Autodesk.AutoCAD.Runtime.Exception e)
            {
                Utility.AcadEditor.WriteMessage(e.Message);
            }
        }


       
        //--------------------------------------------------------------------------
        //
        // ���ܣ���ȡAutoCAD���ݿ�������ʵ��
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        private ObjectIdCollection GetAllEntity()
        {
            ObjectIdCollection returnValue;
            Autodesk.AutoCAD.DatabaseServices.Database curDb;
            curDb = Autodesk.AutoCAD.DatabaseServices.HostApplicationServices.WorkingDatabase;

            Transaction trans = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.TransactionManager.StartTransaction();
            ObjectIdCollection IDCol = new ObjectIdCollection();
            IDCol.Clear();
            BlockTable blockTable;
            BlockTableRecord blkTblRecord;
            Entity entity;
            try
            {
                blockTable = trans.GetObject(curDb.BlockTableId, OpenMode.ForRead) as BlockTable;
                blkTblRecord = trans.GetObject(blockTable[BlockTableRecord.ModelSpace], OpenMode.ForRead) as BlockTableRecord ;

                ObjectId objId;
                foreach (ObjectId tempLoopVar_objId in blkTblRecord)
                {
                    objId = tempLoopVar_objId;
                    entity = trans.GetObject(objId, OpenMode.ForRead) as Entity ;
                    IDCol.Add(entity.ObjectId);
                }
                trans.Commit();
            }
            catch (System.Exception e )
            {
                Utility.AcadEditor.WriteMessage(e.Message);
            }
            finally
            {
                trans.Dispose();
            }
            returnValue = IDCol;

            return returnValue;
        }

        //--------------------------------------------------------------------------
        //
        // ���ܣ�������ʾѡ�е�ʵ��
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        private void HighlightEntity(Autodesk.AutoCAD.DatabaseServices.ObjectIdCollection entityIdCollection)
        {
            Transaction trans = null;
            try
            {
                trans = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.TransactionManager.StartTransaction();
                ObjectId id;
                foreach (ObjectId tempLoopVar_id in entityIdCollection)
                {
                    id = tempLoopVar_id;
                    Entity entity;
                    try
                    {
                        entity = trans.GetObject(id, OpenMode.ForRead)as Entity;
                        entity.UpgradeOpen();
                        Type blockType = entity.GetType();

                        if (entity is BlockReference)
                        {
                            BlockReference blkRef = entity as BlockReference ;
                            BlockTableRecord blkTblRecord = trans.GetObject(blkRef.BlockTableRecord, OpenMode.ForRead) as BlockTableRecord ;
                            ObjectId objId;
                            foreach (ObjectId tempLoopVar_objId in blkTblRecord)
                            {
                                objId = tempLoopVar_objId;
                                Entity blkEntity = trans.GetObject(objId, OpenMode.ForWrite)as Entity ;
                                blkEntity.ColorIndex = 1;
                            }
                        }
                        entity.ColorIndex = 1;
                    }
                    catch (Autodesk.Gis.Map.MapException e )
                    {
                        Utility.AcadEditor.WriteMessage(e.Message);
                    }
                }
                trans.Commit();
            }
            catch (System.Exception e )
            {
                Utility.AcadEditor.WriteMessage(e.Message);
            }
            finally
            {
                trans.Dispose();
            }
        }



    }

    //--------------------------------------------------------------------------
    //
    // ���ܣ����ܺ����࣬���ڻ�ȡEditor
    //
    //  ���ߣ� 
    //
    //  ���ڣ�200708
    //
    //   ��ʷ��
    //--------------------------------------------------------------------------
    public class Utility
    {
        private Utility()
        {
        }

        public static Autodesk.AutoCAD.EditorInput.Editor AcadEditor
        {
            get
            {
                return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            }
        }
    }
}
