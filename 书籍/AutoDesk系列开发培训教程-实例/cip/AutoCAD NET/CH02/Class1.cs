using System;
using System.Collections.Generic;
using System.Text;
using Autodesk.AutoCAD.EditorInput ;
using Autodesk.AutoCAD.Runtime ;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices ;

namespace CH02
{
    public class Class1
    {

        //--------------------------------------------------------------
        // ����:��ȡ�û�����
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //   
        //----------------------------------------------------------------
        [CommandMethod("GetData")]
        public void GetData()
        {
            //��ȡEditor����
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            //��ȡ��������
            PromptIntegerOptions intOp = new PromptIntegerOptions("���������εı�����");
            PromptIntegerResult intRes;
            intRes = ed.GetInteger(intOp);
            //�ж��û�����
            if (intRes.Status == PromptStatus.OK)
            {
                int nSides = intRes.Value;
                ed.WriteMessage("����εı���Ϊ��" + nSides);
            } if (intRes.Status == PromptStatus.Cancel)
            {
                ed.WriteMessage("�û�����ȡ��ESC��/n" );
            }

        }

        //--------------------------------------------------------------
        // ����:Ҫ���û������
        // ���ߣ� 
        // ���ڣ�2007-7-20
        // ˵����
        //   
        //----------------------------------------------------------------
			[CommandMethod("PickPoint")]
        static public void PickPoint() 
			{
                //��ȡEditor����
               Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
			    PromptPointOptions promptPtOp = new PromptPointOptions("ѡ��һ���㣺");
                //ָ���Ļ��㣬���ָ���˸õ㣬����ѡ���ʱ�����һ����Ƥ�ߡ�
                promptPtOp.BasePoint = new Autodesk.AutoCAD.Geometry.Point3d(0, 0, 0);
			    PromptPointResult resPt; 
			    resPt = ed.GetPoint(promptPtOp); 
			    if (resPt.Status == PromptStatus.OK) 
			    {

                    ed.WriteMessage("ѡ��ĵ�Ϊ��" + resPt.Value.ToString());
			    } 
            }


            //--------------------------------------------------------------
            // ����:��ȡѡ��
            // ���ߣ� 
            // ���ڣ�2007-7-20
            // ˵����
            //   
            //----------------------------------------------------------------
			[CommandMethod("SelectEnt")]
        static public void SelectEnt() 
			{
                //��ȡEditor����
               Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
               PromptSelectionOptions selectionOp = new PromptSelectionOptions();
               PromptSelectionResult ssRes = ed.GetSelection(selectionOp);
               if (ssRes.Status == PromptStatus.OK)
               {
                   SelectionSet SS = ssRes.Value;
                   int nCount = SS.Count;
                   ed.WriteMessage("ѡ����{0}��ʵ��"  , nCount);
               }		    
            }

            //--------------------------------------------------------------
            // ����:��ȡѡ�񼯣������ˣ�
            // ���ߣ� 
            // ���ڣ�2007-7-20
            // ˵����
            //   
            //----------------------------------------------------------------
        		[CommandMethod("SelectEnt2")]
        static public void SelectEnt2() 
			{
                //��ȡEditor����
               Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;

                // ����ѡ��ѡ��
               PromptSelectionOptions selectionOp = new PromptSelectionOptions();
                 //����ѡ�񼯹�������ֻѡ������
                TypedValue[] filList = new TypedValue[1];
                filList[0] = new TypedValue((int)DxfCode.Start, "INSERT");
               SelectionFilter filter = new SelectionFilter(filList);

               PromptSelectionResult ssRes = ed.GetSelection(selectionOp, filter);
               if (ssRes.Status == PromptStatus.OK)
               {
                   SelectionSet SS = ssRes.Value;
                   int nCount = SS.Count;
                   ed.WriteMessage("ѡ����{0}����"  , nCount);
               }		    

            }

     

        ///////////////////////
		}
	
		}
 

