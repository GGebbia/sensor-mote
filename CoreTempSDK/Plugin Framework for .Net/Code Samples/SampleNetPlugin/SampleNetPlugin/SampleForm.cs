using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using CoreTemp.Plugin;

namespace SampleNetPlugin
{
    public partial class SampleForm : Form, IPlugin
    {
        public SampleForm()
        {
            InitializeComponent();
        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            this.CoreTempPluginProxyReference.RemotePluginStop();
        }

        eStartStatus IPlugin.Start()
        {
            this.Show(this.CoreTempPluginProxyReference.ParentHandle);
            return eStartStatus.Success;
        }

        void IPlugin.Update(CoreTempSharedData Data)
        {
            this.txtData.Text = Data.ToString();
        }

        void IPlugin.Stop()
        {
            this.Hide();
        }

        eConfigureStatus IPlugin.Configure()
        {
            MessageBox.Show("This is the configuration of the Sample Plugin");
            return eConfigureStatus.Handled;
        }

        void IPlugin.Remove(string Path)
        {
            MessageBox.Show("Removing files from " + Path);
        }

        string IPlugin.Name
        {
            get { return "SamplePlugin"; }
        }

        string IPlugin.Description
        {
            get { return "Sample plugin for Core Temp in .Net"; }
        }

        string IPlugin.Version
        {
            get { return "1.0"; }
        }

        private string getFullError(Exception e, string tabs)
        {
            string message = tabs + e.Message + Environment.NewLine;

            if (e.InnerException != null)
            {
                message += getFullError(e.InnerException, tabs + "\t");
            }

            return message;
        }

        private CoreTempPluginProxy m_CoreTempPluginProxyReference;

        public CoreTempPluginProxy CoreTempPluginProxyReference
        {
            get
            {
                return m_CoreTempPluginProxyReference;
            }
            set
            {
                m_CoreTempPluginProxyReference = value;
            }
        }
    }
}
