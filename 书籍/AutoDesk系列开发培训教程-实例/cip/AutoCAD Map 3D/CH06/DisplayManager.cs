//
// (C) Copyright 2004-2007 by Autodesk, Inc.
//
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
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE. AUTODESK, INC.
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

// DisplayManager.cs


using System;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.DatabaseServices;

using Autodesk.Gis.Map;
using Autodesk.Gis.Map.DisplayManagement;
using Autodesk.Gis.Map.Project;

namespace CH06
{
    //--------------------------------------------------------------------------
    //
    // ���ܣ������࣬��װ����ʾ�������ĺ���
    //
    //  ���ߣ� 
    //
    //  ���ڣ�200708
    //
    //   ��ʷ��
    //--------------------------------------------------------------------------
    public sealed class MyDisplayManager
    {
        private MyDisplayManager()
        {
        }

        private static MyDisplayManager m_Instance = new MyDisplayManager();

        public static MyDisplayManager Instance
        {
            get
            {
                return m_Instance;
            }
        }

         //--------------------------------------------------------------------------
        //
        // ���ܣ��½���ͼ���µĵ�ͼ�����������ʾ�������������б���
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        public bool AddMap(string mapName)
        {
            bool isAdded = false;

            // ��ȡ��ǰAutoCAD MapӦ�ó������
            ProjectModel project = null;
            MapApplication app = HostMapApplicationServices.Application;

            project = app.ActiveProject;

            try
            {
                using (Transaction trans = Utility.TransactionManager.StartTransaction())
                {
                    ObjectId managerId = DisplayManager.Create(project).MapManagerId(project, true);
                    MapManager manager = (MapManager)trans.GetObject(managerId, OpenMode.ForWrite);
    
                    // �����µĵ�ͼ
                    ObjectId id = manager.CreateNewMap();    

                    // ����������ͼ
                    manager.ResetName(id, mapName);
                    string message = string.Format("\n��ͼ {0}�����ɹ���", mapName);
                    Utility.ShowMessage(message);

                    trans.Commit();
                    isAdded = true;
                }
            }
            catch (Autodesk.AutoCAD.Runtime.Exception e)
            {
                string message = string.Format("\n������ͼʧ�ܣ��������: {0}.", e.ErrorStatus);
                Utility.ShowMessage(message);
            }
  
            return isAdded;
        }

       //--------------------------------------------------------------------------
        //
        // ���ܣ�����ʾ�������������б����Ƴ���ͼ
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        public bool RemoveMap(string mapName)
        {
            bool isRemoved = false;
            
            ProjectModel project = null;
            MapApplication app = HostMapApplicationServices.Application;

            project = app.ActiveProject;

            try
            {
                using (Transaction trans = Utility.TransactionManager.StartTransaction())
                {
                    ObjectId managerId = DisplayManager.Create(project).MapManagerId(project, true);
                    MapManager manager = (MapManager)trans.GetObject(managerId, OpenMode.ForWrite);

                    manager.Remove(mapName);
                    trans.Commit();

                    string message = string.Format("\n��ͼ {0} �Ѿ���ɾ����", mapName);
                    Utility.ShowMessage(message);
                    isRemoved = true;
                }
            }
            catch (Autodesk.AutoCAD.Runtime.Exception e)
            {
                string message = string.Format("\n��ͼ {0} ɾ��ʧ�ܡ��������: {1}.", mapName, e.ErrorStatus);
                Utility.ShowMessage(message);
            }

            return isRemoved;
        }

       //--------------------------------------------------------------------------
        //
        // ���ܣ����õ�ͼ��Ϊ��ǰ��ʾ�������еĵ�ͼ
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        public bool SetMapAsCurrent(string mapName)
        {
            bool isSettedAsCurrent = false;
            
            ProjectModel project = null;
            MapApplication app = HostMapApplicationServices.Application;

            project = app.ActiveProject;

            try
            {
                using (Transaction trans = Utility.TransactionManager.StartTransaction())
                {
                    ObjectId managerId = DisplayManager.Create(project).MapManagerId(project, true);
                    MapManager manager = (MapManager)trans.GetObject(managerId, OpenMode.ForWrite);

                    manager.Current = mapName;
                    trans.Commit();

                    string message = string.Format("\n��ͼ{0}�Ѿ���Ϊ��ǰ��ͼ��", mapName);
                    Utility.ShowMessage(message);
                    isSettedAsCurrent = true;
                }
            }
            catch (Autodesk.AutoCAD.Runtime.Exception e)
            {
                string message = string.Format("\n��ͼ{0}������Ϊ��ǰ��ͼ���������: {1}��", mapName, e.ErrorStatus);
                Utility.ShowMessage(message);
            }  

            return isSettedAsCurrent;
        }

       //--------------------------------------------------------------------------
        //
        // ���ܣ����ñ�����ֵ����ʾ�������ĳ߶��б�
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        public bool AddScale()
        {
            bool isAdded = false;
            string message = null;
            ObjectId currentMapId = new ObjectId();
            if (!FindCurrentMapId(ref currentMapId))
            {
                return isAdded;
            }

            MapApplication app = HostMapApplicationServices.Application;
            Editor editor = app.GetDocument(app.ActiveProject).Editor;
            PromptDoubleOptions promptOptions = new PromptDoubleOptions("");
            promptOptions.AllowNone = false;
            promptOptions.AllowZero = false;
            promptOptions.AllowNegative = false;
            promptOptions.Message = "\n�����µı����ߣ� ";
            double scale = 0.0;
            try
            {
                PromptDoubleResult doublePromptResult = editor.GetDouble(promptOptions);
                if (doublePromptResult.Status == PromptStatus.OK)
                {
                    scale = doublePromptResult.Value;  
                    using (Transaction trans = Utility.TransactionManager.StartTransaction())
                    {
                        Map currentMap = (Map)trans.GetObject(currentMapId, OpenMode.ForWrite);
                        // Add the new scale
                        currentMap.AddScaleThreshold(scale, 0);
                        message = string.Format("\n�µı����� {0}����ӡ�", scale);
                        Utility.ShowMessage(message);
                        isAdded = true;
                        trans.Commit();
                    }
                }
            }
            catch (Autodesk.AutoCAD.Runtime.Exception)
            {
                message = string.Format("\n��ӱ����� {0}ʧ�ܡ�", scale);
                Utility.ShowMessage(message);
            }

            return isAdded;
        }

        //--------------------------------------------------------------------------
        //
        // ���ܣ�ɾ������ʾ�������ĳ߶��б��еı�����
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //-------------------------------------------------------------------------- 
        public bool RemoveScale()
        {
            bool isRemoved = false;
            string message = null;
            ObjectId currentMapId = new ObjectId();
            if (!FindCurrentMapId(ref currentMapId))
            {
                return isRemoved;
            }

            MapApplication app = HostMapApplicationServices.Application;
            Editor editor = app.GetDocument(app.ActiveProject).Editor;
            PromptDoubleOptions promptOptions = new PromptDoubleOptions("");
            promptOptions.AllowNone = false;
            promptOptions.AllowZero = false;
            promptOptions.AllowNegative = false;
            promptOptions.Message = "\n����Ҫɾ���ı�����: ";
            double scale = 0;

            try
            {
                PromptDoubleResult doublePromptResult = editor.GetDouble(promptOptions);
                if (doublePromptResult.Status == PromptStatus.OK)
                {
                    scale = doublePromptResult.Value;  
                    using (Transaction trans = Utility.TransactionManager.StartTransaction())
                    {
                        Map currentMap = (Map)trans.GetObject(currentMapId, OpenMode.ForWrite);
                        // Remove the scale
                        currentMap.RemoveScaleThreshold(scale);
                        trans.Commit();
                        message = string.Format("\n������ {0} �Ѿ���ɾ����", scale);
                        Utility.ShowMessage(message);
                        isRemoved = true;
                    }
                }
            }
            catch (Autodesk.AutoCAD.Runtime.Exception)
            {
                message = string.Format("\n����ɾ�������� {0}.", scale);
                Utility.ShowMessage(message);
            }
    
            return isRemoved;
        }


        //--------------------------------------------------------------------------
        //
        // ���ܣ����õ�ǰ������
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //-------------------------------------------------------------------------- 
        public bool SetScaleAsCurrent()
        {
            bool isSettedAsCurrent = false;
            string message = null;
            ObjectId currentMapId = new ObjectId();
            if (!FindCurrentMapId(ref currentMapId))
            {
                return isSettedAsCurrent;
            }

            MapApplication app = HostMapApplicationServices.Application;
            Editor editor = app.GetDocument(app.ActiveProject).Editor;
            PromptDoubleOptions promptOptions = new PromptDoubleOptions("");
            promptOptions.AllowNone = false;
            promptOptions.AllowZero = false;
            promptOptions.AllowNegative = false;
            promptOptions.Message = "\n����Ҫ��Ϊ��ǰ�ı�����: ";
            double scale = 0;

            try
            {
                PromptDoubleResult doublePromptResult = editor.GetDouble(promptOptions);
                if (doublePromptResult.Status == PromptStatus.OK)
                {
                    scale = doublePromptResult.Value;  
                    using (Transaction trans = Utility.TransactionManager.StartTransaction())
                    {
                        Map currentMap = (Map )trans.GetObject(currentMapId, OpenMode.ForWrite);

                        currentMap.SetCurrentScale( scale, true );
                        message = string.Format("\n������ {0}�Ѿ���Ϊ��ǰ��", scale);
                        Utility.ShowMessage(message);
                        isSettedAsCurrent = true;
                        trans.Commit();
                    }
                }
            }
            catch (Autodesk.AutoCAD.Runtime.Exception)
            {
                message = string.Format("\n�������ñ����� {0}Ϊ��ǰ�����ߡ�", scale);
                Utility.ShowMessage(message);
            }
      
            return isSettedAsCurrent;
        }

        //--------------------------------------------------------------------------
        //
        // ���ܣ���ȡ��ǰ��ͼ��ID
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //-------------------------------------------------------------------------- 
        private bool FindCurrentMapId(ref ObjectId currentMapId)
        {
            bool isFound = false;
            ProjectModel project = null;
            MapApplication app = HostMapApplicationServices.Application;

            project = app.ActiveProject;

            try
            {
                using (Transaction trans = Utility.TransactionManager.StartTransaction())
                {
                    ObjectId managerId = DisplayManager.Create(project).MapManagerId(project, true);
                    MapManager manager = trans.GetObject(managerId, OpenMode.ForRead) as MapManager;
                    if (null != manager)
                    {
                        currentMapId = manager.CurrentMapId;
                        isFound = true;
                    }
                    trans.Commit();
                }
            }
            catch (Autodesk.AutoCAD.Runtime.Exception)
            {
                Utility.ShowMessage("\n���ܻ�ȡ��ǰ��ͼ��ID��");
            }

            return isFound;
        }
    }
}
