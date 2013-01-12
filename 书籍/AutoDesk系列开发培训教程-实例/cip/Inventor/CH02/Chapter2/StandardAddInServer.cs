using System;
using System.Runtime.InteropServices;
using Inventor;
using Microsoft.Win32;
using System.Drawing;
using System.Windows.Forms;
using Microsoft.VisualBasic.Compatibility.VB6;


namespace Chapter2
{
    /// <summary>
    /// This is the primary AddIn Server class that implements the ApplicationAddInServer interface
    /// that all Inventor AddIns are required to implement. The communication between Inventor and
    /// the AddIn is via the methods on this interface.
    /// </summary>
    [GuidAttribute("3e39ee93-3b83-4a5b-b991-8350fa1eb960")]
    public class StandardAddInServer : Inventor.ApplicationAddInServer
    {

        // Inventor application object.
        private Inventor.Application m_inventorApplication;
        String strAddInGUID;

        private Inventor.ButtonDefinition buttonDefinition;
        private Inventor.ButtonDefinition buttonDefinition1;
        private Inventor.ButtonDefinition buttonDefinition2;
        private Inventor.ButtonDefinition buttonDefinition3;

        private BrowserForm browserPane;
        private BrowserNode browserNode;

        public StandardAddInServer()
        {
        }

        private void ButtonDefinition_OnExecute(NameValueMap Context)
        {
            browserNode = new BrowserNode(m_inventorApplication);
            //����������ڵ�
            browserNode.QueryModelTree();
            //�������������ͽڵ�
            browserNode.AddNodes();
        }

        void buttonDefinition1_OnExecute(NameValueMap Context)
        {
            //�������������
 //           browserPane = new BrowserForm(m_inventorApplication);
 //           browserPane.Show();
            TestOverrideEnv();
       }

        void buttonDefinition2_OnExecute(NameValueMap Context)
        {
            ActiveteETAgain();
        }

        private void ButtonDefinition3_OnExecute(NameValueMap Context)
        {
            AddParallelEnv1();
            AddParallelEnv2();
            AddParallelEnv3();
            ActivateET();
        }
       
        public void AddParallelEnv1()
        {
            Inventor.Environments Envs;
            Envs = m_inventorApplication.UserInterfaceManager.Environments;

            Inventor.Environment Env;
            Env = Envs["DLxDrawingEnvironment"];

            Inventor.Environment newEnv;
            newEnv = Envs.Add("FEA", "FEA Env's internal name",
                strAddInGUID, Type.Missing, Type.Missing);

            newEnv.PanelBar.DefaultCommandBar = Env.PanelBar.DefaultCommandBar;

            foreach(CommandBar panel in Env.PanelBar.CommandBarList)
            {
                newEnv.PanelBar.CommandBarList.Add(panel);
            }

            foreach(CommandBar cmdBar in Env.ContextMenuList)
            {
                newEnv.ContextMenuList.Add(cmdBar);
            }

            newEnv.PanelBar.CommandBarList.InheritAll = true;
            newEnv.ContextMenuList.InheritAll = true;

            EnvironmentList parEnvs;
            parEnvs = m_inventorApplication.UserInterfaceManager.ParallelEnvironments;
            parEnvs.Add(newEnv);
        }

        public void AddParallelEnv2()
        {
           Inventor.Environments Envs;
           Envs  = m_inventorApplication.UserInterfaceManager.Environments;

           Inventor.Environment Env;
           Env = Envs["AMxAssemblyEnvironment"];

           Inventor.Environment newEnv;
           newEnv = Envs.Add("Rendering", "Rendering Env's internal name",
                   strAddInGUID, Type.Missing, Type.Missing);

           newEnv.PanelBar.DefaultCommandBar = Env.PanelBar.DefaultCommandBar;
           foreach(CommandBar panel in Env.PanelBar.CommandBarList)
           {
               newEnv.PanelBar.CommandBarList.Add(panel);
           }

           foreach(CommandBar cmdBar in Env.ContextMenuList)
           {
               newEnv.ContextMenuList.Add(cmdBar);
           }

           newEnv.PanelBar.CommandBarList.InheritAll = true;
           newEnv.ContextMenuList.InheritAll = true;

           EnvironmentList parEnvs;
           parEnvs = m_inventorApplication.UserInterfaceManager.ParallelEnvironments;

           parEnvs.Add(newEnv);
        }

        public void AddParallelEnv3()
        {
            Inventor.Environments Envs;
            Envs = m_inventorApplication.UserInterfaceManager.Environments;

            Inventor.Environment Env;
            Env = Envs["AMxWeldmentEnvironment"];

            Inventor.Environment newEnv;
            newEnv = Envs.Add("Aerofoil", "Aerofoil Env's internal name",
                   strAddInGUID, Type.Missing, Type.Missing);

            newEnv.PanelBar.DefaultCommandBar = Env.PanelBar.DefaultCommandBar;
            foreach (CommandBar panel in Env.PanelBar.CommandBarList)
            {
                newEnv.PanelBar.CommandBarList.Add(panel);
            }

            foreach (CommandBar cmdBar in Env.ContextMenuList)
            {
                newEnv.ContextMenuList.Add(cmdBar);
            }

            newEnv.PanelBar.CommandBarList.InheritAll = true;
            newEnv.ContextMenuList.InheritAll = true;

            EnvironmentList parEnvs;
            parEnvs = m_inventorApplication.UserInterfaceManager.ParallelEnvironments;

            parEnvs.Add(newEnv);
        }

        public void ActivateET()
        {
            Inventor.Environments Envs;
            Envs = m_inventorApplication.UserInterfaceManager.Environments;

            Inventor.Environment Env;
            Env = Envs["MBxSheetMetalEnvironment"];

            PartDocument parentDoc;
            parentDoc = (PartDocument)m_inventorApplication.ActiveDocument;

            Inventor.Environment newEnv;
            newEnv = Envs.Add("Aerofoil-ET", "ET Env's internal name",
               strAddInGUID, Type.Missing, Type.Missing);

            newEnv.PanelBar.DefaultCommandBar = Env.PanelBar.DefaultCommandBar;
            foreach(CommandBar panel in Env.PanelBar.CommandBarList)
            {
               newEnv.PanelBar.CommandBarList.Add(panel);
            }

            foreach(CommandBar cmdBar in Env.ContextMenuList)
            {
               newEnv.ContextMenuList.Add(cmdBar);
            }

            newEnv.PanelBar.CommandBarList.InheritAll = true;
            newEnv.ContextMenuList.InheritAll = true;

            parentDoc.EnvironmentManager.SetCurrentEnvironment(newEnv, "ET's Cookie");
        }

        public void ActiveteETAgain()
        {
            Inventor.Environments Envs;
            Envs = m_inventorApplication.UserInterfaceManager.Environments;

            PartDocument partDoc;
            partDoc = (PartDocument)m_inventorApplication.ActiveDocument;

            Inventor.Environment newEnv;
            newEnv = Envs["ET Env's internal name"];

            partDoc.EnvironmentManager.SetCurrentEnvironment(newEnv, "ET's Cookie");
        }

        public void TestOverrideEnv()
        {
            Inventor.Environments Envs;
            Envs = m_inventorApplication.UserInterfaceManager.Environments;

            Inventor.Environment Env;
            Env = Envs["DXxPresentationEnvironment"];

            PartDocument partDoc;
            partDoc = (PartDocument)m_inventorApplication.ActiveDocument;

            Inventor.Environment newEnv;
            newEnv = Envs.Add("OverrideTmp", "OverrideTmp Env's internal name",
               strAddInGUID, Type.Missing, Type.Missing);

            newEnv.DefaultMenuBar = Env.DefaultMenuBar;
            newEnv.DefaultToolBar = Env.DefaultToolBar;

            foreach (CommandBar panel in Env.PanelBar.CommandBarList)
            {
               newEnv.PanelBar.CommandBarList.Add(panel);
            }

            EnvironmentManager EnvMgr;
            EnvMgr = partDoc.EnvironmentManager;
            EnvMgr.OverrideEnvironment = newEnv;
        }
       
        #region ApplicationAddInServer Members

        public void Activate(Inventor.ApplicationAddInSite addInSiteObject, bool firstTime)
        {
            // This method is called by Inventor when it loads the addin.
            // The AddInSiteObject provides access to the Inventor Application object.
            // The FirstTime flag indicates if the addin is loaded for the first time.

            // Initialize AddIn members.
            m_inventorApplication = addInSiteObject.Application;

            // TODO: Add ApplicationAddInServer.Activate implementation.
            // e.g. event initialization, command creation etc.
            strAddInGUID = "{" + ((GuidAttribute)System.Attribute.GetCustomAttribute(typeof(StandardAddInServer), typeof(GuidAttribute))).Value + "}";

            //���õ��û����������������
            UserInterfaceManager userInterfaceManager;
            userInterfaceManager = m_inventorApplication.UserInterfaceManager;

            //����buttonDefinition����
            Icon Icon1 = new Icon(this.GetType(), "Icon1.ico");

            stdole.IPictureDisp standardIconIPictureDisp;
            standardIconIPictureDisp = (stdole.IPictureDisp)Support.IconToIPicture(Icon1);

            buttonDefinition = m_inventorApplication.CommandManager.ControlDefinitions.AddButtonDefinition(
                "Display Name",
                "invrSampleCommand",
                CommandTypesEnum.kShapeEditCmdType,
                strAddInGUID,
                "Description Text", 
                "Tooltip",
                standardIconIPictureDisp,
                standardIconIPictureDisp,
                ButtonDisplayEnum.kDisplayTextInLearningMode);

            //Ϊ��ť�������
            buttonDefinition.OnExecute += new ButtonDefinitionSink_OnExecuteEventHandler(ButtonDefinition_OnExecute);
            
            //����Դ�ļ��м���ͼ�겢������ť����
            buttonDefinition1 = m_inventorApplication.CommandManager.ControlDefinitions.AddButtonDefinition(
                "Button 1",
                "invrSampleCommand1",
                CommandTypesEnum.kQueryOnlyCmdType,
                strAddInGUID,
                "This is button 1.",
                "Button 1",
                standardIconIPictureDisp,
                standardIconIPictureDisp,
                ButtonDisplayEnum.kDisplayTextInLearningMode);
            buttonDefinition1.OnExecute += new ButtonDefinitionSink_OnExecuteEventHandler(buttonDefinition1_OnExecute);

            //�ٴ���������ť���壬һ����ͼ�꣬��һ��û��ͼ��
            Icon Icon2 = new Icon(this.GetType(), "Icon2.ico");
            standardIconIPictureDisp = (stdole.IPictureDisp)Support.IconToIPicture(Icon2);
            buttonDefinition2 = m_inventorApplication.CommandManager.ControlDefinitions.AddButtonDefinition(
                "Button 2",
                "invrSampleCommand2",
                CommandTypesEnum.kQueryOnlyCmdType,
                strAddInGUID,
                "This is button 2.",
                "Button 2",
                standardIconIPictureDisp,
                standardIconIPictureDisp,
                ButtonDisplayEnum.kDisplayTextInLearningMode);
            buttonDefinition2.OnExecute += new ButtonDefinitionSink_OnExecuteEventHandler(buttonDefinition2_OnExecute);

            buttonDefinition3 = m_inventorApplication.CommandManager.ControlDefinitions.AddButtonDefinition(
                "Button 3",
                "invrSampleCommand3",
                CommandTypesEnum.kQueryOnlyCmdType,
                strAddInGUID,
                "This is button 3.",
                "Button 3",
                System.Type.Missing,
                System.Type.Missing,
                ButtonDisplayEnum.kDisplayTextInLearningMode);
            buttonDefinition3.OnExecute += new ButtonDefinitionSink_OnExecuteEventHandler(ButtonDefinition3_OnExecute);

            if (firstTime)
            {
                //������������
                CommandBar commandBar;
                commandBar = userInterfaceManager.CommandBars.Add("Test Bar", "intTestBar",
                    CommandBarTypeEnum.kRegularCommandBar, strAddInGUID);

                //����ť�ؼ���ӵ���������
                commandBar.Controls.AddButton(buttonDefinition, 0);
                commandBar.Visible = true;
               
                //���ҵ�װ�乤����
                CommandBar asmCmdBar;
                asmCmdBar = userInterfaceManager.CommandBars["AMxAssemblyPanelCmdBar"];
 
                //������������ӿؼ�
                asmCmdBar.Controls.AddButton(buttonDefinition, 0);

                //����������
                Inventor.Environment partEnv;
                partEnv = userInterfaceManager.Environments["PMxPartEnvironment"];

                //���õ�PanelBar���������
                PanelBar panelBar;
                panelBar = partEnv.PanelBar;

                //����������ӵ����������������б���
                panelBar.CommandBarList.Add(commandBar);

                //�������˵���ʹ�õ�������
                CommandBar partMenuCB;
                partMenuCB = partEnv.DefaultMenuBar;

                //�����������͵�������
                CommandBar flyOutCmdBar;
                flyOutCmdBar = userInterfaceManager.CommandBars.Add("More Commands",
                    "FlyoutCmdBar", CommandBarTypeEnum.kPopUpCommandBar, strAddInGUID);

                //�ڵ��������������������ť
                flyOutCmdBar.Controls.AddButton(buttonDefinition2, 0);
                flyOutCmdBar.Controls.AddButton(buttonDefinition3, 0);

                //Ϊ�����˵������������͵�������
                CommandBar menuPopupCmdBar;
                menuPopupCmdBar = userInterfaceManager.CommandBars.Add("Test",
                    "MenuPopupCmdBar", CommandBarTypeEnum.kPopUpCommandBar, strAddInGUID);

                //�ڵ����˵����������
                menuPopupCmdBar.Controls.AddButton(buttonDefinition1, 0);

                //��������������ӵ������˵���
                menuPopupCmdBar.Controls.AddPopup(flyOutCmdBar, 0);

                //��á��������ؼ������к�
                int helpIndex;
                helpIndex = partMenuCB.Controls["AppHelpMenu"].index;

                //�������˵���ӵ�����˵��С��������ؼ�֮ǰ
                partMenuCB.Controls.AddPopup(menuPopupCmdBar, helpIndex);
            }
        }

        public void Deactivate()
        {
            // This method is called by Inventor when the AddIn is unloaded.
            // The AddIn will be unloaded either manually by the user or
            // when the Inventor session is terminated

            // TODO: Add ApplicationAddInServer.Deactivate implementation

            // Release objects.
            Marshal.ReleaseComObject(m_inventorApplication);
            m_inventorApplication = null;

            Marshal.ReleaseComObject(buttonDefinition);
            buttonDefinition = null;

            GC.WaitForPendingFinalizers();
            GC.Collect();
        }

        public void ExecuteCommand(int commandID)
        {
            // Note:this method is now obsolete, you should use the 
            // ControlDefinition functionality for implementing commands.
        }

        public object Automation
        {
            // This property is provided to allow the AddIn to expose an API 
            // of its own to other programs. Typically, this  would be done by
            // implementing the AddIn's API interface in a class and returning 
            // that class object through this property.

            get
            {
                // TODO: Add ApplicationAddInServer.Automation getter implementation
                return null;
            }
        }

        #endregion

        #region COM Registration functions

        /// <summary>
        /// Registers this class as an AddIn for Autodesk Inventor.
        /// This function is called when the assembly is registered for COM.
        /// </summary>
        [ComRegisterFunctionAttribute()]
        public static void Register(Type t)
        {
            RegistryKey clssRoot = Registry.ClassesRoot;
            RegistryKey clsid = null;
            RegistryKey subKey = null;

            try
            {
                clsid = clssRoot.CreateSubKey("CLSID\\" + AddInGuid(t));
                clsid.SetValue(null, "Chapter2");
                subKey = clsid.CreateSubKey("Implemented Categories\\{39AD2B5C-7A29-11D6-8E0A-0010B541CAA8}");
                subKey.Close();

                subKey = clsid.CreateSubKey("Settings");
                subKey.SetValue("AddInType", "Standard");
                subKey.SetValue("LoadOnStartUp", "1");

                //subKey.SetValue("SupportedSoftwareVersionLessThan", "");
                subKey.SetValue("SupportedSoftwareVersionGreaterThan", "11..");
                //subKey.SetValue("SupportedSoftwareVersionEqualTo", "");
                //subKey.SetValue("SupportedSoftwareVersionNotEqualTo", "");
                //subKey.SetValue("Hidden", "0");
                //subKey.SetValue("UserUnloadable", "1");
                subKey.SetValue("Version", 0);
                subKey.Close();

                subKey = clsid.CreateSubKey("Description");
                subKey.SetValue(null, "Chapter2");
            }
            catch
            {
                System.Diagnostics.Trace.Assert(false);
            }
            finally
            {
                if (subKey != null) subKey.Close();
                if (clsid != null) clsid.Close();
                if (clssRoot != null) clssRoot.Close();
            }

        }

        /// <summary>
        /// Unregisters this class as an AddIn for Autodesk Inventor.
        /// This function is called when the assembly is unregistered.
        /// </summary>
        [ComUnregisterFunctionAttribute()]
        public static void Unregister(Type t)
        {
            RegistryKey clssRoot = Registry.ClassesRoot;
            RegistryKey clsid = null;

            try
            {
                clssRoot = Microsoft.Win32.Registry.ClassesRoot;
                clsid = clssRoot.OpenSubKey("CLSID\\" + AddInGuid(t), true);
                clsid.SetValue(null, "");
                clsid.DeleteSubKeyTree("Implemented Categories\\{39AD2B5C-7A29-11D6-8E0A-0010B541CAA8}");
                clsid.DeleteSubKeyTree("Settings");
                clsid.DeleteSubKeyTree("Description");
            }
            catch { }
            finally
            {
                if (clsid != null) clsid.Close();
                if (clssRoot != null) clssRoot.Close();
            }
        }

        // This function uses reflection to get the value for the GuidAttribute attached to the class.
        private static String AddInGuid(Type t)
        {
            string guid = "";

            try
            {
                Object[] customAttributes = t.GetCustomAttributes(typeof(GuidAttribute), false);
                GuidAttribute guidAttribute = (GuidAttribute)customAttributes[0];
                guid = "{" + guidAttribute.Value.ToString() + "}";
            }
            catch
            {
            }

            return guid;

        }

        #endregion

    }
}
