﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:2.0.50727.832
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// 
// This source code was auto-generated by Microsoft.VSDesigner, Version 2.0.50727.832.
// 
#pragma warning disable 1591

namespace Client.localhost {
    using System.Diagnostics;
    using System.Web.Services;
    using System.ComponentModel;
    using System.Web.Services.Protocols;
    using System;
    using System.Xml.Serialization;
    
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Web.Services", "2.0.50727.42")]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Web.Services.WebServiceBindingAttribute(Name="MapGuideWebServiceSoap", Namespace="http://localhost/Mapguide/")]
    public partial class MapGuideWebService : System.Web.Services.Protocols.SoapHttpClientProtocol {
        
        private System.Threading.SendOrPostCallback GetParcelListOperationCompleted;
        
        private bool useDefaultCredentialsSetExplicitly;
        
        /// <remarks/>
        public MapGuideWebService() {
            this.Url = global::Client.Properties.Settings.Default.Client_localhost_MapGuideWebService;
            if ((this.IsLocalFileSystemWebService(this.Url) == true)) {
                this.UseDefaultCredentials = true;
                this.useDefaultCredentialsSetExplicitly = false;
            }
            else {
                this.useDefaultCredentialsSetExplicitly = true;
            }
        }
        
        public new string Url {
            get {
                return base.Url;
            }
            set {
                if ((((this.IsLocalFileSystemWebService(base.Url) == true) 
                            && (this.useDefaultCredentialsSetExplicitly == false)) 
                            && (this.IsLocalFileSystemWebService(value) == false))) {
                    base.UseDefaultCredentials = false;
                }
                base.Url = value;
            }
        }
        
        public new bool UseDefaultCredentials {
            get {
                return base.UseDefaultCredentials;
            }
            set {
                base.UseDefaultCredentials = value;
                this.useDefaultCredentialsSetExplicitly = true;
            }
        }
        
        /// <remarks/>
        public event GetParcelListCompletedEventHandler GetParcelListCompleted;
        
        /// <remarks/>
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("http://localhost/Mapguide/GetParcelList", RequestNamespace="http://localhost/Mapguide/", ResponseNamespace="http://localhost/Mapguide/", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Wrapped)]
        public ParcelProperty[] GetParcelList(string address, string parcelType, double bufferDistance) {
            object[] results = this.Invoke("GetParcelList", new object[] {
                        address,
                        parcelType,
                        bufferDistance});
            return ((ParcelProperty[])(results[0]));
        }
        
        /// <remarks/>
        public void GetParcelListAsync(string address, string parcelType, double bufferDistance) {
            this.GetParcelListAsync(address, parcelType, bufferDistance, null);
        }
        
        /// <remarks/>
        public void GetParcelListAsync(string address, string parcelType, double bufferDistance, object userState) {
            if ((this.GetParcelListOperationCompleted == null)) {
                this.GetParcelListOperationCompleted = new System.Threading.SendOrPostCallback(this.OnGetParcelListOperationCompleted);
            }
            this.InvokeAsync("GetParcelList", new object[] {
                        address,
                        parcelType,
                        bufferDistance}, this.GetParcelListOperationCompleted, userState);
        }
        
        private void OnGetParcelListOperationCompleted(object arg) {
            if ((this.GetParcelListCompleted != null)) {
                System.Web.Services.Protocols.InvokeCompletedEventArgs invokeArgs = ((System.Web.Services.Protocols.InvokeCompletedEventArgs)(arg));
                this.GetParcelListCompleted(this, new GetParcelListCompletedEventArgs(invokeArgs.Results, invokeArgs.Error, invokeArgs.Cancelled, invokeArgs.UserState));
            }
        }
        
        /// <remarks/>
        public new void CancelAsync(object userState) {
            base.CancelAsync(userState);
        }
        
        private bool IsLocalFileSystemWebService(string url) {
            if (((url == null) 
                        || (url == string.Empty))) {
                return false;
            }
            System.Uri wsUri = new System.Uri(url);
            if (((wsUri.Port >= 1024) 
                        && (string.Compare(wsUri.Host, "localHost", System.StringComparison.OrdinalIgnoreCase) == 0))) {
                return true;
            }
            return false;
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Xml", "2.0.50727.832")]
    [System.SerializableAttribute()]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    [System.Xml.Serialization.XmlTypeAttribute(Namespace="http://localhost/Mapguide/")]
    public partial class ParcelProperty {
        
        private string idField;
        
        private string acreageField;
        
        private string billingAddrField;
        
        private string description1Field;
        
        private string description2Field;
        
        private string description3Field;
        
        private string description4Field;
        
        private string lotDimensionField;
        
        private int lotSizeField;
        
        private string ownerField;
        
        private string zoningField;
        
        /// <remarks/>
        public string ID {
            get {
                return this.idField;
            }
            set {
                this.idField = value;
            }
        }
        
        /// <remarks/>
        public string Acreage {
            get {
                return this.acreageField;
            }
            set {
                this.acreageField = value;
            }
        }
        
        /// <remarks/>
        public string BillingAddr {
            get {
                return this.billingAddrField;
            }
            set {
                this.billingAddrField = value;
            }
        }
        
        /// <remarks/>
        public string Description1 {
            get {
                return this.description1Field;
            }
            set {
                this.description1Field = value;
            }
        }
        
        /// <remarks/>
        public string Description2 {
            get {
                return this.description2Field;
            }
            set {
                this.description2Field = value;
            }
        }
        
        /// <remarks/>
        public string Description3 {
            get {
                return this.description3Field;
            }
            set {
                this.description3Field = value;
            }
        }
        
        /// <remarks/>
        public string Description4 {
            get {
                return this.description4Field;
            }
            set {
                this.description4Field = value;
            }
        }
        
        /// <remarks/>
        public string LotDimension {
            get {
                return this.lotDimensionField;
            }
            set {
                this.lotDimensionField = value;
            }
        }
        
        /// <remarks/>
        public int LotSize {
            get {
                return this.lotSizeField;
            }
            set {
                this.lotSizeField = value;
            }
        }
        
        /// <remarks/>
        public string Owner {
            get {
                return this.ownerField;
            }
            set {
                this.ownerField = value;
            }
        }
        
        /// <remarks/>
        public string Zoning {
            get {
                return this.zoningField;
            }
            set {
                this.zoningField = value;
            }
        }
    }
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Web.Services", "2.0.50727.42")]
    public delegate void GetParcelListCompletedEventHandler(object sender, GetParcelListCompletedEventArgs e);
    
    /// <remarks/>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Web.Services", "2.0.50727.42")]
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.ComponentModel.DesignerCategoryAttribute("code")]
    public partial class GetParcelListCompletedEventArgs : System.ComponentModel.AsyncCompletedEventArgs {
        
        private object[] results;
        
        internal GetParcelListCompletedEventArgs(object[] results, System.Exception exception, bool cancelled, object userState) : 
                base(exception, cancelled, userState) {
            this.results = results;
        }
        
        /// <remarks/>
        public ParcelProperty[] Result {
            get {
                this.RaiseExceptionIfNecessary();
                return ((ParcelProperty[])(this.results[0]));
            }
        }
    }
}

#pragma warning restore 1591