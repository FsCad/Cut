using System;
using System.Collections.Generic;
using System.Text;
using Inventor;
using System.Diagnostics;
using Microsoft.VisualBasic.Compatibility.VB6;
using stdole;
using System.Drawing;
using System.Windows.Forms;


namespace Chapter2
{
    class BrowserNode
    {
        private Inventor.Application m_inventorApplication;

        public BrowserNode(Inventor.Application inventorApplication)
        {
            m_inventorApplication = inventorApplication;
        }

        public void QueryModelTree()
        {
            //����ĵ�����
            Inventor.Document doc;
            doc = m_inventorApplication.ActiveDocument;

            if(m_inventorApplication.Documents.Count == 0)
            {
                MessageBox.Show("There are no open documents!");
            }

            //���ģ����ҳ����������еĶ����ڵ�
            Inventor.BrowserNode topNode;
            topNode = doc.BrowserPanes["ģ��"].TopNode;

            //�Ӷ����ڵ㿪ʼ������recurse����
            recurse(topNode);
        }

        private void recurse(Inventor.BrowserNode node)
        {
            if (node.Visible)
            {
                MessageBox.Show(node.BrowserNodeDefinition.Label);

                foreach (Inventor.BrowserNode bn in node.BrowserNodes)
                {
                    recurse(bn);
                }
            }
        }

        public void AddNodes()
        {
            //����ĵ�����
            Inventor.Document doc;
            doc = m_inventorApplication.ActiveDocument;

            if(m_inventorApplication.Documents.Count == 0)
            {
                MessageBox.Show("There are no open documents!");
            }

            //��õ�ǰ�ĵ��е�BrowserPanes����
            BrowserPanes panes;
            panes = doc.BrowserPanes;

            ClientNodeResources recs;
            recs = panes.ClientNodeResources;

            Image image;
            image = Image.FromFile("c:\\Icon.bmp");
            stdole.IPictureDisp iconDisp;
            iconDisp = (stdole.IPictureDisp)Support.ImageToIPictureDisp(image);

            ClientNodeResource rsc;
            rsc = recs.Add("Test", 0, iconDisp);

            BrowserNodeDefinition def;
            def = (BrowserNodeDefinition)panes.CreateBrowserNodeDefinition("Top Node", 3, rsc, "", Type.Missing, Type.Missing, "");

            BrowserPane pane;
            pane = panes.AddTreeBrowserPane("My Pane", "MyGUID", def);

            BrowserNodeDefinition def1;
            Inventor.BrowserNode node1;
            def1 = (BrowserNodeDefinition)panes.CreateBrowserNodeDefinition("Node2", 5, rsc, "", Type.Missing, Type.Missing, "");
            node1 = pane.TopNode.AddChild(def1);

            BrowserNodeDefinition def2;
            Inventor.BrowserNode node2;
            def2 = (BrowserNodeDefinition)panes.CreateBrowserNodeDefinition("Node3", 6, rsc, "", Type.Missing, Type.Missing, "");
            node2 = pane.TopNode.AddChild(def2);
        }

   }


}
