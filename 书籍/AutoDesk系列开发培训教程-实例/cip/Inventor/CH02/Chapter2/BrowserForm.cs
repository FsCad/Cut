using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Inventor;


namespace Chapter2
{
    public partial class BrowserForm : Form
    {
        public BrowserForm(Inventor.Application InventorApplication)
        {
            InitializeComponent();
            m_inventorApplication = InventorApplication;
        }

        //���������ؼ��ı���
        private MSACAL.Calendar calCtrl;

        //����InventorӦ�ó���ı���
        private Inventor.Application m_inventorApplication;

        //�����¼������������ı���
        private DocumentEvents docEvents;
        private BrowserPane browserPane;

        private void BrowserForm_Load(object sender, EventArgs e)
        {
            //��ü����ĵ���������һ���ĵ�����
            Inventor.Document doc;
            doc = m_inventorApplication.ActiveDocument;

            //���ӵ��ĵ��¼���������Ӧ�ĵ��ر�
            docEvents = doc.DocumentEvents;

            //ʹ�������ؼ������µ����������
            browserPane = doc.BrowserPanes.Add("Calendar", "MSCAL.Calendar");

            //���õ������ؼ�������
            calCtrl = (MSACAL.Calendar)browserPane.Control;

            //���������ؼ���ʾ��ǰ����
            calCtrl.Today();

            //ʹ�´����Ϊ�����
            browserPane.Activate();

            calCtrl.Click += new MSACAL.DCalendarEvents_ClickEventHandler(calCtrl_Click);
        }

        void calCtrl_Click()
        {
            //��Inventor״̬����ʾ������
            m_inventorApplication.StatusBarText = string.Format("Selected new date: {0}-{1}-{2}", calCtrl.Year, calCtrl.Month,calCtrl.Day);

            //��ѡ��2000��1��1��ʱж�ش���
            if (calCtrl.Value.ToString() == "2000-1-1 0:00:00")
            {
                Unload();
            }
        }

        private void BrowserForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            //ж�ش���
            Unload();
        }

        private void Unload()
        {
            calCtrl=null;
            docEvents = null;
            if (browserPane != null)
            {
                browserPane.Delete();
                browserPane = null;
            }
        }
     }
}