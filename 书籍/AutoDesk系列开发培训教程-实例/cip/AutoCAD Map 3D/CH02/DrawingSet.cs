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
    public class CmdDrawingSet
    {

        //--------------------------------------------------------------------------
        //
        // ���ܣ����ͼ�μ���Ϣ
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        [CommandMethod("PrintDrawSet")]
        public void PrintDrawSet()
        {

            // ��ȡMap 3D ��������ͼ�μ�
            MapApplication mapApi = HostMapApplicationServices.Application;
            ProjectModel proj = mapApi.ActiveProject;
            DrawingSet drawingSet = proj.DrawingSet;

            //���ͼ�μ���Ϣ
            if (drawingSet.DirectDrawingsCount > 0)
                Utility.AcadEditor.WriteMessage("\n\tͼ�μ�:");

            for (int i = 0; i < drawingSet.DirectDrawingsCount; i++)
            {
                try
                {
                    AttachedDrawing drawingEntry = drawingSet.DirectAttachedDrawings[i];
                    PrintAttachedDrawingStatus(drawingEntry, 0);
                }
                catch (MapException e )
                {
                    Utility.AcadEditor.WriteMessage(e.Message);
                }
            }   

        }


        //--------------------------------------------------------------------------
        //
        // ���ܣ�����ͼ��
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        [CommandMethod("AttachDraw")]
        public void AttachDraw()
        {

            // ��ȡMap 3D ��������ͼ�μ�
            MapApplication mapApi = HostMapApplicationServices.Application;
            ProjectModel proj = mapApi.ActiveProject;
            DrawingSet drawingSet = proj.DrawingSet;

            // ��ȡ�û�����
            string strName = "";
            PromptResult promptResName = null;
            promptResName = Utility.AcadEditor.GetString("\n���븽��ͼ�����ƣ� ");
            if (promptResName.Status == PromptStatus.OK)
            {
                strName = promptResName.StringResult;
                try
                {
                    AttachedDrawing dwgDrawing = drawingSet.AttachDrawing(strName);
                    return;
                }
                catch (MapException e)
                {
                    Utility.AcadEditor.WriteMessage(e.Message);
                }
            }
            else
            {
                Utility.AcadEditor.WriteMessage("\n����������ƴ����˳���");
            }
            
        }

        //--------------------------------------------------------------------------
        //
        // ���ܣ�����ͼ��
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        [CommandMethod("DetachDraw")]
        public void DetachDraw()
        {

            // ��ȡMap 3D ��������ͼ�μ�
            MapApplication mapApi = HostMapApplicationServices.Application;
            ProjectModel proj = mapApi.ActiveProject;
            DrawingSet drawingSet = proj.DrawingSet;

            // ��ȡ�û�����
            string strName = "";
            PromptResult promptResName = null;
            promptResName = Utility.AcadEditor.GetString("\n���븽��ͼ�����ƣ� ");
            if (promptResName.Status == PromptStatus.OK)
            {
                strName = promptResName.StringResult;
                try
                {
                    drawingSet.DetachDrawing(strName);
                    return;
                }
                catch (MapException e)
                {
                    Utility.AcadEditor.WriteMessage(e.Message);
                }
            }
            else
            {
                Utility.AcadEditor.WriteMessage("\n����������ƴ����˳���");
                return;
            }

        }


        //--------------------------------------------------------------------------
        //
        // ���ܣ�����ͼ��
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        [CommandMethod("ActivateDraw")]
        public void ActivateDraw()
        {

            // ��ȡMap 3D ��������ͼ�μ�
            MapApplication mapApi = HostMapApplicationServices.Application;
            ProjectModel proj = mapApi.ActiveProject;
            DrawingSet drawingSet = proj.DrawingSet;

            // ��ȡ�û�����
            string strName = "";
            PromptResult promptResName = null;
            promptResName = Utility.AcadEditor.GetString("\n����Ҫ����ĸ���ͼ�����ƣ� ");
            if (promptResName.Status == PromptStatus.OK)
            {
                strName = promptResName.StringResult;
                try
                {
                    AttachedDrawing dwgDrawing = drawingSet.DirectAttachedDrawings[strName];
                    dwgDrawing.Activate();
                    return;
                }
                catch (MapException e)
                {
                    Utility.AcadEditor.WriteMessage(e.Message);
                }
            }
            else
            {
                Utility.AcadEditor.WriteMessage("\n����������ƴ����˳���");
                return;
            }

        }


        //--------------------------------------------------------------------------
        //
        // ���ܣ�����ͼ��
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        [CommandMethod("DeactivateDraw")]
        public void DeactivateDraw()
        {

            // ��ȡMap 3D ��������ͼ�μ�
            MapApplication mapApi = HostMapApplicationServices.Application;
            ProjectModel proj = mapApi.ActiveProject;
            DrawingSet drawingSet = proj.DrawingSet;

            // ��ȡ�û�����
            string strName = "";
            PromptResult promptResName = null;
            promptResName = Utility.AcadEditor.GetString("\n����Ҫȡ������ĸ���ͼ�����ƣ� ");
            if (promptResName.Status == PromptStatus.OK)
            {
                strName = promptResName.StringResult;
                try
                {
                    AttachedDrawing dwgDrawing = drawingSet.DirectAttachedDrawings[strName];
                    dwgDrawing.Deactivate();
                    return;
                }
                catch (MapException e)
                {
                    Utility.AcadEditor.WriteMessage(e.Message);
                }
            }
            else
            {
                Utility.AcadEditor.WriteMessage("\n����������ƴ����˳���");
                return;
            }

        }
        //--------------------------------------------------------------------------
        //
        // ���ܣ��������ͼ����Ϣ
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
        private void PrintAttachedDrawingStatus(AttachedDrawing drawing, int nestedLevel)
        {
            if (null == drawing)
                return;

            Utility.AcadEditor.WriteMessage("\n  ");
            for (int i = 0; i < nestedLevel; i++)
            {
                Utility.AcadEditor.WriteMessage("��");
            }
                    Utility.AcadEditor.WriteMessage("·��������");

            switch (drawing.ActiveStatus)
            {
                case AdeDwgStatus.DwgInactive:
                    Utility.AcadEditor.WriteMessage(string.Format("δ���� {0} ({1})", drawing.AliasPath, drawing.Description));
                    break;
                case AdeDwgStatus.DwgActive:
                    Utility.AcadEditor.WriteMessage(string.Format("���� {0} ({1})", drawing.AliasPath, drawing.Description));
                    break;
                case AdeDwgStatus.DwgLocked:
                    Utility.AcadEditor.WriteMessage(string.Format("���� {0} ({1})", drawing.AliasPath, drawing.Description));
                    break;
            }

            for (int i = 0; i < drawing.DirectNestedDrawingsCount; i++)
            {
                AttachedDrawing nestedDrawing = drawing.DirectNestedDrawings[i];
                if (nestedDrawing != null)
                {
                    // 
                    PrintAttachedDrawingStatus(nestedDrawing, nestedDrawing.NestedLevel);
                }
            }
        }

        // end of class
    }
}
