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
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC.
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

// PolygonOverlay.cs

using System;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.Gis.Map;
using Autodesk.Gis.Map.Topology;

namespace CH07
{
    /// <summary>
    /// PolygonOverlay
    /// </summary>
    public sealed class PolygonOverlay
    {
        /// <summary>
        /// Combines polygons with polygons and keeps all geometry. Union acts
        /// like the Boolean OR operation and can be used only with polygons.
        /// </summary>
        /// <param name="sourceTopologyName">[in] The source topology name.</param>
        /// <param name="overlayTopologyName">[in] The overlay topology name.</param>
        /// <returns>  
        /// Returns true if successful.
        /// </returns>
        //�������Ϊ�����Ӷ�������˺͵��Ӷ��������
        private bool Union(string sourceTopologyName, string overlayTopologyName)
        {
            // ��������ģ�Ͷ���
            TopologyModel sourceTopology = null;

            // �����������ݶ��󼯺�
            OverlayDataCollection sourceDataCollection = null;

            MapApplication mapApp = HostMapApplicationServices.Application;
            //��ȡ��ǰ���̵�����ģ��
            Topologies topos = mapApp.ActiveProject.Topologies;

            // Does the Source Topology exist to get information from
            //�жϴ���������ģ���Ƿ����
            if (topos.Exists(sourceTopologyName))
            {		 
                sourceTopology = topos[sourceTopologyName];
            }
            else
            {
                Utility.AcadEditor.WriteMessage(string.Format("\nERROR: The topology {0} doesn't exist.", sourceTopologyName));
                return false;
            }	

            // �������Ӷ��������ģ�Ͷ���
            TopologyModel overlayTopology = null;

            // �����������ݶ��󼯺�
            OverlayDataCollection overlayDataCollection = new OverlayDataCollection();
            string expression = "";
            expression = string.Format(":AREA@TPMCNTR_{0}", overlayTopologyName);
            OverlayData data = new OverlayData(expression, "PolygonArea", Autodesk.Gis.Map.Constants.DataType.Real);
            overlayDataCollection.Add(data);

            // �жϵ�������ģ���Ƿ����
            if (topos.Exists(overlayTopologyName))
            {		 
                overlayTopology = topos[overlayTopologyName];
            }
            else
            {
                Utility.AcadEditor.WriteMessage(string.Format("\nERROR: The topology {0} doesn't exist.", overlayTopologyName));
                return false;
            }

			try
			{
				// ��Դ����ģ�ͣ�����ȡ
				sourceTopology.Open(Autodesk.Gis.Map.Topology.OpenMode.ForRead);

                // �򿪵�������ģ�ͣ�����ȡ
				overlayTopology.Open(Autodesk.Gis.Map.Topology.OpenMode.ForRead);

                // ���ýڵ㴴������
				PointCreationSettings nodeCreationSettings = new PointCreationSettings(
					"result",           // layer name
					1,				    // color, by layer
					true,			    // create new node
					"ACAD_POINT");      // block name

				
                //���ýڵ㴴��
				sourceTopology.SetNodeCreationSettings(nodeCreationSettings);

                //�����˵�����
				string newTopologyName = "Result";
                //�����˵�����
				string newTopologyDesc = "Result topology of an Union operation";

				
                //���������˵Ķ������ݱ�
				ObjectDataTable resultDataTable = new ObjectDataTable();	
				resultDataTable.ODTableName = "UnionResult";
				resultDataTable.ODTableDescription = "The result of the Union Overlay";

				// ��
				sourceTopology.Union(overlayTopology, 
					newTopologyName, 
					newTopologyDesc, 
					resultDataTable, 
					sourceDataCollection, 
					overlayDataCollection);

				TopologyModel newTopology = topos[newTopologyName];
				newTopology.Open(Autodesk.Gis.Map.Topology.OpenMode.ForRead);
				newTopology.ShowGeometry(5);
				newTopology.Close();

				return true;
			}
			catch (MapException e)
            {
                if (2001 == e.ErrorCode)
                {
                    Utility.AcadEditor.WriteMessage("\nERROR: Topology Result already exists.");
                }
                else
                {
                    Utility.AcadEditor.WriteMessage(string.Format("\nERROR: Operation failed with error code: {0}.",  e.ErrorCode));
                }
                return false;
			}
			finally
			{
				// Close the topologys
                sourceTopology.Close();
                overlayTopology.Close();
            }
        }

        /// <summary>
        /// Combines polygons with polygons and keeps all geometry. Union acts
        /// like the Boolean OR operation and can be used only with polygons.
        /// </summary>
        public void Union()
        {
            string sourceTopologyName = "";
            string overlayTopologyName = "";
            Editor editor = Utility.AcadEditor;

            PromptResult promptResult = editor.GetString("\nEnter the Source Topology name:");
            if (promptResult.Status == PromptStatus.Cancel)
            {
                return;
            }
            sourceTopologyName = promptResult.StringResult;

            promptResult = editor.GetString("\nEnter the Overlay Topology name:");
            if (promptResult.Status == PromptStatus.Cancel)
            {
                return;
            }
            overlayTopologyName = promptResult.StringResult;

            Union(sourceTopologyName, overlayTopologyName);
        }

        public PolygonOverlay()
        {
        }
    }
}
