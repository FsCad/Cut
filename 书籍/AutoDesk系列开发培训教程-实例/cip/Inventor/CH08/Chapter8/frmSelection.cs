using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Inventor;

namespace Chapter8
{
    public partial class frmSelection : Form
    {
        private Inventor.Application ThisApplication;
        private InteractionEvents interaction;
        private SelectEvents select;

        public frmSelection(Inventor.Application Application)
        {
            InitializeComponent();

            ThisApplication = Application;
        }

        private void frmSelection_Load(object sender, EventArgs e)
        {
            //�����µ�InteractionEvents����
            interaction = ThisApplication.CommandManager.CreateInteractionEvents();

            //������ʾ
            interaction.StatusBarText = "Select an edge.";

            //���ӵ���ص�ѡ���¼�
            select = interaction.SelectEvents;

            //�������е�����߶�����ѡ��
            select.AddSelectionFilter(SelectionFilterEnum.kPartEdgeFilter);

            //���õ�һѡ��
            select.SingleSelectEnabled = true;

            //��ʼѡ�����
            interaction.Start();

            //�¼���Ӧ
            select.OnSelect += new SelectEventsSink_OnSelectEventHandler(select_OnSelect);
            select.OnPreSelect += new SelectEventsSink_OnPreSelectEventHandler(select_OnPreSelect);
        }

        void select_OnSelect(ObjectsEnumerator JustSelectedEntities, SelectionDeviceEnum SelectionDevice, Inventor.Point ModelPosition, Point2d ViewPosition, Inventor.View View)
        {
            //������ѡ�ߵĳ���
            int i;
            double length = 0.0;
            for (i = 0; i < JustSelectedEntities.Count; i++)
            {
                //��Ϊ�Ѿ�����������Ϊѡ��ߣ����Խ����ص�ͼԪָ��ΪEdge�����ǰ�ȫ��
                Edge edge;
                edge = (Edge)JustSelectedEntities[i];

                //ȷ����ǰ�ߵĳ���
                double min;
                double max;
                edge.Evaluator.GetParamExtents(out min, out max);

                double singleLength;
                edge.Evaluator.GetLengthAtParam(min, max, out singleLength);

                //���㼯�������бߵĳ���
                length = length + singleLength;
            }

            //��ʾ�ߵĳ��Ⱥ�����
            txtLength.Text = string.Format("{0} cm", length);
            txtEdgeCount.Text = string.Format("{0}", JustSelectedEntities.Count);
        }

        void select_OnPreSelect(ref object PreSelectEntity, out bool DoHighlight, ref ObjectCollection MorePreSelectEntities, SelectionDeviceEnum SelectionDevice, Inventor.Point ModelPosition, Point2d ViewPosition, Inventor.View View)
        {
            DoHighlight = true;

            //���õ������ѡ�Ķ�������ã�����ǰ�涨��Ĺ�����������֪���ö���һ����һ����
            Edge edge;
            edge = (Edge)PreSelectEntity;

            //ȷ���Ƿ�����ñ����в����ӵı�
            EdgeCollection edges;
            edges = edge.TangentiallyConnectedEdges;
            if (edges.Count > 1)
            {
                //�������������ߵĶ��󼯺�
                MorePreSelectEntities = ThisApplication.TransientObjects.CreateObjectCollection(null);

                for (int i = 1; i <= edges.Count; i++)
                {
                    MorePreSelectEntities.Add(edges[i]);
                }
            }
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            //ֹͣѡ���ͷ�ȫ������
            interaction.Stop();
            select = null;
            interaction = null;

            //ж�ش���
            this.Hide();
        }

        public void InteractionEventsSink_OnTerminateEventHandler()
        {
            //�ͷ�ȫ������
            select = null;
            interaction = null;

            //ж�ش���
            this.Hide();
        }
    }
}