using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.Windows;
[assembly: CommandClass(typeof(CH06.ModelessForm))]

namespace CH06
{
    public partial class ModelessForm : UserControl
    {
        public ModelessForm()
        {
            InitializeComponent();
            // ��Ӷ���ק�¼���Ӧ������¼�����
            lb_drop.MouseMove += new System.Windows.Forms.MouseEventHandler(lb_drop_MouseMove);
        }

        private void lb_drop_MouseMove(object sender, EventArgs e)
        {
            if (System.Windows.Forms.Control.MouseButtons == System.Windows.Forms.MouseButtons.Left)
            {
                // ��ʼ��ק����
                Autodesk.AutoCAD.ApplicationServices.Application.DoDragDrop(this, this, System.Windows.Forms.DragDropEffects.All, new MyDropTarget());
            }
        }
        private Autodesk.AutoCAD.Windows.PaletteSet palSet;

        [CommandMethod("AddPalette")]
        public void AddPalette()
        {
            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            try
            {
                if (palSet == null)
                {
                    // private Autodesk.AutoCAD.Windows.PaletteSet palSet;//�ⲿ����Ϊ���Ա����
                    palSet = new Autodesk.AutoCAD.Windows.PaletteSet("�ҵ���弯");
                    //palSet.Style = PaletteSetStyles.ShowTabForSingle;
                    palSet.Style = PaletteSetStyles.NameEditable;
                    palSet.Style = PaletteSetStyles.ShowPropertiesMenu;
                    palSet.Style = PaletteSetStyles.ShowAutoHideButton;
                    palSet.Style = PaletteSetStyles.ShowCloseButton;
                    palSet.Opacity = 90;
                    palSet.MinimumSize = new System.Drawing.Size(300, 300);
                    System.Windows.Forms.UserControl myPageCtrl = new ModelessForm();
                    //myPageCtrl.Dock = System.Windows.Forms.DockStyle.Fill;
                    palSet.Add("�ҵ�ҳ��", myPageCtrl);
                    palSet.Visible = true;
                }
            }
            catch
            {
                ed.WriteMessage("������弯����");
            }
        }


    }


   //////////////////////////////////////


    public class MyDropTarget : Autodesk.AutoCAD.Windows.DropTarget
    {
        override public void OnDrop(System.Windows.Forms.DragEventArgs e)
        {
            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            try
            {
                Point3d pt = ed.PointToWorld(new Point(e.X, e.Y));

                using (DocumentLock docLock = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.LockDocument())
                {
                    ModelessForm ctrl = (ModelessForm)e.Data.GetData(typeof(ModelessForm));
                    Utility.CreateEmployee(ctrl.tb_Name.Text, ctrl.tb_Division.Text, Convert.ToDouble(ctrl.tb_Salary.Text), pt);
                }
            }
            catch
            {
                ed.WriteMessage("ʧ��");
            }
        }
    }
    // class MyDropTarget

    public class Utility
    {
        public static ObjectId CreateEmployee(String name, String division, Double salary, Point3d pos)
        {
            Database db = HostApplicationServices.WorkingDatabase;
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                BlockTable bt = (BlockTable)trans.GetObject(db.BlockTableId, OpenMode.ForWrite);
                BlockTableRecord btr = (BlockTableRecord)trans.GetObject(bt[BlockTableRecord.ModelSpace], OpenMode.ForWrite);
                // ����һ��Բ
                Circle cir = new Circle();
                cir.Center = pos;
                cir.Radius = 100;
                cir.ColorIndex = 1;
                btr.AppendEntity(cir); 
                trans.AddNewlyCreatedDBObject(cir, true);

                //���������¼
                Xrecord xRec = new Xrecord();
                xRec.Data = new ResultBuffer(
                    new TypedValue((int)DxfCode.Text, name),
                    new TypedValue((int)DxfCode.Real, salary),
                    new TypedValue((int)DxfCode.Text, division));

                cir.CreateExtensionDictionary();
                DBDictionary brExtDict = (DBDictionary)trans.GetObject(cir.ExtensionDictionary, OpenMode.ForWrite, false);
                brExtDict.SetAt("EmpInfor", xRec); 
                trans.AddNewlyCreatedDBObject(xRec, true);

                trans.Commit();

                //
                return cir.ObjectId;
            }
        }

    }

    ////////////////////////////
}
