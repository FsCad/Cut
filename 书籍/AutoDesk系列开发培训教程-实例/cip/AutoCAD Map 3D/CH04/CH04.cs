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

// AnnotationCS.cs

using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.Runtime;
using System;

namespace CH04
{
	public sealed class AnnotationSampleApp : IExtensionApplication
	{
		public void Initialize()
		{
			Utility.ShowMsg("\nע��ʾ�������ʼ����");
			SampleCommand.CmdList();
		}
		public void Terminate()
		{
            Utility.ShowMsg("\nע��ʾ��������ֹ��");
		}
	}

	public sealed class SampleCommand
	{
		private AnnotationDlg m_form = null;
        //--------------------------------------------------------------------------
        //
        // ���ܣ���������
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
		[Autodesk.AutoCAD.Runtime.CommandMethodAttribute("RunAnnoForm")]
		public void RunAnnoForm()
		{
    	    if (null == m_form)
			{
				m_form = new AnnotationDlg();
			}
			Autodesk.AutoCAD.ApplicationServices.Application.ShowModalDialog(
				Autodesk.AutoCAD.ApplicationServices.Application.MainWindow, m_form);
		}

		/// <summary>
		/// Lists the Commands this sample provided.
		/// </summary>
		[Autodesk.AutoCAD.Runtime.CommandMethodAttribute("CmdList")]
		public static void CmdList()
		{
            Utility.ShowMsg("\n ע��ʾ������������� :\n");
			Utility.ShowMsg("** Cmd : RunAnnoForm\n");
		}
	}

	public sealed class Utility
	{
        //--------------------------------------------------------------------------
        //
        // ���ܣ������Ϣ
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
		public static void ShowMsg(string msg)
		{
			AcadEditor.WriteMessage(msg);
		}
        //--------------------------------------------------------------------------
        //
        // ���ܣ�����AutoCAD Editor����
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
		public static Autodesk.AutoCAD.EditorInput.Editor AcadEditor
		{
			get 
			{
				return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
			}
		}

        //--------------------------------------------------------------------------
        //
        // ���ܣ�����AutoCAD Map 3D �ڲ�����
        //
        //  ���ߣ� 
        //
        //  ���ڣ�200708
        //
        //   ��ʷ��
        //--------------------------------------------------------------------------
		public static bool SendCommand(string cmd)
		{
			try
			{
				Autodesk.AutoCAD.ApplicationServices.Document doc = 
					Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
				doc.SendStringToExecute(cmd, true, false, true);
				return true;
			}
			catch (System.Exception e)
			{
				ShowMsg(e.Message);
				return false;
			}
		}

		private Utility()
		{
		}
	}
}
