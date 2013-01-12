using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Collections; // For ArrayList

using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Colors;

using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.ApplicationServices;

using Autodesk.AutoCAD.Windows;

[assembly: ExtensionApplication(typeof(CH06.Class1))]
//[assembly: CommandClass(typeof(CH06.Class1))]

namespace CH06
{
    public class Class1:Autodesk.AutoCAD.Runtime.IExtensionApplication 
    {
        public static string sAuthorDefault = "����";
        public static string sCompanyDefault = "��˾����";

        public void Initialize()
        {
            AddContextMenu();
            MyOptionPage.AddTabDialog();
        }
        public void Terminate()
        {
            RemoveContextMenu();
            MyOptionPage.RemoveTabDialog();
        }

        ContextMenuExtension m_ContextMenu;


        private void MyMenuItem_OnClick(object Sender, EventArgs e)
        {
            using (DocumentLock docLock = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.LockDocument())
            {
               //����һ����ɫ��Բ
                Database db = HostApplicationServices.WorkingDatabase;
                using (Transaction trans = db.TransactionManager.StartTransaction())
                {
                    BlockTable bt = trans.GetObject(db.BlockTableId, OpenMode.ForRead) as BlockTable ;
                    BlockTableRecord btr = trans.GetObject(bt[BlockTableRecord.ModelSpace], OpenMode.ForWrite) as BlockTableRecord;
                    Circle cir = new Circle(new Point3d(10, 10, 0), Vector3d.ZAxis, 100);
                    cir.ColorIndex  = 1;
                    btr.AppendEntity (cir);
                    trans.AddNewlyCreatedDBObject (cir,true);
                    trans.Commit ();
                }
            }
        }

        private void AddContextMenu()
        {
            //ContextMenuExtension m_ContextMenu;//����Ϊ��ĳ�Ա����
            m_ContextMenu = new ContextMenuExtension();
            m_ContextMenu.Title = "�ҵ��Զ���˵�";
            Autodesk.AutoCAD.Windows.MenuItem mi;
            mi = new Autodesk.AutoCAD.Windows.MenuItem("����Բ");
            mi.Click += MyMenuItem_OnClick;
            m_ContextMenu.MenuItems.Add(mi);

            Autodesk.AutoCAD.ApplicationServices.Application.AddDefaultContextMenuExtension(m_ContextMenu);
            //��Ӷ��󼶱���Ҽ��˵�(˵�������������䲻ͬ��
            //Autodesk.AutoCAD.ApplicationServices.Application.AddObjectContextMenuExtension(Circle.GetClass(typeof(Circle)), m_ContextMenu);
        }

        private void RemoveContextMenu()
        {
            if (m_ContextMenu != null)
            {
                Autodesk.AutoCAD.ApplicationServices.Application.RemoveDefaultContextMenuExtension(m_ContextMenu);
                m_ContextMenu = null;
            }
        }


        ////
    }
}
